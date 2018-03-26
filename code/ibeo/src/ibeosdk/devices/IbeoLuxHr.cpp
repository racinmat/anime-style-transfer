//======================================================================
/*! \file IbeoLuxHr.cpp
 *
 * \copydoc Copyright
 * \author Andreas Mueller (AnM)
 * \date Jul 30, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/devices/IbeoLuxHr.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

IbeoLuxHr::IbeoLuxHr(const std::string& ip, const unsigned short port)
  : IbeoDevice(ip, port)
{}

//======================================================================

IbeoLuxHr::~IbeoLuxHr()
{
	this->disconnect();
}

////======================================================================
//
//statuscodes::Codes IbeoLuxHr::sendCommand(const LuxHrCommandBase& cmd)
//{
//	return IbeoTcpIpDeviceBase::sendCommand(cmd);
//}
//
////======================================================================
//
//statuscodes::Codes IbeoLuxHr::sendCommand(const LuxHrCommandBase& cmd,
//                                            LuxHrCommandReplyBase& reply,
//                                            const boost::posix_time::time_duration timeOut)
//{
//	return IbeoTcpIpDeviceBase::sendCommand(cmd, reply, timeOut);
//}

//======================================================================

} // namespace ibeosdk

//======================================================================
