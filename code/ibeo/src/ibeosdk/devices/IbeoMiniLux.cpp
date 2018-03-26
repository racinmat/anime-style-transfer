//======================================================================
/*! \file IbeoMiniLux.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer
 * \date Oct 04, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/devices/IbeoMiniLux.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

IbeoMiniLux::IbeoMiniLux(const std::string& ip, const unsigned short port)
  : IbeoDevice(ip, port)
{}

//======================================================================

IbeoMiniLux::~IbeoMiniLux()
{
	this->disconnect();
}

//======================================================================

statuscodes::Codes IbeoMiniLux::sendCommand(const MiniLuxCommandBase& cmd)
{
	return IbeoEthDevice::sendCommand(cmd);
}

//======================================================================

statuscodes::Codes IbeoMiniLux::sendCommand(const MiniLuxCommandBase& cmd,
                                            MiniLuxCommandReplyBase& reply,
                                            const boost::posix_time::time_duration timeOut)
{
	return IbeoEthDevice::sendCommand(cmd, reply, timeOut);
}

//======================================================================

} // namespace ibeosdk

//======================================================================
