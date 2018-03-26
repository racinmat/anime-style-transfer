/*
 * \file IbeoLaneEvaluationDemo.cpp
 * \copydoc Copyright
 * \author Ruben Jungnickel (rju)
 *  \data 03.08.2015
 *      Demo project for evaluating a DUT producing Lanes
 */

#include <ibeosdk/lux.hpp>
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>
#include <ibeosdk/scala.hpp>

#include <ibeosdk/devices/IdcFile.hpp>
#include <ibeosdk/datablocks/CarriageWayList6970.hpp>
#include <ibeosdk/datablocks/LogPolygonList2dFloat.hpp>
#include <ibeosdk/datablocks/RefObjectListEcuEtDyn.hpp>
#include <ibeosdk/LaneHandler.hpp>

#include <iostream>
#include <limits>
#include <cstdlib>

//======================================================================

using namespace ibeosdk;

//======================================================================

#ifdef _WIN32
#if _MSC_VER < 1800 //VS 2013 is not tested 1900 == VS 2015
float roundf(const float v)
{
	return v < 0.0 ? ceil(v - 0.5F) : floor(v + 0.5F);
}
#endif // before VS 2013
#endif // _WIN32

//======================================================================

const ibeosdk::Version::MajorVersion majorVersion(5);
const ibeosdk::Version::MinorVersion minorVersion(2);
const ibeosdk::Version::Revision revision(2);
const ibeosdk::Version::PatchLevel patchLevel;
const ibeosdk::Version::Build build;
const std::string info = "IbeoSdkLaneEvaluationDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);

IbeoSDK ibeoSDK;

//======================================================================

void file_evaluation(const std::string& dutFilename, const std::string& refFileName);

//======================================================================

TimeConversion tc;

//======================================================================
class LaneFrameStatistic{
public:
	float m_maxErrorRight; // indication for error bars
	float m_maxErrorLeft;
	float m_rightLanePositionRef; // reference LanePosition
	float m_leftLanePositionRef;
	float m_rightLanePosition;//estimated LanePosition
	float m_leftLanePosition;
};
//======================================================================

class LaneEvaluation {
public:
	LaneEvaluation();
	virtual ~LaneEvaluation();

public:
	void process(const VehicleStateBasicEcu2808& vsb, const lanes::CarriageWayList& cw, const LogPolygonList2dFloat& lane);

private:
	void print(const Point2dFloat& sp, const bool inLane, const float dist);


	//return lateral distance to lane // NAN if supportPoint is outside LaneWindow
	float checkSupportLaneSupportPoint(const Point2dFloat& vehCoords, const lanes::LanePosition& egoLane, bool& inLane);

	float calcDistanceToLaneBoundary(const Point2dFloat& vehCoords, const lanes::LanePosition& egoLane, bool left=true);

	void incCounter(const float dist, uint16_t& match, uint16_t& missMatch, const bool inLane);

private:
	static const float maximalLateralEvalDistance;

private:
	lanes::LaneHandler* m_currenLaneHandler;

	uint32_t m_totalLanesExpected;
	uint32_t m_totalLanesMeasured; // measured Lanes Left/right within search window

	std::vector<LaneFrameStatistic> m_frameStatistics;

	std::ofstream m_evalFile;

	std::ofstream m_falseNegativeLeft; //files for explicit eval
	std::ofstream m_falseNegativeRight;
	std::ofstream m_falseNegativeBoth;

	uint32_t m_frameCounter;
}; // LaneEvaluation

//======================================================================

const float LaneEvaluation::maximalLateralEvalDistance=0.35f;

//======================================================================
//======================================================================
//======================================================================

using namespace ibeosdk;

//======================================================================


LaneEvaluation::LaneEvaluation()
  : m_currenLaneHandler(NULL),
    m_totalLanesExpected(0),
    m_totalLanesMeasured(0),
    m_evalFile(),
    m_falseNegativeLeft(),
    m_falseNegativeRight(),
    m_falseNegativeBoth(),
    m_frameCounter(0)
{}

//======================================================================


LaneEvaluation::~LaneEvaluation() {
	// TODO Auto-generated destructor stub
	const float detectionRate=float(m_totalLanesMeasured)/float(m_totalLanesExpected);
	logInfo<<" DetectionRate"<<detectionRate<<std::endl;

	m_evalFile.open("LaneEvalResult.csv", std::ofstream::out | std::ofstream::app);
	m_evalFile<<detectionRate<<std::endl;

	std::vector<LaneFrameStatistic>::const_iterator it =m_frameStatistics.begin();
	for (; it != m_frameStatistics.end(); ++it){
		m_evalFile << it->m_leftLanePosition
				<< ";" << it->m_leftLanePositionRef
				<< ";" <<it->m_maxErrorLeft
				<< ";" << it->m_rightLanePosition
				<< ";" <<it->m_rightLanePositionRef
				<< ";" <<it->m_maxErrorRight
				<< std::endl;
	}

	m_evalFile.close();
}
//======================================================================

