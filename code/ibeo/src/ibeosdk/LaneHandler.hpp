//======================================================================
/*! \file LaneHandler.hpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief LaneHandler which implements functionality for the CarriageWay / Lane
 *        linked lists
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LANEHANDLER_HPP
#define IBEOSDK_LANEHANDLER_HPP

#include <ibeosdk/datablocks/snippets/LaneSegment.hpp>
#include <ibeosdk/datablocks/snippets/CarriageWayList.hpp>
#include <ibeosdk/datablocks/snippets/PositionWgs84.hpp>
#include <ibeosdk/datablocks/VehicleStateBasicEcu2808.hpp>
#include <ibeosdk/TransformationMatrix2dFloat.hpp>
#include <ibeosdk/Line2dFloat.hpp>

//=============================================================================

namespace ibeosdk{
namespace lanes{

//=============================================================================

/*! \brief Structure with information about offset to a laneSegement center */
struct LanePosition{
	LaneSegmentPtr laneSegment;                            /*! < Pointer to the corresponding \ref LaneSegment */
	TransformationMatrix2dFloat transformationMatrix;      /*! < The position and heading relative to the start of the segment */
	float lateralDistance;                                 /*! < The lateral distance of to the center line of the \ref LaneSegment */
	float longitudinalDistance;                            /*! < The longitudinal distance to the start of the \ref LaneSegment */
	PositionWgs84 gpsPosition;                             /*! < The WGS84 position */
}; // LanePosition

//=============================================================================

/*! \brief
 */
//======================================================================
/*!\class LaneHandler
 * \brief This class provides functionality for handling Lanes.
 *        It can be used to find the corresponding LaneSegment for a given gps point
 *        and calculates offsets to the center of the LaneSegment.
 * \author Stefan Kaufmann (stk)
 * \version 0.1
 * \date Oct 9, 2014
 *///-------------------------------------------------------------------
class LaneHandler{
public:
	//========================================
	// CONSTRUCTOR

	/*! \brief Constructor
	 * \param[in] ways A list holding CarriageWays
	 */
	LaneHandler( const CarriageWayList& ways);

	/** default Constructor */
	LaneHandler();

//=============================================================================
// GET & SET

	/*! \brief Used to find a \ref LaneSegment by gps coordinates of a given point and
	 * returns information about distance to center of lane for the point
	 * and distance from start of the segment along the lane to the point
	 * \param[in] point The gps point */
	LanePosition getLanePosition(const PositionWgs84& point) const;

	/*! \brief Used to find a \ref LaneSegment from a vehicle state.
	 * \attention Only valid, if the \ref CarriageWay was created with reference
	 * GPS(0,0,0).
	 *
	 * Returns information about distance to center of lane for the point
	 * and distance from start of the segment along the lane to the point
	 * \param[in] point The gps point */
	LanePosition getLanePosition(const VehicleStateBasicEcu2808& point) const;

	/*! \brief Calculates the LanePosition of system given relative to a reference system
	 * \param[in] reference The system of the reference position (e.g. the ego position obtained from getLanePosition)
	 * \param[in] relativePosition The relative system for which the LanePosition shall be obtained
	 * \param[out] insideReferenceLane Is set true, if the relative system is within the same lane as the reference system,
	 * false otherwise
	 * \param[out] success Is set true, if the calculation was successful, false otherwise.
	 */
	LanePosition getReferenceLanePosition(const LanePosition& reference, const TransformationMatrix2dFloat& relativePosition,
																				bool& insideReferenceLane, bool& success) const;

	/*! \brief Calculates a transformationMatrix relative to the reference system by giving a longitudinal and lateral distance
		* \param[in] reference The reference LanePosition
		* \param[in] longitudinalDistance The longitudinal distance relative from the reference
		* \param[in] lateralDistance The lateral distance relative from the reference
		* \param[out] success Set to true, if successful, false otherwise */
	TransformationMatrix2dFloat getRelativePosition(const LanePosition& reference, const float& longitudinalDistance, const float& lateralDistance, bool& success) const;

//=============================================================================

private:
	LanePosition calculateLanePosition(const LaneSegmentPtr& segment, const PositionWgs84& point, bool& insideIntersection) const;

//=============================================================================

private:
	CarriageWayList m_ways;
}; // class LaneHandler

//=============================================================================

} // namespace lanes
} // namespace ibeo

//=============================================================================

#endif // IBEOSDK_LANEHANDLER_HPP

//=============================================================================
