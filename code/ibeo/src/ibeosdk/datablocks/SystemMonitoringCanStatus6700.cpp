//======================================================================
/*!
 * \file SystemMonitoringCanStatus6700.cpp
 *
 * ---------------------------------------------------------------------
 * \copydoc Copyright
 *
 * \date   13.04.2017
 * \author Martin Schlodinski (msch)
 */
//======================================================================

#include "SystemMonitoringCanStatus6700.hpp"

#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<SystemMonitoringCanStatus6700>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<SystemMonitoringCanStatus6700>::dataBlockId = DataTypeId(DataTypeId::DataType_SystemMonitoringCanStatus6700);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<SystemMonitoringCanStatus6700>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<SystemMonitoringCanStatus6700>::dataBlockId, ibeosdk::RegisteredDataBlock<SystemMonitoringCanStatus6700>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<SystemMonitoringCanStatus6700>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

SystemMonitoringCanStatus6700::SystemMonitoringCanStatus6700()
  : m_deviceId(0),
    m_messageId(0),
    m_state(StateInit),
    m_stateInformation(0),
    m_updateMissing() {}

//======================================================================

SystemMonitoringCanStatus6700::~SystemMonitoringCanStatus6700() {}

//======================================================================

SystemMonitoringCanStatus6700& SystemMonitoringCanStatus6700::operator=(const SystemMonitoringCanStatus6700& other)
{
	if (this == &other)
		return *this;

	m_deviceId = other.m_deviceId;
	m_messageId = other.m_messageId;
	m_state = other.m_state;
	m_stateInformation = other.m_stateInformation;
	m_updateMissing = other.m_updateMissing;

	return *this;
}

//======================================================================

bool SystemMonitoringCanStatus6700::operator==(const SystemMonitoringCanStatus6700& other) const
{
	return m_deviceId == other.m_deviceId &&
           m_messageId == other.m_messageId &&
           m_state == other.m_state &&
           m_stateInformation == other.m_stateInformation &&
           m_updateMissing == other.m_updateMissing;
}

//======================================================================

bool SystemMonitoringCanStatus6700::operator!=(const SystemMonitoringCanStatus6700& other) const
{
	return !(*this == other);
}

//======================================================================

DataTypeId SystemMonitoringCanStatus6700::getDataType() const { return dataBlockId; }

//======================================================================

std::streamsize SystemMonitoringCanStatus6700::getSerializedSize() const
{
	std::streamsize result = 0;

	result += std::streamsize(sizeof(m_deviceId));
	result += std::streamsize(sizeof(m_messageId));
	result += std::streamsize(sizeof(uint8_t));
	result += std::streamsize(sizeof(m_stateInformation));
	result += std::streamsize(sizeof(uint64_t));

	return result;
}

//======================================================================

bool SystemMonitoringCanStatus6700::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readBE(is, m_deviceId);
	ibeosdk::readBE(is, m_messageId);
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

bool SystemMonitoringCanStatus6700::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, m_deviceId);
	ibeosdk::writeBE(os, m_messageId);
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

std::string SystemMonitoringCanStatus6700::toString() const
{
	std::stringstream ss;
	ss << "CAN device ID: ";
	ss << static_cast<uint32_t>(m_deviceId);
	ss << ", message ID: ";
	ss << m_messageId;
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
