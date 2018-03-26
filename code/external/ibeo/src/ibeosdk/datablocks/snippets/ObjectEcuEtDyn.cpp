//======================================================================
/*! \file ObjectEcuEt.cpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Mar 14, 2014
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ObjectEcuEtDyn.hpp>
#include <ibeosdk/Math.hpp>
#include <ibeosdk/ObjectBasic.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

Point2dFloat ObjectEcuEtDyn::getObjectBoxPosition(RefPointBoxLocation curRefPtLoc,
                                                  const Point2dFloat refPt,
                                                  const float courseAngle,
                                                  const Point2dFloat objBoxSize,
                                                  RefPointBoxLocation targetRefPtLoc)
{
	static const int vect[10][2] = {{0,0},{1,1},{1,-1},{-1,-1},{-1,1},{1,0},{0,-1},{-1,0},{0,1},{0,0}};

	const float sizeXh = 0.5f * objBoxSize.getX();
	const float sizeYh = 0.5f * objBoxSize.getY();
	const float cca = std::cos(courseAngle);
	const float sca = std::sin(courseAngle);
	const Point2dFloat halfLength(cca * sizeXh, sca * sizeXh);
	const Point2dFloat halfWidth(-sca * sizeYh, cca * sizeYh);

	// targetRefPtLoc und curRefPtLoc muessen im Intervall RPL_COG..RPL_OBJECT_CENTER liegen
	targetRefPtLoc =
		(targetRefPtLoc <= RPL_ObjectCenter) ? ((targetRefPtLoc < RPL_CenterOfGravity) ? RPL_CenterOfGravity
		                                                                               : targetRefPtLoc)
	                                         : RPL_ObjectCenter;
	curRefPtLoc =
		(curRefPtLoc <= RPL_ObjectCenter) ? ((curRefPtLoc < RPL_CenterOfGravity) ? RPL_CenterOfGravity
		                                                                         : curRefPtLoc)
	                                      : RPL_ObjectCenter;

	const float dl = float(vect[targetRefPtLoc][0]-vect[curRefPtLoc][0]);
	const float db = float(vect[targetRefPtLoc][1]-vect[curRefPtLoc][1]);

	return (refPt + dl*halfLength + db*halfWidth);
}

//======================================================================

const MeasurementKey ObjectEcuEtDyn::mkey_OxtsTargetNumber = MeasurementKey(0x0190);
const MeasurementKey ObjectEcuEtDyn::mkey_VisibilityLaser  = MeasurementKey(0x0191);
const MeasurementKey ObjectEcuEtDyn::mkey_OcclusionLaser   = MeasurementKey(0x0192);
const MeasurementKey ObjectEcuEtDyn::mkey_VisibilityDut    = MeasurementKey(0x0193);
const MeasurementKey ObjectEcuEtDyn::mkey_OcclusionDut     = MeasurementKey(0x0194);

//======================================================================

ObjectEcuEtDyn::ObjectEcuEtDyn()
  : m_objectId(0),
    m_flags(0),
    m_objectAge(0),
    m_timestamp(0),
    m_hiddenStatusAge(0),
    m_classification(),
    m_classificationQuality(0),
    m_classificationAge(0),
    m_objectBoxSize(),
    m_objBoxSizeSigma(),
    m_courseAngle(0),
    m_courseAngleSigma(0),
    m_relativeVelocity(),
    m_relativeVelocitySigma(),
    m_absoluteVelocity(),
    m_absoluteVelocitySigma(),
    m_objectHeight(0),
    m_objectHeightSigma(0),
    m_motionReferencePoint(),
    m_motionReferencePointSigma(),
    m_longitudianlAcceleration(0),
    m_longitudianlAccelerationSigma(0),
    m_yawRate(0),
    m_yawRateSigma(0),
    m_closestContourPointIndex(0),
    m_referencePointLocation(ibeosdk::RPL_Unknown),
    m_referencePointCoord(),
    m_referencePointCoordSigma(),
    m_referencePointPositionCorrCoeff(0),
    m_centerOfGravity(),
    m_objectPriority(0),
    m_objectExistenceMeas(0),
    m_objectBoxHeightOffset(0),
    m_objectBoxHeightOffsetSigma(0),
    m_reserved3(0),
    m_reserved4(0),
    m_contourPoints(),
    m_measurementList()
{}

//======================================================================

std::streamsize ObjectEcuEtDyn::getSerializedSize() const
{
	const std::streamsize result =
		std::streamsize(sizeof(m_objectId))
		+ std::streamsize(sizeof(m_flags))
		+ std::streamsize(sizeof(m_objectAge))
		+ std::streamsize(sizeof(UINT32) + sizeof(UINT32)) // timestamp
		+ std::streamsize(sizeof(UINT16))
		+ std::streamsize(sizeof(UINT8))
		+ std::streamsize(sizeof(UINT8))
		+ std::streamsize(sizeof(UINT32))
		+ m_objectBoxSize.getSerializedSize()
		+ m_objBoxSizeSigma.getSerializedSize()
		+ std::streamsize(sizeof(float))
		+ std::streamsize(sizeof(float))
		+ m_relativeVelocity.getSerializedSize()
		+ m_relativeVelocitySigma.getSerializedSize()
		+ m_absoluteVelocity.getSerializedSize()
		+ m_absoluteVelocitySigma.getSerializedSize()
		+ std::streamsize(sizeof(float))
		+ std::streamsize(sizeof(float))
		+ m_motionReferencePoint.getSerializedSize()
		+ m_motionReferencePointSigma.getSerializedSize()
		+ std::streamsize(sizeof(float))
		+ std::streamsize(sizeof(float))
		+ std::streamsize(sizeof(float))
		+ std::streamsize(sizeof(float))
		+ std::streamsize(sizeof(UINT8))
		+ std::streamsize(sizeof(UINT8))
		+ std::streamsize(sizeof(UINT16))
		+ m_referencePointCoord.getSerializedSize()
		+ m_referencePointCoordSigma.getSerializedSize()
		+ std::streamsize(sizeof(float))
		+ m_centerOfGravity.getSerializedSize()
		+ std::streamsize(sizeof(UINT16))
		+ std::streamsize(sizeof(float))
		+ std::streamsize(sizeof(INT8))
		+ std::streamsize(sizeof(UINT8))
		+ std::streamsize(sizeof(UINT8))
		+ std::streamsize(sizeof(UINT8))
		+ std::streamsize(m_contourPoints.size()) * Point2dFloat::getSerializedSize_static()
		+ m_measurementList.getSerializedSize();

	return result;
}
//======================================================================

bool ObjectEcuEtDyn::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readBE(is, m_objectId);
	readBE(is, m_flags);
	readBE(is, m_objectAge);
	readBE(is, m_timestamp);
	readBE(is, m_hiddenStatusAge);

	readBE(is, m_classification);
	readBE(is, m_classificationQuality);
	readBE(is, m_classificationAge);

	m_objectBoxSize.deserialize(is);
	m_objBoxSizeSigma.deserialize(is);
	readBE(is, m_courseAngle);
	readBE(is, m_courseAngleSigma);

	m_relativeVelocity.deserialize(is);
	m_relativeVelocitySigma.deserialize(is);
	m_absoluteVelocity.deserialize(is);
	m_absoluteVelocitySigma.deserialize(is);

	readBE(is, m_objectHeight);
	readBE(is, m_objectHeightSigma);

	m_motionReferencePoint.deserialize(is);
	m_motionReferencePointSigma.deserialize(is);

	readBE(is, m_longitudianlAcceleration);
	readBE(is, m_longitudianlAccelerationSigma);

	readBE(is, m_yawRate);
	readBE(is, m_yawRateSigma);

	{
		uint8_t rd8;
		readBE(is, rd8);
		m_contourPoints.resize(rd8);
	}

	readBE(is, m_closestContourPointIndex);

	readBE<16>(is, m_referencePointLocation);
	m_referencePointCoord.deserialize(is);
	m_referencePointCoordSigma.deserialize(is);
	readBE(is, m_referencePointPositionCorrCoeff);

	m_centerOfGravity.deserialize(is);
	readBE(is, m_objectPriority);
	readBE(is, m_objectExistenceMeas);

	readBE(is, m_objectBoxHeightOffset);
	readBE(is, m_objectBoxHeightOffsetSigma);
	readBE(is, m_reserved3);
	readBE(is, m_reserved4);

	// container has already been resized to the right size
	std::vector<Point2dFloat>::iterator cpIter = m_contourPoints.begin();
	for (; cpIter != m_contourPoints.end(); ++cpIter) {
		cpIter->deserialize(is);
	}

	m_measurementList.deserialize(is);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ObjectEcuEtDyn::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, m_objectId);
	writeBE(os, m_flags);
	writeBE(os, m_objectAge);
	writeBE(os, m_timestamp);
	writeBE(os, m_hiddenStatusAge);

	writeBE(os, m_classification);
	writeBE(os, m_classificationQuality);
	writeBE(os, m_classificationAge);

	m_objectBoxSize.serialize(os);
	m_objBoxSizeSigma.serialize(os);
	writeBE(os, m_courseAngle);
	writeBE(os, m_courseAngleSigma);

	m_relativeVelocity.serialize(os);
	m_relativeVelocitySigma.serialize(os);
	m_absoluteVelocity.serialize(os);
	m_absoluteVelocitySigma.serialize(os);

	writeBE(os, m_objectHeight);
	writeBE(os, m_objectHeightSigma);

	m_motionReferencePoint.serialize(os);
	m_motionReferencePointSigma.serialize(os);

	writeBE(os, m_longitudianlAcceleration);
	writeBE(os, m_longitudianlAccelerationSigma);

	writeBE(os, m_yawRate);
	writeBE(os, m_yawRateSigma);

	writeBE(os, UINT8(m_contourPoints.size()));
	writeBE(os, m_closestContourPointIndex);

	writeBE<16>(os, m_referencePointLocation);
	m_referencePointCoord.serialize(os);
	m_referencePointCoordSigma.serialize(os);
	writeBE(os, m_referencePointPositionCorrCoeff);

	m_centerOfGravity.serialize(os);
	writeBE(os, m_objectPriority);
	writeBE(os, m_objectExistenceMeas);

	writeBE(os, m_objectBoxHeightOffset);
	writeBE(os, m_objectBoxHeightOffsetSigma);
	writeBE(os, m_reserved3);
	writeBE(os, m_reserved4);

	std::vector<Point2dFloat>::const_iterator cpIter = m_contourPoints.begin();
	for (; cpIter != m_contourPoints.end(); ++cpIter) {
		cpIter->serialize(os);
	}

	m_measurementList.serialize(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

Point2dFloat ObjectEcuEtDyn::getObjectBoxPosition(const RefPointBoxLocation targetRefPtLoc) const
{
	return getObjectBoxPosition(this->getReferencePointLocation(),
	                            this->getReferencePointCoord(),
	                            this->getCourseAngle(),
	                            this->getObjectBoxSize(),
	                            targetRefPtLoc);
}

//======================================================================

Point2dFloat ObjectEcuEtDyn::getObjectBoxCenter() const
{
	return getObjectBoxPosition(RPL_ObjectCenter);
}

//======================================================================

bool ObjectEcuEtDyn::operator==( const ObjectEcuEtDyn& other ) const
{
	if (m_objectId != other.getObjectId())
		return false;

	if (m_flags != other.getFlags())
		return false;

	if (m_objectAge != other.getObjectAge())
		return false;

	if (m_timestamp != other.getTimestamp())
		return false;

	if (m_hiddenStatusAge != other.getHiddenStatusAge())
		return false;

	if (m_classification != other.getClassification())
		return false;

	if (m_classificationQuality != other.getClassificationQuality())
		return false;

	if (m_classificationAge != other.getClassificationAge())
		return false;

	if (m_objectBoxSize != other.getObjectBoxSize())
		return false;

	if (m_objBoxSizeSigma != other.getObjectBoxSizeSigma())
		return false;

	if ( !fuzzyFloatEqualT<7>(m_courseAngle, other.getCourseAngle()))
		return false;

	if ( !fuzzyFloatEqualT<7>(m_courseAngleSigma, other.getCourseAngleSigma()))
		return false;

	if (m_relativeVelocity != other.getRelativeVelocity())
		return false;

	if (m_relativeVelocitySigma != other.getRelativeVelocitySigma())
		return false;

	if (m_absoluteVelocity != other.getAbsoluteVelocity())
		return false;

	if (m_absoluteVelocitySigma != other.getAbsoluteVelocitySigma())
		return false;

	if ( !fuzzyFloatEqualT<7>(m_objectHeight, other.getObjectHeight()))
		return false;

	if ( !fuzzyFloatEqualT<7>(m_objectHeightSigma, other.getObjectHeightSigma()))
		return false;

	if (m_motionReferencePoint != other.getMotionReferencePoint())
		return false;

	if (m_motionReferencePointSigma != other.getMotionReferencePointSigma())
		return false;

	if ( !fuzzyFloatEqualT<7>(m_longitudianlAcceleration, other.getLongitudinalAcceleration()))
		return false;

	if ( !fuzzyFloatEqualT<7>(m_longitudianlAccelerationSigma, other.getLongitudinalAccelerationSigma()))
		return false;

	if ( !fuzzyFloatEqualT<7>(m_yawRate, other.getYawRate()))
		return false;

	if (m_closestContourPointIndex != other.getClosestContourPointIndex())
		return false;

	if (m_referencePointLocation != other.getReferencePointLocation())
		return false;

	if (m_referencePointCoord != other.getReferencePointCoord())
		return false;

	if (m_referencePointCoordSigma != other.getReferencePointCoordSigma())
		return false;

	if ( !fuzzyFloatEqualT<7>(m_referencePointPositionCorrCoeff, other.getReferencePointPositionCorrCoeff()))
		return false;

	if (getObjectBoxCenter() != other.getObjectBoxCenter())
		return false;

	if (m_centerOfGravity != other.getCenterOfGravity())
		return false;

	if (m_objectPriority != other.getObjectPriority())
		return false;

	if ( !fuzzyFloatEqualT<7>(m_objectExistenceMeas, other.getObjectExistenceMeas()))
		return false;

	if (m_objectBoxHeightOffset != other.getObjectBoxHeightOffset())
		return false;

	if (m_objectBoxHeightOffsetSigma != other.getObjectBoxHeightOffsetSigma())
		return false;

	if (m_reserved3 != other.getReserved3())
		return false;

	if (m_reserved4 != other.getReserved4())
		return false;
	
	for (size_t idx = 0; idx < m_contourPoints.size(); ++idx) {
		if (m_contourPoints[idx] != other.getContourPoints()[idx])
			return false;
	}

	if (m_measurementList != other.getMeasurementList())
		return false;
	
	return true;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
