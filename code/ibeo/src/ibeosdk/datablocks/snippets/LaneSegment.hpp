//======================================================================
/*! \file LaneSegment.hpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief Segment of a lane
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LANESEGMENT_HPP
#define IBEOSDK_LANESEGMENT_HPP

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/datablocks/snippets/LaneSupportPoint.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <boost/unordered_map.hpp>
#include <boost/weak_ptr.hpp>

#include <ibeosdk/LaneType.hpp>

//======================================================================

namespace ibeosdk {
namespace lanes {

//======================================================================

class Lane;
class LaneSegment;

typedef boost::shared_ptr<Lane> LanePtr;
typedef boost::shared_ptr<const Lane> LaneConstPtr;
typedef boost::shared_ptr<LaneSegment> LaneSegmentPtr;
typedef boost::shared_ptr<const LaneSegment> LaneSegmentConstPtr;

//======================================================================
/*! A LaneSegment represents a \ref Lane segment within a parent Lane.
 * Each LaneSegment has a unique id within the parent \ref Lane
 *
 * A Lane Segment ends if the approximation error through straight bounding lines
 * reaches a certain value, or if a \ref CarriageWaySegment ends.
 *
 * The segmentation of a whole \ref CarriageWay is as following:
 *
 * \ref CarriageWay \htmlonly&#8594;\endhtmlonly
 * \ref CarriageWaySegment \htmlonly&#8594;\endhtmlonly
 * \ref Lane \htmlonly&#8594;\endhtmlonly
 * \ref LaneSegment
 *
 * The connection is handled using smart pointers. Therefore it is not possible
 * to create an instance of this class, but calling \ref create will return
 * a shared pointer to a new LaneSegment.
 *
 * \sa CarriageWay \sa CarriageWaySegment \sa Lane
 *
 *///-------------------------------------------------------------------
class LaneSegment : public Snippet {
public: // static methods
	static LaneSegmentPtr copy(const LaneSegmentPtr& other);

private: // constructor
	/*! \brief private default constructor called by \ref create \sa create */
	LaneSegment();

	/*! \brief private constructor called by \ref create \sa create */
	LaneSegment(const LanePtr& parent);

	/*! \brief private constructor called by \ref create \sa create */
	LaneSegment(const LaneSupportPoint& laneStart);

	/*! \brief private constructor called by \ref create \sa create */
	LaneSegment(const UINT64& id,
	            const LaneMarkingType& markingLeft, const LaneMarkingType& markingRight,
	            const BorderType& borderLeft, const BorderType& borderRight,
	            const LaneSupportPoint& laneStart,
	            const UINT64& nextId, const UINT64& prevId,
	            const UINT64& leftId, const UINT64& rightId,
	            const bool& nextInNewSeg, const bool& prevInNewSeg,
	            LanePtr parent);

public: // destructor
	virtual ~LaneSegment() {}

public:
	/*! \brief Default constructor
	 *  Initializes all elements to 0
	 *  \return A shared pointer to the created segment
	 */
	static LaneSegmentPtr create();

	/*! \brief Constructor
	 *  \param[in] parent A pointer to the parent Lane
	 *  \return A shared pointer to the created segment
	 */
	static LaneSegmentPtr create(LanePtr parent);

	/*! \brief Constructor
	 *  \param[in] laneStart The start point of the LaneSegment
	 */
	static LaneSegmentPtr create(const LaneSupportPoint& laneStart);

	/*! \brief Constructor
	 * \param[in] id The unique id of the segment within the parent Lane
	 * \param[in] markingLeft The type of the marking on the left side
	 * \param[in] markingRight The type of the marking on the right side
	 * \param[in] borderLeft The type of the border on the left side
	 * \param[in] borderRight The type of the border on the right side
	 * \param[in] laneStart The start point of the LaneSegment
	 * \param[in] nextId The id of the next LaneSegment (0 if there is none)
	 * \param[in] prevId The id of the preceeding LaneSegment (0 if there is none)
	 * \param[in] leftId The id of the left LaneSegment (0 if there is none)
	 * \param[in] rightId The id of the right LaneSegment (0 if there is none)
	 * \param[in] nextInNewSeg Information if next LaneSegment is in another CarriageWaySegment
	 * \param[in] prevInNewSeg Information if preceeding LaneSegment is in another CarriageWaySegment
	 * \param[in] parent Pointer to the parent Lane
	 */
	static LaneSegmentPtr create(const UINT64& id,
	                             const LaneMarkingType& markingLeft, const LaneMarkingType& markingRight,
	                             const BorderType& borderLeft, const BorderType& borderRight,
	                             const LaneSupportPoint& laneStart,
	                             const UINT64& nextId, const UINT64& prevId,
	                             const UINT64& leftId, const UINT64& rightId,
	                             const bool& nextInNewSeg, const bool& prevInNewSeg,
	                             LanePtr parent);

public: // setter & getter
	/*! \brief Returns the unique id of this LaneSegment within the parent Lane */
	UINT64 getId() const;

	/*! \brief Sets the Id of the lane segment */
	void setId(const UINT64& id);

	/*! \brief Returns a pointer to the following LaneSegment (0 if there is none) */
	LaneSegmentPtr getNext() const;

	/*! \brief Sets the pointer to the next LaneSegment */
	void setNext(const LaneSegmentPtr& next);

	/*! \brief Returns a pointer to the preceeding LaneSegment (0 if there is none) */
	LaneSegmentPtr getPrevious() const;

