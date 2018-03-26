//======================================================================
/*! \file VehicleStateDbReader.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 17, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/database/VehicleStateDbReader.hpp>

#include <ibeosdk/database/DbConnectionFactory.hpp>
#include <ibeosdk/database/DbQueryOptions.hpp>
#include <ibeosdk/datablocks/ScanEcu.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

using namespace dbaccess;
using namespace dbField;

//======================================================================

VehicleStateDbReader::VehicleStateDbReader()
  : DbDataBlockReader(ProcessingJob::JobType_VehicleState),
    m_dbInfProcVhs(),
    m_itrProcVhs()
{}

//======================================================================

bool VehicleStateDbReader::connectToDbInterface(const CollectionName& collection)
{
	DbConnectionFactory::ScopedConnection sc;
	bool collectionCheck = false;

//	bool indexCreation = false;
	if (m_useProcessedData) {
		m_dbInfProcVhs = sc.getDbIfPtr<VehicleStateBasicEcu2808>(collection, collectionCheck);
//		indexCreation = m_dbInfProcVhs->createIndex(collection, BsonBase::bsonFtTimestamp);
	}
	else {
		m_dbInfOnlineVhs = sc.getDbIfPtr<VehicleStateBasicEcu>(collection, collectionCheck);
//		indexCreation = m_dbInfOnlineVhs->createIndex(collection, BsonBase::bsonFtTimestamp);
	}
//	logDebug << "Create index of " <<  collection << ": ok=" << indexCreation << std::endl;

	return collectionCheck;
}

//======================================================================

bool VehicleStateDbReader::connectToDbInterface(const dbaccess::Trip& trip)
{
	std::string cName = trip.getFullDbName(Trip::TripContentVehicleStates);
	return connectToDbInterface(CollectionName(cName));
}

//======================================================================

bool VehicleStateDbReader::connectToDbInterface(const dbaccess::ProcessingJob& proc)
{
	std::string collStr = "";

	const ResultConfig& resultConf = proc.getResultConfig();

	if (resultConf.size() < 1)
		return false;

	if (proc.getJobType() == ProcessingJob::JobType_VehicleState && m_useProcessedData) {
		collStr = resultConf[0];
	}

	return connectToDbInterface(CollectionName(collStr));
}

//======================================================================

void VehicleStateDbReader::notify(DatabaseReplayer& dbReader)
{
	if (m_useProcessedData) {
		this->notifyListeners(dbReader, &(*m_itrProcVhs));
		++m_itrProcVhs;
	}
	else {
		this->notifyListeners(dbReader, &(*m_itrOnlineVhs));
		++m_itrOnlineVhs;
	}

}

//======================================================================

const NTPTime VehicleStateDbReader::getCurrentDataBlockTimestamp() const
{
	NTPTime time;

	if (hasValidData()) {
		if (m_useProcessedData)
			return (*m_itrProcVhs).getTimestamp();
		else
			return (*m_itrOnlineVhs).getTimestamp();
	}
	else
		return NTPTime();
}

//======================================================================

const bool VehicleStateDbReader::hasValidData() const
{
	if (m_useProcessedData)
		return !m_itrProcVhs.end();
	else
		return !m_itrOnlineVhs.end();
}

//======================================================================

void VehicleStateDbReader::queryData()
{
	DbQueryOptions opt = DbQueryOptions().orderBy(DbField_Timestamp);
	if (m_useProcessedData)
		m_itrProcVhs = m_dbInfProcVhs->queryData(opt);
	else
		m_itrOnlineVhs = m_dbInfOnlineVhs->queryData(opt);
}

//======================================================================

bool VehicleStateDbReader::queryTimeRange(NTPTime& start, NTPTime& end)
{
	DbQueryOptions optFirst = DbQueryOptions().orderBy(DbField_Timestamp);
	DbQueryOptions optLast = DbQueryOptions().orderBy(DbField_Timestamp, false);

	bool firstQuery = false;
	bool lastQuery = false;

	if (m_useProcessedData) {
		VehicleStateBasicEcu2808 first, last;

		firstQuery = m_dbInfProcVhs->queryOne(optFirst, first);
		lastQuery = m_dbInfProcVhs->queryOne(optLast, last);

		start = first.getTimestamp();
		end = last.getTimestamp();
	}
	else {
		VehicleStateBasicEcu first, last;

		firstQuery = m_dbInfOnlineVhs->queryOne(optFirst, first);
		lastQuery = m_dbInfOnlineVhs->queryOne(optLast, last);

		start = first.getTimestamp();
		end = last.getTimestamp();
	}

	return firstQuery && lastQuery;
}

//======================================================================

bool VehicleStateDbReader::queryVehicleStateByTimeStamp(const NTPTime& time, VehicleStateBasicEcu& vhs)
{
	mongo::BSONObj obj = MongoDbUtils::createTimestampQuery(time.toPtime());
	return m_dbInfOnlineVhs->queryOne(obj.toString(), vhs);
}

//======================================================================

bool VehicleStateDbReader::queryVehicleStateByTimeStamp(const NTPTime& time, VehicleStateBasicEcu2808& vhs)
{
	mongo::BSONObj obj = MongoDbUtils::createTimestampQuery(time.toPtime());
	return m_dbInfProcVhs->queryOne(obj.toString(), vhs);
}

//======================================================================

bool VehicleStateDbReader::queryVehicleStateRange(const NTPTime& startTime, const NTPTime& endTime,
                                                  const StartRangeMode startRangeMode,
                                                  const EndRangeMode endRangeMode)
{
	DbQueryOptions options = MongoDbUtils::createRangeQueryOption(startTime, endTime,
	                                                              startRangeMode, endRangeMode);

	if (m_useProcessedData) {
		m_itrProcVhs = m_dbInfProcVhs->queryData(options);
		return !m_itrProcVhs.end();
	}
	else {
		m_itrOnlineVhs = m_dbInfOnlineVhs->queryData(options);
		return !m_itrOnlineVhs.end();
	}
}

//======================================================================

bool VehicleStateDbReader::queryVehicleStateRange(const TimeInterval& interval)
{
	return queryVehicleStateRange(interval.getStartTime(), interval.getEndTime(),
	                             (interval.isStartTimeIncluded() ? StartRangeInclusive : StartRangeExclusive),
	                             (interval.isEndTimeIncluded() ? EndRangeInclusive : EndRangeExclusive));
}

//======================================================================

bool VehicleStateDbReader::queryVehicleStateRangeExclusiveEnd(const NTPTime& startTime, const NTPTime& endTime)
{
	DbQueryOptions opt = DbQueryOptions().minEquals(DbField_Timestamp, startTime).max(DbField_Timestamp, endTime);

	if (m_useProcessedData) {
		m_itrProcVhs = m_dbInfProcVhs->queryData(opt);
		return !m_itrProcVhs.end();
	}
	else {
		m_itrOnlineVhs = m_dbInfOnlineVhs->queryData(opt);
		return !m_itrOnlineVhs.end();
	}
}

//======================================================================

} // namespace ibeosdk

//======================================================================
