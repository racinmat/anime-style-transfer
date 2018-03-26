//======================================================================
/*! \file IbeoLuxHr.hpp
 *
 * \copydoc Copyright
 * \author Andreas Mueller (AnM)
 * \date Jul 30, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_IBEOLUXHR_HPP_SEEN
#define IBEOSDK_IBEOLUXHR_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/devices/IbeoDevice.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class IbeoLuxHr
 * \brief Class to connect to a LuxHr sensor.
 * \author Andreas Mueller (AnM)
 * \version 0.1
 * \date Jul 30, 2015
 *
 *///-------------------------------------------------------------------
class IbeoLuxHr : public IbeoDevice<IbeoLuxHr> {
public:
	//========================================
	/*!\brief Create an IbeoLuxHr (connection class).
	 *
	 * This constructor will create an IbeoLuxHr class object
	 * which will try to connect to a LuxHr sensor,
	 * using the given IP address and port number.
	 *
	 * \param[in] ip    IP address of the scanner
	 *                  to be connected with.
	 * \param[in] port  Port number for the connection
	 *                  with the scanner.
	 *///-------------------------------------
	IbeoLuxHr(const std::string& ip, const unsigned short port = 12008);

	//========================================
	/*!\brief Destructor.
	 *
	 * Will disconnect before destruction.
	 *///-------------------------------------
	virtual ~IbeoLuxHr();

//public:
//	//========================================
//	/*!\brief Send a command which expects no reply.
//	 * \param[in] cmd  Command to be sent.
//	 * \return The result of the operation.
//	 *///-------------------------------------
//	virtual statuscodes::Codes sendCommand(const LuxHrCommandBase& cmd);
//
//	//========================================
//	/*!\brief Send a command and wait for a reply.
//	 *
//	 * The command will be sent. The calling thread
//	 * will sleep until a reply has been received
//	 * but not longer than the number of milliseconds
//	 * given in \a timeOut.
//	 *
//	 * \param[in]       cmd    Command to be sent.
//	 * \param[in, out]  reply  The reply container for
//	 *                         the reply to be stored into.
//	 * \param[in]       timeOut  Number of milliseconds to
//	 *                           wait for a reply.
//	 * \return The result of the operation.
//	 *///-------------------------------------
//	virtual statuscodes::Codes sendCommand(const LuxHrCommandBase& cmd,
//	                                       LuxHrCommandReplyBase& reply,
//	                                       const boost::posix_time::time_duration timeOut = boost::posix_time::milliseconds(500));
}; // IbeoLuxHr

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IBEOLUXHR_HPP_SEEN

//======================================================================

