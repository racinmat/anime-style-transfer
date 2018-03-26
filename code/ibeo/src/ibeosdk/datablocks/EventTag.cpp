//======================================================================
/*! \file EventTag.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Mar 18, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/EventTag.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

#include <cstring>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<EventTag>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<EventTag>::dataBlockId = DataTypeId(DataTypeId::DataType_EventTag);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<EventTag>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<EventTag>::dataBlockId, ibeosdk::RegisteredDataBlock<EventTag>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<EventTag>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================
//======================================================================
//======================================================================


//======================================================================

template<>
void writeBE<EventTag::TagClass>(std::ostream& os, const EventTag::TagClass& tc)
{
	writeBE(os, uint16_t(tc));
}

//======================================================================

template<>
void readBE<EventTag::TagClass>(std::istream& is, EventTag::TagClass& tc)
{
	uint16_t rd16;
	readBE(is, rd16);
	tc = EventTag::TagClass(rd16);
}

//======================================================================

EventTag::EventTag()
  : RegisteredDataBlock<EventTag>(),
    m_tagStart(),
    m_tagEnd(),
    m_flags(0),
    m_tagClass(Unknown),
    m_tagId(0),
    m_tagString(),
    m_wgs84(),
    m_roiWidth(0),
    m_roiLength(0)
    //m_reserved
{
	memset(m_reserved, 0xff, nbOfReserved);
}

//======================================================================

EventTag::~EventTag() {}

//======================================================================

DataTypeId EventTag::getDataType() const { return dataBlockId; }

//======================================================================

std::streamsize EventTag::getSerializedSize() const
{
	return 8 + 8 + 1 + 2 + 4 +
			1 + std::streamsize(m_tagString.size()) +
			PositionWgs84::getSerializedSize_static() +
			8 + 8 + nbOfReserved;
}

//======================================================================

bool EventTag::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readBE(is, this->m_tagStart);
	ibeosdk::readBE(is, this->m_tagEnd);
	ibeosdk::readBE(is, this->m_flags);
	ibeosdk::readBE(is, this->m_tagClass);
	ibeosdk::readBE(is, this->m_tagId);
	{
		uint8_t strLength;
		ibeosdk::readBE(is, strLength);
		this->m_tagString.resize(strLength);
		is.read(&this->m_tagString[0], strLength); // assuming contiguous storage for std::string.
	}
	this->m_wgs84.deserialize(is);
	ibeosdk::readBE(is, this->m_roiWidth);
	ibeosdk::readBE(is, this->m_roiLength);

	is.read(&this->m_reserved[0], nbOfReserved);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool EventTag::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, m_tagStart);
	ibeosdk::writeBE(os, m_tagEnd);
	ibeosdk::writeBE(os, m_flags);
	ibeosdk::writeBE(os, m_tagClass);
	ibeosdk::writeBE(os, this->m_tagId);
	{
		const uint8_t strLength = uint8_t(this->m_tagString.size());
		ibeosdk::writeBE(os, strLength);
		os.write(&this->m_tagString[0], strLength); // assuming contiguous storage for std::string.
	}
	this->m_wgs84.serialize(os);
	ibeosdk::writeBE(os, this->m_roiWidth);
	ibeosdk::writeBE(os, this->m_roiLength);

	os.write(&this->m_reserved[0], nbOfReserved);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================

