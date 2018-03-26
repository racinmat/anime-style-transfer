//======================================================================
/*! \file ObjectListEcuEt.cpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Mar 14, 2014
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/ObjectListEcuEtDyn.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

template<>
void readBE<ObjectListEcuEtDyn::DeviceType>(std::istream& is, ObjectListEcuEtDyn::DeviceType& dt)
{
	uint8_t rd8;
	readBE(is, rd8);
	dt = ObjectListEcuEtDyn::DeviceType(rd8);
}

//======================================================================

template<>
void writeBE<ObjectListEcuEtDyn::DeviceType>(std::ostream& os, const ObjectListEcuEtDyn::DeviceType& dt)
{
	writeBE(os, uint8_t(dt));
}

//======================================================================
// Specializations for RegisteredDataBlock<ObjectListEcuEtDyn>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<ObjectListEcuEtDyn>::dataBlockId = DataTypeId(DataTypeId::DataType_EcuObjectListETDyn);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectListEcuEtDyn>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<ObjectListEcuEtDyn>::dataBlockId, ibeosdk::RegisteredDataBlock<ObjectListEcuEtDyn>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectListEcuEtDyn>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

ObjectListEcuEtDyn::ObjectListEcuEtDyn()
  : m_timestamp(),
    m_objectListId(0),
    m_deviceType(DeviceType_Undefined),
    m_deviceInterfaceVersion(0),
    m_flags(0),
    m_reserved1(0),
    m_objects()
{}

//======================================================================

std::streamsize ObjectListEcuEtDyn::getSerializedSize() const
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

DataTypeId ObjectListEcuEtDyn::getDataType() const { return dataBlockId; }

//======================================================================

bool ObjectListEcuEtDyn::deserialize(std::istream& is, const IbeoDataHeader& dh)
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

bool ObjectListEcuEtDyn::serialize(std::ostream& os) const
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

bool ObjectListEcuEtDyn::operator==( const ObjectListEcuEtDyn& other ) const
{
	if (m_timestamp != other.getTimestamp())
		return false;

	if (m_objectListId != other.getObjectListId())
		return false;

	if (m_deviceType != other.getDeviceType())
		return false;

	if (m_deviceInterfaceVersion != other.getDeviceInterfaceVersion())
		return false;

	if (m_flags != other.getFlags())
		return false;

	if (m_reserved1 != other.getReserved1())
		return false;

	if (m_objects.size() != other.getObjects().size())
		return false;

	if (m_objects != other.getObjects())
		return false;
	
	return true;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
