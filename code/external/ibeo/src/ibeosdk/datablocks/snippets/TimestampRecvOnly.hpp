//======================================================================
/*! \file TimestampRecvOnly.hpp
 *\verbatim
 * ------------------------------------------
 *  (C) 2017 Ibeo Automotive Systems GmbH, Hamburg, Germany
 * ------------------------------------------
 *
 *  Created on: Mar 28, 2017
 *          by: Anil Kumar
 *\endverbatim
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_TIMESTAMPRECVONLY_HPP_SEEN
#define IBEOSDK_TIMESTAMPRECVONLY_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/datablocks/snippets/ClockType.hpp>
#include <ibeosdk/datablocks/snippets/TimestampInterface.hpp>

#include <ibeosdk/Time.hpp>
#include <ibeosdk/Math.hpp>
#include <ibeosdk/Log.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class TimestampRecvOnly : public TimestampInterface {
public:
	TimestampRecvOnly();
	TimestampRecvOnly(const NTPTime receivedTimeEcu,
	                  const NTPTime receivedTime,
	                  const ClockType clockType);
	virtual ~TimestampRecvOnly();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	virtual NTPTime getReceivedTime() const { return m_receivedTime; }
	virtual NTPTime getReceivedTimeEcu() const { return m_receivedTimeEcu; }
	virtual ClockType getClockType() const { return m_clockType; }

	virtual NTPTime getMeasurementTime() const
	{
		logDebug << "Measurement time not available, returning the closest estimate, ReceivedTime" << std::endl;
		return m_receivedTime;
	}

	virtual NTPTime getMeasurementTimeEcu() const
	{
		logDebug << "Measurement time not available, returning the closest estimate, ReceivedTimeECU" << std::endl;
		return m_receivedTimeEcu;
	}

	virtual NTPTime getRawDeviceTime() const
	{
		logDebug << "Not available" << std::endl;
		return false;
	}

	virtual bool hasMeasurementTimeEcu() const { return false; }
	virtual bool hasMeasurementTime() const { return false; }

public:
	void setReceivedTime(const NTPTime& receivedTime) { this->m_receivedTime = receivedTime; }
	void setReceivedTimeEcu(const NTPTime& receivedTimeEcu) { this->m_receivedTimeEcu = receivedTimeEcu; }
	void setClockType(const ClockType& clockType) { this->m_clockType = clockType; }

private:
	NTPTime m_receivedTimeEcu;
	NTPTime m_receivedTime;
	ClockType m_clockType;
}; // TimestampRecvOnly

//======================================================================

bool operator==(const TimestampRecvOnly& ts1, const TimestampRecvOnly& ts2);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_TIMESTAMPRECVONLY_HPP_SEEN

//======================================================================
