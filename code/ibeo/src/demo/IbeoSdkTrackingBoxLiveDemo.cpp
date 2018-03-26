//======================================================================
/*! \file IbeoSdkTrackingBoxLiveDemo.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Oct 23, 2015
 *
 * Demo project for connecting to a LUX and process the received
 * data blocks.
 *///-------------------------------------------------------------------

#include <ibeosdk/trackingbox.hpp>
#include <ibeosdk/IpHelper.hpp>

#include <ibeosdk/datablocks/commands/CommandEcuAppBaseStatus.hpp>
#include <ibeosdk/datablocks/commands/ReplyEcuAppBaseStatus.hpp>
#include <ibeosdk/datablocks/commands/CommandEcuAppBaseCtrl.hpp>
#include <ibeosdk/datablocks/commands/EmptyCommandReply.hpp>

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
const std::string info = "IbeoSdkTrackingBoxLiveDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);
IbeoSDK ibeoSDK;

//======================================================================

void live_demo(LogFileManager& logFileManager, std::string ip);

//======================================================================

TimeConversion tc;

//======================================================================

class AllTrackingBoxListener : public ibeosdk::DataListener<ScanLux>,
                               public ibeosdk::DataListener<ObjectListLux>,
                               public ibeosdk::DataListener<VehicleStateBasicLux>,
                               public ibeosdk::DataListener<LogMessageError>,
                               public ibeosdk::DataListener<LogMessageDebug>,
                               public ibeosdk::DataListener<LogMessageNote>,
                               public ibeosdk::DataListener<LogMessageWarning> {
public:
	virtual ~AllTrackingBoxListener() {}

public:
	//========================================
	void onData(const ScanLux* const scan)
	{
		logInfo << "Scan received: # " << scan->getScanNumber()
			<<"  time: " << tc.toString(scan->getStartTimestamp().toPtime(), 3)
			<< std::endl;
	}

	//========================================
	void onData(const ObjectListLux* const pObj)
	{
		logInfo << "Objects received: # " << pObj->getNumberOfObjects() << std::endl;
	}

	//========================================
	void onData(const VehicleStateBasicLux* const vsb)
	{
		logInfo << "VSB for Scan #: " << vsb->getScanNumber() << "  "
				<< tc.toString(vsb->getTimestamp().toPtime()) << std::endl;
	}

	//========================================
	virtual void onData(const LogMessageError* const logMsg)
	{
		logInfo << std::setw(5) << logMsg->getSerializedSize() << " Bytes  "
				<< "LogMessage (Error) received: time: " << logMsg->getTraceLevel() << ": " << logMsg->getMessage() << std::endl;
	}

	//========================================
	virtual void onData(const LogMessageWarning* const logMsg)
	{
		logInfo << std::setw(5) << logMsg->getSerializedSize() << " Bytes  "
				<< "LogMessage (Warning) received: time: " << logMsg->getTraceLevel() << ": " << logMsg->getMessage() << std::endl;
	}

	//========================================
	virtual void onData(const LogMessageNote* const logMsg)
	{

		logInfo << std::setw(5) << logMsg->getSerializedSize() << " Bytes  "
				<< "LogMessage (Note) received: time: " << logMsg->getTraceLevel() << ": " << logMsg->getMessage() << std::endl;
	}

	//========================================
	virtual void onData(const LogMessageDebug* const logMsg)
	{
		logInfo << std::setw(5) << logMsg->getSerializedSize() << " Bytes  "
				<< "LogMessage (Debug) received: time: " << logMsg->getTraceLevel() << ": " << logMsg->getMessage() << std::endl;
	}
}; // AllTrackingBoxListener


//======================================================================
//======================================================================
//======================================================================

int checkArguments(const int argc, const char** argv, bool& hasLogFile)
{
	const int minNbOfNeededArguments = 2;
	const int maxNbOfNeededArguments = 3;

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
		std::cerr << "\tIP is the ip address of the LUX sensor, e.g. 192.168.0.1." << std::endl;
		std::cerr << "\tLOGFILE name of the log file. If ommitted, the log output will be performed to stderr." << std::endl;
		return 1;
	}

	hasLogFile = (argc == maxNbOfNeededArguments);
	return 0;
}

//======================================================================

int main(const int argc, const char** argv)
{
	std::cerr << argv[0] << " Version " << appVersion.toString();
	std::cerr << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;

	bool hasLogFile;
	const int checkResult = checkArguments(argc, argv, hasLogFile);
	if (checkResult != 0)
		exit(checkResult);
	int currArg = 1;

	std::string ip = argv[currArg++];

	const off_t maxLogFileSize = 1000000;

	LogFileManager logFileManager;
	ibeosdk::LogFile::setTargetFileSize(maxLogFileSize);

	if (hasLogFile) {
		ibeosdk::LogFile::setLogFileBaseName(argv[currArg++]);
	}
	const ibeosdk::LogLevel ll = ibeosdk::logLevelFromString("Debug");
	ibeosdk::LogFile::setLogLevel(ll);

	logFileManager.start();

	if (hasLogFile) {
		logInfo << argv[0] << " Version " << appVersion.toString()
		        << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;
	}

	live_demo(logFileManager, ip);

	exit(0);
}

//======================================================================

void live_demo(LogFileManager& logFileManager, std::string ip)
{
	AllTrackingBoxListener allListener;

	const uint16_t port = getPort(ip, 12002);
	IbeoTrackingBox trackingBox(ip, port);
	trackingBox.setLogFileManager(&logFileManager);

	trackingBox.registerListener(&allListener);

	trackingBox.getConnected();

	// Just to keep the program alive
	while (true) {
		if (!trackingBox.isConnected())
			return;
#		ifdef _WIN32
			::Sleep(1);
#		else // _WIN32
			sleep(1);
#		endif // _WIN32
	}
}

//======================================================================
