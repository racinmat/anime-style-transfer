//======================================================================
/*! \file DatabaseReplayer.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 16, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef IBEOSDK_DATABASEREPLAYER_HPP_SEEN
#define IBEOSDK_DATABASEREPLAYER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/devices/IbeoDevice.hpp>
#include <ibeosdk/database/DbConnection.hpp>

#include <ibeosdk/database/basedatamodel/Session.hpp>

#include <ibeosdk/devices/database/TripDbReader.hpp>
#include <ibeosdk/devices/database/SessionDbReader.hpp>
#include <ibeosdk/devices/database/ProcessingDbReader.hpp>

#include <boost/unordered_map.hpp>
#include <exception>

//======================================================================

namespace ibeosdk {
class DbDataBlockReader;

//======================================================================
/*!\class DatabaseReplayer
 * \brief Class to connect to a MongoDB for receiving data in a sequential way.
 * \author Kristian Bischoff (kb)
 * \date Apr 16, 2016
 *///-------------------------------------------------------------------
class DatabaseReplayer : public ibeosdk::IbeoDeviceBase {

typedef boost::unordered_map<dbaccess::ProcessingJob::JobType, dbaccess::ProcessingJob> JobMap;

public:
	//========================================
	/*!\brief Constructor
	 *
	 * \param[in] prop            Connection properties to use for connecting to a database.
	 * \param[in] sessionName     Name of a Session inside the database.
	 * \param[in] tripName        Name of a Trip inside the database.
	 * \param[in] processingName  Name of a Processing inside the database.
	 *///-------------------------------------
	DatabaseReplayer(const dbaccess::DbConnection::ConnectionProperties& prop,
	                 const std::string& sessionName = std::string(),
	                 const std::string& tripName = std::string(),
	                 const std::string& processingName = std::string());

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~DatabaseReplayer() {}

public:
	//========================================
	/*!\brief Establish the connection to the
	 *        hardware.
	 *
	 * Starting the receiving DatabaseReplayer::dataReceiveThread().
	 *///-------------------------------------
	virtual void getConnected();

public:
	//========================================
	/*!\brief This method will be called by the receive thread
	 *        when a new DataBlock has been received completely.
	 *
	 * This class will call notifyListeners and notifyStreamers.
	 *
	 * \param[in]       dh  Header that came along with that
	 *                      DataBlock.
	 * \param[in, out]  is  Stream that contains the contents of
	 *                      the DataBlock that has been received.
	 * \note Not implemented returns NULL
	 *///-------------------------------------
	virtual const DataBlock* onData(const IbeoDataHeader& dh, std::istream& is);

public:
	//========================================
	/*!\brief Register a DbDataBlockReader to this device.
	 *
	 * Each time a message has been received by
	 * this object, the registered listener will be
	 * called which are listening to the received message
	 * type (DataType).
	 * \param[in] dbDataBlockReader  Reader to be registered.
	 * \note There is (currently) no way to unregister a
	 *       listener, so a registered DataListener must
	 *       not be destroyed before this IbeoDevice
	 *       has been destroyed.
	 *///-------------------------------------
	void registerDbDataBlockReader(ibeosdk::DbDataBlockReader& dbDataBlockReader);

public:
	//========================================
	/*!\brief Prints information about all Session's in the given list.
	 *
	 * For each Session, Trip's and Processing's will be shown as well.
	 *
	 * \param[in] list  The session list which holds all information.
	 *///----------------------------------
	void printDbSessionContent(const dbaccess::SessionList& list);

	//========================================
	/*!\brief Prints information about Trip's to a given session.
	 *
	 * This includes the trip name and containing idc files.
	 * \param[in] session  The session which holds trip information.
	 *///----------------------------------
	void printDbSessionTrips(const dbaccess::Session& session);

	//========================================
	/*!\brief Prints information about Processing's to a given session.
	 *
	 * This includes the count, name, id and job count.
	 * For every ProcessingJob, the type, id and result collections
	 * will be shown as well.
	 *
	 * \param[in] session  The session which holds processing information.
	 *///----------------------------------
	void printDbSessionProcs(const dbaccess::Session& session);

protected:
	//========================================
	/*!\brief Main function of the receive thread.
	 *
	 * Establish a database connection with the given connection properties
	 * #m_connectionProperties. Reads the ibeo database version and try to replay
	 * the data specified by #m_sessionName, #m_tripName and #m_processingName.
	 * If one of the above mentioned parameters are empty, the DatabaseReplayer will
	 * ignore the others and print out information about Session's, Trip's and Processing's.
	 *///-------------------------------------
	void dataReceiveThread();

