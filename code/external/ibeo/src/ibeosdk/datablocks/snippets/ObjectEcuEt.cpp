//======================================================================
/*! \file ObjectEcuEt.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 5, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ObjectEcuEt.hpp>
#include <ibeosdk/Math.hpp>
#include <ibeosdk/ObjectBasic.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

ObjectEcuEt::ObjectEcuEt()
  : m_objectId(0),
    m_flags(0),
    m_objectAge(0),
    m_timestamp(),
    m_objectPredAge(0),
    m_classification(ObjectClass_Unclassified),
    m_classCertainty(0),
    m_classAge(0),
    m_reserved0(),
    m_reserved1(),
    m_objBoxCenter(),
    m_objBoxCenterSigma(),
    m_objBoxSize(),
    m_reserved2(),
    m_objBoxOrientation(.0f),
    m_objBoxOrientationSigma(.0f),
    m_relVelocity(),
    m_relVelocitySigma(),
    m_absVelocity(),
    m_absVelocitySigma(),
    m_reserved4(0),
    m_reserved5(.0f),
    m_reserved6(.0f),
    m_reserved7(0),
    m_idxOfClosestPoint(0),
    m_refPointLocation(RPL_Unknown),
    m_refPointCoords(),
    m_refPointCoordsSigma(),
    m_refPointPosCorrCoeffs(.0f),
    m_reserved8(.0f),
    m_reserved9(.0f),
    m_objPriority(0),
    m_objExtMeasurement(.0f),
    m_contourPoints()
{}

//======================================================================

std::streamsize ObjectEcuEt::getSerializedSize() const
{
	return 2 * std::streamsize(sizeof(uint16_t)) // objectId, flags
		+ std::streamsize(sizeof(uint32_t))      // objectAge
		+ std::streamsize(sizeof(NTPTime))       // timestamp
		+ std::streamsize(sizeof(uint16_t))      // objectPredAge
		+ 2*std::streamsize(sizeof(uint8_t))     // classification, classCertainty
		+ std::streamsize(sizeof(uint32_t))      // classAge
		+ 6* Point2dFloat::getSerializedSize_static() // reserved0,1, objBoxCenter, Sigma, Size, revered2
		+ 2*std::streamsize(sizeof(float))       // objCourseAngle, objCourseAngleSigma
		+ 4* Point2dFloat::getSerializedSize_static() // relVelocity, Sigma, absVelocity, Sigma
		+ std::streamsize(sizeof(uint64_t))      // reserved4
		+ 2*std::streamsize(sizeof(float))       // reserved5, 6
		+ std::streamsize(sizeof(uint16_t))      // reserved7
		+ 2*std::streamsize(sizeof(uint8_t))     // nbOfPts, idxOfClosestPoint
		+ std::streamsize(sizeof(uint16_t))      // refPointLocation
		+ 2* Point2dFloat::getSerializedSize_static() // refPointCoords, Sigma
		+ 3*std::streamsize(sizeof(float))       // refPointPosCorrCoeffs, reserved8, 9
		+ std::streamsize(sizeof(uint16_t))      // objPriority
		+ std::streamsize(sizeof(float))         // objextMeasurement
		+ std::streamsize(m_contourPoints.size()) * Point2dFloat::getSerializedSize_static();
}
//======================================================================

bool ObjectEcuEt::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readBE(is, m_objectId);
	readBE(is, m_flags);
	readBE(is, m_objectAge);
	readBE(is, m_timestamp);
	readBE(is, m_objectPredAge);
	readBE(is, m_classification);

	readBE(is, m_classCertainty);
	readBE(is, m_classAge);
	m_reserved0.deserialize(is);
	m_reserved1.deserialize(is);
	m_objBoxCenter.deserialize(is);
	m_objBoxCenterSigma.deserialize(is);
	m_objBoxSize.deserialize(is);
	m_reserved2.deserialize(is);
	readBE(is, m_objBoxOrientation);
	readBE(is, m_objBoxOrientationSigma);
	m_relVelocity.deserialize(is);
	m_relVelocitySigma.deserialize(is);
	m_absVelocity.deserialize(is);
	m_absVelocitySigma.deserialize(is);
	readBE(is, m_reserved4);
	readBE(is, m_reserved5);
	readBE(is, m_reserved6);
	readBE(is, m_reserved7);

	{
		uint8_t rd8;
		readBE(is, rd8);
		m_contourPoints.resize(rd8);
	}

	readBE(is, m_idxOfClosestPoint);
	readBE<16>(is, m_refPointLocation);

	m_refPointCoords.deserialize(is);
	m_refPointCoordsSigma.deserialize(is);
	readBE(is, m_refPointPosCorrCoeffs);
	readBE(is, m_reserved8);
	readBE(is, m_reserved9);
	readBE(is, m_objPriority);
	readBE(is, m_objExtMeasurement);
	std::vector<Point2dFloat>::iterator cpIter = m_contourPoints.begin();
	for (; cpIter != m_contourPoints.end(); ++cpIter) {
		cpIter->deserialize(is);
	}

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ObjectEcuEt::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, m_objectId);
	writeBE(os, m_flags);
	writeBE(os, m_objectAge);
	writeBE(os, m_timestamp);
	writeBE(os, m_objectPredAge);
	writeBE(os, m_classification);

	writeBE(os, m_classCertainty);
	writeBE(os, m_classAge);
	m_reserved0.serialize(os);
	m_reserved1.serialize(os);
	m_objBoxCenter.serialize(os);
	m_objBoxCenterSigma.serialize(os);
	m_objBoxSize.serialize(os);
	m_reserved2.serialize(os);
	writeBE(os, m_objBoxOrientation);
	writeBE(os, m_objBoxOrientationSigma);
	m_relVelocity.serialize(os);
	m_relVelocitySigma.serialize(os);
	m_absVelocity.serialize(os);
	m_absVelocitySigma.serialize(os);
	writeBE(os, m_reserved4);
	writeBE(os, m_reserved5);
	writeBE(os, m_reserved6);
	writeBE(os, m_reserved7);

	writeBE(os, uint8_t(m_contourPoints.size()));
	writeBE(os, m_idxOfClosestPoint);
	writeBE<16>(os, m_refPointLocation);

	m_refPointCoords.serialize(os);
	m_refPointCoordsSigma.serialize(os);
	writeBE(os, m_refPointPosCorrCoeffs);
	writeBE(os, m_reserved8);
	writeBE(os, m_reserved9);
	writeBE(os, m_objPriority);
	writeBE(os, m_objExtMeasurement);
	std::vector<Point2dFloat>::const_iterator cpIter = m_contourPoints.begin();
	for (; cpIter != m_contourPoints.end(); ++cpIter) {
		cpIter->serialize(os);
	}

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ObjectEcuEt::operator==( const ObjectEcuEt& other ) const
{
	if (m_objectId != other.getObjectId())
		return false;

	if (m_flags != other.getFlags())
		return false;

	if (m_objectAge != other.getObjectAge())
		return false;

	if (m_timestamp != other.getTimestamp())
		return false;

	if (m_objectPredAge != other.getObjectPredAge())
		return false;

	if (m_classification != other.getClassification())
		return false;

	if (m_classCertainty != other.getClassCertainty())
		return false;

	if (m_classAge != other.getClassAge())
		return false;

	if (m_reserved0 != other.getReserved0())
		return false;

	if (m_reserved1 != other.getReserved1())
		return false;

	if(m_objBoxCenter != other.getObjBoxCenter())
		return false;

	if(m_objBoxCenterSigma != other.getObjBoxCenterSigma())
		return false;

	if(m_objBoxSize != other.getObjBoxSize())
		return false;

	if(m_reserved2 != other.getReserved2())
		return false;

	if(!fuzzyFloatEqualT<7>(m_objBoxOrientation, other.getObjBoxOrientation()))
		return false;

	if(!fuzzyFloatEqualT<7>(m_objBoxOrientationSigma, other.getObjBoxOrientationSigma()))
		return false;

	if (m_relVelocity != other.getRelVelocity())
		return false;

	if (m_relVelocitySigma != other.getRelVelocitySigma())
		return false;

	if (m_absVelocity != other.getAbsVelocity())
		return false;

	if (m_absVelocitySigma != other.getAbsVelocitySigma())
		return false;

	if(m_reserved4 != other.getReserved4())
		return false;

	if(!fuzzyFloatEqualT<7>(m_reserved5, other.getReserved5()))
		return false;

	if(!fuzzyFloatEqualT<7>(m_reserved6, other.getReserved6()))
		return false;

	if(m_reserved7 != other.getReserved7())
		return false;

	if( m_idxOfClosestPoint != other.getIdxOfClosestPoint())
		return false;

	if (m_refPointLocation != other.getRefPointLocation())
		return false;

	if (m_refPointCoords != other.getRefPointCoords())
		return false;

	if (m_refPointCoordsSigma != other.getRefPointCoordsSigma())
		return false;

	if ( !fuzzyFloatEqualT<7>(m_refPointPosCorrCoeffs, other.getRefPointPosCorrCoeffs()))
		return false;

	if(!fuzzyFloatEqualT<7>(m_reserved8, other.getReserved8()))
		return false;

	if(!fuzzyFloatEqualT<7>(m_reserved9, other.getReserved9()))
		return false;

	if(m_objPriority != other.getObjPriority())
		return false;

	if(!fuzzyFloatEqualT<7>(m_objExtMeasurement, other.getObjExtMeasurement()))
		return false;

	if (m_contourPoints != other.getContourPoints())
		return false;

	return true;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
