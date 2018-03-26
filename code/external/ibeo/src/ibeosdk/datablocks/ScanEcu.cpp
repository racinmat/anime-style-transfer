//======================================================================
/*! \file ScanEcu.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 25, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/ScanEcu.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ScanEcu>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<ScanEcu>::dataBlockId = DataTypeId(DataTypeId::DataType_EcuScan);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ScanEcu>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<ScanEcu>::dataBlockId, ibeosdk::RegisteredDataBlock<ScanEcu>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ScanEcu>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

ScanEcu::ScanEcu()
  : m_scanStartTime(),
    m_endTimeOffset(),
    m_flags(),
    m_scanNumber(),
    m_reserved0(0),
    m_reserved1(0),
    m_scannerInfos(8),
    m_scanPoints(1024)
{}

//======================================================================

ScanEcu::~ScanEcu() {}

//======================================================================

DataTypeId ScanEcu::getDataType() const { return dataBlockId; }

//======================================================================

bool ScanEcu::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readBE(is, m_scanStartTime);
	ibeosdk::readBE(is, m_endTimeOffset);

	ibeosdk::readBE(is, m_flags);
	ibeosdk::readBE(is, m_scanNumber);

	UINT16 nbScanPts = 0;
	ibeosdk::readBE(is, nbScanPts);
	m_scanPoints.resize(nbScanPts);

	UINT8 nbScanInfos = 0;
	ibeosdk::readBE(is, nbScanInfos);
	m_scannerInfos.resize(nbScanInfos);

	ibeosdk::readBE(is, m_reserved0);
	ibeosdk::readBE(is, m_reserved1);

	for (unsigned int i=0; i < nbScanInfos; i++) {
		m_scannerInfos[i].deserialize(is);
	}

	for (unsigned int i=0; i < nbScanPts; i++) {
		m_scanPoints[i].deserialize(is);
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool ScanEcu::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, m_scanStartTime);
	ibeosdk::writeBE(os, m_endTimeOffset);

	ibeosdk::writeBE(os, m_flags);
	ibeosdk::writeBE(os, m_scanNumber);

	const UINT16 nbScanPts = UINT16(m_scanPoints.size());
	ibeosdk::writeBE(os, nbScanPts);

	const UINT8 nbScanInfos = UINT8(m_scannerInfos.size());
	ibeosdk::writeBE(os, nbScanInfos);

	ibeosdk::writeBE(os, m_reserved0);
	ibeosdk::writeBE(os, m_reserved1);

	for (unsigned int i=0; i < nbScanInfos; i++) {
		m_scannerInfos[i].serialize(os);
	}

	for (unsigned int i=0; i < nbScanPts; i++) {
		m_scanPoints[i].serialize(os);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::streamsize ScanEcu::getSerializedSize() const
{
	return std::streamsize(sizeof(NTPTime))
		+ std::streamsize(2*sizeof(UINT32))
		+ std::streamsize(2*sizeof(UINT16))
		+ std::streamsize(sizeof(UINT8))
		+ 3
		+ std::streamsize(UINT8(m_scannerInfos.size())) * ScannerInfo::getSerializedSize_static()
		+ std::streamsize(UINT16(m_scanPoints.size())) * ScanPointEcu::getSerializedSize_static();
}

//======================================================================

bool ScanEcu::operator==( const ScanEcu& other ) const
{
	if ( !(m_scanStartTime == other.getStartTimestamp()))
		return false;

	if (m_endTimeOffset != other.getEndTimeOffset())
		return false;

	if (m_flags != other.getFlags())
		return false;

	if (m_scanNumber != other.getScanNumber())
		return false;

	if (m_reserved0 != other.getReserved0())
		return false;

	if (m_reserved1 != other.getReserved1())
		return false;

	if (m_scannerInfos.size() != other.getScannerInfos().size())
		return false;

	for (size_t scannerInfoIdx=0; scannerInfoIdx < m_scannerInfos.size(); ++scannerInfoIdx) {
		if (m_scannerInfos[scannerInfoIdx] != (other.getScannerInfos())[scannerInfoIdx])
			return false;
	}
	
	if ( m_scanPoints.size() != other.getScanPoints().size() )
		return false;
	
	for (size_t scanPointIdx = 0; scanPointIdx < m_scanPoints.size(); ++scanPointIdx) {
		if (m_scanPoints[scanPointIdx] != (other.getScanPoints())[scanPointIdx])
			return false;
	}
	
	return true;
}


//======================================================================

} // namespace ibeosdk

//======================================================================
