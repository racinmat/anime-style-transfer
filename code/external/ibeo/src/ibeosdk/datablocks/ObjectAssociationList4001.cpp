//======================================================================
/*! \file ObjectAssociationList4001.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 26, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/ObjectAssociationList4001.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

#include <new>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ObjectListEcu>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<ObjectAssociationList4001>::dataBlockId = DataTypeId(DataTypeId::DataType_ObjectAssociationList4001);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectAssociationList4001>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<ObjectAssociationList4001>::dataBlockId, ibeosdk::RegisteredDataBlock<ObjectAssociationList4001>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectAssociationList4001>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================
//======================================================================
//======================================================================

ObjectAssociationList4001::ObjectAssociationList4001()
  : m_refObjListId(0xFF),
    m_refDevType(0),
    m_refDevInterfaceVersion(0x3FFF),
    m_dutObjListId(0xFF),
    m_dutDevType(0),
    m_dutDevInterfaceVersion(0x3FFF),
    // m_reserved
    m_objAssocs()
{
	memset(m_reserved, 0xFF, nbOfReserved);
}

//======================================================================

std::streamsize ObjectAssociationList4001::getSerializedSize() const
{
	return std::streamsize(4*sizeof(uint8_t))
	     + std::streamsize(2*sizeof(uint16_t))
	     + std::streamsize(sizeof(uint32_t))
	     + std::streamsize(nbOfReserved)
	     + std::streamsize(m_objAssocs.size()) * ObjectAssociation4001::getSerializedSize_static();
}

//======================================================================

DataTypeId ObjectAssociationList4001::getDataType() const { return dataBlockId; }

//======================================================================

bool ObjectAssociationList4001::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readBE(is, m_refObjListId);
	ibeosdk::readBE(is, m_refDevType);
	ibeosdk::readBE(is, m_refDevInterfaceVersion);
	ibeosdk::readBE(is, m_dutObjListId);
	ibeosdk::readBE(is, m_dutDevType);
	ibeosdk::readBE(is, m_dutDevInterfaceVersion);

	try {
		uint32_t nbOfObjAssocs;
		ibeosdk::readBE(is, nbOfObjAssocs);
		m_objAssocs.resize(nbOfObjAssocs);
	}
	catch (const std::bad_alloc& ba) {
		logError << "bad_alloc caught: " << ba.what() << std::endl;
		unlock();
		return false;
	}

	for (int32_t i = 0; i < nbOfReserved; ++i) {
		ibeosdk::readBE(is, m_reserved[i]);
	}

	ObjAssocVector::iterator oaIter = m_objAssocs.begin();
	for (; oaIter != m_objAssocs.end(); ++oaIter) {
		if (!oaIter->deserialize(is))
			break;
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool ObjectAssociationList4001::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, m_refObjListId);
	ibeosdk::writeBE(os, m_refDevType);
	ibeosdk::writeBE(os, m_refDevInterfaceVersion);
	ibeosdk::writeBE(os, m_dutObjListId);
	ibeosdk::writeBE(os, m_dutDevType);
	ibeosdk::writeBE(os, m_dutDevInterfaceVersion);

	ibeosdk::writeBE(os, uint32_t(m_objAssocs.size()));

	for (int32_t i = 0; i < nbOfReserved; ++i) {
		ibeosdk::writeBE(os, m_reserved[i]);
	}

	ObjAssocVector::const_iterator oaIter = m_objAssocs.begin();
	for (; oaIter != m_objAssocs.end(); ++oaIter) {
		if (!oaIter->serialize(os))
			break;
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool operator==(const ObjectAssociationList4001& lhs, const ObjectAssociationList4001& rhs)
{
	if (lhs.getRefObjListId() != rhs.getRefObjListId())
		return false;
	if (lhs.getRefDevType() != rhs.getRefDevType())
		return false;
	if (lhs.getRefDevInterfaceVersion() != rhs.getRefDevInterfaceVersion())
		return false;

	if (lhs.getDutObjListId() != rhs.getDutObjListId())
		return false;
	if (lhs.getDutDevType() != rhs.getDutDevType())
		return false;
	if (lhs.getDutDevInterfaceVersion() != rhs.getDutDevInterfaceVersion())
		return false;

	for (int32_t i = 0; i < ObjectAssociationList4001::nbOfReserved; ++i) {
		if (lhs.getReserved(i) != rhs.getReserved(i))
			return false;
	}

	return lhs.getObjectAssociations() == rhs.getObjectAssociations();
}

//======================================================================

}// namespace ibeosdk

//======================================================================
