//======================================================================
/*!
 * \file SystemMonitoringDeviceStatus6701.hpp
 *
 * ---------------------------------------------------------------------
 * \copydoc Copyright
 *
 * \date   13.04.2017
 * \author Martin Schlodinski (msch)
 */
//======================================================================

#ifndef IBEOSDK_SYSTEMMONITORINGDEVICESTATUS6701_HPP_SEEN
#define IBEOSDK_SYSTEMMONITORINGDEVICESTATUS6701_HPP_SEEN

//======================================================================

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/Timestamp.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class SystemMonitoringDeviceStatus6701 : public ibeosdk::RegisteredDataBlock<SystemMonitoringDeviceStatus6701>
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

	//! Type of the device.
	enum DeviceType {
		DeviceTypeUnknown = 0,
		DeviceTypeScanner = 1,
		DeviceTypeCamera  = 2,
		DeviceTypeCan     = 3,
		DeviceTypeWgs84   = 4,
		DeviceTypeGps     = 5,
		DeviceTypeImu     = 6
	};

public:
	//! Constructor.
	SystemMonitoringDeviceStatus6701();

	//!< Destructor.
	virtual ~SystemMonitoringDeviceStatus6701();

public:
	//! Assignment operator.
	SystemMonitoringDeviceStatus6701& operator=(const SystemMonitoringDeviceStatus6701& other);

	//! Equal operator.
	bool operator==(const SystemMonitoringDeviceStatus6701& other) const;

	//! Not equal operator.
	bool operator!=(const SystemMonitoringDeviceStatus6701& other) const;

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

	//! Set the device type.
	void setType(const DeviceType newType) { m_type = newType; }

	//! Set the device type information.
	void setTypeInformation(const uint8_t newTypeInformation) { m_typeInformation = newTypeInformation; }

	//! Set the state of the CAN message.
	void setState(const State newState) { m_state = newState; }

	//! Set the state information of the CAN message.
	void setStateInformation(const uint8_t newStateInformation) { m_stateInformation = newStateInformation; }

	//! Set the timestamp of the last missing message.
	void setUpdateMissing(const boost::posix_time::ptime newUpdateMissing) { m_updateMissing = newUpdateMissing; }

	//! Get the device ID.
	uint8_t getDeviceId() const { return m_deviceId; }

	//! Get the device type.
	DeviceType getType() const { return m_type; }

	//! Get the device type information.
	//
	// If the device type is "DeviceTypeScanner", the return value is
	//      0 for undefined
	//      1 for ibeo LUX
	//      2 for ibeo ScaLa
	//    100 for SICK LD-MRS
	//    101 for SICK LMS.
	//
	// If the device type is "DeviceTypeIMU", the return value is
	//      0 for undefined
	//      1 for Xsens
	//      2 for OXTS
	//      3 for Genesys.
	//
	// Otherwise, the return value is 0 (undefined).
	uint8_t getTypeInformation() const { return m_typeInformation; }

	//! Get the state of the CAN message.
	State getState() const { return m_state; }

	//! Get the state information of the CAN message.
	uint8_t getStateInformation() const { return m_stateInformation; }

	//! Get the timestamp of the last missing message.
	boost::posix_time::ptime getUpdateMissing() const { return m_updateMissing; }

private:
	//                                             Unit                  Serialized size
	uint8_t   m_deviceId;                       //                       + 1 Byte
	DeviceType m_type;                          //                       + 1 Byte
	uint8_t m_typeInformation;                  //                       + 1 Byte
	State m_state;                              //                       + 1 Byte
	uint8_t   m_stateInformation;               //                       + 1 Byte
	boost::posix_time::ptime m_updateMissing;   // ms since 01.01.1970   + 8 Bytes
	//                                                                   = 15 Bytes
}; // SystemMonitoringDeviceStatus6701

//======================================================================

template<>
inline
void readBE<SystemMonitoringDeviceStatus6701::DeviceType>(std::istream& is, SystemMonitoringDeviceStatus6701::DeviceType& value)
{
	uint8_t tmp;
	readBE(is, tmp);
	value = SystemMonitoringDeviceStatus6701::DeviceType(tmp);
}

//======================================================================

template<>
inline
void writeBE<SystemMonitoringDeviceStatus6701::DeviceType>(std::ostream& os, const SystemMonitoringDeviceStatus6701::DeviceType& value)
{
	const uint8_t tmp = value;
	writeBE(os, tmp);
}

//======================================================================

template<>
inline
void readBE<SystemMonitoringDeviceStatus6701::State>(std::istream& is, SystemMonitoringDeviceStatus6701::State& value)
{
	uint8_t tmp;
	readBE(is, tmp);
	value = SystemMonitoringDeviceStatus6701::State(tmp);
}

//======================================================================

template<>
inline
void writeBE<SystemMonitoringDeviceStatus6701::State>(std::ostream& os, const SystemMonitoringDeviceStatus6701::State& value)
{
	const uint8_t tmp = value;
	writeBE(os, tmp);
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SYSTEMMONITORINGDEVICESTATUS6701_HPP_SEEN

//======================================================================
