//======================================================================
/*! \file ObjectListScala.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 17, 2014
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/ObjectListScala.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ObjectListScala>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<ObjectListScala>::dataBlockId = DataTypeId(DataTypeId::DataType_ScalaObjectList);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectListScala>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<ObjectListScala>::dataBlockId, ibeosdk::RegisteredDataBlock<ObjectListScala>::create);

class IdcFile;
class IbeoScala;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectListScala>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

ObjectListScala::ObjectListScala()
  : m_scanStartTimestamp(0),
    m_objects()
{}

//======================================================================

ObjectListScala::ObjectListScala(const NTPTime scanStartTimeStamp)
  : m_scanStartTimestamp(scanStartTimeStamp),
    m_objects()
{}

//======================================================================

ObjectListScala::~ObjectListScala() {}

//======================================================================

void ObjectListScala::addObject(const ObjectScala& newObj)
{
	this->m_objects.push_back(newObj);
}

//======================================================================

const ObjectScala* ObjectListScala::findObject(const UINT16 id) const
{
	std::vector<ObjectScala>::const_iterator oIter = this->m_objects.begin();
	for (; oIter != this->m_objects.end(); ++oIter) {
		if (oIter->getObjectId() == id) {
			return &(*oIter);
		}
	}
	return NULL;
}

//======================================================================

bool ObjectListScala::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readLE(is, m_scanStartTimestamp);
	ibeosdk::readLE(is, m_scanNumber);

	{
		UINT16 nbOfObjects = 0;
		ibeosdk::readLE(is, nbOfObjects);

		m_objects.resize(nbOfObjects);
	}

	std::vector<ObjectScala>::iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		objIter->deserialize(is);
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool ObjectListScala::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeLE(os, m_scanStartTimestamp);
	ibeosdk::writeLE(os, m_scanNumber);

	const UINT16 nbOfObjects = UINT16(this->m_objects.size());
	ibeosdk::writeLE(os, nbOfObjects);

	std::vector<ObjectScala>::const_iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		objIter->serialize(os);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::streamsize ObjectListScala::getSerializedSize() const
{
	std::streamsize sz = 2 * std::streamsize(sizeof(UINT16)) + std::streamsize(sizeof(NTPTime));
	std::vector<ObjectScala>::const_iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		sz += objIter->getSerializedSize();
	}
	return sz;
}

//======================================================================

DataTypeId ObjectListScala::getDataType() const { return dataBlockId; }

//======================================================================

std::string getObjectIdsAsString(const ObjectListScala& objList)
{
	if (objList.getNumberOfObjects() == 0)
		return "";

	// at least 1 object is in the list
	std::stringstream ssList;

	std::vector<ObjectScala>::const_iterator dObjIter = objList.getObjects().begin();
	ssList << dObjIter->getObjectId();
	++dObjIter;
	for (; dObjIter != objList.getObjects().end(); ++dObjIter) {
		ssList << ", " << dObjIter->getObjectId();
	}

	return ssList.str();
}

//======================================================================

}// namespace ibeosdk

//======================================================================
