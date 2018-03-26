//======================================================================
/**
 * \file GpsImu9001TestSupport.cpp
 *
 * ---------------------------------------------------------------------
 * \copydoc Copyright
 *
 * \date   07.04.2017
 * \author Peter Oltmann (peo)
 */
//======================================================================

#include <unittests/common/GpsImu9001TestSupport.hpp>

#include <ibeosdk/datablocks/snippets/ClockType.hpp>
#include <ibeosdk/datablocks/snippets/Timestamp.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

GpsImu9001TestSupport::GpsImu9001TestSupport()
	: TestSupport()
{}

//======================================================================

GpsImu9001TestSupport::~GpsImu9001TestSupport() {}

//======================================================================

ibeosdk::GpsImu9001 GpsImu9001TestSupport::createGpsImu9001()
{
	ibeosdk::GpsImu9001 gpsImu;
	fillRandomly(gpsImu);
	return gpsImu;
}

//======================================================================

void GpsImu9001TestSupport::fillRandomly(GpsImu9001& gpsImu)
{
	const double latitude = getNonNanDblRandValue();
	const double latitudeSigma = getNonNanDblRandValue();
	const double longitude = getNonNanDblRandValue();
	const double longitudeSigma = getNonNanDblRandValue();
	const double altitude = getNonNanDblRandValue();
	const double altitudeSigma = getNonNanDblRandValue();
	const double courseAngle = getNonNanDblRandValue();
	const double courseAngleSigma = getNonNanDblRandValue();
	const double yawAngle = getNonNanDblRandValue();
	const double yawAngleSigma = getNonNanDblRandValue();
	const double pitchAngle = getNonNanDblRandValue();
	const double pitchAngleSigma = getNonNanDblRandValue();
	const double rollAngle = getNonNanDblRandValue();
	const double rollAngleSigma = getNonNanDblRandValue();
	const double crossAcceleration = getNonNanDblRandValue();
	const double crossAccelerationSigma = getNonNanDblRandValue();
	const double longitudinalAcceleration = getNonNanDblRandValue();
	const double longitudinalAccelerationSigma = getNonNanDblRandValue();
	const double verticalAcceleration = getNonNanDblRandValue();
	const double verticalAccelerationSigma = getNonNanDblRandValue();
	const double velocityNorth = getNonNanDblRandValue();
	const double velocityNorthSigma = getNonNanDblRandValue();
	const double velocityWest = getNonNanDblRandValue();
	const double velocityWestSigma = getNonNanDblRandValue();
	const double velocityUp = getNonNanDblRandValue();
	const double velocityUpSigma = getNonNanDblRandValue();
	const double velocityX = getNonNanDblRandValue();
	const double velocityXSigma = getNonNanDblRandValue();
	const double velocityY = getNonNanDblRandValue();
	const double velocityYSigma = getNonNanDblRandValue();
	const double velocityZ = getNonNanDblRandValue();
	const double velocityZSigma = getNonNanDblRandValue();
	const double rollRate = getNonNanDblRandValue();
	const double rollRateSigma = getNonNanDblRandValue();
	const double yawRate = getNonNanDblRandValue();
	const double yawRateSigma = getNonNanDblRandValue();
	const double pitchRate = getNonNanDblRandValue();
	const double pitchRateSigma = getNonNanDblRandValue();
	const double gpsStatus = getNonNanDblRandValue();
	const UINT8 noOfSatellites = getRandValue<uint8_t>();
	const double gpsDilutionOfPrecisionX = getNonNanDblRandValue();
	const double gpsDilutionOfPrecisionY = getNonNanDblRandValue();
	const double gpsDilutionOfPrecisionZ = getNonNanDblRandValue();
	const double gpsDilutionOfPrecisionVertical = getNonNanDblRandValue();
	const double gpsDilutionOfPrecisionPosition = getNonNanDblRandValue();

	const ibeosdk::ClockType clock(getRandValue<uint8_t>(), ibeosdk::ClockType::ClockName(getRandValue<uint8_t>() % 7 ));
	const NTPTime measEcu(getRandValue<NTPTime>());
	const NTPTime recEcu(getRandValue<NTPTime>());
	const NTPTime meas(getRandValue<NTPTime>());
	const NTPTime rec(getRandValue<NTPTime>());
	const NTPTime raw(getRandValue<NTPTime>());
	const ibeosdk::Timestamp timestamp(measEcu, recEcu, raw, meas, rec, clock);

	GpsImu9001::Source source = GpsImu9001::Source(getRandValue<uint8_t>() % 7 );
	GpsImu9001::INSQuality insQuality = GpsImu9001::INSQuality(getRandValue<uint8_t>() % 3 );

	gpsImu.setLatitude(latitude);
	gpsImu.setLatitudeSigma(latitudeSigma);
	gpsImu.setLongitude(longitude);
	gpsImu.setLongitudeSigma(longitudeSigma);
	gpsImu.setAltitude(altitude);
	gpsImu.setAltitudeSigma(altitudeSigma);
	gpsImu.setCourseAngle(courseAngle);
	gpsImu.setCourseAngleSigma(courseAngleSigma);
	gpsImu.setYawAngle(yawAngle);
	gpsImu.setYawAngleSigma(yawAngleSigma);
	gpsImu.setPitchAngle(pitchAngle);
	gpsImu.setPitchAngleSigma(pitchAngleSigma);
	gpsImu.setRollAngle(rollAngle);
	gpsImu.setRollAngleSigma(rollAngleSigma);
	gpsImu.setCrossAcceleration(crossAcceleration);
	gpsImu.setCrossAccelerationSigma(crossAccelerationSigma);
	gpsImu.setLongitudinalAcceleration(longitudinalAcceleration);
	gpsImu.setLongitudinalAccelerationSigma(longitudinalAccelerationSigma);
	gpsImu.setVerticalAcceleration(verticalAcceleration);
	gpsImu.setVerticalAccelerationSigma(verticalAccelerationSigma);
	gpsImu.setVelocityNorth(velocityNorth);
	gpsImu.setmVelocityNorthSigma(velocityNorthSigma);
	gpsImu.setVelocityWest(velocityWest);
	gpsImu.setVelocityWestSigma(velocityWestSigma);
	gpsImu.setVelocityUp(velocityUp);
	gpsImu.setVelocityUpSigma(velocityUpSigma);
	gpsImu.setVelocityX(velocityX);
	gpsImu.setVelocityXSigma(velocityXSigma);
	gpsImu.setVelocityY(velocityY);
	gpsImu.setVelocityYSigma(velocityYSigma);
	gpsImu.setVelocityZ(velocityZ);
	gpsImu.setVelocityZSigma(velocityZSigma);
	gpsImu.setRollRate(rollRate);
	gpsImu.setRollRateSigma(rollRateSigma);
	gpsImu.setYawRate(yawRate);
	gpsImu.setYawRateSigma(yawRateSigma);
	gpsImu.setPitchRate(pitchRate);
	gpsImu.setPitchRateSigma(pitchRateSigma);
	gpsImu.setGpsStatus(gpsStatus);
	gpsImu.setNoOfSatellites(noOfSatellites);
	gpsImu.setGpsDilutionOfPrecisionX(gpsDilutionOfPrecisionX);
	gpsImu.setGpsDilutionOfPrecisionY(gpsDilutionOfPrecisionY);
	gpsImu.setGpsDilutionOfPrecisionZ(gpsDilutionOfPrecisionZ);
	gpsImu.setGpsDilutionOfPrecisionVertical(gpsDilutionOfPrecisionVertical);
	gpsImu.setGpsDilutionOfPrecisionPosition(gpsDilutionOfPrecisionPosition);

	gpsImu.setTimestamp(timestamp);
	gpsImu.setSource(source);
	gpsImu.setINSQuality(insQuality);
}

//======================================================================

} // unittests
} // ibeosdk

//======================================================================
