//======================================================================
/*! \file Timestamp.hpp
 *\verbatim
 * ------------------------------------------
 *  (C) 2017 Ibeo Automotive Systems GmbH, Hamburg, Germany
 * ------------------------------------------
 *
 *  Created on: Mar 28, 2017
 *          by: Anil Kumar
 *\endverbatim
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_TIMESTAMP_HPP_SEEN
#define IBEOSDK_TIMESTAMP_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/datablocks/snippets/ClockType.hpp>
#include <ibeosdk/datablocks/snippets/TimestampInterface.hpp>

#include <ibeosdk/Time.hpp>
#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class Timestamp : public ibeosdk::TimestampInterface {
public:
	Timestamp();
	Timestamp(const NTPTime measurementTimeEcu,
	          const NTPTime receivedTimeEcu,
	          const NTPTime rawDeviceTime,
	          const NTPTime measurementTime,
	          const NTPTime receivedTime,
	          const ClockType clockType);
	Timestamp(const NTPTime measurementTimeEcu, const NTPTime receivedTimeEcu);
	virtual ~Timestamp();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	virtual NTPTime getReceivedTime() const { return m_receivedTime; }
	virtual NTPTime getReceivedTimeEcu() const { return m_receivedTimeEcu; }
	virtual ClockType getClockType() const { return m_clockType; }
	virtual NTPTime getMeasurementTime() const { return m_measurementTime ; }
	virtual NTPTime getMeasurementTimeEcu() const { return m_measurementTimeEcu ; }
	virtual NTPTime getRawDeviceTime() const {return m_rawDeviceTime;}
	virtual bool hasMeasurementTimeEcu() const { return true; }
	virtual bool hasMeasurementTime() const { return true; }

public:
	void setReceivedTime(const NTPTime& receivedTime) { this->m_receivedTime = receivedTime; }
	void setReceivedTimeECU(const NTPTime& receivedTimeEcu) { this->m_receivedTimeEcu = receivedTimeEcu; }
	void setRawDeviceTime(const NTPTime& rawDeviceTime) {this->m_rawDeviceTime = rawDeviceTime;}
	void setMeasurementTime(const NTPTime& measurementTime) { this->m_measurementTime = measurementTime; }
	void setMeasurementTimeEcu(const NTPTime& measurementTimeEcu) { this->m_measurementTimeEcu = measurementTimeEcu; }
	void setClockType(const ClockType& clockType) { this->m_clockType = clockType; }

private:
	NTPTime m_measurementTimeEcu;
	NTPTime m_receivedTimeEcu;
	NTPTime m_rawDeviceTime;
	NTPTime m_measurementTime;
	NTPTime m_receivedTime;
	ClockType m_clockType;
}; // Timestamp

//======================================================================

bool operator==(const Timestamp& ts1, const Timestamp& ts2);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_TIMESTAMP_HPP_SEEN

//======================================================================
