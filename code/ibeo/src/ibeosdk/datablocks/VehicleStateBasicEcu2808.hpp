//======================================================================
/*! \file VehicleStateBasicEcu2808.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Oct 5, 2015
 *///-------------------------------------------------------------------

#ifndef VEHICLESTATEBASICECU2808_HPP_SEEN
#define VEHICLESTATEBASICECU2808_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/PositionWgs84.hpp>
#include <ibeosdk/Time.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class VehicleStateBasicEcu2808 : public RegisteredDataBlock<VehicleStateBasicEcu2808> {
public:
	// flags to be used to signal vehicle state source
	static const uint16_t VSSFlag_Unknown              = 0x0000;
	static const uint16_t VSSFlag_Can                  = 0x0001;
	static const uint16_t VSSFlag_Gps                  = 0x0002;
	static const uint16_t VSSFlag_Imu                  = 0x0004;
	static const uint16_t VSSFlag_LidarOdometry        = 0x0008;
	static const uint16_t VSSFlag_LandmarkLocalication = 0x0010;

public:
	static std::streamsize getSerializedSize_static() { return 204; }

public:
	VehicleStateBasicEcu2808();
	virtual ~VehicleStateBasicEcu2808();

public:
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	UINT32 getMicroseconds() const { return m_microseconds; }
	NTPTime getTimestamp() const { return m_timestamp; }
	uint16_t getSources() const { return m_sources; }
	uint16_t getBlindPredictionAge() const { return m_blindPredictionAge; }

	double getLatitude() const { return m_latitude; }
	float getLatitudeSigma() const { return m_latitudeSigma; }
	double getLongitude() const { return m_longitude; }
	float getLongitudeSigma() const { return m_longitudeSigma; }
	float getAltitude() const { return m_altitude; }
	float getAltitudeSigma() const { return m_altitudeSigma; }

	double getXPosition() const { return m_xPosition; }
	float getXPositionSigma() const { return m_xPositionSigma; }
	double getYPosition() const { return m_yPosition; }
	float getYPositionSigma() const { return m_yPositionSigma; }
	float getZPosition() const { return m_zPosition; }
	float getZPositionSigma() const { return m_zPositionSigma; }

	float getXyCorrelation() const { return m_xyCorrelation; }
	float getXzCorrelation() const { return m_xzCorrelation; }
	float getYzCorrelation() const { return m_yzCorrelation; }

	float getCourseAngle() const { return m_courseAngle; }
	float getCourseAngleSigma() const { return m_courseAngleSigma; }
	float getHeadingAngle() const { return m_headingAngle; }
	float getHeadingAngleSigma() const { return m_headingAngleSigma; }

	float getVehiclePitchAngle() const { return m_vehiclePitchAngle; }
	float getM_vehiclePitchAngleSigma() const { return m_vehiclePitchAngleSigma; }
	float getVehicleRollAngle() const { return m_vehicleRollAngle; }
	float getVehicleRollAngleSigma() const { return m_vehicleRollAngleSigma; }

	float getVehicleVelocity() const { return m_vehicleVelocity; }
	float getVehicleVelocitySigma() const { return m_vehicleVelocitySigma; }

	float getYawRate() const { return m_yawRate; }
	float getYawRateSigma() const { return m_yawRateSigma; }

	float getLongitudinalAccelearation() const { return m_longitudinalAccelearation; }
	float getLongitudinalAccelearationSigma() const { return m_longitudinalAccelearationSigma; }
	float getCrossAccelearation() const { return m_crossAccelearation; }
	float getCrossAccelearationSigma() const { return m_crossAccelearationSigma; }

	float getSteerAngle() const { return m_steerAngle; }
	float getSteeringWheelAngle() const { return m_steeringWheelAngle; }

	float getVehicleWidth() const { return m_vehicleWidth; }
	float getMinTurningCycle() const { return m_minTurningCycle; }

	float getVehicleFrontToFrontAxle() const { return m_vehicleFrontToFrontAxle; }
	float getFrontAxleToRearAxle() const { return m_frontAxleToRearAxle; }
	float getRearAxleToVehicleRear() const { return m_rearAxleToVehicleRear; }
	float getRearAxleToOrigin() const { return m_rearAxleToOrigin; }
	float getStearRatioCoeff0() const { return m_stearRatioCoeff0; }
	float getStearRatioCoeff1() const { return m_stearRatioCoeff1; }
	float getStearRatioCoeff2() const { return m_stearRatioCoeff2; }
	float getStearRatioCoeff3() const { return m_stearRatioCoeff3; }

	/// Returns the reference point as WGS84 type. If reference point is not set,
	/// lat and lon will be 0
	PositionWgs84 getReferencePoint() const;

