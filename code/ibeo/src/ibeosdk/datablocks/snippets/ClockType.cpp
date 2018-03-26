//======================================================================
/*! \file ClockType.cpp
 *\verbatim
 * ------------------------------------------
 *  (C) 2017 Ibeo Automotive Systems GmbH, Hamburg, Germany
 * ------------------------------------------
 *
 *  Created on: Feb 21, 2017
 *          by: Anil Kumar
 *\endverbatim
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/ClockType.hpp>

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/EcefPoint.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

template<>
inline
void readBE<ClockType::ClockName>(std::istream& is, ClockType::ClockName& value)
{
	uint8_t tmp;
	readBE(is, tmp);
	value = ClockType::ClockName(tmp);
}

//======================================================================

template<>
inline
void writeBE<ClockType::ClockName>(std::ostream& os, const ClockType::ClockName& value)
{
	const uint8_t tmp = value;
	writeBE(os, tmp);
}

//======================================================================

std::string ClockType::getClockNameString(const ClockName cn)
{
	switch (cn) {
		case Clock_Unknown:      return "unknown clock type";
		case Clock_Laserscanner: return "Laserscanner";
		case Clock_ECU:          return "ECU";
		case Clock_CanBus:       return "CAN Bus";
		case Clock_Camera:       return "Camera";
		case Clock_GpsImu:       return "GPS-IMU";
		case Clock_Other:        return "Other";
		default:                 return "undefined clock type";
	}
}

//======================================================================

ClockType::ClockType()
  : m_clockId(0),
    m_clockName(Clock_Unknown)
{}

//======================================================================

ClockType::ClockType(const uint8_t clockId, const ClockName clockName)
  : m_clockId(clockId),
    m_clockName(clockName)
{}

//======================================================================

ClockType::~ClockType() {}

//======================================================================

std::streamsize ClockType::getSerializedSize() const
{
	return 2 * sizeof(uint8_t);
}

//======================================================================

bool ClockType::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, this->m_clockId);
	ibeosdk::readBE(is, this->m_clockName);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ClockType::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_clockId);
	ibeosdk::writeBE(os, m_clockName);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool operator==(const ClockType& clk1, const ClockType& clk2)
{
	return    (clk1.getClockId() == clk2.getClockId())
	       && (clk1.getClockName() == clk2.getClockName());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
