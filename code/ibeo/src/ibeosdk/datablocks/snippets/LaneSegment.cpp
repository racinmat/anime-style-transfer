//======================================================================
/*! \file LaneSegment.cpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief Segment of a lane
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/LaneSegment.hpp>
#include <ibeosdk/Line2dFloat.hpp>
#include <ibeosdk/TransformationMatrix2dFloat.hpp>

#include <boost/shared_ptr.hpp>
#include <vector>

namespace ibeosdk{
namespace lanes{

//======================================================================

LaneSegmentPtr LaneSegment::create()
{
	return LaneSegmentPtr(new LaneSegment());
}

//======================================================================

LaneSegmentPtr LaneSegment::create(LanePtr parent)
{
	return LaneSegmentPtr(new LaneSegment(parent));
}

//======================================================================

LaneSegmentPtr LaneSegment::create(const LaneSupportPoint& laneStart)
{
	return LaneSegmentPtr(new LaneSegment(laneStart));
}

//======================================================================

LaneSegmentPtr LaneSegment::create(const UINT64& id,
                                   const LaneMarkingType& markingLeft, const LaneMarkingType& markingRight,
                                   const BorderType& borderLeft, const BorderType& borderRight,
                                   const LaneSupportPoint& laneStart,
                                   const UINT64& nextId, const UINT64& prevId, const UINT64& leftId, const UINT64& rightId,
                                   const bool& nextInNewSeg, const bool& prevInNewSeg,
                                   LanePtr parent)
{
	return LaneSegmentPtr(new LaneSegment(id,
	                                      markingLeft, markingRight,
	                                      borderLeft, borderRight,
	                                      laneStart,
	                                      nextId, prevId, leftId, rightId,
	                                      nextInNewSeg, prevInNewSeg,
	                                      parent));
}

//==============================================================================

LaneSegmentPtr LaneSegment::copy(const LaneSegmentPtr& other)
{
	LaneSegmentPtr out(new LaneSegment());
	out->setId(other->getId());
	out->setLeftMarkingType(other->getLeftMarkingType());
	out->setRightMarkingType(other->getRightMarkingType());
	out->setLeftBorderType(other->getLeftBorderType());
	out->setRightBorderType(other->getRightBorderType());
	out->setStartPoint(other->getStartPoint());
	out->setNextId(other->getNextId());
	out->setPrevId(other->getPrevId());
	out->setLeftId(other->getLeftId());
	out->setRightId(other->getRightId());
	out->setNextInNewSeg(other->isNextInNewSeg());
	out->setPrevInNewSeg(other->isPrevInNewSeg());

	return out;
}

//==============================================================================
LaneSegment::LaneSegment(const UINT64& id,
						 const LaneMarkingType& markingLeft,
						 const LaneMarkingType& markingRight,
						 const BorderType& borderLeft,
						 const BorderType& borderRight,
						 const LaneSupportPoint& laneStart,
						 const UINT64& nextId,
						 const UINT64& prevId,
						 const UINT64& leftId,
						 const UINT64& rightId,
						 const bool& nextInNewSeg,
						 const bool& prevInNewSeg,
						 LanePtr parent)
	:
	m_id(id),
	m_markingLeft(markingLeft),
	m_markingRight(markingRight),
	m_borderLeft(borderLeft),
	m_borderRight(borderRight),
	m_start(laneStart),
	m_nextId(nextId),
	m_prevId(prevId),
	m_leftId(leftId),
	m_rightId(rightId),
	m_nextInNewSeg(nextInNewSeg),
	m_prevInNewSeg(prevInNewSeg),
	m_parent(parent)
{
	init();
}

//==============================================================================

LaneSegment::LaneSegment(const LanePtr& parent) : m_parent(parent)
{
	m_id = 0;
	m_markingLeft = LMT_UNCLASSIFIED;
	m_markingRight = LMT_UNCLASSIFIED;
	m_borderLeft = BT_UNCLASSIFIED;
	m_borderRight = BT_UNCLASSIFIED;
	m_start = LaneSupportPoint();
	m_nextId = 0;
	m_prevId = 0;
	m_leftId = 0;
	m_rightId = 0;
	m_nextInNewSeg = false;
	m_prevInNewSeg = false;
	init();
}

//==============================================================================

LaneSegment::LaneSegment(const LaneSupportPoint& laneStart)
{
	m_id = 0;
	m_markingLeft = LMT_UNCLASSIFIED;
	m_markingRight = LMT_UNCLASSIFIED;
	m_borderLeft = BT_UNCLASSIFIED;
	m_borderRight = BT_UNCLASSIFIED;
	m_start = laneStart;
	m_nextId = 0;
	m_prevId = 0;
	m_leftId = 0;
	m_rightId = 0;
	m_nextInNewSeg = false;
	m_prevInNewSeg = false;
	m_parent = LanePtr();
	init();
}

//==============================================================================

LaneSegment::LaneSegment()
{
	m_id = 0;
	m_markingLeft = LMT_UNCLASSIFIED;
	m_markingRight = LMT_UNCLASSIFIED;
	m_borderLeft = BT_UNCLASSIFIED;
	m_borderRight = BT_UNCLASSIFIED;
	m_start = LaneSupportPoint();
	m_nextId = 0;
	m_prevId = 0;
	m_leftId = 0;
	m_rightId = 0;
	m_nextInNewSeg = false;
	m_prevInNewSeg = false;
	m_parent = LanePtr();
	init();
}

//==============================================================================

void LaneSegment::init()
{
	m_nextSegment = LaneSegmentPtr();
	m_prevSegment = LaneSegmentPtr();
	m_boundingBox = BoundingRectangle();
	m_length = 0.0f;
}

//==============================================================================

std::string const LaneSegment::VERSION = "1.0.0";

//==============================================================================

UINT64 LaneSegment::getId() const
{
	return m_id;
}

//==============================================================================

void LaneSegment::setId(const UINT64& id)
{
	m_id = id;
}

//==============================================================================

LaneSegmentPtr LaneSegment::getNext() const
{
	return m_nextSegment.lock();
}

//==============================================================================

void LaneSegment::setNext(const LaneSegmentPtr& next)
{
	m_nextSegment = next;
	next ? m_nextId = next->getId() : 0;
	// check carriageWaySegment
	if (getParent() && next && next->getParent() )
		m_nextInNewSeg = getParent() != next->getParent();
}

//==============================================================================

bool LaneSegment::hasNext() const
{
	return m_nextSegment.lock() ? true : false;
}

//==============================================================================

LaneSegmentPtr LaneSegment::getPrevious() const
{
	return m_prevSegment.lock();
}

//==============================================================================

void LaneSegment::setPrevious(const LaneSegmentPtr& previous)
{
	m_prevSegment = previous;
	previous ? m_prevId = previous->getId() : 0;
	// check carriageWaySegment
	if (getParent() && previous && previous->getParent() )
		m_prevInNewSeg = getParent() != previous->getParent();
}

//==============================================================================

void LaneSegment::setLeft(const LaneSegmentPtr& left)
{
	m_leftSegment = left;
	left ? m_leftId = left->getId() : 0;
}

//==============================================================================

LaneSegmentPtr LaneSegment::getLeft() const
{
	return m_leftSegment.lock();
}

//==============================================================================

void LaneSegment::setRight(const LaneSegmentPtr& right)
{
	m_rightSegment = right;
	right ? m_rightId = right->getId() : 0;
}

//==============================================================================

LaneSegmentPtr LaneSegment::getRight() const
{
	return m_rightSegment.lock();
}

//==============================================================================

bool LaneSegment::hasPrevious() const
{
	return m_prevSegment.lock() ? true : false;
}

//==============================================================================

bool LaneSegment::hasLeft() const
{
	return m_leftSegment.lock() ? true : false;
}

//==============================================================================

bool LaneSegment::hasRight() const
{
	return m_rightSegment.lock() ? true : false;
}

//==============================================================================

LanePtr LaneSegment::getParent() const
{
		return m_parent.lock();
}

//==============================================================================

void LaneSegment::setParent(const LanePtr& parent)
{
	m_parent = parent;
}

//==============================================================================

LaneMarkingType LaneSegment::getLeftMarkingType() const
{
	return m_markingLeft;
}

//==============================================================================

void LaneSegment::setLeftMarkingType(const LaneMarkingType type)
{
	m_markingLeft = type;
}

//==============================================================================

LaneMarkingType LaneSegment::getRightMarkingType() const
{
	return m_markingRight;
}

//==============================================================================

void LaneSegment::setRightMarkingType(const LaneMarkingType type)
{
	m_markingRight = type;
}

//==============================================================================
BorderType LaneSegment::getLeftBorderType() const
{
	return m_borderLeft;
}

//==============================================================================

void LaneSegment::setLeftBorderType(const BorderType type)
{
	m_borderLeft = type;
}

//==============================================================================

BorderType LaneSegment::getRightBorderType() const
{
	return m_borderRight;
}

//==============================================================================

void LaneSegment::setRightBorderType(const BorderType type)
{
	m_borderRight = type;
}

//==============================================================================

const LaneSupportPoint& LaneSegment::getStartPoint() const
{
	return m_start;
}

//==============================================================================

LaneSupportPoint& LaneSegment::getStartPoint()
{
	return m_start;
}

//==============================================================================

void LaneSegment::setStartPoint(const LaneSupportPoint& point)
{
	m_start = point;
}

//==============================================================================

UINT64 LaneSegment::getLeftId() const
{
	return m_leftId;
}

//==============================================================================

void LaneSegment::setLeftId(const UINT64& leftId)
{
	m_leftId = leftId;
}

//==============================================================================

UINT64 LaneSegment::getRightId() const
{
	return m_rightId;
}

//==============================================================================

void LaneSegment::setRightId(const UINT64& rightId)
{
	m_rightId = rightId;
}

//==============================================================================

UINT64 LaneSegment::getNextId() const
{
	return m_nextId;
}

//==============================================================================

void LaneSegment::setNextId(const UINT64& nextId)
{
	m_nextId = nextId;
}

//==============================================================================

UINT64 LaneSegment::getPrevId() const
{
	return m_prevId;
}

//==============================================================================

void LaneSegment::setPrevId(const UINT64& prevId)
{
	m_prevId = prevId;
}

//==============================================================================

bool LaneSegment::isNextInNewSeg() const
{
	return m_nextInNewSeg;
}

//==============================================================================

void LaneSegment::setNextInNewSeg(const bool inNewSeg)
{
	m_nextInNewSeg = inNewSeg;
}

//==============================================================================

bool LaneSegment::isPrevInNewSeg() const
{
	return m_prevInNewSeg;
}

//==============================================================================

void LaneSegment::setPrevInNewSeg(const bool inNewSeg)
{
	m_prevInNewSeg = inNewSeg;
}

//==============================================================================

void LaneSegment::updateConnectionIDs(const LaneSegmentPtr& reference, const bool override)
{
	if (reference )
	{
		if (m_nextId == 0 || override)
			m_nextId = reference->getNextId();
		if (m_prevId == 0 || override)
			m_prevId = reference->getPrevId();
		if (m_leftId == 0 || override)
			m_leftId = reference->getLeftId();
		if (m_rightId == 0 || override)
			m_rightId = reference->getRightId();
		if (!m_nextInNewSeg || override)
			m_nextInNewSeg = reference->isNextInNewSeg();
		if (!m_prevInNewSeg || override)
			m_prevInNewSeg = reference->isPrevInNewSeg();
	}
}

//==============================================================================

BoundingRectangle LaneSegment::getBoundingBox() const
{
	return m_boundingBox;
}

//==============================================================================

float LaneSegment::getLength() const
{
	return m_length;
}

//==============================================================================

void LaneSegment::calculateLength()
{
	if (hasNext())
	{
		double x, y;
		m_nextSegment.lock()->getStartPoint().getPoint().transformToTangentialPlane2d(m_start.getPoint(), x, y);
		m_length = float(sqrt(x*x + y*y));
	}
}

//==============================================================================

void LaneSegment::calculateWidth()
{
	if (hasNext())
	{
		Line2dFloat left(m_start.getLeftOffset(), getEndOffsetLeft());
		Line2dFloat right(m_start.getRightOffset(), getEndOffsetRight());

		Line2dFloat startCutLine(Point2dFloat(0, 100), Point2dFloat(0, -100));
		Line2dFloat endCutLine(Point2dFloat(getLength(), 100), Point2dFloat(getLength(), -100));

		Point2dFloat cls, crs, cle, cre;
		startCutLine.isIntersecting(left, &cls);
		startCutLine.isIntersecting(right, &crs);
		endCutLine.isIntersecting(left, &cle);
		endCutLine.isIntersecting(right, &cre);

		m_startWidth = (cls - crs).dist();
		m_endWidth = (cle-cre).dist();
	}
}

//==============================================================================

float LaneSegment::getWidth(const float position) const
{
	if (position <= getLength())
	{
		if (getLength() > 0)
		{
			return (m_endWidth - m_startWidth)/getLength() * position + m_startWidth;
		}
		else return 0.0f;
	}

	else if (hasNext() )
		return m_nextSegment.lock()->getWidth(position - getLength());
	else
		return 0.0f;
}

//==============================================================================

void LaneSegment::calculateBoundingBox()
{
	if (hasNext())
	{
		// create bounding box
		PositionWgs84 dummy;
		std::vector<PositionWgs84> points;
		dummy.transformFromTangentialPlaneWithHeading2d(m_start.getLeftOffset().getX(), m_start.getLeftOffset().getY(), m_start.getPoint() ); points.push_back(dummy);
		dummy.transformFromTangentialPlaneWithHeading2d(m_start.getRightOffset().getX(),  m_start.getRightOffset().getY(), m_start.getPoint() ); points.push_back(dummy);
		dummy.transformFromTangentialPlaneWithHeading2d(getEndOffsetLeft().getX(), getEndOffsetLeft().getY() , m_start.getPoint() ); points.push_back(dummy);
		dummy.transformFromTangentialPlaneWithHeading2d(getEndOffsetRight().getX(), getEndOffsetRight().getY(),  m_start.getPoint() ); points.push_back(dummy);

		bool initialized = false;
		for (std::vector<PositionWgs84>::iterator it = points.begin(); it != points.end(); it++)
		{
			if ((*it).getLatitudeInDeg() < m_boundingBox.minLatitude || !initialized)
				m_boundingBox.minLatitude = (*it).getLatitudeInDeg();
			if ((*it).getLatitudeInDeg() > m_boundingBox.maxLatitude || !initialized)
				m_boundingBox.maxLatitude = (*it).getLatitudeInDeg();
			if ((*it).getLongitudeInDeg() < m_boundingBox.minLongitude || !initialized)
				m_boundingBox.minLongitude = (*it).getLongitudeInDeg();
			if ((*it).getLongitudeInDeg() > m_boundingBox.maxLongitude || !initialized)
				m_boundingBox.maxLongitude = (*it).getLongitudeInDeg();

			initialized = true;
		}
	}
}

//==============================================================================

void LaneSegment::calculateOffsets()
{
	if (hasNext())
	{
		LaneSupportPoint endSupport = m_nextSegment.lock()->getStartPoint();

		double x, y;
		endSupport.getPoint().transformToTangentialPlaneWithHeading2d(m_start.getPoint(), x, y);
		Point2dFloat endOffset( (float) x, (float) y);

		const float angleDeviation = endOffset.polarCoordAngle();

		if (!ibeosdk::fuzzyCompareT<6>(angleDeviation, 0.0f))
		{
			// correct start offsets
			TransformationMatrix2dFloat current2Head(-angleDeviation);
			Point2dFloat sol = current2Head * getStartOffsetLeft();
			Point2dFloat sor = current2Head * getStartOffsetRight();
			endOffset = current2Head * endOffset;

			// correct start heading
			PositionWgs84 newStart = m_start.getPoint();
			newStart.setCourseAngleInRad(newStart.getCourseAngleInRad() + angleDeviation);

			m_start = LaneSupportPoint(newStart, sol, sor);
		}

		m_endOffset = endOffset;

		TransformationMatrix2dFloat start2World((float) m_start.getPoint().getCourseAngleInRad());
		TransformationMatrix2dFloat end2World((float) endSupport.getPoint().getCourseAngleInRad());
		TransformationMatrix2dFloat end2start( (start2World.inverted() * end2World).getRotation(), m_endOffset);

		m_endOffsetLeft = end2start * endSupport.getLeftOffset();
		m_endOffsetRight = end2start * endSupport.getRightOffset();
	}
}

//==============================================================================

Point2dFloat LaneSegment::getEndOffset() const
{
	return m_endOffset;
}

//==============================================================================

Point2dFloat LaneSegment::getEndOffsetLeft() const
{
	return m_endOffsetLeft;
}

//==============================================================================

Point2dFloat LaneSegment::getEndOffsetRight() const
{
	return m_endOffsetRight;
}

//==============================================================================

Point2dFloat LaneSegment::getStartOffsetLeft() const
{
	return m_start.getLeftOffset();
}

//==============================================================================

Point2dFloat LaneSegment::getStartOffsetRight() const
{
	return m_start.getRightOffset();
}

//==============================================================================

void LaneSegment::cleanIds()
{
	m_nextId = hasNext() ? getNext()->getId() : 0;
	m_prevId = hasPrevious() ? getPrevious()->getId() : 0;
	m_leftId = hasLeft() ? getLeft()->getId() : 0;
	m_rightId = hasRight() ? getRight()->getId() : 0;

	if (getParent() && getNext() && getNext()->getParent())
		m_nextInNewSeg = getParent() != getNext()->getParent();

	if (getParent() && getPrevious() && getPrevious()->getParent())
		m_prevInNewSeg = getParent() != getPrevious()->getParent();
}

//==============================================================================
std::streamsize LaneSegment::getSerializedSize() const
{
	return
			8 +                 // UINT64    m_id
			8 +                 // UINT64    m_nextId
			8 +                 // UINT64    m_prevId
			8 +                 // UINT64    m_leftId
			8 +                 // UINT64    m_rightId
			1 +                 // UINT8     m_markingLeft
			1 +                 // UINT8     m_markingRight
			1 +                 // UINT8     m_borderLeft
			1 +                 // UINT8     m_borderRight
			1 +                 // UINT8     m_nextInNewSeg
			1 +                 // UINT8     m_prevInNewSeg
			m_start.getSerializedSize(); 		// unknown
}

//==============================================================================

bool LaneSegment::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, getId());                                                                         // 8
	ibeosdk::writeBE(os, m_nextId);                                                                        // 8
	ibeosdk::writeBE(os, m_prevId);                                                                        // 8
	ibeosdk::writeBE(os, m_leftId);                                                                        // 8
	ibeosdk::writeBE(os, m_rightId);                                                                       // 8
	ibeosdk::writeBE(os, static_cast<UINT8>(m_markingLeft) );                                              // 1;
	ibeosdk::writeBE(os, static_cast<UINT8>(m_markingRight) );                                             // 1
	ibeosdk::writeBE(os, static_cast<UINT8>(m_borderLeft) );                                               // 1;
	ibeosdk::writeBE(os, static_cast<UINT8>(m_borderRight) );                                              // 1;
	ibeosdk::writeBE(os, m_nextInNewSeg);                                                                  // 1;
	ibeosdk::writeBE(os, m_prevInNewSeg);                                                                  // 1;

	// write LaneSupportPoints
	m_start.serialize(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//==============================================================================

bool LaneSegment::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	UINT8 lmtl, lmtr, btl, btr;

	ibeosdk::readBE(is, m_id);
	ibeosdk::readBE(is, m_nextId);
	ibeosdk::readBE(is, m_prevId);
	ibeosdk::readBE(is, m_leftId);
	ibeosdk::readBE(is, m_rightId);
	ibeosdk::readBE(is, lmtl);
	ibeosdk::readBE(is, lmtr);
	ibeosdk::readBE(is, btl);
	ibeosdk::readBE(is, btr);
	ibeosdk::readBE(is, m_nextInNewSeg);
	ibeosdk::readBE(is, m_prevInNewSeg);

	m_markingLeft  = static_cast<LaneMarkingType>( lmtl );
	m_markingRight = static_cast<LaneMarkingType>( lmtr );
	m_borderLeft   = static_cast<BorderType>( btl );
	m_borderRight  = static_cast<BorderType>( btr );

	m_start.deserialize(is);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//==============================================================================

PositionWgs84 LaneSegment::getEndGps()
{
	PositionWgs84 out;
	out.transformFromTangentialPlaneWithHeading2d(getLength(), .0f, m_start.getPoint());
	return out;
}

//==============================================================================

void LaneSegment::updateProperties()
{
	calculateBoundingBox();
	calculateLength();
	calculateOffsets();
	calculateWidth();
}

//==============================================================================

} // namespace lanes
} // namespace ibeo
