//======================================================================
/*! \file FilteredObjectAttributes.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 23, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_FILTEREDOBJECTATTRIBUTES_HPP_SEEN
#define IBEOSDK_FILTEREDOBJECTATTRIBUTES_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <ibeosdk/ObjectBasic.hpp>
#include <ibeosdk/Point2d.hpp>
#include <ibeosdk/PointSigma2d.hpp>
#include <ibeosdk/datablocks/snippets/ContourPoint.hpp>

#include <vector>
#include <iostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class DynamicFlagExistenceValues {
public:
	static const uint8_t mask;
	static const uint8_t init;
	static const uint8_t existing;

public:
	static bool isInitial(const uint8_t flags){ return (flags & mask) == init; }
	static bool isExisting(const uint8_t flags) { return (flags & mask) == existing; }

	static void setAsInitial(uint8_t& flags) { flags = uint8_t((flags & ~mask) | init); }
	static void setAsExisting(uint8_t& flags) { flags = uint8_t((flags & ~mask) | existing); }
}; // DynamicFlagExistenceValues

//======================================================================

class DynamicFlagDynState {
public:
	static const uint8_t mask;
	static const uint8_t unknownDynState;
	static const uint8_t dynamicMoving;
	static const uint8_t dynamicStopped;
	static const uint8_t dynamicStarted;
	static const uint8_t aPrioryStaticObj;

public:
	static bool isOfUnknownDynState(const uint8_t flags) { return (flags & mask) == unknownDynState; }
	static bool isDynamicMoving(const uint8_t flags) { return (flags & mask) == dynamicMoving; }
	static bool isDynamicStopped(const uint8_t flags) { return (flags & mask) == dynamicStopped; }
	static bool isDynamicStarted(const uint8_t flags) { return (flags & mask) == dynamicStarted; }
	static bool isAPrioryStaticObj(const uint8_t flags) { return (flags & mask) == aPrioryStaticObj; }

	static void setToUnknownDynState(uint8_t& flags) { flags = uint8_t((flags & ~mask) | unknownDynState); }
	static void setToDynamicMoving(uint8_t& flags) { flags = uint8_t((flags & ~mask) | dynamicMoving); }
	static void setToDynamicStopped(uint8_t& flags) { flags = uint8_t((flags & ~mask) | dynamicStopped); }
	static void setToDynamicStarted(uint8_t& flags) { flags = uint8_t((flags & ~mask) | dynamicStarted); }
	static void setToAPrioryStaticObj(uint8_t& flags) { flags = uint8_t((flags & ~mask) | aPrioryStaticObj); }

}; // DynamicFlagDynState

//======================================================================

class FilteredObjectAttributes : public ibeosdk::Snippet,
                                 protected DynamicFlagExistenceValues,
                                 protected DynamicFlagDynState {
public:
	FilteredObjectAttributes();
	virtual ~FilteredObjectAttributes();
public:
	bool isValid() const { return m_isValid; }
	bool hasContourPoints() const { return m_hasContourPoints; }

	uint8_t getPriority() const { return m_priority; }

	uint16_t getObjectAge() const { return m_objectAge; }
	uint16_t getHiddenStatusAge() const { return m_hiddenStatusAge; }
	uint8_t getDynamicFlag() const { return m_dynamicFlag; }

	bool isInitial() const { return DynamicFlagExistenceValues::isInitial(m_dynamicFlag); }
	bool isExisting() const { return DynamicFlagExistenceValues::isExisting(m_dynamicFlag); }

	bool isOfUnknownDynState() const { return DynamicFlagDynState::isOfUnknownDynState(m_dynamicFlag); }
	bool isDynamicMoving() const { return DynamicFlagDynState::isDynamicMoving(m_dynamicFlag); }
	bool isDynamicStopped() const { return DynamicFlagDynState::isDynamicStopped(m_dynamicFlag); }
	bool isDynamicStarted() const { return DynamicFlagDynState::isDynamicStarted(m_dynamicFlag); }
	bool isAPrioryStaticObj() const { return DynamicFlagDynState::isAPrioryStaticObj(m_dynamicFlag); }

	uint16_t getRelativeTimeOfMeasure() const { return m_relativeTimeOfMeasure; }
	Point2d getPositionClosestObjectPoint() const { return m_positionClosestObjectPoint; }

	Point2d getRelativeVelocity() const { return m_relativeVelocity; }
	PointSigma2d getRelativeVelocitySigma() const { return m_relativeVelocitySigma; }
	rawObjectClass::RawObjectClass getClassification() const { return m_classification; }
	uint8_t getClassificationQuality() const { return m_classificationQuality; }
	uint16_t getClassificationAge() const { return m_classificationAge; }

	uint16_t getReserved() const { return m_reserved; }
	PointSigma2d getObjectBoxSize() const { return m_objectBoxSize; }
	PointSigma2d getObjectBoxSizeSigma() const { return m_objectBoxSizeSigma; }
	int16_t getObjectBoxOrientation() const { return m_objectBoxOrientation; }
	uint16_t getObjectBoxOrientationSigma() const { return m_objectBoxOrientationSigma; }
	uint8_t getObjectBoxHeight() const { return m_objectBoxHeight; }
	RefPointBoxLocation getReferencePointLocation() const { return m_referencePointLocation; }
	Point2d getReferencePointCoord() const { return m_referencePointCoord; }
	PointSigma2d getReferencePointCoordSigma() const { return m_referencePointCoordSigma; }
	int16_t getReferencePointPositionCorrCoeff() const { return m_referencePointPositionCorrCoeff; }
	uint8_t getExistenceProbaility() const { return m_existenceProbaility; }
	uint8_t getPossibleNbOfContourPoints() const { return m_possibleNbOfContourPoints; }

	Point2d getAbsoluteVelocity() const { return m_absoluteVelocity; }
	PointSigma2d getAbsoluteVelocitySigma() const { return m_absoluteVelocitySigma; }
	int16_t getVelocityCorrCoeff() const { return m_velocityCorrCoeff; }
	Point2d getAcceleration() const { return m_acceleration; }
	PointSigma2d getAccelerationSigma() const { return m_accelerationSigma; }
	int16_t getAccelerationCorrCoeff() const { return m_accelerationCorrCoeff; }
	int16_t getYawRate() const { return m_yawRate; }
	uint16_t getYawRateSigma() const { return m_yawRateSigma; }

	const std::vector<ContourPoint>& getContourPoints() const { return m_contourPoints; }
	std::vector<ContourPoint>& getContourPoints() { return m_contourPoints; }

public:
	void setIsValid(const bool newIsValid) { m_isValid = newIsValid; }
	void setHasContourPoints(const bool newHasContourPoints) { m_hasContourPoints = newHasContourPoints; }

	void setPriority(const uint8_t newPriority) { m_priority = newPriority; }

	void setObjectAge(const uint16_t newObjectAge) { m_objectAge = newObjectAge; }
	void setHiddenStatusAge(const uint16_t newHiddenStatusAge) { m_hiddenStatusAge = newHiddenStatusAge; }
	void setDynamicFlag(const uint8_t newDynamicFlag) { m_dynamicFlag = newDynamicFlag; }

	void setAsInitial() { DynamicFlagExistenceValues::setAsInitial(m_dynamicFlag); }
	void setAsExisting() { DynamicFlagExistenceValues::setAsExisting(m_dynamicFlag); }

	void setToUnknownDynState() { DynamicFlagDynState::setToUnknownDynState(m_dynamicFlag); }
	void setToDynamicMoving() { DynamicFlagDynState::setToDynamicMoving(m_dynamicFlag); }
	void setToDynamicStopped() { DynamicFlagDynState::setToDynamicStopped(m_dynamicFlag); }
	void setToDynamicStarted() { DynamicFlagDynState::setToDynamicStarted(m_dynamicFlag); }
	void setToAPrioryStaticObj() { DynamicFlagDynState::setToAPrioryStaticObj(m_dynamicFlag); }

	void setRelativeTimeOfMeasure(const uint16_t newRelativeTimeOfMeasure) { m_relativeTimeOfMeasure = newRelativeTimeOfMeasure; }
	void setPositionClosestObjectPoint(const Point2d newPositionClosestObjectPoint) { m_positionClosestObjectPoint = newPositionClosestObjectPoint; }

	void setRelativeVelocity(const Point2d newRelativeVelocity) { m_relativeVelocity = newRelativeVelocity; }
	void setRelativeVelocitySigma(const PointSigma2d newRelativeVelocitySigma) { m_relativeVelocitySigma = newRelativeVelocitySigma; }
	void setClassification(const rawObjectClass::RawObjectClass newClassification) { m_classification = newClassification; }
	void setClassificationQuality(const uint8_t newClassificationQuality) { m_classificationQuality = newClassificationQuality; }
	void setClassificationAge(const uint16_t newClassificationAge) { m_classificationAge = newClassificationAge; }

	void setReserved(const uint16_t newReserved) { m_reserved = newReserved; }
	void setObjectBoxSize(const PointSigma2d newObjectBoxSize) { m_objectBoxSize = newObjectBoxSize; }
	void setObjectBoxSizeSigma(const PointSigma2d newObjectBoxSizeSigma) { m_objectBoxSizeSigma = newObjectBoxSizeSigma; }
	void setObjectBoxOrientation(const int16_t newObjectBoxOrientation) { m_objectBoxOrientation = newObjectBoxOrientation; }
	void setObjectBoxOrientationSigma(const uint16_t newObjectBoxOrientationSigma) { m_objectBoxOrientationSigma = newObjectBoxOrientationSigma; }
	void setObjectBoxHeight(const uint8_t newObjectBoxHeight) { m_objectBoxHeight = newObjectBoxHeight; }
	void setReferencePointLocation(const RefPointBoxLocation newReferencePointLocation) { m_referencePointLocation = newReferencePointLocation; }
	void setReferencePointCoord(const Point2d newReferencePointCoord) { m_referencePointCoord = newReferencePointCoord; }
	void setReferencePointCoordSigma(const PointSigma2d newReferencePointCoordSigma) { m_referencePointCoordSigma = newReferencePointCoordSigma; }
	void setReferencePointPositionCorrCoeff(const int16_t newReferencePointPositionCorrCoeff) { m_referencePointPositionCorrCoeff = newReferencePointPositionCorrCoeff; }
	void setExistenceProbaility(const uint8_t newExistenceProbaility) { m_existenceProbaility = newExistenceProbaility; }

	void setAbsoluteVelocity(const Point2d newAbsoluteVelocity) { m_absoluteVelocity = newAbsoluteVelocity; }
	void setAbsoluteVelocitySigma(const PointSigma2d newAbsoluteVelocitySigma) { m_absoluteVelocitySigma = newAbsoluteVelocitySigma; }
	void setVelocityCorrCoeff(const int16_t newVelocityCorrCoeff) { m_velocityCorrCoeff = newVelocityCorrCoeff; }
	void setAcceleration(const Point2d newAcceleration) { m_acceleration = newAcceleration; }
	void setAccelerationSigma(const PointSigma2d newAccelerationSigma) { m_accelerationSigma = newAccelerationSigma; }
	void setAccelerationCorrCoeff(const int16_t newAccelerationCorrCoeff) { m_accelerationCorrCoeff = newAccelerationCorrCoeff; }
	void setYawRate(const int16_t newWRate) { m_yawRate = newWRate; }
	void setYawRateSigma(const uint16_t newWRateSigma) { m_yawRateSigma = newWRateSigma; }
	// no setter for m_positionClosestObjectPoint, will be adjusted when serializing.

	void setPossibleNbOfContourPoints(const uint8_t possibleNbOfCtPts) { m_possibleNbOfContourPoints = possibleNbOfCtPts; }

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

protected:
	bool m_isValid;          // not serialized
	bool m_hasContourPoints; // not serialized

	uint8_t m_priority;

	uint16_t m_objectAge;
	uint16_t m_hiddenStatusAge;
	uint8_t m_dynamicFlag;

	uint16_t m_relativeTimeOfMeasure;
	Point2d m_positionClosestObjectPoint;

	Point2d m_relativeVelocity;
	PointSigma2d m_relativeVelocitySigma;

	rawObjectClass::RawObjectClass m_classification; // as uint8_t
	uint8_t m_classificationQuality;
	uint16_t m_classificationAge;

	uint16_t m_reserved;

	PointSigma2d m_objectBoxSize;
	PointSigma2d m_objectBoxSizeSigma;
	int16_t m_objectBoxOrientation;
	uint16_t m_objectBoxOrientationSigma;
	uint8_t m_objectBoxHeight;

	RefPointBoxLocation m_referencePointLocation; // as uint8_t
	Point2d m_referencePointCoord;
	PointSigma2d m_referencePointCoordSigma;
	int16_t m_referencePointPositionCorrCoeff;

	uint8_t m_existenceProbaility;

	Point2d m_absoluteVelocity;
	PointSigma2d m_absoluteVelocitySigma;
	int16_t m_velocityCorrCoeff;

	Point2d m_acceleration;
	PointSigma2d m_accelerationSigma;
	int16_t m_accelerationCorrCoeff;

	int16_t m_yawRate;
	uint16_t m_yawRateSigma;

	mutable uint8_t m_possibleNbOfContourPoints;
	std::vector<ContourPoint> m_contourPoints;
}; // FilteredObjectAttributes

//======================================================================
//======================================================================
//======================================================================

bool operator==(const ibeosdk::FilteredObjectAttributes& lhs, const ibeosdk::FilteredObjectAttributes& rhs);
bool operator!=(const ibeosdk::FilteredObjectAttributes& lhs, const ibeosdk::FilteredObjectAttributes& rhs);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_FILTEREDOBJECTATTRIBUTES_HPP_SEEN

//======================================================================
