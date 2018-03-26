//======================================================================
/*! \file ProcessingDbReader.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 7, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/database/ProcessingDbReader.hpp>

#include <ibeosdk/database/DbConnectionFactory.hpp>
#include <ibeosdk/database/MongoDbUtils.hpp>
#include <ibeosdk/database/DbQueryOptions.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

using namespace dbaccess;
using namespace dbField;

//======================================================================

ProcessingDbReader::ProcessingDbReader()
  : m_dbInterface(),
    m_dbProcJobInterface(),
    m_itr()
{}

//======================================================================

void ProcessingDbReader::connect()
{
	DbConnectionFactory::ScopedConnection sc;
	m_dbInterface = sc.getDbIfPtr();
}

//======================================================================

bool ProcessingDbReader::getProcessingJob(dbaccess::ProcessingJob& procJob)
{
	if(MongoDbUtils::collectionExists(m_dbInterface->m_mDbConnection->getDbClientConnection(),
	                                  procJob.getCollectionName())) {

		//	For MongoDB ver. >3.x
		//	DbQueryOptions opt = DbQueryOptions().match(DbField_MongoId, mongo::OID(proc.getDbId()));
		//	mongo::BSONObj obj = m_dbInterface->queryOne(proc.getCollectionName(), opt);

		//	For MongoDB ver. <3.x
		mongo::BSONObjBuilder objBuild;
		objBuild.append(MongoDbUtils::convertFieldToName(DbField_MongoId), mongo::OID(procJob.getDbId()));

		const mongo::BSONObj obj = m_dbInterface->queryOne(procJob.getCollectionName(), objBuild.obj());

		if (obj.isEmpty())
			return false;

		BsonT<ProcessingJob>::createDataType(procJob, obj);
		return true;
	}
	else {
		return false;
	}
}

//======================================================================

ProcJobList ProcessingDbReader::getProcessingJobs(const dbaccess::Trip& trip)
{
	const CollectionName cn(trip.getFullDbName(Trip::TripContentProcessings));

	bool collectionCheck = false;

	DbConnectionFactory::ScopedConnection sc;
	m_dbProcJobInterface = sc.getDbIfPtr<ProcessingJob>(cn, collectionCheck);

	ProcJobList procJobs;
	if(collectionCheck) {
		m_itr = m_dbProcJobInterface->queryData();

		while (!m_itr.end()) {
			procJobs.push_back(*m_itr);
			++m_itr;
		}
	}

	return procJobs;
}

//======================================================================

void ProcessingDbReader::fillProcessingJobs(dbaccess::ProcJobList& jobList)
{
	ProcJobList::iterator itr = jobList.begin();
	for (; itr != jobList.end(); ++itr)
		getProcessingJob(*itr);
}

//======================================================================

bool ProcessingDbReader::getProcessing(dbaccess::Processing& proc)
{
	if(MongoDbUtils::collectionExists(m_dbInterface->m_mDbConnection->getDbClientConnection(),
	                                  proc.getCollectionName())) {

		//	For MongoDB ver. >3.x
		//	DbQueryOptions opt = DbQueryOptions().match(DbField_MongoId, mongo::OID(proc.getDbId()));
		//	mongo::BSONObj obj = m_dbInterface->queryOne(proc.getCollectionName(), opt);

		//	For MongoDB ver. <3.x
		mongo::BSONObjBuilder objBuild;
		objBuild.append(MongoDbUtils::convertFieldToName(DbField_MongoId), mongo::OID(proc.getDbId()));
		const mongo::BSONObj obj = m_dbInterface->queryOne(proc.getCollectionName(), objBuild.obj());

		if (obj.isEmpty())
			return false;

		BsonT<Processing>::createDataType(proc, obj);
		fillProcessingJobs(proc.jobList());
		return true;
	}
	else {
		return false;
	}
}

//======================================================================

ProcList ProcessingDbReader::getProcessings()
{
	DbConnectionFactory::ScopedConnection sc;
	CollectionName cn(Processing().getFullDbName(BaseEntity::NotUsed));

	bool collectionCheck = false;
	DbInterfaceT<Processing>::DbIfPtr procInterface = sc.getDbIfPtr<Processing>(cn, collectionCheck);

	ProcList list;
	if(collectionCheck) {
		DbQueryIterator<Processing> itr = procInterface->queryData();
		while (!itr.end()) {
			Processing proc = *itr;
			fillProcessingJobs(proc.jobList());
			list.push_back(proc);
		}
	}

	return list;
}

//======================================================================

void ProcessingDbReader::fillProcessings(dbaccess::ProcList& list)
{
	ProcList::iterator itr = list.begin();
	for (; itr != list.end(); ++itr)
		getProcessing(*itr);
}

//======================================================================

} // namespace ibeosdk

//======================================================================
