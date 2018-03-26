//======================================================================
/*! \file IbeoSdkDbReplayDemo.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 01, 2016
 *
 * Demo project for reading data from a MongoDB database and process the data blocks.
 *
 * Example parameters for info mode and write to logfile
 * -I "127.0.0.1" -U "" -C "" -S "" -T "" -P "" -l log
 *
 * Example parameters for replay data
 * -I "127.0.0.1" -U "" -C "" -S "SessionName" -T "TripName" -P "ProcessingName"
 *
 *///-------------------------------------------------------------------

#include <ibeosdk/lux.hpp>
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>
#include <ibeosdk/scala.hpp>

#include <ibeosdk/database/initMongoClient.hpp>
#include <ibeosdk/devices/DatabaseReplayer.hpp>
#include <ibeosdk/devices/database/CanMessageDbReader.hpp>
#include <ibeosdk/devices/database/ImageDbReader.hpp>
#include <ibeosdk/devices/database/MeasurementList2821DbReader.hpp>
#include <ibeosdk/devices/database/ObjectListDbReader.hpp>
#include <ibeosdk/devices/database/ScanDbReader.hpp>
#include <ibeosdk/devices/database/VehicleStateDbReader.hpp>

#include <boost/program_options.hpp>

#include <iostream>
#include <cstdlib>

//======================================================================

using namespace ibeosdk;

//======================================================================

const ibeosdk::Version::MajorVersion majorVersion(5);
const ibeosdk::Version::MinorVersion minorVersion(2);
const ibeosdk::Version::Revision revision(2);
const ibeosdk::Version::PatchLevel patchLevel;
const ibeosdk::Version::Build build;
const std::string info = "IbeoSdkDbReplayDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);

IbeoSDK ibeoSDK;

//======================================================================

TimeConversion tc;

//======================================================================

class AllListener : public ibeosdk::DataListener<ScanEcu>,
                    public ibeosdk::DataListener<ObjectListEcuEtDyn>,
                    public ibeosdk::DataListener<Image>,
                    public ibeosdk::DataListener<PositionWgs84_2604>,
                    public ibeosdk::DataListener<MeasurementList2821>,
                    public ibeosdk::DataListener<VehicleStateBasicEcu2808>,
                    public ibeosdk::DataListener<CanMessage> {
public:
	//========================================
	void onData(const ScanEcu* const scan)
	{
		logInfo << std::setw(5) << scan->getSerializedSize() << " Bytes  "
				<< "ScanEcu received: # " << scan->getScanNumber()
				<< "  #Pts: " << scan->getNumberOfScanPoints()
				<< "  ScanStart: " << tc.toString(scan->getStartTimestamp().toPtime(), 3)
				<< std::endl;
	}

	//========================================
	void onData(const ObjectListEcuEtDyn* const objList)
	{
		logInfo << std::setw(5) << objList->getSerializedSize() << " Bytes  " << "ObjectListEcUEtDyn received: time "
				<< tc.toString(objList->getTimestamp().toPtime(), 3) << "  with " << objList->getNbOfObjects() << " elements." << std::endl;
	}

	//========================================
	void onData(const Image* const image)
	{
		logInfo << std::setw(5) << image->getSerializedSize() << " Bytes  " << "Image received: time: " << tc.toString(image->getTimestamp().toPtime(), 3) << std::endl;
	}

	//========================================
	void onData(const PositionWgs84_2604* const wgs84)
	{
		logInfo << std::setw(5) << wgs84->getSerializedSize() << " Bytes  "
				<< "PositionWGS84 received: time: " << tc.toString(wgs84->getPosition().getTimestamp().toPtime(), 3)
				<< std::endl;
	}

	//========================================
	void onData(const VehicleStateBasicEcu2808* const vsb)
	{
		logInfo << std::setw(5) << vsb->getSerializedSize() << " Bytes  "
				<< "VSB (ECU) received: time: " << tc.toString(vsb->getTimestamp().toPtime(), 3)
				<< std::endl;
	}
	//========================================
	void onData(const MeasurementList2821* const ml)
	{
		logInfo << std::setw(5) << ml->getSerializedSize() << " Bytes  "
				<< "MeasurementList received: time: " << tc.toString(ml->getTimestamp().toPtime(), 3)
				<< " LN: '" << ml->getListName() << "' GN: '" << ml->getGroupName() << "'" << "Num: " << ml->getMeasList().getSize()
				<< std::endl;

//		typedef std::vector<Measurement> MLVector;
//
//		MLVector::const_iterator itMl = ml->getMeasList().getMeasurements().begin();
//		int ctr = 0;
//		for (; itMl != ml->getMeasList().getMeasurements().end(); ++itMl, ++ctr)
//		{
//			logInfo << " M" << ctr << ":" << (*itMl) << std::endl;
//		}
	}

	void onData(const CanMessage* const cm)
	{
		logInfo << std::setw(5) << cm->getSerializedSize() << " Bytes  "
				<< "CanMessage received: time: " << tc.toString(cm->getTimestamp().toPtime(), 3)
				<< std::endl;
	}
}; // AllListener

