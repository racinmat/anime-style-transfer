//======================================================================
/*! \file ObjectListLux.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/ObjectListLux.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ObjectListLux>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<ObjectListLux>::dataBlockId = DataTypeId(DataTypeId::DataType_LuxObjectList);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectListLux>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<ObjectListLux>::dataBlockId, ibeosdk::RegisteredDataBlock<ObjectListLux>::create);

class IdcFile;
class IbeoLux;
class IbeoTrackingBox;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectListLux>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoLux>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoTrackingBox>::getInstance().registerDataBlock(registerIdInitial)
		));

//======================================================================

ObjectListLux::ObjectListLux()
  : m_scanStartTimestamp(0),
    m_objects()
{
	m_objects.reserve(nbOfObjsToBeReserveInVector);
}

//======================================================================

ObjectListLux::ObjectListLux(const NTPTime scanStartTimeStamp)
  : m_scanStartTimestamp(scanStartTimeStamp),
    m_objects()
{
	m_objects.reserve(nbOfObjsToBeReserveInVector);
}

//======================================================================

ObjectListLux::~ObjectListLux() {}

//======================================================================

void ObjectListLux::addObject(const ObjectLux& newObj)
{
	this->m_objects.push_back(newObj);
}

//======================================================================

const ObjectLux* ObjectListLux::findObject(const UINT16 id) const
{
	std::vector<ObjectLux>::const_iterator oIter = this->m_objects.begin();
	for (; oIter != this->m_objects.end(); ++oIter) {
		if (oIter->getObjectId() == id) {
			return &(*oIter);
		}
	}
	return NULL;
}

//======================================================================

bool ObjectListLux::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readLE(is, m_scanStartTimestamp);

	{
		UINT16 nbOfObjects = 0;
		ibeosdk::readLE(is, nbOfObjects);

		m_objects.resize(nbOfObjects);
	}

	std::vector<ObjectLux>::iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		objIter->deserialize(is);
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool ObjectListLux::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeLE(os, m_scanStartTimestamp);
	const UINT16 nbOfObjects = UINT16(this->m_objects.size());
	ibeosdk::writeLE(os, nbOfObjects);

	std::vector<ObjectLux>::const_iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		objIter->serialize(os);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::streamsize ObjectListLux::getSerializedSize() const
{
	std::streamsize sz = std::streamsize(sizeof(UINT16) + sizeof(NTPTime));
	std::vector<ObjectLux>::const_iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		sz += objIter->getSerializedSize();
	}
	return sz;
}

//======================================================================

DataTypeId ObjectListLux::getDataType() const { return dataBlockId; }

//======================================================================

std::string getObjectIdsAsString(const ObjectListLux& objList)
{
	if (objList.getNumberOfObjects() == 0)
		return "";

	// at least 1 object is in the list
	std::stringstream ssList;

	std::vector<ObjectLux>::const_iterator dObjIter = objList.getObjects().begin();
	ssList << dObjIter->getObjectId();
	++dObjIter;
	for (; dObjIter != objList.getObjects().end(); ++dObjIter) {
		ssList << ", " << dObjIter->getObjectId();
	}

	return ssList.str();
}

//======================================================================

} // namespace ibeosdk

//======================================================================

