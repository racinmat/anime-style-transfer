//======================================================================
/*! \file FrameEndSeparator.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Nov 26, 2013
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/FrameEndSeparator.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<FrameEndSeparator>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<FrameEndSeparator>::dataBlockId = DataTypeId(DataTypeId::DataType_FrameEndSeparator);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<FrameEndSeparator>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<FrameEndSeparator>::dataBlockId, ibeosdk::RegisteredDataBlock<FrameEndSeparator>::create);

class IdcFile;
class IbeoEcu;
class IbeoScala;
class IbeoMiniLux;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<FrameEndSeparator>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoMiniLux>::getInstance().registerDataBlock(registerIdInitial)
		)));

//======================================================================
//======================================================================
//======================================================================

//static
std::streamsize FrameEndSeparator::getSerializedSize_static()
{
	return 32;
}

//======================================================================

FrameEndSeparator::FrameEndSeparator()
  : RegisteredDataBlock<FrameEndSeparator>(),
    m_frameId(0),
    m_sizeOfThisFrame(0),
    m_sizeOfNextFrame(0),
    m_creationTime()
{
	memset(m_reserved, 0, nbOfReserved*sizeof(UINT16));
}

//======================================================================

FrameEndSeparator::~FrameEndSeparator() {}

//======================================================================

DataTypeId FrameEndSeparator::getDataType() const { return dataBlockId; }

//======================================================================

std::streamsize FrameEndSeparator::getSerializedSize() const
{
	return getSerializedSize_static();
}

//======================================================================

bool FrameEndSeparator::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readBE(is, m_frameId);
	ibeosdk::readBE(is, m_sizeOfThisFrame);
	ibeosdk::readBE(is, m_sizeOfNextFrame);
	ibeosdk::readBE(is, m_creationTime);

	for (int r = 0; r < nbOfReserved; ++r)
		ibeosdk::readBE(is, m_reserved[r]);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool FrameEndSeparator::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, m_frameId);
	ibeosdk::writeBE(os, m_sizeOfThisFrame);
	ibeosdk::writeBE(os, m_sizeOfNextFrame);
	ibeosdk::writeBE(os, m_creationTime);

	for (int r = 0; r < nbOfReserved; ++r)
		ibeosdk::writeBE(os, m_reserved[r]);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
