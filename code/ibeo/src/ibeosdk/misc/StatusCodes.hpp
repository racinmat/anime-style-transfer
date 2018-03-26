//======================================================================
/*! \file StatusCodes.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 14, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_STATUSCODES_HPP_SEEN
#define IBEOSDK_STATUSCODES_HPP_SEEN

//======================================================================

namespace ibeosdk {
namespace statuscodes {

//======================================================================

enum Codes {
	EverythingOk = 0,                //!< No error
	NotConnected,                    //!< Device is not connected.
	MismatchingCommandAndReply,      //!<
	FailedToPrepareSendingCommand,   //!< Error in preparing the send buffer.
	FailedToPrepareSendingDataBlock, //!< Error in preparing the send buffer.
	SendingCommandFailed,            //!< Sending the command has failed.
	SendingDataBlockFailed,          //!< Sending of the data block has failed.
	ReplyMismatch,                   //!< Received wrong command reply. Command ids not matching.
	TimeOut,                         //!< The reply has not received in proper time.
	TimeOutCriticalSection,          //!< Critical section was blocked too long.
	ReceiveCommandErrorReply,        //!< An command error reply has been received.
	DataBlockBlockedByFilter,        //!< Datablock blocked by datatype id filter

	StreamerNotRegistered,
	ListenerNotRegistered
}; // Codes

//======================================================================

} // namespace statuscodes
} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_STATUSCODES_HPP_SEEN

//======================================================================
