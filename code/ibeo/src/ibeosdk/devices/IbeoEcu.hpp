//======================================================================
/*! \file IbeoEcu.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 24, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_IBEOECU_HPP_SEEN
#define IBEOSDK_IBEOECU_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/devices/IbeoDevice.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class IbeoEcu : public IbeoDevice<IbeoEcu> {
public:
	//========================================
	/*!\brief Create an IbeoEcu (connection class).
	 *
	 * This constructor will create an IbeoEcu class object
	 * which will try to connect to an ECU,
	 * using the given IP address and port number.
	 *
	 * \param[in] ip    IP address of the ECU
	 *                  to be connected with.
	 * \param[in] port  Port number for the connection
	 *                  with the scanner.
	 *///-------------------------------------
	IbeoEcu(const std::string& ip, const unsigned short port = 12002);

	//========================================
	/*!\brief Destructor.
	 *
	 * Will disconnect before destruction.
	 *///-------------------------------------
	virtual ~IbeoEcu();

public:
	//========================================
	/*!\brief Establish the connection to the hardware.
	 *
	 * Reimplements IbeoDevice::getConnected. In
	 * addition it will send a setFilter command
	 * to the ECU to make all messages passes its
	 * output filter.
	 *///-------------------------------------
	virtual void getConnected();

public:
	//========================================
	/*!\brief Send a command which expects no reply.
	 * \param[in] cmd  Command to be sent.
	 * \return The result of the operation.
	 *///-------------------------------------
	virtual statuscodes::Codes sendCommand(const EcuCommandBase& cmd);

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
	virtual statuscodes::Codes sendCommand(const EcuCommandBase& cmd,
	                                       EcuCommandReplyBase& reply,
	                                       const boost::posix_time::time_duration timeOut = boost::posix_time::milliseconds(500));
}; // IbeoEcu

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IBEOECU_HPP_SEEN

//======================================================================

