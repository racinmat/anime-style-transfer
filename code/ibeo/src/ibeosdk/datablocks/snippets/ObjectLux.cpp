//======================================================================
/*! \file ObjectLux.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ObjectLux.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

//static
const UINT16 ObjectLux::contourIsInvalid = 0xFFFF;

//======================================================================

ObjectLux::ObjectLux()
  : m_id(0),
    m_age(0),
    m_predictionAge(0),
    m_relativeTimestamp(0),
    m_refPoint(),
    m_refPointSigma(),
    m_closestPoint(),
    m_boundingBoxCenter(),
    m_boundingBoxWidth(0),
    m_boundingBoxLength(0),
    m_objectBoxCenter(),
    m_objectBoxSizeX(0),
    m_objectBoxSizeY(0),
    m_objectBoxOrientation(0),
    m_absVelocity(),
    m_absVelSigmaX(0),
    m_absVelSigmaY(0),
    m_relVelocity(),
    m_class(luxObjectClass::LuxObjectClass_Unclassified),
    m_classAge(0),
    m_classCertainty(0),
    m_numContourPoints(0),
    m_numContourPointsIsValid(true),
    m_contourPoints()
{
	m_contourPoints.reserve(16);
}

//======================================================================

ObjectLux::ObjectLux(const ObjectLux& other)
  : m_id(other.m_id),
    m_age(other.m_age),
    m_predictionAge(other.m_predictionAge),
    m_relativeTimestamp(other.m_relativeTimestamp),
    m_refPoint(other.m_refPoint),
    m_refPointSigma(other.m_refPointSigma),
    m_closestPoint(other.m_closestPoint),
    m_boundingBoxCenter(other.m_boundingBoxCenter),
    m_boundingBoxWidth(other.m_boundingBoxWidth),
    m_boundingBoxLength(other.m_boundingBoxLength),
    m_objectBoxCenter(other.m_objectBoxCenter),
    m_objectBoxSizeX(other.m_objectBoxSizeX),
    m_objectBoxSizeY(other.m_objectBoxSizeY),
    m_objectBoxOrientation(other.m_objectBoxOrientation),
    m_absVelocity(other.m_absVelocity),
    m_absVelSigmaX(other.m_absVelSigmaX),
    m_absVelSigmaY(other.m_absVelSigmaY),
    m_relVelocity(other.m_relVelocity),
    m_class(other.m_class),
    m_classAge(other.m_classAge),
    m_classCertainty(other.m_classCertainty),
    m_numContourPoints(other.m_numContourPoints),
    m_numContourPointsIsValid(other.m_numContourPointsIsValid),
    m_contourPoints(other.m_contourPoints)
{}

//======================================================================

ObjectLux& ObjectLux::operator= (const ObjectLux& other)
{
	if (this == &other)
		return *this;

	this->m_id  = other.m_id;
	this->m_age  = other.m_age;
	this->m_predictionAge  = other.m_predictionAge;
	this->m_relativeTimestamp  = other.m_relativeTimestamp;
	this->m_refPoint  = other.m_refPoint;
	this->m_refPointSigma  = other.m_refPointSigma;
	this->m_closestPoint  = other.m_closestPoint;
	this->m_boundingBoxCenter  = other.m_boundingBoxCenter;
	this->m_boundingBoxWidth  = other.m_boundingBoxWidth;
	this->m_boundingBoxLength  = other.m_boundingBoxLength;
	this->m_objectBoxCenter  = other.m_objectBoxCenter;
	this->m_objectBoxSizeX  = other.m_objectBoxSizeX;
	this->m_objectBoxSizeY  = other.m_objectBoxSizeY;
	this->m_objectBoxOrientation  = other.m_objectBoxOrientation;
	this->m_absVelocity  = other.m_absVelocity;
	this->m_absVelSigmaX  = other.m_absVelSigmaX;
	this->m_absVelSigmaY  = other.m_absVelSigmaY;
	this->m_relVelocity  = other.m_relVelocity;
	this->m_class  = other.m_class;
	this->m_classAge  = other.m_classAge;
	this->m_classCertainty  = other.m_classCertainty;
	this->m_numContourPointsIsValid = other.m_numContourPointsIsValid;
	this->m_numContourPoints  = other.m_numContourPoints;
	this->m_contourPoints = other.m_contourPoints;

	return *this;
}

//======================================================================

ObjectLux::~ObjectLux() {}

//======================================================================

std::streamsize ObjectLux::getSerializedSize() const
{
	return 4* std::streamsize(sizeof(UINT16))
			+ 4 * Point2d::getSerializedSize_static()
			+ 2 * std::streamsize(sizeof(UINT16))
			+ 1 * Point2d::getSerializedSize_static()
			+ 2 * std::streamsize(sizeof(UINT16))
			+ 1 * std::streamsize(sizeof(INT16))
			+ 1 * Point2d::getSerializedSize_static()
			+ 2 * std::streamsize(sizeof(UINT16))
			+ 1 * Point2d::getSerializedSize_static()
			+ 4 * std::streamsize(sizeof(UINT16))
			+ (m_numContourPointsIsValid ? m_numContourPoints : 1)* Point2d::getSerializedSize_static();
}

//======================================================================

bool ObjectLux::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeLE(os, m_id);
	ibeosdk::writeLE(os, m_age);
	ibeosdk::writeLE(os, m_predictionAge);
	ibeosdk::writeLE(os, m_relativeTimestamp);
	ibeosdk::writeLE(os, m_refPoint);
	ibeosdk::writeLE(os, m_refPointSigma);
	ibeosdk::writeLE(os, m_closestPoint);
	ibeosdk::writeLE(os, m_boundingBoxCenter);

	ibeosdk::writeLE(os, m_boundingBoxWidth);
	ibeosdk::writeLE(os, m_boundingBoxLength);

	ibeosdk::writeLE(os, m_objectBoxCenter);
	ibeosdk::writeLE(os, m_objectBoxSizeX);
	ibeosdk::writeLE(os, m_objectBoxSizeY);
	ibeosdk::writeLE(os, m_objectBoxOrientation);

	ibeosdk::writeLE(os, m_absVelocity);
	ibeosdk::writeLE(os, m_absVelSigmaX);
	ibeosdk::writeLE(os, m_absVelSigmaY);

	ibeosdk::writeLE(os, m_relVelocity);

	const UINT16 c=m_class;
	ibeosdk::writeLE(os, c);

	ibeosdk::writeLE(os, m_classAge);
	ibeosdk::writeLE(os, m_classCertainty);

	if (m_numContourPointsIsValid)
		ibeosdk::writeLE(os, m_numContourPoints);
	else
		ibeosdk::writeLE(os, ObjectLux::contourIsInvalid);

	std::vector<Point2d>::const_iterator cpIter = m_contourPoints.begin();
	for (; cpIter != m_contourPoints.end(); ++cpIter) {
		ibeosdk::writeLE(os, *cpIter);
	}

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ObjectLux::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readLE(is, m_id);
	ibeosdk::readLE(is, m_age);
	ibeosdk::readLE(is, m_predictionAge);
	ibeosdk::readLE(is, m_relativeTimestamp);

	ibeosdk::readLE(is, m_refPoint);
	ibeosdk::readLE(is, m_refPointSigma);

	ibeosdk::readLE(is, m_closestPoint);

	ibeosdk::readLE(is, m_boundingBoxCenter);
	ibeosdk::readLE(is, m_boundingBoxWidth);
	ibeosdk::readLE(is, m_boundingBoxLength);

	ibeosdk::readLE(is, m_objectBoxCenter);
	ibeosdk::readLE(is, m_objectBoxSizeX);
	ibeosdk::readLE(is, m_objectBoxSizeY);
	ibeosdk::readLE(is, m_objectBoxOrientation);

	ibeosdk::readLE(is, m_absVelocity);
	ibeosdk::readLE(is, m_absVelSigmaX);
	ibeosdk::readLE(is, m_absVelSigmaY);

	ibeosdk::readLE(is, m_relVelocity);

	UINT16 c=ObjectClass_Unclassified;
	ibeosdk::readLE(is, c);
	switch (c) {
	case  1: m_class = luxObjectClass::LuxObjectClass_UnknownSmall; break;
	case  2: m_class = luxObjectClass::LuxObjectClass_UnknownBig; break;
	case  3: m_class = luxObjectClass::LuxObjectClass_Pedestrian; break;
	case  4: m_class = luxObjectClass::LuxObjectClass_Bike; break;
	case  5: m_class = luxObjectClass::LuxObjectClass_Car; break;
	case  6: m_class = luxObjectClass::LuxObjectClass_Truck; break;
	case 12: m_class = luxObjectClass::LuxObjectClass_Bicycle; break;
	default: m_class = luxObjectClass::LuxObjectClass_Unclassified; break;
	}

	ibeosdk::readLE(is, m_classAge);
	ibeosdk::readLE(is, m_classCertainty);

	ibeosdk::readLE(is, m_numContourPoints);
	m_numContourPointsIsValid = (this->m_numContourPoints != ObjectLux::contourIsInvalid);
	if (!m_numContourPointsIsValid) {
		m_numContourPoints = 1;
	}

	if (this->m_numContourPoints <= ObjectLux::maxContourPoints) {
		m_contourPoints.resize(m_numContourPoints);

		std::vector<Point2d>::iterator cpIter = m_contourPoints.begin();
		for (; cpIter != m_contourPoints.end(); ++cpIter) {
			ibeosdk::readLE(is, *cpIter);
		}
	}
	else {
		is.clear(std::ios::failbit);
	}

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const ObjectLux& luxObj)
{
	os << "LUX object - " << "id: " << luxObj.getObjectId() << " "
	   << "age: " << luxObj.getObjectAge() << " "
	   << "predictionAge: " << luxObj.getPredictionAge() << " "
	   << "relativeTimestamp: " << luxObj.getRelativeTimestamp() << " "
	   << "refPoint: " << luxObj.getReferencePoint() << " "
	   << "refPointSigma: " << luxObj.getReferencePointSigma() << " "
	   << "closestPoint: " << luxObj.getClosestPoint() << " "
	   << "boundingBoxCenter: " << luxObj.getBoundingBoxCenter() << " "
	   << "boundingBoxWidth: " << luxObj.getBoundingBoxWidth() << " "
	   << "boundingBoxLength: " << luxObj.getBoundingBoxLength() << " "
	   << "objectBoxCenter: " << luxObj.getObjectBoxCenter() << " "
	   << "objectBoxLength: " << luxObj.getObjectBoxSizeX() << " "
	   << "objectBoxWidth: " << luxObj.getObjectBoxSizeY() << " "
	   << "objectBoxOrientation: " << luxObj.getObjectBoxOrientation() << " "
	   << "absVelocity: " << luxObj.getAbsoluteVelocity() << " "
	   << "absVelSigmaX: " << luxObj.getAbsoluteVelocitySigmaX() << " "
	   << "absVelSigmaY: " << luxObj.getAbsoluteVelocitySigmaY() << " "
	   << "relVelocity: " << luxObj.getRelativeVelocity() << " "
	   << "class: " << luxObj.getClassification() << " "
	   << "classAge: " << luxObj.getClassificationAge() << " "
	   << "classCertainty: " << luxObj.getClassificationCertainty() << " "
	   << "numContourPointsIsValid: " << (luxObj.isNumContourPointsValid() ? "yes" : "no") << " "
	   << "numContourPoints: " << luxObj.getNumberOfContourPoints()
	   << std::endl;
	return os;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
