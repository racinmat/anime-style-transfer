//======================================================================
/*! \file ReplyLuxGetStatus.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 10, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/commands/ReplyLuxGetStatus.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

bool ReplyLuxGetStatus::Timestamp::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readLE(is, m_YYYY);
	ibeosdk::readLE(is, m_MMDD);
	ibeosdk::readLE(is, m_hhmm);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());

}

//======================================================================

bool ReplyLuxGetStatus::Timestamp::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeLE(os, m_YYYY);
	ibeosdk::writeLE(os, m_MMDD);
	ibeosdk::writeLE(os, m_hhmm);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::string ReplyLuxGetStatus::Timestamp::toString() const
{
	std::stringstream ss;
	ss << std::setfill('0');
	ss << std::hex << std::setw(4) << getYear() << "-"
		<< std::setw(2) << int(getMonth()) << "-"
		<< std::setw(2) << int(getDay()) << " "
		<< std::setw(2) << int(getHour()) << ":"
		<< std::setw(2) << int(getMinute());
	return ss.str();
}

//======================================================================




//======================================================================
//======================================================================
//======================================================================




//======================================================================

ReplyLuxGetStatus::ReplyLuxGetStatus()
  : LuxCommandReply<CommandId::CmdLuxGetStatus>(),
    m_firmwareVersion(0),
    m_fpgaVersion(0),
    m_scannerStatus(0x0000),
    m_reserved1(0x00000000UL),
    m_temperature(0),
    m_serialNumber0(0),
    m_serialNumber1(0),
    m_reserved2(0x0000),
    m_fpgaTimeStamp(),
    m_dspTimestamp()
{}

//======================================================================

bool ReplyLuxGetStatus::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readLE(is, m_commandId);
	readLE(is, m_firmwareVersion);
	readLE(is, m_fpgaVersion);
	readLE(is, m_scannerStatus);
	readLE(is, m_reserved1);
	readLE(is, m_temperature);
	readLE(is, m_serialNumber0);
	readLE(is, m_serialNumber1);
	readLE(is, m_reserved2);
	m_fpgaTimeStamp.deserialize(is);
	m_dspTimestamp.deserialize(is);

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool ReplyLuxGetStatus::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeLE(os, m_commandId);
	writeLE(os, m_firmwareVersion);
	writeLE(os, m_fpgaVersion);
	writeLE(os, m_scannerStatus);
	writeLE(os, m_reserved1);
	writeLE(os, m_temperature);
	writeLE(os, m_serialNumber0);
	writeLE(os, m_serialNumber1);
	writeLE(os, m_reserved2);
	m_fpgaTimeStamp.serialize(os);
	m_dspTimestamp.serialize(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::string ReplyLuxGetStatus::versionToString(const uint16_t version)
{
	std::stringstream ss;

	ss << std::hex << (version >> 12) << '.'
			<< ((version >> 8) & 0xF) << '.'
			<< ((version & 0xF0) >> 4)
			<< (version & 0xF);

	return ss.str();
}

//======================================================================

std::string ReplyLuxGetStatus::toString() const
{
	std::stringstream ss;
	ss << "FV: " << versionToString(m_firmwareVersion) << ", "
		<< "DV: " << versionToString(m_fpgaVersion) << ", "
		<< "SS: " << toHex(m_scannerStatus) << ", "
		<< "T: " << getTemperatureDeg() << "°, "
		<< "S0: " << toHex(m_serialNumber0) << ", "
		<< "S1: " << toHex(m_serialNumber1) << ", "
		<< "FT: " << m_fpgaTimeStamp.toString() << ", "
		<< "DT: " << m_dspTimestamp.toString();
	return ss.str();
}

//======================================================================

}// namespace ibeosdk

//======================================================================
