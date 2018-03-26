//======================================================================
/*!
 * \file SystemMonitoringSystemStatus6705.hpp
 *
 * ---------------------------------------------------------------------
 * \copydoc Copyright
 *
 * \date   13.04.2017
 * \author Martin Schlodinski (msch)
 */
//======================================================================

#ifndef IBEOSDK_SYSTEMMONITORINGSYSTEMSTATUS6705_HPP_SEEN
#define IBEOSDK_SYSTEMMONITORINGSYSTEMSTATUS6705_HPP_SEEN

//======================================================================

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/Timestamp.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class SystemMonitoringSystemStatus6705 : public ibeosdk::RegisteredDataBlock<SystemMonitoringSystemStatus6705>
{
public:
	//! State of the system.
	enum State {
		StateInit    = 0,
		StateOK      = 1,
		StateWarning = 2,
		StateError   = 3
	};

	//! State information bits
	enum StateInformationBits {
		bit00_highLatency      = 0,
		bit03_memoryAlmostFull = 3,
		bit05_timeout          = 5,
		bit06_memoryFull       = 6
	};

public:
	//! Constructor.
	SystemMonitoringSystemStatus6705();

	//!< Destructor.
	virtual ~SystemMonitoringSystemStatus6705();

public:
	//! Assignment operator.
	SystemMonitoringSystemStatus6705& operator=(const SystemMonitoringSystemStatus6705& other);

	//! Equal operator.
	bool operator==(const SystemMonitoringSystemStatus6705& other) const;

	//! Not equal operator.
	bool operator!=(const SystemMonitoringSystemStatus6705& other) const;

public:
	virtual DataTypeId getDataType() const;

	virtual std::streamsize getSerializedSize() const;

	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);

	virtual bool serialize(std::ostream& os) const;

public:
	/*!
	 * \brief Convert the object to a string.
	 * \return the object as a string.
	 */
	std::string toString() const;

	//! Set the time stamp of the last update.
	void setLastUpdateTimestamp(const boost::posix_time::ptime newLastUpdateTimestamp) { m_lastUpdateTimestamp = newLastUpdateTimestamp; }

	//! Set the system state.
	void setState(const State newState) { m_state = newState; }

	//! Set the system state information.
	void setStateInformation(const uint8_t newStateInformation) { m_stateInformation = newStateInformation; }

	//! Set the CPU usage.
	void setCurrentCpuUsage(const uint8_t newCpuUsage) { m_currentCpuUsage = newCpuUsage; }

	//! Set the RAM usage.
	void setCurrentRamUsage(const uint8_t newRamUsage) { m_currentRamUsage = newRamUsage; }

	//! Set the HDD usage.
	void setCurrentHddUsage(const uint8_t newHddUsage) { m_currentHddUsage = newHddUsage; }

	//! Set the HDD warning level.
	void setHddWarningLevel(const uint8_t newHddWarningLevel) { m_HddWarningLevelPercent = newHddWarningLevel; }

	//! Set the HDD error level.
	void setHddErrorLevel(const uint8_t newHddErrorLevel) { m_HddErrorLevelPercent = newHddErrorLevel; }

	//! Get the time stamp of the last update.
	const boost::posix_time::ptime getLastUpdateTimestamp() const { return m_lastUpdateTimestamp; }

	//! Get the system state.
	State getState() const { return m_state; }

	//! Get the system state information.
	uint8_t getStateInformation() const { return m_stateInformation; }

	//! Get the CPU usage.
	uint8_t getCurrentCpuUsage() const { return m_currentCpuUsage; }

	//! Get the RAM usage.
	uint8_t getCurrentRamUsage() const { return m_currentRamUsage; }

	//! Get the HDD usage.
	uint8_t getCurrentHddUsage() const { return m_currentHddUsage; }

	//! Get the HDD warning level.
	uint8_t getHDDWarningLevel() const { return m_HddWarningLevelPercent; }

	//! Get the HDD error level.
	uint8_t getHDDErrorLevel() const { return m_HddErrorLevelPercent; }

private:
	//                                                   Unit                  Serialized size
	boost::posix_time::ptime m_lastUpdateTimestamp;   // ms since 01.01.1970   + 8 Bytes
	State m_state;                                    //                       + 1 Byte
	uint8_t m_stateInformation;                       //                       + 1 Byte
	uint8_t m_currentCpuUsage;                        //                       + 1 Byte
	uint8_t m_currentRamUsage;                        //                       + 1 Byte
	uint8_t m_currentHddUsage;                        //                       + 1 Byte
	uint8_t m_HddWarningLevelPercent;                 //                       + 1 Byte
	uint8_t m_HddErrorLevelPercent;                   //                       + 1 Byte
	//                                                                         = 15 Bytes
}; // SystemMonitoringSystemStatus6705

//======================================================================

template<>
inline
void readBE<SystemMonitoringSystemStatus6705::State>(std::istream& is, SystemMonitoringSystemStatus6705::State& value)
{
	uint8_t tmp;
	readBE(is, tmp);
	value = SystemMonitoringSystemStatus6705::State(tmp);
}

//======================================================================

template<>
inline
void writeBE<SystemMonitoringSystemStatus6705::State>(std::ostream& os, const SystemMonitoringSystemStatus6705::State& value)
{
	const uint8_t tmp = value;
	writeBE(os, tmp);
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SYSTEMMONITORINGSYSTEMSTATUS6705_HPP_SEEN

//======================================================================
