//======================================================================
/*! \file IbeoTrackingBox.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Oct 23, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/devices/IbeoTrackingBox.hpp>
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

IbeoTrackingBox::IbeoTrackingBox(const std::string& ip, const unsigned short port)
  : IbeoDevice(ip, port)
{}

//======================================================================

IbeoTrackingBox::~IbeoTrackingBox()
{
	this->disconnect();
}

//======================================================================

void IbeoTrackingBox::getConnected()
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

statuscodes::Codes IbeoTrackingBox::sendCommand(const EcuCommandBase& cmd)
{
	return IbeoEthDevice::sendCommand(cmd);
}

//======================================================================

statuscodes::Codes IbeoTrackingBox::sendCommand(const EcuCommandBase& cmd,
                                        EcuCommandReplyBase& reply,
                                        const boost::posix_time::time_duration timeOut)
{
	return IbeoEthDevice::sendCommand(cmd, reply, timeOut);
}


//======================================================================

} // namespace ibeosdk

//======================================================================
