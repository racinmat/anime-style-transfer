//======================================================================
/*! \file PositionWgs84_2604.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 24, 2014
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/PositionWgs84_2604.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<PositionWgs84_2604>
//======================================================================
template<>
const DataTypeId ibeosdk::RegisteredDataBlock<PositionWgs84_2604>::dataBlockId = DataTypeId(DataTypeId::DataType_PositionWgs84);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<PositionWgs84_2604>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<PositionWgs84_2604>::dataBlockId, ibeosdk::RegisteredDataBlock<PositionWgs84_2604>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<PositionWgs84_2604>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

PositionWgs84_2604::PositionWgs84_2604() : m_positionWgs84() {}

//======================================================================

PositionWgs84_2604::~PositionWgs84_2604() {}

//======================================================================

std::streamsize PositionWgs84_2604::getSerializedSize() const
{
	return m_positionWgs84.getSerializedSize();
}

//======================================================================

DataTypeId PositionWgs84_2604::getDataType() const { return dataBlockId; }

//======================================================================

bool PositionWgs84_2604::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	m_positionWgs84.deserialize(is);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool PositionWgs84_2604::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	m_positionWgs84.serialize(os);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
