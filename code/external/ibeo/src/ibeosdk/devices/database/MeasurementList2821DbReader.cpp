//======================================================================
/*! \file MeasurementList2821DbReader.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 21, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/database/MeasurementList2821DbReader.hpp>
#include <ibeosdk/database/DbConnectionFactory.hpp>
#include <ibeosdk/database/DbQueryOptions.hpp>
#include <ibeosdk/database/MongoDbUtils.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

using namespace dbaccess;
using namespace dbField;

//======================================================================

MeasurementList2821DbReader::MeasurementList2821DbReader()
  : DbDataBlockReader(ProcessingJob::JobType_None)
{}

//======================================================================

bool MeasurementList2821DbReader::connectToDbInterface(const dbaccess::CollectionName& collection)
{
	bool collectionCheck = false;

	DbConnectionFactory::ScopedConnection sc;
	m_dbInterface = sc.getDbIfPtr<MeasurementList2821>(collection, collectionCheck);

//	if(collectionCheck) {
//		bool ret = m_dbInterface->createIndex(collection, BsonBase::bsonFtTimestamp);
//		logDebug << "Create index of " <<  collection << ": ok=" << ret << std::endl;
//	}
	return collectionCheck;
}

//======================================================================

bool MeasurementList2821DbReader::connectToDbInterface(const dbaccess::Trip& trip)
{
	std::string cName = trip.getFullDbName(Trip::TripContentMeasurementList);
	return connectToDbInterface(CollectionName(cName));
}

//======================================================================

void MeasurementList2821DbReader::notify(DatabaseReplayer& dbReader)
{
	this->notifyListeners(dbReader, &(*m_itr));
	++m_itr;
}

//======================================================================

const NTPTime MeasurementList2821DbReader::getCurrentDataBlockTimestamp() const
{
	if (hasValidData())
		return (*m_itr).getTimestamp();
	else
		return NTPTime();
}

//======================================================================

const bool MeasurementList2821DbReader::hasValidData() const
{
	return !m_itr.end();
}

//======================================================================

void MeasurementList2821DbReader::queryData()
{
	m_itr = m_dbInterface->queryData(DbQueryOptions().orderBy(DbField_Timestamp));
}

//======================================================================

bool MeasurementList2821DbReader::queryTimeRange(NTPTime& start, NTPTime& end)
{
	MeasurementList2821 first, last;

	bool firstQuery = m_dbInterface->queryOne(DbQueryOptions().orderBy(DbField_Timestamp), first);
	bool lastQuery = m_dbInterface->queryOne(DbQueryOptions().orderBy(DbField_Timestamp, false), last);

	if (firstQuery && lastQuery)
	{
		start = first.getTimestamp();
		end = last.getTimestamp();
	}

	return firstQuery && lastQuery;
}

//======================================================================

bool MeasurementList2821DbReader::queryMeasurementListByTimeStamp(const NTPTime& time, MeasurementList2821& msl)
{
	return m_dbInterface->queryOne(MongoDbUtils::createTimestampQuery(time.toPtime()).toString(), msl);
}

//======================================================================

bool MeasurementList2821DbReader::queryMeasurementListRange(const NTPTime& startTime, const NTPTime& endTime,
                                                            const StartRangeMode startRangeMode,
                                                            const EndRangeMode endRangeMode)
{
	DbQueryOptions options = MongoDbUtils::createRangeQueryOption(startTime, endTime,
	                                                              startRangeMode, endRangeMode);

	m_itr = m_dbInterface->queryData(options);

	return !m_itr.end();
}

//======================================================================

bool MeasurementList2821DbReader::queryMeasurementListRange(const TimeInterval& interval)
{
	return queryMeasurementListRange(interval.getStartTime(),
	                       interval.getEndTime(),
	                       (interval.isStartTimeIncluded() ? StartRangeInclusive : StartRangeExclusive),
	                       (interval.isEndTimeIncluded() ? EndRangeInclusive : EndRangeExclusive));
}

//======================================================================

} // namespace ibeosdk

//======================================================================
