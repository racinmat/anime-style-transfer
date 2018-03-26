//======================================================================
/*! \file RegisteredDataBlock.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 29, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_REGISTEREDDATABLOCK_HPP_SEEN
#define IBEOSDK_REGISTEREDDATABLOCK_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/datablocks/DataBlock.hpp>
#include <ibeosdk/listener/DataListener.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

template<class DataBlockImpl>
class RegisteredDataBlock : public DataBlock {
public:
	RegisteredDataBlock()
	  : DataBlock()
	{}

	virtual ~RegisteredDataBlock() {}

public:
	static DataBlockImpl* create() { return new DataBlockImpl; }
	static DataTypeId getDataBlockId() { return dataBlockId; }

public:
	virtual bool callListener(DataListenerBase& l) const
	{
		if (DataListener<DataBlockImpl>* lImpl = dynamic_cast<DataListener<DataBlockImpl>*>(&l)) {
			lImpl->onData(dynamic_cast<const DataBlockImpl* const>(this));
			return true;
		}
		return false;
	}

protected:
	static const DataTypeId dataBlockId;

private:
	static const DataBlockRegisterId registerIdInitial;
	static const DataBlockRegisterId registerId;
}; // RegisteredDataBlock

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_REGISTEREDDATABLOCK_HPP_SEEN

//======================================================================
