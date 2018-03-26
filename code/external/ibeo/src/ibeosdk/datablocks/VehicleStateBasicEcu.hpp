//======================================================================
/*! \file VehicleStateBasicEcu.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 2, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_VEHICLESTATEBASICECU_HPP_SEEN
#define IBEOSDK_VEHICLESTATEBASICECU_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/Time.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class VehicleStateBasicEcu : public RegisteredDataBlock<VehicleStateBasicEcu> {
public:
	static std::streamsize getSerializedSize_static() { return 90; }

public:
	VehicleStateBasicEcu();
	virtual ~VehicleStateBasicEcu();

public:
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	UINT32 getMicroseconds() const { return m_microseconds; }
	NTPTime getTimestamp() const { return m_timestamp; }
	INT32 getXPos() const { return m_xPos; }
	INT32 getYPos() const { return m_yPos; }
	float getCourseAngle() const { return m_courseAngle; }
	float getLongitudinalVelocity() const { return m_longitudinalVelocity; }
	float getYawRate() const { return m_yawRate; }
	float getSteeringWheelAngle() const { return m_steeringWheelAngle; }
	float getReserved0() const { return m_reserved0; }
	float getFrontWheelAngle() const { return m_frontWheelAngle; }
	UINT16 getReserved1() const { return m_reserved1; }
	float getVehicleWidth() const { return m_vehicleWidth; }
	float getReserved2() const { return m_reserved2; }
	float getVehicleFrontToFrontAxle() const { return m_vehicleFrontToFrontAxle; }
	float getFrontAxleToRearAxle() const { return m_frontAxleToRearAxle; }
	float getRearAxleToVehicleRear() const { return m_rearAxleToVehicleRear; }
	float getRearAxleToOrigin() const { return m_rearAxleToOrigin; }
	float getStearRatioCoeff0() const { return m_stearRatioCoeff0; }
	float getStearRatioCoeff1() const { return m_stearRatioCoeff1; }
	float getStearRatioCoeff2() const { return m_stearRatioCoeff2; }
	float getStearRatioCoeff3() const { return m_stearRatioCoeff3; }
	float getLongitudinalAcceleration() const { return m_longitudinalAcceleration; }

public: // setter
	void setMicroseconds(const UINT32 newMicroseconds) { m_microseconds = newMicroseconds; }
	void setTimestamp(const NTPTime newTimestamp) { m_timestamp = newTimestamp; }
	void setXPos(const INT32 newXPos) { m_xPos = newXPos; }
	void setYPos(const INT32 newYPos) { m_yPos = newYPos; }
	void setCourseAngle(const float newCourseAngle) { m_courseAngle = newCourseAngle; }
	void setLongitudinalVelocity(const float newLongitudinalVelocity) { m_longitudinalVelocity = newLongitudinalVelocity; }
	void setYawRate(const float newYawRate) { m_yawRate = newYawRate; }
	void setSteeringWheelAngle(const float newSteeringWheelAngle) { m_steeringWheelAngle = newSteeringWheelAngle; }
	void setReserved0(const float newReserved0) { m_reserved0 = newReserved0; }
	void setFrontWheelAngle(const float newFrontWheelAngle) { m_frontWheelAngle = newFrontWheelAngle; }
	void setReserved1(const UINT16 newReserved1) { m_reserved1 = newReserved1; }
	void setVehicleWidth(const float newVehicleWidth) { m_vehicleWidth = newVehicleWidth; }
	void setReserved2(const float newReserved2) { m_reserved2 = newReserved2; }
	void setVehicleFrontToFrontAxle(const float newVehicleFrontToFrontAxle) { m_vehicleFrontToFrontAxle = newVehicleFrontToFrontAxle; }
	void setFrontAxleToRearAxle(const float newFrontAxleToRearAxle) { m_frontAxleToRearAxle = newFrontAxleToRearAxle; }
	void setRearAxleToVehicleRear(const float newRearAxleToVehicleRear) { m_rearAxleToVehicleRear = newRearAxleToVehicleRear; }
	void setRearAxleToOrigin(const float newRearAxleToOrigin) { m_rearAxleToOrigin = newRearAxleToOrigin; }
	void setStearRatioCoeff0(const float newStearRatioCoeff0) { m_stearRatioCoeff0 = newStearRatioCoeff0; }
	void setStearRatioCoeff1(const float newStearRatioCoeff1) { m_stearRatioCoeff1 = newStearRatioCoeff1; }
	void setStearRatioCoeff2(const float newStearRatioCoeff2) { m_stearRatioCoeff2 = newStearRatioCoeff2; }
	void setStearRatioCoeff3(const float newStearRatioCoeff3) { m_stearRatioCoeff3 = newStearRatioCoeff3; }
	void setLongitudinalAcceleration(const float newLongitudinalAcceleration) { m_longitudinalAcceleration = newLongitudinalAcceleration; }

public:
	bool operator==(const VehicleStateBasicEcu& other) const;
	bool operator!=(const VehicleStateBasicEcu& other) const { return !((*this) == other); }

protected:
	UINT32 m_microseconds;           //!< microseconds since startup.
	NTPTime m_timestamp;             //!< timestamp of this data
	INT32 m_xPos;                    //!< Absolute X Position from origin [m * 1e-4]
	INT32 m_yPos;                    //!< Absolute Y Position from origin [m * 1e-4]
	float m_courseAngle;             //!< Orientation [rad]
	float m_longitudinalVelocity;    //!< Current longitudinal velocity of the vehicle [m/s]
	float m_yawRate;                 //!< Difference in Heading during Timediff [rad/s]
	float m_steeringWheelAngle;      //!< steering wheel angle [rad]
	float m_reserved0;
	float m_frontWheelAngle;         //!< front wheel angle [rad]
	UINT16 m_reserved1;
	float m_vehicleWidth;            //!< Vehicle width [m]
	float m_reserved2;
	float m_vehicleFrontToFrontAxle; //!< Distance: vehicle's front axle to vehicle's front [m]
	float m_frontAxleToRearAxle;     //!< Distance: vehicle's rear axle to vehicle's front axle [m]
	float m_rearAxleToVehicleRear;   //!< Distance: vehicle's rear axle to vehicle's rear [m]
	float m_rearAxleToOrigin;        //!< Distance: vehicle's rear axle to origin [m]
	float m_stearRatioCoeff0;        //!< s_3*x^3 + s_2*x^2 + s-2*x^1 + s_0
	float m_stearRatioCoeff1;        //!< s_3*x^3 + s_2*x^2 + s-2*x^1 + s_0
	float m_stearRatioCoeff2;        //!< s_3*x^3 + s_2*x^2 + s-2*x^1 + s_0
	float m_stearRatioCoeff3;        //!< s_3*x^3 + s_2*x^2 + s-2*x^1 + s_0
	float m_longitudinalAcceleration;//!< Current longitudinal acceleration of the vehicle [m/s^2]
}; // VehicleStateBasicEcu

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_VEHICLESTATEBASICECU_HPP_SEEN

//======================================================================
