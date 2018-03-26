//======================================================================
/*! \file TimestampRecvOnlyRecvOnly.cpp
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

#include <ibeosdk/datablocks/snippets/TimestampRecvOnly.hpp>
#include <ibeosdk/datablocks/snippets/ClockType.hpp>

#include <ibeosdk/EcefPoint.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

TimestampRecvOnly::TimestampRecvOnly()
  : m_receivedTimeEcu(NTPTime()),
    m_receivedTime(NTPTime()),
    m_clockType(ClockType(255, ClockType::Clock_Unknown))
{}

//======================================================================

TimestampRecvOnly::TimestampRecvOnly(const NTPTime receivedTimeEcu,
                                     const NTPTime receivedTime,
                                     const ClockType clockType)
  : m_receivedTimeEcu(receivedTimeEcu),
    m_receivedTime(receivedTime),
    m_clockType(clockType)
{}

//======================================================================

TimestampRecvOnly::~TimestampRecvOnly() {}

//======================================================================

std::streamsize TimestampRecvOnly::getSerializedSize() const
{
	int32_t result = 0;
	result += 8 * 2;
	result += int32_t(this->m_clockType.getSerializedSize());
	return result;
}

//======================================================================

bool TimestampRecvOnly::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, this->m_receivedTimeEcu);
	ibeosdk::readBE(is, this->m_receivedTime);
	this->m_clockType.deserialize(is);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool TimestampRecvOnly::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_receivedTimeEcu);
	ibeosdk::writeBE(os, m_receivedTime);
	m_clockType.serialize(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool operator==(const TimestampRecvOnly& ts1, const TimestampRecvOnly& ts2)
{
	return    (ts1.getReceivedTimeEcu() == ts2.getReceivedTimeEcu())
	       && (ts1.getReceivedTime() == ts2.getReceivedTime())
	       && (ts1.getClockType() == ts2.getClockType());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
