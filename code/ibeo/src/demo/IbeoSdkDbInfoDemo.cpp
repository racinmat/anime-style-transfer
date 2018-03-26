//======================================================================
/*! \file IbeoSdkDbInfoDemo.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 01, 2016
 *
 * Demo project for reading data from a Mongo database and process the data blocks.
 *
 * Example parameters for info mode and write to logfile
 * -I "127.0.0.1" -U "" -C "" -l log
 *  which is equivalent to
 *  -l log
 *
 * Example parameters for replay data
 * -I "127.0.0.1" -U "" -C ""
 *
 *///-------------------------------------------------------------------

#include <ibeosdk/lux.hpp>
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>
#include <ibeosdk/scala.hpp>

#include <ibeosdk/devices/DatabaseReplayer.hpp>
#include <ibeosdk/devices/database/CanMessageDbReader.hpp>
#include <ibeosdk/devices/database/ImageDbReader.hpp>
#include <ibeosdk/devices/database/MeasurementList2821DbReader.hpp>
#include <ibeosdk/devices/database/ObjectListDbReader.hpp>
#include <ibeosdk/devices/database/ScanDbReader.hpp>
#include <ibeosdk/devices/database/VehicleStateDbReader.hpp>

#include <ibeosdk/database/initMongoClient.hpp>
#include <ibeosdk/database/basedatamodel/Session.hpp>
#include <ibeosdk/database/basedatamodel/Trip.hpp>
#include <ibeosdk/database/basedatamodel/Processing.hpp>
#include <ibeosdk/database/DbConnectionFactory.hpp>

#include <boost/program_options.hpp>
#include <iostream>
#include <cstdlib>

//======================================================================

using namespace ibeosdk;
using namespace dbaccess;

//======================================================================

const ibeosdk::Version::MajorVersion majorVersion(5);
const ibeosdk::Version::MinorVersion minorVersion(2);
const ibeosdk::Version::Revision revision(2);
const ibeosdk::Version::PatchLevel patchLevel;
const ibeosdk::Version::Build build;
const std::string info = "IbeoSdkDbInfoDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);

IbeoSDK ibeoSDK;

//======================================================================

//TimeConversion tc;

//======================================================================

int checkArguments(const int argc, const char** argv, bool& hasSessionName, bool& hasTripName, bool& hasLogFile)
{
	const int minNbOfNeededArguments = 2;
	const int maxNbOfNeededArguments = 5;

	bool wrongNbOfArguments = false;
	if (argc < minNbOfNeededArguments) {
		std::cerr << "Missing argument" << std::endl;
		wrongNbOfArguments = true;
	}
	else if (argc > maxNbOfNeededArguments) {
		std::cerr << "Too many argument" << std::endl;
		wrongNbOfArguments = true;
	}

	if (wrongNbOfArguments) {
		std::cerr << argv[0] << " " << " IP [LOGFILE]" << std::endl;
		std::cerr << "\tIP The IP address of the database to connect to." << std::endl;
		std::cerr << "\tLOGFILE name of the log file. If omitted, the log output will be performed to stderr." << std::endl;
		return 1;
	}

	hasSessionName = (argc>=3);
	hasTripName = (argc>=4);
	hasLogFile = (argc == maxNbOfNeededArguments);
	return 0;
}

//======================================================================
//======================================================================
//======================================================================

