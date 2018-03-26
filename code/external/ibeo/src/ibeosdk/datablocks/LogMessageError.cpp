//======================================================================
/*! \file LogMessageError.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 29, 2014
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/LogMessageError.hpp>

#include <ibeosdk/DataBlockBufferProvider.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<LogMessageError>::dataBlockId = DataTypeId(DataTypeId::DataType_LogError);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<LogMessageError>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<LogMessageError>::dataBlockId, ibeosdk::RegisteredDataBlock<LogMessageError>::create);

class IdcFile;
class IbeoEcu;
class IbeoLux;
class IbeoTrackingBox;
class IbeoScala;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<LogMessageError>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoLux>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoTrackingBox>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(registerIdInitial)
		))));
//======================================================================

}// namespace ibeosdk

//======================================================================
