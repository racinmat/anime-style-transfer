//======================================================================
/*! \file UnfilteredObjectAttributes.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 23, 2014
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/UnfilteredObjectAttributes.hpp>

#include <ibeosdk/ObjectBasic.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

UnfilteredObjectAttributes::UnfilteredObjectAttributes()
  : m_isValid(false),
    m_hasContourPoints(false),
    m_priority(0),
    m_relativeTimeOfMeasure(0),
    m_positionClosestObjectPoint(),
    m_reserved(0),
    m_objectBoxSize(),
    m_objectBoxSizeSigma(),
    m_objectBoxOrientation(0),
    m_objectBoxOrientationSigma(0),
    m_objectBoxHeight(0),
    m_referencePointLocation(RPL_Unknown),
    m_referencePointCoord(),
    m_referencePointCoordSigma(),
    m_referencePointPositionCorrCoeff(0),
    m_existenceProbaility(0),
    m_possibleNbOfContourPoints(0),
    m_contourPoints()
{}

//======================================================================

UnfilteredObjectAttributes::~UnfilteredObjectAttributes() {}

//======================================================================

std::streamsize UnfilteredObjectAttributes::getSerializedSize() const
{
	if (!m_isValid)
		return 0;

	return std::streamsize(sizeof(uint8_t))
	     + std::streamsize(sizeof(uint16_t))
	     + Point2d::getSerializedSize_static()
	     + std::streamsize(sizeof(uint16_t))
	     + 2*PointSigma2d::getSerializedSize() // objBox sz/sigma
	     + 2*std::streamsize(sizeof(uint16_t))
	     + 2*std::streamsize(sizeof(uint8_t))  // ... refPointLoc
	     + Point2d::getSerializedSize_static()
	     + PointSigma2d::getSerializedSize()
	     + std::streamsize(sizeof(int16_t))
	     + std::streamsize(sizeof(uint8_t)) // exist Prob
	     + std::streamsize(sizeof(uint8_t)) // nb of points
	     + (m_hasContourPoints ? m_possibleNbOfContourPoints * ContourPoint::getSerializedSize_static()
	                           : 0);
}

//======================================================================

bool UnfilteredObjectAttributes::deserialize(std::istream& is)
{
	if (!m_isValid)
		return true;

	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_priority);
	ibeosdk::readBE(is, m_relativeTimeOfMeasure);
	ibeosdk::readBE(is, m_positionClosestObjectPoint);
	ibeosdk::readBE(is, m_reserved);
	ibeosdk::readBE(is, m_objectBoxSize);
	ibeosdk::readBE(is, m_objectBoxSizeSigma);
	ibeosdk::readBE(is, m_objectBoxOrientation);
	ibeosdk::readBE(is, m_objectBoxOrientationSigma);
	ibeosdk::readBE(is, m_objectBoxHeight);
	ibeosdk::readBE<8>(is, m_referencePointLocation);
	ibeosdk::readBE(is, m_referencePointCoord);
	ibeosdk::readBE(is, m_referencePointCoordSigma);
	ibeosdk::readBE(is, m_referencePointPositionCorrCoeff);
	ibeosdk::readBE(is, m_existenceProbaility);

	ibeosdk::readBE(is, m_possibleNbOfContourPoints);
	// only if this UnfilteredObjectAttributes object is dedicated to
	// have contour points, nbOfContourPoints presents the number
	// of contour points that have been attached.
	// Otherwise nbOfContourPoints is just the number of contour
	// points that would have been written in case it was allowed to.
	if (m_hasContourPoints) {
		m_contourPoints.resize(m_possibleNbOfContourPoints);
		std::vector<ContourPoint>::iterator cpIter = m_contourPoints.begin();
		for (; cpIter != m_contourPoints.end(); ++cpIter) {
			cpIter->deserialize(is);
		}
	}
	else {
		m_contourPoints.clear();
	}

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool UnfilteredObjectAttributes::serialize(std::ostream& os) const
{
	if (!m_isValid)
		return true;

	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_priority);
	ibeosdk::writeBE(os, m_relativeTimeOfMeasure);
	ibeosdk::writeBE(os, m_positionClosestObjectPoint);
	ibeosdk::writeBE(os, m_reserved);
	ibeosdk::writeBE(os, m_objectBoxSize);
	ibeosdk::writeBE(os, m_objectBoxSizeSigma);
	ibeosdk::writeBE(os, m_objectBoxOrientation);
	ibeosdk::writeBE(os, m_objectBoxOrientationSigma);
	ibeosdk::writeBE(os, m_objectBoxHeight);
	ibeosdk::writeBE<8>(os, m_referencePointLocation);
	ibeosdk::writeBE(os, m_referencePointCoord);
	ibeosdk::writeBE(os, m_referencePointCoordSigma);
	ibeosdk::writeBE(os, m_referencePointPositionCorrCoeff);
	ibeosdk::writeBE(os, m_existenceProbaility);

	if (!m_contourPoints.empty())
		m_possibleNbOfContourPoints = uint8_t(m_contourPoints.size());

	ibeosdk::writeBE(os, m_possibleNbOfContourPoints);

	// only if this UnfilteredObjectAttributes object is dedicated to
	// have contour points, the contour points will be stored.
	if (m_hasContourPoints) {
		std::vector<ContourPoint>::const_iterator cpIter = m_contourPoints.begin();
		for (; cpIter != m_contourPoints.end(); ++cpIter) {
			cpIter->serialize(os);
		}
	}

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================
//======================================================================
//======================================================================

bool operator==(const UnfilteredObjectAttributes& lhs, const UnfilteredObjectAttributes& rhs)
{
	if (lhs.isValid() != rhs.isValid())
		return false;
	if (!lhs.isValid()) return true; // if the structure is not valid, its contents is irrelevant.

	if (lhs.hasContourPoints() != rhs.hasContourPoints())
		return false;

	if (lhs.getPriority() != rhs.getPriority())
		return false;
	if (lhs.getRelativeTimeOfMeasure() != rhs.getRelativeTimeOfMeasure())
		return false;
	if (lhs.getPositionClosestObjectPoint() != rhs.getPositionClosestObjectPoint())
		return false;
	if (lhs.getReserved() != rhs.getReserved())
		return false;

	if (lhs.getObjectBoxSize() != rhs.getObjectBoxSize())
		return false;
	if (lhs.getObjectBoxSizeSigma() != rhs.getObjectBoxSizeSigma())
		return false;
	if (lhs.getObjectBoxOrientation() != rhs.getObjectBoxOrientation())
		return false;
	if (lhs.getObjectBoxOrientationSigma() != rhs.getObjectBoxOrientationSigma())
		return false;
	if (lhs.getObjectBoxHeight() != rhs.getObjectBoxHeight())
		return false;

	if (lhs.getReferencePointLocation() != rhs.getReferencePointLocation())
		return false;
	if (lhs.getReferencePointCoord() != rhs.getReferencePointCoord())
		return false;
	if (lhs.getReferencePointCoordSigma() != rhs.getReferencePointCoordSigma())
		return false;
	if (lhs.getReferencePointPositionCorrCoeff() != rhs.getReferencePointPositionCorrCoeff())
		return false;

	if (lhs.getExistenceProbaility() != rhs.getExistenceProbaility())
		return false;

	if (lhs.getPossibleNbOfContourPoints() != rhs.getPossibleNbOfContourPoints())
		return false;
	if (lhs.hasContourPoints()) {
		if (lhs.getContourPoints() != rhs.getContourPoints())
			return false;
	}
	return true;
}

//======================================================================

bool operator!=(const UnfilteredObjectAttributes& lhs, const UnfilteredObjectAttributes& rhs)
{
	return !(lhs == rhs);
}

//======================================================================

}// namespace ibeosdk

//======================================================================
