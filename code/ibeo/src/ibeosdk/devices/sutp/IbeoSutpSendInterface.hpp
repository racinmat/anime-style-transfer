//======================================================================
/*! \file IbeoSutpSendInterface.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date May 3, 2017
 *///-------------------------------------------------------------------

#ifndef SRC_IBEOSDK_DEVICES_SUTP_IBEOSUTPSENDINTERFACE_HPP_SEEN
#define SRC_IBEOSDK_DEVICES_SUTP_IBEOSUTPSENDINTERFACE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>

#include <boost/function.hpp>
#include <boost/asio.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class IbeoSutpSendInterface {
public:
	typedef boost::function<bool(boost::asio::streambuf& outBuff)> SendingFunction;

public:
	IbeoSutpSendInterface(SendingFunction sender);
	virtual ~IbeoSutpSendInterface();

public:
	void send(const IbeoDataHeader& dh, const char* const buffer);
//	void send(const IbeoDataHeader& dh, std::istream& is);

protected:
	void serializeSutpHeader(std::ostream& os,
	                         const uint16_t fragmentId, const uint16_t nbOfFragments,
	                         const DataTypeId dtId);

protected:
	static const uint64_t timeOutDurationMs;
	static const int64_t fragmentSize;

protected:
	SendingFunction m_sending;

	//! sequenceNumber counter for sending SUTP packages
	uint16_t m_segNo;

	//! id of the device that owns this connection
	uint8_t m_deviceId;

	uint32_t m_nbOfSendPackages;

}; // IbeoSutpSendInterface

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // SRC_IBEOSDK_DEVICES_SUTP_IBEOSUTPSENDINTERFACE_HPP_SEEN

//======================================================================
