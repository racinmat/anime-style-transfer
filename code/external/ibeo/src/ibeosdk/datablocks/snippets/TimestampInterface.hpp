//======================================================================
/*! \file TimestampInterface.hpp
 *\verbatim
 * ------------------------------------------
 *  (C) 2017 Ibeo Automotive Systems GmbH, Hamburg, Germany
 * ------------------------------------------
 *
 *  Created on: Mar 28, 2017
 *          by: Anil Kumar
 *\endverbatim
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_TIMESTAMPINTERFACE_HPP_SEEN
#define IBEOSDK_TIMESTAMPINTERFACE_HPP_SEEN

//======================================================================
#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <ibeosdk/Time.hpp>
#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class TimestampInterface : public ibeosdk::Snippet {

public:
	TimestampInterface(){};
	virtual ~TimestampInterface(){};

public:
	virtual NTPTime getReceivedTime() const = 0;
	virtual NTPTime getMeasurementTime() const = 0;
	virtual NTPTime getReceivedTimeEcu() const = 0;
	virtual NTPTime getMeasurementTimeEcu() const = 0;
	virtual ClockType getClockType() const = 0;
	virtual NTPTime getRawDeviceTime() const = 0;
	virtual bool hasMeasurementTimeEcu() const = 0;
	virtual bool hasMeasurementTime() const = 0;
};

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_TIMESTAMPINTERFACE_HPP_SEEN

//======================================================================
