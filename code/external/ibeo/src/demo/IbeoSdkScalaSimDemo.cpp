//======================================================================
/*! \file IbeoSdkFileDemo.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd))
 * \date Jun 2, 2016
 *
 * Demo project for simulating an Scala device.
 *///-------------------------------------------------------------------

#include <ibeosdk/lux.hpp>
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>
#include <ibeosdk/scala.hpp>

#include <ibeosdk/devices/IdcFile.hpp>
#include <ibeosdk/devices/IbeoTcpIpAcceptorScala.hpp>

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
const std::string info = "IbeoSdkScalaSimDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);

IbeoSDK ibeoSDK;

//======================================================================

TimeConversion tc;

//======================================================================

class ScalaSim {
public:
	ScalaSim(const std::string filename,
	         ibeosdk::LogFileManager* const logFileManager,
	         const boost::asio::deadline_timer::duration_type writeExpirationTime,
	         const unsigned short port = 12004);

public:
	void run();

protected:
	ibeosdk::IbeoTcpIpAcceptorScala m_acceptor;
	std::string m_idcFileName;
	IdcFile m_idcFile;
}; // ScalaSim

//======================================================================

ScalaSim::ScalaSim(const std::string filename,
                   ibeosdk::LogFileManager* const logFileManager,
                   const boost::asio::deadline_timer::duration_type writeExpirationTime,
                   const unsigned short port)
  : m_acceptor(logFileManager, writeExpirationTime, port),
    m_idcFileName(filename),
    m_idcFile()
{
	m_acceptor.init();
}

//======================================================================

void ScalaSim::run()
{
	m_idcFile.open(m_idcFileName);
	if (!m_idcFile.isOpen()) {
		logError << "Cannot open IDC file '" << m_idcFileName << "'" << std::endl;
		return;
	}

	while (!m_acceptor.hasSessions()) {
#	  ifdef _WIN32
		::Sleep(1);
#	  else // _WIN32
		sleep(1);
#	  endif // _WIN32
	}

	logInfo << "Start sending" << std::endl;

	while (m_idcFile.isGood()) {
		const DataBlock* db = m_idcFile.getNextDataBlock();

		if (db) {
			logDebug << "Send data block: " << toHex(db->getDataType()) << std::endl;
			m_acceptor.sendDataBlock(*db);
		}
	} // while

	logInfo << "Finished sending" << std::endl;
}

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
		std::cerr << argv[0] << " " << " INPUTFILENAME [LOGFILE]" << std::endl;
		std::cerr << "\tINPUTFILENAME Name of the file to use as input." << std::endl;
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

	std::string filename = argv[currArg++];

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

	boost::asio::deadline_timer::duration_type writeExpirationTime(boost::posix_time::seconds(10));
	ScalaSim sim(filename, &logFileManager, writeExpirationTime, 12004);
	sim.run();

	exit(0);
}

//======================================================================
