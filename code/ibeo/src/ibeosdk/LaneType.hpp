//======================================================================
/*! \file LaneType.hpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief Definition of structs and enums for carriageways and lanes
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LANETYPE_HPP
#define IBEOSDK_LANETYPE_HPP

//======================================================================

#include <ibeosdk/datablocks/snippets/PositionWgs84.hpp>

#include <map>

//======================================================================

namespace ibeosdk{
namespace lanes {

//======================================================================
/*! \brief Enumeration with different CarriageWay types
 *///-------------------------------------------------------------------
enum CarriageWayType
{
	CWT_MOTORWAY,     //!< Used for highways
	CWT_TRUNK,        //!< Used for highway similar roads (Germany: Autobahnaehnliche Strasse, umgangssprachlich: Schnellstraße)
	CWT_PRIMARY,      //!< Used for primary roads (Germany: Bundesstrasse)
	CWT_SECONDARY,    //!< Used for secondary roads (Germany: Landesstrasse)
	CWT_TERTIARY,     //!< Used for tertiary roads (Germany: Kreisstrasse)
	CWT_RESIDENTIAL,  //!< Used for residential roads (Germany: Wohngebiet)
	CWT_SERVICE,      //!< Used for service roads (Germany: Zufahrtsstrasse, Seitenstrasse)
	CWT_UNCLASSIFIED  //!< Used if road is not classifiable
};

//======================================================================
/*! \brief Enumeration for Lane types
 *///-------------------------------------------------------------------
enum LaneType
{
	LT_FORWARD,       //!< Driving direction is forward
	LT_BACKWARD,      //!< Driving direction id backward
	LT_EXIT,          //!< Exit Lane (Germany: Ausfahrt)
	LT_ONCOMING,      //!< Oncoming Lane (German: Beschleunigungsspur)
	LT_BREAKDOWN,     //!< Breakdown Lane (Germany: Standspur)
	LT_UNCLASSIFIED   //!< Not classifiable
};

//======================================================================
/*! \brief Enumeration for Lane Markings
 *///-------------------------------------------------------------------

enum LaneMarkingType
{
	LMT_SOLID,
	LMT_DASHED,
	LMT_UNCLASSIFIED,
	LMT_NONE
};

//======================================================================
/*! \brief Enumeration for Border types
 *///-------------------------------------------------------------------
enum BorderType
{
	BT_LANE_CHANGE_POSSIBLE,      //!< A neighbouring lane exist and a Lane change is possible
	BT_LANE_NO_CHANGE_POSSIBLE,   //!< A neighbouring lane exist but no Lane change is possible (e.g. due to guard rails)
	BT_ONCOMING_LANE,             //!< An oncoming Lane
	BT_END_OF_STREET_SOLID,       //!< End of the road of solid nature (e.g. guard rail, buildings)
	BT_END_OF_STREET_EARTH,       //!< End of the road of planar and accessible nature (e.g. earth, asphalt)
	BT_UNCLASSIFIED               //!< unclassified border
};

//======================================================================
/*! \brief Structure for holding a bounding rectangle in geographic coordinates
 *///-------------------------------------------------------------------
struct BoundingRectangle {
public:
	/*! \brief constructor which initializes with virtual coordinates which are
	 * out of range (+-1000) so the will be overwritten when checking real boundaries
	 */
	BoundingRectangle()
	  : minLongitude(1000),
	    maxLongitude(-1000),
	    minLatitude(1000),
	    maxLatitude(-1000)
	{}

public:
	bool checkInside(const PositionWgs84& point) const
	{
		return (   point.getLatitudeInDeg()  <= this->maxLatitude
				&& point.getLatitudeInDeg()  >= this->minLatitude
				&& point.getLongitudeInDeg() >= this->minLongitude
				&& point.getLongitudeInDeg() <= this->maxLongitude);
	}

public:
	double minLongitude; //!< minimal longitude position in degrees
	double maxLongitude; //!< maximal longitude position in degrees
	double minLatitude;  //!< minimal latitude position in degrees
	double maxLatitude;  //!< maximal latitude position in degrees
}; // BoundingRectangle

//======================================================================
/*! \brief Constructor for std::map initialization, since const private
 *         maps need to be initialized by Constructor or function within
 *         C++98 standard.
 *///-------------------------------------------------------------------
struct MapConstructor {
public:
	/*! \brief Creates the map from std::string to \ref CarriageWayType */
	static std::map<std::string, CarriageWayType> createCWTMap()
	{
		std::map<std::string, CarriageWayType> map;
		map["motorway"]     = CWT_MOTORWAY;
		map["trunk"]        = CWT_TRUNK;
		map["primary"]      = CWT_PRIMARY;
		map["secondary"]    = CWT_SECONDARY;
		map["tertiary"]     = CWT_TERTIARY;
		map["residential"]  = CWT_RESIDENTIAL;
		map["service"]      = CWT_SERVICE;
		map["unclassified"] = CWT_UNCLASSIFIED;
		return map;
	}

	/*! \brief Creates the map from std::string to \ref LaneType */
	static std::map<std::string, LaneType> createLTMap()
	{
		std::map<std::string, LaneType> map;
		map["forward"]      = LT_FORWARD;
		map["backward"]     = LT_BACKWARD;
		map["exit"]         = LT_EXIT;
		map["oncoming"]     = LT_ONCOMING;
		map["breakdown"]    = LT_BREAKDOWN;
		map["unclassified"] = LT_UNCLASSIFIED;
		return map;
	}

