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

#ifndef IBEOSDK_CARRIAGEWAY_HPP
#define IBEOSDK_CARRIAGEWAY_HPP

//==============================================================================

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/datablocks/snippets/CarriageWaySegment.hpp>
#include <ibeosdk/LaneType.hpp>

#include <boost/unordered_map.hpp>

//==============================================================================

namespace ibeosdk {
namespace lanes {

//==============================================================================

typedef boost::shared_ptr<CarriageWay> CarriageWayPtr;
typedef boost::shared_ptr<const CarriageWay> CarriageWayConstPtr;

//==============================================================================
/*! \brief A CarriageWay represents one surface of a road and has a unique identifier.
 * The identifier is a combination of the \ref CarriageWayType of  the road and a
 * number (e.g. A for \ref CarriageWayType motorway and 1 represents A1).
 *
 * In addition each CarriageWay holds a list of segments. Within one segment,
 * the number of lanes is constant. If there are preceeding and following segments,
 * these segments are linked against each other. It is therefore possible to
 * store multiple linked lists of segments within on CarriageWay (e.g. for different
 * driving directions or if there are gaps betweeb segments).
 *
 * A CarriageWay is always the highest representation of a road. The segmentation
 * is as following:
 *
 * \ref CarriageWay \htmlonly&#8594;\endhtmlonly
 * \ref CarriageWaySegment \htmlonly&#8594;\endhtmlonly
 * \ref Lane \htmlonly&#8594;\endhtmlonly
 * \ref LaneSegment
 *
 * The connection is handled using smart pointers. Therefore it is not possible
 * to create an instance of this class, but calling \ref create() will return
 * a shared pointer to a new CarriageWay.
 *
 * \sa CarriageWaySegment \sa Lane \sa LaneSegment
 *///---------------------------------------------------------------------------
class CarriageWay : public ibeosdk::Snippet {
public:
	/*! \brief Default constructor
	 * Initializes all elements to 0
	 * \return A shared pointer to the created CarriageWay */
	static CarriageWayPtr create();


	/*! \brief Constructor
	 * \param[in] id A unique id for this CarriageWay for internal identification
	 * \param[in] nationalId The national number of the road (e.g. 1 for B1 or A1)
	 * \param[in] type The type of the road (motorway, trunk, ...)
	 * \param[in] segments A map holding all CarriageWaySegments with unique identifiers
	 */
	static CarriageWayPtr create(const UINT64& id,
	                             const UINT16& nationalId,
	                             const CarriageWayType& type,
	                             const CarriageWaySegmentMap& segments= CarriageWaySegmentMap());

private: // CONSTRUCTOR
	/*! \brief private constructor called by \ref create() \sa create */
	CarriageWay();

	/*! \brief private constructor called by \ref create() \sa create */
	CarriageWay( const UINT64& id,
		const UINT16& nationalId,
		const CarriageWayType& type,
		const CarriageWaySegmentMap& segments= CarriageWaySegmentMap());

public:
	/*! \brief Version of this class */
	static const std::string VERSION;

public: // GET & SET

	/*! \brief Returns national id of this road */
	UINT16 getNationalId() const;

	/*! \brief Returns the type of the road */
	CarriageWayType getType() const;

	/*! \brief Sets the type of the road to the given parameter */
	void setType(const CarriageWayType& type);

	/*! \brief Returns all segments of the road
	 * \return A map holding pointers to all segments of the road which can be
	 *  accessed by a unique id */
	const CarriageWaySegmentMap& getSegments() const;

	/*! \brief Sets all road segments to the given list */
	void setSegments(const CarriageWaySegmentMap& segments);

	/*! \brief Returns the internally used unique id of the road */
	UINT64 getId() const;

	/*! \brief Sets the id of the CarriageWay */
	void setId(const UINT64& id);

	/*! \brief Returns a bounding rectangle of the way which is currently not the
	 * minimal one, but the one aligned to the north vector */
	BoundingRectangle getBoundingBox() const;

	//========================================

	/*! \brief Inserts a new segment to the list for this CarriageWay.
	 * \return True if the insertation is possible (id not already in the list)
	 * and false if an insertation is not possible */
	bool insert(CarriageWaySegmentPtr segment);

	/*! \brief Resolves the connections between segments, lanes, etc.
	 * This function has to be called once the segments are all filled. It
	 * generates pointers between LaneSegments, Lanes, CarriageWaySegments as well
	 * as pointers to the parent object. */
	void resolveConnections(const CarriageWayPtr& instance);
	//========================================

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

	//========================================
private:
	void init();

	//========================================

private:
	UINT64 m_id;
	UINT16 m_nationalId;

	CarriageWayType m_type;

	CarriageWaySegmentMap m_segments;

	BoundingRectangle m_boundingBox;
}; // class CarriageWay

//======================================================================

} // namespace lanes
} // namespace ibeo

//======================================================================

#endif // IBEOSDK_CARRIAGEWAY_HPP

//======================================================================
