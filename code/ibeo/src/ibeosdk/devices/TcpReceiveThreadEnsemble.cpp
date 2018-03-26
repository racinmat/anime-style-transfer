//======================================================================
/*! \file TcpReceiveThreadEnsemble.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date May 5, 2017
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/Log.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/scoped_ptr.hpp>

#include <ibeosdk/devices/TcpReceiveThreadEnsemble.hpp>
#include <ibeosdk/devices/IbeoDeviceBase.hpp>
#include <ibeosdk/MsgBuffer.hpp>
#include <ibeosdk/EventMonitor.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

TcpReceiveThreadEnsemble::TcpReceiveThreadEnsemble(const boost::asio::ip::address& ip,
                                                   const unsigned short port,
                                                   ReceiveDataBlock receiveDataBlock)
  : m_ioService(),
    m_endPoint(ip, port),
    m_socket(),
    m_expiryTime(),
    m_receiveDataBlock(receiveDataBlock),
    m_mutex(),
    m_threadStateCondition(),
    m_thread(NULL),
    m_threadState(TS_NotRunning),
    m_rMutex(),
    m_replyPtr(NULL),
    m_expectedReplyId(CommandId(CommandId::CmdLuxReset)),
    m_eventMonitor(),
    m_eventReplyReceived(m_eventMonitor.newEvent()),
    m_sizeOfPrevCommand(0)
{}

//======================================================================

TcpReceiveThreadEnsemble::~TcpReceiveThreadEnsemble()
{

	try {
		Lock lock(m_mutex);
		if (m_threadState == TS_Running || m_threadState == TS_Starting) {
			// Thread has not yet been stopped.
			// Ask for termination and wait till a state change
			// has been performed.
			m_threadState = TS_Stopping;
			m_threadStateCondition.wait(lock);
		}
	}
	catch (...) {
		logError << "failed 2" << std::endl;
	}

	if (this->m_thread->joinable()) {
		this->m_thread->join();
		delete this->m_thread;
		this->m_thread = NULL;
	}
}

//======================================================================

boost::optional<boost::asio::deadline_timer::duration_type> TcpReceiveThreadEnsemble::getExpiryTime()
{
	return m_expiryTime;
}

//======================================================================

void TcpReceiveThreadEnsemble::setExpiryTime(const boost::asio::deadline_timer::duration_type expiryTime)
{
	Lock lock(m_mutex);
	m_expiryTime.reset(expiryTime);
	logDebug2 << "Set new ExpiryTime" << std::endl;
}

//======================================================================

void TcpReceiveThreadEnsemble::resetExpiryTime()
{
	Lock lock(m_mutex);
	m_expiryTime.reset();
}

//======================================================================

void TcpReceiveThreadEnsemble::run()
{
	if (m_thread)
		return;

	try {
		Lock lock(m_mutex);

		m_thread = new boost::thread(&TcpReceiveThreadEnsemble::recvThreadMain, this);
		while ((this->m_threadState==TS_NotRunning || this->m_threadState==TS_Starting)) {
			m_threadStateCondition.wait(lock);
		}
	}
	catch (const boost::lock_error& e) {
		logError << e.what() << std::endl;
	}

}

//======================================================================

bool TcpReceiveThreadEnsemble::powerUpConnection()
{

	if (!m_receiveDataBlock) {
		logError << "Failed to start TCP/IP receive thread. No onData method has be specified." << std::endl;
		return false;
	}

	try {
		Lock lock(m_mutex);
		m_threadState = TS_Starting;
		m_threadStateCondition.notify_all();
	}
	catch (const boost::lock_error& e) {
		logError << e.what() << std::endl;
		return false;
	}

	try {
		m_socket.reset(new boost::asio::ip::tcp::socket(m_ioService));
	}
	catch (const std::exception& e) {
		logError << "Failed to allocate socket. - " << e.what() << std::endl;
		Lock lock(m_mutex);
		m_threadState = TS_StartFailed;
		m_threadStateCondition.notify_all();
		return false;
	}

	boost::system::error_code ec;
	m_socket->connect(m_endPoint, ec);

	if (ec) {
		logError << "Failed to connect to device (" << m_endPoint << ") " << ec.message() << std::endl;
		Lock lock(m_mutex);
		m_threadState = TS_StartFailed;
		m_threadStateCondition.notify_all();
		return false;
	}


	logDebug << "Connected to device on " << m_endPoint << std::endl;

	try {
		boost::asio::ip::tcp::no_delay option(true);
		m_socket->set_option(option);
	}
	catch (const std::exception& e) {
		logError << "Failed to allocate socket. - "	<< e.what() << std::endl;
		Lock lock(m_mutex);
		m_threadState = TS_StartFailed;
		m_threadStateCondition.notify_all();
		return false;
	}

	Lock lock(m_mutex);
	m_threadState = TS_Running;
	m_threadStateCondition.notify_all();

	return true;
}

//======================================================================

void TcpReceiveThreadEnsemble::recvThreadMain()
{
	const bool powerUpOk= powerUpConnection();
	boost::system::error_code error;
	const char* bodyBuf;
	const IbeoDataHeader* dh;
	ibeosdk::MsgBuffer msgBuf(m_socket.get(), bufferSize);


	try {
		Lock lock(m_mutex);
		m_threadState = powerUpOk ? TS_Running : TS_RunFailed;
		m_threadStateCondition.notify_all();
	}
	catch (const boost::lock_error& e) {
		logError << e.what() << std::endl;
		return;
	}

	if (!powerUpOk) {
		return;
	}

	boost::optional<boost::asio::deadline_timer::duration_type> expiryTime;

	try {
		while (this->m_threadState != TS_Stopping && m_socket->is_open()) {

			{
				Lock lock(m_mutex);
				expiryTime = m_expiryTime;
			}

			if (msgBuf.recvMsg(dh, bodyBuf, expiryTime)) {
				logDebug << "Received DataBlock of type " << toHex(dh->getDataType()) << std::dec << std::endl;

				if ((dh->getDataType() == DataTypeId::DataType_Reply)) {
					onReceiveCommandReply(*dh, bodyBuf);
					continue; // do not inform any streamer or listener
				}

				m_receiveDataBlock(dh, bodyBuf);
			}
		}
	}
	catch (const boost::system::error_code& errorCode) {
		logError << "In receive loop: " << errorCode.message() << std::endl;
		m_threadState = TS_RunFailed;
		m_threadStateCondition.notify_all();
		return;
	}

	if (!m_socket->is_open() && this->m_threadState != TS_Stopping) {
		try {
			Lock lock(this->m_mutex);
			m_threadState = TS_RunFailed;
			m_threadStateCondition.notify_all();
		}
		catch (const boost::lock_error& e) {
			logError << e.what() << std::endl;
		}

		return;
	}

	m_threadState = TS_NotRunning;
	m_threadStateCondition.notify_all();
}

//======================================================================

void TcpReceiveThreadEnsemble::disconnect() {
	if (m_socket.get() != NULL) {
		m_socket->close();
		m_socket.reset();
	}
}

//======================================================================

void TcpReceiveThreadEnsemble::stop()
{
	if (isRunning(false)) {
		try {
			Lock lock(this->m_mutex);
			m_threadState = TS_Stopping;
		}
		catch (const boost::lock_error& e) {
			logError << e.what() << std::endl;
		}
	}
	disconnect();
}

//======================================================================

bool TcpReceiveThreadEnsemble::isRunning(const bool useMutex)
{
	ThreadState state = TS_NotRunning;
	if (useMutex) {
		try {
			Lock lock(this->m_mutex);
			state = m_threadState;
		}
		catch (const boost::lock_error& e) {
			logError << e.what() << std::endl;
		}
	}
	else {
		state = m_threadState;
	}

	if (state == TS_Starting || state == TS_Running) {
		return true;
	}
	return false;
}

//======================================================================

bool TcpReceiveThreadEnsemble::isConnected()
{
	if (m_socket == NULL)
		return false;

	if (!m_socket->is_open())
		return false;

	return true;
}

//======================================================================

bool TcpReceiveThreadEnsemble::isNotYetRunning(const bool useMutex)
{
	ThreadState state = TS_NotRunning;
	if (useMutex) {
		try {
			Lock lock(this->m_mutex);
			state = m_threadState;
		}
		catch (const boost::lock_error& e) {
			logError << e.what() << std::endl;
		}
	}
	else {
		state = m_threadState;
	}

	if (state == TS_NotRunning || state == TS_Starting) {
		return true;
	}
	return false;
}

//======================================================================

statuscodes::Codes TcpReceiveThreadEnsemble::sendCommand(const CommandBase& cmd)
{
	if (!isConnected()) {
		// cannot send a command if the device is not connected.
		logError << " command (id " << toHex(cmd.getCommandId()) << ") failed " << statuscodes::NotConnected << std::endl;
		return statuscodes::NotConnected;
	}

	// Send command
	logDebug2 << "Sending " << " command (id " << toHex(cmd.getCommandId()) << ") " << "; expecting no reply" << std::endl;

	const statuscodes::Codes errorCode = sendCommandInternal(cmd);
	logDebug2 << "Done. " << errorCode << std::endl;

	return errorCode;
}

//======================================================================

statuscodes::Codes TcpReceiveThreadEnsemble::sendCommand(const CommandBase& cmd,
                                                         CommandReplyBase& reply,
                                                         const boost::posix_time::time_duration timeOut)
{
	if (!isConnected()) {
		// cannot send a command if the device is not connected.
		logError << " command (id " << toHex(cmd.getCommandId()) << ") failed  Reply: "
				<< toHex(reply.getCommandId()) << " "
				<< statuscodes::NotConnected << std::endl;
		return statuscodes::NotConnected;
	}

	if (cmd.getCommandId() != reply.getCommandId()) {
		logError << " command (id " << toHex(cmd.getCommandId()) << ") failed  Reply: "
				<< toHex(reply.getCommandId()) << " "
				<< statuscodes::MismatchingCommandAndReply << std::endl;
		return statuscodes::MismatchingCommandAndReply;
	}

	{
		// Pass reply buffer to onReceiveCommandReply() which is running
		// in the reception thread
		boost::recursive_timed_mutex::scoped_lock criticalSection(m_rMutex);
		assert (!m_replyPtr); // previous reply must have been completely processed
		m_replyPtr = &reply;
		m_expectedReplyId = cmd.getCommandId();
	}

	statuscodes::Codes errorCode = statuscodes::EverythingOk;
	try {
		// Send command
		logDebug2 << "Sending " << " command (id " << toHex(cmd.getCommandId())
				<< ")" << std::endl;

		if (sendCommandInternal(cmd) != statuscodes::EverythingOk)
			throw statuscodes::SendingCommandFailed;

		// Wait for reply. The event m_eventReplyReceived will be signaled by
		EventMonitor::Mask event = m_eventMonitor.wait(timeOut, m_eventReplyReceived);

		if (event == EventMonitor::TimeOutEvent)
			throw statuscodes::TimeOut;
		else if (   (cmd.getCommandId() | CommandReplyBase::errorMask)
		         != (reply.getCommandId() | CommandReplyBase::errorMask))
			throw statuscodes::ReplyMismatch;
		else if (reply.isErrorReply())
			throw statuscodes::ReceiveCommandErrorReply;
	} // try
	catch (const statuscodes::Codes& ec) {
		errorCode = ec;
		logError << " command (id " << toHex(cmd.getCommandId())
				<< ") failed  Reply: " << toHex(reply.getCommandId())
				<< " " << ec << std::endl;
	} // catch

	{
		// Clear reply buffer for reception thread
		boost::recursive_timed_mutex::scoped_lock criticalSection(m_rMutex);
		assert (m_replyPtr);
		m_replyPtr = NULL;
		m_expectedReplyId = CommandId(0x7fff);
	}

	return errorCode;
}

//======================================================================

statuscodes::Codes TcpReceiveThreadEnsemble::sendCommandInternal(const CommandBase& cmd)
{
	boost::posix_time::ptime timeout
	= boost::posix_time::microsec_clock::universal_time() + boost::posix_time::milliseconds(50);
	boost::recursive_timed_mutex::scoped_timed_lock criticalSection(m_rMutex, timeout);

	if (criticalSection) { // if we have got thread-safe access in time
		if (isConnected()) {
			const uint32_t cmdLength = uint32_t(cmd.getCommandLength());
			const uint32_t sizeOfThisMsg = cmdLength;
			const uint32_t sizeOfBuffer = sizeOfThisMsg + IbeoDataHeader::getHeaderSize();

			boost::scoped_ptr<char> sendBuf(new char[sizeOfBuffer]);
			boost::iostreams::stream<boost::iostreams::array> strm(sendBuf.get(), sizeOfBuffer);
			std::ostream& s = (std::ostream&)strm;

			const IbeoDataHeader header(DataTypeId::DataType_Command,
			                            m_sizeOfPrevCommand,
			                            sizeOfThisMsg,
			                            1,
			                            NTPTime());

			logDebug2 << "Sending command with " <<  sizeOfThisMsg << " bytes"<< std::endl;

			header.serialize(s);
			cmd.serializeToStream(s);
			if (!s.good())
				return statuscodes::FailedToPrepareSendingCommand;

			const size_t nbOfBytesSend = m_socket->send(boost::asio::buffer(sendBuf.get(), sizeOfBuffer));
			if (nbOfBytesSend != size_t(sizeOfBuffer)) {
				logError << "Send " << nbOfBytesSend << " expected: " << sizeOfBuffer << std::endl;

				return statuscodes::SendingCommandFailed;
			}

			m_sizeOfPrevCommand = uint32_t(sizeOfThisMsg);

			return statuscodes::EverythingOk;
		}
		return statuscodes::NotConnected;
	}

	logError << "Timeout while locking critical section" << std::endl;
	return statuscodes::TimeOutCriticalSection;
}

//======================================================================

void TcpReceiveThreadEnsemble::onReceiveCommandReply(const IbeoDataHeader& dh, const char* bodyBuf)
{
	{
		const uint8_t* bb = reinterpret_cast<const uint8_t*>(bodyBuf);
		logDebug << "INCOMMING REPLY: " << toHex(uint16_t(*(bb+1) + ((*(bb))<<8)))
				<< "  Size: " << dh.getMessageSize() << std::endl;
	}

	if (m_replyPtr != NULL) {
		boost::iostreams::stream<boost::iostreams::array_source> strm(bodyBuf, dh.getMessageSize());
		std::istream& s = (std::istream&)strm;
		if (m_replyPtr->deserializeFromStream(s, dh)) {
			logDebug2 << "Received Reply -- good" << std::endl;
		}
		else {
			logDebug2 << "Received Reply -- failed" << std::endl;
			if (m_replyPtr)
				m_replyPtr->setErrorReply();
		}

		// Wake up main thread waiting in command()
		m_eventMonitor.signal(m_eventReplyReceived);
	}
	else {
		logDebug2 << "INCOMMING REPLY UNEXPECTED " << std::endl;
	}
}

//======================================================================

} // namespace ibeosdk

//======================================================================
