//======================================================================
/*! \file IbeoSdkMiniLuxLiveDemo.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Oct 5, 2013
 *
 * Demo project for connecting to a MiniLux and process the received
 * data blocks.
 *///-------------------------------------------------------------------

#include <ibeosdk/minilux.hpp>
#include <ibeosdk/IpHelper.hpp>

#include <ibeosdk/datablocks/commands/CommandMiniLuxReset.hpp>
#include <ibeosdk/datablocks/commands/CommandMiniLuxGetStatus.hpp>
#include <ibeosdk/datablocks/commands/CommandMiniLuxGetParameter.hpp>
#include <ibeosdk/datablocks/commands/CommandMiniLuxSetParameter.hpp>
#include <ibeosdk/datablocks/commands/EmptyCommandReply.hpp>
#include <ibeosdk/datablocks/commands/ReplyMiniLuxGetStatus.hpp>
#include <ibeosdk/datablocks/commands/ReplyMiniLuxGetParameter.hpp>
#include <ibeosdk/datablocks/commands/CommandMiniLuxSetNtpTimestampSync.hpp>

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
const std::string info = "IbeoSdkMiniLuxLiveDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);
IbeoSDK ibeoSDK;

//======================================================================

void live_demo(LogFileManager& logFileManager, std::string ip);

//======================================================================

TimeConversion tc;

//======================================================================

class AllMiniLuxListener : public ibeosdk::DataListener<FrameEndSeparator>,
                           public ibeosdk::DataListener<Scan2208> {
public:
	virtual ~AllMiniLuxListener() {}

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
}; // AllMiniLuxListener


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
		std::cerr << "\tIP is the ip address of the Ibeo MiniLux, e.g. 192.168.0.1." << std::endl;
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

//============================================================================

void live_demo(LogFileManager& logFileManager, std::string ip)
{
	AllMiniLuxListener allMiniLuxListener;

	const uint16_t port = getPort(ip, 12006);
	IbeoMiniLux miniLux(ip, port);
	miniLux.setLogFileManager(&logFileManager);

	miniLux.registerListener(&allMiniLuxListener);


	miniLux.getConnected();

	// Just to keep the program alive
	while (true) {
		if (!miniLux.isConnected())
			return;
#		ifdef _WIN32
			::Sleep(1);
#		else // _WIN32
			sleep(1);

//			CommandMiniLuxReset resetCmd;

//			CommandMiniLuxGetStatus getStatus;
//			ReplyMiniLuxGetStatus replyStatus;
//			lux.sendCommand(getStatus, replyStatus);
//			logError << "Status: " << replyStatus.toString() << std::endl;
//
//			CommandMiniLuxGetParameter getParam(ParameterIndex(0x1200));
//			ReplyMiniLuxGetParameter replyParam;
//			lux.sendCommand(getParam, replyParam);
//			logError << "SENSORMOUNTING_X: " << int16_t(replyParam.getParameterData()) << std::endl;
//
//			CommandMiniLuxSetParameter setParam(ParameterIndex(0x1200), ParameterData(1500));
//			ReplyMiniLuxSetParameter replySetParam;
//			lux.sendCommand(setParam, replySetParam);
//
//			lux.sendCommand(getParam, replyParam);
//			logError << "SENSORMOUNTING_X: " << int16_t(replyParam.getParameterData()) << std::endl;
			logError << "========================================================" << std::endl;
			logError << "Current time: " << Time::localTime() << std::endl;
			CommandMiniLuxSetNtpTimestampSync timeSync(NTPTime(Time::localTime()));
			ReplyMiniLuxSetNTPTimestampSync syncReply;
			miniLux.sendCommand(timeSync, syncReply);
#		endif // _WIN32
	}
}

//============================================================================
