//======================================================================
/*! \file ObjectLux.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_OBJECTLUX_HPP_SEEN
#define IBEOSDK_OBJECTLUX_HPP_SEEN

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

class ObjectLux : public Snippet {
public:
	ObjectLux();
	ObjectLux(const ObjectLux& other);
	ObjectLux& operator= (const ObjectLux& other);

	virtual ~ObjectLux();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	UINT16 getObjectId() const { return m_id; }
	UINT16 getObjectAge() const { return m_age; }
	UINT16 getPredictionAge() const { return m_predictionAge; }

	UINT16 getRelativeTimestamp() const { return m_relativeTimestamp; }
	Point2d getReferencePoint() const { return m_refPoint; }
	PointSigma2d getReferencePointSigma() const { return m_refPointSigma; }

	Point2d getClosestPoint() const { return m_closestPoint; }

	Point2d getBoundingBoxCenter() const { return m_boundingBoxCenter; }
	UINT16 getBoundingBoxWidth() const { return m_boundingBoxWidth; }
	UINT16 getBoundingBoxLength() const { return m_boundingBoxLength; }


	Point2d getObjectBoxCenter() const { return m_objectBoxCenter; }
	UINT16 getObjectBoxSizeX() const { return m_objectBoxSizeX; }
	UINT16 getObjectBoxSizeY() const { return m_objectBoxSizeY; }
	INT16 getObjectBoxOrientation() const { return m_objectBoxOrientation; }

	Point2d getAbsoluteVelocity() const { return m_absVelocity; }
	UINT16 getAbsoluteVelocitySigmaX() const { return m_absVelSigmaX; }
	UINT16 getAbsoluteVelocitySigmaY() const { return m_absVelSigmaY; }

	Point2d getRelativeVelocity() const { return m_relVelocity; }

	luxObjectClass::LuxObjectClass getClassification() const { return m_class; }
	UINT16 getClassificationAge() const { return m_classAge; }
	UINT16 getClassificationCertainty() const { return m_classCertainty; }

	UINT16 getNumberOfContourPoints() const { return m_numContourPoints; }
	bool isNumContourPointsValid() const { return this->m_numContourPointsIsValid; }
	const std::vector<Point2d>& getContourPoints() const { return m_contourPoints; }
	std::vector<Point2d>& getContourPoints() { return m_contourPoints; }

public:
	void setObjectId(const UINT16 id) { this->m_id = id; }
	void setObjectAge(const UINT16 age) { this->m_age = age; }
	void setPredictionAge(const UINT16 predictionAge) { this->m_predictionAge = predictionAge; }
	void setRelativeTimestamp(const UINT16 relativeTimestamp) { this->m_relativeTimestamp = relativeTimestamp; }
	void setRefPoint(const Point2d refPoint) { this->m_refPoint = refPoint; }
	void setRefPointSigma(const PointSigma2d refPointSigma) { this->m_refPointSigma = refPointSigma; }
	void setClosestPoint(const Point2d closestPoint) { this->m_closestPoint = closestPoint; }
	void setBoundingBoxCenter(const Point2d boundingBoxCenter) { this->m_boundingBoxCenter = boundingBoxCenter; }
	void setBoundingBoxWidth(const UINT16 boundingBoxWidth) { this->m_boundingBoxWidth = boundingBoxWidth; }
	void setBoundingBoxLength(const UINT16 boundingBoxLength) { this->m_boundingBoxLength = boundingBoxLength; }
	void setObjectBoxCenter(const Point2d objectBoxCenter) { this->m_objectBoxCenter = objectBoxCenter; }
	void setObjectBoxLength(const UINT16 objectBoxLength) { this->m_objectBoxSizeX = objectBoxLength; }
	void setObjectBoxWidth(const UINT16 objectBoxWidth) { this->m_objectBoxSizeY = objectBoxWidth; }
	void setObjectBoxOrientation(const INT16 objectBoxOrientation) { this->m_objectBoxOrientation = objectBoxOrientation; }
	void setAbsVelocity(const Point2d absVelocity) { this->m_absVelocity = absVelocity; }
	void setAbsVelSigmaX(const UINT16 absVelSigmaX) { this->m_absVelSigmaX = absVelSigmaX; }
	void setAbsVelSigmaY(const UINT16 absVelSigmaY) { this->m_absVelSigmaY = absVelSigmaY; }
	void setRelVelocity(const Point2d relVelocity) { this->m_relVelocity = relVelocity; }
	void setClass(const luxObjectClass::LuxObjectClass cl) { this->m_class = cl; }
	void setClassAge(const UINT16 classAge) { this->m_classAge = classAge; }
	void setClassCertainty(const UINT16 classCertainty) { this->m_classCertainty = classCertainty; }
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
	static const int maxContourPoints = 34;

protected:
	UINT16 m_id;
	UINT16 m_age;
	UINT16 m_predictionAge;

	UINT16 m_relativeTimestamp;
	Point2d m_refPoint;
	PointSigma2d m_refPointSigma;
	Point2d m_closestPoint;
	Point2d m_boundingBoxCenter;
	UINT16 m_boundingBoxWidth; // y-value
	UINT16 m_boundingBoxLength; // x-value
	Point2d m_objectBoxCenter;
	UINT16 m_objectBoxSizeX; // x-value
	UINT16 m_objectBoxSizeY; // y-value
	INT16 m_objectBoxOrientation; // angle in [deg/100].
	Point2d m_absVelocity;
	UINT16 m_absVelSigmaX;
	UINT16 m_absVelSigmaY;
	Point2d m_relVelocity;
	luxObjectClass::LuxObjectClass m_class;
	UINT16 m_classAge;
	UINT16 m_classCertainty;
	UINT16 m_numContourPoints;
	bool m_numContourPointsIsValid;
	std::vector<Point2d> m_contourPoints;
}; // ObjectLux

//======================================================================

std::ostream& operator<<(std::ostream& os, const ObjectLux& luxObj);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTLUX_HPP_SEEN

//======================================================================

