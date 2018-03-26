//======================================================================
/*! \file CanMessageDbReader.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 22, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/database/CanMessageDbReader.hpp>
#include <ibeosdk/database/DbConnectionFactory.hpp>
#include <ibeosdk/database/DbQueryOptions.hpp>
#include <ibeosdk/database/MongoDbUtils.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

using namespace dbaccess;
using namespace dbField;

//======================================================================

CanMessageDbReader::CanMessageDbReader()
  : DbDataBlockReader(ProcessingJob::JobType_None)
{}

//======================================================================

bool CanMessageDbReader::connectToDbInterface(const dbaccess::CollectionName& collection)
{
	bool collectionCheck = false;

	DbConnectionFactory::ScopedConnection sc;
	m_dbInterface = sc.getDbIfPtr<CanMessage>(collection, collectionCheck);

//	if(collectionCheck) {
//		bool ret = m_dbInterface->createIndex(collection, BsonBase::bsonFtTimestamp);
//		logDebug << "Create index of " <<  collection << ": ok=" << ret << std::endl;
//	}

	return collectionCheck;
}

//======================================================================

bool CanMessageDbReader::connectToDbInterface(const dbaccess::Trip& trip)
{
	std::string cName = trip.getFullDbName(Trip::TripContentCANMessages);
	return connectToDbInterface(CollectionName(cName));
}

//======================================================================

void CanMessageDbReader::notify(DatabaseReplayer& dbReader)
{
	this->notifyListeners(dbReader, &(*m_itr));
	++m_itr;
}

//======================================================================

const NTPTime CanMessageDbReader::getCurrentDataBlockTimestamp() const
{
	if (hasValidData())
		return (*m_itr).getTimestamp();
//		return (*m_itr).getH();
	else
		return NTPTime();
}

//======================================================================

const bool CanMessageDbReader::hasValidData() const
{
	return !m_itr.end();
}

//======================================================================

void CanMessageDbReader::queryData()
{
	m_itr = m_dbInterface->queryData(DbQueryOptions().orderBy(DbField_Timestamp));
}

//======================================================================

bool CanMessageDbReader::queryTimeRange(NTPTime& start, NTPTime& end)
{
	CanMessage first, last;

	const bool firstQuery = m_dbInterface->queryOne(DbQueryOptions().orderBy(DbField_Timestamp), first);
	const bool lastQuery = m_dbInterface->queryOne(DbQueryOptions().orderBy(DbField_Timestamp, false), last);

	if (firstQuery && lastQuery) {
		start = first.getTimestamp();
		end = last.getTimestamp();
	}

	return firstQuery && lastQuery;
}

//======================================================================

bool CanMessageDbReader::queryCanMessageByTimeStamp(const NTPTime& time, CanMessage& canMsg)
{
	return m_dbInterface->queryOne(MongoDbUtils::createTimestampQuery(time.toPtime()).toString(), canMsg);
}

//======================================================================

bool CanMessageDbReader::queryCanMessageRange(const NTPTime& startTime, const NTPTime& endTime,
                                              const StartRangeMode startRangeMode,
                                              const EndRangeMode endRangeMode)
{
	DbQueryOptions options = MongoDbUtils::createRangeQueryOption(startTime, endTime,
	                                                              startRangeMode, endRangeMode);

	m_itr = m_dbInterface->queryData(options);

	return !m_itr.end();
}

//======================================================================

bool CanMessageDbReader::queryCanMessageRange(const TimeInterval& interval)
{
	return queryCanMessageRange(interval.getStartTime(),
	                            interval.getEndTime(),
	                            (interval.isStartTimeIncluded() ? StartRangeInclusive : StartRangeExclusive),
	                            (interval.isEndTimeIncluded() ? EndRangeInclusive : EndRangeExclusive));
}

//======================================================================

}// namespace ibeosdk

//======================================================================