//======================================================================
//======================================================================
//======================================================================

class CustomLogStreamCallbackExample : public CustomLogStreamCallback {
public:
	virtual ~CustomLogStreamCallbackExample() {}
public:
	virtual void onLineEnd(const char* const s, const int)
	{
		std::cerr << s << std::endl;
	}
}; // CustomLogStreamCallback

//======================================================================
//======================================================================
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
	    m_sessionName(),
	    m_tripName(),
	    m_processing(),
	    m_outputFile(),
	    m_maxFileSize(100000000)
	{
		using namespace boost::program_options;
		variables_map cmdVars;

		options_description desc("IbeoSdkDbReplayDemo\n");
		desc.add_options()
			("ip,I", value<std::string>(&m_ipAsStr)->default_value("127.0.0.1"), "Ip address of the database.")
			("username,U", value<std::string>(&m_userName)->default_value(""), "User name for the database.")
			("password,C", value<std::string>(&m_password)->default_value(""), "Password for the database.")
			("sessionname,S", value<std::string>(&m_sessionName)->default_value(""), "Name of the session in the database.")
			("tripname,T", value<std::string>(&m_tripName)->default_value(""), "Name of the trip in the database.")
			("processing,P", value<std::string>(&m_processing)->default_value(""), "Name of the processing in the database.")
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
		pos_opts.add("sessionname",1);
		pos_opts.add("tripname",1);
		pos_opts.add("processing",1);
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
	std::string getSessionName() const { return m_sessionName; }
	std::string getTripName() const { return m_tripName; }
	std::string getProcessing() const { return m_processing; }

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
	std::string m_sessionName;
	std::string m_tripName;
	std::string m_processing;

	std::string m_outputFile;
	off_t m_maxFileSize;
}; // CommandLineArguments

//======================================================================
//======================================================================
//======================================================================

void dbReplay_demo(LogFileManager& logFileManager, const CommandLineArguments& cmdargs);

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

	static CustomLogStreamCallbackExample clsce;

	if (!cmdargs.hasLogFile())
		LogFile::setCustomLogStreamCallback(&clsce);

	logFileManager.start();

	if (cmdargs.hasLogFile()) {
		logInfo << argv[0] << " Version " << appVersion.toString()
		        << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;
	}

	dbReplay_demo(logFileManager, cmdargs);

	exit(0);
}

//======================================================================

void dbReplay_demo(LogFileManager& logFileManager, const CommandLineArguments& cmdargs)
{
	dbaccess::initMongoClient();

	dbaccess::DbConnection::ConnectionProperties prop(cmdargs.getUserName(), cmdargs.getPassword(),cmdargs.getIpAsString(), 27017);

	AllListener allListener;

	ImageDbReader imgReader;
	MeasurementList2821DbReader measListReader;
	CanMessageDbReader canMsgReader;
	VehicleStateDbReader vhsReader;
	ScanDbReader scanReader;

	ObjectListDbReader objListReader;
	objListReader.useProcessedData();

	DatabaseReplayer dbReader(prop, cmdargs.getSessionName(), cmdargs.getTripName(), cmdargs.getProcessing());
	dbReader.registerListener(&allListener);

	dbReader.registerDbDataBlockReader(imgReader);
	dbReader.registerDbDataBlockReader(measListReader);
	dbReader.registerDbDataBlockReader(canMsgReader);
	dbReader.registerDbDataBlockReader(vhsReader);
	dbReader.registerDbDataBlockReader(scanReader);
	dbReader.registerDbDataBlockReader(objListReader);

	dbReader.getConnected();

	// Just to keep the program alive
	while (true) {
		if (!dbReader.isConnected()) {
			dbaccess::shutdownMongoClient();
			return;
		}
#		ifdef _WIN32
			::Sleep(1);
#		else // _WIN32
			sleep(1);
#		endif // _WIN32
	}
}

//======================================================================
