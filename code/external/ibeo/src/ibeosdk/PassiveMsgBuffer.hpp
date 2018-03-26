//======================================================================
/*! \file PassiveMsgBuffer.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 12, 2016
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_PASSIVEMSGBUFFER_HPP_SEEN
#define IBEOSDK_PASSIVEMSGBUFFER_HPP_SEEN

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
 * \date Jul 12, 2016
 *
 *
 *///-------------------------------------------------------------------
class PassiveMsgBuffer : public MsgBufferBase {
public:
	//========================================
	/*!\brief Constructor
	 *
	 * \param[in]       bufSize     Size of the buffer
	 *                              which will be allocated
	 *                              to hold the received
	 *                              message data. This size has to be
	 *                              significantly larger than the largest
	 *                              to be expected message.
	 *///-------------------------------------
	PassiveMsgBuffer(const int bufSize);

	//========================================
	/*!\brief Destructor.
	 *///-------------------------------------
	virtual ~PassiveMsgBuffer();

public:
	bool getMessage(const IbeoDataHeader*& recvDataHeader,
	                const char*& startOfMsgBody,
	                const int nbOfBytesReceived);

protected:
	bool increaseInsPos(const int nbOfBytesReceived);
}; // PassiveMsgBuffer

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_PASSIVEMSGBUFFER_HPP_SEEN

//======================================================================

