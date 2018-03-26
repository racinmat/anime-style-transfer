//======================================================================
/*! \file DatabaseReplayer.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 16, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/DatabaseReplayer.hpp>

#include <ibeosdk/devices/database/DbDataBlockReader.hpp>
#include <ibeosdk/devices/database/PointCloudDbReader.hpp>
#include <ibeosdk/devices/database/ScanDbReader.hpp>
#include <ibeosdk/devices/database/VehicleStateDbReader.hpp>
#include <ibeosdk/devices/database/VersionDbReader.hpp>

#include <ibeosdk/database/DbConnectionFactory.hpp>

#include <boost/thread.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

using namespace dbaccess;

//======================================================================

DatabaseReplayer::DatabaseReplayer(const dbaccess::DbConnection::ConnectionProperties& prop,
                                   const std::string& sessionName,
                                   const std::string& tripName,
                                   const std::string& processingName)
  : m_connectionProperties(prop),
    m_sessionName(sessionName),
    m_tripName(tripName),
    m_processingName(processingName),
    m_sessionReader(),
    m_tripReader(),
    m_procReader(),
    m_dbDataBlockReader()
{}

//======================================================================

void DatabaseReplayer::getConnected()
{
	if (m_thread) return;

	Lock lock(m_mutex);
	m_thread = new boost::thread(&DatabaseReplayer::dataReceiveThread, this);

	while ((this->m_threadState==TS_NotRunning || this->m_threadState==TS_Starting)) {
		m_recvCondition.wait(lock);
	}
	return;
}

//======================================================================

const DataBlock* DatabaseReplayer::onData(const IbeoDataHeader& dh, std::istream& is)
{
	return NULL;
}

//======================================================================

void DatabaseReplayer::registerDbDataBlockReader(ibeosdk::DbDataBlockReader& dbDataBlockReader)
{
	m_dbDataBlockReader.push_back(&dbDataBlockReader);
}

//======================================================================

bool DatabaseReplayer::has_only_spaces(const std::string& str)
{
	return str.find_first_not_of (' ') == str.npos;
}

//======================================================================

void DatabaseReplayer::dataReceiveThread()
{
	{
		Lock lock(this->m_mutex);
		m_threadState = TS_Starting;
	}

	logInfo << "Connecting to database." << std::endl;

	try {
		DbConnectionFactory::instance().connect(m_connectionProperties, m_result);
		if (m_result.m_dbErrorCode != DbConnection::ConnectionResult::RfDb_NoError)
			throw m_result;
	}
	catch (const DbConnection::ConnectionResult& error) {
		logError << "Connection failed with error code: '" << error.m_dbErrorCode << std::endl;
		logError << "Error message: " << error.m_dbErrMsg << std::endl;
		m_threadState = TS_StartFailed;
		m_recvCondition.notify_all();
		return;
	}

	lockAndNotify(TS_Running);

	VersionDbReader v;
	v.connect();

	bool onlyWsInName = has_only_spaces(m_tripName) && has_only_spaces(m_sessionName)
	                    && has_only_spaces(m_processingName);

	m_tripReader.connect();
	m_procReader.connect();

	try {
		DatabaseVersion version = v.getModelVersion();
		logInfo << "IBEO EVS database version: " << version.getMajorValue() << "."
				<< version.getMinorValue() << "." <<version.getPathValue() << std::endl;

		if (m_sessionName.empty() || m_tripName.empty() || m_processingName.empty() || onlyWsInName) {
			if (onlyWsInName)
				logInfo << "The name for session, trip or processing contains only whitespaces or is empty."
				        << "See following info mode to determine available content." << std::endl;

			logInfo << "======================== Database Info ===========================" << std::endl;

			queryInfoData();
			printDbSessionContent(m_sessionReader.getSessionList());

			logInfo << "==================== Database Info Completed ======================" << std::endl;
			lockAndNotify(TS_Stopping);

			return;
		}
		else {
			logDebug << "Reading data from database." << std::endl;

			queryReplayData();
			notifyDataByTimestamp();

		} //else
	}
	catch (const DatabaseException& re) {
		logError << "Reception failed: '" << re.what() << "'" << std::endl;
		lockAndNotify(TS_RunFailed);
	}

	logInfo << "No more data to send." << std::endl;

	lockAndNotify(TS_Stopping);
}

//======================================================================

void DatabaseReplayer::notifyDataByTimestamp()
{
	bool allDone = false;
	do {
		NTPTime oldestTime;
		DbDataBlockReader* readerWithOldestData = NULL;

		bool iteratorsEmpty = false;

		std::list<DbDataBlockReader*>::iterator dBEmptyiter = m_dbDataBlockReader.begin();
		for (; dBEmptyiter != m_dbDataBlockReader.end(); ++dBEmptyiter)
			iteratorsEmpty = iteratorsEmpty || (*dBEmptyiter)->hasValidData();

		allDone = !iteratorsEmpty;

		std::list<DbDataBlockReader*>::iterator iterPipe = m_dbDataBlockReader.begin();
		for (; iterPipe != m_dbDataBlockReader.end(); ++iterPipe) {
			if ((*iterPipe)->hasValidData()) {
				NTPTime currentTime = (*iterPipe)->getCurrentDataBlockTimestamp();

				if (oldestTime == NTPTime(0) || oldestTime > currentTime) {
					oldestTime = currentTime;
					readerWithOldestData = *iterPipe;
				}
				else if (oldestTime == currentTime) { //check if oldest or current is a Scan
					if (    dynamic_cast<ScanDbReader*>(*iterPipe)
					    && (!dynamic_cast<ScanDbReader*>(readerWithOldestData))) {
						oldestTime = currentTime;
						readerWithOldestData = *iterPipe;
					}
				} // else if
			} // if *iterPipe->hasValidData()
		} // for

		if (readerWithOldestData)
			readerWithOldestData->notify(*this);

	} while (!allDone);
}

//======================================================================

void DatabaseReplayer::queryInfoData()
{
	Session session("");
	if(m_sessionReader.connect(CollectionName(session.getFullDbName(BaseEntity::NotUsed)))) {
		m_sessionReader.queryData();

		SessionList& list = m_sessionReader.getSessionList();
		SessionList::iterator itr = list.begin();
		for (; itr != list.end(); ++itr) {
			m_tripReader.fillTrips(itr->getTrips());
			m_procReader.fillProcessings(itr->getProcessings());
		}
	}
}

//======================================================================

void DatabaseReplayer::queryReplayData()
{
	Trip replayTrip(m_tripName);

	Session session(m_sessionName);
	if(m_sessionReader.connect(CollectionName(session.getFullDbName(BaseEntity::NotUsed)))){

		// Get session from database and check
		if (!m_sessionReader.querySessionByName(session)) {
			logError << "Session : " << "'" << m_sessionName << "'"
					<< "not found in database." << std::endl;

			lockAndNotify(TS_Stopping);
			return;
		}

		// Get trip from database and check
		if (!m_tripReader.getTrip(replayTrip)) {
			logError << "Trip: " << "'" << m_tripName << "'"
					<< "not found in database." << std::endl;

			lockAndNotify(TS_Stopping);
			return;
		}

		// Get session trips from database and check relation
		m_tripReader.fillTrips(session.getTrips());
		if (!session.hasTrip(m_tripName)) {
			logError << "Trip: " << "'" << m_tripName << "'"
			           << "is not part of the session:'" << m_sessionName << "'" << std::endl;

			lockAndNotify(TS_Stopping);
			return;
		}

		// Get session processings from database and check relation
		m_procReader.fillProcessings(session.getProcessings());

		Processing proc;
		if (!session.hasProcessing(m_processingName, proc)) {
			logInfo << "Processing : " << "'" << m_processingName << "'"
					<< "not found in database." << std::endl;

			lockAndNotify(TS_Stopping);
			return;
		}

		JobMap jobMap;
		ProcJobList& jobList = proc.jobList();

		ProcJobList::const_iterator jItr = jobList.begin();
		for (; jItr != jobList.end(); ++jItr) {
			jobMap[jItr->getJobType()] = *jItr;
		}

		// Connect each DbDataBlockReader and query first data sets
		connectDbReader(jobMap, replayTrip);
	}
}

//======================================================================

void DatabaseReplayer::connectDbReader(const JobMap& jobs, const dbaccess::Trip& trip)
{
	std::list<DbDataBlockReader*>::iterator iter = m_dbDataBlockReader.begin();
	for (; iter != m_dbDataBlockReader.end(); ++iter) {
		DbDataBlockReader* dbReader = (*iter);
		const ProcessingJob::JobType& type = dbReader->getSupportedJobType();

		bool connected = false;
		try {
			switch (type) {
			case ProcessingJob::JobType_VehicleState:
				{
					VehicleStateDbReader* vhr = dynamic_cast<VehicleStateDbReader*>(dbReader);

					if (vhr->isProcessedDataActive())
						connected = vhr->connectToDbInterface(jobs.at(type));
					else
						connected = vhr->connectToDbInterface(trip);
					break;
				}
			case ProcessingJob::JobType_DynamicObjects:
				{
					if (dbReader->isProcessedDataActive())
						connected = dbReader->connectToDbInterface(jobs.at(type));
					else
						connected = dbReader->connectToDbInterface(trip);
					break;
				}
			default:
				connected = dbReader->connectToDbInterface(trip);
				break;
			}
		}
		catch (const std::out_of_range& oor) {
			logError << "Error while connection DbDataBlockReaders to collections : '" << oor.what() << "'" << std::endl;
			lockAndNotify(TS_RunFailed);
			return;
		}

		if(!connected) {
			logError << "Error while connecting DbDataBlockReaders" << std::endl;
			lockAndNotify(TS_RunFailed);
			return;
		}

		dbReader->queryData();
	}
}

//======================================================================

void DatabaseReplayer::lockAndNotify(const ThreadState& state)
{
	if (state == TS_Stopping || state == TS_RunFailed)
		logInfo << "Shutting down." << std::endl;

	Lock lock(this->m_mutex);
	m_threadState = state;
	m_recvCondition.notify_all();
}

//======================================================================

void DatabaseReplayer::printDbSessionContent(const dbaccess::SessionList& list)
{
	logInfo << "Sessions in database: " << list.size() << std::endl;
	SessionList::const_iterator itr = list.begin();
	for (; itr != list.end(); ++itr) {
		logInfo << "===================================================" << std::endl;
		logInfo << "Session: '" << itr->getName() << "'" << std::endl;

		printDbSessionTrips(*itr);
		printDbSessionProcs(*itr);
	}
}

//======================================================================

void DatabaseReplayer::printDbSessionTrips(const dbaccess::Session& session)
{
	const TripList& tl = session.getTrips();
	logInfo << tl.size() << " Trip(s) found in database for session '" << session.getName() << '\'' << std::endl;

	TripList::const_iterator tlIter = tl.begin();
	for (; tlIter != tl.end(); ++tlIter) {

		std::vector<std::string> idcList = tlIter->getIdcPaths();

		logInfo << "Trip: '" << tlIter->getName() << "' consists of " << idcList.size()  << " idc-file(s):" << std::endl;

		std::vector<std::string>::const_iterator idcIter = idcList.begin();
		for (; idcIter != idcList.end(); ++idcIter) {
			logInfo << "IDC: '" << *idcIter << "'" << std::endl;
		}
	}
}

//======================================================================

void DatabaseReplayer::printDbSessionProcs(const dbaccess::Session& session)
{
	const ProcList& pl = session.getProcessings();

	logInfo << pl.size() << " Processing(s) found in database for session '"
			<< session.getName() << '\'' << std::endl;

	ProcList::const_iterator plItr = pl.begin();
	for (; plItr != pl.end(); ++plItr) {

		const ProcJobList& jList = plItr->getJobList();

		logInfo << "Processing: '" << plItr->getName() << "' with ID: '"
				<< plItr->getDbId() << "' contains " << jList.size() << " job(s)." << std::endl;

		ProcJobList::const_iterator jobItr = jList.begin();
		for (; jobItr != jList.end(); ++jobItr) {
			logInfo << "Job: " << jobItr->getTypeAsString() << " with ID: '"
					<< jobItr->getDbId() << "'" << std::endl;

			const ResultConfig& resultConf =  jobItr->getResultConfig();
			logInfo << resultConf.size() << " Results in following Collection(s):" << std::endl;

			ResultConfig::const_iterator resultItr = resultConf.begin();
			for (; resultItr != resultConf.end(); ++resultItr) {
				logInfo << *resultItr << std::endl;
			}
		}
	}
}

//======================================================================

}// namespace ibeosdk

//======================================================================
