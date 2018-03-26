//======================================================================
/*! \file MongoDbUtils.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef MONGODBUTILS_HPP_SEEN
#define MONGODBUTILS_HPP_SEEN

//======================================================================

#include <ibeosdk/database/CollectionName.hpp>
#include <ibeosdk/database/DbQueryOptions.hpp>
#include <ibeosdk/devices/database/DbDataBlockReader.hpp>

#include <ibeosdk/datablocks/ScanEcu.hpp>
#include <ibeosdk/datablocks/ObjectListEcuEtDyn.hpp>

#ifdef __linux__
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wextra"
#endif // _linux
#ifdef _WIN32
#include <WinSock2.h>
#include <windows.h>
#endif // _WIN32
#include <mongo/client/dbclient.h>
#ifdef __linux__
#	pragma GCC diagnostic pop
#endif // _linux

#include <boost/date_time.hpp>
#include <boost/algorithm/string.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================
/*!\class MongoDbUtils
 * \brief Util class to support the handling with DbQueryOptions.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Mar 9, 2016
 *///------------------------------------------------------------------
class MongoDbUtils {

public:
	//========================================
	/*!\brief Checks the collection existence inside the database.
	 *
	 * \param[in] client          The connection to the database.
	 * \param[in] collectionName  The collection to check.
	 * \return  \c true if collection exists.
	 *          \c false otherwise.
	 *///-------------------------------------
	static bool collectionExists(mongo::DBClientConnection& client, const CollectionName& collectionName);

public:
	//========================================
	/*!\brief Returns the MongoDB field name for the specified field.
	 *
	 * \param[in] field Dbfield to convert.
	 * \return The DbField as a std::string
	 *///-------------------------------------
	static std::string convertFieldToName(const dbField::DbField);

	//========================================
	/*!\brief Returns the MongoDB comparison operator for a specified criterion.
	 *
	 * Supported operators: $eq, $ne, $lt, §lte, $gt, $gte
	 *
	 * \note The '$eq' query selector was introduced in MongoDB version 3.0.
	 *       For versions <3.0 don't use DbQueryOptions::match. Build up
	 *       a standard query instead.
	 *
	 * \param[in] crit DbQueryOptions::Criterion to convert.
	 * \return The MongoDB operator as a std::string
	 *///-------------------------------------
	static std::string convertMatchCriterionToName(const DbQueryOptions::Criterion crit);

	//========================================
	/*!\brief Returns a mongo::BSONObj including the return fields (includes and excludes).
	 *
	 * \param[in] options The DbQueryOptions which will be converted to a mongo::BSONObj.
	 * \return The  created options as mongo::BSONObj
	 *///-------------------------------------
	static mongo::BSONObj convertReturnFields(const DbQueryOptions& options);

	//========================================
	/*!\brief Returns a mongo::Query including only query criteria.
	 *
	 * \param[in] options The DbQueryOptions which will be converted to a mongo query.
	 * \return The converted options as mongo::Query.
	 *///-------------------------------------
	static mongo::Query convertDbQueryOptionsToQuery(const DbQueryOptions& options);

	//========================================
	/*!\brief Returns a mongo::Query including both query criteria and order-by.
	 *
	 * \param[in] options The DbQueryOptions which will be converted to a mongo::Query.
	 * \return The created options as mongo::Query
	 *///-------------------------------------
	static mongo::Query convertDbQueryOptionsToFullQuery(const DbQueryOptions& options);

	//========================================
	/*!\brief Returns a DbQueryOptions including a time range depending on range modes.
	 *
	 * \param[in] startTime       The begin of the time range.
	 * \param[in] endTime         The end of the time range.
	 * \param[in] startRangeMode  Tells the query whether to include
	 *                            or exclude the start time in or from the
	 *                            time interval to query for.
	 * \param[in] endRangeMode    Tells the query whether to include
	 *                            or exclude the end time in or from the
	 *                            time interval to query for.
	 * \return The created DbQueryOptions object.
	 *///-------------------------------------
	static DbQueryOptions createRangeQueryOption(const NTPTime& startTime, const NTPTime& endTime,
	                                             const DbDataBlockReader::StartRangeMode startRangeMode,
	                                             const DbDataBlockReader::EndRangeMode endRangeMode);

public:
	//========================================
	/*!\brief Converts a boost ptime (in UTC) to mongo::Date_t
	 *
	 * \param[in] pt The boost ptime which will be converted to a mongo::Date_t.
	 * \return The mongo::Date_t created from the boost ptime.
	 *///-------------------------------------
	static mongo::Date_t convertToMongoTimestamp(const boost::posix_time::ptime& pt);

