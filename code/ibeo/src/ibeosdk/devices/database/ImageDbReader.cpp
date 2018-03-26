//======================================================================
/*! \file ImageDbReader.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 21, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/database/ImageDbReader.hpp>
#include <ibeosdk/database/DbConnectionFactory.hpp>
#include <ibeosdk/database/DbQueryOptions.hpp>
#include <ibeosdk/database/MongoDbUtils.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

using namespace dbaccess;
using namespace dbField;

//======================================================================

ImageDbReader::ImageDbReader()
  : DbDataBlockReader(ProcessingJob::JobType_None)
{}

//======================================================================

bool ImageDbReader::connectToDbInterface(const CollectionName& collection)
{
	bool collectionCheck = false;

	DbConnectionFactory::ScopedConnection sc;
	m_dbInterface = sc.getDbIfPtr<Image>(collection, collectionCheck);

//	if(collectionCheck) {
//		bool ret = m_dbInterface->createIndex(collection, BsonBase::bsonFtTimestamp);
//		logDebug << "Create index of " <<  collection << ": ok=" << ret << std::endl;
//	}
	return collectionCheck;
}

//======================================================================

bool ImageDbReader::connectToDbInterface(const dbaccess::Trip& trip)
{
	std::string cName = trip.getFullDbName(Trip::TripContentImages);
	return connectToDbInterface(CollectionName(cName));
}

//======================================================================

void ImageDbReader::notify(DatabaseReplayer& dbReader)
{
	this->notifyListeners(dbReader, &(*m_itr));
	++m_itr;
}

//======================================================================

const NTPTime ImageDbReader::getCurrentDataBlockTimestamp() const
{
	if (hasValidData())
		return (*m_itr).getTimestamp();
	else
		return NTPTime();
}

//======================================================================

const bool ImageDbReader::hasValidData() const
{
	return !m_itr.end();
}

//======================================================================

void ImageDbReader::queryData()
{
	m_itr = m_dbInterface->queryData(DbQueryOptions().orderBy(DbField_Timestamp));
}

//======================================================================

bool ImageDbReader::queryTimeRange(NTPTime& start, NTPTime& end)
{
	Image first, last;

	bool firstQuery = m_dbInterface->queryOne(DbQueryOptions().orderBy(DbField_Timestamp), first);
	bool lastQuery = m_dbInterface->queryOne(DbQueryOptions().orderBy(DbField_Timestamp, false), last);

	if (firstQuery && lastQuery) {
		start = first.getTimestamp();
		end = last.getTimestamp();
	}

	return firstQuery && lastQuery;
}

//======================================================================

bool ImageDbReader::queryImageByTimeStamp(const NTPTime& time, Image& image)
{
	return m_dbInterface->queryOne(MongoDbUtils::createTimestampQuery(time.toPtime()).toString(), image);
}

//======================================================================

bool ImageDbReader::queryImageRange(const NTPTime& startTime, const NTPTime& endTime,
                                    const StartRangeMode startRangeMode,
                                    const EndRangeMode endRangeMode)
{
	DbQueryOptions options = MongoDbUtils::createRangeQueryOption(startTime, endTime,
	                                                              startRangeMode, endRangeMode);

	m_itr = m_dbInterface->queryData(options);

	return !m_itr.end();
}

//======================================================================

bool ImageDbReader::queryImageRange(const TimeInterval& interval)
{
	return queryImageRange(interval.getStartTime(),
	                       interval.getEndTime(),
	                       (interval.isStartTimeIncluded() ? StartRangeInclusive : StartRangeExclusive),
	                       (interval.isEndTimeIncluded() ? EndRangeInclusive : EndRangeExclusive));
}

//======================================================================

} // namespace ibeosdk

//======================================================================
