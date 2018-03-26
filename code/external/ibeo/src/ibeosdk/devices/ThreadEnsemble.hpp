//======================================================================
/*! \file ThreadEnsemble.hpp
 *
 * \copydoc Copyright
 * \author Yannic Wilkening (YW)
 * \date July 20, 2017
 *///-------------------------------------------------------------------

#ifndef SRC_IBEOSDK_DEVICES_THREADENSEMBLE_HPP_SEEN
#define SRC_IBEOSDK_DEVICES_THREADENSEMBLE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/devices/IbeoDeviceBase.hpp>

#include <boost/asio.hpp>
#include <boost/optional.hpp>

//======================================================================

namespace ibeosdk {

class IbeoDataHeader;
class DataBlock;

//======================================================================

class ThreadEnsemble {
protected:
	enum ThreadState {
		TS_NotRunning,
		TS_Starting,
		TS_Running,
		TS_Stopping,
		TS_StartFailed,
		TS_RunFailed
	}; // ThreadState

	typedef boost::mutex::scoped_lock Lock;
	typedef boost::mutex Mutex;

public:
	virtual ~ThreadEnsemble(){};

public:
	virtual void run() = 0;
	virtual void stop() = 0;
	virtual bool isRunning(const bool useMutex = true) = 0;
	virtual bool isNotYetRunning(const bool useMutex = true) = 0;

	//========================================
	/*!\brief Get the expiry time for IO operations.
	 * \return The expiry time for IO operations.
	 *///-------------------------------------
	virtual boost::optional<boost::asio::deadline_timer::duration_type> getExpiryTime() = 0;

	//========================================
	/*!\brief Set the expiry time for IO operations.
	 * \param[in] expiryTime  The expiry time for IO operations.
	 *///-------------------------------------
	virtual void setExpiryTime(const boost::asio::deadline_timer::duration_type expiryTime) = 0;

	//========================================
	/*!\brief Reset the expiry time for IO operations.
	 * \param[in] expiryTime  The expiry time for IO operations.
	 *///-------------------------------------
	virtual void resetExpiryTime() = 0;

	//========================================
	/*!\brief Send a command which expects no reply.
	 * \param[in] cmd  Command to be sent.
	 * \return The result of the operation.
	 * \sa ErrorCode
	 *///-------------------------------------
	virtual statuscodes::Codes sendCommand(const CommandBase& cmd) = 0;

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
	virtual statuscodes::Codes sendCommand(const CommandBase& cmd,
	                                       CommandReplyBase& reply,
	                                       const boost::posix_time::time_duration timeOut = boost::posix_time::milliseconds(500)) = 0;

protected:
	virtual void recvThreadMain() = 0;
	virtual bool powerUpConnection() = 0;
	virtual void disconnect() = 0;

}; // ThreadEnsemble

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // SRC_IBEOSDK_DEVICES_THREADENSEMBLE_HPP_SEEN

//======================================================================
