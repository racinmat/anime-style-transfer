//======================================================================
/*! \file IdcTrailer.cpp
 *\verbatim
 * ------------------------------------------
 *  (C) 2015 Ibeo Automotive Systems GmbH, Hamburg, Germany
 * ------------------------------------------
 *
 *  Created on: Jul 2, 2015
 *          by: Jan Christian Dittmer
 *\endverbatim
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/IdcTrailer.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ObjectListScala>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<IdcTrailer>::dataBlockId = DataTypeId(DataTypeId::DataType_IdcTrailer);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<IdcTrailer>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<IdcTrailer>::dataBlockId, ibeosdk::RegisteredDataBlock<IdcTrailer>::create);

class IdcFile;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<IdcTrailer>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(registerIdInitial)
		;

//======================================================================

IdcTrailer::IdcTrailer() {}

//======================================================================

IdcTrailer::~IdcTrailer() {}

//======================================================================

std::streamsize IdcTrailer::getSerializedSize() const { return 0; }

//======================================================================

DataTypeId IdcTrailer::getDataType() const { return dataBlockId; }

//======================================================================

bool IdcTrailer::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	return !is.fail() && (0 == dh.getMessageSize());
}
//======================================================================

bool IdcTrailer::serialize(std::ostream& os) const
{
	return !os.fail();
}
//======================================================================

} // namespace ibeosdk

//======================================================================
