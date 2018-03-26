//======================================================================
/*! \file ObjectEcuEtDyn.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Mar 14, 2014
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_OBJECTECUETDYN_HPP_SEEN
#define IBEOSDK_OBJECTECUETDYN_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/datablocks/snippets/Point2dFloat.hpp>
#include <ibeosdk/Time.hpp>
#include <ibeosdk/ObjectBasic.hpp>
#include <ibeosdk/datablocks/snippets/MeasurementList.hpp>
#include <ibeosdk/datablocks/snippets/Measurement.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <vector>
#include <iostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class ObjectEcuEtDyn : public ibeosdk::Snippet {
public:
	enum Flags {
		Flags_TrackedByStationaryModel  = 0x0040, ///< is object tracked using stationary model
		Flags_Mobile                    = 0x0080, ///< Has been detected/validated as mobile. (the current tracking model is irrelevant; this flag just means it has been moving at some time)
		Flags_Validated                 = 0x0100  ///< Object (stationary or dynamic) has been validated, i.e. valid enough to send out to the interface
	}; // Flags

public:
	static Point2dFloat getObjectBoxPosition(RefPointBoxLocation curRefPtLoc,
	                                         const Point2dFloat refPt,
	                                         const float courseAngle,
	                                         const Point2dFloat objBoxSize,
	                                         RefPointBoxLocation targetRefPtLoc);

public:
	ObjectEcuEtDyn();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	void addMeasurement(const Measurement& meas) { m_measurementList.addMeasurement(meas); }
	void addContourPoint(const Point2dFloat& contourPoint) {m_contourPoints.push_back(contourPoint); }

public: // getter
	UINT32 getObjectId() const { return m_objectId; }
	UINT16 getFlags() const { return m_flags; }
	bool trackedByStationaryModel() const {return (m_flags & Flags_TrackedByStationaryModel)==Flags_TrackedByStationaryModel; }
	bool mobile() const {return (m_flags & Flags_Mobile) == Flags_Mobile; }
	bool motionModelValidated() const {return (m_flags & Flags_Validated) == Flags_Validated; }
	UINT32 getObjectAge() const { return m_objectAge; }
	NTPTime getTimestamp() const { return m_timestamp; }
	UINT16 getHiddenStatusAge() const { return m_hiddenStatusAge; }
	UINT16 getObjectPredictionAge() const { return getHiddenStatusAge(); }

	ObjectClass getClassification() const { return m_classification; }
	UINT8 getClassificationQuality() const { return m_classificationQuality; }
	UINT32 getClassificationAge() const { return m_classificationAge; }
	Point2dFloat getObjectBoxSize() const { return m_objectBoxSize; }
	Point2dFloat getObjectBoxSizeSigma() const { return m_objBoxSizeSigma; }
	float getCourseAngle() const { return m_courseAngle; }
	float getCourseAngleSigma() const { return m_courseAngleSigma; }
	Point2dFloat getRelativeVelocity() const { return m_relativeVelocity; }
	Point2dFloat getRelativeVelocitySigma() const {return m_relativeVelocitySigma; }
	Point2dFloat getAbsoluteVelocity() const { return m_absoluteVelocity; }
	Point2dFloat getAbsoluteVelocitySigma() const { return m_absoluteVelocitySigma; }
	float getObjectHeight() const { return m_objectHeight; }
	float getObjectHeightSigma() const { return m_objectHeightSigma; }
	Point2dFloat getMotionReferencePoint() const { return m_motionReferencePoint; }
	Point2dFloat getMotionReferencePointSigma() const { return m_motionReferencePointSigma; }
	float getLongitudinalAcceleration() const { return m_longitudianlAcceleration; }
	float getLongitudinalAccelerationSigma() const { return m_longitudianlAccelerationSigma; }
	float getYawRate() const { return m_yawRate; }
	float getYawRateSigma() const { return m_yawRateSigma; }
	UINT8 getNumContourPoints() const { return UINT8(m_contourPoints.size()); }
	UINT8 getClosestContourPointIndex() const { return m_closestContourPointIndex; }
	RefPointBoxLocation getReferencePointLocation() const { return m_referencePointLocation; }
	Point2dFloat getReferencePointCoord() const { return m_referencePointCoord; }
	Point2dFloat getReferencePointCoordSigma() const { return m_referencePointCoordSigma; }
	float getReferencePointPositionCorrCoeff() const { return m_referencePointPositionCorrCoeff; }

	Point2dFloat getObjectBoxPosition(const RefPointBoxLocation targetRefPtLoc) const;
	Point2dFloat getObjectBoxCenter() const;

	Point2dFloat getCenterOfGravity() const { return m_centerOfGravity; }
	UINT16 getObjectPriority() const { return m_objectPriority; }
	float getObjectExistenceMeas() const { return m_objectExistenceMeas; }

	INT8 getObjectBoxHeightOffset() const { return m_objectBoxHeightOffset; }
	float getObjectBoxHeightOffsetCm() const { return m_objectBoxHeightOffset * 4.f; }
	UINT8 getObjectBoxHeightOffsetSigma() const { return m_objectBoxHeightOffsetSigma; }

	UINT8 getReserved3() const { return m_reserved3; }
	UINT8 getReserved4() const { return m_reserved4; }

	const std::vector<Point2dFloat>& getContourPoints() const { return m_contourPoints; }

	const MeasurementList& getMeasurementList() const { return m_measurementList; }
	const MeasurementList& dynamicObjectProperties() const { return getMeasurementList(); }

public: // setter
	void setObjectId(const UINT32 id) { m_objectId = id; }
	void setFlags(const UINT16 flags) { m_flags = flags; }
	void setObjectAge(const UINT32 objectAge) { m_objectAge = objectAge; }
	void setTimestamp(const NTPTime timestamp) { m_timestamp = timestamp; }
	void setHiddenStatusAge(const UINT16 hiddenStatusAge) { m_hiddenStatusAge = hiddenStatusAge; }

	void setClassification(const ObjectClass classification) { m_classification = classification; }
	void setClassificationQuality(const UINT8 classificationQuality) { m_classificationQuality = classificationQuality; }
	void setClassificationAge(const UINT32 classificationAge) { m_classificationAge = classificationAge; }

	void setObjectBoxSize(const Point2dFloat& objectBoxSize) { m_objectBoxSize = objectBoxSize; }
	void setObjectBoxSizeSigma(const Point2dFloat& objectBoxSizeSigma) { m_objBoxSizeSigma = objectBoxSizeSigma; }
	void setCourseAngle(const float courseAngle) { m_courseAngle = courseAngle; }
	void setCourseAngleSigma(const float courseAngleSigma) { m_courseAngleSigma = courseAngleSigma; }

	void setRelativeVelocity(const Point2dFloat& relativeVelocity) { m_relativeVelocity = relativeVelocity; }
	void setRelativeVelocitySigma(const Point2dFloat& relativeVelocitySigma) {m_relativeVelocitySigma = relativeVelocitySigma; }
	void setAbsoluteVelocity(const Point2dFloat& absoluteVelocity) { m_absoluteVelocity = absoluteVelocity; }
	void setAbsoluteVelocitySigma(const Point2dFloat& absoluteVelocitySigma) { m_absoluteVelocitySigma = absoluteVelocitySigma; }

	void setObjectHeight(const float objectHeight) { m_objectHeight = objectHeight; }
	void setObjectHeightSigma(const float objectHeightSigma) { m_objectHeightSigma = objectHeightSigma; }

	void setMotionReferencePoint(const Point2dFloat& motionReferencePoint) { m_motionReferencePoint = motionReferencePoint; }
	void setMotionReferencePointSigma(const Point2dFloat& motionReferencePointSigma) { m_motionReferencePointSigma = motionReferencePointSigma; }

	void setLongitudinalAcceleration(const float longitudinalAcceleration) { m_longitudianlAcceleration = longitudinalAcceleration; }
	void setLongitudinalAccelerationSigma(const float longitudinalAccelerationSigma) { m_longitudianlAccelerationSigma = longitudinalAccelerationSigma; }

	void setYawRate(const float yawRate) { m_yawRate = yawRate; }
	void setYawRateSigma(const float yawRateSigma) { m_yawRateSigma = yawRateSigma; }

	void setClosestContourPointIndex(const UINT8 closestContourPointIndex) { assert(closestContourPointIndex < m_contourPoints.size()); m_closestContourPointIndex = closestContourPointIndex; }

	void setReferencePointLocation(const RefPointBoxLocation referencePointLocation) { m_referencePointLocation = referencePointLocation; }
	void setReferencePointCoord(const Point2dFloat& referencePointCoord) { m_referencePointCoord = referencePointCoord; }
	void setReferencePointCoordSigma(const Point2dFloat& referencePointCoordSigma) { m_referencePointCoordSigma = referencePointCoordSigma; }
	void setReferencePointPositionCorrCoeff(const float referencePointPositionCorrCoeff) { m_referencePointPositionCorrCoeff = referencePointPositionCorrCoeff; }

	void setCenterOfGravity(const Point2dFloat& centerOfGravity) { m_centerOfGravity = centerOfGravity; }
	void setObjectPriority(const UINT16 objectPriority) { m_objectPriority = objectPriority; }
	void setObjectExistenceMeas(const float objectExistenceMeas) { m_objectExistenceMeas = objectExistenceMeas; }

	void setContourPoints(const std::vector<Point2dFloat>& contourPoints) { m_contourPoints = contourPoints; }
	void setMeasurementList(const MeasurementList& measList) { m_measurementList = measList; }

	void setObjectBoxHeightOffset(const INT8& objectBoxHeightOffset)  { m_objectBoxHeightOffset = objectBoxHeightOffset; }
	void setObjectBoxHeightOffsetSigma(const UINT8& objectBoxHeightOffsetSigma)  { m_objectBoxHeightOffsetSigma = objectBoxHeightOffsetSigma; }

	void setReserved3(const UINT8& reserved3) {m_reserved3 = reserved3;}
	void setReserved4(const UINT8& reserved4) {m_reserved4 = reserved4;}

public:
	Point2dFloat convertRefPoint(const RefPointBoxLocation toPos) const
	{
		return ibeosdk::convertRefPoint(this->getReferencePointCoord(),
		                                this->getCourseAngle(),
		                                this->getObjectBoxSize(),
		                                this->getReferencePointLocation(),
		                                toPos);
	}

public:
	static const MeasurementKey mkey_OxtsTargetNumber;
	static const MeasurementKey mkey_VisibilityLaser;
	static const MeasurementKey mkey_OcclusionLaser;
	static const MeasurementKey mkey_VisibilityDut;
	static const MeasurementKey mkey_OcclusionDut;
	
public:
	bool operator==(const ObjectEcuEtDyn& other) const;
	bool operator!=(const ObjectEcuEtDyn& other) const { return !((*this) == other); }

protected:
	UINT32 m_objectId;
	UINT16 m_flags;
	UINT32 m_objectAge;
	NTPTime m_timestamp;
	UINT16 m_hiddenStatusAge;

	ObjectClass m_classification;
	UINT8 m_classificationQuality;
	UINT32 m_classificationAge;

	Point2dFloat m_objectBoxSize;
	Point2dFloat m_objBoxSizeSigma;
	float m_courseAngle;
	float m_courseAngleSigma;

	Point2dFloat m_relativeVelocity;
	Point2dFloat m_relativeVelocitySigma;
	Point2dFloat m_absoluteVelocity;
	Point2dFloat m_absoluteVelocitySigma;

	float m_objectHeight;
	float m_objectHeightSigma;

	Point2dFloat m_motionReferencePoint;
	Point2dFloat m_motionReferencePointSigma;

	float m_longitudianlAcceleration;
	float m_longitudianlAccelerationSigma;

	float m_yawRate;
	float m_yawRateSigma;

	// m_numContourPoints (UINT8)
	UINT8 m_closestContourPointIndex;

	RefPointBoxLocation m_referencePointLocation;
	Point2dFloat m_referencePointCoord;
	Point2dFloat m_referencePointCoordSigma;
	float m_referencePointPositionCorrCoeff;

	Point2dFloat m_centerOfGravity;
	UINT16 m_objectPriority;
	float m_objectExistenceMeas;

	// height of buttom edge of the object box
	// from the ground (z=0 in vehicle coordinates)
	// Unit: 4 cm units. -5.12m .. +5.08m
	INT8 m_objectBoxHeightOffset;
	// uncertainity of objectBoxHeightOffset in cm.
	// max value: 2.55m
	UINT8 m_objectBoxHeightOffsetSigma;
	UINT8 m_reserved3;
	UINT8 m_reserved4;

	std::vector<Point2dFloat> m_contourPoints;
	MeasurementList m_measurementList;
}; // ObjectEcuEtDyn

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTECUETDYN_HPP_SEEN

//======================================================================

