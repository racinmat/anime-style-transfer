//======================================================================
/*! \file MongoDbUtils.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/MongoDbUtils.hpp>
#include <ibeosdk/database/datamodel/BsonT.hpp>
#include <mongo/client/dbclientinterface.h>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

using namespace dbField;

//======================================================================

bool MongoDbUtils::collectionExists(mongo::DBClientConnection& client, const CollectionName& collectionName)
{
	bool collExists = false;
	try {
		collExists = client.exists(collectionName);
		if(!collExists) {
			logError << "Collection '" << collectionName <<  "' does not exists in database." << std::endl;
		}
	}
	catch (const mongo::MsgAssertionException& e) {
		logError << e.what() << std::endl;
		throw;
	}
	return collExists;
}

//======================================================================
std::string MongoDbUtils::convertFieldToName(const DbField field)
{
	switch (field)
	{
		case DbField_Id:
			return BsonBase::bsonFtId;

		case DbField_MongoId:
			return BsonBase::bsonFtMongoId;

		case DbField_Timestamp:
			return BsonBase::bsonFtTimestamp;

		case DbField_ObjectId:
			return BsonBase::bsonFtObjId;

		case DbField_Type:
			return BsonBase::bsonFtType;

		case DbField_Class:
			return BsonBase::bsonFtClass;

		case DbField_BinaryData:
			return BsonBase::bsonFtBinaryData;

		case DbField_File:
			return BsonBase::bsonFtFile;

		case DbField_X:
			return BsonBase::bsonFtX;

		case DbField_Y:
			return BsonBase::bsonFtY;

		case DbField_Z:
			return BsonBase::bsonFtZ;

		case DbField_Latitude:
			return BsonBase::bsonFtLat;

		case DbField_Longitude:
			return BsonBase::bsonFtLon;

		case DbField_Altitude:
			return BsonBase::bsonFtAlt;

		case DbField_Version:
			return BsonBase::bsonFtVersion;

		default:
			assert(false);
			return "__undefined__field ";
	};
}

//======================================================================

std::string MongoDbUtils::convertMatchCriterionToName(const DbQueryOptions::Criterion crit)
{
	switch (crit)
	{
		case DbQueryOptions::Crit_Eq:
			return "$eq"; //TODO this is for MongoDB Ver. 3.0

		case DbQueryOptions::Crit_Neq:
			return "$ne";

		case DbQueryOptions::Crit_Lt:
			return "$lt";

		case DbQueryOptions::Crit_Lte:
			return "$lte";

		case DbQueryOptions::Crit_Gt:
			return "$gt";

		case DbQueryOptions::Crit_Gte:
			return "$gte";

		default:
			assert(false);
			return "__undefined__criterion ";
	}
}

//======================================================================

mongo::BSONObj MongoDbUtils::convertReturnFields(const DbQueryOptions& options)
{
	mongo::BSONObj returnFields;
	{
		mongo::BSONObjBuilder bobReturnFields;

		const DbQueryOptions::ReturnFieldList& returnFieldList = options.getReturnFields();
		DbQueryOptions::ReturnFieldList::const_iterator itr = returnFieldList.begin();
		for (; itr != returnFieldList.end(); ++itr)
		{
			const std::string field = convertFieldToName(itr->first);
			bobReturnFields.append(field, itr->second);
		}

		returnFields = bobReturnFields.obj();
	}

	return returnFields;
}

//======================================================================

mongo::Query MongoDbUtils::convertDbQueryOptionsToQuery(const DbQueryOptions& options)
{
	mongo::BSONObjBuilder bobQuery;

	// Use DbQueryOptions criteria to build query
	const DbQueryOptions::MatchCriteria& criteria = options.getMatchCriteria();
	if (!criteria.empty())
	{
		mongo::BSONArrayBuilder babCriteria;
		DbQueryOptions::MatchCriteria::const_iterator itr = criteria.begin();
		for (; itr != criteria.end(); ++itr) {
			mongo::BSONObjBuilder bobField;
			{
				const std::string critName = MongoDbUtils::convertMatchCriterionToName(itr->m_crit);
				mongo::BSONObjBuilder bobCriterion;

				if (itr->m_val.empty()) {
					assert(false);
				}
				else if (itr->m_val.type() == typeid(UINT8)) {
					bobCriterion.append(critName, boost::any_cast<UINT8>(itr->m_val));
				}
				else if (itr->m_val.type() == typeid(UINT16)) {
					bobCriterion.append(critName, boost::any_cast<UINT16>(itr->m_val));
				}
				else if (itr->m_val.type() == typeid(UINT32)) {
					bobCriterion.append(critName, boost::any_cast<UINT32>(itr->m_val));
				}
				else if (itr->m_val.type() == typeid(UINT64)) {
					bobCriterion.append(critName, (long long)boost::any_cast<UINT64>(itr->m_val));
				}
				else if (itr->m_val.type() == typeid(INT8)) {
					bobCriterion.append(critName, boost::any_cast<INT8>(itr->m_val));
				}
				else if (itr->m_val.type() == typeid(INT16)) {
					bobCriterion.append(critName, boost::any_cast<INT16>(itr->m_val));
				}
				else if (itr->m_val.type() == typeid(INT32)) {
					bobCriterion.append(critName, boost::any_cast<INT32>(itr->m_val));
				}
				else if (itr->m_val.type() == typeid(INT64)) {
					bobCriterion.append(critName, (long long)boost::any_cast<INT64>(itr->m_val));
				}
				else if (itr->m_val.type() == typeid(float)) {
					bobCriterion.append(critName, boost::any_cast<float>(itr->m_val));
				}
				else if (itr->m_val.type() == typeid(double)) {
					bobCriterion.append(critName, boost::any_cast<double>(itr->m_val));
				}
				else if (itr->m_val.type() == typeid(boost::posix_time::ptime)) {
					bobCriterion.append(critName, convertToMongoTimestamp(boost::any_cast<const boost::posix_time::ptime&>(itr->m_val)));
				}
				else if (itr->m_val.type() == typeid(NTPTime)) {
					bobCriterion.append(critName, convertToMongoTimestamp((boost::any_cast<const NTPTime&>(itr->m_val)).toPtime()));
				}
				else if (itr->m_val.type() == typeid(std::string)) {
					bobCriterion.append(critName, boost::any_cast<std::string>(itr->m_val));
				}
				else {
					assert(false);
				}

				mongo::BSONObj objCriterion = bobCriterion.obj();
				const std::string fieldName = MongoDbUtils::convertFieldToName(itr->m_field);
				bobField.append(fieldName, objCriterion);
			}

			mongo::BSONObj objField = bobField.obj();
			babCriteria.append(objField);
		}
		mongo::BSONArray arrCriteria = babCriteria.arr();

		bobQuery.appendArray("$and", arrCriteria);
	}

	mongo::BSONObj objQuery = bobQuery.obj();

	return mongo::Query(objQuery);
}

//======================================================================

mongo::Query MongoDbUtils::convertDbQueryOptionsToFullQuery(const DbQueryOptions& options)
{
	mongo::BSONObj subObjQuery = convertDbQueryOptionsToQuery(options).obj;

	mongo::BSONObj subObjOrderBy;

	{
		mongo::BSONObjBuilder bobOrderBy;

		const DbQueryOptions::OrderByList& orderByList = options.getOrderBy();
		DbQueryOptions::OrderByList::const_iterator itr = orderByList.begin();
		for (; itr != orderByList.end(); ++itr) {
			const std::string field = convertFieldToName(itr->first); // TODO Check for invalid fields
			// Mongo expects "FieldName : 1" for ascending, and "FieldName : -1" for descending
			bobOrderBy.append(field, itr->second ? INT32(1) : INT32(-1));
		}

		subObjOrderBy = bobOrderBy.obj();
	}

	mongo::BSONObjBuilder bobQuery;
	bobQuery.append("query", subObjQuery);
	bobQuery.append("orderby", subObjOrderBy);

	return mongo::Query(bobQuery.obj());
}

//======================================================================

DbQueryOptions MongoDbUtils::createRangeQueryOption(const NTPTime& startTime, const NTPTime& endTime,
                                      const DbDataBlockReader::StartRangeMode startRangeMode,
                                      const DbDataBlockReader::EndRangeMode endRangeMode)
{
	DbQueryOptions options;

	if (startRangeMode == DbDataBlockReader::StartRangeInclusive) {
		options.minEquals(DbField_Timestamp, startTime);
	}
	else { // StartRangeExclusive
		options.min(DbField_Timestamp, startTime);
	}

	if (endRangeMode == DbDataBlockReader::EndRangeInclusive) {
		options.maxEquals(DbField_Timestamp, endTime);
	}
	else { // EndRangeExclusive
		options.max(DbField_Timestamp, endTime);
	}

	return options;
}

//======================================================================

mongo::Date_t MongoDbUtils::convertToMongoTimestamp(const boost::posix_time::ptime& pt)
{
	boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
	boost::posix_time::time_duration timediff = pt - epoch;
	mongo::Date_t timestamp(timediff.total_milliseconds());

	return timestamp;
}

//======================================================================

boost::posix_time::ptime MongoDbUtils::convertToBoostTimestamp(const mongo::Date_t& mt)
{
	boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
	boost::posix_time::time_duration timediff = boost::posix_time::milliseconds(mt.millis);
	boost::posix_time::ptime pt(epoch + timediff);

	return pt;
}

//======================================================================

std::string MongoDbUtils::createDefaultTimestampQuery(const ScanEcu& scan, unsigned int maxOffsetMillis)
{
	boost::posix_time::ptime start = scan.getStartTimestamp().toPtime();
	boost::posix_time::ptime end = (scan.getStartTimestamp() + NTPTime(scan.getEndTimeOffset())).toPtime();
	boost::posix_time::time_duration dt = end - start;
	if (dt.total_milliseconds() > maxOffsetMillis) {
		end = start + boost::posix_time::milliseconds(maxOffsetMillis);
	}

	std::string query = createDefaultTimestampQuery(start, end);

	return query;
}

//======================================================================

std::string MongoDbUtils::createDefaultTimestampQuery(const boost::posix_time::ptime startTsInUTC,
                                                      const boost::posix_time::time_duration timeSpan)
{
	return createDefaultTimestampQuery(startTsInUTC, startTsInUTC+timeSpan);
}

//======================================================================

std::string MongoDbUtils::createDefaultTimestampQuery(const boost::posix_time::ptime start, const boost::posix_time::ptime end)
{
	return createTimestampQuery(start, end).toString();
}

//======================================================================

mongo::BSONObj MongoDbUtils::createTimestampQuery(const boost::posix_time::ptime atTime)
{
	mongo::Date_t mAtTime = MongoDbUtils::convertToMongoTimestamp(atTime);

	mongo::BSONObjBuilder bob;
	bob.appendDate("Timestamp", mAtTime);
	return bob.obj(); // attention, after calling obj(), bob is empty.
}

//======================================================================

mongo::BSONObj MongoDbUtils::createTimestampQuery(const boost::posix_time::ptime start, const boost::posix_time::ptime end)
{
	const mongo::Date_t mStartTime = MongoDbUtils::convertToMongoTimestamp(start);
	const mongo::Date_t mEndTime = MongoDbUtils::convertToMongoTimestamp(end);

	mongo::BSONObjBuilder bob;
	mongo::BSONObjBuilder sub(bob.subobjStart("Timestamp"));
	sub.appendDate("$gte", mStartTime);
	sub.appendDate("$lte", mEndTime);
	sub.done();
	return bob.obj(); // attention, after calling obj(), bob is empty.
}

//======================================================================

mongo::BSONObj MongoDbUtils::createFieldQuery(const std::string& field)
{
	mongo::BSONObjBuilder bob;
	bob.append(field, 1);
	return bob.obj(); // attention, after calling obj(), bob is empty.
}

//======================================================================

std::string MongoDbUtils::createDefaultIdQuery(const int startId, const int endId)
{
	std::stringstream query;
	query << "{ \"ID\": " << mongo::BSONObjBuilder().append("$gte", startId).append("$lte", endId).obj() << "}";

	return query.str();
}

//======================================================================

std::string MongoDbUtils::combineStrings(const std::vector<std::string>& data)
{
	std::string combined;
	std::vector<std::string>::const_iterator it = data.begin();
	for (; it != data.end(); ++it) {
		combined += *it + ";";
	}
	return combined;
}

//======================================================================

std::vector<std::string> MongoDbUtils::splitStrings(const std::string& data)
{
	std::vector<std::string> splitted;
	boost::split(splitted, data, boost::is_any_of(";"));
	return splitted;
}

//======================================================================

} // dbaccess
} // ibeosdk

//======================================================================
