//======================================================================
/*! \file IbeoSdkCarriageWayList6970Demo.cpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (StK)
 * \date March 11, 2015
 *
 * Demo project for reading IDC files and process the CarriageWayList6970.
 * \sa IbeoSdkFileDemo.cpp
 *///-------------------------------------------------------------------

#include <ibeosdk/lux.hpp>
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>
#include <ibeosdk/scala.hpp>
#include <ibeosdk/datablocks/CarriageWayList6970.hpp>
#include <ibeosdk/datablocks/RefObjectListEcuEtDyn.hpp>

#include <ibeosdk/devices/IdcFile.hpp>
#include <ibeosdk/LaneHandler.hpp>

#include <iostream>
#include <cstdlib>
#include <fstream>


//======================================================================

using namespace ibeosdk;

//======================================================================

const ibeosdk::Version::MajorVersion majorVersion(5);
const ibeosdk::Version::MinorVersion minorVersion(2);
const ibeosdk::Version::Revision revision(2);
const ibeosdk::Version::PatchLevel patchLevel;
const ibeosdk::Version::Build build;
const std::string info = "IbeoSdkCarriageWayList6970Demo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);

IbeoSDK ibeoSDK;

//======================================================================

void file_demo(const std::string& filename);

//======================================================================

TimeConversion tc;

//======================================================================

class AllListener : public ibeosdk::DataListener<RefObjectListEcuEtDyn>,
                    public ibeosdk::DataListener<VehicleStateBasicEcu2808>,
                    public ibeosdk::DataListener<CarriageWayList6970> {
public:
	AllListener()
	  : m_bufferedVehicleState(),
	    m_bufferedObjectList(),
	    m_hasBufferedVehicleState(false),
	    m_hasBufferedObjectList(false)
	{}

	virtual ~AllListener() {}

public:
	//========================================
	void onData(const RefObjectListEcuEtDyn* const objList)
	{
		m_bufferedObjectList = *objList;
		m_hasBufferedObjectList = true;
	}

	//========================================
	void onData(const VehicleStateBasicEcu2808* const vsb)
	{
		m_bufferedVehicleState = *vsb;
		m_hasBufferedVehicleState = true;
	}

	//========================================

	void onData(const CarriageWayList6970* const cwl)
	{
		std::string tab = "        ";

		// wait until a valid vehicle state and an object list are available
		if (m_hasBufferedVehicleState) {
			lanes::CarriageWayList list = cwl->getCarriageWayList();
			const lanes::LaneHandler lh(list);

			// compute ego position in lane
			lanes::LanePosition egoPosition = lh.getLanePosition(m_bufferedVehicleState);

			// check if valid lane segment was found
			if (egoPosition.laneSegment) {
				logInfo << "Ego lateral offset is: " << std::setprecision(2) << egoPosition.lateralDistance << " m" << std::endl;

				if (m_hasBufferedObjectList) {
					// compute object distances
					std::vector<ObjectEcuEtDyn>::const_iterator it = m_bufferedObjectList.getObjects().begin();
					for (; it != m_bufferedObjectList.getObjects().end(); ++it) {
						// create transformation matrix of object relative to ego vehicle
						Point2dFloat objectReferencePoint = it->convertRefPoint(RPL_RearCenter);
						TransformationMatrix2dFloat objSystem(it->getCourseAngle(), objectReferencePoint);
						bool inside_ego, success;

						std::vector<Measurement> mml = it->getMeasurementList().getMeasurements();
						float latDis = .0f;
						bool inside_egoLane = .0f;

						// extract Oela information stored in Objects
						std::vector<Measurement>::iterator itmm = mml.begin();
						for (; itmm != mml.end(); ++itmm) {
							if (itmm->getKey() == MeasurementKey::Oela_LateralEgoLaneDistance)
								latDis = itmm->getData<float>();
							else if (itmm->getKey() == MeasurementKey::Oela_EgoLaneFlag)
								inside_egoLane = itmm->getData<bool>();
						} // for itmm

						lanes::LanePosition objPosition = lh.getReferenceLanePosition(egoPosition, objSystem, inside_ego, success);

						if (success) {
							logInfo << "Object " << it->getObjectId() <<": " << std::endl;
							logInfo << tab <<"lateral lane distance:      " << std::setprecision(2) << objPosition.lateralDistance << " m" << std::endl;
							logInfo << tab <<"longitudinal lane distance: " << std::setprecision(2) << objPosition.longitudinalDistance << " m" << std::endl;
							logInfo << tab <<"orientation in lane:        " << std::setprecision(2) << objPosition.transformationMatrix.getRotation().getAngle() / M_PI * 180 << " deg" << std::endl;
							logInfo << tab <<"inside ego vehicle lane:    " << inside_ego << std::endl;
							logInfo << std::endl;
							logInfo << tab <<"lateral lane distance (stored): " << std::setprecision(2) << latDis << " m" << std::endl;
							logInfo << tab <<"inside ego vehicle lane (stored): " << std::setprecision(2) << inside_egoLane << std::endl;
						} // if success
					} // for all ref objects
				} // if obj list has been initialized
			} // if valid lane segment was found
		} // if valid vehicle state and an object list are available
	}

protected:
	VehicleStateBasicEcu2808 m_bufferedVehicleState;
	RefObjectListEcuEtDyn m_bufferedObjectList;

	bool m_hasBufferedVehicleState;
	bool m_hasBufferedObjectList;
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
		std::cerr << argv[0] << " " << " INPUTFILENAME STREAMFILE [LOGFILE]" << std::endl;
		std::cerr << "\tINPUTFILENAME Name of the file to use as input instead of a sensor." << std::endl;
		std::cerr << "\tSTREAMFILE Name of the file to use as output for points info (csv file)." << std::endl;
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
		AllListener allListener;

		file.registerListener(&allListener);

		const DataBlock* db = NULL;
		unsigned short nbMessages = 0; // # of messages we parsed

		while (file.isGood()) {
			db = file.getNextDataBlock();
			if (db == NULL) {
				continue; // might be eof or unknown file type
			}
			file.notifyListeners(db);
			++nbMessages;
		} // while file is good

		logDebug << "EOF reached. " << nbMessages << " known blocks found." << std::endl;
	} // if file is open
	else {
		logError << "File not readable." << std::endl;
	}
}

//======================================================================
