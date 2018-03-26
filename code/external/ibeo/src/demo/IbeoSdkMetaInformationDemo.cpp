//======================================================================
/*! \file IbeoSdkMetaInformationDemo.cpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Jun 6, 2016
 *
 * Demo file for reading meta information from an idc-file
 *///-------------------------------------------------------------------

#include <ibeosdk/IbeoSDK.hpp>
#include <ibeosdk/LogFileManager.hpp>
#include <ibeosdk/misc/Version.hpp>
#include <ibeosdk/devices/IdcFile.hpp>
#include <ibeosdk/datablocks/MetaInformationList7110.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationAppBaseConfig.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationEcuId.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationKeywords.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationVersionNumber.hpp>

using namespace ibeosdk;

const ibeosdk::Version::MajorVersion majorVersion(5);
const ibeosdk::Version::MinorVersion minorVersion(2);
const ibeosdk::Version::Revision revision(2);
const ibeosdk::Version::PatchLevel patchLevel;
const ibeosdk::Version::Build build;
const std::string sdkinfo = "IbeoSdkMetaInformationDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, sdkinfo);
IbeoSDK ibeoSDK;

//==============================================================================

void file_demo(const std::string& filename);

//==============================================================================

class MetaInformationListener : public ibeosdk::DataListener<MetaInformationList7110>
{
public:
	MetaInformationListener();
	virtual ~MetaInformationListener() {}

public:
	virtual void onData(const MetaInformationList7110*const dbImpl);
};

//==============================================================================

MetaInformationListener::MetaInformationListener()
{

}

//==============================================================================

void MetaInformationListener::onData(const MetaInformationList7110*const dbImpl)
{
	typedef MetaInformationList7110::MetaInformationMap MetaInfoMap;
	typedef MetaInformationList7110::MetaInformationSPtrVector InfoVector;

	if (!dbImpl)
	{
		std::cerr << "Null pointer sent to onData with type MetaInformationList7110" << std::endl;
		return;
	}

	const MetaInformationList7110 metaList = *dbImpl;
	const MetaInfoMap& metaInfoMap = metaList.getMetaInformationMap();

	logInfo << "The following meta information is provided within the file: " << std::endl;

	for (MetaInfoMap::const_iterator infoMapIter = metaInfoMap.begin(); infoMapIter != metaInfoMap.end(); ++infoMapIter)
	{
		const InfoVector& infoVec = infoMapIter->second;
		logInfo << "  Type: " << infoMapIter->first << std::endl;
		for (InfoVector::const_iterator infoIter = infoVec.begin(); infoIter != infoVec.end(); ++infoIter)
		{
			const MetaInformationSPtr& info = *infoIter;
			switch(info->getType())
			{
				case MetaInformation::MetaInfoType_AppBaseConfig:
				{
					const MetaInformationAppBaseConfig& abconfig = dynamic_cast<const MetaInformationAppBaseConfig&>(*info);
					logInfo << "    " << abconfig.getAppBaseConfig() << std::endl;
					break;
				}
				case MetaInformation::MetaInfoType_EcuId:
				{
					const MetaInformationEcuId& ecuId = dynamic_cast<const MetaInformationEcuId&>(*info);
					logInfo << "    " << ecuId.getEcuId() << std::endl;
					break;
				}
				case MetaInformation::MetaInfoType_Keywords:
				{
					const MetaInformationKeywords& keywords = dynamic_cast<const MetaInformationKeywords&>(*info);
					const MetaInformationKeywords::StringSet& kwSet = keywords.getKeywords();

					std::stringstream stream;
					for (MetaInformationKeywords::StringSet::const_iterator kwIter = kwSet.begin(); kwIter != kwSet.end(); ++kwIter)
						stream << *kwIter << "; " << std::endl;
					logInfo << "    " << stream.str() << std::endl;

					break;
				}
				case MetaInformation::MetaInfoType_VersionNumber:
				{
					const MetaInformationVersionNumber& versionNumber = dynamic_cast<const MetaInformationVersionNumber&>(*info);
					logInfo << "    " << versionNumber.getSoftwareType() << ": "
						<< versionNumber.getMajorVersion() << "."
						<< versionNumber.getMinorVersion() << "."
						<< versionNumber.getPatchVersion() << "."
						<< versionNumber.getExtraString()
						<< std::endl;
					break;
				}
				default:
					break;
			}
		}
	}
}

//==============================================================================


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

//==============================================================================

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
		MetaInformationListener listener;

		file.registerListener(&listener);

		const DataBlock* db = NULL;
		unsigned short nbMessages = 0; // # of messages we parsed

		while (file.isGood()) {
			db = file.getNextDataBlock();
			if (db == NULL) {
				continue; // might be eof or unknown file type
			}
			file.notifyListeners(db);
			++nbMessages;
		}

		logDebug << "EOF reached. " << nbMessages << " known blocks found." << std::endl;
	}
	else {
		logError << "File not readable." << std::endl;
	}
}
