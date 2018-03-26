//======================================================================
/*! \file MsgBuffer.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jun 28, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_MSGBUFFER_HPP_SEEN
#define IBEOSDK_MSGBUFFER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/MsgBufferBase.hpp>
#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>

#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\brief
 * \author Jan Christian Dittmer (jcd)
 * \version 1.0
 * \date Jun 28, 2012
 *
 * Class to receive ibeo messages from an TCP/IP socket and prepare them
 * for decoding.
 *
 * \attention This class can only be used if there is other thread
 *            performing a run (or equivalent) operation on the
 *            socket's io_service.
 *///-------------------------------------------------------------------
class MsgBuffer  : public MsgBufferBase {
public:
	//========================================
	/*!\brief Constructor
	 *
	 * \param[in, out]  recvSocket  Socket from which messages data
	 *                              will be received.
	 * \param[in]       bufSize     Size of the buffer
	 *                              which will be allocated
	 *                              to hold the received
	 *                              message data. This size has to be
	 *                              significantly larger than the largest
	 *                              to be expected message.
	 *///-------------------------------------
	MsgBuffer(boost::asio::ip::tcp::socket* recvSocket,
	          const int bufSize);

	//========================================
	/*!\brief Destructor.
	 *///-------------------------------------
	virtual ~MsgBuffer();

public:
	//========================================
	/*!\brief Receive Messages from the device.
	 *
	 * This variant of recvMsg will not have a timeout.
	 * Internally it will use a timeout but will try again if
	 * that duration has expired.
	 *
	 * \param[out] recvDataHeader  If a complete message is in the buffer,
	 *                             \a recvDataHeader will point to the attribute
	 *                             \a recvDataHeader which
	 *                             will contain the valid header data.
	 *                             Otherwise \a recvDataHeader will be \c NULL.
	 * \param[out] startOfMsgBody  If a complete message is in the buffer,
	 *                             \a startOfMsgBody will point to the start
	 *                             of the message body inside the objects buffer.
	 *                             Otherwise \a startOfMsgBody will be \c NULL.
	 * \return \c true if a complete message is in the buffer, ready to be processed.
	 *         The message is described by \a recvDataHeader and \a startOfMsgBody.
	 *         \c false There is no complete message in the buffer yet. \a recvDataHeader
	 *         and \a startOfMsgBody will be NULL.
	 * \sa recvMsgInternal
	 *///-------------------------------------
	bool recvMsg(const IbeoDataHeader*& recvDataHeader,
	             const char*& startOfMsgBody);

	//========================================
	/*!\brief Receive Messages from the device. U
	 *
	 * This variant of recvMsg has a timeout.
	 * If the timeout duration has expired, recvMsg will throw an exception.
	 *
	 * \param[out] recvDataHeader  If a complete message is in the buffer,
	 *                             \a recvDataHeader will point to the attribute
	 *                             \a recvDataHeader which
	 *                             will contain the valid header data.
	 *                             Otherwise \a recvDataHeader will be \c NULL.
	 * \param[out] startOfMsgBody  If a complete message is in the buffer,
	 *                             \a startOfMsgBody will point to the start
	 *                             of the message body inside the objects buffer.
	 *                             Otherwise \a startOfMsgBody will be \c NULL.
	 * \param[in]  expiryTime      Expiring time IO operations.
	 * \return \c true if a complete message is in the buffer, ready to be processed.
	 *         The message is described by \a recvDataHeader and \a startOfMsgBody.
	 *         \c false There is no complete message in the buffer yet. \a recvDataHeader
	 *         and \a startOfMsgBody will be NULL.
	 * \sa recvMsgInternal
	 *///-------------------------------------
	bool recvMsg(const IbeoDataHeader*& recvDataHeader,
	             const char*& startOfMsgBody,
	             const boost::asio::deadline_timer::duration_type expiryTime);

