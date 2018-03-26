//======================================================================
/*! \file Lane.hpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief Lane which has a list of LaneSegments
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LANE_HPP
#define IBEOSDK_LANE_HPP

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/LaneType.hpp>

#include <boost/unordered_map.hpp>

#include <ibeosdk/datablocks/snippets/LaneSegment.hpp>

//======================================================================

namespace ibeosdk{
namespace lanes{

//======================================================================

class CarriageWaySegment;
class Lane;


typedef boost::shared_ptr<CarriageWaySegment> CarriageWaySegmentPtr;
typedef boost::shared_ptr<const CarriageWaySegment> CarriageWaySegmentConstPtr;
typedef boost::shared_ptr<Lane> LanePtr;
typedef boost::shared_ptr<const Lane> LaneConstPtr;

//======================================================================
/*!\brief A Lane represents a Lane within a \ref CarriageWaySegment
 *
 * Each Lane has a unique id within the parent \ref CarriageWaySegment
 *
 * A Lane holds a list of LaneSegments as well as pointers to preceeding,
 * following and neighbouring Lanes.
 *
 * The segmentation of a whole road is as following:
 *
 * \ref CarriageWay \htmlonly&#8594;\endhtmlonly
 * \ref CarriageWaySegment \htmlonly&#8594;\endhtmlonly
 * \ref Lane \htmlonly&#8594;\endhtmlonly
 * \ref LaneSegment
 *
 * The connection is handled using smart pointers. Therefore it is not possible
 * to create an instance of this class, but calling \ref create will return
 * a shared pointer to a new Lane.
 *
 * \sa CarriageWay \sa CarriageWaySegment \sa LaneSegment
 *///-------------------------------------------------------------------
class Lane : public ibeosdk::Snippet {
private: // constructors
	//========================================
	/*! \brief private constructor called by \ref create() \sa create */
	Lane();

	/*! \brief private constructor called by \ref create \sa create */
	Lane(CarriageWaySegmentPtr parent);

	/*! \brief private constructor called by \ref create \sa create */
	Lane(const UINT8& id,
	     const UINT64 laneId,
	     const LaneType& type,
	     CarriageWaySegmentPtr parent,
	     const UINT8& nextLaneId,
	     const UINT8& previousLaneId,
	     const UINT8& leftLaneId,
	     const UINT8& rightLaneId);

public:
	/*! \brief Default constructor
	 * Initializes all members to 0
	 * \return A shared pointer to the created Lane
	 */
	static LanePtr create();

	/*! \brief Constructor
	 * \param[in] parent A pointer to the parent CarriageWaySegment
	 * \return A shared pointer to the created Lane
	 */
	static LanePtr create(CarriageWaySegmentPtr parent);

	/*! \brief Constructor
	 * \param[in] id The unique id of this Lane within the parent CarriageWaySegment
	 * \param[in] laneId The unique id of the Lane within the parent CarriageWay
	 * \param[in] type The \ref LaneType of this Lane
	 * \param[in] parent A pointer to the parent CarriageWaySegment
	 * \param[in] nextLaneId The id of the following Lane (0 if there is none)
	 * \param[in] previousLaneId The id of the preceeding Lane ( 0 if there is none)
	 * \param[in] leftLaneId The id of the left neighbouring Lane (0 if there is none)
	 * \param[in] rightLaneId The id of the right neighbouring Lane (0 if there is none)
	 */
	static LanePtr create(const UINT8& id,
	                      const UINT64 laneId,
	                      const LaneType& type,
	                      CarriageWaySegmentPtr parent,
	                      const UINT8& nextLaneId,
	                      const UINT8& previousLaneId,
	                      const UINT8& leftLaneId,
	                      const UINT8& rightLaneId);

	static LanePtr copy(const LanePtr& other);

public: // setter & getter
	//========================================
	/*! \brief Returns the unique id for the Lane within the \ref CarriageWaySegment */
	UINT8 getId() const;

	/*! \brief Sets the unique id for the Lane within the \ref CarriageWaySegment */
	void setId(const UINT8& id);

	/*! \brief Returns the unique id of the Lane within the parent \ref CarriageWay (0 if there is none) */
	UINT64 getLaneId() const;

