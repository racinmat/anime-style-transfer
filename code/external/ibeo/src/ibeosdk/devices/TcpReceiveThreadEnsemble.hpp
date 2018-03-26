//======================================================================
/*! \file TcpReceiveThreadEnsemble.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date May 5, 2017
 *///-------------------------------------------------------------------

#ifndef SRC_IBEOSDK_DEVICES_TCPRECEIVETHREAD_HPP_SEEN
#define SRC_IBEOSDK_DEVICES_TCPRECEIVETHREAD_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/devices/IbeoDeviceBase.hpp>
#include <ibeosdk/devices/ThreadEnsemble.hpp>

#include <boost/asio.hpp>
#include <boost/optional.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class TcpReceiveThreadEnsemble : public ThreadEnsemble {
public:
	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> TcpSocketPtr;
	typedef CommandReplyBase* CommandReplyBasePtr;

	typedef const ibeosdk::DataBlock* DataBlockPtr;
	typedef boost::function <void(const ibeosdk::IbeoDataHeader* dh, const char* bodyBuf)> ReceiveDataBlock;

	typedef boost::condition Condition;

public:
	static const int bufferSize = 4*65536; // 4*64K

public:
	TcpReceiveThreadEnsemble(const boost::asio::ip::address& ip,
                             const unsigned short port,
	                         ReceiveDataBlock receiveDataBlock);
	virtual ~TcpReceiveThreadEnsemble();

public:
	void run();
	void stop();
	bool isRunning(const bool useMutex = true);
	bool isNotYetRunning(const bool useMutex = true);

	//========================================
	/*!\brief Get the expiry time for IO operations.
	 * \return The expiry time for IO operations.
	 *///-------------------------------------
	boost::optional<boost::asio::deadline_timer::duration_type> getExpiryTime();

	//========================================
	/*!\brief Set the expiry time for IO operations.
	 * \param[in] expiryTime  The expiry time for IO operations.
	 *///-------------------------------------
	void setExpiryTime(const boost::asio::deadline_timer::duration_type expiryTime);

	//========================================
	/*!\brief Reset the expiry time for IO operations.
	 * \param[in] expiryTime  The expiry time for IO operations.
	 *///-------------------------------------
	void resetExpiryTime();

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
	void recvThreadMain();
	bool powerUpConnection();

	statuscodes::Codes sendCommandInternal(const CommandBase& cmd);
	void onReceiveCommandReply(const IbeoDataHeader& dh, const char* bodyBuf);
	bool isConnected();
	void disconnect();

protected:
	boost::asio::io_service m_ioService;

	boost::asio::ip::tcp::endpoint m_endPoint;

	TcpSocketPtr m_socket;

	//========================================
	/*!\brief The expiry time for IO operations.
	 *///-------------------------------------
	boost::optional<boost::asio::deadline_timer::duration_type> m_expiryTime;

	ReceiveDataBlock m_receiveDataBlock;

	Mutex m_mutex;
	Condition m_threadStateCondition;

	boost::thread* m_thread;

	//========================================
	/*!\brief State of the recieve thread.
	 * \sa m_thread
	 *///-------------------------------------
	ThreadState m_threadState;

	boost::recursive_timed_mutex m_rMutex;
	CommandReplyBasePtr m_replyPtr;
	CommandId m_expectedReplyId;
	EventMonitor m_eventMonitor;
	EventMonitor::Mask m_eventReplyReceived;
	uint32_t m_sizeOfPrevCommand;

}; // TcpReceiveThread

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // SRC_IBEOSDK_DEVICES_TCPRECEIVETHREAD_HPP_SEEN

//======================================================================
