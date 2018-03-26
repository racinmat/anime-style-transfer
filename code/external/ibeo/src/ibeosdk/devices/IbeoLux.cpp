//======================================================================
/*! \file IbeoLux.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 24, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/devices/IbeoLux.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

IbeoLux::IbeoLux(const std::string& ip, const unsigned short port)
  : IbeoDevice(ip, port)
{}

//======================================================================

IbeoLux::~IbeoLux()
{
	this->disconnect();
}

//======================================================================

statuscodes::Codes IbeoLux::sendCommand(const LuxCommandBase& cmd)
{
	return IbeoEthDevice::sendCommand(cmd);
}

//======================================================================

statuscodes::Codes IbeoLux::sendCommand(const LuxCommandBase& cmd,
                                        LuxCommandReplyBase& reply,
                                        const boost::posix_time::time_duration timeOut)
{
	return IbeoEthDevice::sendCommand(cmd, reply, timeOut);
}

//======================================================================

} // namespace ibeosdk

//======================================================================