	/*! \brief Sets the unique id of the Lane within the parent \ref CarriageWay */
	void setLaneId(const UINT64& id);

	/*! \brief Returns the pointer to the parent \ref CarriageWaySegment */
	const CarriageWaySegmentPtr getParent() const;

	/*! \brief Sets the pointer to the parent \ref CarriageWaySegment */
	void setParent(const CarriageWaySegmentPtr& parent);

	/*! \brief Return the pointer to the following Lane (0 if there is none) */
	const LanePtr getNext() const;

	/*! \brief Sets the pointer to the next Lane */
	void setNext(const LanePtr& next);

	/*! \brief Return the pointer to the preceeding Lane (0 if there is none) */
	const LanePtr getPrevious() const;

	/*! \brief Sets the pointer to the previous Lane */
	void setPrevious(const LanePtr& previous);

	/*! \brief Return the pointer to the left neighbouring Lane (0 if there is none) */
	const LanePtr getLeft() const;

	/*! \brief Sets the pointer to the left Lane */
	void setLeft(const LanePtr& left);

	/*! \brief Return the pointer to the right neighbouring Lane (0 if there is none) */
	const LanePtr getRight() const;

	/*! \brief Sets the pointer to the right Lane */
	void setRight(const LanePtr& right);

	/*! \brief Returns the type of the Lane */
	LaneType getType() const;

	void setType(const LaneType type);

	UINT8 getNextLaneId() const;
	void setNextLaneId(const UINT8 nextId);

	UINT8 getPrevLaneId() const;
	void setPrevLaneId(const UINT8 prevId);

	UINT8 getLeftLaneId() const;
	void setLeftLaneId(const UINT8 leftId);

	UINT8 getRightLaneId() const;
	void setRightLaneId(const UINT8 rightId);

	void updateConnectionIDs(const LanePtr& reference, const bool override = true);

	/*! \brief Returns a pointer to the map with all child LaneSegments */
	const LaneSegmentMap& getSegments() const;
	LaneSegmentMap& getSegments();

	/*! \brief Sets all child LaneSegments to the given ones */
	void setSegments(const LaneSegmentMap& segments);

	/*! \brief Returns a bounding rectangle of the way which is currently not the
	 * minimal one, but the one aligned to the north vector
	 */
	BoundingRectangle getBoundingBox() const;
	UINT64 getNextFreeKey() const;


	//========================================

	/*! \brief Inserts a single LaneSegment to this lane */
	bool insert(LaneSegmentPtr segment);

	/*! \brief Returns true if the Lane has a following Lane, false otherwise */
	bool hasNext() const;

	/*! \brief Returns true if the Lane has a preceeding Lane, false otherwise */
	bool hasPrevious() const;

	/*! \brief Returns true if the Lane has a left neighbouring Lane, false otherwise */
	bool hasLeft() const;

	/*! \brief Returns true if the Lane has a right neighbouring Lane, false otherwise */
	bool hasRight() const;

	void cleanIds();

public:
	//========================================
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

	//========================================

private:
	void init();

public: // public static attributes
	//========================================
	/*! \brief The version of this implementation */
	static std::string const VERSION;

private: // private attributes
	//========================================
	// Members
	UINT8 m_id;
	UINT64 m_laneId;

	LaneType m_type;
	boost::weak_ptr<CarriageWaySegment> m_parent;

	UINT8 m_nextLaneId;
	UINT8 m_prevLaneId;
	UINT8 m_leftLaneId;
	UINT8 m_rightLaneId;

	boost::weak_ptr<Lane> m_nextLane;
	boost::weak_ptr<Lane> m_prevLane;
	boost::weak_ptr<Lane> m_leftLane;
	boost::weak_ptr<Lane> m_rightLane;
	LaneSegmentMap m_segments;

	BoundingRectangle m_boundingBox;

	friend class CarriageWay;
}; // class Lane

//======================================================================

typedef boost::unordered_map<UINT8, LanePtr> LaneMap;
typedef LaneMap::value_type LaneMapEntry;

//======================================================================

} // namespace lanes
} // namespace ibeo

//======================================================================

#endif // IBEOSDK_LANE_HPP

//======================================================================