	//========================================
	/*!\brief Receive Messages from the device. U
	 *
	 * If \a expiryTime is set, it will be used and in case
	 * of a timeout an exception will be throu
	 * This variant of recvMsg will not have a timeout.
	 * Internally it will use a timeout but will try again if
	 * that duration has expired.
	 * This variant of recvMsg has a timeout.
	 * If the timeout duration has expired, recvMsg will throw an exception.
	 *
	 * \param[out] recvDataHeader  If a complete message is in the buffer,
	 *                             \a recvDataHeader will point to the attribute
	 *                             \a recvDataHeader which
	 *                             will contain the valid header data.
	 *                             Otherwise \a recvDataHeader will be \c NULL.
	 * \param[out] startOfMsgBody  If a complete message is in the buffer,
	 *                             \a startOfMsgBody will point to the start
	 *                             of the message body inside the objects buffer.
	 *                             Otherwise \a startOfMsgBody will be \c NULL.
	 * \param[in]  expiryTime      Optional expiring time IO operations.
	 * \return \c true if a complete message is in the buffer, ready to be processed.
	 *         The message is described by \a recvDataHeader and \a startOfMsgBody.
	 *         \c false There is no complete message in the buffer yet. \a recvDataHeader
	 *         and \a startOfMsgBody will be NULL.
	 * \note If no expiryTime has been provided, the timer will wake up every 1 seconds
	 *       to give the possibility to interrupt the io operation from outside, i.e.
	 *       killing the thread.
	 * \sa recvMsgInternal
	 *///-------------------------------------
	bool recvMsg(const IbeoDataHeader*& recvDataHeader,
	             const char*& startOfMsgBody,
	             const boost::optional<boost::asio::deadline_timer::duration_type> expiryTime);

protected:
	//========================================
	/*!\brief Receive Messages from the device.
	 *
	 * \param[out] recvDataHeader  If a complete message is in the buffer,
	 *                             \a recvDataHeader will point to the attribute
	 *                             \a recvDataHeader which
	 *                             will contain the valid header data.
	 *                             Otherwise \a recvDataHeader will be \c NULL.
	 * \param[out] startOfMsgBody  If a complete message is in the buffer,
	 *                             \a startOfMsgBody will point to the start
	 *                             of the message body inside the objects buffer.
	 *                             Otherwise \a startOfMsgBody will be \c NULL.
	 * \param[in]  expiryTime      Expiring time for the IO operations.
	 * \return \c true if a complete message is in the buffer, ready to be processed.
	 *         The message is described by \a recvDataHeader and \a startOfMsgBody.
	 *         \c false There is no complete message in the buffer yet. \a recvDataHeader
	 *         and \a startOfMsgBody will be NULL.
	 * \sa getNewDataIfNeeded
	 *///-------------------------------------
	bool recvMsgInternal(const IbeoDataHeader*& dh,
	                     const char*& startOfMsgBody,
	                     const boost::asio::deadline_timer::duration_type expiryTime);

	//========================================
	/*!\brief Receive new data from the socket if necessary.
	 *
	 * This method will be called by recvMsgInternal.
	 * \param[in] expiryTime      Expiring time for the IO operations.
	 *///-------------------------------------
	int getNewDataIfNeeded(const boost::asio::deadline_timer::duration_type expiryTime);


	//========================================
	/*!\brief Read from the socket asynchronously.
	 *
	 * The method issues an asynchronous read operation
	 * on the given socket \a s. If the duration of
	 * \a expiryTime has been expired, the read operation
	 * will be canceled and the method returns.
	 *
	 * This method is running a deadline timer and an local
	 * event loop for the io_service to receive the events
	 * from the timer and the asynchronous read operation.
	 *
	 * \param[in]  s               Socket from which to be read.
	 * \param[out] buffers         A buffer where to read into.
	 * \param[in]  expiryTime      Expiring time for the IO operations.
	 * \param[out] readError       Error state of the read statement.
	 *                             If there was a timeout and the flag
	 *                             m_stopOnTimeOut was set, the error
	 *                             will be that the operation has been
	 *                             canceled.
	 *                             If there was a timeout without this
	 *                             flag has been set. There will be no
	 *                             error.
	 *
	 * \return The number of byte have been read by the read
	 *         operation.
	 * \attention This method cannot be used if any other thread is
	 *            consuming the events from the same socket's io_service.
	 *///-------------------------------------
	int readWithTimeout(boost::asio::ip::tcp::socket& s,
	                    const boost::asio::mutable_buffers_1& buffers,
	                    const boost::asio::deadline_timer::duration_type& expiryTime,
	                    boost::system::error_code& readError);

	//========================================
	/*!\brief Handler used by readWithTimeout.
	 * \param[in] error  Error code of the deadline
	 *                   timer wait operation.
	 *///-------------------------------------
	void waitHandler(const boost::system::error_code& error) { m_timerResult.reset(error); }

	//========================================
	/*!\brief Handler used by readWithTimeout.
	 * \param[in] error          Error code of the read
	 *                           operation.
	 * \param[in] nbOfBytesRead  The number of bytes
	 *                           have been read by the
	 *                           read operation.
	 *///-------------------------------------
	void asynReadSomeHandler(const boost::system::error_code& error, const size_t nbOfBytesRead)
	{
		m_readResult.reset(error);
		m_nbOfBytesRead = nbOfBytesRead;
	}

protected:
	//========================================
	/*!\brief Pointer to socket from which messages data will be received.
	 *///-------------------------------------
	boost::asio::ip::tcp::socket* m_socket;

	//========================================
	/*!\brief Deadline timer used by readWithTimeout.
	 *///-------------------------------------
	boost::asio::deadline_timer m_timer;

	//========================================
	/*!\brief Optional error code used by readWithTimeout for the timer result.
	 *///-------------------------------------
	boost::optional<boost::system::error_code> m_timerResult;

	//========================================
	/*!\brief Optional error code used by readWithTimeout for the read result.
	 *///-------------------------------------
	boost::optional<boost::system::error_code> m_readResult;

	//========================================
	/*!\brief Variable to store the number of bytes
	 *        that has been received in readWithTimeout.
	 *///-------------------------------------
	size_t m_nbOfBytesRead;

	//========================================
	/*!\brief Flag to tell, whether a read operation shall
	 *        issue an error if there was a timeout.
	 *
	 * \c true if the user set a timeout duration.
	 * \c false if there is no external timeout duration set,
	 * then the deadline timer is used to get back from the
	 * io_service::run_one method so this thread is not blocked
	 * any longer and can be killed from outside.
	 *///-------------------------------------
	bool m_stopOnTimeOut;
}; // MsgBuffer

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_MSGBUFFER_HPP_SEEN

//======================================================================

