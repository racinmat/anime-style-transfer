//======================================================================
/*! \file OxtsStatus.cpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Jun 30, 2014
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/OxtsStatus.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ObjectListEcu>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<OxtsStatus>::dataBlockId = DataTypeId(DataTypeId::DataType_OxtsStatus);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<OxtsStatus>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<OxtsStatus>::dataBlockId, ibeosdk::RegisteredDataBlock<OxtsStatus>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<OxtsStatus>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================
//======================================================================
//======================================================================

OxtsStatus::OxtsStatus()
  : m_hunterStatus(),
    m_dutPosition(),
    m_dutViewAngleHorizontal(),
    m_dutViewAngleVertical(),
    m_dutRange(),
    m_targetStatus()
{}

//======================================================================

std::streamsize OxtsStatus::getSerializedSize() const
{
	std::streamsize sz = 2*std::streamsize(sizeof(UINT8))
	                   + m_dutPosition.getSerializedSize()
	                   + 3* std::streamsize(sizeof(float));

	std::vector<TargetStatus>::const_iterator objIter = m_targetStatus.begin();
	for (; objIter != m_targetStatus.end(); ++objIter) {
		sz += objIter->getSerializedSize();
	}

	return sz;
}

//======================================================================

DataTypeId OxtsStatus::getDataType() const { return dataBlockId; }

//======================================================================

bool OxtsStatus::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	UINT8 nbOfTargets;
	ibeosdk::readBE(is, nbOfTargets);

	ibeosdk::readBE(is, m_hunterStatus);
	m_dutPosition.deserialize(is);
	ibeosdk::readBE(is, m_dutViewAngleHorizontal);
	ibeosdk::readBE(is, m_dutViewAngleVertical);
	ibeosdk::readBE(is, m_dutRange);

	if (m_targetStatus.size() != nbOfTargets) {
		m_targetStatus.resize(nbOfTargets);
	}

	std::vector<TargetStatus>::iterator targetIter = m_targetStatus.begin();
	for (; targetIter != m_targetStatus.end(); ++targetIter) {
		targetIter->deserialize(is);
	}

	unlock();
	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool OxtsStatus::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	UINT8 nbOfTargets = UINT8(m_targetStatus.size());
	ibeosdk::writeBE(os, nbOfTargets);

	ibeosdk::writeBE(os, m_hunterStatus);
	m_dutPosition.serialize(os);
	ibeosdk::writeBE(os, m_dutViewAngleHorizontal);
	ibeosdk::writeBE(os, m_dutViewAngleVertical);
	ibeosdk::writeBE(os, m_dutRange);

	std::vector<TargetStatus>::const_iterator targetIter = m_targetStatus.begin();
	for (; targetIter != m_targetStatus.end(); ++targetIter) {
		targetIter->serialize(os);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