public: // setter
	void setMicroseconds(const UINT32 newMicroseconds) { m_microseconds = newMicroseconds; }
	void setTimestamp(const NTPTime newTimestamp) { m_timestamp = newTimestamp; }
	void setSources(const uint16_t newSources) { m_sources = newSources; }
	void setBlindPredictionAge(const uint16_t newBlindPredictionAge) { m_blindPredictionAge = newBlindPredictionAge; }

	void setLatitude(const double newLatitude) { m_latitude = newLatitude; }
	void setLatitudeSigma(const float newLatitudeSigma) { m_latitudeSigma = newLatitudeSigma; }
	void setLongitude(const double newLongitude) { m_longitude = newLongitude; }
	void setLongitudeSigma(const float newLongitudeSigma) { m_longitudeSigma = newLongitudeSigma; }
	void setAltitude(const float newAltitude) { m_altitude = newAltitude; }
	void setAltitudeSigma(const float newAltitudeSigma) { m_altitudeSigma = newAltitudeSigma; }

	void setXPosition(const double newXPosition) { m_xPosition = newXPosition; }
	void setXPositionSigma(const float newXPositionSigma) { m_xPositionSigma = newXPositionSigma; }
	void setYPosition(const double newYPosition) { m_yPosition = newYPosition; }
	void setYPositionSigma(const float newYPositionSigma) { m_yPositionSigma = newYPositionSigma; }
	void setZPosition(const float newZPosition) { m_zPosition = newZPosition; }
	void setZPositionSigma(const float newZPositionSigma) { m_zPositionSigma = newZPositionSigma; }

	void setXyCorrelation(const float newXyCorrelation) { m_xyCorrelation = newXyCorrelation; }
	void setXzCorrelation(const float newXzCorrelation) { m_xzCorrelation = newXzCorrelation; }
	void setYzCorrelation(const float newYzCorrelation) { m_yzCorrelation = newYzCorrelation; }

	void setCourseAngle(const float newCourseAngle) { m_courseAngle = newCourseAngle; }
	void setCourseAngleSigma(const float newCourseAngleSigma) { m_courseAngleSigma = newCourseAngleSigma; }
	void setHeadingAngle(const float newHeadingAngle) { m_headingAngle = newHeadingAngle; }
	void setHeadingAngleSigma(const float newHeadingAngleSigma) { m_headingAngleSigma = newHeadingAngleSigma; }

	void setVehiclePitchAngle(const float newVehiclePitchAngle) { m_vehiclePitchAngle = newVehiclePitchAngle; }
	void setVehiclePitchAngleSigma(const float newVehiclePitchAngleSigma) { m_vehiclePitchAngleSigma = newVehiclePitchAngleSigma; }
	void setVehicleRollAngle(const float newVehicleRollAngle) { m_vehicleRollAngle = newVehicleRollAngle; }
	void setVehicleRollAngleSigma(const float newVehicleRollAngleSigma) { m_vehicleRollAngleSigma = newVehicleRollAngleSigma; }

	void setVehicleVelocity(const float newVehicleVelocity) { m_vehicleVelocity = newVehicleVelocity; }
	void setVehicleVelocitySigma(const float newVehicleVelocitySigma) { m_vehicleVelocitySigma = newVehicleVelocitySigma; }

	void setYawRate(const float newYawRate) { m_yawRate = newYawRate; }
	void setYawRateSigma(const float newYawRateSigma) { m_yawRateSigma = newYawRateSigma; }

	void setLongitudinalAccelearation(const float newLongitudinalAccelearation) { m_longitudinalAccelearation = newLongitudinalAccelearation; }
	void setLongitudinalAccelearationSigma(const float newLongitudinalAccelearationSigma) { m_longitudinalAccelearationSigma = newLongitudinalAccelearationSigma; }
	void setCrossAccelearation(const float newCrossAccelearation) { m_crossAccelearation = newCrossAccelearation; }
	void setCrossAccelearationSigma(const float newCrossAccelearationSigma) { m_crossAccelearationSigma = newCrossAccelearationSigma; }

	void setSteerAngle(const float newSteerAngle) { m_steerAngle = newSteerAngle; }
	void setSteeringWheelAngle(const float newSteeringWheelAngle) { m_steeringWheelAngle = newSteeringWheelAngle; }

	void setVehicleWidth(const float newVehicleWidth) { m_vehicleWidth = newVehicleWidth; }
	void setMinTurningCycle(const float newMinTurningCycle) { m_minTurningCycle = newMinTurningCycle; }

	void setVehicleFrontToFrontAxle(const float newVehicleFrontToFrontAxle) { m_vehicleFrontToFrontAxle = newVehicleFrontToFrontAxle; }
	void setFrontAxleToRearAxle(const float newFrontAxleToRearAxle) { m_frontAxleToRearAxle = newFrontAxleToRearAxle; }
	void setRearAxleToVehicleRear(const float newRearAxleToVehicleRear) { m_rearAxleToVehicleRear = newRearAxleToVehicleRear; }
	void setRearAxleToOrigin(const float newRearAxleToOrigin) { m_rearAxleToOrigin = newRearAxleToOrigin; }
	void setStearRatioCoeff0(const float newStearRatioCoeff0) { m_stearRatioCoeff0 = newStearRatioCoeff0; }
	void setStearRatioCoeff1(const float newStearRatioCoeff1) { m_stearRatioCoeff1 = newStearRatioCoeff1; }
	void setStearRatioCoeff2(const float newStearRatioCoeff2) { m_stearRatioCoeff2 = newStearRatioCoeff2; }
	void setStearRatioCoeff3(const float newStearRatioCoeff3) { m_stearRatioCoeff3 = newStearRatioCoeff3; }

