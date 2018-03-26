//======================================================================
/*! \file IbeoTcpIpAcceptorScala.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jun 02, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/IbeoTcpIpAcceptorScala.hpp>

#include <boost/iostreams/stream.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

IbeoTcpIpAcceptorScala::SessionScala::SessionScala(IbeoTcpIpAcceptorBase* const parent,
                                                   boost::asio::io_service& io_service,
                                                   const UINT8 deviceId)
  : IbeoTcpIpAcceptorBase::SessionBase(parent, io_service, deviceId)
{}

//======================================================================

void IbeoTcpIpAcceptorScala::SessionScala::startListen() {}

//======================================================================





//======================================================================
//======================================================================
//======================================================================





//======================================================================

IbeoTcpIpAcceptorScala::IbeoTcpIpAcceptorScala(LogFileManager* const logFileManager, const unsigned short port)
  : IbeoTcpIpAcceptorBase(logFileManager, port)
{}

//======================================================================

IbeoTcpIpAcceptorScala::IbeoTcpIpAcceptorScala(ibeosdk::LogFileManager* const logFileManager,
                                               boost::asio::deadline_timer::duration_type writeExpirationTime,
                                               const unsigned short port)
  : IbeoTcpIpAcceptorBase(logFileManager, writeExpirationTime, port)
{}

//======================================================================

IbeoTcpIpAcceptorBase::SessionBase::Ptr IbeoTcpIpAcceptorScala::getNewSessionPtr(boost::asio::io_service& io_service)
{
	m_sessionPtr.reset(new SessionScala(this, io_service));
	return  m_sessionPtr;
}

//======================================================================

}
