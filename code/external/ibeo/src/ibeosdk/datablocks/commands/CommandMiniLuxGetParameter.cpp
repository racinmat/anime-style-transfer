//======================================================================
/*! \file CommandMiniLuxGetParameter.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 10, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/commands/CommandMiniLuxGetParameter.hpp>

#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

CommandMiniLuxGetParameter::CommandMiniLuxGetParameter(const ParameterIndex parameterIndex)
  : MiniLuxCommand<CommandId::CmdLuxGetParameter>(),
    m_reserved(0x0000),
    m_parameterIndex(parameterIndex)
{}

//======================================================================

bool CommandMiniLuxGetParameter::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readLE(is, m_commandId);
	readLE(is, m_reserved);
	m_parameterIndex.readLE(is);

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool CommandMiniLuxGetParameter::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeLE(os, m_commandId);
	writeLE(os, m_reserved);
	m_parameterIndex.writeLE(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