	//========================================
	/*!\brief Queries information about Session's, Trip's and Processing's.
	 *
	 * Uses SessionDbReader, TripDbReader and ProcessingDbReader.
	 *///-------------------------------------
	void queryInfoData();

	//========================================
	/*!\brief Queries the data depending on the specified parameters
	 * #m_sessionName, #m_tripName and #m_processingName.
	 *
	 * Calls DatabaseReplayer::connectDbReader(const JobMap& jobs, const dbaccess::Trip& trip).
	 *///-------------------------------------
	void queryReplayData();

	//========================================
	/*!\brief Iterates through all DbDataBlockReaders to
	 * send the object with the oldest time stamp.
	 * If a ScanEcu and another DataBlock have the same time stamp, the ScanEcu
	 * will be send first.
	 *///-------------------------------------
	void notifyDataByTimestamp();

	//========================================
	/*!\brief Locks the #m_mutex and calls notify_all on IbeoDeviceBase::Condition
	 * \param[in]  state  IbeoDeviceBase::ThreadState
	 *///-------------------------------------
	void lockAndNotify(const ThreadState& state);

	//======================================================================
	/*!\brief Check if the given string contains only spaces.
	 * \param[in]  str  String to be checked.
	 * \return \c true str has only spaces,
	 *         \c false otherwise.
	 *///-------------------------------------
	bool has_only_spaces(const std::string& str);

	//======================================================================
	/*!\brief Connects the registered DbDataBlockReaders to the database
	 *
	 * Uses a JobMap to combine DbDataBlockReaders (for Processing's) with
	 * corresponding Processing's from the database.
	 * If a DbDataBlockReaders has no support for Processing's, the Trip specify
	 * the right connection
	 *
	 * \param[in]  jobs  JobMap - mapping from type to corresponding ProcessingJob
	 * \param[in]  trip  Selected trip from the session #m_tripName
	 *///-------------------------------------
	void connectDbReader(const JobMap& jobs, const dbaccess::Trip& trip);

private: // not supported
	//========================================
	virtual statuscodes::Codes sendCommand(const CommandBase&,
	                                       CommandReplyBase&,
	                                       const boost::posix_time::time_duration)
	{
		return statuscodes::SendingCommandFailed;
	}

	//========================================
	virtual statuscodes::Codes sendCommand(const CommandBase&)
	{
		return statuscodes::SendingCommandFailed;
	}

	//========================================
	virtual void onReceiveCommandReply(const IbeoDataHeader&, const char*) {}

protected:
	//========================================
	/*!\brief Holds the connection properties.
	 *///-------------------------------------
	const dbaccess::DbConnection::ConnectionProperties m_connectionProperties;

	//========================================
	/*!\brief Holds the connection result.
	 *///-------------------------------------
	dbaccess::DbConnection::ConnectionResult m_result;

	//========================================
	/*!\brief Name of a Session the DatabaseReplayer is associated to.
	 *///-------------------------------------
	const std::string m_sessionName;

	//========================================
	/*!\brief Name of a Trip the DatabaseReplayer is associated to.
	 *///-------------------------------------
	const std::string m_tripName;

	//========================================
	/*!\brief Name of a Processing the DatabaseReplayer is associated to.
	 *///-------------------------------------
	const std::string m_processingName;

	//========================================
	/*!\brief SessionDbReader to query for Session's
	 *///-------------------------------------
	SessionDbReader m_sessionReader;

	//========================================
	/*!\brief TripDbReader to query for Trip's
	 *///-------------------------------------
	TripDbReader m_tripReader;

	//========================================
	/*!\brief ProcessingDbReader to query for Processings's
	 *        and ProcessingJob's
	 *///-------------------------------------
	ProcessingDbReader m_procReader;

	//========================================
	/*!\brief List for connected DbDataBlockReader's
	 *///-------------------------------------
	std::list<DbDataBlockReader*> m_dbDataBlockReader;

	friend class DbDataBlockReader;
}; // DatabaseReplayer

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_DATABASEREPLAYER_HPP_SEEN

//======================================================================
