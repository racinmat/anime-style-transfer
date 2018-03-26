//======================================================================
/*!
 * \file SystemMonitoringDeviceStatus6701.cpp
 *
 * ---------------------------------------------------------------------
 * \copydoc Copyright
 *
 * \date   13.04.2017
 * \author Martin Schlodinski (msch)
 */
//======================================================================

#include <ibeosdk/datablocks/SystemMonitoringDeviceStatus6701.hpp>

#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<SystemMonitoringDeviceStatus6701>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<SystemMonitoringDeviceStatus6701>::dataBlockId = DataTypeId(DataTypeId::DataType_SystemMonitoringDeviceStatus6701);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<SystemMonitoringDeviceStatus6701>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<SystemMonitoringDeviceStatus6701>::dataBlockId, ibeosdk::RegisteredDataBlock<SystemMonitoringDeviceStatus6701>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<SystemMonitoringDeviceStatus6701>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

SystemMonitoringDeviceStatus6701::SystemMonitoringDeviceStatus6701()
  : m_deviceId(0),
    m_type(DeviceTypeUnknown),
    m_typeInformation(0),
    m_state(StateInit),
    m_stateInformation(0),
    m_updateMissing() {}

//======================================================================

SystemMonitoringDeviceStatus6701::~SystemMonitoringDeviceStatus6701() {}

//======================================================================

SystemMonitoringDeviceStatus6701& SystemMonitoringDeviceStatus6701::operator=(const SystemMonitoringDeviceStatus6701& other)
{
	if (this == &other)
		return *this;

	m_deviceId = other.m_deviceId;
	m_type = other.m_type;
	m_typeInformation = other.m_typeInformation;
	m_state = other.m_state;
	m_stateInformation = other.m_stateInformation;
	m_updateMissing = other.m_updateMissing;

	return *this;
}

//======================================================================

bool SystemMonitoringDeviceStatus6701::operator==(const SystemMonitoringDeviceStatus6701& other) const
{
	return m_deviceId == other.m_deviceId &&
           m_type == other.m_type &&
           m_typeInformation == other.m_typeInformation &&
           m_state == other.m_state &&
           m_stateInformation == other.m_stateInformation &&
           m_updateMissing == other.m_updateMissing;
}

//======================================================================

bool SystemMonitoringDeviceStatus6701::operator!=(const SystemMonitoringDeviceStatus6701& other) const
{
	return !(*this == other);
}

//======================================================================

DataTypeId SystemMonitoringDeviceStatus6701::getDataType() const { return dataBlockId; }

//======================================================================

std::streamsize SystemMonitoringDeviceStatus6701::getSerializedSize() const
{
	std::streamsize result = 0;

	result += std::streamsize(sizeof(m_deviceId));
	result += std::streamsize(sizeof(uint8_t));
	result += std::streamsize(sizeof(m_typeInformation));
	result += std::streamsize(sizeof(uint8_t));
	result += std::streamsize(sizeof(m_stateInformation));
	result += std::streamsize(sizeof(uint64_t));

	return result;
}

//======================================================================

bool SystemMonitoringDeviceStatus6701::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readBE(is, m_deviceId);
	ibeosdk::readBE(is, m_type);
	ibeosdk::readBE(is, m_typeInformation);
	ibeosdk::readBE(is, m_state);
	ibeosdk::readBE(is, m_stateInformation);

	int64_t buffer;
	ibeosdk::readBE(is, buffer);

	const boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));

	// convert milliseconds to ptime
	m_updateMissing = epoch + boost::posix_time::milliseconds(buffer);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool SystemMonitoringDeviceStatus6701::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, m_deviceId);
	ibeosdk::writeBE(os, m_type);
	ibeosdk::writeBE(os, m_typeInformation);
	ibeosdk::writeBE(os, m_state);
	ibeosdk::writeBE(os, m_stateInformation);

	const boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));

	// convert ptime to int64
	const boost::posix_time::time_duration updateMissingFromEpoch = m_updateMissing - epoch;
	const uint64_t updateMissingMilliseconds = static_cast<uint64_t>(updateMissingFromEpoch.total_milliseconds());

	ibeosdk::writeBE(os, updateMissingMilliseconds);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::string SystemMonitoringDeviceStatus6701::toString() const
{
	std::stringstream ss;
	ss << "Device ID: ";
	ss << static_cast<uint32_t>(m_deviceId);
	ss << ", device type: ";

	switch (m_type) {
	case DeviceTypeScanner:
		ss << "Scanner";
		break;

	case DeviceTypeCamera:
		ss << "Camera";
		break;

	case DeviceTypeCan:
		ss << "CAN";
		break;

	case DeviceTypeWgs84:
		ss << "WGS84";
		break;

	case DeviceTypeGps:
		ss << "GPS";
		break;

	case DeviceTypeImu:
		ss << "IMU";
		break;

	default:
		ss << "Unknown";
		break;
	}

	ss << ", state: ";

	switch (m_state) {
	case StateInit:
		ss << "Initialization";
		break;

	case StateOK:
		ss << "OK";
		break;

	case StateWarning:
		ss << "Warning";
		break;

	case StateError:
		ss << "Error";
		break;

	default:
		ss << "Unknown";
		break;
	}

	return ss.str();
}

//======================================================================

} // namespace ibeosdk

//======================================================================
