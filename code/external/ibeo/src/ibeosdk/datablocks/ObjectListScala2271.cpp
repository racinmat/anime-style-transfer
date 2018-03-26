//======================================================================
/*! \file ObjectListScala2271.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 23, 2014
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/ObjectListScala2271.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ObjectListScala>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<ObjectListScala2271>::dataBlockId = DataTypeId(DataTypeId::DataType_ScalaObjectList2271);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectListScala2271>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<ObjectListScala2271>::dataBlockId, ibeosdk::RegisteredDataBlock<ObjectListScala2271>::create);

class IdcFile;
class IbeoScala;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectListScala2271>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

ObjectListScala2271::ObjectListScala2271()
  : m_scanStartTimestamp(),
    m_scanNumber(0),
    m_objectListId(0),
    m_deviceId(0),
    m_deviceInterfaceVersion(0),
    m_reserved(0),
    m_objects()
{}

//======================================================================

ObjectListScala2271::ObjectListScala2271(const NTPTime scanStartTimeStamp)
  : m_scanStartTimestamp(scanStartTimeStamp),
    m_scanNumber(0),
    m_objectListId(0),
    m_deviceId(0),
    m_deviceInterfaceVersion(0),
    m_reserved(0),
    m_objects()
{}

//======================================================================

ObjectListScala2271::~ObjectListScala2271() {}

//======================================================================

std::streamsize ObjectListScala2271::getSerializedSize() const
{
	std::streamsize sz = std::streamsize(sizeof(NTPTime))
	                   + 3*std::streamsize(sizeof(uint16_t))
	                   + 2* std::streamsize(sizeof(uint8_t))
	                   + std::streamsize(sizeof(uint32_t));

	std::vector<ObjectScala2271>::const_iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		sz += objIter->getSerializedSize();
	}
	return sz;
}

//======================================================================

DataTypeId ObjectListScala2271::getDataType() const { return dataBlockId; }

//======================================================================

bool ObjectListScala2271::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	readBE(is, m_scanStartTimestamp);
	readBE(is, m_scanNumber);
	readBE(is, m_objectListId);
	readBE(is, m_deviceId);
	readBE(is, m_deviceInterfaceVersion);
	readBE(is, m_reserved);
	{
		uint16_t nbOfObjects;
		readBE(is, nbOfObjects);
		m_objects.resize(nbOfObjects);
	}

	std::vector<ObjectScala2271>::iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		if (!objIter->deserialize(is))
			return false;
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool ObjectListScala2271::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	writeBE(os, m_scanStartTimestamp);
	writeBE(os, m_scanNumber);
	writeBE(os, m_objectListId);
	writeBE(os, m_deviceId);
	writeBE(os, m_deviceInterfaceVersion);
	writeBE(os, m_reserved);

	const uint16_t nbOfObjects = uint16_t(m_objects.size());
	writeBE(os, nbOfObjects);

	std::vector<ObjectScala2271>::const_iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		objIter->serialize(os);
	}
	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================
//======================================================================
//======================================================================

bool operator==(const ObjectListScala2271& lhs, const ObjectListScala2271& rhs)
{
	if (lhs.getScanStartTimestamp() != rhs.getScanStartTimestamp()) return false;
	if (lhs.getScanNumber() != rhs.getScanNumber()) return false;
	if (lhs.getObjectListId() != rhs.getObjectListId()) return false;
	if (lhs.getDeviceId() != rhs.getDeviceId()) return false;
	if (lhs.getDeviceInterfaceVersion() != rhs.getDeviceInterfaceVersion()) return false;
	if (lhs.getReserved() != rhs.getReserved()) return false;

	return (lhs.getObjects() == rhs.getObjects());
}

//======================================================================

bool operator!=(const ObjectListScala2271& lhs, const ObjectListScala2271& rhs)
{
	return !(lhs == rhs);
}

//======================================================================

}// namespace ibeosdk

//======================================================================
