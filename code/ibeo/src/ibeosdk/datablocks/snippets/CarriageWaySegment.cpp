//======================================================================
/*! \file CarriageWaySegment.cpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 *
 * \brief CarriageWaySegment which has a constant number of lanes
 *///-------------------------------------------------------------------


#include <ibeosdk/datablocks/snippets/CarriageWaySegment.hpp>
#include <ibeosdk/datablocks/snippets/CarriageWay.hpp>

//======================================================================

namespace ibeosdk{
namespace lanes{

//======================================================================

std::string const CarriageWaySegment::VERSION = "1.0.0";

//======================================================================

CarriageWaySegmentPtr CarriageWaySegment::create()
{
	return CarriageWaySegmentPtr(new CarriageWaySegment());
}

//======================================================================

CarriageWaySegmentPtr CarriageWaySegment::create(CarriageWayPtr parent)
{
	return CarriageWaySegmentPtr(new CarriageWaySegment(parent));
}

//======================================================================

CarriageWaySegmentPtr CarriageWaySegment::create(const UINT64& id,
                                                 const UINT8& nLanes,
                                                 CarriageWayPtr parent)
{
	return CarriageWaySegmentPtr(new CarriageWaySegment(id, nLanes, parent) );
}

//======================================================================

CarriageWaySegmentPtr CarriageWaySegment::create(const UINT64& id,
                                                 const UINT8& nLanes,
                                                 CarriageWayPtr parent,
                                                 const UINT64 nextSegmentID,
                                                 const UINT64 previousSegmentID)
{
	return CarriageWaySegmentPtr(new CarriageWaySegment(id, nLanes, parent,
		nextSegmentID, previousSegmentID));
}

//======================================================================

CarriageWaySegmentPtr CarriageWaySegment::copy(const CarriageWaySegmentPtr& other)
{
	CarriageWaySegmentPtr out(new CarriageWaySegment());
	out->setId(other->getId());
	out->setPrevId(other->getPrevId());
	out->setNextId(other->getNextId());

	return out;
}

//======================================================================

CarriageWaySegment::CarriageWaySegment(
	const UINT64& id,
                                       const UINT8& nLanes,
                                       CarriageWayPtr parent)
  : m_id(id),
    m_nLanes(nLanes),
    m_parent(parent)
{
	init();
}

//======================================================================

CarriageWaySegment::CarriageWaySegment(const UINT64& id,
                                       const UINT8& nLanes,
                                       CarriageWayPtr parent,
                                       const UINT64 nextSegmentID,
                                       const UINT64 previousSegmentID)
  : m_id(id),
    m_nLanes(nLanes),
    m_parent(parent),
    m_nextId(nextSegmentID),
    m_prevId(previousSegmentID)
{
	init();
}

//======================================================================

CarriageWaySegment::CarriageWaySegment(CarriageWayPtr parent)
  : m_id(0),
    m_nLanes(0),
    m_parent(parent),
    m_nextId(0),
    m_prevId(0)
{
	init();
}

//======================================================================

CarriageWaySegment::CarriageWaySegment()
  : m_id(0),
    m_nLanes(0),
    m_parent(CarriageWayPtr()),
    m_nextId(0),
    m_prevId(0)
{
	init();
}

//======================================================================

void CarriageWaySegment::init()
{
	m_lanes = LaneMap();
	m_boundingBox = BoundingRectangle();
	m_nextSegment = CarriageWaySegmentPtr();
	m_prevSegment = CarriageWaySegmentPtr();
}

//======================================================================

const LaneMap& CarriageWaySegment::getLanes() const
{
	return m_lanes;
}

//======================================================================

LaneMap& CarriageWaySegment::getLanes()
{
	return m_lanes;
}

//======================================================================

UINT8 CarriageWaySegment::getNumberOfLanes() const
{
	return UINT8(m_lanes.size());
}

//======================================================================

const CarriageWaySegmentPtr CarriageWaySegment::getNext() const
{
	return m_nextSegment.lock();
}

//======================================================================

void CarriageWaySegment::setNext(const CarriageWaySegmentPtr& next)
{
	m_nextSegment = next;
	m_nextId = next ? next->getId() : 0;
}

//======================================================================

const CarriageWaySegmentPtr CarriageWaySegment::getPrevious() const
{
	return m_prevSegment.lock();
}

//======================================================================

void CarriageWaySegment::setPrevious(const CarriageWaySegmentPtr& previous)
{
	m_prevSegment = previous;
	m_prevId = previous ? previous->getId() : 0;
}

//======================================================================

void CarriageWaySegment::setParent(const CarriageWayPtr& parent)
{
	m_parent = parent;
}

//=============================================================================

const CarriageWayPtr CarriageWaySegment::getParent() const
{
	return m_parent.lock();
}

//======================================================================

UINT64 CarriageWaySegment::getId() const
{
	return m_id;
}

//======================================================================

void CarriageWaySegment::setId(const UINT64& id)
{
	m_id = id;
}

//======================================================================

UINT64 CarriageWaySegment::getNextId() const
{
	return m_nextId;
}

//======================================================================

void CarriageWaySegment::setNextId(const UINT64& nextId)
{
	m_nextId = nextId;
}

//======================================================================

UINT64 CarriageWaySegment::getPrevId() const
{
	return m_prevId;
}

//=============================================================================

void CarriageWaySegment::setPrevId(const UINT64& prevId)
{
	m_prevId = prevId;
}

//======================================================================

void CarriageWaySegment::updateConnectionIDs(const CarriageWaySegmentPtr& reference, const bool override)
{
	if (reference) {
		if (m_nextId == 0 || override)
			m_nextId = reference->getNextId();
		if (m_prevId == 0 || override)
			m_prevId = reference->getPrevId();
	}
}

//======================================================================
bool CarriageWaySegment::insert(LanePtr lane)
{
	// inserts the lane to the map with its unique id and checks if another
	// lane with the same id already exists.
	return (m_lanes.insert(LaneMapEntry(lane->getId(), lane))).second;
}

//======================================================================

bool CarriageWaySegment::hasNext() const
{
	return m_nextSegment.lock() == 0 ? false : true;
}

//======================================================================

bool CarriageWaySegment::hasPrevious() const
{
	return m_prevSegment.lock() == 0 ? false : true;
}

//======================================================================

BoundingRectangle CarriageWaySegment::getBoundingBox() const
{
	return m_boundingBox;
}

//======================================================================

void CarriageWaySegment::cleanIds()
{
	m_nextId = hasNext() ? getNext()->getId() : 0;
	m_prevId = hasPrevious() ? getPrevious()->getId() : 0;

	for (LaneMap::iterator it = m_lanes.begin(); it != m_lanes.end(); ++it)
		it->second->cleanIds();
}

//======================================================================

std::streamsize CarriageWaySegment::getSerializedSize() const
{
	std::streamsize size = 0;

	LaneMap::const_iterator it = m_lanes.begin();
	for (; it != m_lanes.end(); ++it)
		size += it->second->getSerializedSize();

	return
				8 +                                // UINT64 m_id
				1 +                                // UINT8  m_lanes
				8 +                                // UINT64 m_nextId
				8 +                                // UINT64 m_prevId
				size;
}

//======================================================================

bool CarriageWaySegment::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, getId());            // 8
	ibeosdk::writeBE(os, getNumberOfLanes()); // 1;
	ibeosdk::writeBE(os, m_nextId);           // 8;
	ibeosdk::writeBE(os, m_prevId);           // 8;

	LaneMap::const_iterator it = m_lanes.begin();
	for (; it != m_lanes.end(); ++it)
		it->second->serialize(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool CarriageWaySegment::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_id);
	ibeosdk::readBE(is, m_nLanes);
	ibeosdk::readBE(is, m_nextId);
	ibeosdk::readBE(is, m_prevId);

	for (UINT16 i = 0; i < m_nLanes; ++i) {
		LanePtr lane = Lane::create();
		lane->deserialize(is);
		insert(lane);
	}

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

void CarriageWaySegment::setLanes(const LaneMap& lanes)
{
	m_lanes = lanes;
}

//======================================================================

UINT8 CarriageWaySegment::getNextFreeKey() const
{
	UINT8 maxKey = 0;
	for (LaneMap::const_iterator it = m_lanes.begin(); it != m_lanes.end(); ++it)
	{
		if (it->first > maxKey)
			maxKey = it->first;
	}

	return ++maxKey;
}

//======================================================================

} // namespace lanes
} // namespace ibeo

//======================================================================

