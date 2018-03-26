//======================================================================
/*! \file BsonT_Trip.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef BSONT_TRIP_HPP_SEEN
#define BSONT_TRIP_HPP_SEEN

//======================================================================

#ifndef BSONT_HPP_SEEN
#	error "Must be include by BsonT.hpp"
#endif // BSONT_HPP_SEEN

//======================================================================

namespace ibeosdk {
namespace dbaccess {
class Trip;

//======================================================================
/*!\class BsonT<Trip>
 * \brief BsonT representation for a Trip.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
template<>
class BsonT<Trip> : public BsonBase
{
public:
	//========================================
	/*!\brief Converts a BSON object to a Trip object.
	 *
	 * \param[in,out] trip The Trip to fill.
	 * \param[in] bsonObj The BSON object from the database query.
	 *///-------------------------------------
	static void createTripInfo(Trip& trip,
	                           const mongo::BSONObj& bsonObj);

public:
	//========================================
	/*!\brief Database field for the trip name.
	 *///-------------------------------------
	static const std::string bsonFtTripName;

	//========================================
	/*!\brief Database field for the trip files.
	 *///-------------------------------------
	static const std::string bsonFtTripFiles;

	//========================================
	/*!\brief Database field for the trip keywords.
	 *///-------------------------------------
	static const std::string bsonFtTripKeywords;

	//========================================
	/*!\brief Database field for the trip description.
	 *///-------------------------------------
	static const std::string bsonFtTripDescription;

	//========================================
	/*!\brief Database field for the trip date.
	 *///-------------------------------------
	static const std::string bsonFtTripDate;
}; // BsonT<Trip>

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // BSONT_TRIP_HPP_SEEN

//======================================================================
