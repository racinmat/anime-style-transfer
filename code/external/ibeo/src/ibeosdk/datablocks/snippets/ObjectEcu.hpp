//======================================================================
/*! \file ObjectEcu.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_OBJECTECU_HPP_SEEN
#define IBEOSDK_OBJECTECU_HPP_SEEN

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

class ObjectEcu : public Snippet {
public:
	ObjectEcu();
	ObjectEcu(const ObjectEcu& src);

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

//	int getSerializedSize() const;

public:
	UINT16 getObjectId() const { return m_id; }
	UINT16 getReserved() const { return m_reserved; }
	UINT32 getObjectAge() const { return m_age; }

	NTPTime getTimestamp() const { return m_timestamp; }
	UINT16 getHiddenStatusAge() const { return m_hiddenStatusAge; }

	ObjectClass getClassification() const { return m_class; }
	UINT8 getClassificationCertainty() const { return m_classCertainty; }
	UINT32 getClassificationAge() const { return m_classAge; }

	Point2dFloat getBoundingBoxCenter() { return m_boundingBoxCenter; }
	const Point2dFloat& getBoundingBoxCenter() const { return m_boundingBoxCenter; }
	Point2dFloat getBoundingBoxSize() { return m_boundingBoxSize; }
	const Point2dFloat& getBoundingBoxSize() const { return m_boundingBoxSize; }

	Point2dFloat getObjectBoxCenter() { return m_objectBoxCenter; }
	const Point2dFloat& getObjectBoxCenter() const { return m_objectBoxCenter; }
	Point2dFloat getObjectBoxSigma() { return m_objectBoxSigma; }
	const Point2dFloat& getObjectBoxSigma() const { return m_objectBoxSigma; }
	Point2dFloat getObjectBoxSize() { return m_objectBoxSize; }
	const Point2dFloat& getObjectBoxSize() const { return m_objectBoxSize; }
	UINT64 getReserved2() const { return m_reserved2; }

	IBEOSDK_DEPRECATED float getYawAngle() const { return m_objectBoxOrientation; }
	IBEOSDK_DEPRECATED float getYawAngleSigma() const { return m_objectBoxOrientationSigma; }
	float getObjectBoxOrientation() const { return m_objectBoxOrientation; }
	float getObjectBoxOrientationSigma() const { return m_objectBoxOrientationSigma; }

	Point2dFloat getRelativeVelocity() { return m_relVelocity; }
	const Point2dFloat& getRelativeVelocity() const { return m_relVelocity; }
	Point2dFloat getRelativeVelocitySigma() { return m_relVelocitySigma; }
	const Point2dFloat& getRelativeVelocitySigma() const { return m_relVelocitySigma; }

	Point2dFloat getAbsoluteVelocity() { return m_absVelocity; }
	const Point2dFloat& getAbsoluteVelocity() const { return m_absVelocity; }
	Point2dFloat getAbsoluteVelocitySigma() { return m_absVelocitySigma; }
	const Point2dFloat& getAbsoluteVelocitySigma() const { return m_absVelocitySigma; }
	float getOrientation() const { return this->m_orientation; }

	UINT8 getNumberOfContourPoints() const { return m_numContourPoints; }
	std::vector<Point2dFloat>& getContourPoints() { return m_contourPoints; }
	const std::vector<Point2dFloat>& getContourPoints() const { return m_contourPoints; }

	UINT8 getIndexOfClosestPoint() const { return m_indexOfClosedPoint; }

public:
	void setObjectId(const UINT16 newObjectId) { m_id = newObjectId; }
	void setReserved( const UINT16 newReserved) { m_reserved = newReserved; }
	void setObjectAge(const UINT32 newObjectAge) { m_age = newObjectAge; }

	void setTimestamp(const NTPTime newTimestamp) { m_timestamp = newTimestamp; }
	void setHiddenStatusAge(const UINT16 newHiddenStatusAge) { m_hiddenStatusAge = newHiddenStatusAge; }

	void setClassification(const ObjectClass newClassification) { m_class = newClassification; }
	void setClassificationCertainty(const UINT8 newClassificationCertainty) { m_classCertainty = newClassificationCertainty; }
	void setClassificationAge(const UINT32 newClassificationAge) { m_classAge = newClassificationAge; }

	void setBoundingBoxCenter(const Point2dFloat& newBoundingBoxCenter) { m_boundingBoxCenter = newBoundingBoxCenter; }
	void setBoundingBoxSize(const Point2dFloat& newBoundingBoxSize) { m_boundingBoxSize=newBoundingBoxSize; }
	void setReserved2(const UINT64& newReserved2) { m_reserved2 = newReserved2; }
	

	void setObjectBoxCenter(const Point2dFloat& newObjectBoxCenter) { m_objectBoxCenter=newObjectBoxCenter; }
	void setObjectBoxSigma(const Point2dFloat& newObjectBoxSigma) { m_objectBoxSigma=newObjectBoxSigma; }
	void setObjectBoxSize(const Point2dFloat& newObjectBoxSize) { m_objectBoxSize=newObjectBoxSize; }

	IBEOSDK_DEPRECATED void setYawAngle(const float newObjBoxOrientation) { m_objectBoxOrientation=newObjBoxOrientation; }
	IBEOSDK_DEPRECATED void setYawAngleSigma(const float newObjBoxOrientationSigma) { m_objectBoxOrientationSigma=newObjBoxOrientationSigma; }
	void setObjBoxOrientation(const float newObjBoxOrientation) { m_objectBoxOrientation = newObjBoxOrientation; }
	void setObjBoxOrientationSigma(const float newObjBoxOrientationSigma) { m_objectBoxOrientationSigma = newObjBoxOrientationSigma; }

	void setRelativeVelocity(const Point2dFloat& newRelativeVelocity) { m_relVelocity=newRelativeVelocity; }
	void setRelativeVelocitySigma(const Point2dFloat& newRelativeVelocitySigma) { m_relVelocitySigma=newRelativeVelocitySigma; }

	void setAbsoluteVelocity(const Point2dFloat& newAbsoluteVelocity) { m_absVelocity=newAbsoluteVelocity; }
	void setAbsoluteVelocitySigma(const Point2dFloat& newAbsoluteVelocitySigma) { m_absVelocitySigma=newAbsoluteVelocitySigma; }
	void setOrientation(const float newOrientation) { this->m_orientation = newOrientation; }

	void setNumberOfContourPoints(const UINT8 newNumberOfContourPoints) { m_numContourPoints = newNumberOfContourPoints; }
	void setContourPoints(const std::vector<Point2dFloat>& newContourPoints) { m_contourPoints = newContourPoints; }

	void setIndexOfClosestPoint(const UINT8 newIndexOfClosestPoint) { m_indexOfClosedPoint = newIndexOfClosestPoint; }
	
public:
	bool operator==(const ObjectEcu& other) const;
	bool operator!=(const ObjectEcu& other) const { return !((*this) == other); }

protected:
	static const int nbOfBytesInReserved3 = 18;

protected:
	UINT16 m_id;
	UINT16 m_reserved;
	UINT32 m_age;
	NTPTime m_timestamp;
	UINT16 m_hiddenStatusAge;

	ObjectClass m_class;
	UINT8 m_classCertainty;
	UINT32 m_classAge;

	Point2dFloat m_boundingBoxCenter;
	Point2dFloat m_boundingBoxSize;

	Point2dFloat m_objectBoxCenter;
	Point2dFloat m_objectBoxSigma;
	Point2dFloat m_objectBoxSize;

	UINT64 m_reserved2;

	float m_objectBoxOrientation;
	float m_objectBoxOrientationSigma;

	Point2dFloat m_relVelocity;
	Point2dFloat m_relVelocitySigma;

	Point2dFloat m_absVelocity;
	Point2dFloat m_absVelocitySigma;

	char m_reserved3[nbOfBytesInReserved3];

	UINT8 m_numContourPoints;
	UINT8 m_indexOfClosedPoint;
	std::vector<Point2dFloat> m_contourPoints;

protected: // not serialized data
	float m_orientation;
}; // ObjectEcu

//======================================================================

std::ostream& operator<<(std::ostream& os, const ObjectEcu& o);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTECU_HPP_SEEN

//======================================================================