class CommandLineArguments {
public:
	CommandLineArguments(int argc, const char** argv)
	  : m_hasLogFile(false),
	    m_logFilename(),
	    m_valid(false),
	    m_databaseIp(),
	    m_ipAsStr(),
	    m_userName(),
	    m_password(),
	    m_outputFile(),
	    m_maxFileSize(100000000)
	{
		using namespace boost::program_options;
		variables_map cmdVars;

		options_description desc("IbeoSdkDbInfoDemo\n");
		desc.add_options()
			("ip,I", value<std::string>(&m_ipAsStr)->default_value("127.0.0.1"), "Ip address of the database.")
			("username,U", value<std::string>(&m_userName)->default_value(""), "User name for the database.")
			("password,C", value<std::string>(&m_password)->default_value(""), "Password for the database.")
			("maxfilesize,s", value<off_t>(&m_maxFileSize)->default_value(100000000),
					"Exceeding this size will trigger splitting the output file.")
			("logfile,l", value<std::string>(&m_logFilename),
					"Name of the log file. If omitted, std::cerr will be used.")
			("help,h", "Show this message")
			;

		positional_options_description pos_opts;
		pos_opts.add("ip",1);
		pos_opts.add("username",1);
		pos_opts.add("password",1);
		pos_opts.add("logfile",1);

		try {
			store(command_line_parser(argc, argv).options(desc).positional(pos_opts).run(), cmdVars);
		}
		catch (const boost::program_options::multiple_occurrences&) {
			std::cerr << "*** Command line option has given more than once or too many parameters provided." << std::endl;
			desc.print(std::cerr);
			exit(1);
		}
		catch (const boost::program_options::error& e) {
			std::cerr << "*** Error in command line arguments: " << e.what() << std::endl;
			desc.print(std::cerr);
			exit(1);
		}

		try {
			notify(cmdVars);
		}
		catch (const boost::program_options::error& e) {
			std::cerr << "*** Error in command line arguments: " << e.what() << std::endl;
			desc.print(std::cerr);
			exit(1);
		}

		if (cmdVars.count("help")) {
			desc.print(std::cerr);
			exit(0);
		}

		m_hasLogFile = (cmdVars.count("logfile") > 0);

		boost::system::error_code ec;
		m_databaseIp = boost::asio::ip::address::from_string(m_ipAsStr, ec);
		if (ec) {
			std::cerr << "IP address '" << m_ipAsStr << "' is not valid: " << ec.message() << std::endl;
		}
		else {
			m_valid = true;
		}
	}

public:
	bool isValid() const { return m_valid; }

	bool hasLogFile() const { return m_hasLogFile; }
	const std::string& getLogFileName() const { return m_logFilename; }

	boost::asio::ip::address getDatabaseIp() const { return m_databaseIp; }
	std::string getIpAsString() const { return m_ipAsStr; }

	std::string getUserName() const { return m_userName; }
	std::string getPassword() const { return m_password; }

	const std::string& getOutputFile() const { return m_outputFile; }
	off_t getMaxFileSize() const { return m_maxFileSize; }

protected:
	bool m_hasLogFile;
	std::string m_logFilename;
	bool m_valid;

	boost::asio::ip::address m_databaseIp;
	std::string m_ipAsStr;

	std::string m_userName;
	std::string m_password;

	std::string m_outputFile;
	off_t m_maxFileSize;
}; // CommandLineArguments

//======================================================================
//======================================================================
//======================================================================

void dbInfo(LogFileManager& logFileManager, const CommandLineArguments& cmdargs);

//======================================================================

int main(const int argc, const char** argv)
{
	std::cerr << argv[0] << " Version " << appVersion.toString();
	std::cerr << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;

	CommandLineArguments cmdargs(argc, argv);
	if (!cmdargs.isValid())
		exit(1);

	LogFileManager logFileManager;
	ibeosdk::LogFile::setTargetFileSize(cmdargs.getMaxFileSize());

	if (cmdargs.hasLogFile()) {
		ibeosdk::LogFile::setLogFileBaseName(cmdargs.getLogFileName());
	}
	ibeosdk::LogFile::setTargetFileSize(cmdargs.getMaxFileSize());
	const ibeosdk::LogLevel ll = ibeosdk::logLevelFromString("Info");
	ibeosdk::LogFile::setLogLevel(ll);

	logFileManager.start();

	if (cmdargs.hasLogFile()) {
		logInfo << argv[0] << " Version " << appVersion.toString()
		        << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;
	}

	dbInfo(logFileManager, cmdargs);

	exit(0);
}

//======================================================================

void printDbSessionTrips(std::stringstream& ss, const dbaccess::Session& session)
{
	const TripList& tl = session.getTrips();
	ss << "  " << tl.size() << " Trip(s) found in database for session '" << session.getName() << '\'' << std::endl;

	TripList::const_iterator tlIter = tl.begin();
	for (; tlIter != tl.end(); ++tlIter) {

		std::vector<std::string> idcList = tlIter->getIdcPaths();

		ss << "  Trip: '" << tlIter->getName() << "' consists of " << idcList.size()  << " idc-file(s):" << std::endl;

		std::vector<std::string>::const_iterator idcIter = idcList.begin();
		for (; idcIter != idcList.end(); ++idcIter) {
			ss << "    IDC: '" << *idcIter << "'" << std::endl;
		}
	}
}

