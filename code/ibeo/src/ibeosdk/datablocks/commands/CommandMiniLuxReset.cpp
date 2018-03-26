//======================================================================
/*! \file CommandMiniLuxReset.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 10, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/commands/CommandMiniLuxReset.hpp>

#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

CommandMiniLuxReset::CommandMiniLuxReset()
  : MiniLuxCommand<CommandId::CmdLuxReset>(),
    m_reserved(0x0000)
{}

//======================================================================

bool CommandMiniLuxReset::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readLE(is, m_commandId);
	readLE(is, m_reserved);

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool CommandMiniLuxReset::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeLE(os, m_commandId);
	writeLE(os, m_reserved);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
