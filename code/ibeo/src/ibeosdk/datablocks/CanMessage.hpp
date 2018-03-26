//======================================================================
/*! \file CanMessage.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 27, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_CANMESSAGE_HPP_SEEN
#define IBEOSDK_CANMESSAGE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class CanMessage : public ibeosdk::RegisteredDataBlock<CanMessage> {
public:
	//========================================
	/*! \brief CAN message types
	 *
	 * \note These message types have been extracted from PCAN header files.
	 *///-------------------------------------
	enum MsgType {
		MsgTypeStandard = 0x00, ///< Standard data frame (11-bit ID)
		MsgTypeRTR      = 0x01, ///< Remote request frame
		MsgTypeExtended = 0x02, ///< Extended data frame (CAN 2.0B, 29-bit ID)
		MsgTypeErrFrame = 0x40, ///< Error frame
		MsgTypeStatus   = 0x80  ///< Status information
	};

	//========================================
	/*!\brief Enumeration of byte numbers 0 to 7
	 *///-------------------------------------
	enum ByteNumber {
		Byte0, Byte1, Byte2, Byte3, Byte4, Byte5, Byte6, Byte7
	};

public:
	CanMessage();
	virtual ~CanMessage();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	bool hasExtendedCanId() const { return (this->m_msgType & MsgTypeExtended) == MsgTypeExtended; }
	bool hasValidTimestamp() const { return !this->m_timestamp.toPtime().is_not_a_date_time(); }

public: // getter
	uint8_t getVersion() const { return m_version; }
	uint8_t getLength() const { return m_length; }
	uint8_t getData(const ByteNumber byte) const { return m_data[byte]; }
	MsgType getMsgType() const { return m_msgType; }
	uint32_t getCanId() const { return m_canId; }
	uint32_t getUsSinceStartup() const { return m_usSinceStartup; }
	NTPTime getTimestamp() const { return m_timestamp; }
	uint8_t getDeviceId() const { return m_deviceId; }

public: // setter
	void setVersion(const uint8_t newVersion) { m_version = newVersion; }
	void setLength(const uint8_t newLength) { m_length = newLength; }
	void setData(const ByteNumber byte, const uint8_t newData) { m_data[byte] = newData; }
	void setMsgType(const MsgType newMsgType) { m_msgType = newMsgType; }
	void setCanId(const uint32_t newCanId) { m_canId = newCanId; }
	void setUsSinceStartup(const uint32_t newUsSinceStartup) { m_usSinceStartup = newUsSinceStartup; }
	void setTimestamp(const NTPTime newTimestamp) { m_timestamp = newTimestamp; }
	void setDeviceId(const uint8_t newDeviceId) { m_deviceId = newDeviceId; }

protected:
	static const int maxVersion = 15;
	static const int maxMsgSize = 8;
	static const unsigned int maxStdId = 0x7ff;
	static const unsigned int maxExtId = 0x1fffffff; ///< Maximum extended CAN identifier
	static const unsigned int stdTsBitMask = 0x00008000;
	static const unsigned int extTsBitMask = 0x80000000;

protected:
	uint8_t m_version; // 4 bits
	uint8_t m_length;  // 4 bits

	uint8_t m_data[maxMsgSize];
	MsgType m_msgType;
	uint32_t m_canId; // serialized as 2 or 4 bytes

	uint32_t m_usSinceStartup; //!< Microseconds since device startup.
	NTPTime m_timestamp;

	uint8_t m_deviceId;        //!< id of device
}; // CanMessage

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_CANMESSAGE_HPP_SEEN

//======================================================================
