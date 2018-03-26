//======================================================================
/*! \file LogMessageNote.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 29, 2014
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/LogMessageNote.hpp>

#include <ibeosdk/DataBlockBufferProvider.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<LogMessageNote>::dataBlockId = DataTypeId(DataTypeId::DataType_LogNote);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<LogMessageNote>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<LogMessageNote>::dataBlockId, ibeosdk::RegisteredDataBlock<LogMessageNote>::create);

class IdcFile;
class IbeoEcu;
class IbeoLux;
class IbeoTrackingBox;
class IbeoScala;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<LogMessageNote>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoLux>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoTrackingBox>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(registerIdInitial)
		))));
//======================================================================

}// namespace ibeosdk

//======================================================================
