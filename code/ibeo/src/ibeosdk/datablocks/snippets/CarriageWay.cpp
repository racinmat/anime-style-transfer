//==============================================================================
/**
 * \file CarriageWay.hpp
 * \brief CarriageWay which stores CarriageWaySegments
 *
 * -----------------------------------------------------------------------------
 * \copyright &copy; 2014 Ibeo Automotive Systems GmbH, Hamburg, Germany
 *
 * \date   Oct 9, 2014
 * \author Stefan Kaufmann (stk)
 */
//==============================================================================

#include <ibeosdk/datablocks/snippets/CarriageWay.hpp>

//==============================================================================

namespace ibeosdk{
namespace lanes{

//==============================================================================

const std::string CarriageWay::VERSION = "1.0.0";

//==============================================================================

CarriageWayPtr CarriageWay::create()
{
	return CarriageWayPtr(new CarriageWay());
}

//==============================================================================

CarriageWayPtr CarriageWay::create(const UINT64& id,
                                   const UINT16& nationalId,
                                   const CarriageWayType& type,
                                   const CarriageWaySegmentMap& segments)
{
	return CarriageWayPtr(new CarriageWay(id, nationalId, type, segments));
}

//==============================================================================

CarriageWay::CarriageWay()
  : m_id(0),
    m_nationalId(0),
    m_type(CWT_UNCLASSIFIED),
    m_segments()
{
	init();
}

//==============================================================================

CarriageWay::CarriageWay(const UINT64& id,
                         const UINT16& nationalId,
                         const CarriageWayType& type,
                         const CarriageWaySegmentMap& segments)
  : m_id(id),
    m_nationalId(nationalId),
    m_type(type),
    m_segments(segments)
{
	init();
}

//=============================================================================

void CarriageWay::init()
{
	m_boundingBox = BoundingRectangle();
}

//==============================================================================

UINT16 CarriageWay::getNationalId() const
{
	return m_nationalId;
}

//=============================================================================

CarriageWayType CarriageWay::getType() const
{
	return m_type;
}

//=============================================================================

const CarriageWaySegmentMap& CarriageWay::getSegments() const
{
	return m_segments;
}

//=============================================================================

void CarriageWay::setType(const CarriageWayType& type)
{
	m_type = type;
}

//=============================================================================

void CarriageWay::setSegments(const CarriageWaySegmentMap& segments)
{
	m_segments = segments;
}

//=============================================================================

bool CarriageWay::insert(CarriageWaySegmentPtr segment)
{
	// inserts the segment to the map with its unique id and checks if another
	// segment with the same id already exists.
	return (m_segments.insert(CarriageWaySegmentMapEntry(segment->getId(), segment))).second;
}

//=============================================================================

UINT64 CarriageWay::getId() const
{
	return m_id;
}

//=============================================================================

void CarriageWay::setId(const UINT64& id)
{
	m_id = id;
}

//=============================================================================

BoundingRectangle CarriageWay::getBoundingBox() const
{
	return m_boundingBox;
}

//=============================================================================

void CarriageWay::resolveConnections(const CarriageWayPtr& instance)
{
	// 1. Create all connections between carriageway segments, since these
	// are required for connections between lanes and lane segments
	for (CarriageWaySegmentMap::const_iterator it = m_segments.begin(); it != m_segments.end(); ++it)
	{
		// pointer to segment (the object itself has to be edited, not a copy)
		CarriageWaySegmentPtr cws = it->second;
		if (cws)
		{
			cws->setParent(instance);

			// link next element if id not 0 and existing
			CarriageWaySegmentMap::iterator itf = m_segments.find(cws->getNextId());
			if ( cws->getNextId() != 0 && itf != m_segments.end() )
				cws->setNext(itf->second);

			// link next element if id not 0 and existing
			itf = m_segments.find(cws->getPrevId());
			if ( cws->getPrevId() != 0 && itf != m_segments.end() )
				cws->setPrevious(itf->second);
		}
	}

	// 2. Create all connections between lanes
	// now the pointers to previous and next segment between CarriageWaySegments
	// are all resolved
	CarriageWaySegmentMap::const_iterator cwsmIter = m_segments.begin();
	for (; cwsmIter != m_segments.end(); ++cwsmIter) {
		CarriageWaySegmentPtr cws = cwsmIter->second;
		if (cws) {
			LaneMap::const_iterator laneIter = cws->getLanes().begin();
			for (; laneIter != cws->getLanes().end(); ++laneIter) {
				LanePtr lane = laneIter->second;
				if (lane) {
					lane->setParent(cws);
					// link left neighbour
					LaneMap::iterator itf = cws->getLanes().find(lane->getLeftLaneId());
					if ( lane->getLeftLaneId() != 0 && itf != cws->getLanes().end() )
						lane->setLeft(itf->second);

					// link right neighbour
					itf = cws->getLanes().find(lane->getRightLaneId());
					if ( lane->getRightLaneId() != 0 && itf != cws->getLanes().end())
						lane->setRight(itf->second);

					// link next lane (always in next CarriageWaySegment)
					if (cws->hasNext()) {
						itf = cws->getNext()->getLanes().find(lane->getNextLaneId());
						if ( lane->getNextLaneId() != 0  && itf != cws->getNext()->getLanes().end() )
							lane->setNext(itf->second);
					}

					// link previous lane (always in previous CarriageWaySegment)
					if (cws->hasPrevious()) {
						itf = cws->getPrevious()->getLanes().find(lane->getPrevLaneId());
						if ( lane->getPrevLaneId() != 0 && itf != cws->getPrevious()->getLanes().end() )
							lane->setPrevious(itf->second);
					} // if
				} // if lane
			} // for laneIter
		} // if cws
	} // for cwsmIter

	// 3. Create all connections between LaneSegments
	// now the pointers to previous and next lane are all resolved
	cwsmIter = m_segments.begin();
	for (; cwsmIter != m_segments.end(); ++cwsmIter) {
		CarriageWaySegmentPtr cws = cwsmIter->second;
		if (cws) {
			LaneMap::const_iterator itlane = cws->getLanes().begin();
			for (; itlane != cws->getLanes().end(); ++itlane) {
				LanePtr lane = itlane->second;
				if (lane) {
					LaneSegmentMap::const_iterator itLaneSeg = lane->getSegments().begin();
					for (; itLaneSeg != lane->getSegments().end(); ++itLaneSeg) {
						LaneSegmentPtr laneSeg = itLaneSeg->second;

						if (laneSeg) {
							LaneSegmentMap::iterator itf;
							laneSeg->setParent(lane);
							// link next segment if in same CarriageWaySegment
							if (!laneSeg->isNextInNewSeg()) {
								itf = lane->getSegments().find(laneSeg->getNextId());
								if (laneSeg->getNextId() != 0 && itf != lane->getSegments().end() )
									laneSeg->setNext(itf->second);
							} // if

							// if in new CarriageWaySegment
							else if (lane->hasNext()) {
								itf = lane->getNext()->getSegments().find(laneSeg->getNextId());
								if (laneSeg->getNextId() != 0 && itf != lane->getNext()->getSegments().end())
									laneSeg->setNext(itf->second);
							}

							// link previous segment if in same CarriageWaySegment
							if (!laneSeg->isPrevInNewSeg()) {
								itf = lane->getSegments().find(laneSeg->getPrevId());
								if ( laneSeg->getPrevId() != 0 && itf != lane->getSegments().end())
									laneSeg->setPrevious(itf->second);
							}

							// if in new CarriageWaySegment
							else if (lane->hasPrevious()) {
								itf = lane->getPrevious()->getSegments().find(laneSeg->getPrevId());
								if (laneSeg->m_prevId != 0 && itf != lane->getPrevious()->getSegments().end())
									laneSeg->setPrevious(itf->second);
							}

							// link left
							if (laneSeg->getLeftId() != 0 && lane->hasLeft()) {
								itf = lane->getLeft()->getSegments().find(laneSeg->getLeftId());
								if ( itf != lane->getLeft()->getSegments().end())
									laneSeg->setLeft(itf->second);
							}

							// link right
							if (laneSeg->getRightId() != 0 && lane->hasRight()) {
								itf = lane->getRight()->getSegments().find(laneSeg->getRightId());
								if ( itf != lane->getRight()->getSegments().end())
									laneSeg->setRight(itf->second);
							}

							laneSeg->updateProperties();

							// set bounding box for LANE from child bounds
							if (laneSeg->getBoundingBox().minLatitude < lane->getBoundingBox().minLatitude)
								lane->m_boundingBox.minLatitude = laneSeg->getBoundingBox().minLatitude;
							if (laneSeg->getBoundingBox().maxLatitude > lane->getBoundingBox().maxLatitude)
								lane->m_boundingBox.maxLatitude = laneSeg->getBoundingBox().maxLatitude;
							if (laneSeg->getBoundingBox().minLongitude < lane->getBoundingBox().minLongitude)
								lane->m_boundingBox.minLongitude = laneSeg->getBoundingBox().minLongitude;
							if (laneSeg->getBoundingBox().maxLongitude > lane->getBoundingBox().maxLongitude)
								lane->m_boundingBox.maxLongitude = laneSeg->getBoundingBox().maxLongitude;
						}
					}

					// set bounding box for CARRIAGE_WAY_SEGMENT from child bounds
					if (lane->getBoundingBox().minLatitude < cws->getBoundingBox().minLatitude)
						cws->m_boundingBox.minLatitude = lane->getBoundingBox().minLatitude;
					if (lane->getBoundingBox().maxLatitude > cws->getBoundingBox().maxLatitude)
						cws->m_boundingBox.maxLatitude = lane->getBoundingBox().maxLatitude;
					if (lane->getBoundingBox().minLongitude < cws->getBoundingBox().minLongitude)
						cws->m_boundingBox.minLongitude = lane->getBoundingBox().minLongitude;
					if (lane->getBoundingBox().maxLongitude > cws->getBoundingBox().maxLongitude)
						cws->m_boundingBox.maxLongitude = lane->getBoundingBox().maxLongitude;
				}
			}


			// set bounding box for CARRIAGE_WAY from child bounds
			if (cws->getBoundingBox().minLatitude < m_boundingBox.minLatitude)
				m_boundingBox.minLatitude = cws->getBoundingBox().minLatitude;
			if (cws->getBoundingBox().maxLatitude > m_boundingBox.maxLatitude)
				m_boundingBox.maxLatitude = cws->getBoundingBox().maxLatitude;
			if (cws->getBoundingBox().minLongitude < m_boundingBox.minLongitude)
				m_boundingBox.minLongitude = cws->getBoundingBox().minLongitude;
			if (cws->getBoundingBox().maxLongitude > m_boundingBox.maxLongitude)
				m_boundingBox.maxLongitude = cws->getBoundingBox().maxLongitude;
		}
	}
}

//==============================================================================

bool CarriageWay::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_id);                                         // 8
	ibeosdk::writeBE(os, m_nationalId);                                 // 2;
	ibeosdk::writeBE(os, static_cast<UINT8>(m_type));                   // 1
	ibeosdk::writeBE(os, static_cast<UINT64>(m_segments.size() ));      // 8

