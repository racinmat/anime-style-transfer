//======================================================================
/*! \file TimeRecordTestSupport9000.cpp
 *
 * \copydoc Copyright
 * \author Anil Kumar (ank)
 * \date Feb 24, 2017
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/ClockType.hpp>
#include <math.h>
#include <unittests/common/TimeRecord9000TestSupport.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

TimeRecord9000TestSupport::TimeRecord9000TestSupport()
  : TestSupport()
{}

//======================================================================

TimeRecord9000TestSupport::~TimeRecord9000TestSupport() {}

//======================================================================

ibeosdk::TimeRecord9000 TimeRecord9000TestSupport::createTimeRecord9000()
{
	ibeosdk::TimeRecord9000 t;
	fillRandomly(t);
	return t;
}

//======================================================================

void TimeRecord9000TestSupport::fillRandomly(ibeosdk::TimeRecord9000& tr)
{
	const uint8_t nbOfMeasurements = getRandValue<uint8_t>();
	std::vector<NTPTime> internalTime;
	std::vector<NTPTime> externalTime;
	std::vector<ibeosdk::TimeRecord9000::FixMode> fixModeVector;

	for (int i = 0; i < nbOfMeasurements; ++i) {
		const NTPTime genIntTime = getRandValue<NTPTime>();
		internalTime.push_back(genIntTime);
	}

	for (int i = 0; i < nbOfMeasurements; ++i) {
		const NTPTime genExtTime = getRandValue<NTPTime>();
		externalTime.push_back(genExtTime);
	}

	for (int i = 0; i < nbOfMeasurements; ++i) {
		const TimeRecord9000::FixMode fixMode = TimeRecord9000::FixMode(getRandValue<uint8_t>() % 4 );
		fixModeVector.push_back(fixMode);
	}

	tr.setAll(externalTime, internalTime, fixModeVector);

	ibeosdk::ClockType clockInternal(getRandValue<uint8_t>(), ibeosdk::ClockType::ClockName(getRandValue<uint8_t>() % 7 ));
	ibeosdk::ClockType clockExternal(getRandValue<uint8_t>(), ibeosdk::ClockType::ClockName(getRandValue<uint8_t>() % 7 ));
	tr.setInternalClockType(clockInternal);
	tr.setExternalClockType(clockExternal);
}

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================
