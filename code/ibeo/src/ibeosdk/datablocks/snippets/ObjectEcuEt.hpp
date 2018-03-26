//======================================================================
/*! \file ObjectEcuEt.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 5, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_OBJECTECUET_HPP_SEEN
#define IBEOSDK_OBJECTECUET_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Point2dFloat.hpp>
#include <ibeosdk/Time.hpp>
#include <ibeosdk/ObjectBasic.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <vector>
#include <iostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class ObjectEcuEt : public ibeosdk::Snippet {
public:
	enum Flags {
		Flags_TrackedByStationaryModel  = 0x0040, ///< is object tracked using stationary model
		Flags_Mobile                    = 0x0080, ///< Has been detected/validated as mobile. (the current tracking model is irrelevant; this flag just means it has been moving at some time)
		Flags_Validated                 = 0x0100  ///< Object (stationary or dynamic) has been validated, i.e. valid enough to send out to the interface
	}; // Flags

public:
	ObjectEcuEt();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	uint16_t getObjectId() const { return m_objectId; }

	uint16_t getFlags() const { return m_flags; }
	bool trackedByStationaryModel() const {return (m_flags & Flags_TrackedByStationaryModel)==Flags_TrackedByStationaryModel; }
	bool mobile() const {return (m_flags & Flags_Mobile) == Flags_Mobile; }
	bool motionModelValidated() const {return (m_flags & Flags_Validated) == Flags_Validated; }

	uint32_t getObjectAge() const { return m_objectAge; }
	NTPTime getTimestamp() const { return m_timestamp; }
	uint16_t getObjectPredAge() const { return m_objectPredAge; }
	ObjectClass getClassification() const { return m_classification; }
	uint8_t getClassCertainty() const { return m_classCertainty; }
	uint32_t getClassAge() const { return m_classAge; }
	Point2dFloat getReserved0() const {return m_reserved0; }
	Point2dFloat getReserved1() const {return m_reserved1; }
	Point2dFloat getObjBoxCenter() const { return m_objBoxCenter; }
	Point2dFloat getObjBoxCenterSigma() const { return m_objBoxCenterSigma; }
	Point2dFloat getObjBoxSize() const { return m_objBoxSize; }
	Point2dFloat getReserved2() const {return m_reserved2; }
	IBEOSDK_DEPRECATED float getObjCourseAngle() const { return m_objBoxOrientation; }
	IBEOSDK_DEPRECATED float getObjCourseAngleSigma() const { return m_objBoxOrientationSigma; }
	float getObjBoxOrientation() const { return m_objBoxOrientation; }
	float getObjBoxOrientationSigma() const { return m_objBoxOrientationSigma; }
	Point2dFloat getRelVelocity() const { return m_relVelocity; }
	Point2dFloat getRelVelocitySigma() const { return m_relVelocitySigma; }
	Point2dFloat getAbsVelocity() const { return m_absVelocity; }
	Point2dFloat getAbsVelocitySigma() const { return m_absVelocitySigma; }
	uint64_t getReserved4() const { return m_reserved4; }
	float getReserved5() const { return m_reserved5; }
	float getReserved6() const { return m_reserved6; }
	uint16_t getReserved7() const { return m_reserved7; }
	uint8_t getNbOfContourPoints() const { return uint8_t(m_contourPoints.size()); }
	uint8_t getIdxOfClosestPoint() const { return m_idxOfClosestPoint; }
	RefPointBoxLocation getRefPointLocation() const { return m_refPointLocation; }
	Point2dFloat getRefPointCoords() const { return m_refPointCoords; }
	Point2dFloat getRefPointCoordsSigma() const { return m_refPointCoordsSigma; }
	float getRefPointPosCorrCoeffs() const { return m_refPointPosCorrCoeffs; }
	float getReserved8() const { return m_reserved8; }
	float getReserved9() const { return m_reserved9; }
	uint16_t getObjPriority() const { return m_objPriority; }
	float getObjExtMeasurement() const { return m_objExtMeasurement; }
	const std::vector<Point2dFloat>& getContourPoints() const { return m_contourPoints; }
	std::vector<Point2dFloat>& getContourPoints() { return m_contourPoints; }

public: // setter
	void setObjectId(const uint16_t newObjectId) { m_objectId = newObjectId; }

	void setFlags(const uint16_t newFlags) { m_flags = newFlags; }
	void setTrackedByStationaryModel(const bool set = true)
	{
		m_flags=set ? uint16_t(m_flags | uint16_t(Flags_TrackedByStationaryModel))
		            : uint16_t(m_flags & ~uint16_t(Flags_TrackedByStationaryModel));
	}
	void setMobile(const bool set = true)
	{
		m_flags=set ? uint16_t(m_flags | uint16_t(Flags_Mobile))
		            : uint16_t(m_flags & ~uint16_t(Flags_Mobile));
	}
	void setMotionModelValidated(const bool set = true)
	{
		m_flags=set ? uint16_t(m_flags | uint16_t(Flags_Validated))
		            : uint16_t(m_flags & ~uint16_t(Flags_Validated));
	}

	void setObjectAge(const uint32_t newObjectAge) { m_objectAge = newObjectAge; }
	void setTimestamp(const NTPTime newTimestamp) { m_timestamp = newTimestamp; }
	void setObjectPredAge(const uint16_t newObjectPredAge) { m_objectPredAge = newObjectPredAge; }
	void setClassification(const ObjectClass newClassification) { m_classification = newClassification; }
	void setClassCertainty(const uint8_t newClassCertainty) { m_classCertainty = newClassCertainty; }
	void setClassAge(const uint32_t newClassAge) { m_classAge = newClassAge; }
	void setReserved0( const Point2dFloat reserved0 ) { m_reserved0 = reserved0; }
	void setReserved1( const Point2dFloat reserved1 ) { m_reserved1 = reserved1; }
	void setObjBoxCenter(const Point2dFloat newObjBoxCenter) { m_objBoxCenter = newObjBoxCenter; }
	void setObjBoxCenterSigma(const Point2dFloat newObjBoxCenterSigma) { m_objBoxCenterSigma = newObjBoxCenterSigma; }
	void setObjBoxSize(const Point2dFloat newObjBoxSize) { m_objBoxSize = newObjBoxSize; }
	void setReserved2( const Point2dFloat reserved2 ) { m_reserved2 = reserved2; }
	IBEOSDK_DEPRECATED void setObjCourseAngle(const float newObjBoxOrientation) { m_objBoxOrientation = newObjBoxOrientation; }
	IBEOSDK_DEPRECATED void setObjCourseAngleSigma(const float newObjBoxOrientationSigma) { m_objBoxOrientationSigma = newObjBoxOrientationSigma; }
	void setObjBoxOrientation(const float newObjBoxOrientation) { m_objBoxOrientation = newObjBoxOrientation; }
	void setObjBoxOrientationSigma(const float newObjBoxOrientationSigma) { m_objBoxOrientationSigma = newObjBoxOrientationSigma; }
	void setRelVelocity(const Point2dFloat newRelVelocity) { m_relVelocity = newRelVelocity; }
	void setRelVelocitySigma(const Point2dFloat newRelVelocitySigma) { m_relVelocitySigma = newRelVelocitySigma; }
	void setAbsVelocity(const Point2dFloat newAbsVelocity) { m_absVelocity = newAbsVelocity; }
	void setAbsVelocitySigma(const Point2dFloat newAbsVelocitySigma) { m_absVelocitySigma = newAbsVelocitySigma; }
	void setReserved4( const uint64_t reserved4 ) { m_reserved4 = reserved4; }
	void setReserved5( const float reserved5 ) { m_reserved5 = reserved5; }
	void setReserved6( const float reserved6 ) { m_reserved6 = reserved6; }
	void setReserved7( const uint16_t reserved7 ) { m_reserved7 = reserved7; }
	void setNbOfContourPoints(const uint8_t newNbOfContourPoints) { m_contourPoints.resize(newNbOfContourPoints); }
	void setIdxOfClosestPoint(const uint8_t newIdxOfClosestPoint) { m_idxOfClosestPoint = newIdxOfClosestPoint; }
	void setRefPointLocation(const RefPointBoxLocation newRefPointLocation) { m_refPointLocation = newRefPointLocation; }
	void setRefPointCoords(const Point2dFloat newRefPointCoords) { m_refPointCoords = newRefPointCoords; }
	void setRefPointCoordsSigma(const Point2dFloat newRefPointCoordsSigma) { m_refPointCoordsSigma = newRefPointCoordsSigma; }
	void setRefPointPosCorrCoeffs(const float newRefPointPosCorrCoeffs) { m_refPointPosCorrCoeffs = newRefPointPosCorrCoeffs; }
	void setReserved8( const float reserved8 ) { m_reserved8 = reserved8; }
	void setReserved9( const float reserved9 ) { m_reserved9 = reserved9; }
	void setObjPriority(const uint16_t newObjPriority) { m_objPriority = newObjPriority; }
	void setObjExtMeasurement(const float newObjExtMeasurement) { m_objExtMeasurement = newObjExtMeasurement; }
	void setContourPoints( const std::vector<Point2dFloat> contourPoints ) { m_contourPoints = contourPoints; }

public:
	Point2dFloat convertRefPoint(const RefPointBoxLocation toPos) const
	{
		return ibeosdk::convertRefPoint(this->getRefPointCoords(),
		                                this->getObjBoxOrientation(),
		                                this->getObjBoxSize(),
		                                this->getRefPointLocation(),
		                                toPos);
	}

public:
	bool operator==(const ObjectEcuEt& other) const;
	bool operator!=(const ObjectEcuEt& other) const { return !((*this) == other); }

protected:
	uint16_t m_objectId;
	uint16_t m_flags;
	uint32_t m_objectAge;
	NTPTime m_timestamp;
	uint16_t m_objectPredAge;
	ObjectClass m_classification; // uint8_t
	uint8_t m_classCertainty;
	uint32_t m_classAge;
	Point2dFloat m_reserved0;
	Point2dFloat m_reserved1;
	Point2dFloat m_objBoxCenter;
	Point2dFloat m_objBoxCenterSigma;
	Point2dFloat m_objBoxSize;
	Point2dFloat m_reserved2;
	float m_objBoxOrientation; //!<[rad]
	float m_objBoxOrientationSigma; //!<[rad]
	Point2dFloat m_relVelocity;
	Point2dFloat m_relVelocitySigma;
	Point2dFloat m_absVelocity;
	Point2dFloat m_absVelocitySigma;

	// 18 bytes reserved/internal
	uint64_t m_reserved4;
	float m_reserved5;
	float m_reserved6;
	uint16_t m_reserved7;

	// nb of contour points  uint8_t
	uint8_t m_idxOfClosestPoint;

	RefPointBoxLocation m_refPointLocation; // uint16_t
	Point2dFloat m_refPointCoords;
	Point2dFloat m_refPointCoordsSigma;
	float m_refPointPosCorrCoeffs;
	float m_reserved8;
	float m_reserved9;
	uint16_t m_objPriority;
	float m_objExtMeasurement;
	std::vector<Point2dFloat> m_contourPoints;
}; // ObjectEcuEt

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTECUET_HPP_SEEN

//======================================================================

