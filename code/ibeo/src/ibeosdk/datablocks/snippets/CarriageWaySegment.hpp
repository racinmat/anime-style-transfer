//======================================================================
/*! \file CarriageWaySegment.hpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 *
 * \brief CarriageWaySegment which has a constant number of lanes
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_CARRIAGEWAYSEGMENT_HPP
#define IBEOSDK_CARRIAGEWAYSEGMENT_HPP

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <boost/unordered_map.hpp>

#include <ibeosdk/datablocks/snippets/Lane.hpp>

//======================================================================

namespace ibeosdk{
namespace lanes{

//======================================================================

class CarriageWay;
class CarriageWaySegment;

typedef boost::shared_ptr<CarriageWay> CarriageWayPtr;
typedef boost::shared_ptr<const CarriageWay> CarriageWayConstPtr;
typedef boost::shared_ptr<CarriageWaySegment> CarriageWaySegmentPtr;
typedef boost::shared_ptr<const CarriageWaySegment> CarriageWaySegmentConstPtr;

//======================================================================
/*! \brief A CarriageWaySegment represents a single segment of a \ref CarriageWay.
 * Each CarriageWaySegment has a unique id within the parent \ref CarriageWay.
 *
 * A \ref CarriageWay holds a constant number of Lanes. The segmentation of a whole
 * \ref CarriageWay is as following:
 *
 * \ref CarriageWay \htmlonly&#8594;\endhtmlonly
 * \ref CarriageWaySegment \htmlonly&#8594;\endhtmlonly
 * \ref Lane \htmlonly&#8594;\endhtmlonly
 * \ref LaneSegment
 *
 * The connection is handled using smart pointers. Therefore it is not possible
 * to create an instance of this class, but calling \ref create() will return
 * a shared pointer to a new CarriageWaySegment.
 *
 * \sa CarriageWay \sa Lane \sa LaneSegment
 *///-------------------------------------------------------------------
class CarriageWaySegment : public ibeosdk::Snippet {
public:
	/*! \brief Default constructor
	 * Initializes all elements to 0
	 * \return A shared pointer to the created CarriageWaySegment */
	static CarriageWaySegmentPtr create();

	/*! \brief Constructor
	 * \param[in] parent The pointer to the parent CarriageWay
	 * \return A shared pointer to the created CarriageWaySegment */
	static CarriageWaySegmentPtr create(CarriageWayPtr parent);

	/*! \brief Constructor
	 * \param[in] id The unique id within the parent CarriageWay
	 * \param[in] nLanes The constant number of Lanes within that segment
	 * \param[in] parent The pointer to the parent CarriageWay
	 * \return A shared pointer to the created CarriageWaySegment */
	static CarriageWaySegmentPtr create(const UINT64& id,
	                                    const UINT8& nLanes,
	                                    CarriageWayPtr parent);

	/*! \brief Constructor
	 * \param[in] id The unique id within the parent CarriageWay
	 * \param[in] nLanes The constant number of Lanes within that segment
	 * \param[in] parent The pointer to the parent CarriageWay
	 * \param[in] nextSegmentID The unique id of the next segment within the same
	 * CarriageWay
	 * \param[in] previousSegmentID The unique id of the previos segment within the
	 * same CarriageWay
	 * \return A shared pointer to the created CarriageWaySegment */
	static CarriageWaySegmentPtr create(const UINT64& id,
	                                    const UINT8& nLanes,
	                                    CarriageWayPtr parent,
	                                    const UINT64 nextSegmentID,
	                                    const UINT64 previousSegmentID);


	static CarriageWaySegmentPtr copy( const CarriageWaySegmentPtr& other );

	//========================================
private:
	/*! \brief private constructor called by \ref create() \sa create */
	CarriageWaySegment();

	/*! \brief private constructor called by \ref create() \sa create */
	CarriageWaySegment(CarriageWayPtr parent);

	/*! \brief private constructor called by \ref create() \sa create */
	CarriageWaySegment(const UINT64& id,
	                   const UINT8& nLanes,
	                   CarriageWayPtr parent);

	/*! \brief private constructor called by \ref create() \sa create */
	CarriageWaySegment(const UINT64& id,
	                   const UINT8& nLanes,
	                   CarriageWayPtr parent,
	                   const UINT64 nextSegmentID,
	                   const UINT64 previousSegmentID);

	//========================================
public: // GET & SET

	/*! \brief Returns the number of lanes within that segment */
	UINT8 getNumberOfLanes() const;

	/*! \brief Sets the length of this segment (length along center line) */
	void setLength(const float& length);

	/*! \brief Returns the length of this segment */
	float getLength() const;

	/*! \brief Returns a constant reference to the map of child lanes */
	const LaneMap& getLanes() const;

	/*! \brief Returns a reference to the map of child lanes */
	LaneMap& getLanes();

	/*! \brief Sets the child lanes to the given map */
	void setLanes(const LaneMap& lanes);

	/*! \brief Returns the pointer to the next segment (0 if there is none) */
	const CarriageWaySegmentPtr getNext() const;

	/*! \brief Sets the pointer to the next segment */
	void setNext(const CarriageWaySegmentPtr& next);

	/*! \brief Returns the pointer to the previos segment (0 if there is none) */
	const CarriageWaySegmentPtr getPrevious() const;

	/*! \brief Sets the pointer to the previous segment */
	void setPrevious(const CarriageWaySegmentPtr& previous);

	/*! \brief Sets the pointer to the parent \ref CarriageWay */
	void setParent(const CarriageWayPtr& parent);

	/*! \brief Returns the pointer to the parent CarriageWay (0 if not set) */
	const CarriageWayPtr getParent() const;

	/// Returns the ID of the segment
	UINT64 getId() const;
	UINT64 getNextId() const;
	UINT64 getPrevId() const;

	void setId(const UINT64& id);
	void setNextId(const UINT64& nextId);
	void setPrevId(const UINT64& prevId);

	void updateConnectionIDs(const CarriageWaySegmentPtr& reference, const bool override = true);

	/** Returns a bounding rectangle of the way which is currently not the
	 * minimal one, but the one aligned to the north vector
	 */
	BoundingRectangle getBoundingBox() const;

	UINT8 getNextFreeKey() const;

	//========================================

	/// Inserts a single lane to the map of child lanes
	bool insert(LanePtr lane);

	/*! \brief Returns true if the segment has a following segment, false otherwise */
	bool hasNext() const;

	/*! \brief Returns true if the segment has a preceeding segment, false otherwise */
	bool hasPrevious() const;

	void cleanIds();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

private:
	void init();

public:
	/*! \brief The version of this implementation */
	static std::string const VERSION;

private:
	//========================================
	// initializer list
	UINT64 m_id;
	UINT8 m_nLanes;
	boost::weak_ptr<CarriageWay> m_parent;
	UINT64 m_nextId;
	UINT64 m_prevId;


	LaneMap m_lanes;

	boost::weak_ptr<CarriageWaySegment> m_nextSegment;
	boost::weak_ptr<CarriageWaySegment> m_prevSegment;

	BoundingRectangle m_boundingBox;
	//========================================
	friend class CarriageWay;
}; // class CarriageWaySegment

//======================================================================

typedef boost::unordered_map<UINT64, CarriageWaySegmentPtr> CarriageWaySegmentMap;
typedef CarriageWaySegmentMap::value_type CarriageWaySegmentMapEntry;

//======================================================================

} // namespace lanes
} // namespace ibeo

//======================================================================

#endif // IBEOSDK_CARRIAGEWAYSEGMENT_HPP

//======================================================================
