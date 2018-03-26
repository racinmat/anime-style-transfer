//======================================================================
/*! \file ObjectScala.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 17, 2014
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/ObjectScala.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

//static
const UINT16 ObjectScala::contourIsInvalid = 0xFFFF;

//======================================================================

ObjectScala::ObjectScala()
: m_id(0),
  m_age(0),
  m_predictionAge(0),
  m_relativeTimestamp(0),
  m_reserved0(0),
  m_referencePointLocation(RPL_Unknown),
  m_refPoint(),
  m_refPointSigma(),
  m_referencePointPositionCorrCoeff(0),
  m_objectPriority(0),
  m_objectExistenceMeasure(0),
  m_closestPoint(),
  m_boundingBoxCenter(),
  m_boundingBoxWidth(0),
  m_boundingBoxLength(0),
  m_objectBoxCenter(),
  m_objectBoxSizeX(0),
  m_objectBoxSizeY(0),
  m_objectBoxOrientation(0),
  m_objectBoxSizeSigma(),
  m_objectBoxOrientationSigma(0),
  m_absVelocity(),
  m_absVelSigma(),
  m_relVelocity(),
  m_relVelSigma(),
  m_class(ObjectClass_Unclassified),
  m_flags(0),
  m_classAge(0),
  m_classConfidence(0),
  m_numContourPoints(0),
  m_numContourPointsIsValid(true),
  m_contourPoints()
{
	m_contourPoints.reserve(16);
}

//======================================================================

ObjectScala::ObjectScala(const ObjectScala& other)
  : m_id(other.m_id),
    m_age(other.m_age),
	m_predictionAge(other.m_predictionAge),
	m_relativeTimestamp(other.m_relativeTimestamp),
	m_reserved0(other.m_reserved0),
	m_referencePointLocation(other.m_referencePointLocation),
	m_refPoint(other.m_refPoint),
	m_refPointSigma(other.m_refPointSigma),
	m_referencePointPositionCorrCoeff(other.m_referencePointPositionCorrCoeff),
	m_objectPriority(other.m_objectPriority),
	m_objectExistenceMeasure(other.m_objectExistenceMeasure),
	m_closestPoint(other.m_closestPoint),
	m_boundingBoxCenter(other.m_boundingBoxCenter),
	m_boundingBoxWidth(other.m_boundingBoxWidth),
	m_boundingBoxLength(other.m_boundingBoxLength),
	m_objectBoxCenter(other.m_objectBoxCenter),
	m_objectBoxSizeX(other.m_objectBoxSizeX),
	m_objectBoxSizeY(other.m_objectBoxSizeY),
	m_objectBoxOrientation(other.m_objectBoxOrientation),
	m_objectBoxSizeSigma(other.m_objectBoxSizeSigma),
	m_objectBoxOrientationSigma(other.m_objectBoxOrientationSigma),
	m_absVelocity(other.m_absVelocity),
	m_absVelSigma(other.m_absVelSigma),
	m_relVelocity(other.m_relVelocity),
	m_relVelSigma(other.m_relVelSigma),
	m_class(other.m_class),
	m_flags(other.m_flags),
	m_classAge(other.m_classAge),
	m_classConfidence(other.m_classConfidence),
	m_numContourPoints(other.m_numContourPoints),
	m_numContourPointsIsValid(other.m_numContourPointsIsValid),
	m_contourPoints(other.m_contourPoints)
{}

//======================================================================

ObjectScala& ObjectScala::operator=(const ObjectScala& other)
{
	if (this == &other)
		return *this;

	this->m_id  = other.m_id;
	this->m_age  = other.m_age;
	this->m_predictionAge  = other.m_predictionAge;
	this->m_relativeTimestamp  = other.m_relativeTimestamp;
	this->m_reserved0 = other.m_reserved0;
	this->m_referencePointLocation = other.m_referencePointLocation;
	this->m_refPoint  = other.m_refPoint;
	this->m_refPointSigma  = other.m_refPointSigma;
	this->m_referencePointPositionCorrCoeff = other.m_referencePointPositionCorrCoeff;
	this->m_objectPriority = other.m_objectPriority;
	this->m_objectExistenceMeasure = other.m_objectExistenceMeasure;
	this->m_closestPoint  = other.m_closestPoint;
	this->m_boundingBoxCenter  = other.m_boundingBoxCenter;
	this->m_boundingBoxWidth  = other.m_boundingBoxWidth;
	this->m_boundingBoxLength  = other.m_boundingBoxLength;
	this->m_objectBoxCenter  = other.m_objectBoxCenter;
	this->m_objectBoxSizeX  = other.m_objectBoxSizeX;
	this->m_objectBoxSizeY  = other.m_objectBoxSizeY;
	this->m_objectBoxOrientation  = other.m_objectBoxOrientation;
	this->m_objectBoxSizeSigma = other.m_objectBoxSizeSigma;
	this->m_objectBoxOrientationSigma = other.m_objectBoxOrientationSigma;
	this->m_absVelocity  = other.m_absVelocity;
	this->m_absVelSigma  = other.m_absVelSigma;
	this->m_relVelocity  = other.m_relVelocity;
	this->m_relVelSigma  = other.m_relVelSigma;
	this->m_class  = other.m_class;
	this->m_flags = other.m_flags;
	this->m_classAge  = other.m_classAge;
	this->m_classConfidence  = other.m_classConfidence;
	this->m_numContourPoints  = other.m_numContourPoints;
	this->m_numContourPointsIsValid = other.m_numContourPointsIsValid;
	this->m_contourPoints = other.m_contourPoints;

	return *this;
}

//======================================================================

ObjectScala::~ObjectScala()
{
// TODO Auto-generated destructor stub
}

//======================================================================

std::streamsize ObjectScala::getSerializedSize() const
{
	return 4* std::streamsize(sizeof(UINT16))       // id .. relativeTimestamp
			+ 2 * std::streamsize(sizeof(UINT8))    // reserved .. refPtLoc
			+ Point2d::getSerializedSize_static() + PointSigma2d::getSerializedSize()
			+ 3 * std::streamsize(sizeof(UINT16))   // refPtPosCorrCoeff .. objExMeas
			+ 2 * Point2d::getSerializedSize_static() // closesPt .. bbCenter
			+ 2 * std::streamsize(sizeof(UINT16))   // bbBox size
			+ Point2d::getSerializedSize_static() // obj box center
			+ 2 * std::streamsize(sizeof(UINT16)) + 1 * std::streamsize(sizeof(INT16))
			+ 1 * PointSigma2d::getSerializedSize() // objBoxSigma
			+ 1 * std::streamsize(sizeof(UINT16))   // objBoxOriSigma
			+ 2 * Point2d::getSerializedSize_static() + 2 * PointSigma2d::getSerializedSize()
			+ 2 * std::streamsize(sizeof(UINT8))    // class, flags
			+ 3 * std::streamsize(sizeof(UINT16))   // classAge .. m_numContourPoints
			+ (m_numContourPointsIsValid ? m_numContourPoints : 1)* Point2d::getSerializedSize_static();
}

//======================================================================

bool ObjectScala::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeLE(os, m_id);
	ibeosdk::writeLE(os, m_age);
	ibeosdk::writeLE(os, m_predictionAge);

	ibeosdk::writeLE(os, m_relativeTimestamp);
	ibeosdk::writeLE(os, m_reserved0);
	writeBE<8>(os, m_referencePointLocation);

	ibeosdk::writeLE(os, m_refPoint);
	ibeosdk::writeLE(os, m_refPointSigma);
	ibeosdk::writeLE(os, m_referencePointPositionCorrCoeff);

	ibeosdk::writeLE(os, m_objectPriority);
	ibeosdk::writeLE(os, m_objectExistenceMeasure);
	ibeosdk::writeLE(os, m_closestPoint);

	ibeosdk::writeLE(os, m_boundingBoxCenter);
	ibeosdk::writeLE(os, m_boundingBoxWidth);
	ibeosdk::writeLE(os, m_boundingBoxLength);

	ibeosdk::writeLE(os, m_objectBoxCenter);
	ibeosdk::writeLE(os, m_objectBoxSizeX);
	ibeosdk::writeLE(os, m_objectBoxSizeY);
	ibeosdk::writeLE(os, m_objectBoxOrientation);
	ibeosdk::writeLE(os, m_objectBoxSizeSigma);
	ibeosdk::writeLE(os, m_objectBoxOrientationSigma);

	ibeosdk::writeLE(os, m_absVelocity);
	ibeosdk::writeLE(os, m_absVelSigma);

	ibeosdk::writeLE(os, m_relVelocity);
	ibeosdk::writeLE(os, m_relVelSigma);

	ibeosdk::writeLE(os, uint8_t(m_class));
	ibeosdk::writeLE(os, m_flags);
	ibeosdk::writeLE(os, m_classAge);
	ibeosdk::writeLE(os, m_classConfidence);

	if (m_numContourPointsIsValid)
		ibeosdk::writeLE(os, m_numContourPoints);
	else
		ibeosdk::writeLE(os, ObjectScala::contourIsInvalid);

	std::vector<Point2d>::const_iterator cpIter = m_contourPoints.begin();
	for (; cpIter != m_contourPoints.end(); ++cpIter) {
		ibeosdk::writeLE(os, *cpIter);
	}

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ObjectScala::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readLE(is, m_id);
	ibeosdk::readLE(is, m_age);
	ibeosdk::readLE(is, m_predictionAge);

	ibeosdk::readLE(is, m_relativeTimestamp);
	ibeosdk::readLE(is, m_reserved0);

	readBE<8>(is, m_referencePointLocation);
	ibeosdk::readLE(is, m_refPoint);
	ibeosdk::readLE(is, m_refPointSigma);
	ibeosdk::readLE(is, m_referencePointPositionCorrCoeff);

	ibeosdk::readLE(is, m_objectPriority);
	ibeosdk::readLE(is, m_objectExistenceMeasure);
	ibeosdk::readLE(is, m_closestPoint);

	ibeosdk::readLE(is, m_boundingBoxCenter);
	ibeosdk::readLE(is, m_boundingBoxWidth);
	ibeosdk::readLE(is, m_boundingBoxLength);

	ibeosdk::readLE(is, m_objectBoxCenter);
	ibeosdk::readLE(is, m_objectBoxSizeX);
	ibeosdk::readLE(is, m_objectBoxSizeY);
	ibeosdk::readLE(is, m_objectBoxOrientation);
	ibeosdk::readLE(is, m_objectBoxSizeSigma);
	ibeosdk::readLE(is, m_objectBoxOrientationSigma);

	ibeosdk::readLE(is, m_absVelocity);
	ibeosdk::readLE(is, m_absVelSigma);

	ibeosdk::readLE(is, m_relVelocity);
	ibeosdk::readLE(is, m_relVelSigma);

	UINT8 c=ObjectClass_Unclassified;
	ibeosdk::readLE(is, c);
	switch (c) {
	case 1: m_class = ObjectClass_UnknownSmall; break;
	case 2: m_class = ObjectClass_UnknownBig; break;
	case 3: m_class = ObjectClass_Pedestrian; break;
	case 4: m_class = ObjectClass_Bike; break;
	case 5: m_class = ObjectClass_Car; break;
	case 6: m_class = ObjectClass_Truck; break;
	default: m_class = ObjectClass_Unclassified; break;
	}

	ibeosdk::readLE(is, m_flags);
	ibeosdk::readLE(is, m_classAge);
	ibeosdk::readLE(is, m_classConfidence);

	ibeosdk::readLE(is, m_numContourPoints);
	m_numContourPointsIsValid = (this->m_numContourPoints != ObjectScala::contourIsInvalid);
	if (!m_numContourPointsIsValid) {
		m_numContourPoints = 1;
	}

	m_contourPoints.resize(m_numContourPoints);
	std::vector<Point2d>::iterator cpIter = m_contourPoints.begin();
	for (; cpIter != m_contourPoints.end(); ++cpIter) {
		ibeosdk::readLE(is, *cpIter);
	}

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

////======================================================================
//
//std::ostream& operator<<(std::ostream& os, const ObjectScala& obj)
//{
//	os << "LUX object - " << "id: " << obj.getObjectId() << " "
//	   << "age: " << obj.getObjectAge() << " "
//	   << "predictionAge: " << obj.getPredictionAge() << " "
//	   << "relativeTimestamp: " << obj.getRelativeTimestamp() << " "
//
//	   <<
//	   << "refPoint: " << obj.getReferencePoint() << " "
//	   << "refPointSigma: " << obj.getReferencePointSigma() << " "
//	   << "closestPoint: " << obj.getClosestPoint() << " "
//	   << "boundingBoxCenter: " << obj.getBoundingBoxCenter() << " "
//	   << "boundingBoxWidth: " << obj.getBoundingBoxWidth() << " "
//	   << "boundingBoxLength: " << obj.getBoundingBoxLength() << " "
//	   << "objectBoxCenter: " << obj.getObjectBoxCenter() << " "
//	   << "objectBoxLength: " << obj.getObjectBoxSizeX() << " "
//	   << "objectBoxWidth: " << obj.getObjectBoxSizeY() << " "
//	   << "objectBoxOrientation: " << obj.getObjectBoxOrientation() << " "
//	   << "absVelocity: " << obj.getAbsoluteVelocity() << " "
//	   << "absVelSigmaX: " << obj.getAbsoluteVelocitySigmaX() << " "
//	   << "absVelSigmaY: " << obj.getAbsoluteVelocitySigmaY() << " "
//	   << "relVelocity: " << obj.getRelativeVelocity() << " "
//	   << "class: " << obj.getClassification() << " "
//	   << "classAge: " << obj.getClassificationAge() << " "
//	   << "classCertainty: " << obj.getClassificationCertainty() << " "
//	   << "numContourPointsIsValid: " << (obj.isNumContourPointsValid() ? "yes" : "no") << " "
//	   << "numContourPoints: " << obj.getNumberOfContourPoints()
//	   << std::endl;
//	return os;
//}

//======================================================================

}// namespace ibeosdk

//======================================================================
