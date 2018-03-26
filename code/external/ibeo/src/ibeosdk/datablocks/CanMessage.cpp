//======================================================================
/*! \file CanMessage.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 27, 2014
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/CanMessage.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<CanMessage>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<CanMessage>::dataBlockId = DataTypeId(DataTypeId::DataType_CanMessage);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<CanMessage>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<CanMessage>::dataBlockId, ibeosdk::RegisteredDataBlock<CanMessage>::create);

class IdcFile;
class IbeoEcu;
class IbeoTrackingBox;
class IbeoLux;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<CanMessage>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoTrackingBox>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoLux>::getInstance().registerDataBlock(registerIdInitial)
		)));

//======================================================================
//======================================================================
//======================================================================

template<>
inline
void readBE<CanMessage::MsgType>(std::istream& is, CanMessage::MsgType& value)
{
	uint8_t tmp;
	readBE(is, tmp);
	value = CanMessage::MsgType(tmp);
}

//======================================================================

template<>
inline
void writeBE<CanMessage::MsgType>(std::ostream& os, const CanMessage::MsgType& value)
{
	const uint8_t tmp = value;
	writeBE(os, tmp);
}


//======================================================================

CanMessage::CanMessage()
  : m_version(0),
    m_length(0),
    m_msgType(MsgTypeStandard),
    m_canId(0),
    m_usSinceStartup(0),
    m_timestamp(0),
    m_deviceId(0)
{}

//======================================================================

CanMessage::~CanMessage() {}

//======================================================================

std::streamsize CanMessage::getSerializedSize() const
{
	return
			+ 1
			+ m_length
			+ 1
			+ (this->hasExtendedCanId() ? 4 : 2)
			+ (this->hasValidTimestamp() ? 12 : 0)
			+ 1;
}

//======================================================================

DataTypeId CanMessage::getDataType() const { return dataBlockId; }

//======================================================================

bool CanMessage::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	uint8_t lenVer;
	ibeosdk::readBE(is, lenVer);
	m_version = lenVer & 0x0F;
	m_length = (lenVer >> 4) & 0x0F;

	for (int i = 0; i < this->m_length; ++i)
		ibeosdk::readBE(is, this->m_data[i]);

	ibeosdk::readBE(is, this->m_msgType);

	bool hasRecvTimestamp = false;

	if (this->hasExtendedCanId()) {
		ibeosdk::readBE(is, this->m_canId);
		hasRecvTimestamp = (m_canId & extTsBitMask) == extTsBitMask;
		m_canId &= ~extTsBitMask;
	}
	else {
		uint16_t canId;
		ibeosdk::readBE(is, canId);
		this->m_canId = canId;
		hasRecvTimestamp = (this->m_canId & stdTsBitMask) == stdTsBitMask;
		m_canId &= ~stdTsBitMask;
	}

	if (hasRecvTimestamp) {
		ibeosdk::readBE(is, this->m_usSinceStartup);
		ibeosdk::readBE(is, this->m_timestamp);
	}
	else {
		this->m_usSinceStartup = 0;
		this->m_timestamp.setInvalid();
	}

	ibeosdk::readBE(is, this->m_deviceId);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool CanMessage::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	const uint8_t lenVer = uint8_t(this->m_length << 4) | this->m_version;
	ibeosdk::writeBE(os, lenVer);
	for (int i = 0; i < this->m_length; ++i)
		ibeosdk::writeBE(os, this->m_data[i]);
	ibeosdk::writeBE(os, this->m_msgType);

	const bool hasValidTs = this->hasValidTimestamp();

	if (this->hasExtendedCanId()) {
		const uint32_t canId = this->m_canId | (hasValidTs ? extTsBitMask : 0);
		ibeosdk::writeBE(os, canId);
	}
	else {
		const uint16_t canId = uint16_t(this->m_canId | (hasValidTs ? stdTsBitMask : 0));
		ibeosdk::writeBE(os, canId);
	}

	if (hasValidTs) {
		ibeosdk::writeBE(os, this->m_usSinceStartup);
		ibeosdk::writeBE(os, this->m_timestamp);
	}

	ibeosdk::writeBE(os, this->m_deviceId);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
