//======================================================================
/*! \file ScanLux.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 25, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/ScanLux.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ScanLux>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<ScanLux>::dataBlockId = DataTypeId(DataTypeId::DataType_LuxScan);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ScanLux>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<ScanLux>::dataBlockId, ibeosdk::RegisteredDataBlock<ScanLux>::create);

class IdcFile;
class IbeoLux;
class IbeoTrackingBox;
class IbeoScala;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ScanLux>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoLux>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoTrackingBox>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(registerIdInitial)
		)));

//======================================================================

ScanLux::ScanLux()
  : m_scanNumber(0),
    m_scannerStatus(0),
    m_syncPhaseOffset(0),
    m_startNTP(0),
    m_endNTP(0),
    m_angleTicksPerRotation(0),
    m_startAngle(0),
    m_endAngle(0),

    m_mountingPosYawAngle(0),
    m_mountingPosPitchAngle(0),
    m_mountingPosRollAngle(0),
    m_mountingPosX(0),
    m_mountingPosY(0),
    m_mountingPosZ(0),
    m_flags(0),

    m_points()
{}


//======================================================================

ScanLux::~ScanLux() {}

//======================================================================

bool ScanLux::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	// Note: This data type is encoded by using Little Endian

	lock();
	m_points.clear();

	ibeosdk::readLE(is, m_scanNumber);
	ibeosdk::readLE(is, m_scannerStatus);
	ibeosdk::readLE(is, m_syncPhaseOffset);
	ibeosdk::readLE(is, m_startNTP);
	ibeosdk::readLE(is, m_endNTP);
	if (!this->timeCheck()) { unlock(); return false; }

	ibeosdk::readLE(is, m_angleTicksPerRotation);
	if (!nbOfAngleTicksPerRotationCheck()) { unlock(); return false; }

	ibeosdk::readLE(is, m_startAngle);
	ibeosdk::readLE(is, m_endAngle);

	{ // read the number of the scan points
		// and resize the point vector.
		UINT16 nbScanPts=0;
		ibeosdk::readLE(is, nbScanPts);
		// adjust the list size
		m_points.resize(nbScanPts);
	}

	ibeosdk::readLE(is, m_mountingPosYawAngle);
	ibeosdk::readLE(is, m_mountingPosPitchAngle);
	ibeosdk::readLE(is, m_mountingPosRollAngle);
	ibeosdk::readLE(is, m_mountingPosX);
	ibeosdk::readLE(is, m_mountingPosY);
	ibeosdk::readLE(is, m_mountingPosZ);
	ibeosdk::readLE(is, m_flags);

	std::vector<ScanPointLux>::iterator ptIter = m_points.begin();
	for (; ptIter != m_points.end();++ptIter) {
		ptIter->deserialize(is);
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

std::streamsize ScanLux::getSerializedSize() const
{
	return std::streamsize(3*sizeof(UINT16) + 2*sizeof(NTPTime)) +
			std::streamsize(2*sizeof(UINT16)+2*sizeof(INT16)) +
			std::streamsize(m_points.size()) * ScanPointLux::getSerializedSize_static() +
			std::streamsize(6*sizeof(UINT16) + sizeof(UINT16));
}

//======================================================================

DataTypeId ScanLux::getDataType() const { return dataBlockId; }

//======================================================================

bool ScanLux::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeLE(os, m_scanNumber);
	ibeosdk::writeLE(os, m_scannerStatus);
	ibeosdk::writeLE(os, m_syncPhaseOffset);
	ibeosdk::writeLE(os, m_startNTP);
	ibeosdk::writeLE(os, m_endNTP);
	ibeosdk::writeLE(os, m_angleTicksPerRotation);
	ibeosdk::writeLE(os, m_startAngle);
	ibeosdk::writeLE(os, m_endAngle);
	const UINT16 nbScanPts = UINT16(m_points.size());
	ibeosdk::writeLE(os, nbScanPts);
	ibeosdk::writeLE(os, m_mountingPosYawAngle);
	ibeosdk::writeLE(os, m_mountingPosPitchAngle);
	ibeosdk::writeLE(os, m_mountingPosRollAngle);
	ibeosdk::writeLE(os, m_mountingPosX);
	ibeosdk::writeLE(os, m_mountingPosY);
	ibeosdk::writeLE(os, m_mountingPosZ);
	ibeosdk::writeLE(os, m_flags);

	for (unsigned int i=0; i < nbScanPts; ++i) {
		m_points[i].serialize(os);
	}

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================


bool ScanLux::timeCheck() const
{
	if (m_startNTP >= m_endNTP ) {
		logError << "Read scan #" << this->m_scanNumber
		    << ": Scan end time must be larger than scan start time."
		    << "  - discarding scan."
		    << std::endl;
		return false;
	}
	return true;
}

//======================================================================

bool ScanLux::nbOfAngleTicksPerRotationCheck() const
{
	if (m_angleTicksPerRotation != nbOfAngleTicksPerRotation) {
		logError << "Read scan #" << this->m_scanNumber
		    << ": number of angle ticks per rotation are " << this->m_angleTicksPerRotation
		    << " expected: " << nbOfAngleTicksPerRotation
		    << "  - discarding scan."
		    << std::endl;
		return false;
	}
	return true;
}

//======================================================================

} // namespace ibeosdk

//======================================================================

