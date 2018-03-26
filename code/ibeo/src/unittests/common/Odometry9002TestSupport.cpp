//======================================================================
/*! \file OdometryTestSupport9002.cpp
 *
 * \copydoc Copyright
 * \author Anil Kumar (ank)
 * \date Mar 28, 2017
 *///-------------------------------------------------------------------

//======================================================================

#include <math.h>
#include <unittests/common/Odometry9002TestSupport.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

Odometry9002TestSupport::Odometry9002TestSupport()
  : TestSupport()
{}

//======================================================================

Odometry9002TestSupport::~Odometry9002TestSupport() {}

//======================================================================

ibeosdk::Odometry9002 Odometry9002TestSupport::createOdometry9002()
{
	ibeosdk::Odometry9002 o;
	fillRandomly(o);
	return o;
}

//======================================================================

void Odometry9002TestSupport::fillRandomly(ibeosdk::Odometry9002& od)
{
	const double steeringAngle = getNonNanDblRandValue();
	od.setSteeringAngle(steeringAngle);

	const double steeringWheelAngle = getNonNanDblRandValue();
	od.setSteeringWheelAngle(steeringWheelAngle);

	const double steeringWheelAngleVelocity = getNonNanDblRandValue();
	od.setSteeringWheelAngleVelocity(steeringWheelAngleVelocity);

	const double wheelSpeedFL = getNonNanDblRandValue();
	od.setWheelSpeedFL(wheelSpeedFL);

	const double wheelSpeedFR = getNonNanDblRandValue();
	od.setWheelSpeedFR(wheelSpeedFR);

	const double wheelSpeedRL = getNonNanDblRandValue();
	od.setWheelSpeedRL(wheelSpeedRL);

	const double wheelSpeedRR = getNonNanDblRandValue();
	od.setWheelSpeedRR(wheelSpeedRR);

	const double wheelCircumference = getNonNanDblRandValue();
	od.setWheelCircumference(wheelCircumference);

	const double vehVelocity = getNonNanDblRandValue();
	od.setVehVelocity(vehVelocity);

	const double vehAcceleration = getNonNanDblRandValue();
	od.setVehAcceleration(vehAcceleration);

	const double vehYawRate = getNonNanDblRandValue();
	od.setVehYawRate(vehYawRate);

	const ibeosdk::ClockType clock(getRandValue<uint8_t>(), ibeosdk::ClockType::ClockName(getRandValue<uint8_t>() % 7 ));

	const NTPTime measEcu(getRandValue<NTPTime>());
	const NTPTime recEcu(getRandValue<NTPTime>());
	const NTPTime meas(getRandValue<NTPTime>());
	const NTPTime rec(getRandValue<NTPTime>());
	const NTPTime raw(getRandValue<NTPTime>());
	const ibeosdk::Timestamp timestamp(measEcu, recEcu, raw, meas, rec, clock);
	od.setTimestamp(timestamp);
}

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================
