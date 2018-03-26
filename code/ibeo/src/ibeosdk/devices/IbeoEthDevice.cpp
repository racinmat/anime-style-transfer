//======================================================================
/*! \file IbeoEthDevice.cpp
 *
 * \copydoc Copyright
 * \author Yannic Wilkening (YW)
 * \date July 20, 2017
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/devices/IbeoEthDevice.hpp>

#include <ibeosdk/devices/TcpReceiveThreadEnsemble.hpp>
#include <ibeosdk/devices/UdpReceiveThreadEnsemble.hpp>
#include <ibeosdk/devices/IbeoEthType.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

IbeoEthDevice::IbeoEthDevice(const std::string& ip, const unsigned short port,
                             const IbeoTypeEthTcp&)
  : IbeoDeviceBase(),
    m_strIP(ip),
    m_port(port),
    m_mutex(),
    m_threadState(TS_NotRunning),
    m_receiveThread(NULL),
    m_ethType(EthTcp)
{}

//======================================================================

IbeoEthDevice::IbeoEthDevice(const std::string& ip, const unsigned short port,
                             const boost::asio::ip::address_v4 ifa, const IbeoTypeEthUdp&)
  : IbeoDeviceBase(),
    m_strIP(ip),
    m_port(port),
    m_ifa(ifa),
    m_mutex(),
    m_threadState(TS_NotRunning),
    m_receiveThread(NULL),
    m_ethType(EthUdp)
{}

IbeoEthDevice::IbeoEthDevice(const std::string& ip, const unsigned short port,
                             const boost::asio::ip::address_v4 ifa, const IbeoTypeEthUdpMulticast&)
  : IbeoDeviceBase(),
    m_strIP(ip),
    m_port(port),
    m_ifa(ifa),
    m_mutex(),
    m_threadState(TS_NotRunning),
    m_receiveThread(NULL),
    m_ethType(EthUdpMulticast)
{}

//======================================================================

IbeoEthDevice::~IbeoEthDevice()
{
	{
		Lock lock(m_mutex);
		m_threadState = TS_NotRunning;
	}

	if (m_receiveThread) {
		m_receiveThread->stop();
		delete m_receiveThread;
		m_receiveThread = NULL;
	}
}

//======================================================================

void IbeoEthDevice::receivedDataBlock(const IbeoDataHeader* dh, const char* bodyBuf)
{
	logDebug2 << "Received " << toHex(dh->getDataType()) << std::endl;

	if (!m_streamers.empty()) {
		// Message has been completely received. So, we can notify all streamers
		notifyStreamers(*dh, bodyBuf);

	}
	if ((!m_listeners.empty()) && (m_listeners.find(dh->getDataType()) != m_listeners.end())) {
		// Message has been completely received. So, we can parse it and notify all clients
		boost::iostreams::stream<boost::iostreams::array_source> strm(bodyBuf, dh->getMessageSize());
		notifyListeners(onData(*dh, strm));
	}
}

//======================================================================

void IbeoEthDevice::getConnected()
{
	boost::system::error_code ec;
	boost::asio::ip::address ipAdr = boost::asio::ip::address::from_string(m_strIP, ec);

	if (ec) {
		logError << "Invalid ip address (" << m_strIP << ") " << ec.message() << std::endl;
		Lock lock(this->m_mutex);
		m_threadState = TS_StartFailed;
		return;
	}

	if (m_receiveThread) {
		logWarning << "Receive thread already running." << std::endl;
		return;
	}

	if (m_ethType == EthUdp) {
		UdpReceiveThreadEnsemble::ReceiveDataBlock rdb = boost::bind(&IbeoEthDevice::receivedDataBlock, this, _1, _2);
		m_receiveThread = new UdpReceiveThreadEnsemble(ipAdr, m_port, rdb, m_ifa);
	}
	else if (m_ethType == EthUdpMulticast) {
		UdpReceiveThreadEnsemble::ReceiveDataBlock rdb = boost::bind(&IbeoEthDevice::receivedDataBlock, this, _1, _2);
		m_receiveThread = new UdpReceiveThreadEnsemble(ipAdr, m_port, rdb, m_ifa, true);
	}
	else {
		TcpReceiveThreadEnsemble::ReceiveDataBlock rdb = boost::bind(&IbeoEthDevice::receivedDataBlock, this, _1, _2);
		m_receiveThread = new TcpReceiveThreadEnsemble(ipAdr, m_port, rdb);
	}

	m_receiveThread->setExpiryTime(boost::posix_time::seconds(10));
	m_receiveThread->run();
	m_threadState = TS_Running;

//	}
}

//======================================================================

void IbeoEthDevice::disconnect()
{
	IbeoDeviceBase::disconnect();
	try {
		Lock lock(m_mutex);
		m_threadState = TS_NotRunning;
	}
	catch (const boost::lock_error& e) {
		logError << e.what() << std::endl;
	}

	if (m_receiveThread) {
		m_receiveThread->stop();
		delete m_receiveThread;
		m_receiveThread = NULL;
	}
}

//======================================================================

bool IbeoEthDevice::isConnected()
{
	if (m_receiveThread) {
		return m_receiveThread->isRunning();
	}
	return false;
}

//======================================================================

statuscodes::Codes IbeoEthDevice::sendCommand(const CommandBase& cmd)
{
	if (m_receiveThread) {
		return m_receiveThread->sendCommand(cmd);
	}
	else {
		return statuscodes::SendingCommandFailed;
	}
}

//======================================================================

statuscodes::Codes IbeoEthDevice::sendCommand(const CommandBase& cmd,
                                              CommandReplyBase& reply,
                                              const boost::posix_time::time_duration timeOut)
{
	if (m_receiveThread) {
		return m_receiveThread->sendCommand(cmd, reply, timeOut);
	}
	else {
		return statuscodes::SendingCommandFailed;
	}
}

//======================================================================

}// namespace ibeosdk

//======================================================================
