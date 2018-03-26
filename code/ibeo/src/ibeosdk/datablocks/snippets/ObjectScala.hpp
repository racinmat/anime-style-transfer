//======================================================================
/*! \file ObjectScala.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 17, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_OBJECTSCALA_HPP_SEEN
#define IBEOSDK_OBJECTSCALA_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <ibeosdk/ObjectBasic.hpp>
#include <ibeosdk/Point2d.hpp>
#include <ibeosdk/PointSigma2d.hpp>


#include <vector>
#include <iostream>
#include <cmath>

//======================================================================

namespace ibeosdk {

//======================================================================

class ObjectScala : public ibeosdk::Snippet {
public:
	enum Flags {
		Flag_Stationary            = 0x01, ///< Stationary model if set.
		Flag_Mobile                = 0x02, ///< Mobile, can only be set if object tracked by dynamic model.
		Flag_MotionModelValidated  = 0x04  ///< Motion model validated, is true if object is validated according to motion model.
	}; // Flags

public:
	ObjectScala();
	ObjectScala(const ObjectScala& other);
	ObjectScala& operator= (const ObjectScala& other);

	virtual ~ObjectScala();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	UINT16 getObjectId() const { return m_id; }
	UINT16 getObjectAge() const { return m_age; }
	UINT16 getPredictionAge() const { return m_predictionAge; }

	UINT16 getRelativeTimestamp() const { return m_relativeTimestamp; }
	RefPointBoxLocation getRefPointBoxLocation() const { return m_referencePointLocation; }
	Point2d getReferencePoint() const { return m_refPoint; }
	PointSigma2d getReferencePointSigma() const { return m_refPointSigma; }

	UINT16 getRefPointPosCorrCoeff() const { return m_referencePointPositionCorrCoeff; }
	UINT16 getObjectPriority() const { return m_objectPriority; }
	UINT16 getObjectExistenceMeasure() const { return m_objectExistenceMeasure; }

	Point2d getClosestPoint() const { return m_closestPoint; }

	Point2d getBoundingBoxCenter() const { return m_boundingBoxCenter; }
	UINT16 getBoundingBoxWidth() const { return m_boundingBoxWidth; }
	UINT16 getBoundingBoxLength() const { return m_boundingBoxLength; }


	Point2d getObjectBoxCenter() const { return m_objectBoxCenter; }
	UINT16 getObjectBoxSizeX() const { return m_objectBoxSizeX; }
	UINT16 getObjectBoxSizeY() const { return m_objectBoxSizeY; }
	INT16 getObjectBoxOrientation() const { return m_objectBoxOrientation; }
	PointSigma2d getObjectBoxSizeSigma() const { return m_objectBoxSizeSigma; }
	UINT16 getObjectBoxOrientationSigma() const { return m_objectBoxOrientationSigma; }

	Point2d getAbsoluteVelocity() const { return m_absVelocity; }
	PointSigma2d getAbsoluteVelocitySigma() const { return m_absVelSigma; }

	Point2d getRelativeVelocity() const { return m_relVelocity; }
	PointSigma2d getRelativeVelocitySigma() const { return m_relVelSigma; }

	ObjectClass getClassification() const { return m_class; }

	UINT8 getFlags() const { return m_flags; }
	bool trackedByStationary() const {return (m_flags & Flag_Stationary) == Flag_Stationary; }
	bool mobile() const {return (m_flags & Flag_Mobile) == Flag_Mobile; }
	bool motionModelValidated() const {return (m_flags & Flag_MotionModelValidated) == Flag_MotionModelValidated; }

	UINT16 getClassificationAge() const { return m_classAge; }
	UINT16 getClassificationConfidence() const { return m_classConfidence; }

	UINT16 getNumberOfContourPoints() const { return m_numContourPoints; }
	const std::vector<Point2d>& getContourPoints() const { return m_contourPoints; }
	std::vector<Point2d>& getContourPoints() { return m_contourPoints; }

public:
	void setObjectId(const UINT16 id) { this->m_id = id; }
	void setObjectAge(const UINT16 age) { this->m_age = age; }
	void setPredictionAge(const UINT16 predictionAge) { this->m_predictionAge = predictionAge; }
	void setRelativeTimestamp(const UINT16 relativeTimestamp) { this->m_relativeTimestamp = relativeTimestamp; }

	void setRefPointBoxLocation(const RefPointBoxLocation newRefPtLocation) { m_referencePointLocation = newRefPtLocation; }
	void setRefPoint(const Point2d refPoint) { this->m_refPoint = refPoint; }
	void setRefPointSigma(const PointSigma2d refPointSigma) { this->m_refPointSigma = refPointSigma; }
	void setReferencePointPositionCorrCoeff(const UINT16 corrCoeff) { this->m_referencePointPositionCorrCoeff = corrCoeff; }

	void setObjectPriority(const UINT16 newObjPrio) { this->m_objectPriority = newObjPrio; }
	void setObjectExistenceMeasure(const UINT16 newExistenceMeasure) { this->m_objectExistenceMeasure = newExistenceMeasure; }
	void setClosestPoint(const Point2d newClosesPoint) { this->m_closestPoint = newClosesPoint; }

	void setBoundingBoxCenter(const Point2d boundingBoxCenter) { this->m_boundingBoxCenter = boundingBoxCenter; }
	void setBoundingBoxWidth(const UINT16 boundingBoxWidth) { this->m_boundingBoxWidth = boundingBoxWidth; }
	void setBoundingBoxLength(const UINT16 boundingBoxLength) { this->m_boundingBoxLength = boundingBoxLength; }

	void setObjectBoxCenter(const Point2d objectBoxCenter) { this->m_objectBoxCenter = objectBoxCenter; }
	void setObjectBoxLength(const UINT16 objectBoxLength) { this->m_objectBoxSizeX = objectBoxLength; }
	void setObjectBoxWidth(const UINT16 objectBoxWidth) { this->m_objectBoxSizeY = objectBoxWidth; }
	void setObjectBoxOrientation(const INT16 objectBoxOrientation) { this->m_objectBoxOrientation = objectBoxOrientation; }
	void setObjectBoxSizeSigma(const PointSigma2d newObjBoxSizeSigma) { this->m_objectBoxSizeSigma = newObjBoxSizeSigma; }
	void setObjectBoxOrientationSigma(const UINT16 newOrientSigma) { this->m_objectBoxOrientationSigma = newOrientSigma; }


	void setAbsVelocity(const Point2d absVelocity) { this->m_absVelocity = absVelocity; }
	void setAbsVelSigma(const PointSigma2d absVelSigma) { this->m_absVelSigma = absVelSigma; }
	void setRelVelocity(const Point2d relVelocity) { this->m_relVelocity = relVelocity; }
	void setRelVelSigma(const PointSigma2d relVelSigma) { this->m_relVelSigma = relVelSigma; }

	void setClass(const ObjectClass cl) { this->m_class = cl; }

	void getFlags(const UINT8 newFlags) { this->m_flags = newFlags; }
	void setTrackedByStationaryModel(const bool set = true)
	{
		m_flags=set ? UINT8(m_flags | UINT8(Flag_Stationary))
		            : UINT8(m_flags & ~UINT8(Flag_Stationary));
	}
	void setMobile(const bool set = true)
	{
		m_flags=set ? UINT8(m_flags | UINT8(Flag_Mobile))
		            : UINT8(m_flags & ~UINT8(Flag_Mobile));
	}
	void setMotionModelValidated(const bool set = true)
	{
		m_flags=set ? UINT8(m_flags | UINT8(Flag_MotionModelValidated))
		            : UINT8(m_flags & ~UINT8(Flag_MotionModelValidated));
	}

	void setClassAge(const UINT16 classAge) { this->m_classAge = classAge; }
	void setClassCertainty(const UINT16 classConfidence) { this->m_classConfidence = classConfidence; }

	void setNumContourPoints(const UINT16 numContourPoints)
	{
		if (numContourPoints!= contourIsInvalid) {
			this->m_numContourPoints = numContourPoints;
			this-> m_numContourPointsIsValid = true;
		}
		else {
			this->m_numContourPoints = 1;
			this-> m_numContourPointsIsValid = false;
		}
	}

	void setNumCoutourPointsValid(const bool valid) { this->m_numContourPointsIsValid = valid; }
	void setContourPoints(const std::vector<Point2d>& newContourPts) { this->m_contourPoints = newContourPts; }


public:
	static float angle2rad(const INT16 ticks)
	{
		const UINT16 angleTicksPerRotation = 36000;
		// (x < 0) ? ((x % N) + N) : x % N
		return float(((ticks < 0) ? float((ticks % angleTicksPerRotation) + angleTicksPerRotation)
		                          : float(ticks % angleTicksPerRotation))
		             * 2.f * M_PI / float(angleTicksPerRotation));
	}

protected:
	static const UINT16 contourIsInvalid;

protected:
	UINT16 m_id;
	UINT16 m_age;
	UINT16 m_predictionAge;

	UINT16 m_relativeTimestamp;
	UINT8 m_reserved0;

	RefPointBoxLocation m_referencePointLocation; // as UINT8
	Point2d m_refPoint;
	PointSigma2d m_refPointSigma;
	UINT16 m_referencePointPositionCorrCoeff;

	UINT16 m_objectPriority;
	UINT16 m_objectExistenceMeasure;
	Point2d m_closestPoint;

	Point2d m_boundingBoxCenter;
	UINT16 m_boundingBoxWidth; // y-value
	UINT16 m_boundingBoxLength; // x-value

	Point2d m_objectBoxCenter;
	UINT16 m_objectBoxSizeX; // x-value
	UINT16 m_objectBoxSizeY; // y-value
	INT16 m_objectBoxOrientation; // angle in [deg/100].
	PointSigma2d m_objectBoxSizeSigma;
	UINT16 m_objectBoxOrientationSigma;

	Point2d m_absVelocity;
	PointSigma2d m_absVelSigma;

	Point2d m_relVelocity;
	PointSigma2d m_relVelSigma;

	ObjectClass m_class; // as UINT8
	UINT8 m_flags;
	UINT16 m_classAge;
	UINT16 m_classConfidence;

	UINT16 m_numContourPoints;
	bool m_numContourPointsIsValid;
	std::vector<Point2d> m_contourPoints;
}; // ObjectScala

//======================================================================

std::ostream& operator<<(std::ostream& os, const ObjectScala& obj);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTSCALA_HPP_SEEN

//======================================================================
