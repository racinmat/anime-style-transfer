#include "imu.h"
using namespace adtfstreaming;

void PrecomputeCRC(unsigned int *apTable) {
	unsigned int i, j, CRC;

	for (i = 0; i < 256; i++) {
		CRC = i;

		for (j = 0; j < 8; ++j) {
			if (CRC & 0x0001) {
				CRC = (CRC >> 1) ^ 0xA6BC;
			}
			else {
				CRC >>= 1;
			}
		}

		apTable[i] = CRC;
	}
}

unsigned int CalcCRC(const unsigned char *aInput, size_t aLength) {

	static unsigned int table[256];
	static bool setup = false;

	if (!setup) {
		PrecomputeCRC(table);
		setup = true;
	}

	unsigned int CRC = 0;

	for (size_t i = 0; i < aLength; i++) {
		CRC = table[(CRC ^ aInput[i]) & 0xFF] ^ (CRC >> 8);
	}

	return (~CRC) & 0xFFFF;
}

bool OxtsImuProcessor::checkIntegrity(imuPacket &packet) {
	//names taken from ncom manual pp 6
	tUInt8 crc1 = 0, crc2 = 0, crc3 = 0;

	//crc1 = calcCRC((tUInt8*)&packet, CRC1_POS);
	//crc2 = calcCRC((tUInt8*)&packet, CRC2_POS);
	crc3 = CalcCRC((tUInt8 *)&packet, CRC3_POS);

	//packets with navigationStatus == 11 are NOT used (manual pp 7)
	return packet.checksum3 == crc3 && packet.navigationStatus != internalUse;
}

inline int getInt24(tUInt8 *buffer, tUInt8 pos) {
	if (buffer[pos + 2] & 0x80) { //negative number
		return (0xff << 24) | (buffer[pos + 2] << 16) | (buffer[pos + 1] << 8) | buffer[pos + 0];
	}
	else {
		return buffer[pos] | buffer[pos + 1] << 8 | buffer[pos + 2] << 16;
	}
}

inline int getInt16(tUInt8 *buffer, tUInt8 pos) {
	return buffer[pos] | buffer[pos + 1] << 8;
}

inline tFloat32 getFloat32(tUInt8 *buffer, tUInt8 pos) {
	return *(tFloat32 *)&buffer[pos];
}

inline tFloat64 getFloat64(tUInt8 *buffer, tUInt8 pos) {
	return *(tFloat64 *)&buffer[pos];
}

void OxtsImuProcessor::processPacket(imuPacket &packet) {
	now.navigationStatus = (imuStatus)packet.navigationStatus;

	//batch A
	int ms = getInt16(packet.batchA, 0);

	if (ms == 0) {
		procGpsInt((tUInt8 *)&++oxMinutes);
	}

	now.second = ms / 1000;
	now.millis = ms - now.second * 1000;

	now.accX      = getInt24(packet.batchA, 2) * 1e-4f;
	now.accY      = getInt24(packet.batchA, 5) * 1e-4f;
	now.accZ      = getInt24(packet.batchA, 8) * 1e-4f;
	now.rollRate  = getInt24(packet.batchA, 11) * 1e-5f;
	now.pitchRate = getInt24(packet.batchA, 14) * 1e-5f;
	now.yawRate   = getInt24(packet.batchA, 17) * 1e-5f;

	//batch B
	now.latitude  = getFloat64(packet.batchB, 0);
	now.longitude = getFloat64(packet.batchB, 8);
	now.altitude  = getFloat32(packet.batchB, 16);
	now.veloNorth = getInt24(packet.batchB, 20) * 1e-4f;
	now.veloEast  = getInt24(packet.batchB, 23) * 1e-4f;
	now.veloDown  = getInt24(packet.batchB, 26) * 1e-4f;
	now.heading   = getInt24(packet.batchB, 29) * 1e-6f;
	now.pitch     = getInt24(packet.batchB, 32) * 1e-6f;
	now.roll      = getInt24(packet.batchB, 35) * 1e-6f;

	processStatusInfo(packet);

	imuData out = now;
	QDateTime correctedTime(QDate(now.year, now.month, now.day), QTime(now.hour, now.minute, now.second, now.millis));
	correctedTime = correctedTime.addSecs(now.utcOffset);
	out.year = correctedTime.date().year();
	out.month = correctedTime.date().month();
	out.day = correctedTime.date().day();
	out.hour = correctedTime.time().hour();
	out.minute = correctedTime.time().minute();
	out.second = correctedTime.time().second();
	out.millis = correctedTime.time().msec();

	out.ntpTime = ibeosdk::NTPTime();
	out.ntpTime.setTime_s_us(correctedTime.toTime_t(), correctedTime.time().msec() * 1000);

	out.valid = m_packetStatus.all();
}

