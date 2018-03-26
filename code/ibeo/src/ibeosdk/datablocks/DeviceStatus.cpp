//======================================================================
/*! \file DeviceStatus.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Nov 4, 2013
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/DeviceStatus.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<DeviceStatus>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<DeviceStatus>::dataBlockId = DataTypeId(DataTypeId::DataType_DeviceStatus);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<DeviceStatus>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<DeviceStatus>::dataBlockId, ibeosdk::RegisteredDataBlock<DeviceStatus>::create);

class IdcFile;
class IbeoEcu;
class IbeoScala;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<DeviceStatus>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(registerIdInitial)
		));


//======================================================================
//======================================================================
//======================================================================

//static
std::streamsize DeviceStatus::getSerializedSize_static()
{
	return 168;
}

//======================================================================

DeviceStatus::DeviceStatus()
  : RegisteredDataBlock<DeviceStatus>(),
    m_serialNumberOfScanner(),
    m_scannerType(),
    m_reserved0(0),
    m_fpgaVersion(),
    m_dspVersion(),
    m_hostVersion(),
    m_fpgaModusRegister(0),
    m_reserved1(0),
    m_sensorTemperature(.0f),
    m_frequency(.0f),
    m_apdTableVoltage(.0f),
    m_adaptiveApdVoltage(nbOfApdSectors, .0f),
    m_minApdVoltageOffset(.0f),
    m_maxApdVoltageOffset(.0f),
    m_reserved2(0),
    m_reserved3(0),
    m_noiseMeasurementThreshold(.0f),
    m_referenceNoise(0),
    m_actualNoise(nbOfApdSectors, 0),
    m_reservedA(nbOfReservedA, 0)
{}

//======================================================================

DeviceStatus::~DeviceStatus() {}

//======================================================================

DataTypeId DeviceStatus::getDataType() const { return dataBlockId; }

//======================================================================

std::streamsize DeviceStatus::getSerializedSize() const
{
	return getSerializedSize_static();
}

//======================================================================

bool DeviceStatus::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	if (!m_serialNumberOfScanner.deserialize(is)) { unlock(); return false; }
	UINT8 st;
	ibeosdk::readLE(is, st);
	m_scannerType = ScannerType(st);
	ibeosdk::readLE(is, m_reserved0);

	if (!m_fpgaVersion.deserialize(is)) { unlock(); return false; }
	if (!m_dspVersion.deserialize(is)) { unlock(); return false; }
	if (!m_hostVersion.deserialize(is)) { unlock(); return false; }

	ibeosdk::readLE(is, m_fpgaModusRegister);
	ibeosdk::readLE(is, m_reserved1);

	ibeosdk::readLE(is, m_sensorTemperature);
	ibeosdk::readLE(is, m_frequency);
	ibeosdk::readLE(is, m_apdTableVoltage);
	for (unsigned int s = 0; s < nbOfApdSectors; ++s)
		ibeosdk::readLE(is, m_adaptiveApdVoltage[s]);
	ibeosdk::readLE(is, m_minApdVoltageOffset);
	ibeosdk::readLE(is, m_maxApdVoltageOffset);

	ibeosdk::readLE(is, m_reserved2);
	ibeosdk::readLE(is, m_reserved3);

	ibeosdk::readLE(is, m_noiseMeasurementThreshold);
	ibeosdk::readLE(is, m_referenceNoise);
	for (unsigned int s = 0; s < nbOfApdSectors; ++s)
		ibeosdk::readLE(is, m_actualNoise[s]);

	for (unsigned int r = 0; r < nbOfReservedA; ++r)
		ibeosdk::readLE(is, m_reservedA[r]);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool DeviceStatus::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	if (!m_serialNumberOfScanner.serialize(os)) { unlock(); return false; }
	UINT8 st = m_scannerType;
	ibeosdk::writeLE(os, st);
	ibeosdk::writeLE(os, m_reserved0);

	if (!m_fpgaVersion.serialize(os)) { unlock(); return false; }
	if (!m_dspVersion.serialize(os)) { unlock(); return false; }
	if (!m_hostVersion.serialize(os)) { unlock(); return false; }

	ibeosdk::writeLE(os, m_fpgaModusRegister);
	ibeosdk::writeLE(os, m_reserved1);

	ibeosdk::writeLE(os, m_sensorTemperature);
	ibeosdk::writeLE(os, m_frequency);
	ibeosdk::writeLE(os, m_apdTableVoltage);
	for (unsigned int s = 0; s < nbOfApdSectors; ++s)
		ibeosdk::writeLE(os, m_adaptiveApdVoltage[s]);
	ibeosdk::writeLE(os, m_minApdVoltageOffset);
	ibeosdk::writeLE(os, m_maxApdVoltageOffset);

	ibeosdk::writeLE(os, m_reserved2);
	ibeosdk::writeLE(os, m_reserved3);

	ibeosdk::writeLE(os, m_noiseMeasurementThreshold);
	ibeosdk::writeLE(os, m_referenceNoise);
	for (unsigned int s = 0; s < nbOfApdSectors; ++s)
		ibeosdk::writeLE(os, m_actualNoise[s]);

	for (unsigned int r = 0; r < nbOfReservedA; ++r)
		ibeosdk::writeLE(os, m_reservedA[r]);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================

//======================================================================
