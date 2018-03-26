//======================================================================
/*! \file BsonT_Trip.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/datamodel/BsonT.hpp>
#include <ibeosdk/database/basedatamodel/Trip.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

const std::string BsonT<Trip>::bsonFtTripName = "TripName";
const std::string BsonT<Trip>::bsonFtTripFiles = "TripFiles";
const std::string BsonT<Trip>::bsonFtTripKeywords = "TripKeywords";
const std::string BsonT<Trip>::bsonFtTripDescription = "TripDescription";
const std::string BsonT<Trip>::bsonFtTripDate = "TripDate";

//======================================================================

void BsonT<Trip>::createTripInfo(Trip& trip,
                                 const mongo::BSONObj& bsonObj)
{
	const std::string name = bsonObj.getField(bsonFtTripName).String();
	trip.reset(name);

	std::string data = bsonObj.getField(bsonFtTripFiles).String();
	const std::vector<std::string> files = MongoDbUtils::splitStrings(data);

	for (unsigned int f = 0; f < files.size(); ++f) {
		if (!files.at(f).empty())
			trip.addIdcPath(files.at(f));
	} // for all files

	data = bsonObj.getField(bsonFtTripKeywords).String();
	const std::vector<std::string> keywords = MongoDbUtils::splitStrings(data);

	for (unsigned int kw = 0; kw < keywords.size(); ++kw) {
		if (!keywords.at(kw).empty())
			trip.addKeyword(keywords.at(kw));
	} // for keywords

	const std::string description = bsonObj.getField(bsonFtTripDescription).String();
	trip.setDescription(description);

	const mongo::Date_t date = bsonObj.getField(bsonFtTripDate).Date();
	trip.setDate(MongoDbUtils::convertToBoostTimestamp(date));
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
