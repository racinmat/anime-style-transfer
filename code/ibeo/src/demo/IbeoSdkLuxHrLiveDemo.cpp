//======================================================================
/*! \file IbeoSdkLuxHrLiveDemo.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 30, 2015
 *
 * Demo project for connecting to a LuxHr and process the received
 * data blocks.
 *///-------------------------------------------------------------------

#include <ibeosdk/luxhr.hpp>
#include <ibeosdk/IpHelper.hpp>

//#include <ibeosdk/datablocks/commands/CommandMiniLuxReset.hpp>
//#include <ibeosdk/datablocks/commands/CommandMiniLuxGetStatus.hpp>
//#include <ibeosdk/datablocks/commands/CommandMiniLuxGetParameter.hpp>
//#include <ibeosdk/datablocks/commands/CommandMiniLuxSetParameter.hpp>
//#include <ibeosdk/datablocks/commands/EmptyCommandReply.hpp>
//#include <ibeosdk/datablocks/commands/ReplyMiniLuxGetStatus.hpp>
//#include <ibeosdk/datablocks/commands/ReplyMiniLuxGetParameter.hpp>
//#include <ibeosdk/datablocks/commands/CommandMiniLuxSetNtpTimestampSync.hpp>
#include <ibeosdk/datablocks/FrameEndSeparator.hpp>
#include <ibeosdk/datablocks/Scan2208.hpp>

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
const std::string info = "IbeoSdkLuxHrLiveDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);
IbeoSDK ibeoSDK;

//======================================================================

void live_demo(LogFileManager& logFileManager, std::string ip);

//======================================================================

TimeConversion tc;

//======================================================================

class AllLuxHrListener : public ibeosdk::DataListener<FrameEndSeparator>,
                         public ibeosdk::DataListener<Scan2208> {
public:
	virtual ~AllLuxHrListener() {}

public:
	//========================================
	void onData(const FrameEndSeparator* const fes)
	{
		logInfo << "Frame received: # " << fes->getFrameId() << std::endl;
	}

	//========================================
	void onData(const Scan2208* const scan)
	{
		logInfo << "Scan received: # " << scan->getScanNumber()
			<<"  time: " << tc.toString(scan->getSubScans().at(0).getStartScanTimestamp().toPtime(), 3)
			<< std::endl;
		logInfo << "ScStartTs: " << scan->getSubScans().at(0).getStartScanTimestamp().toPtime() << std::endl;
		logInfo << "ScRevTs: " << boost::posix_time::microsec_clock::local_time() << std::endl;
	}
}; // AllLuxHrListener


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
		std::cerr << "\tIP is the ip address of the Ibeo LuxHr, e.g. 192.168.0.1." << std::endl;
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
	const ibeosdk::LogLevel ll = ibeosdk::logLevelFromString("Debug2");
	ibeosdk::LogFile::setLogLevel(ll);

	logFileManager.start();

	if (hasLogFile) {
		logInfo << argv[0] << " Version " << appVersion.toString()
		        << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;
	}

	live_demo(logFileManager, ip);

	exit(0);
}

//============================================================================

void live_demo(LogFileManager& logFileManager, std::string ip)
{
	AllLuxHrListener allLuxHrListener;

	const uint16_t port = getPort(ip, 12008);
	IbeoLuxHr luxHr(ip, port);
	luxHr.setLogFileManager(&logFileManager);

	luxHr.registerListener(&allLuxHrListener);

	luxHr.getConnected();

	// Just to keep the program alive
	while (true) {
		if (!luxHr.isConnected())
			return;
#		ifdef _WIN32
			::Sleep(1);
#		else // _WIN32
			sleep(1);


//			logError << "Current time: " << Time::localTime() << std::endl;
//			CommandMiniLuxSetNtpTimestampSync timeSync(NTPTime(Time::localTime()));
//			ReplyMiniLuxSetNTPTimestampSync syncReply;
//			luxHr.sendCommand(timeSync, syncReply);
#		endif // _WIN32
	}
}

//============================================================================
