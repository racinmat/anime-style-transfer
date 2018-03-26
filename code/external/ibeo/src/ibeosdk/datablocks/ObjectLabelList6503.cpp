//======================================================================
/*! \file ObjectLabelList6503.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 28, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/ObjectLabelList6503.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ObjectLabelList6503>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<ObjectLabelList6503>::dataBlockId = DataTypeId(DataTypeId::DataType_ObjectLabel6503);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectLabelList6503>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<ObjectLabelList6503>::dataBlockId, ibeosdk::RegisteredDataBlock<ObjectLabelList6503>::create);

class IdcFile;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ObjectLabelList6503>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(registerIdInitial
		);

//======================================================================
//======================================================================
//======================================================================

ObjectLabelList6503::ObjectLabelList6503()
  : m_timeOffsetUs(0),
    m_timestamp(),
    m_scanNumber(0),
    m_scanMidTimestamp(),
    // m_reserved
    m_labels()
{
	memset(&m_reserved[0], 0, nbOfReserved * sizeof(uint16_t));
}

//======================================================================

std::streamsize ObjectLabelList6503::getSerializedSize() const
{
	std::streamsize sz = std::streamsize(2*sizeof(uint32_t))
	                   + std::streamsize(2*sizeof(uint64_t))
	                   + std::streamsize(nbOfReserved * sizeof(uint16_t))
	                   + std::streamsize(1*sizeof(uint16_t));

	LabelVector::const_iterator lIter = m_labels.begin();
	for (;lIter != m_labels.end(); ++lIter) {
		sz += lIter->getSerializedSize();
	}
	return sz;
}

//======================================================================

DataTypeId ObjectLabelList6503::getDataType() const { return dataBlockId; }

//======================================================================

bool ObjectLabelList6503::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readBE(is, m_timeOffsetUs);
	ibeosdk::readBE(is, m_timestamp);
	ibeosdk::readBE(is, m_scanNumber);
	ibeosdk::readBE(is, m_scanMidTimestamp);

	for (uint16_t r = 0; r < nbOfReserved; ++r) {
		ibeosdk::readBE(is, m_reserved[r]);
	}

	uint16_t nbOfLabels = 0;
	ibeosdk::readBE(is, nbOfLabels);
	m_labels.resize(nbOfLabels);

	LabelVector::iterator lIter = m_labels.begin();
	for (; lIter != m_labels.end(); ++lIter) {
		if (!lIter->deserialize(is)) {
			break;
		}
	}

	unlock();
	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool ObjectLabelList6503::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, m_timeOffsetUs);
	ibeosdk::writeBE(os, m_timestamp);
	ibeosdk::writeBE(os, m_scanNumber);
	ibeosdk::writeBE(os, m_scanMidTimestamp);

	for (uint16_t r = 0; r < nbOfReserved; ++r) {
		ibeosdk::writeBE(os, m_reserved[r]);
	}

	ibeosdk::writeBE(os, uint16_t(m_labels.size()));

	LabelVector::const_iterator lIter = m_labels.begin();
	for (; lIter != m_labels.end(); ++lIter) {
		if (!lIter->serialize(os)) {
			break;
		}
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool operator==(const ObjectLabelList6503& lhs, const ObjectLabelList6503& rhs)
{
	if ( lhs.getTimeOffsetUs() != rhs.getTimeOffsetUs()
	  || lhs.getTimestamp() != rhs.getTimestamp()
	  || lhs.getScanNumber() != rhs.getScanNumber()
	  || lhs.getScanMidTimestamp() != rhs.getScanMidTimestamp())
		return false;

	for (uint16_t r = 0; r < ObjectLabel6503::nbOfReserved; ++r) {
		if (lhs.getReserved(r) != rhs.getReserved(r))
			return false;
	}


	if (lhs.getLabels() != rhs.getLabels())
		return true;

	return true;
}

//======================================================================

bool operator!=(const ObjectLabelList6503& lhs, const ObjectLabelList6503& rhs)
{
	if ( lhs.getTimeOffsetUs() != rhs.getTimeOffsetUs()
	  || lhs.getTimestamp() != rhs.getTimestamp()
	  || lhs.getScanNumber() != rhs.getScanNumber()
	  || lhs.getScanMidTimestamp() != rhs.getScanMidTimestamp())
		return true;

	for (uint16_t r = 0; r < ObjectLabelList6503::nbOfReserved; ++r) {
		if (lhs.getReserved(r) != rhs.getReserved(r))
			return true;
	}

	if (lhs.getLabels() != rhs.getLabels())
		return true;

	return false;
}

//======================================================================

}// namespace ibeosdk

//======================================================================
