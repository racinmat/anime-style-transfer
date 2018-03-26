//======================================================================
/*! \file ReplyEcuAppBaseStatus.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 8, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/commands/ReplyEcuAppBaseStatus.hpp>

#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

static
void readBE(std::istream& is, ReplyEcuAppBaseStatus::AppBaseStatusId& statusId)
{
	uint16_t tmp;
	readBE(is, tmp);
	statusId = ReplyEcuAppBaseStatus::AppBaseStatusId(tmp);
}

//======================================================================

static
void writeBE(std::ostream& os, const ReplyEcuAppBaseStatus::AppBaseStatusId statusId)
{
	writeBE(os, uint16_t(statusId));
}

//======================================================================

ReplyEcuAppBaseStatus::ReplyEcuAppBaseStatus()
  : EcuCommandReply<CommandId::CmdManagerAppBaseStatus>(),
    AppBaseStatusDefinitions(),
    m_statusId(AppBaseStatusId_Recording),
    m_data()
{}

//======================================================================

bool ReplyEcuAppBaseStatus::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readBE(is, m_commandId);
	readBE(is, m_statusId);

	is.unsetf(std::ios::skipws);
	std::istreambuf_iterator<char> eos;
	std::string s(std::istreambuf_iterator<char>(is), eos);
	is.setf(std::ios::skipws);
	this->m_data = s;

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool ReplyEcuAppBaseStatus::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, m_commandId);
	writeBE(os, m_statusId);

	std::ostreambuf_iterator<char> strOut(os);
	copy(this->m_data.begin(), this->m_data.end(), strOut);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
