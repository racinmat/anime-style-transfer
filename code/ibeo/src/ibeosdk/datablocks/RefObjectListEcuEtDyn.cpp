//======================================================================
/*! \file RefObjectListEcuEtDyn.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Mar 05, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/RefObjectListEcuEtDyn.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ObjectListEcuEt>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<RefObjectListEcuEtDyn>::dataBlockId = DataTypeId(DataTypeId::DataType_ReferencesObjListETDyn);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<RefObjectListEcuEtDyn>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<RefObjectListEcuEtDyn>::dataBlockId, ibeosdk::RegisteredDataBlock<RefObjectListEcuEtDyn>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<RefObjectListEcuEtDyn>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

RefObjectListEcuEtDyn::RefObjectListEcuEtDyn()
  : m_timestamp(),
    m_objectListId(0),
    m_deviceType(0),
    m_deviceInterfaceVersion(0),
    m_flags(0),
    m_reserved1(0),
    m_objects()
{}

//======================================================================

std::streamsize RefObjectListEcuEtDyn::getSerializedSize() const
{
	std::streamsize sz = std::streamsize(
	                     sizeof(NTPTime)
	                   + sizeof(UINT8)
	                   + sizeof(UINT8)
	                   + sizeof(UINT16)
	                   + sizeof(UINT16)
	                   + sizeof(UINT16));

	std::vector<ObjectEcuEtDyn>::const_iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		sz += objIter->getSerializedSize();
	}

	return sz;
}

//======================================================================

DataTypeId RefObjectListEcuEtDyn::getDataType() const { return dataBlockId; }

//======================================================================

bool RefObjectListEcuEtDyn::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	readBE(is, m_timestamp);
	readBE(is, m_objectListId);
	readBE(is, m_deviceType);
	readBE(is, m_deviceInterfaceVersion);
	readBE(is, m_flags);
	readBE(is, m_reserved1);
	UINT16 nbOfObjects = 0;
	readBE(is, nbOfObjects);

	m_objects.resize(nbOfObjects);

	std::vector<ObjectEcuEtDyn>::iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		objIter->deserialize(is);
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool RefObjectListEcuEtDyn::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	writeBE(os, m_timestamp);
	writeBE(os, m_objectListId);
	writeBE(os, m_deviceType);
	writeBE(os, m_deviceInterfaceVersion);
	writeBE(os, m_flags);
	writeBE(os, m_reserved1);
	writeBE(os, UINT16(m_objects.size()));

	std::vector<ObjectEcuEtDyn>::const_iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		objIter->serialize(os);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
