//======================================================================
/*! \file IbeoTcpIpAcceptorBase.hpp
 *
 * \copydoc Copyright
 * \author Julia Nitsch (jn)
 * \date May 17, 2016
 *///-------------------------------------------------------------------

//======================================================================

#ifndef IBEOSDK_IBEOTCPIPACCEPTORBASE_HPP_SEEN
#define IBEOSDK_IBEOTCPIPACCEPTORBASE_HPP_SEEN

//======================================================================

#include <ibeosdk/LogFileManager.hpp>
#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/misc/StatusCodes.hpp>

#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/optional/optional.hpp>

#include <set>

//======================================================================

namespace ibeosdk {

//======================================================================

class DataBlock;

//======================================================================
/*!\brief Class for accepting connections via TCP/IP and sending data
 *        via this connection.
 * \author Julia Nitsch (jn)
 * \version 0.1
 * \date May 17, 2016
 *///-------------------------------------------------------------------
class IbeoTcpIpAcceptorBase {
public:
	//========================================
	/*!\brief Convenience ptr for boost shared_ptr.
	 *///-------------------------------------
	typedef boost::shared_ptr<IbeoTcpIpAcceptorBase> Ptr;

	//=====================================================================
	/*!\brief Class which holds the current connection.
	 * \author Julia Nitsch (jn)
	 * \version 0.1
	 * \date May 17, 2016
	 *///-------------------------------------------------------------------
	class SessionBase {
	public:
		//========================================
		/*!\brief Convenience ptr for boost shared_ptr.
		 *///-------------------------------------
		typedef boost::shared_ptr<SessionBase> Ptr;

	public:
		//========================================
		/*!\brief Creates a SessionBase.
		 *
		 * \param[in] io_service      Service which
		 *                            established connection.
		 * \param[in] deviceId        Device id of our simulated device
		 *                            needed for ibeo data header.
		 *///-------------------------------------
		SessionBase(IbeoTcpIpAcceptorBase* const parent,
		            boost::asio::io_service& io_service,
		            const UINT8 deviceId = 1);

		//========================================
		/*!\brief Creates a SessionBase.
		 *
		 * \param[in] io_service      Service which
		 * \param[in] rangeStart      Start of initial filter
		 *                            range which decides
		 *                            which DataBlock will be
		 *                            send via this session.
		 * \param[in] rangeEnd        End of initial filter
		 *                            range which decides
		 *                            which DataBlock will be
		 *                            send via this session.
		 *                            established connection.
		 * \param[in] deviceId        Device id of our simulated device
		 *                            needed for ibeo data header.
		 *///-------------------------------------
		SessionBase(IbeoTcpIpAcceptorBase* const parent,
			        boost::asio::io_service& io_service,
		            const DataTypeId rangeStart,
		            const DataTypeId rangeEnd,
		            const UINT8 deviceId = 1);

		//========================================
		/*!\brief Destructs a SessionBase.
		 *
		 * Closes socket.
		 *///-------------------------------------
		virtual ~SessionBase();

		//========================================
		/*!\brief Send a DataBlock.
		 * \param[in] dataBlock  the DataBlock which should be sent.
		 * \return The result whether the operation could be started or not.
		 * \sa ErrorCode
		 *///-------------------------------------
		statuscodes::Codes sendDatablock(const ibeosdk::DataBlock& dataBlock);

		//========================================
		/*!\brief Starts listening thread.
		 *///-------------------------------------
		void startListening() { startListen(); }

		//========================================
		/*!\brief Checks whether connection is valid.
		 * \return \c true if connection is still valid,
		 *         \c false otherwise.
		 *///-------------------------------------
		bool isConnectionValid() { return (m_connectionALive && m_socket.is_open()); }

	public:
		boost::asio::ip::tcp::socket& getSocket() { return m_socket; }
		bool getConnectionALive() { return m_connectionALive; }

	public:
		void setConnectionALive(const bool connectionALive) { m_connectionALive = connectionALive; }

		//========================================
		/*!\brief
		 * \param[in] startRange      Start of filter
		 *                            range which decides
		 *                            which DataBlock will be
		 *                            send via this session.
		 * \param[in] endRange        End of filter
		 *                            range which decides
		 *                            which DataBlock will be
		 *                            send via this session.
		 *                            established connection.
		 *
		 *///-------------------------------------
		void setRange(const DataTypeId& startRange, const DataTypeId& endRange)
		{
			boost::mutex::scoped_lock lock(m_sendMutex);

			m_startRange = startRange;
			m_endRange = endRange;
		}

		void setSizeOfPrevMsg(const uint32_t sizePrevMsg) { m_sizePrevMsg = sizePrevMsg; }

	public:
		void cancelAsyncIos() { m_socket.cancel(); }

