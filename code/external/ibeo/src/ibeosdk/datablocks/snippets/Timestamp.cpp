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
//======================================================================

#include <ibeosdk/datablocks/snippets/Timestamp.hpp>

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/EcefPoint.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

Timestamp::Timestamp()
  : m_measurementTimeEcu(NTPTime()),
    m_receivedTimeEcu(NTPTime()),
    m_rawDeviceTime(NTPTime()),
    m_measurementTime(NTPTime()),
    m_receivedTime(NTPTime()),
    m_clockType(ClockType(255, ClockType::Clock_Unknown))

{}

//======================================================================

Timestamp::Timestamp(const NTPTime measurementTimeEcu,
                     const NTPTime receivedTimeEcu,
                     const NTPTime rawDeviceTime,
                     const NTPTime measurementTime,
                     const NTPTime receivedTime,
                     const ClockType clockType)
  : m_measurementTimeEcu(measurementTimeEcu),
    m_receivedTimeEcu(receivedTimeEcu),
    m_rawDeviceTime(rawDeviceTime),
    m_measurementTime(measurementTime),
    m_receivedTime(receivedTime),
    m_clockType(clockType)
{}

//======================================================================

Timestamp::Timestamp(const NTPTime measurementTimeEcu, const NTPTime receivedTimeEcu)
  : m_measurementTimeEcu(measurementTimeEcu),
    m_receivedTimeEcu(receivedTimeEcu)
{}

//======================================================================

Timestamp::~Timestamp() {}

//======================================================================

std::streamsize Timestamp::getSerializedSize() const
{
	std::streamsize result = 0;
	result += 8 * 5;
	result += this->m_clockType.getSerializedSize();
	return result;
}

//======================================================================

bool Timestamp::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, this->m_measurementTimeEcu);
	ibeosdk::readBE(is, this->m_receivedTimeEcu);
	ibeosdk::readBE(is, this->m_rawDeviceTime);
	ibeosdk::readBE(is, this->m_measurementTime);
	ibeosdk::readBE(is, this->m_receivedTime);
	this->m_clockType.deserialize(is);


	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool Timestamp::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_measurementTimeEcu);
	ibeosdk::writeBE(os, m_receivedTimeEcu);
	ibeosdk::writeBE(os, m_rawDeviceTime);
	ibeosdk::writeBE(os, m_measurementTime);
	ibeosdk::writeBE(os, m_receivedTime);
	m_clockType.serialize(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool operator==(const Timestamp& ts1, const Timestamp& ts2)
{
	return    (ts1.getMeasurementTimeEcu() == ts2.getMeasurementTimeEcu())
	       && (ts1.getReceivedTimeEcu() == ts2.getReceivedTimeEcu())
	       && (ts1.getRawDeviceTime() == ts2.getRawDeviceTime())
	       && (ts1.getMeasurementTime() == ts2.getMeasurementTime())
	       && (ts1.getReceivedTime() == ts2.getReceivedTime())
	       && (ts1.getClockType() == ts2.getClockType());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
