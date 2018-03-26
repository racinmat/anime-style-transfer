//======================================================================
/*! \file CommandEcuAppBaseStatus.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 7, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/commands/CommandEcuAppBaseStatus.hpp>

#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

static
void readBE(std::istream& is, CommandManagerAppBaseStatus::AppBaseStatusId& statusId)
{
	uint16_t tmp;
	readBE(is, tmp);
	statusId = CommandManagerAppBaseStatus::AppBaseStatusId(tmp);
}

//======================================================================

static
void writeBE(std::ostream& os, const CommandManagerAppBaseStatus::AppBaseStatusId statusId)
{
	writeBE(os, uint16_t(statusId));
}

//======================================================================

CommandManagerAppBaseStatus::CommandManagerAppBaseStatus()
  : EcuCommand<CommandId::CmdManagerAppBaseStatus>(),
    AppBaseStatusDefinitions(),
    m_statusId(AppBaseStatusId_Recording)
{}

//======================================================================

bool CommandManagerAppBaseStatus::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readBE(is, m_commandId);
	readBE(is, m_statusId);

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool CommandManagerAppBaseStatus::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, m_commandId);
	writeBE(os, m_statusId);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