//======================================================================

void printDbSessionProcs(std::stringstream& ss, const dbaccess::Session& session)
{
	const ProcList& pl = session.getProcessings();

	ss << "  " << pl.size() << " Processing(s) found in database for session '"
			<< session.getName() << '\'' << std::endl;

	ProcList::const_iterator plItr = pl.begin();
	for (; plItr != pl.end(); ++plItr) {

		const ProcJobList& jList = plItr->getJobList();

		ss << "  Processing: '" << plItr->getName() << "' with ID: '"
				<< plItr->getDbId() << "' contains " << jList.size() << " job(s)." << std::endl;

		ProcJobList::const_iterator jobItr = jList.begin();
		for (; jobItr != jList.end(); ++jobItr) {
			ss << "    Job: " << jobItr->getTypeAsString() << " with ID: '"
					<< jobItr->getDbId() << "'" << std::endl;

			const ResultConfig& resultConf =  jobItr->getResultConfig();
			ss << "      " << resultConf.size() << " Results in following Collection(s):" << std::endl;

			ResultConfig::const_iterator resultItr = resultConf.begin();
			for (; resultItr != resultConf.end(); ++resultItr) {
				ss << "        " << *resultItr << std::endl;
			}
		}
	}
}

////======================================================================
//
//void printDbSessionContent(const dbaccess::SessionList& list)
//{
//	std::stringstream ss;
//	ss << std::endl;
//	ss << "Sessions in database: " << list.size() << std::endl;
//	SessionList::const_iterator itr = list.begin();
//	for (; itr != list.end(); ++itr) {
//		ss << "===================================================" << std::endl;
//		ss << "Session: '" << itr->getName() << "'" << std::endl;
//
//		printDbSessionTrips(ss, *itr);
//		printDbSessionProcs(ss, *itr);
//	}
//	logInfo << ss.str();
//}

//======================================================================

void dbInfo(LogFileManager& logFileManager, const CommandLineArguments& cmdargs)
{
	using namespace ibeosdk;
	using namespace dbaccess;

	initMongoClient();

	const dbaccess::DbConnection::ConnectionProperties prop(cmdargs.getUserName(), cmdargs.getPassword(),cmdargs.getIpAsString(), 27017);
	dbaccess::DbConnection::ConnectionResult m_result;
	try {
		DbConnectionFactory::instance().connect(prop, m_result);
		if (m_result.m_dbErrorCode != DbConnection::ConnectionResult::RfDb_NoError)
			throw m_result;
	}
	catch (const DbConnection::ConnectionResult& error) {
		logError << "Connection failed with error code: '" << error.m_dbErrorCode << std::endl;
		logError << "Error message: " << error.m_dbErrMsg << std::endl;
		return;
	}

	SessionDbReader sessionReader;
	TripDbReader tripReader;
	ProcessingDbReader procReader;

	tripReader.connect();
	procReader.connect();

	Session session("");
	//logInfo << CollectionName(session.getFullDbName(BaseEntity::NotUsed)) << std::endl;
	if(sessionReader.connect(CollectionName(session.getFullDbName(BaseEntity::NotUsed)))) {
		try {
			sessionReader.queryData();
			SessionList& list = sessionReader.getSessionList();
			SessionList::iterator itr = list.begin();
			for (; itr != list.end(); ++itr) {
				tripReader.fillTrips(itr->getTrips());
				procReader.fillProcessings(itr->getProcessings());
			}

			std::stringstream ss;
			ss << std::endl;

			ss << "Sessions in database: " << list.size() << std::endl;
			itr = list.begin();
			for (; itr != list.end(); ++itr) {
				ss << "  ===================================================" << std::endl;
				ss << "  Session: '" << itr->getName() << "'" << std::endl;

				printDbSessionTrips(ss, *itr);
				ss << std::endl;
				printDbSessionProcs(ss, *itr);
			}

			logInfo << ss.str();
		}
		catch (const DatabaseException& e) {
			logError << "Reception failed: '" << e.what() << "'" << std::endl;
		}
	}

	shutdownMongoClient();
}

//======================================================================
