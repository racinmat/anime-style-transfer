//======================================================================
/*! \file LogPolygonList2dFloat.cpp
 *
 * \copydoc Copyright
 * \author Ruben Jungnickel (rju)
 * \date Sep 11, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/LogPolygonList2dFloat.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ObjectListEcu>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<LogPolygonList2dFloat>::dataBlockId = DataTypeId(DataTypeId::DataType_LogPolygonList2dFloat);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<LogPolygonList2dFloat>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<LogPolygonList2dFloat>::dataBlockId, ibeosdk::RegisteredDataBlock<LogPolygonList2dFloat>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<LogPolygonList2dFloat>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================
//======================================================================
//======================================================================

LogPolygonList2dFloat::LogPolygonList2dFloat()
  : m_logList()
{}

//======================================================================

LogPolygonList2dFloat::~LogPolygonList2dFloat() {}

//======================================================================

std::streamsize LogPolygonList2dFloat::getSerializedSize() const
{
	std::streamsize ret=sizeof(UINT16);

	std::vector<LogPolygon2dFloat>::const_iterator it = m_logList.begin();
	for (; it != m_logList.end(); ++it)
		ret += it->getSerializedSize();

	return ret;
}

//======================================================================

DataTypeId LogPolygonList2dFloat::getDataType() const { return dataBlockId; }

//======================================================================

bool LogPolygonList2dFloat::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	UINT16 nbPolygons;
	ibeosdk::readBE(is, nbPolygons);
	if (m_logList.size()!= nbPolygons)
		m_logList.resize(nbPolygons);

	std::vector<LogPolygon2dFloat>::iterator logIter = m_logList.begin();
	for (; logIter != m_logList.end(); ++logIter)
		logIter->deserialize(is);

	unlock();
	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool LogPolygonList2dFloat::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	const UINT16 nbOfPolygons = UINT16(this->m_logList.size());
	ibeosdk::writeBE(os, nbOfPolygons);

	std::vector<LogPolygon2dFloat>::const_iterator logIter = m_logList.begin();
	for (; logIter != m_logList.end(); ++logIter) {
		logIter->serialize(os);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
