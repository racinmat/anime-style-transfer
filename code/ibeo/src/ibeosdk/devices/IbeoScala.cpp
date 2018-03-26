//======================================================================
/*! \file IbeoScala.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer
 * \date Oct 04, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/devices/IbeoScala.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

IbeoScala::IbeoScala(const std::string& ip,
                     const unsigned short port,
                     const IbeoTypeEthTcp& ethTcp)
  : IbeoDevice(ip, port, ethTcp)
{}

//======================================================================

IbeoScala::IbeoScala(const std::string& ip,
                     const unsigned short port,
                     const IbeoTypeEthUdp& ethUdp,
                     const boost::asio::ip::address_v4 ifa)
  : IbeoDevice(ip, port, ethUdp, ifa)
{}

//======================================================================

IbeoScala::IbeoScala(const std::string& ip,
                     const unsigned short port,
                     const IbeoTypeEthUdpMulticast& ethMulticast,
                     const boost::asio::ip::address_v4 ifa)
  : IbeoDevice(ip, port, ethMulticast, ifa)
{}

//======================================================================

IbeoScala::~IbeoScala()
{
	this->disconnect();
}

//======================================================================

} // namespace ibeosdk

//======================================================================
