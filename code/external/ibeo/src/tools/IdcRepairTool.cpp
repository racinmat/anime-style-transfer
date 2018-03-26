//======================================================================
/*! \file IdcRepairTool.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 22, 2015
 *
 * Tool which copies an IDC file to fix some problems for ILV compatibility:
 * # Set the SizeOfPrevMsg entry in the first IbeoDataHeader to 0
 * # Add a FrameIndex.
 *
 * Base on IbeoSdkFileDemo.cpp
 *///-------------------------------------------------------------------

#include <ibeosdk/lux.hpp>
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>
#include <ibeosdk/scala.hpp>

#include <ibeosdk/devices/IdcFile.hpp>

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
const std::string info = "IdcRepairTool";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);

IbeoSDK ibeoSDK;

//======================================================================

void file_demo(const std::string& filename);

//======================================================================

TimeConversion tc;

//======================================================================

class TargetFile : public ibeosdk::DataStreamer, public ibeosdk::IdcFile {
public:
	TargetFile(const std::string& filename)
	  : IdcFile()
	{
		create(filename);
	}

	virtual ~TargetFile() {}

public:
	virtual void onData(const IbeoDataHeader& dh, const char* const data)
	{
		this->write(dh, data);
	}
}; // TargetFile

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
		std::cerr << "\tINPUTFILENAME Name of the file to use as input instead of a sensor." << std::endl;
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

	static CustomLogStreamCallbackExample clsce;

	if (!hasLogFile)
		LogFile::setCustomLogStreamCallback(&clsce);

	logFileManager.start();

	if (hasLogFile) {
		logInfo << argv[0] << " Version " << appVersion.toString()
		        << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;
	}

	file_demo(filename);

	exit(0);
}

//======================================================================

void file_demo(const std::string& filename)
{
	IdcFile file;
	file.open(filename);
	if (file.isOpen()) {
		TargetFile allListener(filename+"out.idc");

		file.registerStreamer(&allListener);


		IbeoDataHeader dh;
		unsigned short nbMessages = 0; // # of messages we parsed

		bool foundScan = false;

		while (file.isGood()) {
			boost::shared_ptr<char> db = file.getNextDataBlockRaw(dh);
			if (db.get() == NULL) {
				continue; // might be eof or unknown file type
			}

			if (   (dh.getDataType() == DataTypeId::DataType_IdcTrailer)
			    || (dh.getDataType() == DataTypeId::DataType_FrameIndex)) {
				// do not copy the trailer and the index into the new
				// file. Those datablocks will be automatically generated
				// when the file will be closed.
				continue;
			}



			if (allListener.getFrameIndex().getFramingPolicy().isTrigger(dh.getDataType(), FramingPolicy::deviceIdAny)) {
				foundScan = true;
			}


			if (foundScan) {
				file.notifyStreamers(dh, db);
			}
			++nbMessages;
		}

		logDebug << "EOF reached. " << nbMessages << " known blocks found." << std::endl;
	}
	else {
		logError << "File not readable." << std::endl;
	}
}

//======================================================================
