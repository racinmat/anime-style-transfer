//======================================================================
/*! \file Lane.cpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief Lane which has a list of LaneSegments
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/Lane.hpp>

//======================================================================

namespace ibeosdk {
namespace lanes {

//======================================================================

std::string const Lane::VERSION = "1.0.0";

//======================================================================

LanePtr Lane::create()
{
	return LanePtr(new Lane());
}

//======================================================================

LanePtr Lane::create(CarriageWaySegmentPtr parent)
{
	return LanePtr(new Lane(parent));
}

//======================================================================

LanePtr Lane::create(const UINT8& id,
                     const UINT64 laneId,
                     const LaneType& type,
                     CarriageWaySegmentPtr parent,
                     const UINT8& nextLaneId,
                     const UINT8& previousLaneId,
                     const UINT8& leftLaneId,
                     const UINT8& rightLaneId)
{
	return LanePtr(new Lane(id, laneId, type, parent, nextLaneId, previousLaneId, leftLaneId, rightLaneId));
}

//======================================================================

LanePtr Lane::copy(const LanePtr& other)
{
	LanePtr out(new Lane());

	out->setId(other->getId());
	out->setLaneId(other->getLaneId());
	out->setType(other->getType());
	out->setNextLaneId(other->getNextLaneId());
	out->setPrevLaneId(other->getPrevLaneId());
	out->setLeftLaneId(other->getLeftLaneId());
	out->setRightLaneId(other->getRightLaneId());

	return out;
}

//======================================================================

Lane::Lane(const UINT8& id,
           const UINT64 laneId,
           const LaneType& type,
           CarriageWaySegmentPtr parent,
           const UINT8& nextLaneId,
           const UINT8& previousLaneId,
           const UINT8& leftLaneId,
           const UINT8& rightLaneId)
  : m_id(id),
    m_laneId(laneId),
    m_type(type),
    m_parent(parent),
    m_nextLaneId(nextLaneId),
    m_prevLaneId(previousLaneId),
    m_leftLaneId(leftLaneId),
    m_rightLaneId(rightLaneId)
{
	init();
}

//======================================================================

Lane::Lane(CarriageWaySegmentPtr parent)
  : m_parent(parent)
{
	m_id = 0;
	m_laneId = 0;
	m_type = LT_UNCLASSIFIED;
	m_nextLaneId = 0;
	m_prevLaneId = 0;
	m_leftLaneId = 0;
	m_rightLaneId = 0;
	init();
}

//======================================================================

Lane::Lane()
{
	m_id = 0;
	m_laneId = 0;
	m_type = LT_UNCLASSIFIED;
	m_parent = CarriageWaySegmentPtr();
	m_nextLaneId = 0;
	m_prevLaneId = 0;
	m_leftLaneId = 0;
	m_rightLaneId = 0;
	init();
}

//==============================================================================

void Lane::init()
{
	m_segments = LaneSegmentMap();
	m_nextLane = LanePtr();
	m_prevLane = LanePtr();
	m_leftLane = LanePtr();
	m_rightLane = LanePtr();
	m_boundingBox = BoundingRectangle();
}

//==============================================================================

UINT8 Lane::getId() const
{
	return m_id;
}

//==============================================================================

void Lane::setId(const UINT8& id)
{
	m_id = id;
}

//==============================================================================

UINT64 Lane::getLaneId() const
{
	return m_laneId;
}

//==============================================================================

void Lane::setLaneId(const UINT64& id)
{
	m_laneId = id;
}

//==============================================================================

const CarriageWaySegmentPtr Lane::getParent() const
{
	return m_parent.lock();
}

//==============================================================================

void Lane::setParent(const CarriageWaySegmentPtr& parent)
{
	m_parent = parent;
}

//==============================================================================

const LanePtr Lane::getLeft() const
{
	return m_leftLane.lock();
}

//==============================================================================

void Lane::setLeft(const LanePtr& left)
{
	m_leftLane = left;
	m_leftLaneId = left ? left->getId() : 0;
}

//==============================================================================

const LanePtr Lane::getNext() const
{
	return m_nextLane.lock();
}

//==============================================================================

void Lane::setNext(const LanePtr& next)
{
	m_nextLane = next;
	m_nextLaneId = next ? next->getId() : 0;
}

//==============================================================================

const LanePtr Lane::getPrevious() const
{
	return m_prevLane.lock();
}

//==============================================================================

void Lane::setPrevious(const LanePtr& previous)
{
	m_prevLane = previous;
	m_prevLaneId = previous ? previous->getId() : 0;
}

//==============================================================================

const LanePtr Lane::getRight() const
{
	return m_rightLane.lock();
}

//==============================================================================

void Lane::setRight(const LanePtr& right)
{
	m_rightLane = right;
	m_rightLaneId =  right ? right->getId() : 0;
}

//==============================================================================

bool Lane::insert(LaneSegmentPtr const segment)
{
		// inserts the segment to the map with its unique id and checks if another
	// segment with the same id already exists.
	return (m_segments.insert(LaneSegmentMapEntry(segment->getId(), segment))).second;
}

//==============================================================================

bool Lane::hasNext() const
{
	return m_nextLane.lock() == 0 ? false : true;
}

//==============================================================================

bool Lane::hasPrevious() const
{
	return m_prevLane.lock() == 0 ? false : true;
}

//==============================================================================

bool Lane::hasLeft() const
{
	return m_leftLane.lock() == 0 ? false : true;
}

//==============================================================================

bool Lane::hasRight() const
{
	return m_rightLane.lock() == 0? false : true;
}

//==============================================================================

LaneType Lane::getType() const
{
	return m_type;
}

//==============================================================================

void Lane::setType(const LaneType type)
{
	m_type = type;
}

//==============================================================================
const LaneSegmentMap& Lane::getSegments() const
{
	return m_segments;
}

//==============================================================================
LaneSegmentMap& Lane::getSegments()
{
	return m_segments;
}

//==============================================================================

UINT8 Lane::getLeftLaneId() const
{
	return m_leftLaneId;
}

//==============================================================================

void Lane::setLeftLaneId(const UINT8 leftId)
{
	m_leftLaneId = leftId;
}

//==============================================================================

UINT8 Lane::getRightLaneId() const
{
	return m_rightLaneId;
}

//==============================================================================

void Lane::setRightLaneId(const UINT8 rightId)
{
	m_rightLaneId = rightId;
}

//==============================================================================

void Lane::updateConnectionIDs(const LanePtr& reference, const bool override)
{
	if (reference)
	{
		if (m_leftLaneId == 0 || override)
			m_leftLaneId = reference->getLeftLaneId();
		if (m_rightLaneId == 0 || override)
			m_rightLaneId = reference->getRightLaneId();
		if (m_nextLaneId == 0 || override)
			m_nextLaneId = reference->getNextLaneId();
		if (m_prevLaneId == 0 || override)
			m_prevLaneId = reference->getPrevLaneId();
	}
}

//==============================================================================

UINT8 Lane::getNextLaneId() const
{
	return m_nextLaneId;
}

//==============================================================================

void Lane::setNextLaneId(const UINT8 nextId)
{
	m_nextLaneId = nextId;

}

//==============================================================================

UINT8 Lane::getPrevLaneId() const
{
	return m_prevLaneId;
}

//==============================================================================

void Lane::setPrevLaneId(const UINT8 prevId)
{
	m_prevLaneId = prevId;
}

//==============================================================================

BoundingRectangle Lane::getBoundingBox() const
{
	return m_boundingBox;
}

//==============================================================================

UINT64 Lane::getNextFreeKey() const
{
	UINT64 maxKey = 0;
	for (LaneSegmentMap::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it)
	{
		if (it->first > maxKey)
			maxKey = it->first;
	}

	return ++maxKey;
}

//==============================================================================

void Lane::cleanIds()
{
	m_nextLaneId = hasNext() ? getNext()->getId() : 0;
	m_prevLaneId = hasPrevious() ? getPrevious()->getId() : 0;
	m_leftLaneId = hasLeft() ? getLeft()->getId() : 0;
	m_rightLaneId = hasRight() ? getRight()->getId() : 0;

	for (LaneSegmentMap::iterator it = m_segments.begin(); it != m_segments.end(); ++it)
		it->second->cleanIds();
}

//==============================================================================
std::streamsize Lane::getSerializedSize() const
{
	std::streamsize size = 0;

	for (LaneSegmentMap::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it)
		size += it->second->getSerializedSize();

	return  1 +                                      // UINT8   m_id;
	        8 +                                      // UINT64  m_laneId
	        1 +                                      // UINT8   m_type
	        1 +                                      // UINT8   m_nextLaneId
	        1 +                                      // UINT8   m_prevLaneId
	        1 +                                      // UINT8   m_leftLaneId
	        1 +                                      // UINT8   m_rightLaneId
	        8 +                                      // UINT64  number of segments
	        size;
}

//==============================================================================

bool Lane::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, getId());                                                 // 1
	ibeosdk::writeBE(os, getLaneId());                                             // 8;
	ibeosdk::writeBE(os, static_cast<UINT8>(getType()));                           // 1
	ibeosdk::writeBE(os, m_nextLaneId);                                            // 1;
	ibeosdk::writeBE(os, m_prevLaneId);                                            // 1;
	ibeosdk::writeBE(os, m_leftLaneId);                                            // 1;
	ibeosdk::writeBE(os, m_rightLaneId);                                           // 1;
	ibeosdk::writeBE(os, static_cast<UINT64>(m_segments.size()));                  // 8;

	// write all segments
	for (LaneSegmentMap::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it)
		it->second->serialize(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//==============================================================================

bool Lane::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	UINT8 type;
	UINT64 nSegments;

	ibeosdk::readBE(is, m_id);				// 1
	ibeosdk::readBE(is, m_laneId);			// 8
	ibeosdk::readBE(is, type);				// 1
	ibeosdk::readBE(is, m_nextLaneId);		// 1
	ibeosdk::readBE(is, m_prevLaneId);		// 1
	ibeosdk::readBE(is, m_leftLaneId);		// 1
	ibeosdk::readBE(is, m_rightLaneId);		// 1
	ibeosdk::readBE(is, nSegments);

	m_type = static_cast<LaneType>(type);

	// read segments
	for (UINT64 i = 0; i < nSegments; ++i) {
		LaneSegmentPtr seg = LaneSegment::create();
		seg->deserialize(is);

		insert(seg);
	}

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//==============================================================================

void Lane::setSegments(const LaneSegmentMap& segments)
{
	m_segments = segments;
}

//==============================================================================

} // namespace lanes
} // namespace ibeo
