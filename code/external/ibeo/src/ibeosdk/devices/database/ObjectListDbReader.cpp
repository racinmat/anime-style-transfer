//======================================================================
/*! \file ObjectListDbReader.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 21, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/database/ObjectListDbReader.hpp>

#include <ibeosdk/database/DbConnectionFactory.hpp>
#include <ibeosdk/database/DbQueryOptions.hpp>

#include <ibeosdk/database/MongoDbUtils.hpp>
#include <ibeosdk/datablocks/ScanEcu.hpp>
//======================================================================

namespace ibeosdk {

//======================================================================

using namespace dbaccess;
using namespace dbField;

//======================================================================

ObjectListDbReader::ObjectListDbReader()
  : DbDataBlockReader(ProcessingJob::JobType_DynamicObjects),
    m_hasValidData(false),
    m_currentList(),
    m_currentScansStartTs(),
    m_dbInterface(),
    m_scanDbInterface(),
    m_scanDbCursor(),
    m_lastScanIsBeyondRange(false)
{}

//======================================================================

bool ObjectListDbReader::connectToDbInterface(const dbaccess::CollectionName& scanCollection,
                                              const dbaccess::CollectionName& objTrajCollection,
                                              const dbaccess::CollectionName& objInvCollection)
{
	bool collCheckObj = false;
	bool collCheckScan = false;

	DbConnectionFactory::ScopedConnection sc;
	m_dbInterface = sc.getDbObjIfPtr(objInvCollection, objTrajCollection, collCheckObj);
	m_scanDbInterface = sc.getDbIfPtr<ScanEcu>(scanCollection, collCheckScan);

	return collCheckObj && collCheckScan;
}

//======================================================================

bool ObjectListDbReader::connectToDbInterface(const dbaccess::Trip& trip)
{
	CollectionName objInv(trip.getFullDbName(BaseEntity::ContentDynamicObjects));
	CollectionName objTraj(trip.getFullDbName(BaseEntity::ContentDynamicObjectStreams));

	return connectToDbInterface(CollectionName(trip.getFullDbName(Trip::TripContentScans)),
	                            objTraj, objInv);
}

//======================================================================

bool ObjectListDbReader::connectToDbInterface(const dbaccess::ProcessingJob& proc)
{
	if (m_useProcessedData) {
		std::string objInv = "";
		std::string objTraj = "";

		const ResultConfig& resultConf = proc.getResultConfig();

		if (resultConf.size() < 2 || proc.getJobType() != ProcessingJob::JobType_DynamicObjects)
			assert(false);

		ResultConfig::const_iterator resultItr = resultConf.begin();
		for (; resultItr != resultConf.end(); ++resultItr) {
			if (findSubStr(*resultItr, BaseEntity::ContentSuffix_ObjTraj))
				objTraj = *resultItr;
			else if (findSubStr(*resultItr, BaseEntity::ContentSuffix_DynObj))
				objInv = *resultItr;
		}

		if (objInv.empty() || objTraj.empty())
			assert(false);

		Trip temp(proc.getTripName());

		return connectToDbInterface(CollectionName(temp.getFullDbName(Trip::TripContentScans)),
		                            CollectionName(objTraj), CollectionName(objInv));
	}
	return false;
}

//======================================================================

void ObjectListDbReader::queryData()
{
	resetCurrentValues();

	m_scanDbCursor = m_scanDbInterface->fastQuery(DbQueryOptions().orderBy(DbField_Timestamp).includeField(DbField_Timestamp));
	getObjectList(m_currentList);
}

//======================================================================

bool ObjectListDbReader::queryTimeRange(NTPTime& start, NTPTime& end)
{
	ScanEcu first, last;

	bool firstQuery = m_scanDbInterface->queryOne(DbQueryOptions().orderBy(DbField_Timestamp), first);
	bool lastQuery = m_scanDbInterface->queryOne(DbQueryOptions().orderBy(DbField_Timestamp, false), last);

	if(firstQuery && lastQuery)
	{
		start = first.getStartTimestamp();
		end = last.getStartTimestamp();
	}

	return firstQuery && lastQuery;
}

//======================================================================

bool ObjectListDbReader::queryObjectListByTimeStamp(const NTPTime& time, ObjectListEcuEtDyn& objList)
{
	resetCurrentValues();

	DbQueryOptions options = DbQueryOptions();
	options = options.orderBy(DbField_Timestamp);
	options = options.includeField(DbField_Timestamp);
	options = options.min(DbField_Timestamp, time);
	options.limit(2);

	m_scanDbCursor = m_scanDbInterface->fastQuery(options);

	getObjectList(objList);

	resetCurrentValues();

	return objList.getNbOfObjects() != 0;
}

//======================================================================

void ObjectListDbReader::prepareScanDbCursor(const NTPTime& startTime, const NTPTime& endTime,
                                             const StartRangeMode startRangeMode,
                                             const EndRangeMode endRangeMode)
{
	DbQueryOptions options;

	options.orderBy(DbField_Timestamp)
	       .includeField(DbField_Timestamp)
	       .min(DbField_Timestamp, endTime)
	       .limit(1);
	m_scanDbCursor = m_scanDbInterface->fastQuery(options);

	NTPTime endTimeForQuery = endTime;

	m_lastScanIsBeyondRange = m_scanDbCursor->more();
	if (m_lastScanIsBeyondRange) {
		mongo::BSONObj obj = m_scanDbCursor->next();
		endTimeForQuery = NTPTime(MongoDbUtils::convertToBoostTimestamp(obj.getField(BsonBase::bsonFtTimestamp).Date()));
	}

	options = MongoDbUtils::createRangeQueryOption(startTime, endTimeForQuery, startRangeMode, endRangeMode);
	options.orderBy(DbField_Timestamp)
	       .includeField(DbField_Timestamp);

	m_scanDbCursor = m_scanDbInterface->fastQuery(options);
}

//======================================================================

bool ObjectListDbReader::queryObjectListRange(const NTPTime& startTime, const NTPTime& endTime,
                                              const StartRangeMode startRangeMode,
                                              const EndRangeMode endRangeMode)
{
	resetCurrentValues();

	prepareScanDbCursor(startTime, endTime, startRangeMode, endRangeMode);

	if (m_scanDbCursor->more()) {
		getObjectList(m_currentList);
		return m_currentList.getNbOfObjects() != 0;
	}
	else {
		return false;
	}
}

//======================================================================

bool ObjectListDbReader::queryObjectListRange(const TimeInterval& interval)
{
	return queryObjectListRange(interval.getStartTime(),
	                       interval.getEndTime(),
	                       (interval.isStartTimeIncluded() ? StartRangeInclusive : StartRangeExclusive),
	                       (interval.isEndTimeIncluded() ? EndRangeInclusive : EndRangeExclusive));
}

//======================================================================

void ObjectListDbReader::notify(DatabaseReplayer& dbReader)
{
	notifyListeners(dbReader, &m_currentList);
	next();
}

//======================================================================

const NTPTime ObjectListDbReader::getCurrentDataBlockTimestamp() const
{
	return m_currentList.getTimestamp();
}

//======================================================================

const ibeosdk::ObjectEcuEtDyn* ObjectListDbReader::getFirstObjectInvariant()
{
	m_invariantCursor = m_dbInterface->getAllInvariantData();
	return getNextObjectInvariant();
}

//======================================================================

const ibeosdk::ObjectEcuEtDyn* ObjectListDbReader::getNextObjectInvariant()
{
	if (m_dbInterface->getNextInvariantData(m_invariantCursor, m_objInvariant)) {
		return &m_objInvariant;
	}
	return NULL;
}

//======================================================================

void ObjectListDbReader::getObjectList(ObjectListEcuEtDyn& objList)
{
	objList.setToBeRefObjList(this->m_useProcessedData);

	boost::posix_time::ptime nextScansStartTs;

	if (m_currentScansStartTs.is_not_a_date_time()) {
		getNextScanTimestamp(m_currentScansStartTs);
	}


	getNextScanTimestamp(nextScansStartTs);

	const bool hasQueryStartTs = !m_currentScansStartTs.is_not_a_date_time();
	const bool hasQueryEndTs = !nextScansStartTs.is_not_a_date_time();


	if (!hasQueryStartTs) {
		m_hasValidData = false;
		return;
	}

	if (!hasQueryEndTs && m_lastScanIsBeyondRange) {
		m_hasValidData = false;
		return;
	}

	// here hasQueryStartTs = true
	DbQueryOptions opt;
	if (hasQueryEndTs)
		opt.minEquals(DbField_Timestamp, m_currentScansStartTs).max(DbField_Timestamp, nextScansStartTs);
	else
		opt.minEquals(DbField_Timestamp, m_currentScansStartTs);

	mongo::Query query = MongoDbUtils::convertDbQueryOptionsToFullQuery(opt);

	m_hasValidData = m_dbInterface->getObjectList(objList, query);

	if (objList.getTimestamp().is_not_a_date_time()) {
		objList.setTimestamp(NTPTime(m_currentScansStartTs));
	}

	if (m_hasValidData) {
		IbeoDataHeader header;
		header.setTimestamp(objList.getTimestamp());
		objList.setDataHeader(header);
	}

	m_currentScansStartTs = nextScansStartTs;
}

//======================================================================

void ObjectListDbReader::getNextScanTimestamp(boost::posix_time::ptime& time)
{
	if (m_scanDbCursor->more()) {
		mongo::BSONObj obj = m_scanDbCursor->next();
		time = MongoDbUtils::convertToBoostTimestamp(obj.getField(BsonBase::bsonFtTimestamp).Date());
	}
}

//======================================================================

void ObjectListDbReader::getNextTimestampFromCursor(dbaccess::MongoCursorPtr cursor,
                                                    boost::posix_time::ptime& time)
{
	if (cursor->more()) {
		mongo::BSONObj obj = cursor->next();
		if(obj.hasField(BsonBase::bsonFtTimestamp)) {
			time = MongoDbUtils::convertToBoostTimestamp(obj.getField(BsonBase::bsonFtTimestamp).Date());
		}
		else {
			time = boost::posix_time::not_a_date_time;
		}
	}
}

//======================================================================

void ObjectListDbReader::resetCurrentValues()
{
	m_currentScansStartTs = boost::posix_time::not_a_date_time;
	m_currentList = ObjectListEcuEtDyn();
}

//======================================================================

} // namespace ibeosdk

//======================================================================
