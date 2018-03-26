//======================================================================
/*! \file BsonT_Session.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/datamodel/BsonT.hpp>
#include <ibeosdk/database/basedatamodel/Session.hpp>
#include <ibeosdk/database/CollectionName.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

const std::string BsonT<Session>::bsonTypeName = "SessionInfo";
const std::string BsonT<Session>::bsonFtSessionName = "SessionName";
const std::string BsonT<Session>::bsonFtSessionKeywords = "SessionKeywords";
const std::string BsonT<Session>::bsonFtSessionDescription = "SessionDescription";
const std::string BsonT<Session>::bsonFtSessionDate = "SessionDate";
const std::string BsonT<Session>::bsonFtSessionTrips = "SessionTrips";
const std::string BsonT<Session>::bsonFtSessionProcessings = "processings";

//======================================================================

void BsonT<Session>::createDataType(Session& data, const mongo::BSONObj& bsonObj)
{
	const std::string name = bsonObj.getField(bsonFtSessionName).String();
	data.reset(name);

	std::string stringData = bsonObj.getField(bsonFtSessionKeywords).String();
	const std::vector<std::string> keywords = MongoDbUtils::splitStrings(stringData);

	for (unsigned int kw = 0; kw < keywords.size(); ++kw) {
		if (!keywords.at(kw).empty())
			data.addKeyword(keywords.at(kw));
	} // for all keywords

	const std::string description = bsonObj.getField(bsonFtSessionDescription).String();
	data.setDescription(description);

	const mongo::Date_t date = bsonObj.getField(bsonFtSessionDate).Date();
	data.setDate(MongoDbUtils::convertToBoostTimestamp(date));

	stringData = bsonObj.getField(bsonFtSessionTrips).String();
	const std::vector<std::string> trips = MongoDbUtils::splitStrings(stringData);

	for (unsigned int t = 0; t < trips.size(); ++t) {
		if (!trips.at(t).empty())
			data.addTrip(trips.at(t));
	} // for all trips

	std::vector<mongo::BSONElement> procList = bsonObj.getField(bsonFtSessionProcessings).Array();

	std::vector<mongo::BSONElement>::const_iterator pIter = procList.begin();
	for (; pIter != procList.end(); ++pIter) {
		CollectionName cName((*pIter).dbrefNS());
		Processing newProcessing(cName, (*pIter).dbrefOID().toString());

		data.addProcessing(newProcessing);
	} // for all processings
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
