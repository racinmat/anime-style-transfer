//======================================================================
/*!
 * \file SystemMonitoringSystemStatus6705.cpp
 *
 * ---------------------------------------------------------------------
 * \copydoc Copyright
 *
 * \date   13.04.2017
 * \author Martin Schlodinski (msch)
 */
//======================================================================

#include <ibeosdk/datablocks/SystemMonitoringSystemStatus6705.hpp>

#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<SystemMonitoringSystemStatus6705>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<SystemMonitoringSystemStatus6705>::dataBlockId = DataTypeId(DataTypeId::DataType_SystemMonitoringSystemStatus6705);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<SystemMonitoringSystemStatus6705>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<SystemMonitoringSystemStatus6705>::dataBlockId, ibeosdk::RegisteredDataBlock<SystemMonitoringSystemStatus6705>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<SystemMonitoringSystemStatus6705>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

SystemMonitoringSystemStatus6705::SystemMonitoringSystemStatus6705()
  : m_lastUpdateTimestamp(),
    m_state(StateInit),
    m_stateInformation(0),
    m_currentCpuUsage(0),
    m_currentRamUsage(0),
    m_currentHddUsage(0),
    m_HddWarningLevelPercent(0),
    m_HddErrorLevelPercent(0) {}

//======================================================================

SystemMonitoringSystemStatus6705::~SystemMonitoringSystemStatus6705() {}

//======================================================================

SystemMonitoringSystemStatus6705& SystemMonitoringSystemStatus6705::operator=(const SystemMonitoringSystemStatus6705& other)
{
	if (this == &other)
		return *this;

	m_lastUpdateTimestamp = other.m_lastUpdateTimestamp;
	m_state = other.m_state;
	m_stateInformation = other.m_stateInformation;
	m_currentCpuUsage = other.m_currentCpuUsage;
	m_currentRamUsage = other.m_currentRamUsage;
	m_currentHddUsage = other.m_currentHddUsage;
	m_HddWarningLevelPercent = other.m_HddWarningLevelPercent;
	m_HddErrorLevelPercent = other.m_HddErrorLevelPercent;

	return *this;
}

//======================================================================

bool SystemMonitoringSystemStatus6705::operator==(const SystemMonitoringSystemStatus6705& other) const
{
	return m_lastUpdateTimestamp == other.m_lastUpdateTimestamp &&
           m_state == other.m_state &&
           m_stateInformation == other.m_stateInformation &&
           m_currentCpuUsage == other.m_currentCpuUsage &&
           m_currentRamUsage == other.m_currentRamUsage &&
           m_currentHddUsage == other.m_currentHddUsage &&
           m_HddWarningLevelPercent == other.m_HddWarningLevelPercent &&
           m_HddErrorLevelPercent == other.m_HddErrorLevelPercent;
}

//======================================================================

bool SystemMonitoringSystemStatus6705::operator!=(const SystemMonitoringSystemStatus6705& other) const
{
	return !(*this == other);
}

//======================================================================

DataTypeId SystemMonitoringSystemStatus6705::getDataType() const { return dataBlockId; }

//======================================================================

std::streamsize SystemMonitoringSystemStatus6705::getSerializedSize() const
{
	std::streamsize result = 0;

	result += std::streamsize(sizeof(uint64_t));
	result += std::streamsize(sizeof(uint8_t));
	result += std::streamsize(sizeof(m_stateInformation));
	result += std::streamsize(sizeof(m_currentCpuUsage));
	result += std::streamsize(sizeof(m_currentRamUsage));
	result += std::streamsize(sizeof(m_currentHddUsage));
	result += std::streamsize(sizeof(m_HddWarningLevelPercent));
	result += std::streamsize(sizeof(m_HddErrorLevelPercent));

	return result;
}

//======================================================================

bool SystemMonitoringSystemStatus6705::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	int64_t buffer;
	ibeosdk::readBE(is, buffer);

	const boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));

	// convert milliseconds to ptime
	m_lastUpdateTimestamp = epoch + boost::posix_time::milliseconds(buffer);

	ibeosdk::readBE(is, m_state);
	ibeosdk::readBE(is, m_stateInformation);
	ibeosdk::readBE(is, m_currentCpuUsage);
	ibeosdk::readBE(is, m_currentRamUsage);
	ibeosdk::readBE(is, m_currentHddUsage);
	ibeosdk::readBE(is, m_HddWarningLevelPercent);
	ibeosdk::readBE(is, m_HddErrorLevelPercent);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool SystemMonitoringSystemStatus6705::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	const boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));

	// convert ptime to int64
	const boost::posix_time::time_duration lastUpdateFromEpoch = m_lastUpdateTimestamp - epoch;
	const uint64_t lastUpdateMilliseconds = static_cast<uint64_t>(lastUpdateFromEpoch.total_milliseconds());

	ibeosdk::writeBE(os, lastUpdateMilliseconds);

	ibeosdk::writeBE(os, m_state);
	ibeosdk::writeBE(os, m_stateInformation);
	ibeosdk::writeBE(os, m_currentCpuUsage);
	ibeosdk::writeBE(os, m_currentRamUsage);
	ibeosdk::writeBE(os, m_currentHddUsage);
	ibeosdk::writeBE(os, m_HddWarningLevelPercent);
	ibeosdk::writeBE(os, m_HddErrorLevelPercent);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::string SystemMonitoringSystemStatus6705::toString() const
{
	std::stringstream ss;
	ss << "System state: ";

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
