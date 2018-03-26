//======================================================================
/*! \file CommandLuxSetNtpTimestampSync.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 13, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/commands/CommandLuxSetNtpTimestampSync.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

CommandLuxSetNtpTimestampSync::CommandLuxSetNtpTimestampSync(const NTPTime timestamp)
  : LuxCommand<CommandId::CmdLuxSetNTPTimestampSync>(),
    m_reserved0(0x00000000UL),
    m_timestamp(timestamp)
{}

//======================================================================

bool CommandLuxSetNtpTimestampSync::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readLE(is, m_commandId);
	readLE(is, m_reserved0);

	// cannot use readLE or readBE here since the long words
	// are in BE order and the seconds and fracs are in LE order
	// internally.
	uint32_t secs;  readLE(is, secs);
	uint32_t fracs; readLE(is, fracs);
	m_timestamp.set(secs, fracs);

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool CommandLuxSetNtpTimestampSync::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeLE(os, m_commandId);
	writeLE(os,m_reserved0);

	// cannot use writeLE or writeBE here since the long words
	// are in BE order and the seconds and fracs are in LE order
	// internally.
	writeLE(os, m_timestamp.getSeconds());
	writeLE(os, m_timestamp.getFracSeconds());

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
