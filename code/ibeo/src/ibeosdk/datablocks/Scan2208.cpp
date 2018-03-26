//======================================================================
/*! \file Scan2208.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Aug 2, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/Scan2208.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<Scan2208>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<Scan2208>::dataBlockId = DataTypeId(DataTypeId::DataType_Scan2208);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<Scan2208>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<Scan2208>::dataBlockId, ibeosdk::RegisteredDataBlock<Scan2208>::create);

class IdcFile;
class IbeoMiniLux;
class IbeoLuxHr;
class IbeoScala;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<Scan2208>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoMiniLux>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoLuxHr>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(registerIdInitial)
		)));

//======================================================================

Scan2208::Scan2208()
  : scanNumber(0),
    scannerType(ScannerType_Invalid),
    scannerStatus(0),
    angleTicksPerRotation(0),
    processingFlags(0),
    mountingPosition(),
    deviceId(0)
{}

//======================================================================

Scan2208::~Scan2208() {}

//======================================================================

bool Scan2208::operator==(const Scan2208& other) const
{
	bool eq = this->scanNumber == other.scanNumber
	       && this->scannerType == other.scannerType
	       && this->scannerStatus == other.scannerStatus
	       && this->angleTicksPerRotation == other.angleTicksPerRotation
	       && this->processingFlags == other.processingFlags
	       && this->mountingPosition == other.mountingPosition;

	if (!eq)
		return false;

	for (int t = 0; t < Scan2208::nbOfThresholds; ++t) {
		eq &= this->thresholds[t] == other.thresholds[t];
	}

	if (!eq)
		return false;

	for (int r = 0; r < Scan2208::nbOfReserved; ++r) {
		eq &= this->reserved[r] == other.reserved[r];
	}

	eq &= this->deviceId == other.deviceId;
	eq &= this->subScans.size() == other.subScans.size();

	if (!eq)
		return false;

	for (unsigned int ss = 0; ss < this->subScans.size(); ++ss) {
		if (this->subScans.at(ss) != other.subScans.at(ss))
			return false;
	} // for ss
	return true;
}

//======================================================================

bool Scan2208::operator!=(const Scan2208& other) const
{
	bool neq = this->scanNumber != other.scanNumber
			|| this->scannerType != other.scannerType
			|| this->scannerStatus != other.scannerStatus
			|| this->angleTicksPerRotation != other.angleTicksPerRotation
			|| this->processingFlags != other.processingFlags
			|| this->mountingPosition != other.mountingPosition;

	if (neq)
		return true;

	for (int t = 0; t < Scan2208::nbOfThresholds; ++t) {
		neq |= this->thresholds[t] == other.thresholds[t];
	}
	if (neq)
		return true;

	for (int r = 0; r < Scan2208::nbOfReserved; ++r) {
		neq |= this->reserved[r] == other.reserved[r];
	}

	neq |= this->deviceId == other.deviceId;
	neq |= this->subScans.size() == other.subScans.size();

	if (neq)
		return true;

	for (unsigned int ss = 0; ss < this->subScans.size(); ++ss) {
		if (this->subScans.at(ss) != other.subScans.at(ss))
			return true;
	}
	return false;

}

//======================================================================

std::streamsize Scan2208::getSerializedSize() const
{
	uint32_t sz = 52;
	for (unsigned int s = 0; s < subScans.size(); ++s)
		sz += uint32_t(subScans.at(s).getSerializedSize());
	return sz;
}

//======================================================================

DataTypeId Scan2208::getDataType() const { return dataBlockId; }

//======================================================================

bool Scan2208::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	readBE(is, scanNumber);

	uint16_t tmp;
	readBE(is, tmp);
	scannerType = ScannerType(tmp);

	readBE(is, scannerStatus);
	readBE(is, angleTicksPerRotation);
	readBE(is, processingFlags);

	mountingPosition.deserialize(is);

	for (int t = 0; t < nbOfThresholds; ++t)
		readBE(is, thresholds[t]);

	for (int r = 0; r < nbOfReserved; ++r)
		readBE(is, reserved[r]);

	readBE(is, deviceId);

	uint8_t nbOfSubScans;
	readBE(is, nbOfSubScans);
	subScans.resize(nbOfSubScans);
	for (unsigned int s = 0; s < nbOfSubScans; ++s)
		if (!subScans[s].deserialize(is))
			return false;

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool Scan2208::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	writeBE(os, this->scanNumber);
	writeBE(os, uint16_t(this->scannerType));
	writeBE(os, this->scannerStatus);
	writeBE(os, this->angleTicksPerRotation);
	writeBE(os, this->processingFlags);
	mountingPosition.serialize(os);

	for (int t = 0; t < nbOfThresholds; ++t)
		writeBE(os, this->thresholds[t]);

	for (int r = 0; r < nbOfReserved; ++r)
		writeBE(os, this->reserved[r]);

	writeBE(os, deviceId);
	const uint8_t nbOfSubScans = uint8_t(subScans.size());
	writeBE(os, nbOfSubScans);

	for (unsigned int s = 0; s < nbOfSubScans; ++s)
		if (!subScans.at(s).serialize(os))
			return false;

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
