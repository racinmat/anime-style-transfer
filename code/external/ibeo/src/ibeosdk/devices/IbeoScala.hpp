//======================================================================
/*! \file IbeoScala.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer
 * \date Oct 04, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_IBEOSCALA_HPP_SEEN
#define IBEOSDK_IBEOSCALA_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/devices/IbeoDevice.hpp>
#include <ibeosdk/devices/IbeoEthType.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class IbeoScala
 * \brief Class to connect to a Scala sensor.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Oct 1, 2013
 *
 *///-------------------------------------------------------------------
class IbeoScala : public IbeoDevice<IbeoScala> {
public:
	//========================================
	/*!\brief Create an IbeoScala (connection class).
	 *
	 * This constructor will create an IbeoLUX class object
	 * which will try to connect to a Scala sensor,
	 * using the given IP address and port number using TCP protocol.
	 *
	 * \param[in] ip      IP address of the scanner
	 *                    to be connected with.
	 * \param[in] port    Port number for the connection
	 *                    with the scanner.
	 * \param[in] ethTcp  Protocol type for the connection
	 *                    with the scanner.
	 *///-------------------------------------
	IbeoScala(const std::string& ip,
	          const unsigned short port = 12004,
	          const IbeoTypeEthTcp& ethTcp = IbeoTypeEthTcp());

	//========================================
	/*!\brief Create an IbeoScala (connection class).
	 *
	 * This constructor will create an IbeoLUX class object
	 * which will try to connect to a Scala sensor,
	 * using the given IP address, port number and ethernet protocol.
	 *
	 * \param[in] ip      IP address of the scanner
	 *                    to be connected with.
	 * \param[in] port    Port number for the connection
	 *                    with the scanner.
	 * \param[in] ethTcp  Protocol type for the connection
	 *                    with the scanner.
	 * \param[in] ifa     Address of network interface for the connection
	 *                    with the scanner.
	 *///-------------------------------------
	IbeoScala(const std::string& ip,
	          const unsigned short port,
	          const IbeoTypeEthUdp& ethUdp,
	          const boost::asio::ip::address_v4 ifa = boost::asio::ip::address_v4::any());

	//========================================
	/*!\brief Create an IbeoScala (connection class).
	 *
	 * This constructor will create an IbeoLUX class object
	 * which will try to connect to a Scala sensor,
	 * using the given IP address, port number and ethernet protocol.
	 *
	 * \param[in] ip      IP address of the scanner
	 *                    to be connected with.
	 * \param[in] port    Port number for the connection
	 *                    with the scanner.
	 * \param[in] ethTcp  Protocol type for the connection
	 *                    with the scanner.
	 * \param[in] ifa     Address of network interface for the connection
	 *                    with the scanner.
	 *///-------------------------------------
	IbeoScala(const std::string& ip,
	          const unsigned short port,
	          const IbeoTypeEthUdpMulticast& ethMulticast,
	          const boost::asio::ip::address_v4 ifa = boost::asio::ip::address_v4::any());

	//========================================
	/*!\brief Destructor.
	 *
	 * Will disconnect before destruction.
	 *///-------------------------------------
	virtual ~IbeoScala();


private: // not supported
	using IbeoDevice<IbeoScala>::sendCommand;
}; // IbeoScala

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IBEOSCALA_HPP_SEEN

//======================================================================

