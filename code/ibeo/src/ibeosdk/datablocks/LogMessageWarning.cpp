//======================================================================
/*! \file LogMessageWarning.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 29, 2014
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/LogMessageWarning.hpp>

#include <ibeosdk/DataBlockBufferProvider.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<LogMessageWarning>::dataBlockId = DataTypeId(DataTypeId::DataType_LogWarning);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<LogMessageWarning>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<LogMessageWarning>::dataBlockId, ibeosdk::RegisteredDataBlock<LogMessageWarning>::create);

class IdcFile;
class IbeoEcu;
class IbeoLux;
class IbeoTrackingBox;
class IbeoScala;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<LogMessageWarning>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoLux>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoTrackingBox>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(registerIdInitial)
		))));
//======================================================================

}// namespace ibeosdk

//======================================================================
