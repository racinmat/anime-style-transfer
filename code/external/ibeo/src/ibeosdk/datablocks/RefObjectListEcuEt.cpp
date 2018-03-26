//======================================================================
/*! \file ObjectListEcuEt.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 5, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/RefObjectListEcuEt.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ObjectListEcuEt>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<RefObjectListEcuEt>::dataBlockId = DataTypeId(DataTypeId::DataType_ReferencesObjListET);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<RefObjectListEcuEt>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<RefObjectListEcuEt>::dataBlockId, ibeosdk::RegisteredDataBlock<RefObjectListEcuEt>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<RefObjectListEcuEt>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

RefObjectListEcuEt::RefObjectListEcuEt()
  : m_timestamp(),
    m_objects()
{}

//======================================================================

std::streamsize RefObjectListEcuEt::getSerializedSize() const
{
	std::streamsize sz = std::streamsize(sizeof(NTPTime)) + std::streamsize(sizeof(uint16_t));

	std::vector<ObjectEcuEt>::const_iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		sz += objIter->getSerializedSize();
	}

	return sz;
}

//======================================================================

DataTypeId RefObjectListEcuEt::getDataType() const { return dataBlockId; }

//======================================================================

bool RefObjectListEcuEt::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	readBE(is, m_timestamp);
	uint16_t nbOfObjects = 0;
	readBE(is, nbOfObjects);
	m_objects.resize(nbOfObjects);

	std::vector<ObjectEcuEt>::iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		objIter->deserialize(is);
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool RefObjectListEcuEt::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	writeBE(os, m_timestamp);
	writeBE(os, uint16_t(m_objects.size()));

	std::vector<ObjectEcuEt>::const_iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		objIter->serialize(os);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
