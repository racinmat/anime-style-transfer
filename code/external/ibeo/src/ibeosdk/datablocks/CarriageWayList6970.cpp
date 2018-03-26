//======================================================================
/*! \file CarriageWayList6970.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Feb 24, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/CarriageWayList6970.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<CarriageWayList6970>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<CarriageWayList6970>::dataBlockId = DataTypeId(DataTypeId::DataType_CarriageWayList);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<CarriageWayList6970>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<CarriageWayList6970>::dataBlockId, ibeosdk::RegisteredDataBlock<CarriageWayList6970>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<CarriageWayList6970>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);


//======================================================================

CarriageWayList6970::CarriageWayList6970() {}

//======================================================================

CarriageWayList6970::~CarriageWayList6970() {}

//======================================================================

std::streamsize CarriageWayList6970::getSerializedSize() const
{
	return m_carriageWayList.getSerializedSize();
}

//======================================================================

DataTypeId CarriageWayList6970::getDataType() const { return dataBlockId; }

//======================================================================

bool CarriageWayList6970::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	m_carriageWayList.serialize(os);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool CarriageWayList6970::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	m_carriageWayList.deserialize(is);
	
	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

}// namespace ibeosdk

//======================================================================
