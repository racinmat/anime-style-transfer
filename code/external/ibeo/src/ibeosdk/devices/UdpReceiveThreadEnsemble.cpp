//======================================================================
/*! \file UdpReceiveThread.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date May 5, 2017
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/Log.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <ibeosdk/devices/UdpReceiveThreadEnsemble.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

UdpReceiveThreadEnsemble::UdpReceiveThreadEnsemble(const boost::asio::ip::address& ip,
                                                   const unsigned short port,
                                                   ReceiveDataBlock receiveDataBlock,
                                                   const boost::asio::ip::address_v4 ifa,
                                                   bool enableMulticast)
  : m_ioService(),
    m_endPoint(ifa, port),
    m_ipAdr(ip),
    m_socket(),
    m_enableMulticast(enableMulticast),
    m_timerResult(),
    m_readResult(),
    m_auxTimer(m_ioService),
    m_stopOnTimeOut(true),
    m_expiryTime(),
    m_sutpInterface(boost::bind(&UdpReceiveThreadEnsemble::handleSutpMsgs,
                                this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred)),
    m_receiveDataBlock(receiveDataBlock),
    m_udpRecvMutex(),
    m_udpRecvThreadStateCondition(),
    m_udpRecvThread(NULL),
    m_sutpMutex(),
    m_sutpThread(NULL),
    m_udpRecvThreadState(TS_NotRunning)
{}

//======================================================================

UdpReceiveThreadEnsemble::~UdpReceiveThreadEnsemble()
{
	try {
		Lock lock(m_sutpMutex);
		if (m_sutpThreadState == TS_Running) {
			m_sutpThreadState = TS_Stopping;

			m_sutpInterface.getPacketCompleteCondition().notify_all();
		}
	}
	catch (...) {
		logError << "failed" << std::endl;
	}

	try {
		Lock lock(m_udpRecvMutex);
		if (m_udpRecvThreadState == TS_Running || m_udpRecvThreadState == TS_Starting) {
			// Thread has not yet been stopped.
			// Ask for termination and wait till a state change
			// has been performed.
			m_udpRecvThreadState = TS_Stopping;
			m_udpRecvThreadStateCondition.wait(lock);
		}
	}
	catch (...) {
		logError << "failed 2" << std::endl;
	}

	if (this->m_sutpThread->joinable()) {
		this->m_sutpThread->join();
		delete this->m_sutpThread;
		this->m_sutpThread = NULL;
	}
	if (this->m_udpRecvThread->joinable()) {
		this->m_udpRecvThread->join();
		delete this->m_udpRecvThread;
		this->m_udpRecvThread = NULL;
	}
}

//======================================================================

boost::optional<boost::asio::deadline_timer::duration_type> UdpReceiveThreadEnsemble::getExpiryTime()
{
	return m_expiryTime;
}

//======================================================================

void UdpReceiveThreadEnsemble::setExpiryTime(const boost::asio::deadline_timer::duration_type expiryTime)
{
	Lock lock(m_udpRecvMutex);
	m_expiryTime.reset(expiryTime);
	logDebug2 << "Set new ExpiryTime" << std::endl;
}

//======================================================================

void UdpReceiveThreadEnsemble::resetExpiryTime()
{
	Lock lock(m_udpRecvMutex);
	m_expiryTime.reset();
}

//======================================================================

void UdpReceiveThreadEnsemble::run()
{
	if (m_udpRecvThread)
		return;

	try {
		Lock lock(m_udpRecvMutex);

		m_sutpThread = new boost::thread(&UdpReceiveThreadEnsemble::sutpHandlingThreadMain, this);
		m_udpRecvThread = new boost::thread(&UdpReceiveThreadEnsemble::recvThreadMain, this);

		while ((this->m_udpRecvThreadState==TS_NotRunning || this->m_udpRecvThreadState==TS_Starting)) {
			m_udpRecvThreadStateCondition.wait(lock);
		}
	}
	catch (const boost::lock_error& e) {
		logError << e.what() << std::endl;
	}

}

//======================================================================

bool UdpReceiveThreadEnsemble::powerUpConnection()
{
	if (!m_receiveDataBlock) {
		logError << "Failed to start UDP receive thread. No onData method has be specified." << std::endl;
		return false;
	}

	try {
		Lock lock(m_udpRecvMutex);
		m_udpRecvThreadState = TS_Starting;
		m_udpRecvThreadStateCondition.notify_all();
	}
	catch (const boost::lock_error& e) {
		logError << e.what() << std::endl;
		return false;
	}

	try {
		m_socket.reset(new boost::asio::ip::udp::socket(m_ioService));
	}
	catch (const std::exception& e) {
		logError << "Failed to allocate socket. - "<< e.what() << std::endl;
		Lock lock(m_udpRecvMutex);
		m_udpRecvThreadState = TS_StartFailed;
		m_udpRecvThreadStateCondition.notify_all();
		return false;
	}

	try {
		m_socket->open(boost::asio::ip::udp::v4());
		m_socket->set_option(boost::asio::ip::udp::socket::reuse_address(true));
		m_socket->set_option(boost::asio::socket_base::receive_buffer_size(bufferSize));
	}
	catch (const std::exception& e) {
		logError << "Failed to open and configure socket. - "<< e.what() << std::endl;
		try {
			Lock lock(m_udpRecvMutex);
			m_udpRecvThreadState = TS_StartFailed;
			m_udpRecvThreadStateCondition.notify_all();
		}
		catch (const boost::lock_error& e1) {
			logError << e1.what() << std::endl;
		}

		return false;
	}

	boost::system::error_code ec;

	m_socket->bind(m_endPoint);
	if (m_enableMulticast) {
		m_socket->set_option(boost::asio::ip::multicast::join_group(m_ipAdr));
	}

	if (ec) {
		logError << "Failed to connect to device (" << m_endPoint << ") " << ec.message() << std::endl;
		try {
			Lock lock(m_udpRecvMutex);
			m_udpRecvThreadState = TS_StartFailed;
			m_udpRecvThreadStateCondition.notify_all();
		}
		catch (const boost::lock_error& e) {
			logError << e.what() << std::endl;
		}
		return false;
	}

	logDebug2 << "Connected to device on " << m_endPoint << std::endl;

	try {
		Lock lock(m_udpRecvMutex);
		m_udpRecvThreadState = TS_Running;
		m_udpRecvThreadStateCondition.notify_all();
	}
	catch (const boost::lock_error& e) {
		logError << e.what() << std::endl;
		return false;
	}

	return true;
}

//======================================================================

void UdpReceiveThreadEnsemble::recvThreadMain()
{
	const bool powerUpOk= powerUpConnection();

	try {
		Lock lock(m_udpRecvMutex);
		m_udpRecvThreadState = powerUpOk ? TS_Running : TS_RunFailed;
		m_udpRecvThreadStateCondition.notify_all();
	}
	catch (const boost::lock_error& e) {
		logError << e.what() << std::endl;
		return;
	}

	if (!powerUpOk) {
		return;
	}


	boost::optional<boost::asio::deadline_timer::duration_type> expiryTime;
	boost::system::error_code error;

	try {
		while (this->m_udpRecvThreadState != TS_Stopping && m_socket->is_open()) {
			try {
				Lock lock(m_udpRecvMutex);
				expiryTime = m_expiryTime;
			}
			catch (const boost::lock_error& e) {
				logError << e.what() << std::endl;
			}

			if (receiveWithTimeout(*m_socket, m_endPoint, expiryTime.get(), error)) {

//				try {
//					Lock lock(this->m_udpRecvMutex); // enforce extra lock here to give
//					                                 // canceling a better chance.
//					this->m_udpRecvThreadState = TS_Running;
//				}
//				catch (const boost::lock_error& e) {
//					logError << e.what() << std::endl;
//				}

			}
			else {
				try {
					Lock lock(m_udpRecvMutex);
					this->m_udpRecvThreadState = TS_Stopping;
					m_udpRecvThreadStateCondition.notify_all();
				}
				catch (const boost::lock_error& e) {
					logError << e.what() << std::endl;
				}
			}
		}
	}
	catch (const boost::system::error_code& errorCode) {
		logError << "In receive loop: " << errorCode.message() << std::endl;
		m_udpRecvThreadState = TS_RunFailed;
		m_udpRecvThreadStateCondition.notify_all();
		return;
	}

	if (!m_socket->is_open() && this->m_udpRecvThreadState != TS_Stopping) {
		try {
			Lock lock(m_udpRecvMutex);
			m_udpRecvThreadState = TS_RunFailed;
			m_udpRecvThreadStateCondition.notify_all();
		}
		catch (const boost::lock_error& e) {
			logError << e.what() << std::endl;
		}

		return;
	}

	m_udpRecvThreadState = TS_NotRunning;
	m_udpRecvThreadStateCondition.notify_all();
}

//======================================================================

void UdpReceiveThreadEnsemble::disconnect() {
	if (m_socket.get() != NULL) {
		m_socket.reset();
	}
}

//======================================================================

void UdpReceiveThreadEnsemble::stop()
{
	if (isRunning(false)) {
		try {
			Lock lock(m_udpRecvMutex);
			m_udpRecvThreadState = TS_Stopping;
		}
		catch (const boost::lock_error& e) {
			logError << e.what() << std::endl;
		}
	}
	disconnect();
}

//======================================================================

bool UdpReceiveThreadEnsemble::isRunning(const bool useMutex)
{
	ThreadState state = TS_NotRunning;
	if (useMutex) {
		try {
			Lock lock(m_udpRecvMutex);
			state = m_udpRecvThreadState;
		}
		catch (const boost::lock_error& e) {
			logError << e.what() << std::endl;
		}
	}
	else {
		state = m_udpRecvThreadState;
	}

	if (state == TS_Starting || state == TS_Running) {
		return true;
	}
	return false;
}

//======================================================================

bool UdpReceiveThreadEnsemble::isNotYetRunning(const bool useMutex)
{
	ThreadState state = TS_NotRunning;
	if (useMutex) {
		try {
			Lock lock(m_udpRecvMutex);
			state = m_udpRecvThreadState;
		}
		catch (const boost::lock_error& e) {
			logError << e.what() << std::endl;
		}
	}
	else {
		state = m_udpRecvThreadState;
	}

	if (state == TS_NotRunning || state == TS_Starting) {
		return true;
	}
	return false;
}

//======================================================================

void UdpReceiveThreadEnsemble::handleReceive(const boost::system::error_code& error, std::size_t len)
{
	if (!m_timerResult) {
		logDebug2 << "Received some data. Length: " << len << std::endl;

		m_readResult.reset(error);

		if ((error == 0) && (len > 0)) {
			boost::iostreams::stream<boost::iostreams::array_source> is(m_inCArrBuffer, len);
			m_sutpInterface.onReceiveRaw(is, uint32_t(len));
		}
	}
}

//======================================================================

void UdpReceiveThreadEnsemble::sutpHandlingThreadMain()
{
	try {
		Lock lock(this->m_sutpMutex);
		m_sutpThreadState = TS_Running;
	}
	catch (const boost::lock_error& e) {
		logError << e.what() << std::endl;
	}


	boost::optional<boost::asio::deadline_timer::duration_type> expiryTime;

	boost::condition& packetComplete = m_sutpInterface.getPacketCompleteCondition();
	boost::recursive_mutex& packetMutex = m_sutpInterface.getPacketCompleteMutex();
	while (this->m_sutpThreadState != TS_Stopping) {
		try {
			boost::recursive_mutex::scoped_lock lock(packetMutex);
			packetComplete.wait(packetMutex);
		}
		catch (const boost::lock_error& e) {
			logError << e.what() << std::endl;
		}

		m_sutpInterface.processCache();
	}


	try {
		Lock lock(this->m_sutpMutex);
		m_sutpThreadState = TS_NotRunning;
	}
	catch (const boost::lock_error& e) {
		logError << e.what() << std::endl;

	}

	logDebug2 << "sutpHandlingThreadMain: I am dead!" << std::endl;
}

//======================================================================

bool UdpReceiveThreadEnsemble::handleSutpMsgs(const char* bodyBuf, size_t len)
{

	Lock lock(m_sutpMutex);
	{
		boost::iostreams::array_source as(bodyBuf, len);
		boost::iostreams::stream<boost::iostreams::array_source> strm(as);
		std::istream& is = static_cast<std::istream&>(strm);

		IbeoDataHeader dh;
		if (len < IbeoDataHeader::getHeaderSize()) {
			return false;
		}

		if (dh.deserialize(is)) {
			if (len < dh.getMessageSize() + IbeoDataHeader::getHeaderSize()) {
				return false;
			}
			m_receiveDataBlock(&dh, bodyBuf+IbeoDataHeader::getHeaderSize());
			return true;
		}
		return false;
	}
}

//======================================================================

void UdpReceiveThreadEnsemble::waitHandler(const boost::system::error_code& error)
{
	if (!m_readResult) {
		logDebug2 << "TimeOut Event" << std::endl;
	}
	m_timerResult.reset(error);
}

//======================================================================

bool UdpReceiveThreadEnsemble::receiveWithTimeout(boost::asio::ip::udp::socket& socket,
                                                  boost::asio::ip::udp::endpoint& ep,
                                                  const boost::asio::deadline_timer::duration_type& expiryTime,
                                                  boost::system::error_code& readError)
{
	m_timerResult.reset();
	m_readResult.reset();

	std::memset(m_inCArrBuffer, 0, bufferSize);

	m_auxTimer.expires_from_now(expiryTime);
	m_auxTimer.async_wait(boost::bind(&UdpReceiveThreadEnsemble::waitHandler, this, _1));

	socket.async_receive_from(boost::asio::buffer(m_inCArrBuffer, bufferSize),
	                          ep,
	                          boost::bind(&UdpReceiveThreadEnsemble::handleReceive,
	                                      this,
	                                      boost::asio::placeholders::error,
	                                      boost::asio::placeholders::bytes_transferred));

	socket.get_io_service().reset();
	while (socket.get_io_service().run_one()) {
		if (m_readResult) {
			m_auxTimer.cancel();
		}
		else if (m_timerResult) {
			socket.cancel();
			if (m_stopOnTimeOut) {
				return false;
			}
		}
	}

	if (m_readResult && *m_readResult) {
		readError = *m_readResult;
	}
	else {
		readError = boost::system::error_code();
	}

	return true;
}

//======================================================================

statuscodes::Codes UdpReceiveThreadEnsemble::sendCommand(const CommandBase& /*cmd*/)
{
	return statuscodes::SendingCommandFailed; // UNUSED
}

//======================================================================

statuscodes::Codes UdpReceiveThreadEnsemble::sendCommand(const CommandBase& /*cmd*/,
                                                    CommandReplyBase& /*reply*/,
                                                    const boost::posix_time::time_duration /*timeOut*/)
{
	return statuscodes::SendingCommandFailed; // UNUSED
}

//======================================================================

} // namespace ibeosdk

//======================================================================
