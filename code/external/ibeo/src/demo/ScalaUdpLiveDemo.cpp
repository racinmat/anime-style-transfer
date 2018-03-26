//======================================================================
/*! \file ScalaUdpLiveDemo.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date May 4, 2017
 *
 * Demo project for connecting to a Scala via UDP and process the received
 * data blocks.
 *///-------------------------------------------------------------------

#include <ibeosdk/scala.hpp>
#include <ibeosdk/devices/IbeoScala.hpp>
#include <ibeosdk/IpHelper.hpp>

#include <boost/shared_ptr.hpp>

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
const std::string info = "IbeoSdkScalaLiveDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);
IbeoSDK ibeoSDK;

//======================================================================

void printHelp(const char* name);

bool getArguments(const int argc, const char** argv,
                  bool& useMulticast,
                  std::string& ip,
                  std::string& ifIp,
                  std::string& port,
                  std::string& logFile);

void live_demo(LogFileManager& logFileManager,
               const bool& useMulticast,
               const std::string& ip,
               const std::string& port,
               const boost::asio::ip::address_v4& ifIp = boost::asio::ip::address_v4::any());

//======================================================================

TimeConversion tc;

//======================================================================

class AllScalaListener : public ibeosdk::DataListener<FrameEndSeparator>,
                         public ibeosdk::DataListener<ScanLux>,
                         public ibeosdk::DataListener<Scan2208>,
                         public ibeosdk::DataListener<ScalaFpgaRawData2310>,
                         public ibeosdk::DataListener<ObjectListLux>,
                         public ibeosdk::DataListener<ObjectListScala>,
                         public ibeosdk::DataListener<ObjectListScala2271>,
                         public ibeosdk::DataListener<VehicleStateBasicLux>,
                         public ibeosdk::DataListener<DeviceStatus>,
                         public ibeosdk::DataListener<DeviceStatus6303>,
                         public ibeosdk::DataListener<LogMessageError>,
                         public ibeosdk::DataListener<LogMessageDebug>,
                         public ibeosdk::DataListener<LogMessageNote>,
                         public ibeosdk::DataListener<LogMessageWarning> {
public:
	virtual ~AllScalaListener() {}

public:
	//========================================
	void onData(const FrameEndSeparator* const fes)
	{
		logInfo << std::setw(5) << fes->getSerializedSize() << " Bytes  "
				<< "Frame received: # " << fes->getFrameId() << std::endl;
	}

	//========================================
	void onData(const ScanLux* const scan)
	{
		logInfo << std::setw(5) << scan->getSerializedSize() << " Bytes  "
				<< "Scan 2202 received: # " << scan->getScanNumber()
			<<"  time: " << tc.toString(scan->getStartTimestamp().toPtime(), 3)
			<< std::endl;
	}

	//========================================
	void onData(const Scan2208* const scan)
	{
		logInfo << std::setw(5) << scan->getSerializedSize() << " Bytes  "
				<< "Scan 2208 received: # " << scan->getScanNumber()
			<<"  time: " << tc.toString(scan->getSubScans().at(0).getStartScanTimestamp().toPtime(), 3)
			<< std::endl;
	}

	//=======================================
	void onData(const ScalaFpgaRawData2310* const scan)
	{
		logInfo << std::setw(5) << scan->getSerializedSize() << " Bytes  "
				<< "Scan 2310 received: # " << scan->getHeaderNtpTime() << std::endl;
	}

	//========================================
	void onData(const ObjectListLux* const objs)
	{
		logInfo << std::setw(5) << objs->getSerializedSize() << " Bytes  "
				<< "ObjectList 2221 received. #Obj:" << objs->getNumberOfObjects()
				<< std::endl;
	}

	//========================================
	void onData(const ObjectListScala* const objs)
	{
		logInfo << std::setw(5) << objs->getSerializedSize() << " Bytes  "
				<< "ObjectList 2270 received. Scan: " << objs->getScanNumber()
				<< "  #Obj:" << objs->getNumberOfObjects()
				<< std::endl;

	}

	//========================================

	void onData(const ObjectListScala2271* const objs)
	{
		logInfo << std::setw(5) << objs->getSerializedSize() << " Bytes  "
				<< "ObjectList 2271 received. Scan: " << objs->getScanNumber()
				<< "  ObjLstId: " << int(objs->getObjectListId())
				<< "  #Obj:" << objs->getNumberOfObjects()
				<< std::endl;

	}

	//========================================

	void onData(const VehicleStateBasicLux* const vsb)
	{
		logInfo << std::setw(5) << vsb->getSerializedSize() << " Bytes  "
				<< "VSB for Scan #: " << vsb->getScanNumber() << "  "
				<< tc.toString(vsb->getTimestamp().toPtime()) << std::endl;
	}

	//========================================
	void onData(const DeviceStatus* const deviceStatus)
	{
		logInfo << std::setw(5) << deviceStatus->getSerializedSize() << " Bytes  "
				<< "DevStat received" << std::endl;
	}

	//========================================
	void onData(const DeviceStatus6303* const deviceStatus)
	{
		logInfo << std::setw(5) << deviceStatus->getSerializedSize() << " Bytes  "
				<< "DevStat 0x6303 received" << std::endl;
	}

	//========================================
	virtual void onData(const LogMessageError* const logMsg)
	{
		logInfo << std::setw(5) << logMsg->getSerializedSize() << " Bytes  "
				<< "LogMessage (Error) received: time: " << logMsg->getHeaderNtpTime().toPtime() << ": " << logMsg->getMessage() << std::endl;
	}

	//========================================
	virtual void onData(const LogMessageWarning* const logMsg)
	{
		logInfo << std::setw(5) << logMsg->getSerializedSize() << " Bytes  "
				<< "LogMessage (Warning) received: time: " << logMsg->getHeaderNtpTime().toPtime() << ": " << logMsg->getMessage() << std::endl;
	}

	//========================================
	virtual void onData(const LogMessageNote* const logMsg)
	{
		logInfo << std::setw(5) << logMsg->getSerializedSize() << " Bytes  "
				<< "LogMessage (Note) received: time: " << logMsg->getHeaderNtpTime().toPtime() << ": " << logMsg->getMessage() << std::endl;
	}

	//========================================
	virtual void onData(const LogMessageDebug* const logMsg)
	{
		logInfo << std::setw(5) << logMsg->getSerializedSize() << " Bytes  "
				<< "LogMessage (Debug) received: time: " << logMsg->getHeaderNtpTime().toPtime() << ": " << logMsg->getMessage() << std::endl;
	}
}; // AllScalaListener

