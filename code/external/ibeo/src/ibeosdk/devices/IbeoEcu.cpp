//======================================================================
/*! \file IbeoEcu.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 24, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/devices/IbeoEcu.hpp>
#include <ibeosdk/datablocks/commands/CommandEcuSetFilter.hpp>
#include <ibeosdk/datablocks/commands/EmptyCommandReply.hpp>


#include <boost/array.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

IbeoEcu::IbeoEcu(const std::string& ip, const unsigned short port)
  : IbeoDevice(ip, port)
{}

//======================================================================

IbeoEcu::~IbeoEcu()
{
	this->disconnect();
}

//======================================================================

void IbeoEcu::getConnected()
{
	IbeoDevice::getConnected();

	// clear data type filter
	const DataTypeId startRange(DataTypeId::DataType_Unknown);
	const DataTypeId endRange(DataTypeId::DataType_LastId);
	CommandEcuSetFilter setFilterCmd(startRange, endRange);
	ReplyEcuSetFilter reply;
	this->sendCommand(setFilterCmd, reply, boost::posix_time::milliseconds(500));
}

//======================================================================

statuscodes::Codes IbeoEcu::sendCommand(const EcuCommandBase& cmd)
{
	return IbeoEthDevice::sendCommand(cmd);
}

//======================================================================

statuscodes::Codes IbeoEcu::sendCommand(const EcuCommandBase& cmd,
                                        EcuCommandReplyBase& reply,
                                        const boost::posix_time::time_duration timeOut)
{
	return IbeoEthDevice::sendCommand(cmd, reply, timeOut);
}


//======================================================================

} // namespace ibeosdk

//======================================================================