	CarriageWaySegmentMap::const_iterator it = m_segments.begin();
	for (; it != m_segments.end(); ++it)
		it->second->serialize(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//==============================================================================

std::streamsize CarriageWay::getSerializedSize() const
{
	UINT64 size = 0;

	CarriageWaySegmentMap::const_iterator it = m_segments.begin();
	for (; it != m_segments.end(); ++it)
		size += UINT64(it->second->getSerializedSize());

	return std::streamsize(
		8 +     // UINT64 m_id
		2 +     // UINT16 m_nationalId
		1 +     // UINT8  m_type
		8 +     // UINT64 number of segments
		size);   // size of segments
}

//==============================================================================

bool CarriageWay::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	UINT8 type;
	UINT64 nSegments;

	ibeosdk::readBE(is, m_id);
	ibeosdk::readBE(is, m_nationalId);
	ibeosdk::readBE(is, type);
	ibeosdk::readBE(is, nSegments);
	m_type = static_cast<CarriageWayType>(type);

	for (UINT64 i = 0; i < nSegments; ++i) {
		CarriageWaySegmentPtr segment = CarriageWaySegment::create();
		segment->deserialize(is);
		insert(segment);
	}

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//==============================================================================

} // namespace lanes
} // namespace ibeo

//==============================================================================