public:
	bool operator==(const VehicleStateBasicEcu2808& other) const;
	bool operator!=(const VehicleStateBasicEcu2808& other) const { return !((*this) == other); }
	
protected:
	UINT32 m_microseconds;
	NTPTime m_timestamp;
	uint16_t m_sources;
	uint16_t m_blindPredictionAge;

	double m_latitude;
	float m_latitudeSigma;
	double m_longitude;
	float m_longitudeSigma;
	float m_altitude;
	float m_altitudeSigma;

	double m_xPosition;
	float m_xPositionSigma;
	double m_yPosition;
	float m_yPositionSigma;
	float m_zPosition;
	float m_zPositionSigma;

	float m_xyCorrelation;
	float m_xzCorrelation;
	float m_yzCorrelation;

	float m_courseAngle;
	float m_courseAngleSigma;
	float m_headingAngle;
	float m_headingAngleSigma;

	float m_vehiclePitchAngle;
	float m_vehiclePitchAngleSigma;
	float m_vehicleRollAngle;
	float m_vehicleRollAngleSigma;

	float m_vehicleVelocity;
	float m_vehicleVelocitySigma;

	float m_yawRate;                //!< Difference in Heading during Timediff [rad/s]
	float m_yawRateSigma;

	float m_longitudinalAccelearation;
	float m_longitudinalAccelearationSigma;
	float m_crossAccelearation;
	float m_crossAccelearationSigma;

	float m_steerAngle;
	float m_steeringWheelAngle;      //!< steering wheel angle [rad]

	float m_vehicleWidth;            //!< Vehicle width [m]
	float m_minTurningCycle;

	float m_vehicleFrontToFrontAxle; //!< Distance: vehicle's front axle to vehicle's front [m]
	float m_frontAxleToRearAxle;     //!< Distance: vehicle's rear axle to vehicle's front axle [m]
	float m_rearAxleToVehicleRear;   //!< Distance: vehicle's rear axle to vehicle's rear [m]
	float m_rearAxleToOrigin;        //!< Distance: vehicle's rear axle to origin [m]
	float m_stearRatioCoeff0;        //!< s_3*x^3 + s_2*x^2 + s-2*x^1 + s_0
	float m_stearRatioCoeff1;        //!< s_3*x^3 + s_2*x^2 + s-2*x^1 + s_0
	float m_stearRatioCoeff2;        //!< s_3*x^3 + s_2*x^2 + s-2*x^1 + s_0
	float m_stearRatioCoeff3;        //!< s_3*x^3 + s_2*x^2 + s-2*x^1 + s_0
}; // VehicleStateBasicEcu2808

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // VEHICLESTATEBASICECU2808_HPP_SEEN

//======================================================================
