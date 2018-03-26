//======================================================================
/*! \file VehicleStateBasicEcu2806.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Jan 14, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_VEHICLESTATEBASICECU2806_HPP_SEEN
#define IBEOSDK_VEHICLESTATEBASICECU2806_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/Time.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class VehicleStateBasicEcu2806 : public RegisteredDataBlock<VehicleStateBasicEcu2806> {
public:
	static std::streamsize getSerializedSize_static() { return 86; }

public:
	VehicleStateBasicEcu2806();
	virtual ~VehicleStateBasicEcu2806();

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
	float getFrontWheelAngle() const { return m_frontWheelAngle; }
	float getVehicleWidth() const { return m_vehicleWidth; }
	float getVehicleFrontToFrontAxle() const { return m_vehicleFrontToFrontAxle; }
	float getFrontAxleToRearAxle() const { return m_frontAxleToRearAxle; }
	float getRearAxleToVehicleRear() const { return m_rearAxleToVehicleRear; }
	float getStearRatioCoeff0() const { return m_stearRatioCoeff0; }
	float getStearRatioCoeff1() const { return m_stearRatioCoeff1; }
	float getStearRatioCoeff2() const { return m_stearRatioCoeff2; }
	float getStearRatioCoeff3() const { return m_stearRatioCoeff3; }

public: // setter
	void setMicroseconds(const UINT32 newMicroseconds) { m_microseconds = newMicroseconds; }
	void setTimestamp(const NTPTime newTimestamp) { m_timestamp = newTimestamp; }
	void setXPos(const INT32 newXPos) { m_xPos = newXPos; }
	void setYPos(const INT32 newYPos) { m_yPos = newYPos; }
	void setCourseAngle(const float newCourseAngle) { m_courseAngle = newCourseAngle; }
	void setLongitudinalVelocity(const float newLongitudinalVelocity) { m_longitudinalVelocity = newLongitudinalVelocity; }
	void setYawRate(const float newYawRate) { m_yawRate = newYawRate; }
	void setSteeringWheelAngle(const float newSteeringWheelAngle) { m_steeringWheelAngle = newSteeringWheelAngle; }
	void setFrontWheelAngle(const float newFrontWheelAngle) { m_frontWheelAngle = newFrontWheelAngle; }
	void setVehicleWidth(const float newVehicleWidth) { m_vehicleWidth = newVehicleWidth; }
	void setVehicleFrontToFrontAxle(const float newVehicleFrontToFrontAxle) { m_vehicleFrontToFrontAxle = newVehicleFrontToFrontAxle; }
	void setFrontAxleToRearAxle(const float newFrontAxleToRearAxle) { m_frontAxleToRearAxle = newFrontAxleToRearAxle; }
	void setRearAxleToVehicleRear(const float newRearAxleToVehicleRear) { m_rearAxleToVehicleRear = newRearAxleToVehicleRear; }
	void setStearRatioCoeff0(const float newStearRatioCoeff0) { m_stearRatioCoeff0 = newStearRatioCoeff0; }
	void setStearRatioCoeff1(const float newStearRatioCoeff1) { m_stearRatioCoeff1 = newStearRatioCoeff1; }
	void setStearRatioCoeff2(const float newStearRatioCoeff2) { m_stearRatioCoeff2 = newStearRatioCoeff2; }
	void setStearRatioCoeff3(const float newStearRatioCoeff3) { m_stearRatioCoeff3 = newStearRatioCoeff3; }


protected:
	UINT32 m_microseconds;           //!< microseconds since startup.
	NTPTime m_timestamp;             //!< timestamp of this data
	INT32 m_xPos;                    //!< Absolute X Position from origin [mm]
	INT32 m_yPos;                    //!< Absolute Y Position from origin [mm]
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
	float m_reserved3;
	float m_stearRatioCoeff0;        //!< s_3*x^3 + s_2*x^2 + s-2*x^1 + s_0
	float m_stearRatioCoeff1;        //!< s_3*x^3 + s_2*x^2 + s-2*x^1 + s_0
	float m_stearRatioCoeff2;        //!< s_3*x^3 + s_2*x^2 + s-2*x^1 + s_0
	float m_stearRatioCoeff3;        //!< s_3*x^3 + s_2*x^2 + s-2*x^1 + s_0
}; // VehicleStateBasicEcu

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_VEHICLESTATEBASICECU2806_HPP_SEEN

//======================================================================
