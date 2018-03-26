//======================================================================
/*! \file LogMessageDebug.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 29, 2014
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/LogMessageDebug.hpp>

#include <ibeosdk/DataBlockBufferProvider.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<LogMessageDebug>::dataBlockId = DataTypeId(DataTypeId::DataType_LogDebug);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<LogMessageDebug>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<LogMessageDebug>::dataBlockId, ibeosdk::RegisteredDataBlock<LogMessageDebug>::create);

class IdcFile;
class IbeoEcu;
class IbeoLux;
class IbeoTrackingBox;
class IbeoScala;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<LogMessageDebug>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoLux>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoTrackingBox>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(registerIdInitial)
		))));

//======================================================================

}// namespace ibeosdk

//======================================================================
