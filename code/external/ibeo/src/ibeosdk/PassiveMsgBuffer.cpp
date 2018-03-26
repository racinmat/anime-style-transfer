//======================================================================
/*! \file PassiveMsgBuffer.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 12, 2016
 *///-------------------------------------------------------------------

#include <ibeosdk/PassiveMsgBuffer.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/inttypes.hpp>

#include <boost/array.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/bind.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

PassiveMsgBuffer::PassiveMsgBuffer(const int bufSize)
  : MsgBufferBase(bufSize)
{}

//======================================================================

PassiveMsgBuffer::~PassiveMsgBuffer() {}

//======================================================================

bool PassiveMsgBuffer::getMessage(const IbeoDataHeader*& recvDataHeader,
                                  const char*& startOfMsgBody,
                                  const int nbOfBytesReceived)
{
	increaseInsPos(nbOfBytesReceived);
	const bool receivedDataBlock = processBuffer(recvDataHeader, startOfMsgBody, nbOfBytesReceived);
	bufferCleanup();
	return receivedDataBlock;
}

//======================================================================

bool PassiveMsgBuffer::increaseInsPos(const int nbOfBytesReceived)
{
	this->m_insAt += nbOfBytesReceived;

	if (this->m_insAt > this->m_bufCapacity) {
		this->m_insAt = -1;
		return false;
	}
	return true;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
