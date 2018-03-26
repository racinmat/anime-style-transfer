//======================================================================
/*! \file VehicleStateBasicLux.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 2, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_VEHICLESTATEBASICLUX_HPP_SEEN
#define IBEOSDK_VEHICLESTATEBASICLUX_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/Time.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class VehicleStateBasicLux : public RegisteredDataBlock<VehicleStateBasicLux> {
public:
	enum ErrorFlags {
		EF_ParamAxleDistNotSet = 0x0001, ///< Axle dist parameter is not set, i.e. is set to zero.
		EF_LastSwaNotUpToDate  = 0x0100, ///< There is no latest (current) measurement of steering wheel angle (SWA).
		EF_LastFwaNotUpToDate  = 0x0200, ///< There is no latest (current) measurement of front wheel angle, or could not be calculated by SWA
		EF_BufferEmpty         = 0x0800  ///< No CAN data received
	}; // ErrorFlags

public:
	static std::streamsize getSerializedSize_static() { return 46; }

public:
	VehicleStateBasicLux();
	virtual ~VehicleStateBasicLux();

public:
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	NTPTime getTimestamp() const { return this->m_timestamp; }
	UINT16 getScanNumber() const { return this->m_scanNumber; }
	UINT16 getErrorFlags() const { return this->m_errorFlags; }
	INT16 getLongitudinalVelocity() const { return this->m_longitudinalVelocity; }
	INT16 getSteeringWheeAngle() const { return this->m_steeringWheeAngle; }
	INT16 getWheelAngle() const { return this->m_wheelAngle; }
	INT32 getXPos() const { return this->m_xPos; }
	INT32 getYPos() const { return this->m_yPos; }
	INT16 getCourseAngle() const { return this->m_courseAngle; }
	UINT16 getTimeDiff() const { return this->m_timeDiff; }
	INT16 getXDiff() const { return this->m_xDiff; }
	INT16 getYDiff() const { return this->m_yDiff; }
	INT16 getYaw() const { return this->m_yaw; }
	INT16 getCurrentYawRate() const { return this->m_currentYawRate; }

public: // setter
	void setTimestamp(const NTPTime newTimestamp) { this->m_timestamp = newTimestamp; }
	void setScanNumber(const UINT16 newScanNumber) { this->m_scanNumber = newScanNumber; }
	void setErrorFlags(const UINT16 newErrorFlags) { this->m_errorFlags = newErrorFlags; }
	void setLongitudinalVelocity(const INT16 newLongitudinalVelocity) { this->m_longitudinalVelocity = newLongitudinalVelocity; }
	void setSteeringWheeAngle(const INT16 newSteeringWheeAngle) { this->m_steeringWheeAngle = newSteeringWheeAngle; }
	void setWheelAngle(const INT16 newWheelAngle) { this->m_wheelAngle = newWheelAngle; }
	void setXPos(const INT32 newXPos) { this->m_xPos = newXPos; }
	void setYPos(const INT32 newYPos) { this->m_yPos = newYPos; }
	void setCourseAngle(const INT16 newCourseAngle) { this->m_courseAngle = newCourseAngle; }
	void setTimeDiff(const UINT16 newTimeDiff) { this->m_timeDiff = newTimeDiff; }
	void setXDiff(const INT16 newXDiff) { this->m_xDiff = newXDiff; }
	void setYDiff(const INT16 newYDiff) { this->m_yDiff = newYDiff; }
	void setYaw(const INT16 newYaw) { this->m_yaw = newYaw; }
	void setCurrentYawRate(const INT16 newCurrentYawRate) { this->m_currentYawRate = newCurrentYawRate; }


protected:
	NTPTime m_timestamp;          //!< timestamp of this data
	UINT16 m_scanNumber;          //!< scan number of the scan this vehicle state is associated with.
	UINT16 m_errorFlags;          //!< Holds error flags in case of an error.
	INT16 m_longitudinalVelocity; //!< Longitudinal Velocity (Car) [0.01 m/s]
	INT16 m_steeringWheeAngle;    //!< steering wheel angle [0.001rad]
	INT16 m_wheelAngle;           //!<wheel angle (already converted from steering wheel angle if necessary) [0.0001 rad]
	INT16 m_reserved0;            //!< unused, reserved for future use

	// calculated (in DSP) movement of scanner

	// absolute values
	INT32 m_xPos;        //!< Absolute X Position from origin [0.01m]
	INT32 m_yPos;        //!< Absolute Y Position from origin [0.01m]
	INT16 m_courseAngle; //!< Absolute orientation at time timeStamp [0.0001 rad]
	// relative values
	UINT16 m_timeDiff; //!< Absolute orientation at time timeStamp [0.0001 rad]
	INT16 m_xDiff;     //!< Difference in X during time difference to last transmission [0.001m]
	INT16 m_yDiff;     //!< Difference in Y during time difference to last transmission [0.001m]
	INT16 m_yaw;       //!< Difference in Heading during Timediff [0.0001 rad]
	// general informations
	UINT16 m_reserved1;        //!< unused, reserved for future use
	//========================================
	/*!\brief Current yaw rate [0.0001rad] from latest CAN-Message. Since 2.5.00.
	 * \remark Available since firmware version 2.5.00.
	 *///-------------------------------------
	INT16 m_currentYawRate;
	UINT32 m_reserved2;       //!< unused, reserved for future use
}; // VehicleStateBasicLux

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_VEHICLESTATEBASICLUX_HPP_SEEN

//======================================================================
