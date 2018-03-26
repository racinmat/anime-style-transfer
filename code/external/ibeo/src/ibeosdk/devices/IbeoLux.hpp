//======================================================================
/*! \file IbeoLux.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 24, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_IBEOLUX_HPP_SEEN
#define IBEOSDK_IBEOLUX_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/devices/IbeoDevice.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class IbeoLux
 * \brief Class to connect to a LUX3/LUX4 sensor.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Oct 1, 2013
 *///-------------------------------------------------------------------
class IbeoLux : public IbeoDevice<IbeoLux> {
public:
	//========================================
	/*!\brief Create an IbeoLux (connection class).
	 *
	 * This constructor will create an IbeoLux class object
	 * which will try to connect to a LUX3/LUX4 sensor,
	 * using the given IP address and port number.
	 *
	 * \param[in] ip    IP address of the scanner
	 *                  to be connected with.
	 * \param[in] port  Port number for the connection
	 *                  with the scanner.
	 *///-------------------------------------
	IbeoLux(const std::string& ip, const unsigned short port = 12002);

	//========================================
	/*!\brief Destructor.
	 *
	 * Will disconnect before destruction.
	 *///-------------------------------------
	virtual ~IbeoLux();
public:
	//========================================
	/*!\brief Send a command which expects no reply.
	 * \param[in] cmd  Command to be sent.
	 * \return The result of the operation.
	 *///-------------------------------------
	virtual statuscodes::Codes sendCommand(const LuxCommandBase& cmd);

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
	virtual statuscodes::Codes sendCommand(const LuxCommandBase& cmd,
	                                       LuxCommandReplyBase& reply,
	                                       const boost::posix_time::time_duration timeOut = boost::posix_time::milliseconds(500));
}; // IbeoLux

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IBEOLUX_HPP_SEEN

//======================================================================

