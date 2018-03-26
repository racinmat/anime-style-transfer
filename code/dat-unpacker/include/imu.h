#pragma once
#include <adtf_streaming.h>
#include <QDateTime>
#include <bitset>
#include <fstream>
#include <ibeosdk/Time.hpp>

const tUInt8 SIZE_A = 20;
const tUInt8 SIZE_B = 38;
const tUInt8 SIZE_S = 8;
const tUInt8 CRC1_POS = 22;
const tUInt8 CRC2_POS = 61;
const tUInt8 CRC3_POS = 71;
//const tUInt8 PACKET_SIZE = 71; //without crc

struct imuPacket {
	tUInt8 sync;
	tUInt8 batchA[SIZE_A];
	tUInt8 navigationStatus;
	tUInt8 checksum1;
	tUInt8 batchB[SIZE_B];
	tUInt8 checksum2;
	tUInt8 statusChannel;
	tUInt8 batchS[SIZE_S];
	tUInt8 checksum3;
};

enum imuStatus {
	invalid = 0,
	raw = 1,
	init = 2,
	locking = 3,
	locked = 4,
	reserved = 5,
	expiredFW = 6,
	blockedFW = 7,
	statusOnly = 10,
	internalUse = 11,
	triggerInit = 20,
	triggerLocking = 21,
	triggerLocked = 22
};

enum channelStatus {
	gpsInfo = 0,
	kalmanInnovation1 = 1,
	gnssReceiver = 2,
	positionAccur = 3,
	velocityAccur = 4,
	orientAccur = 5,
	gyroBias = 6,
	accleBias = 7,
	gyroScale = 8,
	gyroBiasAccur = 9,
	accelBiasAccur = 10,
	gyroScaleAccur = 11,
	primAntPosEst = 12,
	dualAntOrEst = 13,
	primAntPosAccur = 14,
	dualAntOrientAccur = 15,
	ins2hostObjRot = 16,
	gnssReceiverSecond = 17,
	imuInfo = 18,
	imuSoftVer = 19,
	diffCorrInfo = 20,
	discSpace = 21,
	rtProcessingInfo = 22,
	sysUpTime = 23,
	asyncPackTriggerFall = 24,
	dummy25,
	outputDisplLeverArm = 26,
	dualAntAmg1 = 27,
	dualAntAmb2 = 28,
	navConfInit = 29,
	osInfo = 30,
	hwCfgInfo = 31,
	kalmanInnovation2 = 32,
	zeroVelLever = 33,
	zeroVelLeverAccur = 34,
	latAdvslip = 35,
	latAdvslipAccur = 36,
	headingMissAngle = 37,
	zeroVeloSet = 38,
	latAdvSlip = 39,
	ncomId = 40,
	outputBaudRate = 41,
	headingLock = 42,
	asyncPackTirggerRise = 43,
	wheelSpeedCfg = 44,
	wheelSpeedCnt = 45,
	wheelSpeedla = 46,
	wheelSpeedlaAccur = 47,
	dop = 48,
	omnistar = 49,
	commDecInfo = 50,
	addsp1 = 51,
	addsp2 = 52,
	addsp3 = 53,
	addsp4 = 54,
	gnssPrimInfo = 55,
	gnssSecondInfo = 56,
	primAntPosEstExt = 57,
	vehicle2OutFrameRot = 58,
	imuDecStat = 59,
	surfAnglesDef = 60,
	gnssExtIntInfo = 61,
	gnssExtInfo = 62,
	angAccFS = 63,
	gnssExtHwCfg = 64,
	asyncPackTriggCamera = 65,
	extLocalDefLL = 66,
	extLocalDefAH = 67,
	addsp5 = 68,
	addsp6 = 69,
	addsp7 = 70,
	addsp8 = 71,
	statusInfo1 = 72,
	statusInfo2 = 73,
	linAccFS = 74,
	misc = 75,
	diffCorrInfo2 = 76,
	diffCorrCfg = 77,
	canBusInfo = 78,
	asyncPackTriggerFall2 = 79,
	asyncPackTirggerRise2 = 80,
	asyncPackTriggCamera2 = 81,
	hwConfInfoXNav = 82,
	statusInfoXNav1 = 83,
	statusInfoXNav2 = 84,
	dummy85,
	dummy86,
	dummy87,
	kalmanInnovation3 = 88,
	vertAdvSlip = 89,
	vertAdvSlipAccur = 90,
	pitchMisAngle = 91,
	vertAdvSlipOpt = 92,
	channelStatusEnd
};

struct _accuracy {
	tFloat32 northPos;
	tFloat32 eastPos;
	tFloat32 downPos;
	tFloat32 northVelo;
	tFloat32 eastVelo;
	tFloat32 downVelo;
	tFloat32 heading;
	tFloat32 pitch;
	tFloat32 roll;

	tUInt8 headingQuality;
	tUInt8 headingSearchType;
	tUInt8 headingSearchStatus;
	tUInt8 headingSearchReady;
	tUInt16 startAmbiguities;
	tUInt16 remainingAmbiguities;

};

struct imuData {
	imuStatus navigationStatus;
	tUInt16 year;
	tUInt8 month;
	tUInt8 day;
	tUInt8 hour;
	tInt8 minute;
	tInt8 second;
	tUInt16 millis;

	ibeosdk::NTPTime ntpTime;

	tFloat32 accX; //m*s-2
	tFloat32 accY;
	tFloat32 accZ;

	tFloat32 rollRate; //rad*s-1
	tFloat32 pitchRate;
	tFloat32 yawRate;

	tFloat64 latitude; //rad
	tFloat64 longitude;
	tFloat32 altitude; //m

	tFloat32 veloNorth; //m*s-1
	tFloat32 veloEast;
	tFloat32 veloDown;

	tFloat32 heading; //rad
	tFloat32 pitch;
	tFloat32 roll;

	//status info
	tUInt8 numOfSatelites;
	tInt8 utcOffset;

	_accuracy accuracy;

	bool valid;
};


//this is the beginning of GPS, time is calculated since
const QDateTime gpsStart = QDateTime(QDate(1980, 1, 6), QTime(0, 0));

class OxtsImuProcessor {

	public:
		OxtsImuProcessor(const tChar *__info);
		virtual ~OxtsImuProcessor();

	private:
		imuData now;
		tUInt32 oxMinutes;
		std::bitset<channelStatusEnd> m_packetStatus;



		bool checkIntegrity(imuPacket &packet);
		void processPacket(imuPacket &packet);
		void processStatusInfo(imuPacket &packet);

		//status functions

		void procGpsInt(tUInt8 *buffer);
		void procPositionAccur(tUInt8 *buffer);
		void procVelocityAccur(tUInt8 *buffer);
		void procOrientAccur(tUInt8 *buffer);
		void procIns2hostObjRot(tUInt8 *buffer);
		void processdualAntAmg1(tUInt8 *buffer);

};
