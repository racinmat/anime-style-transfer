//======================================================================
/*! \file ScanDbReader.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 17, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/database/ScanDbReader.hpp>
#include <ibeosdk/database/DbConnectionFactory.hpp>
#include <ibeosdk/database/DbQueryOptions.hpp>
#include <ibeosdk/datablocks/ScanEcu.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

using namespace dbaccess;
using namespace dbField;

//======================================================================

ScanDbReader::ScanDbReader()
  : DbDataBlockReader(ProcessingJob::JobType_None)
{}

//======================================================================

bool ScanDbReader::connectToDbInterface(const CollectionName& collection)
{
	bool collectionCheck = false;
	DbConnectionFactory::ScopedConnection sc;
	m_dbInterface = sc.getDbIfPtr<ScanEcu>(collection, collectionCheck);

//	if(collectionCheck) {
//		bool ret = m_dbInterface->createIndex(collection, BsonBase::bsonFtTimestamp);
//		logDebug << "Create index of " <<  collection << ": ok=" << ret << std::endl;
//	}

	return collectionCheck;
}

//======================================================================

bool ScanDbReader::connectToDbInterface(const dbaccess::Trip& trip)
{
	std::string cName = trip.getFullDbName(Trip::TripContentScans);
	return connectToDbInterface(CollectionName(cName));
}

//======================================================================

void ScanDbReader::notify(DatabaseReplayer& dbReader)
{
	notifyListeners(dbReader, &(*m_itr));
	++m_itr;
}

//======================================================================

const NTPTime ScanDbReader::getCurrentDataBlockTimestamp() const
{
	if (hasValidData())
		return (*m_itr).getStartTimestamp();
	else
		return NTPTime();
}

//======================================================================

const bool ScanDbReader::hasValidData() const
{
	return !m_itr.end();
}

//======================================================================

void ScanDbReader::queryData()
{
	m_itr = m_dbInterface->queryData(DbQueryOptions().orderBy(DbField_Timestamp));
}

//======================================================================

bool ScanDbReader::queryTimeRange(NTPTime& start, NTPTime& end)
{
	ScanEcu first, last;

	const bool firstQuery = m_dbInterface->queryOne(DbQueryOptions().orderBy(DbField_Timestamp), first);
	const bool lastQuery = m_dbInterface->queryOne(DbQueryOptions().orderBy(DbField_Timestamp, false), last);

	if (firstQuery && lastQuery) {
		start = first.getStartTimestamp();
		end = last.getStartTimestamp();
	}

	return firstQuery && lastQuery;
}

//======================================================================

bool ScanDbReader::queryScanByTimeStamp(const NTPTime& time, ScanEcu& scan)
{
	return m_dbInterface->queryOne(MongoDbUtils::createTimestampQuery(time.toPtime()).toString(), scan);
}

//======================================================================

bool ScanDbReader::queryScanRange(const NTPTime& startTime, const NTPTime& endTime,
	                              const StartRangeMode startRangeMode,
	                              const EndRangeMode endRangeMode)
{
	DbQueryOptions options = MongoDbUtils::createRangeQueryOption(startTime, endTime,
	                                                              startRangeMode, endRangeMode);

	m_itr = m_dbInterface->queryData(options);

	return !m_itr.end();
}

//======================================================================

bool ScanDbReader::queryScanRange(const TimeInterval& interval)
{
	return queryScanRange(interval.getStartTime(),
	                       interval.getEndTime(),
	                       (interval.isStartTimeIncluded() ? StartRangeInclusive : StartRangeExclusive),
	                       (interval.isEndTimeIncluded() ? EndRangeInclusive : EndRangeExclusive));
}

//======================================================================

} // namespace ibeosdk

//======================================================================
