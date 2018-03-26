//======================================================================
/*! \file CommandEcuAppBaseCtrl.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 7, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/commands/CommandEcuAppBaseCtrl.hpp>

#include <ibeosdk/io.hpp>

#include <ios>
#include <iterator>

//======================================================================

namespace ibeosdk {

//======================================================================

static
void readBE(std::istream& is, CommandEcuAppBaseCtrl::AppBaseCtrlId& ctrId)
{
	uint16_t tmp;
	readBE(is, tmp);
	ctrId = CommandEcuAppBaseCtrl::AppBaseCtrlId(tmp);
}

//======================================================================

static
void writeBE(std::ostream& os, const CommandEcuAppBaseCtrl::AppBaseCtrlId ctrId)
{
	writeBE(os, uint16_t(ctrId));
}

//======================================================================

CommandEcuAppBaseCtrl::CommandEcuAppBaseCtrl(const AppBaseCtrlId ctrlId)
  : EcuCommand<CommandId::CmdManagerAppBaseCtrl>(),
    m_ctrlId(ctrlId),
    m_data()
{}


//======================================================================

CommandEcuAppBaseCtrl::CommandEcuAppBaseCtrl(const AppBaseCtrlId ctrlId,
                                                     const std::string& data)
  : EcuCommand<CommandId::CmdManagerAppBaseCtrl>(),
    m_ctrlId(ctrlId),
    m_data(data)
{}

//======================================================================

bool CommandEcuAppBaseCtrl::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readBE(is, m_commandId);
	readBE(is, m_ctrlId);

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

bool CommandEcuAppBaseCtrl::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, m_commandId);
	writeBE(os, m_ctrlId);

	std::ostreambuf_iterator<char> strOut(os);
	copy(this->m_data.begin(), this->m_data.end(), strOut);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
