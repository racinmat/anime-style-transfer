//======================================================================
/*! \file IbeoEthDevice.hpp
 *
 * \copydoc Copyright
 * \author Yannic Wilkening (YW)
 * \date July 20, 2017
 *///-------------------------------------------------------------------

#ifndef SRC_IBEOSDK_DEVICES_IBEOETHDEVICE_HPP_SEEN
#define SRC_IBEOSDK_DEVICES_IBEOETHDEVICE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/devices/IbeoEthDevice.hpp>

#include <ibeosdk/devices/sutp/IbeoSutpInterface.hpp>
#include <ibeosdk/devices/ThreadEnsemble.hpp>
#include <ibeosdk/devices/IbeoEthType.hpp>
#include <boost/asio.hpp>
#include <boost/optional.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class IbeoEthDevice
 * \brief Base class for all Ibeo devices connected via TCP/IP or UDP.
 * \author Yannic Wilkening (YW)
 * \version 0.1
 * \date July 20, 2017
 *///-------------------------------------------------------------------
class IbeoEthDevice : public IbeoDeviceBase {
public:
	typedef boost::shared_ptr<boost::asio::ip::udp::socket> UdpSocketPtr;

private:
	enum EthType {EthTcp, EthUdp, EthUdpMulticast};

public:
	//========================================
	/*!\brief Create an IbeoEthDevice (general device class).
	 *
	 * This constructor will create an IbeoEthDevice class object
	 * for devices communicating by network.
	 *
	 * \param[in] ip      IP address of the device
	 *                    to be connected with.
	 * \param[in] port    Port number for the device.
	 * \param[in] ethType Protocol type for the connection
	 *                    with the device defaults to TCP.
	 *///-------------------------------------
	IbeoEthDevice(const std::string& ip, const unsigned short port, const IbeoTypeEthTcp&);
	//========================================
	/*!\brief Create an IbeoEthDevice (general device class).
	 *
	 * This constructor will create an IbeoEthDevice class object
	 * for devices communicating by network.
	 *
	 * \param[in] ip      IP address of the device
	 *                    to be connected with.
	 * \param[in] port    Port number for the device.
	 * \param[in] ethType Protocol type for the connection
	 *                    with the device defaults to Udp.
	 * \param[in] ifa     Address of network interface for the connection
	 *                    with the device.
	 *///-------------------------------------
	IbeoEthDevice(const std::string& ip, const unsigned short port, const boost::asio::ip::address_v4 ifa, const IbeoTypeEthUdp&);
	//========================================
	/*!\brief Create an IbeoEthDevice (general device class).
	 *
	 * This constructor will create an IbeoEthDevice class object
	 * for devices communicating by network.
	 *
	 * \param[in] ip      IP address of the device
	 *                    to be connected with.
	 * \param[in] port    Port number for the device.
	 * \param[in] ifa     Address of network interface for the connection
	 *                    with the device.
	 * \param[in] ethType Protocol type for the connection
	 *                    with the device defaults to UdpMulticast.
	 *///-------------------------------------
	IbeoEthDevice(const std::string& ip, const unsigned short port, const boost::asio::ip::address_v4 ifa, const IbeoTypeEthUdpMulticast&);
	virtual ~IbeoEthDevice();

private:
	IbeoEthDevice(const IbeoEthDevice&); // forbidden
	IbeoEthDevice& operator=(const IbeoEthDevice&); // forbidden

public:
	void receivedDataBlock(const IbeoDataHeader* dh, const char* bodyBuf);

	//========================================
	/*!\brief Establish the connection to the
	 *        hardware.
	 *
	 * Starting the receiving thread.
	 *///-------------------------------------
	virtual void getConnected();

	//========================================
	/*!\brief Disconnect the TCP/IP connection
	 *        to the hardware device.
	 *///-------------------------------------
	virtual void disconnect();

	//========================================
	/*!\brief Checks whether the TCP/IP connection
	 *        to the hardware device is there and
	 *        the receiving thread is still running.
	 * \return \c true if messages from the hardware
	 *         can still be received. \c false otherwise.
	 *///-------------------------------------
	bool isConnected();

	//========================================
	/*!\brief Send a command which expects no reply.
	 * \param[in] cmd  Command to be sent.
	 * \return The result of the operation.
	 * \sa ErrorCode
	 *///-------------------------------------
	statuscodes::Codes sendCommand(const CommandBase& cmd);

	//========================================
	/*!\brief Send a command and wait for a reply.
	 *
	 * The command will be sent. The calling thread
	 * will sleep until a reply has been received
	 * but not longer than the number of milliseconds
	 * given in \a timeOut.
	 *
	 * \param[in]       cmd    Command to be sent.
	 * \param[in, out]  reply  The reply container for
	 *                         the reply to be stored into.
	 * \param[in]       timeOut  Number of milliseconds to
	 *                           wait for a reply.
	 * \return The result of the operation.
	 * \sa ErrorCode
	 *///-------------------------------------
	statuscodes::Codes sendCommand(const CommandBase& cmd,
	                               CommandReplyBase& reply,
	                               const boost::posix_time::time_duration timeOut = boost::posix_time::milliseconds(500));

protected:
	//========================================
	/*!\brief TCP/IP address of the device as string.
	 *///-------------------------------------
	std::string m_strIP;

	//========================================
	/*!\brief Port number of the device.
	 *///-------------------------------------
	unsigned short m_port;

	boost::asio::ip::address_v4 m_ifa;

	Mutex m_mutex;

	ThreadState m_threadState;

	ThreadEnsemble* m_receiveThread;

	EthType m_ethType;

}; // IbeoEthDevice

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // SRC_IBEOSDK_DEVICES_IBEOETHDEVICE_HPP_SEEN

//======================================================================