void OxtsImuProcessor::processStatusInfo(imuPacket &packet) {
	channelStatus chStatus = (channelStatus)packet.statusChannel;

	switch (chStatus) {
		case gpsInfo:
			procGpsInt(packet.batchS);
			break;

		case kalmanInnovation1:
			break;

		case gnssReceiver:
			break;

		case positionAccur:
			procPositionAccur(packet.batchS);
			break;

		case velocityAccur:
			procVelocityAccur(packet.batchS);
			break;

		case orientAccur:
			procOrientAccur(packet.batchS);
			break;

		case gyroBias:
			break;

		case accleBias:
			break;

		case gyroScale:
			break;

		case gyroBiasAccur:
			break;

		case accelBiasAccur:
			break;

		case gyroScaleAccur:
			break;

		case primAntPosEst:
			break;

		case dualAntOrEst:
			break;

		case primAntPosAccur:
			break;

		case dualAntOrientAccur:
			break;

		case ins2hostObjRot:
			procIns2hostObjRot(packet.batchS);
			break;

		case gnssReceiverSecond:
			break;

		case imuInfo:
			break;

		case imuSoftVer:
			break;

		case diffCorrInfo:
			break;

		case discSpace:
			break;

		case rtProcessingInfo:
			break;

		case sysUpTime:
			break;

		case asyncPackTriggerFall:
			break;

		case dummy25:
			break;

		case outputDisplLeverArm:
			break;

		case dualAntAmg1:
			processdualAntAmg1(packet.batchS);
			break;

		case dualAntAmb2:
			break;

		case navConfInit:
			break;

		case osInfo:
			break;

		case hwCfgInfo:
			break;

		case kalmanInnovation2:
			break;

		case zeroVelLever:
			break;

		case zeroVelLeverAccur:
			break;

		case latAdvslip:
			break;

		case latAdvslipAccur:
			break;

		case headingMissAngle:
			break;

		case zeroVeloSet:
			break;

		case latAdvSlip:
			break;

		case ncomId:
			break;

		case outputBaudRate:
			break;

		case headingLock:
			break;

		case asyncPackTirggerRise:
			break;

		case wheelSpeedCfg:
			break;

		case wheelSpeedCnt:
			break;

		case wheelSpeedla:
			break;

		case wheelSpeedlaAccur:
			break;

		case dop:
			break;

		case omnistar:
			break;

		case commDecInfo:
			break;

		case addsp1:
			break;

		case addsp2:
			break;

		case addsp3:
			break;

		case addsp4:
			break;

		case gnssPrimInfo:
			break;

		case gnssSecondInfo:
			break;

		case primAntPosEstExt:
			break;

		case vehicle2OutFrameRot:
			break;

		case imuDecStat:
			break;

		case surfAnglesDef:
			break;

		case gnssExtIntInfo:
			break;

		case gnssExtInfo:
			break;

		case angAccFS:
			break;

		case gnssExtHwCfg:
			break;

		case asyncPackTriggCamera:
			break;

		case extLocalDefLL:
			break;

		case extLocalDefAH:
			break;

		case addsp5:
			break;

		case addsp6:
			break;

		case addsp7:
			break;

		case addsp8:
			break;

		case statusInfo1:
			break;

		case statusInfo2:
			break;

		case linAccFS:
			break;

		case misc:
			break;

		case diffCorrInfo2:
			break;

		case diffCorrCfg:
			break;

		case canBusInfo:
			break;

		case asyncPackTriggerFall2:
			break;

		case asyncPackTirggerRise2:
			break;

		case asyncPackTriggCamera2:
			break;

		case hwConfInfoXNav:
			break;

		case statusInfoXNav1:
			break;

		case statusInfoXNav2:
			break;

		case kalmanInnovation3:
			break;

		case vertAdvSlip:
			break;

		case vertAdvSlipAccur:
			break;

		case pitchMisAngle:
			break;

		case vertAdvSlipOpt:
			break;

		default:
			break;
	}

	m_packetStatus.set(chStatus);
}

void OxtsImuProcessor::procGpsInt(tUInt8 *buffer) {
	tUInt32 minutes = *(tUInt32 *)&buffer[0];

	QDateTime datum = gpsStart.addSecs(minutes * 60);

	now.year = datum.date().year();
	now.month = datum.date().month();
	now.day = datum.date().day();
	now.hour = datum.time().hour();
	now.minute = datum.time().minute();
	now.numOfSatelites = buffer[4];

	oxMinutes = minutes;
}

void OxtsImuProcessor::procPositionAccur(tUInt8 *buffer) {
	if (buffer[6] < 150) { //age ok
		now.accuracy.northPos = getInt16(buffer, 0) * 1e-3;
		now.accuracy.eastPos = getInt16(buffer, 2) * 1e-3;
		now.accuracy.downPos = getInt16(buffer, 4) * 1e-3;
	}
}

void OxtsImuProcessor::procVelocityAccur(tUInt8 *buffer) {
	if (buffer[6] < 150) { //age ok
		now.accuracy.northVelo = getInt16(buffer, 0) * 1e-3;
		now.accuracy.eastVelo = getInt16(buffer, 2) * 1e-3;
		now.accuracy.downVelo = getInt16(buffer, 4) * 1e-3;
	}
}

void OxtsImuProcessor::procOrientAccur(tUInt8 *buffer) {
	if (buffer[6] < 150) { //age ok
		now.accuracy.heading = getInt16(buffer, 0) * 1e-5;
		now.accuracy.pitch = getInt16(buffer, 2) * 1e-5;
		now.accuracy.roll = getInt16(buffer, 4) * 1e-5;
	}
}

void OxtsImuProcessor::procIns2hostObjRot(tUInt8 *buffer) {
	tUInt8 offset = buffer[7];

	if (offset & 1) { //validity check manual channel 16
		now.utcOffset = (tInt8)offset >> 1;
	}
}

void OxtsImuProcessor::processdualAntAmg1(tUInt8 *buffer) {
	now.accuracy.headingQuality = buffer[0];
	now.accuracy.headingSearchType = buffer[1];
	now.accuracy.headingSearchStatus = buffer[2];
	now.accuracy.headingSearchReady = buffer[3];
	memcpy(&now.accuracy.startAmbiguities, &buffer[4], 2);
	memcpy(&now.accuracy.remainingAmbiguities, &buffer[6], 2);
	//now.accuracy.startAmbiguities = *(tUInt16*)buffer[4];
	//now.accuracy.remainingAmbiguities = *(tUInt16*)buffer[6];

}