void LaneEvaluation::process(const VehicleStateBasicEcu2808& vsb,
                             const lanes::CarriageWayList& cw,
                             const LogPolygonList2dFloat& lane)
{
	logInfo << " start LaneEvaluationProcessing" << std::endl;
	using namespace lanes;

	LaneHandler lh(cw);
	m_currenLaneHandler=&lh;
	LanePosition vehCoordsRef=lh.getLanePosition(vsb);

	//increment TPs
	bool currentLeft = false; // left lane is measured and available
	bool currentRight = false; // right lane is measured and available

	LogPolygonList2dFloat::const_iterator leftLaneB=lane.getLogList().end();
	LogPolygonList2dFloat::const_iterator rightLaneB=lane.getLogList().end();
	float minDistLeft=std::numeric_limits<float>::max();
	float minDistRight=std::numeric_limits<float>::max();

	LogPolygonList2dFloat::const_iterator it = lane.getLogList().begin();
	for (; it != lane.getLogList().end(); ++it) {

		uint16_t foundMatch=0;
		uint16_t foundMissMatch=0;

		Polygon2dFloat::const_iterator ot=it->getPoly().begin();
		for (; ot != it->getPoly().end(); ++ot) {
			bool inLane=false;

			const float dist =checkSupportLaneSupportPoint(*ot, vehCoordsRef, inLane);
			incCounter(dist,foundMatch, foundMissMatch, inLane);
//			print(*ot,inLane,dist); //debug Output
		}

		bool middleInLane;
		const Point2dFloat& middleLaneP=it->getPoly().at(1);
		float middlePointDist=checkSupportLaneSupportPoint(middleLaneP,vehCoordsRef,middleInLane);
		if (middleLaneP.getY()> 0.f) {
			if (minDistLeft>middlePointDist){
				minDistLeft=middlePointDist;
				leftLaneB=it;
			}
		}
		else {
			if (minDistRight>middlePointDist){
				minDistRight=middlePointDist;
				rightLaneB=it;
			}
		}

		if (foundMatch>=2){
			if ((it->getPoly().begin()->getY() >0.f) ){
				currentLeft=true;
			}
			else {
				currentRight = true;
			}
		}

	} // for
	if ((leftLaneB!=lane.getLogList().end()) && (rightLaneB!=lane.getLogList().end())) {
		LaneFrameStatistic stat;
		stat.m_leftLanePositionRef=calcDistanceToLaneBoundary(Point2dFloat(leftLaneB->getPoly().at(1).getX(),0.f),vehCoordsRef,true);
		stat.m_rightLanePositionRef=calcDistanceToLaneBoundary(Point2dFloat(rightLaneB->getPoly().at(1).getX(),0.f),vehCoordsRef,false);
		stat.m_maxErrorLeft=minDistLeft;
		stat.m_maxErrorRight=minDistRight;
		stat.m_leftLanePosition=leftLaneB->getPoly().at(1).getY();
		stat.m_rightLanePosition=rightLaneB->getPoly().at(1).getY();

		m_frameStatistics.push_back(stat);
	}

	m_totalLanesMeasured=(currentLeft)? m_totalLanesMeasured+1:m_totalLanesMeasured;
	m_totalLanesMeasured=(currentRight)? m_totalLanesMeasured+1:m_totalLanesMeasured;
	m_totalLanesExpected+=2;

	++m_frameCounter;
}

//======================================================================

float LaneEvaluation::checkSupportLaneSupportPoint(const Point2dFloat& vehCoords,
                                                   const lanes::LanePosition& egoLane,
                                                   bool& inLane){
	using namespace lanes;
	bool succ;
//	{
//		logInfo << "EgoLane Distance " << egoLane.longitudinalDistance << "/" << egoLane.lateralDistance << "." << std::endl;
//	}

	float latDist=NaN;
	TransformationMatrix2dFloat goalMatrix(vehCoords);

	LanePosition lp=m_currenLaneHandler->getReferenceLanePosition(egoLane, goalMatrix, inLane, succ);

	if (succ) {
		const float width=egoLane.laneSegment->getWidth(vehCoords.getX());
		const float delta=fabsf(fabsf(lp.lateralDistance)-(width*0.5f));

		latDist=delta;

	}
	else {
		latDist=-1.f;
	}

	return latDist;
}

//======================================================================
/**
 * this function calculates the distance to the reference lane Boundary.
 * As default it calculates the distance to the left lane boundary. If left=false the distance to the right lane boundary will be calculated
 */
