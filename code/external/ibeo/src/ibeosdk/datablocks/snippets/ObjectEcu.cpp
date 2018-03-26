//======================================================================
/*! \file ObjectEcu.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ObjectEcu.hpp>
#include <ibeosdk/io.hpp>
#include <ibeosdk/Math.hpp>

#include <cstring>

//======================================================================

namespace ibeosdk {

//======================================================================

ObjectEcu::ObjectEcu()
  : m_id(0),
    m_reserved(0),
    m_age(0),
    m_timestamp(0),
    m_hiddenStatusAge(0),
    m_class(),
    m_classCertainty(0),
    m_classAge(0),
    m_boundingBoxCenter(),
    m_boundingBoxSize(),
    m_objectBoxCenter(),
    m_objectBoxSigma(),
    m_objectBoxSize(),
    m_reserved2(0),
    m_objectBoxOrientation(0.),
    m_objectBoxOrientationSigma(0),
    m_relVelocity(),
    m_relVelocitySigma(),
    m_absVelocity(),
    m_absVelocitySigma(),
    // m_reserved4
    m_numContourPoints(0),
    m_indexOfClosedPoint(0),
    m_contourPoints(m_numContourPoints),
    m_orientation(0.f)
{
	m_contourPoints.reserve(16);
	std::memset(m_reserved3, 0, nbOfBytesInReserved3);
}

//======================================================================

ObjectEcu::ObjectEcu(const ObjectEcu& other)
  : m_id(other.m_id),
    m_reserved(other.m_reserved),
    m_age(other.m_age),
    m_timestamp(other.m_timestamp),
    m_hiddenStatusAge(other.m_hiddenStatusAge),
    m_class(other.m_class),
    m_classCertainty(other.m_classCertainty),
    m_classAge(other.m_classAge),
    m_boundingBoxCenter(other.m_boundingBoxCenter),
    m_boundingBoxSize(other.m_boundingBoxSize),
    m_objectBoxCenter(other.m_objectBoxCenter),
    m_objectBoxSigma(other.m_objectBoxSigma),
    m_objectBoxSize(other.m_objectBoxSize),
    m_reserved2(other.m_reserved2),
    m_objectBoxOrientation(other.m_objectBoxOrientation),
    m_objectBoxOrientationSigma(other.m_objectBoxOrientationSigma),
    m_relVelocity(other.m_relVelocity),
    m_relVelocitySigma(other.m_relVelocitySigma),
    m_absVelocity(other.m_absVelocity),
    m_absVelocitySigma(other.m_absVelocitySigma),
    // m_reserved3
    m_numContourPoints(other.m_numContourPoints),
    m_indexOfClosedPoint(other.m_indexOfClosedPoint),
    m_contourPoints(other.m_contourPoints),
    m_orientation(other.m_orientation)
{
	std::memcpy(m_reserved3, other.m_reserved3, nbOfBytesInReserved3);
}

//======================================================================

std::streamsize ObjectEcu::getSerializedSize() const
{
	return 132 + 8*m_numContourPoints;
}

//======================================================================

bool ObjectEcu::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_id);
	ibeosdk::readBE(is, m_reserved);
	ibeosdk::readBE(is, m_age);
	ibeosdk::readBE(is, m_timestamp);
	ibeosdk::readBE(is, m_hiddenStatusAge);

	UINT8 c;
	ibeosdk::readBE(is, c);
	switch (c) {
	case 1: m_class = ObjectClass_UnknownSmall; break;
	case 2: m_class = ObjectClass_UnknownBig; break;
	case 3: m_class = ObjectClass_Pedestrian; break;
	case 4: m_class = ObjectClass_Bike; break;
	case 5: m_class = ObjectClass_Car; break;
	case 6: m_class = ObjectClass_Truck; break;
	default: m_class = ObjectClass_Unclassified; break;
	} // switch c

	ibeosdk::readBE(is, m_classCertainty);
	ibeosdk::readBE(is, m_classAge);

	m_boundingBoxCenter.deserialize(is);
	m_boundingBoxSize.deserialize(is);

	m_objectBoxCenter.deserialize(is);
	m_objectBoxSigma.deserialize(is);
	m_objectBoxSize.deserialize(is);

	ibeosdk::readBE(is, m_reserved2);

	ibeosdk::readBE(is, m_objectBoxOrientation);

	ibeosdk::readBE(is, m_objectBoxOrientationSigma);

	m_relVelocity.deserialize(is);
	m_relVelocitySigma.deserialize(is);


	m_absVelocity.deserialize(is);
	m_absVelocitySigma.deserialize(is);

	for (int i = 0; i < nbOfBytesInReserved3; ++i)
		ibeosdk::readBE(is, m_reserved3[i]); // 18 bytes reserved

	ibeosdk::readBE(is, m_numContourPoints);
	ibeosdk::readBE(is, m_indexOfClosedPoint);

	if (m_contourPoints.size() != m_numContourPoints)
		m_contourPoints.resize(m_numContourPoints);

	std::vector<Point2dFloat>::iterator cpIter = m_contourPoints.begin();
	for (; cpIter != m_contourPoints.end(); ++cpIter) {
		cpIter->deserialize(is);
	}

	// derived data
	m_orientation = float(atan2(m_absVelocity.getY(), m_absVelocity.getX()));

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ObjectEcu::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_id);
	ibeosdk::writeBE(os, m_reserved);// 2 bytes reserved
	ibeosdk::writeBE(os, m_age);
	ibeosdk::writeBE(os, m_timestamp);
	ibeosdk::writeBE(os, m_hiddenStatusAge);

	const UINT8 c = m_class;
	ibeosdk::writeBE(os, c);

	ibeosdk::writeBE(os, m_classCertainty);
	ibeosdk::writeBE(os, m_classAge);

	m_boundingBoxCenter.serialize(os);
	m_boundingBoxSize.serialize(os);

	m_objectBoxCenter.serialize(os);
	m_objectBoxSigma.serialize(os);
	m_objectBoxSize.serialize(os);

	ibeosdk::writeBE(os, m_reserved2);// 8 bytes reserved

	ibeosdk::writeBE(os, m_objectBoxOrientation);

	ibeosdk::writeBE(os, m_objectBoxOrientationSigma);// 4 bytes reserved

	m_relVelocity.serialize(os);
	m_relVelocitySigma.serialize(os);


	m_absVelocity.serialize(os);
	m_absVelocitySigma.serialize(os);

	for (int i = 0; i < nbOfBytesInReserved3; ++i)
		ibeosdk::writeBE(os, m_reserved3[i]); // 18 bytes reserved

	ibeosdk::writeBE(os, m_numContourPoints);
	ibeosdk::writeBE(os, m_indexOfClosedPoint);


	std::vector<Point2dFloat>::const_iterator cpIter = m_contourPoints.begin();
	for (; cpIter != m_contourPoints.end(); ++cpIter) {
		cpIter->serialize(os);
	}

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const ObjectEcu& o)
{
	os.width(3);
//	os.precision(10);
	os << o.getObjectId() << ": ";
	os << "Age: " << o.getObjectAge() << ", ";
	os << "TS: " << o.getTimestamp() << ", ";
	os << "HS: " << o.getHiddenStatusAge() << ", ";
	os << "CL: " << o.getClassification() << "  ";
	os << "CC: " << int(o.getClassificationCertainty()) << ", ";
	os << "CA: " << o.getClassificationAge() << ", ";
	os << "BBC: " << o.getBoundingBoxCenter() << ", ";
	os << "BBS: " << o.getBoundingBoxSize() << ", ";
	os << "OBC: " << o.getObjectBoxCenter() << ", ";
	os << "OBCs: " << o.getObjectBoxSigma() << ", ";
	os << "OBS: " << o.getObjectBoxSize() << ", ";
	os << "Yaw: " << o.getObjectBoxOrientation() << ", ";
	os << "RV: " << o.getRelativeVelocity() << ", ";
	os << "RVs: " << o.getRelativeVelocitySigma() << ", ";
	os << "AV: " << o.getAbsoluteVelocity() << ", ";
	os << "AVs: " << o.getAbsoluteVelocitySigma() << ", ";
	os << "#CP:" << int(o.getNumberOfContourPoints()) <<  ", ";
	os << "iCP:" << int(o.getIndexOfClosestPoint()) <<  ", ";
	os << "ori:" << o.getOrientation() <<  ", ";

	return os;
}

//======================================================================

bool ObjectEcu::operator==( const ObjectEcu& other ) const
{
	if (m_id != other.getObjectId())
		return false;

	if (m_reserved != other.getReserved())
		return false;

	if (m_age != other.getObjectAge())
		return false;

	if (m_timestamp != other.getTimestamp())
		return false;

	if (m_hiddenStatusAge != other.getHiddenStatusAge())
		return false;

	if (m_class != other.getClassification())
		return false;

	if (m_classCertainty != other.getClassificationCertainty())
		return false;

	if (m_classAge != other.getClassificationAge())
		return false;

	if (m_boundingBoxCenter != other.getBoundingBoxCenter())
		return false;

	if (m_boundingBoxSize != other.getBoundingBoxSize())
		return false;

	if (m_objectBoxCenter != other.getObjectBoxCenter())
		return false;

	if (m_objectBoxSigma != other.getObjectBoxSigma())
		return false;

	if (m_objectBoxSize != other.getObjectBoxSize())
		return false;

	if (m_reserved2 != other.getReserved2())
		return false;

	if ( !(fuzzyCompareT<7>(m_objectBoxOrientation, other.getObjectBoxOrientation())))
		return false;

	if ( !(fuzzyCompareT<7>(m_objectBoxOrientationSigma, other.getObjectBoxOrientationSigma())))
		return false;

	if (m_relVelocity != other.getRelativeVelocity())
		return false;

	if (m_relVelocitySigma != other.getRelativeVelocitySigma())
		return false;

	if (m_absVelocity != other.getAbsoluteVelocity())
		return false;

	if (m_absVelocitySigma != other.getAbsoluteVelocitySigma())
		return false;

	if (getNumberOfContourPoints() != other.getNumberOfContourPoints())
		return false;

	if (m_indexOfClosedPoint != other.getIndexOfClosestPoint())
		return false;

	for (size_t idx = 0; idx < m_contourPoints.size(); ++idx) {
		if (m_contourPoints[idx] != other.getContourPoints()[idx])
			return false;
	}
	
	return true;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
