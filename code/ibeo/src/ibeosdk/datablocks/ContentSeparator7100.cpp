//======================================================================
/*! \file ContentSeparator7100.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 4, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/ContentSeparator7100.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ContentSeparator>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<ContentSeparator7100>::dataBlockId = DataTypeId(DataTypeId::DataType_ContentSeparator7100);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ContentSeparator7100>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<ContentSeparator7100>::dataBlockId, ibeosdk::RegisteredDataBlock<ContentSeparator7100>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ContentSeparator7100>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================
//======================================================================
//======================================================================

template<>
void writeBE<ContentSeparator7100::ContentSeparatorType>(std::ostream& os, const ContentSeparator7100::ContentSeparatorType& cst)
{
	writeBE(os, uint16_t(cst));
}

//======================================================================

template<>
void readBE<ContentSeparator7100::ContentSeparatorType>(std::istream& is, ContentSeparator7100::ContentSeparatorType& cst)
{
	uint16_t rd16;
	readBE(is, rd16);
	cst = ContentSeparator7100::ContentSeparatorType(rd16);
}

//======================================================================

DataTypeId ContentSeparator7100::getDataType() const
{
	return dataBlockId;
}

//======================================================================

std::streamsize ContentSeparator7100::getSerializedSize() const
{
	return std::streamsize(sizeof(uint16_t) + sizeof(uint32_t));
}

//======================================================================

bool ContentSeparator7100::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	readBE(is, m_separatorType);
	readBE(is, m_sizeOfContent);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool ContentSeparator7100::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	writeBE(os, m_separatorType);
	writeBE(os, m_sizeOfContent);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