	//========================================
	/*!\brief Converts a mongo::Date_t (in UTC) to boost date.
	 *
	 * \param[in] mt The mongo::Date_t which will be converted to a boost date.
	 * \return The boost date created from the mongo::Date_t.
	 *///-------------------------------------
	static boost::posix_time::ptime convertToBoostTimestamp(const mongo::Date_t& mt);

public:
	//========================================
	/*!\brief Creates a std::string for querying  on 'Timestamp' field.
	 *
	 * Uses MongoDbUtils::createDefaultTimestampQuery(boost::posix_time::ptime, boost::posix_time::ptime).
	 *
	 * \param[in] scan             A ScanEcu object to use. Using ScanEcu::getStartTimestamp and
	 *                             ScanEcu::getEndTimeOffset, a start and end time stamp will
	 *                             be extracted.
	 * \param[in] maxOffsetMillis  Sets a maximum offset limit (milliseconds).
	 * \return Created std::string.
	 *///-------------------------------------
	static std::string createDefaultTimestampQuery(const ScanEcu& scan, unsigned int maxOffsetMillis = 40);

	//========================================
	/*!\brief Creates a std::string for querying  on 'Timestamp' field.
	 *
	 * Uses MongoDbUtils::createDefaultTimestampQuery(boost::posix_time::ptime, boost::posix_time::ptime).
	 *
	 * \param[in] start     A time stamp, that marks the minimum (time as to be in UTC).
	 * \param[in] timeSpan  A time duration to add to the start.
	 * \return Created std:.string.
	 *///-------------------------------------
	static std::string createDefaultTimestampQuery(const boost::posix_time::ptime start,
	                                               const boost::posix_time::time_duration timeSpan
	                                                   = boost::posix_time::milliseconds(13));

	//========================================
	/*!\brief Creates a std::string for querying  on 'Timestamp' field.
	 *
	 * Uses MongoDbUtils::createTimestampQuery(boost::posix_time::ptime, boost::posix_time::ptime).
	 *
	 * \param[in] start  A time stamp, that marks the minimum (time as to be in UTC).
	 * \param[in] end    A time stamp, that marks the maximum (time as to be in UTC).
	 * \return Created std:.string.
	 *///-------------------------------------
	static std::string createDefaultTimestampQuery(const boost::posix_time::ptime start,
	                                               const boost::posix_time::ptime end);

	//========================================
	/*!\brief Creates a mongo::BSONObj for querying  on 'Timestamp' field.
	 *
	 * \param[in] atTime A time stamp, the documents has to match (time as to be in UTC).
	 * \return Created mongo::BSONObj.
	 *///-------------------------------------
	static mongo::BSONObj createTimestampQuery(const boost::posix_time::ptime atTime);

	//========================================
	/*!\brief Creates a mongo::BSONObj for querying  on 'Timestamp' field.
	 *
	 * \param[in] start  A time stamp, that marks the minimum (time as to be in UTC).
	 * \param[in] end    A time stamp, that marks the maximum (time as to be in UTC).
	 * \return Created mongo::BSONObj.
	 *///-------------------------------------
	static mongo::BSONObj createTimestampQuery(const boost::posix_time::ptime start,
	                                           const boost::posix_time::ptime end);

public:
	//========================================
	/*!\brief Creates a mongo::BSONObj for querying with given field.
	 *
	 * The resulting mongo::BSONObj has the form: '{ field : 1}'.
	 *
	 * \param[in] field Field name as a string.
	 * \return Created mongo::BSONObj with included field.
	 *///-------------------------------------
	static mongo::BSONObj createFieldQuery(const std::string& field);

	//========================================
	/*!\brief Creates a std::string for querying on 'ID' field.
	 *
	 * The range starts with startId and ends with endId (both are included).
	 *
	 * \param[in] startId  An identifier value that marks the minimum.
	 * \param[in] endId    An identifier value that marks the maximum.
	 * \return Query as std::string.
	 *///-------------------------------------
	static std::string createDefaultIdQuery(const int startId, const int endId);

public:
	//========================================
	/*!\brief Combine a vector of strings into one string separated by ';'
	 *
	 * \param[in] data The string vector which will be converted.
	 * \return The string created from the vector.
	 *///-------------------------------------
	static std::string combineStrings(const std::vector<std::string>& data);

	//========================================
	/*!\brief Splits a string into a vector separated by ';'.
	 *
	 * \param[in] data The string to split.
	 * \return The vector created from the string.
	 *///-------------------------------------
	static std::vector<std::string> splitStrings(const std::string& data);
}; // MongoDbUtils

//======================================================================

} // dbaccess
} // ibeosdk

//======================================================================

#endif // MONGODBUTILS_HPP_SEEN

//======================================================================