	/*! \brief Creates the map from std::string to \ref LaneMarkingType */
	static std::map<std::string, LaneMarkingType> createLMTMap()
	{
		std::map<std::string, LaneMarkingType> map;
		map["unclassified"]   = LMT_UNCLASSIFIED;
		map["solid"]          = LMT_SOLID;
		map["dashed"]         = LMT_DASHED;
		return map;
	}

	/*! \brief Creates a map from std::string to \ref BorderType */
	static std::map<std::string, BorderType> createBorderMap()
	{
		std::map<std::string, BorderType> map;
		map["unclassified"]         = BT_UNCLASSIFIED;
		map["lanechangepossible"]   = BT_LANE_CHANGE_POSSIBLE;
		map["lanenochangepossible"] = BT_LANE_NO_CHANGE_POSSIBLE;
		map["oncominglane"]         = BT_ONCOMING_LANE;
		map["endofstreetsolid"]     = BT_END_OF_STREET_SOLID;
		map["endofstreetearth"]     = BT_END_OF_STREET_EARTH;
		return map;
	}

	/*! \brief Creates a map from std::string to bool*/
	static std::map<std::string, bool> createBoolMap()
	{
		std::map<std::string, bool> map;
		map["true"]   = true;
		map["false"]  = false;
		return map;
	}

	/*! \brief Creates a map from \ref CarriageWayType to std::string */
	static std::map<CarriageWayType, std::string> createFromCWTMap()
	{
		std::map<CarriageWayType, std::string> map;
		map[CWT_MOTORWAY]     = "motorway";
		map[CWT_TRUNK]        = "trunk";
		map[CWT_PRIMARY]      = "primary";
		map[CWT_SECONDARY]    = "secondary";
		map[CWT_TERTIARY]     = "tertiary";
		map[CWT_RESIDENTIAL]  = "residential";
		map[CWT_SERVICE]      = "service";
		map[CWT_UNCLASSIFIED] = "unclassified";
		return map;
	}

	/*! \brief Creates a map from \ref LaneType to std::string */
	static std::map<LaneType, std::string> createLTToStringMap()
	{
		std::map<LaneType, std::string> map;
		map[LT_FORWARD]      = "forward";
		map[LT_BACKWARD]     = "backward";
		map[LT_EXIT]         = "exit";
		map[LT_ONCOMING]     = "oncoming";
		map[LT_BREAKDOWN]    = "breakdown";
		map[LT_UNCLASSIFIED] = "unclassified";
		return map;
	}

	/*! \brief Creates a map from \ref LaneMarkingType to std::string */
	static std::map<LaneMarkingType, std::string> createLMTToStringMap()
	{
		std::map<LaneMarkingType, std::string> map;
		map[LMT_UNCLASSIFIED] = "unclassified";
		map[LMT_SOLID]        = "solid";
		map[LMT_DASHED]       = "dashed";
		return map;
	}

	/*! \brief Creates a map from \ref BorderType to std::string */
	static std::map<BorderType, std::string> createBorderToStringMap()
	{
		std::map<BorderType, std::string> map;
		map[BT_UNCLASSIFIED]             = "unclassified";
		map[BT_LANE_CHANGE_POSSIBLE]     = "lanechangepossible";
		map[BT_LANE_NO_CHANGE_POSSIBLE]  = "lanenochangepossible";
		map[BT_ONCOMING_LANE]            = "oncominglane";
		map[BT_END_OF_STREET_SOLID]      = "endofstreetsolid";
		map[BT_END_OF_STREET_EARTH]      = "endofstreetearth";
		return map;
	}

	/*! \brief Creates a map from bool to std::string */
	static std::map<bool, std::string> createFromBoolMap()
	{
		std::map<bool, std::string> map;
		map[true] 	= "true";
		map[false] 	= "false";
		return map;
	}
}; // MapConstructor

//======================================================================

typedef MapConstructor mapConstructor; // for backwards compatiblitly

//======================================================================

/*! \brief Maps a std::string to a \ref CarriageWayType */
const std::map<std::string, CarriageWayType> CarriageWayTypeFromStringMap =
	MapConstructor::createCWTMap();

/*! \brief Maps a \ref CarriageWayType to a std::string */
const std::map<CarriageWayType, std::string> CarriageWayTypeToStringMap =
	MapConstructor::createFromCWTMap();

/*! \brief Maps a std::string to a \ref LaneType */
const std::map<std::string, LaneType> LaneTypeFromStringMap =
	MapConstructor::createLTMap();

/*! \brief Maps a \ref LaneType to a std::string */
const std::map<LaneType,std::string> LaneTypeToStringMap =
	MapConstructor::createLTToStringMap();

/*! \brief Maps std::string to a \ref LaneMarkingType */
const std::map<std::string, LaneMarkingType> LaneMarkingTypeFromStringMap =
	MapConstructor::createLMTMap();

/*! \brief Maps a \ref LaneMarkingType to a std::tring */
const std::map<LaneMarkingType, std::string> LaneMarkingTypeToStringMap =
	MapConstructor::createLMTToStringMap();

/*! \brief Maps a std::string to a \ref BorderType */
const std::map<std::string, BorderType> BorderTypeFromStringMap =
	MapConstructor::createBorderMap();

/*! \brief Maps a \ref BorderType to a std::string */
const std::map<BorderType, std::string> BorderTypeToStringMap =
	MapConstructor::createBorderToStringMap();

/*! \brief Maps a std::string to a bool value */
const std::map<std::string, bool> BoolFromStringMap =
	MapConstructor::createBoolMap();

/*! \brief Maps a bool to a std::string */
const std::map<bool, std::string> BoolToStringMap =
	MapConstructor::createFromBoolMap();

//======================================================================

} // namespace lanes
} // namespace ibeo

//======================================================================

#endif // IBEOSDK_LANETYPE_HPP

//======================================================================