	protected:
		//========================================
		/*!\brief Worker function for m_ListenThreadPtr
		 *        needs to be overloaded by child.
		 *///-------------------------------------
		virtual void startListen() = 0;

	protected:
		IbeoTcpIpAcceptorBase* const m_parent;

		//========================================
		/*!\brief Socket which holds the connection.
		 *///-------------------------------------
		boost::asio::ip::tcp::socket m_socket;

		//========================================
		/*!\brief Holds device id, needed when
		 *        writing an IbeoDataHeader.
		 * \note Default value is 1.
		 *///-------------------------------------
		UINT8 m_deviceId;

	protected:
		//========================================
		/*!\brief Lock sending function
		 *
		 * Can be called by listening thread, when replying,
		 * but also by other writing threads.
		 *///-------------------------------------
		boost::mutex m_sendMutex;

		//========================================
		/*!\brief Saves the size of the last send msg.
		 *
		 * Needed when writing an IbeoDataHeader when
		 * sending a DataBlock or replying to commands.
		 *///-------------------------------------
		UINT32 m_sizePrevMsg;

		//========================================
		/*!\brief Flag which holds information/guess
		 *        whether the socket is still connected.
		 *///-------------------------------------
		bool m_connectionALive;

		//========================================
		/*!\brief Starting range id for DataBlocks which
		 *        should be sent.
		 * \note Initialized with DataTypeId::DataType_Unknown.
		 *///-------------------------------------
		DataTypeId m_startRange;

		//========================================
		/*!\brief Ending range id for DataBlocks which
		 *        should be sent
		 * \note Initialized with DataTypeId::DataType_LastId.
		 *///-------------------------------------
		DataTypeId m_endRange;

		std::vector<char> m_sendBuffer;
	}; //IbeoTcpIpAcceptorBase::SessionBase

public:
	//========================================
	/*!\brief Creates an IbeoTcpIpAcceptorBase.
	 *
	 * \param[in] logFileManager  LogFileManager which is
	 *                            handle the splitting of
	 *                            output files and log files.
	 * \param[in] port            Port number for the connection.
	 *///-------------------------------------
	IbeoTcpIpAcceptorBase(ibeosdk::LogFileManager* const logFileManager, const unsigned short port = 12002);

	//========================================
	/*!\brief Creates an IbeoTcpIpAcceptorBase.
	 *
	 * \param[in] logFileManager  LogFileManager which is
	 *                            handle the splitting of
	 *                            output files and log files.
	 * \param[in] port            Port number for the connection.
	 *///-------------------------------------
	IbeoTcpIpAcceptorBase(ibeosdk::LogFileManager* const logFileManager,
	                      const boost::asio::deadline_timer::duration_type writeExpirationTime,
	                      const unsigned short port = 12002);

	//========================================
	/*!\brief Creates an IbeoTcpIpAcceptorBase.
	 *
	 * \param[in] logFileManager  LogFileManager which is
	 *                            handle the splitting of
	 *                            output files and log files.
	 * \param[in] port            Port number for the connection.
	 *///-------------------------------------
	IbeoTcpIpAcceptorBase(ibeosdk::LogFileManager* const logFileManager,
	                      const boost::optional<boost::asio::deadline_timer::duration_type> writeExpirationTime,
	                      const unsigned short port = 12002);

	//========================================
	/*!\brief Destructor of the IbeoTcpIpAcceptor class.
	 *
	 * Stopping io service thread and
	 * destroying the socket.
	 *///-------------------------------------
	virtual ~IbeoTcpIpAcceptorBase();

public:
	//========================================
	/*!\brief Initialization for acceptor.
	 *        Start to accept new connections.
	 *///-------------------------------------
	void init();

	//========================================
	/*!\brief Returns whether there is at least
	 *        one session active.
	 * \return \c true if at least one session is active.
	 *         \c false otherwise.
	 *///-------------------------------------
	bool hasSessions() const { return !m_sessions.empty(); }

	//========================================
	/*!\brief Get the number of active sessions.
	 * \return The number of activate sessions.
	 *///-------------------------------------
	int getNbOfSession() const { return int(m_sessions.size()); }

	//========================================
	/*!\brief Sends \a dataBlock to all open connections.
	 *        And wait till the writes have finished.
	 * \param[in] dataBlock  The DataBlock which should
	 *                       be sent.
	 * \return The result of the operation.
	 * \sa statuscodes::Codes
	 *///-------------------------------------
	statuscodes::Codes sendDataBlock(const DataBlock& dataBlock);

protected:
	//========================================
	/*!\brief Sends \a dataBlock to all open connections.
	 * \param[in] dataBlock  The DataBlock which should
	 *                       be sent.
	 *///-------------------------------------
	void issueWriteOperations(const DataBlock& dataBlock);

