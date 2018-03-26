
//======================================================================
/*! \file ScalaFpgaRawData2310.cpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/ScalaFpgaRawData2310.hpp>

#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<ScalaFpgaRawData>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<ScalaFpgaRawData2310>::dataBlockId = DataTypeId(DataTypeId::DataType_ScalaFpgaRawData);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ScalaFpgaRawData2310>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<ScalaFpgaRawData2310>::dataBlockId, ibeosdk::RegisteredDataBlock<ScalaFpgaRawData2310>::create);

class IdcFile;
class IbeoEcu;
class IbeoScala;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<ScalaFpgaRawData2310>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(registerIdInitial)
		));

//======================================================================
//======================================================================
//======================================================================

ScalaFpgaRawData2310::ScalaFpgaRawData2310()
  : RegisteredDataBlock<ScalaFpgaRawData2310>(),
    m_magicWord(0x5CA7ADA7),
    m_internalDataTypeId(0xD0D2),
    m_reserved00(0),
    m_payloadSize(48),
    m_utcSeconds(0),
    m_flexrayMeasTime(0),
    m_reserved01(0),
    m_reserved02(0),
    m_flexrayMasterClock(0),
    m_reserved03(0),
    m_versionInfo0(0),
    m_versionInfo1(0),
    m_scanStartTime(),
    m_scanEndTime(),
    m_scanMidTime(),
    m_reserved04(0),
    m_fpgaData()
{}

//======================================================================

ScalaFpgaRawData2310::~ScalaFpgaRawData2310() {}

//======================================================================

std::streamsize ScalaFpgaRawData2310::getSerializedSize() const
{
	return std::streamsize(60) + m_fpgaData.getSerializedSize();
}

//======================================================================

DataTypeId ScalaFpgaRawData2310::getDataType() const { return dataBlockId; }

//======================================================================

bool ScalaFpgaRawData2310::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	writeBE(os, m_magicWord);
	writeBE(os, m_internalDataTypeId);
	writeBE(os, m_reserved00);
	writeBE(os, m_payloadSize);
	writeBE(os, m_utcSeconds);
	writeBE(os, m_flexrayMeasTime);
	writeBE(os, m_reserved01);
	writeBE(os, m_reserved02);
	writeBE(os, m_flexrayMasterClock);
	writeBE(os, m_reserved03);
	writeBE(os, m_versionInfo0);
	writeBE(os, m_versionInfo1);
	writeBE(os, m_scanStartTime);
	writeBE(os, m_scanEndTime);
	writeBE(os, m_scanMidTime);
	writeBE(os, m_reserved04);
	m_fpgaData.serialize(os);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ScalaFpgaRawData2310::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());
	bool ok = true;

	lock();

	readBE(is, m_magicWord);
	readBE(is, m_internalDataTypeId);
	readBE(is, m_reserved00);
	readBE(is, m_payloadSize);
	readBE(is, m_utcSeconds);
	readBE(is, m_flexrayMeasTime);
	readBE(is, m_reserved01);
	readBE(is, m_reserved02);
	readBE(is, m_flexrayMasterClock);
	readBE(is, m_reserved03);
	readBE(is, m_versionInfo0);
	readBE(is, m_versionInfo1);
	readBE(is, m_scanStartTime);
	readBE(is, m_scanEndTime);
	readBE(is, m_scanMidTime);
	readBE(is, m_reserved04);
	ok = m_fpgaData.deserialize(is);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize()
	       && ok;
}

//======================================================================

bool ScalaFpgaRawData2310::extractScan(Scan2208& scan) const
{
	scan.setScanNumber(m_reserved00);
	scan.setScannerType(ScannerType_ScalaB2);
	scan.setScannerStatus(0); // TODO
	scan.setAngleTicksPerRotation(11520);
	scan.setProcessingFlags(0); // TODO
	scan.getMountingPosition().set(0, 0, 0, 0, 0, 0);
	// TODO: threshold 0 - 6
	scan.setDeviceId(uint8_t(this->getScalaFpgaRawData().getHeader().getDeviceId()));
	Scan2208::SubScanVector& subScans = scan.getSubScans();
	subScans.resize(1);
	SubScan2208& sub = subScans[0];

	sub.setStartScanTimestamp(m_scanStartTime);
	sub.setEndScanTimestamp(m_scanEndTime);
	sub.setStartScanAngle(int16_t(72.5*32)); // TODO
	sub.setEndScanAngle(int16_t(-72.5*32)); // TODO
	sub.setFlags(0); // TODO
	sub.setMirrorSide(uint8_t(m_fpgaData.getTrailer().getMirrorSide())); // TODO
	sub.setMirrorTilt(uint16_t(0.4*500)); // TODO
	sub.getScanPoints().clear();
	sub.getScanPoints().resize(m_fpgaData.getPoints().size()); // TODO

	std::vector<ScalaFpgaRawScanPoint>::const_iterator srcPtIter = m_fpgaData.getPoints().begin();
	SubScan2208::PointVector::iterator targetPtIter = sub.getScanPoints().begin();
	for (; targetPtIter != sub.getScanPoints().end(); ++targetPtIter, ++srcPtIter) {
		// TODO all
		targetPtIter->setThresholdId(0);
		targetPtIter->setEchoId(srcPtIter->getEchoId());
		targetPtIter->setReserved(0);
		targetPtIter->setLayerId(srcPtIter->getChannelId());
		targetPtIter->setFlags(rawTo2208Flags(uint16_t(((srcPtIter->getFlagsHigh()) << 8) | (srcPtIter->getFlagsLow()))));
		targetPtIter->setHorizontalAngle(srcPtIter->getAngle());
		targetPtIter->setRadialDistance(srcPtIter->getRadialDistance());
		targetPtIter->setEchoPulseWidth(srcPtIter->getEchoPulseWidth());
		targetPtIter->setPfValue(0);
	}
	return true;
}

//======================================================================

uint16_t ScalaFpgaRawData2310::rawTo2208Flags(const uint16_t rawFlags) const
{
	// RSPF_Transparent
	// RSPF_Rain
	// RSPF_Ground
	// RSPF_Dirt
	// RSPF_HighTresholdH1
	// ...
	// RSPF_HighTresholdH2
	uint16_t flags2208 = rawFlags & 0x009f; // first 5 flags and flag 7 have the same meaning

	if ((rawFlags & ScalaFpgaRawScanPointBase::RSPF_NearRange) == ScalaFpgaRawScanPointBase::RSPF_NearRange)
		setFlag(flags2208, uint16_t(ScanPoint2208::SPFML_NearRange));

	if ((rawFlags & ScalaFpgaRawScanPointBase::RSPF_Noise) == ScalaFpgaRawScanPointBase::RSPF_Noise)
		setFlag(flags2208, uint16_t(ScanPoint2208::SPFML_Noise));

	// unmapped: RSPF_CandidateInvalid
	// unmapped: RSPF_RainStep1Done
	// unmapped: RSPF_RainStep2Done

	// unmapped: RSPF_GroundStep1Done
	// unmapped: RSPF_GroundStep2Done
	// unmapped: RSPF_BlueValidCaculated
	// unmapped: RSPF_Flushed

	return flags2208;
}

//======================================================================

void ScalaFpgaRawData2310::setFlag(uint16_t& flagsToBeModified, const uint16_t flagToSet) const
{
	flagsToBeModified = uint16_t(flagsToBeModified | flagToSet);

}

//======================================================================

}// namespace ibeosdk

//======================================================================
