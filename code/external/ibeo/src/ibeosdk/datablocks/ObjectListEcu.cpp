//======================================================================
/*! \file ObjectListEcu.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/ObjectListEcu.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ObjectListEcu>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<ObjectListEcu>::dataBlockId = DataTypeId(DataTypeId::DataType_EcuObjectList);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectListEcu>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<ObjectListEcu>::dataBlockId, ibeosdk::RegisteredDataBlock<ObjectListEcu>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectListEcu>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================
//======================================================================
//======================================================================

ObjectListEcu::ObjectListEcu()
  : m_timestamp(),
    m_objects()
{
//	m_objects.reserve(maxEcuObjects);
}

//======================================================================

std::streamsize ObjectListEcu::getSerializedSize() const
{
	std::streamsize sz = std::streamsize(sizeof(NTPTime)) + std::streamsize(sizeof(UINT16));

	std::vector<ObjectEcu>::const_iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		sz += objIter->getSerializedSize();
	}
	return sz;
}

//======================================================================

DataTypeId ObjectListEcu::getDataType() const { return dataBlockId; }

//======================================================================

bool ObjectListEcu::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readBE(is, m_timestamp);

	{
		UINT16 nbOfObjects;
		ibeosdk::readBE(is, nbOfObjects);

//		if (nbOfObjects > ObjectListEcu::maxEcuObjects) {
//			logWarning << "Object list with too many objects (" << nbOfObjects << ") received."
//			" List will be ignored." << std::endl;
//			return false;
//		}

		if (m_objects.size() != nbOfObjects)
			m_objects.resize(nbOfObjects);
	}

	std::vector<ObjectEcu>::iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		objIter->deserialize(is);
	}

	unlock();
	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool ObjectListEcu::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, m_timestamp);
	const UINT16 nbOfObjects = UINT16(this->m_objects.size());
	ibeosdk::writeBE(os, nbOfObjects);

	std::vector<ObjectEcu>::const_iterator objIter = m_objects.begin();
	for (; objIter != m_objects.end(); ++objIter) {
		objIter->serialize(os);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::string getObjectIdsAsString(const ObjectListEcu& objList)
{
	if (objList.getNumberOfObjects() == 0)
		return "";

	// at least 1 object is in the list
	std::stringstream ssList;

	std::vector<ObjectEcu>::const_iterator dObjIter = objList.getObjects().begin();
	ssList << dObjIter->getObjectId();
	++dObjIter;
	for (; dObjIter != objList.getObjects().end(); ++dObjIter) {
		ssList << ", " << dObjIter->getObjectId();
	}

	return ssList.str();
}

//======================================================================

bool ObjectListEcu::operator==( const ObjectListEcu& other ) const
{
	if (m_timestamp != other.getTimestamp())
		return false;

	if (m_objects.size() != other.getNumberOfObjects())
		return false;
	
	return (m_objects == other.getObjects());
}


//======================================================================

} // namespace ibeosdk

//======================================================================
