//======================================================================
/*! \file OxtsMessage.cpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Jun 30, 2014
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/OxtsMessage.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<OxtsMessage>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<OxtsMessage>::dataBlockId = DataTypeId(DataTypeId::DataType_OxtsMessage);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<OxtsMessage>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<OxtsMessage>::dataBlockId, ibeosdk::RegisteredDataBlock<OxtsMessage>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<OxtsMessage>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================
//======================================================================
//======================================================================

OxtsMessage::OxtsMessage()
  : m_ipAddress(),
    m_data()
{}

//======================================================================

std::streamsize OxtsMessage::getSerializedSize() const
{
	std::streamsize sz = std::streamsize(
	                     sizeof(UINT32)
	                   + (sizeof(UINT8) * m_data.size()));
	return sz;
}

//======================================================================

DataTypeId OxtsMessage::getDataType() const { return dataBlockId; }

//======================================================================

bool OxtsMessage::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readBE(is, m_ipAddress);

	{
		std::vector<UINT8>::size_type dataSize = dh.getMessageSize() - sizeof(UINT32);

		if (m_data.size() != dataSize)
			m_data.resize(dataSize);
	}

	std::vector<UINT8>::iterator objIter = m_data.begin();
	for (; objIter != m_data.end(); ++objIter) {
		ibeosdk::readBE(is, *objIter);
	}

	unlock();
	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool OxtsMessage::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, m_ipAddress);

	std::vector<UINT8>::const_iterator objIter = m_data.begin();
	for (; objIter != m_data.end(); ++objIter) {
		ibeosdk::writeBE(os, objIter);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
