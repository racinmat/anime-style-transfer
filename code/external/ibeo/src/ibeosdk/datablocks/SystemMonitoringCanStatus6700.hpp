//======================================================================
/*!
 * \file SystemMonitoringCanStatus6700.hpp
 *
 * ---------------------------------------------------------------------
 * \copydoc Copyright
 *
 * \date   13.04.2017
 * \author Martin Schlodinski (msch)
 */
//======================================================================

#ifndef IBEOSDK_SYSTEMMONITORINGCANSTATUS6700_HPP_SEEN
#define IBEOSDK_SYSTEMMONITORINGCANSTATUS6700_HPP_SEEN

//======================================================================

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/Timestamp.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class SystemMonitoringCanStatus6700 : public ibeosdk::RegisteredDataBlock<SystemMonitoringCanStatus6700>
{
public:
	//! State of the device.
	enum State {
		StateInit    = 0,
		StateOK      = 1,
		StateWarning = 2,
		StateError   = 3
	};

	//! State information bits
	enum StateInformationBits {
		bit00_notEnoughSignals = 0,
		bit03_errorWasPresent  = 3,
		bit04_dropRateHigh     = 4,
		bit05_jitterHigh       = 5,
		bit07_timeout          = 7
	};

public:
	//! Constructor.
	SystemMonitoringCanStatus6700();

	//!< Destructor.
	virtual ~SystemMonitoringCanStatus6700();

public:
	//! Assignment operator.
	SystemMonitoringCanStatus6700& operator=(const SystemMonitoringCanStatus6700& other);

	//! Equal operator.
	bool operator==(const SystemMonitoringCanStatus6700& other) const;

	//! Not equal operator.
	bool operator!=(const SystemMonitoringCanStatus6700& other) const;

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

	//! Set the device ID.
	void setDeviceId(const uint8_t newDeviceId) { m_deviceId = newDeviceId; }

	//! Set the CAN message identifier.
	void setMessageId(const uint32_t newMessageId) { m_messageId = newMessageId; }

	//! Set the state of the CAN message.
	void setState(const State newState) { m_state = newState; }

	//! Set the state information of the CAN message.
	void setStateInformation(const uint8_t newStateInformation) { m_stateInformation = newStateInformation; }

	//! Set the timestamp of the last missing message.
	void setUpdateMissing(const boost::posix_time::ptime newUpdateMissing) { m_updateMissing = newUpdateMissing; }

	//! Get the device ID.
	uint8_t getDeviceId() const { return m_deviceId; }

	//! Get the CAN message identifier.
	uint32_t getMessageId() const { return m_messageId; }

	//! Get the state of the CAN message.
	State getState() const { return m_state; }

	//! Get the state information of the CAN message.
	uint8_t getStateInformation() const { return m_stateInformation; }

	//! Get the timestamp of the last missing message.
	boost::posix_time::ptime getUpdateMissing() const { return m_updateMissing; }

private:
	//                                             Unit            Serialized size
	uint8_t   m_deviceId;                       //                 + 1 Byte
	uint32_t  m_messageId;                      //                 + 4 Bytes
	State m_state;                              //                 + 1 Byte
	uint8_t   m_stateInformation;               //                 + 1 Byte
	boost::posix_time::ptime m_updateMissing;   //                 + 8 Bytes
	//                                                             = 15 Bytes
}; // SystemMonitoringCanStatus6700

//======================================================================

template<>
inline
void readBE<SystemMonitoringCanStatus6700::State>(std::istream& is, SystemMonitoringCanStatus6700::State& value)
{
	uint8_t tmp;
	readBE(is, tmp);
	value = SystemMonitoringCanStatus6700::State(tmp);
}

//======================================================================

template<>
inline
void writeBE<SystemMonitoringCanStatus6700::State>(std::ostream& os, const SystemMonitoringCanStatus6700::State& value)
{
	const uint8_t tmp = value;
	writeBE(os, tmp);
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SYSTEMMONITORINGCANSTATUS6700_HPP_SEEN

//======================================================================