//======================================================================
//======================================================================
//======================================================================

void printHelp(const char* name) {
	std::cerr << name << " " << " UDP [LOGFILE]" << std::endl;
	std::cerr << "\t -h, --help, print help" << std::endl;
	std::cerr << "\t--ip is the ip of the multicast group, default is 224.100.100.100" << std::endl;
	std::cerr << "\t--ifIp ip address of interface to use, default is address:any()" << std::endl;
	std::cerr << "\t--port is the udp port the Scala sensor sends to, default is 22017" << std::endl;
	std::cerr << "\t-u Scala sensor uses udp unicast, default if not specified." << std::endl;
	std::cerr << "\t-m Scala sensor uses udp multicast." << std::endl;
	std::cerr << "\t-l LOGFILE name of the log file. If ommitted, the log output will be performed to stderr." << std::endl;
}

//======================================================================

bool getArguments(const int argc, const char** argv,
                  bool& useMulticast,
                  std::string& ip,
                  std::string& ifIp,
                  std::string& port,
                  std::string& logFile)
{
	bool printHelpUsed = false;

	for (int i=0; i<argc; i++) {
		if (std::string(argv[i]) == "--ip") {
			if (i+1 < argc) ip = std::string(argv[i+1]);
		}
		if (std::string(argv[i]) == "--ifIp") {
			if (i+1 < argc) ifIp = std::string(argv[i+1]);
		}
		if (std::string(argv[i]) == "--port") {
			if (i+1 < argc) port = std::string(argv[i+1]);
		}
		if (std::string(argv[i]) == "-m") {
			useMulticast = true;
		}
		if (std::string(argv[i]) == "-l") {
			if (i+1 < argc) logFile = std::string(argv[i+1]);
		}
		if ((std::string(argv[i]) == "-h") || (std::string(argv[i]) == "--help")) {
			printHelpUsed = true;
			printHelp(argv[0]);
		}
	}

	return !printHelpUsed;
}

//======================================================================

int main(const int argc, const char** argv)
{
	std::cerr << argv[0] << " Version " << appVersion.toString();
	std::cerr << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;

	const off_t maxLogFileSize = 1000000;

	bool useMulticast = false;
	std::string port = "22017";
	std::string ip = "224.100.100.100";
	std::string ifIp = "";
	std::string logFile = "";

	if (getArguments(argc, argv, useMulticast, ip, ifIp, port, logFile)) {

		LogFileManager logFileManager;
		ibeosdk::LogFile::setTargetFileSize(maxLogFileSize);

		if (logFile.length()) {
			ibeosdk::LogFile::setLogFileBaseName(logFile);
		}
		const ibeosdk::LogLevel ll = ibeosdk::logLevelFromString("Debug");
		ibeosdk::LogFile::setLogLevel(ll);

		logFileManager.start();

		if (logFile.length()) {
			logInfo << argv[0] << " Version " << appVersion.toString()
					<< "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;
		}

		if (ifIp.length()) {
			live_demo(logFileManager, useMulticast, ip, port, boost::asio::ip::address_v4::from_string(ifIp));
		} else {
			live_demo(logFileManager, useMulticast, ip, port);
		}

	}

	exit(0);
}

//======================================================================

void live_demo(LogFileManager& logFileManager,
               const bool& useMulticast,
               const std::string& ip,
               const std::string& port,
               const boost::asio::ip::address_v4& ifIp)
{
	AllScalaListener allScalaListener;

	uint16_t portVal;
	std::stringstream ss;
	ss.str(port);
	ss >> portVal;

	boost::shared_ptr<IbeoScala> scala;

	if (useMulticast) {
		scala.reset(new IbeoScala(ip, portVal, IbeoTypeEthUdpMulticast(), ifIp));
	} else {
		scala.reset(new IbeoScala(ip, portVal, IbeoTypeEthUdp(), ifIp));
	}

	scala->setLogFileManager(&logFileManager);
	scala->registerListener(&allScalaListener);
	scala->getConnected();

	// Just to keep the program alive
	while (true) {
		if (!scala->isConnected())
			return;
#	  ifdef _WIN32
		::Sleep(1);
#	  else // _WIN32
		sleep(1);
#	  endif // _WIN32
	}
}

//======================================================================