float LaneEvaluation::calcDistanceToLaneBoundary(const Point2dFloat& vehCoords,
                                                   const lanes::LanePosition& egoLane, bool left){
	using namespace lanes;

	float latDist=NaN;
	TransformationMatrix2dFloat goalMatrix(vehCoords);
	bool inLane, succ;
	LanePosition lp=m_currenLaneHandler->getReferenceLanePosition(egoLane, goalMatrix, inLane, succ);

	if (succ) {
		const float width=egoLane.laneSegment->getWidth(vehCoords.getX());
		const float centerToLaneBoundary=(left)? (width*0.5f) : -(width*0.5f);
		const float delta=-lp.lateralDistance+centerToLaneBoundary;
		latDist=delta;
	}

	return latDist;
}

//======================================================================

void LaneEvaluation::incCounter(const float dist, uint16_t& match, uint16_t& missMatch, const bool inLane)
{
	if (dist<=maximalLateralEvalDistance)
		++match;
	if (inLane && dist>maximalLateralEvalDistance)
		++missMatch;
}

//======================================================================

void LaneEvaluation::print(const Point2dFloat& sp, const bool /*inLane*/, const float dist)
{
	std::stringstream str;

	if (dist<=maximalLateralEvalDistance){
		str<<"dist:"<<roundf(dist*100.f)/100.f<<" SupportPoint:("<<sp.getX()<<"/"<<sp.getY()<<")";

		logInfo << " Point is out of Evaluation distance" << str.str() << std::endl;
	}
}

//======================================================================


class AllListener : public ibeosdk::DataListener<VehicleStateBasicEcu2808>,
                    public ibeosdk::DataListener<LogPolygonList2dFloat>,
                    public ibeosdk::DataListener<CarriageWayList6970> {
public:
	virtual ~AllListener() {}

public:
	//========================================
	void onData(const LogPolygonList2dFloat* const lanes)
	{
		// Copy LaneBoundaries
		m_lastLaneBoundaryList=*lanes;

		if (!m_lastCarriageWay.empty()){
			m_laneEval.process(m_lastVehicleState,m_lastCarriageWay,m_lastLaneBoundaryList);
		}

	}

	void onData(const VehicleStateBasicEcu2808* const vsb)
	{
		//copy VehicleState
		m_lastVehicleState=*vsb;
	}

	void onData(const CarriageWayList6970* const cwl)
	{
		// copy CarriageWay
		m_lastCarriageWay=cwl->getCarriageWayList();
	}

	//========================================


private:
	LaneEvaluation m_laneEval;
	LogPolygonList2dFloat m_lastLaneBoundaryList;
	VehicleStateBasicEcu2808 m_lastVehicleState;
	lanes::CarriageWayList m_lastCarriageWay;
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
	const int minNbOfNeededArguments = 3;
	const int maxNbOfNeededArguments = 4;

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
		std::cerr << argv[0] << " " << " DUTFILENAME REFERENCEFILENAME [LOGFILE]" << std::endl;
		std::cerr << "\tDUTFILENAME Name of the file to use for evaluation (Device under Test)." << std::endl;
		std::cerr << "\tREFERENCEFILENAME Name of the file to use as reference for evaluation test." << std::endl;
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

	const std::string dutFilename = argv[currArg++];
	const std::string refFilename = argv[currArg++];

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

	file_evaluation(dutFilename, refFilename);

	exit(0);
}

//======================================================================

void file_evaluation(const std::string& dutFilename, const std::string& refFileName)
{
	IdcFile dutFile;
	dutFile.open(dutFilename);

	IdcFile refFile;
	refFile.open(refFileName);

	// this assumes that both idc files are in sync
	if (dutFile.isOpen() && refFile.isOpen()) {
		AllListener allListener;
		dutFile.registerListener(&allListener);
		refFile.registerListener(&allListener);

		const DataBlock* db = NULL;
		unsigned short nbMessages = 0; // # of messages we parsed

		DataTypeId frameDataType=DataTypeId::DataType_EcuScan;
		while (dutFile.isGood() && refFile.isGood()) {
			// TODO: in while refFile.isGood has always to be true
			do {
				db = refFile.getNextDataBlock();

				if (db == NULL) {
					continue; // might be eof or unknown file type
				}
				refFile.notifyListeners(db);

				++nbMessages;
			} while (   (   db==NULL
			             && refFile.isGood())
			         || (   refFile.isGood()
			             && db->getDataType() != frameDataType));


			// TODO: in while dutFile.isGood has always to be true
			do {
				db = dutFile.getNextDataBlock();

				if (db == NULL) {
					continue; // might be eof or unknown file type
				}
				dutFile.notifyListeners(db);

				++nbMessages;
			} while(   (   db==NULL
			            && dutFile.isGood())
			        || (   dutFile.isGood()
			            && db->getDataType()!=frameDataType));
		} // while dutFile & refFile are good

		logDebug << "EOF reached. " << nbMessages << " known blocks found." << std::endl;
	} // while dutFile & refFile are open
	else {
		logError << "File not readable." << std::endl;
	}
}

//======================================================================
