//======================================================================
/*! \file IbeoSdkMiniLuxLiveDemo.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (KB)
 * \date Mar 29, 2016
 *
 * Demo project for reading IDC files and process the PointCloudPlane7510.
 *///-------------------------------------------------------------------

#include <ibeosdk/ecu.hpp>
#include <ibeosdk/EcefPoint.hpp>
#include <ibeosdk/devices/IdcFile.hpp>
#include <ibeosdk/datablocks/PointCloudPlane7510.hpp>

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
const std::string info = "IbeoSdkPointCloudPlane7510Demo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);

IbeoSDK ibeoSDK;

//======================================================================

void file_demo(const std::string& filename);

//======================================================================

class Listener : public ibeosdk::DataListener<VehicleStateBasicEcu2808>,
                 public ibeosdk::DataListener<PointCloudPlane7510>
{
public:
	Listener();
	virtual ~Listener() {}

public:
	void onData(const VehicleStateBasicEcu2808* const vsb);
	void onData(const PointCloudPlane7510* const pcp);

	void generatePointCloud(const PointCloudPlane7510* const pcp,
	                        const VehicleStateBasicEcu2808* const vsb);

	void addPoint(const PointProxy& pp,
	              const ReferencePlane& targetRefPlane,
	              const EcefPoint& targetRefPointsEcef,
	              const Matrix3dFloat& invTargetRotMatrix);

protected:
	VehicleStateBasicEcu2808 m_bufferedVehicleState;
	bool m_hasBufferedVehicleState;
}; // Listener

//======================================================================
//======================================================================
//======================================================================

Listener::Listener()
 : m_bufferedVehicleState(),
   m_hasBufferedVehicleState(false)
{}

//======================================================================
//======================================================================
//======================================================================

void Listener::onData(const VehicleStateBasicEcu2808* const vsb)
{
	m_bufferedVehicleState = *vsb;
	m_hasBufferedVehicleState = true;
}

//======================================================================
//======================================================================
//======================================================================

void Listener::onData(const PointCloudPlane7510* const pcp)
{
	if (m_hasBufferedVehicleState)
		generatePointCloud(pcp, &m_bufferedVehicleState);
}

//======================================================================
//======================================================================
//======================================================================

void Listener::generatePointCloud(const PointCloudPlane7510* const pcp,
	                              const VehicleStateBasicEcu2808* const vsb)
{
	ReferencePlane targetRefPlane(*vsb);

	EcefPoint targetRefPointsEcef;

	PositionWgs84::llaToEcef(targetRefPlane.getGpsPoint().getLatitudeInRad(),
	                         targetRefPlane.getGpsPoint().getLongitudeInRad(),
	                         targetRefPointsEcef);

	Matrix3dFloat invTargetRotMatrix = targetRefPlane.getRotationMatrix().inverted();

	PointCloudPlane7510::const_iterator pIter = pcp->begin();
	PointCloudPlane7510::const_iterator end = pcp->end();

	for (; pIter != end; ++pIter) {
		addPoint(*pIter, targetRefPlane, targetRefPointsEcef, invTargetRotMatrix);
	}
}

//======================================================================
//======================================================================
//======================================================================

void Listener::addPoint(const PointProxy& pp,
                        const ReferencePlane& targetRefPlane,
                        const EcefPoint& targetRefPointsEcef,
                        const Matrix3dFloat& invTargetRotMatrix)
{
	const ReferencePlane& originRefPlane = pp.getReferencePlane();

	const Point3dFloat& originOffset = pp.getOffsetFromReferencePoint();
	Point3dFloat targetOffset;

	if (targetRefPlane != pp.getReferencePlane())
		PointBase::transformToShiftedReferencePlaneF3d(originRefPlane,
		                                               targetRefPointsEcef, invTargetRotMatrix,
		                                               originOffset, targetOffset);
	else
		targetOffset = originOffset;
}

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
		Listener listener;

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

//======================================================================


