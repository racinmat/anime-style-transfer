//======================================================================
/*! \file UnfilteredObjectAttributes.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 23, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_UNFILTEREDOBJECTATTRIBUTES_HPP_SEEN
#define IBEOSDK_UNFILTEREDOBJECTATTRIBUTES_HPP_SEEN

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

class UnfilteredObjectAttributes : public ibeosdk::Snippet {
public:
	UnfilteredObjectAttributes();
	virtual ~UnfilteredObjectAttributes();

public:
	bool isValid() const { return m_isValid; }
	bool hasContourPoints() const { return m_hasContourPoints; }

	uint8_t getPriority() const { return m_priority; }
	uint16_t getRelativeTimeOfMeasure() const { return m_relativeTimeOfMeasure; }
	Point2d getPositionClosestObjectPoint() const { return m_positionClosestObjectPoint; }
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

	const std::vector<ContourPoint>& getContourPoints() const { return m_contourPoints; }
	std::vector<ContourPoint>& getContourPoints() { return m_contourPoints; }

public:
	void setIsValid(const bool newIsValid) { m_isValid = newIsValid; }
	void setHasContourPoints(const bool newHasContourPoints) { m_hasContourPoints = newHasContourPoints; }
	void setPriority(const uint8_t newPriority) { m_priority = newPriority; }
	void setRelativeTimeOfMeasure(const uint16_t newRelativeTimeOfMeasure) { m_relativeTimeOfMeasure = newRelativeTimeOfMeasure; }
	void setPositionClosestObjectPoint(const Point2d newPositionClosestObjectPoint) { m_positionClosestObjectPoint = newPositionClosestObjectPoint; }
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
	uint16_t m_relativeTimeOfMeasure;
	Point2d m_positionClosestObjectPoint;
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

	mutable uint8_t m_possibleNbOfContourPoints;
	std::vector<ContourPoint> m_contourPoints;
}; // UnfilteredObjectAttributes

//======================================================================
//======================================================================
//======================================================================

bool operator==(const UnfilteredObjectAttributes& lhs, const UnfilteredObjectAttributes& rhs);
bool operator!=(const UnfilteredObjectAttributes& lhs, const UnfilteredObjectAttributes& rhs);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_UNFILTEREDOBJECTATTRIBUTES_HPP_SEEN

//======================================================================
