//======================================================================
/*! \file IbeoMiniLux.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer
 * \date Oct 04, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_IBEOMINILUX_HPP_SEEN
#define IBEOSDK_IBEOMINILUX_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/devices/IbeoDevice.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class IbeoMiniLux
 * \brief Class to connect to a MiniLux sensor.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Oct 1, 2013
 *
 *///-------------------------------------------------------------------
class IbeoMiniLux : public IbeoDevice<IbeoMiniLux> {
public:
	//========================================
	/*!\brief Create an IbeoMiniLux (connection class).
	 *
	 * This constructor will create an IbeoMiniLux class object
	 * which will try to connect to a MiniLux sensor,
	 * using the given IP address and port number.
	 *
	 * \param[in] ip    IP address of the scanner
	 *                  to be connected with.
	 * \param[in] port  Port number for the connection
	 *                  with the scanner.
	 *///-------------------------------------
	IbeoMiniLux(const std::string& ip, const unsigned short port = 12006);

	//========================================
	/*!\brief Destructor.
	 *
	 * Will disconnect before destruction.
	 *///-------------------------------------
	virtual ~IbeoMiniLux();

public:
	//========================================
	/*!\brief Send a command which expects no reply.
	 * \param[in] cmd  Command to be sent.
	 * \return The result of the operation.
	 *///-------------------------------------
	virtual statuscodes::Codes sendCommand(const MiniLuxCommandBase& cmd);

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
	 *///-------------------------------------
	virtual statuscodes::Codes sendCommand(const MiniLuxCommandBase& cmd,
	                                       MiniLuxCommandReplyBase& reply,
	                                       const boost::posix_time::time_duration timeOut = boost::posix_time::milliseconds(500));
}; // IbeoMiniLux

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IBEOMINILUX_HPP_SEEN

//======================================================================