	/*! \brief Sets the pointer to the previous LaneSegment */
	void setPrevious(const LaneSegmentPtr& previous);

	/*! \brief Returns a pointer to the left LaneSegment (0 if there is none) */
	LaneSegmentPtr getLeft() const;

	/*! \brief Sets the pointer to the left LaneSegment */
	void setLeft(const LaneSegmentPtr& left);

	/*! \brief Returns a pointer to the right LaneSegment (0 if there is none) */
	LaneSegmentPtr getRight() const;

	/*! \brief Sets the pointer to the right LaneSegment */
	void setRight(const LaneSegmentPtr& right);

	/*! \brief Returns true if there is a following segment, false otherwise */
	bool hasNext() const;

	/*! \brief Returns true if there is a preceeding segment, false otherwise */
	bool hasPrevious() const;

	/*! \brief Returns true if there is a left segment, false otherwise */
	bool hasLeft() const;

	/*! \brief Returns true if there is a right segment, false otherwise */
	bool hasRight() const;

	/*! \brief Returns the pointer to the parent Lane (0 if not set) */
	LanePtr getParent() const;

	/*! \brief Sets the pointer to the parent Lane */
	void setParent(const LanePtr& parent);

	/*! \brief Return the type of the left marking */
	LaneMarkingType getLeftMarkingType() const;
	void setLeftMarkingType(const LaneMarkingType type);

	/*! Return the type of the right marking */
	LaneMarkingType getRightMarkingType() const;
	void setRightMarkingType(const LaneMarkingType type);

	/*! Returns the type of the left border */
	BorderType getLeftBorderType() const;
	void setLeftBorderType(const BorderType type);

	/*! Returns the type of the right border */
	BorderType getRightBorderType() const;
	void setRightBorderType(const BorderType type);

	/*! Returns the start point of the segment */
	LaneSupportPoint& getStartPoint();

	const LaneSupportPoint& getStartPoint() const;

	void setStartPoint(const LaneSupportPoint& point);

	/*! \brief Returns a bounding rectangle of the way which is currently not the
	 * minimal one, but the one aligned to the north vector
	 */
	BoundingRectangle getBoundingBox() const;

	/*! Returns the length of the segment */
	float getLength() const;

	/*! \brief Returns the width of the segment at the given position. The position is
	 * relative to the start of the segment and follows along the lane
	 */
	float getWidth(const float position = .0f) const;

	/*! \brief Returns the offset from the center of the end point to the center of the start point */
	Point2dFloat getEndOffset() const;

	/*! \brief Returns the offset from the left end bound to the center of the start point */
	Point2dFloat getEndOffsetLeft() const;

	/*! \brief Returns the offset from the right end bound to the center of the start point */
	Point2dFloat getEndOffsetRight() const;

	/*! \brief Returns the offset from the left start bound to the center of the start point */
	Point2dFloat getStartOffsetLeft() const;

	/*! \brief Returns the offset from the right start bound to the center of the start point */
	Point2dFloat getStartOffsetRight() const;

	/*! \brief Returns the GPS position of the end point (center) of the segment */
	PositionWgs84 getEndGps();

	UINT64 getNextId() const;
	void setNextId(const UINT64& nextId);

	UINT64 getPrevId() const;
	void setPrevId(const UINT64& prevId);

	UINT64 getLeftId() const;
	void setLeftId(const UINT64& leftId);

	UINT64 getRightId() const;
	void setRightId(const UINT64& rightId);

	bool isNextInNewSeg() const;
	void setNextInNewSeg(const bool inNewSeg);

	bool isPrevInNewSeg() const;
	void setPrevInNewSeg(const bool inNewSeg);

	void updateConnectionIDs(const LaneSegmentPtr& reference, const bool override = true);
	void updateProperties();
	void cleanIds();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

private:
	void init();
	void calculateLength();
	void calculateWidth();
	void calculateBoundingBox();
	void calculateOffsets();

public:
	/*! \brief The version of implementation */
	static std::string const VERSION;


private: // serialized members
	UINT64 m_id;
	LaneMarkingType m_markingLeft;
	LaneMarkingType m_markingRight;

	BorderType m_borderLeft;
	BorderType m_borderRight;

	LaneSupportPoint m_start;

	UINT64 m_nextId;
	UINT64 m_prevId;
	UINT64 m_leftId;
	UINT64 m_rightId;

	bool m_nextInNewSeg;
	bool m_prevInNewSeg;

private: // unserialized members
	float m_length;
	float m_endWidth;
	float m_startWidth;

	Point2dFloat m_endOffset;
	Point2dFloat m_endOffsetLeft;
	Point2dFloat m_endOffsetRight;

	boost::weak_ptr<Lane> m_parent;

	boost::weak_ptr<LaneSegment> m_nextSegment;
	boost::weak_ptr<LaneSegment> m_prevSegment;
	boost::weak_ptr<LaneSegment> m_leftSegment;
	boost::weak_ptr<LaneSegment> m_rightSegment;

	BoundingRectangle m_boundingBox;

public:
	friend class CarriageWay;
}; // class LaneSegment

//======================================================================

typedef boost::unordered_map<UINT64, LaneSegmentPtr> LaneSegmentMap;
typedef LaneSegmentMap::value_type LaneSegmentMapEntry;

//======================================================================

} // namespace lanes
} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_LANESEGMENT_HPP

//======================================================================