	//========================================
	/*!\brief Wait for all write operations to
	 *        be completed.
	 *///-------------------------------------
	void waitForWriteOperationsBeCompleted();


	//========================================
	/*!\brief Handler for timeout situations.
	 * \param[in] session              Session that started
	 *                                 the deadline timer.
	 * \param[in] error                Result of waiting for
	 *                                 the deadline timer of
	 *                                 the \a session.
	 * \param[in] nbOfBytesTransfered  Number of bytes written
	 *                                 from the buffer. If an
	 *                                 error occurred this will be
	 *                                 less than the expected
	 *                                 size.
	 *///-------------------------------------
	void writeDone(SessionBase* const session,
	               const boost::system::error_code& error,
	               const std::size_t nbOfBytesTransfered);

	void writeTimeout(const boost::system::error_code& error);

	void cancelWriteOperations();

protected:
	//========================================
	/*!\brief Gets current session ptr.
	 *
	 * Needs to be implemented by child classes.
	 * \return Session ptr casted to sessionBase.
	 *///-------------------------------------
	virtual SessionBase::Ptr getSessionPtr() = 0;

	//========================================
	/*!\brief Gets new session ptr initialized with io_service.
	 * \param[in] io_service  Service which handles connections.
	 * \return Session ptr casted to sessionBase.
	 *///-------------------------------------
	virtual SessionBase::Ptr getNewSessionPtr(boost::asio::io_service& io_service) = 0;

private:
	//========================================
	/*!\brief Working function for accepting new
	 *        requests from m_acceptIOServiceThreadPtr.
	 *///-------------------------------------
	void acceptorIOServiceThread();

	//========================================
	/*!\brief Handles accepts, running in context
	 *        of m_acceptIOServiceThreadPtr.
	 *///-------------------------------------
	void handleAccept(const boost::system::error_code& error);

	//========================================
	/*!\brief Closing acceptor.
	 *///-------------------------------------
	void closeAcceptor();

private:
	//========================================
	/*!\brief Session ptr which will handle the
	 *        next accept.
	 *
	 * Will be append to m_sessions when a new
	 * session has been accepted. And a new
	 * session pointer will be created for the
	 * next session to be accepted.
	 *///-------------------------------------
	SessionBase::Ptr m_sessionPtr;

	//========================================
	/*!\brief Handles writing of logfile.
	 * \attention Needs to be set by constructor.
	 *///-------------------------------------
	LogFileManager* m_logFileManager;

	//========================================
	/*!\brief Service which handles accepts and
	 *        then handed over to sessions.
	 *///-------------------------------------
	boost::asio::io_service m_ioService;

private:
	//========================================
	/*!\brief Prevent sessions to be accessed
	 *       simultaneously by multiple threads.
	 *///-------------------------------------
	boost::mutex m_sessionsMutex;

	//========================================
	/*!\brief Vector which holds all open connections.
	 *
	 * If connection is detected as being closed
	 * (e.g during writing) it will be deleted.
	 *///-------------------------------------
	std::vector<SessionBase::Ptr> m_sessions;

private: // used by write operation
	//========================================
	/*!\brief Maps holds for each session the
	 *        expected number of bytes to be
	 *        send.
	 *///-------------------------------------
	std::map<SessionBase*, uint32_t> m_activeSending;

	//========================================
	/*!\brief Current status of the write operations
	 *        to all sessions.
	 *///-------------------------------------
	statuscodes::Codes m_sendingStatus;

	//========================================
	/*!\brief Mutex to guard \a m_activeSending.
	 *///-------------------------------------
	boost::mutex m_writeCondMutex;

	//========================================
	/*!\brief Condition to signaling whether
	 *        all write operations have been
	 *        completed.
	 *///-------------------------------------
	boost::condition_variable writeCondition;

	//========================================
	/*!\brief Expiring duration for write operations.
	 *///-------------------------------------
	boost::asio::deadline_timer::duration_type m_writeExpirationPeriod;

	//========================================
	/*!\brief Deadline timer for write operations.
	 *///-------------------------------------
	boost::asio::deadline_timer m_writeExprirationTimer;


	enum WriteState {
		WS_Idle,
		WS_InProgress,
		WS_Error,
		WS_TimeOut,
		WS_Completed
	};

	WriteState m_writeState;

private:
	//========================================
	/*!\brief Accepting requests.
	 *///-------------------------------------
	boost::asio::ip::tcp::acceptor m_acceptor;

	//========================================
	/*!\brief Thread waiting for connection requests.
	 *///-------------------------------------
	boost::scoped_ptr<boost::thread> m_acceptIOServiceThreadPtr;
}; //IbeoTcpIpAcceptorBase

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IBEOTCPIPACCEPTORBASE_HPP_SEEN

//======================================================================
