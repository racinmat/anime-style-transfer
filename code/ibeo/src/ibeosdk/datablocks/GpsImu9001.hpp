//======================================================================
/*!
 * \file GpsImu9001.hpp
 *
 * ---------------------------------------------------------------------
 * \copydoc Copyright
 *
 * \date   07.04.2017
 * \author Peter Oltmann (peo)
 */
//======================================================================

#ifndef IBEOSDK_GPSIMU9001_HPP_SEEN
#define IBEOSDK_GPSIMU9001_HPP_SEEN

//======================================================================

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/Timestamp.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class GpsImu9001 : public ibeosdk::RegisteredDataBlock<GpsImu9001>
{
public:
	//! Sources that provide GPS/IMU measurements.
	enum Source
	{
		Source_CAN = 0,
		Source_XSensIMU = 1,
		Source_OxtsRt = 2,
		Source_GenesysADMA = 3,
		Source_SpatialDual = 4,
		Source_TFC = 5,
		Source_VBOX3i = 6,
		Source_Unknown = 99
	};

	//! Quality flags for the GPS/IMU measurements.
	enum INSQuality
	{
		INSQuality_GPS = 0,         //!< Only GPS (default). filtered output
		INSQuality_DGPSCourse = 1,  //!< Pre-RTK. filtered output
		INSQuality_DGPSPrecise = 2, //!< RTK precision. filtered output
		INSQuality_NoFix = 3,		//!< No GPS-Fix, Predicted position only
		INSQuality_GPSRaw = 4,		//!< Only GPS, raw, unfiltered output
		INSQuality_DGPSCourseRaw = 5, //!< Pre-RTK, raw, unfiltered output
		INSQuality_DGPSPreciseRaw = 6, //!< RTK precision, unfiltered output
		INSQuality_NotReliable = 7 //Sensoroutput is not reliable, sensor internal filter might not be initialized
	};

public:
	//! Constructor.
	GpsImu9001();

	//!< Destructor.
	virtual ~GpsImu9001();

public:
	//! Assignment operator.
	GpsImu9001& operator=(const GpsImu9001& other);

	//! Equal operator.
	bool operator==(const GpsImu9001& other) const;

	//! Not equal operator.
	bool operator!=(const GpsImu9001& other) const;

public:
	virtual DataTypeId getDataType() const;

	virtual std::streamsize getSerializedSize() const;

	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);

	virtual bool serialize(std::ostream& os) const;

public:
	/*!
	 * \brief Convert the source enum to a string.
	 * \param s the source type.
	 * \return the source type as a string.
	 */
	static std::string toString(const Source s);

	//! Set the latitude [rad].
	void setLatitude(const double latitude) { m_latitude = latitude; }

	//! Set the latitude uncertainty [rad].
	void setLatitudeSigma(const double latitudeSigma) { m_latitudeSigma = latitudeSigma; }

	//! Set the longitude [rad].
	void setLongitude(const double longitude) { m_longitude = longitude; }

	//! Set the longitude uncertainty [rad].
	void setLongitudeSigma(const double longitudeSigma) { m_longitudeSigma = longitudeSigma; }

	//! Set the altitude uncertainty [m].
	void setAltitude(const double altitude) { m_altitude = altitude; }

	//! Set the altitude uncertainty [m].
	void setAltitudeSigma(const double altitudeSigma) { m_altitudeSigma = altitudeSigma; }

	/*!
	 * \brief Set the course angle [rad].
	 *
	 * Relative to North. Interval: [0;2*Pi] The course angle is the angle the vehicle is traveling to. If you
	 * drift, it is different to the yaw angle, which is the direction of the vehicle is heading/looking at.
	 * Example: 0=North, Pi/2=West
	 */
	void setCourseAngle(const double courseAngle) { m_courseAngle = courseAngle; }

	//! Set the course angle uncertainty [rad].
	void setCourseAngleSigma(const double courseAngleSigma) { m_courseAngleSigma = courseAngleSigma; }

	/*!
	 * \brief setYawAngle
	 *
	 * Yaw Angle in [rad], Interval [0;2*Pi]. The yaw angle is the angle the vehicle is heading/looking at. If you
	 * drift, it is different to the course angle, which is the direction of travelling or the track angle. Example:
	 * 0=North, Pi/2=West
	 */
	void setYawAngle(const double yawAngle) { m_yawAngle = yawAngle; }

	//! Set the yaw angle uncertainty [rad].
	void setYawAngleSigma(const double yawAngleSigma) { m_yawAngleSigma = yawAngleSigma; }

	//! Set the pitch angle uncertainty [rad].
	void setPitchAngle(const double pitchAngle) { m_pitchAngle = pitchAngle; }

	//! Set the pitch angle uncertainty [rad].
	void setPitchAngleSigma(const double pitchAngleSigma) { m_pitchAngleSigma = pitchAngleSigma; }

	//! Set the roll angle [rad].
	void setRollAngle(const double rollAngle) { m_rollAngle = rollAngle; }

	//! Set the roll angle uncertainty [rad].
	void setRollAngleSigma(const double rollAngleSigma) { m_rollAngleSigma = rollAngleSigma; }

	//! Set the cross angle [m/s^2].
	void setCrossAcceleration(const double crossAcceleration) { m_crossAcceleration = crossAcceleration; }

	//! Set the cross angle uncertainty [m/s^2].
	void setCrossAccelerationSigma(const double crossAccelerationSigma)
	{
		m_crossAccelerationSigma = crossAccelerationSigma;
	}

	//! Set the longitudinal angle  [m/s^2].
	void setLongitudinalAcceleration(const double longitudinalAcceleration)
	{
		m_longitudinalAcceleration = longitudinalAcceleration;
	}

	//! Set the longitudinal angle uncertainty [m/s^2].
	void setLongitudinalAccelerationSigma(const double longitudinalAccelerationSigma)
	{
		m_longitudinalAccelerationSigma = longitudinalAccelerationSigma;
	}

	//! Set the vertical angle [m/s^2].
	void setVerticalAcceleration(const double verticalAcceleration) { m_verticalAcceleration = verticalAcceleration; }

	//! Set the vertical angle uncertainty [m/s^2].
	void setVerticalAccelerationSigma(const double verticalAccelerationSigma)
	{
		m_verticalAccelerationSigma = verticalAccelerationSigma;
	}

	//! Set the velocity north [m/s].
	void setVelocityNorth(const double velocityNorth) { m_velocityNorth = velocityNorth; }

	//! Set the velocity north uncertainty [m/s].
	void setmVelocityNorthSigma(const double velocityNorthSigma) { m_velocityNorthSigma = velocityNorthSigma; }

	//! Set the velocity west [m/s].
	void setVelocityWest(const double velocityWest) { m_velocityWest = velocityWest; }

	//! Set the velocity west uncertainty [m/s].
	void setVelocityWestSigma(const double velocityWestSigma) { m_velocityWestSigma = velocityWestSigma; }

	//! Set the velocity up [m/s].
	void setVelocityUp(const double velocityUp) { m_velocityUp = velocityUp; }

	//! Set the velocity up uncertainty [m/s].
	void setVelocityUpSigma(const double velocityUpSigma) { m_velocityUpSigma = velocityUpSigma; }

	//! Set the velocity in x direction [m/s].
	void setVelocityX(const double velocityX) { m_velocityX = velocityX; }

	//! Set the velocity uncertainty in x direction [m/s].
	void setVelocityXSigma(const double velocityXSigma) { m_velocityXSigma = velocityXSigma; }

	//! Set the velocity in y direction [m/s].
	void setVelocityY(const double velocityY) { m_velocityY = velocityY; }

	//! Set the velocity uncertainty in y direction [m/s].
	void setVelocityYSigma(const double velocityYSigma) { m_velocityYSigma = velocityYSigma; }

	//! Set the velocity in z direction [m/s].
	void setVelocityZ(const double velocityZ) { m_velocityZ = velocityZ; }

	//! Set the velocity uncertainty in z direction [m/s].
	void setVelocityZSigma(const double velocityZSigma) { m_velocityZSigma = velocityZSigma; }

	//! Set the roll rate [rad/s].
	void setRollRate(const double rollRate) { m_rollRate = rollRate; }

	//! Set the roll rate uncertainty [rad/s].
	void setRollRateSigma(const double rollRateSigma) { m_rollRateSigma = rollRateSigma; }

	//! Set the yaw rate [rad/s].
	void setYawRate(const double yawRate) { m_yawRate = yawRate; }

	//! Set the yaw rate uncertainty [rad/s].
	void setYawRateSigma(const double yawRateSigma) { m_yawRateSigma = yawRateSigma; }

	//! Set the pitch rate [rad/s].
	void setPitchRate(const double pitchRate) { m_pitchRate = pitchRate; }

	//! Set the pitch rate uncertainty [rad/s].
	void setPitchRateSigma(const double pitchRateSigma) { m_pitchRateSigma = pitchRateSigma; }

	//! Set the GPS status [none] (tbd).
	void setGpsStatus(const double gpsStatus) { m_gpsStatus = gpsStatus; }

	//! Set the number of satellites.
	void setNoOfSatellites(const UINT8 noOfSatellites) { m_noOfSatellites = noOfSatellites; }

	//! Set the dilution of precision in x direction.
	void setGpsDilutionOfPrecisionX(const double gpsDilutionOfPrecisionX)
	{
		m_gpsDilutionOfPrecisionX = gpsDilutionOfPrecisionX;
	}

	//! Set the dilution of precision in y direction.
	void setGpsDilutionOfPrecisionY(const double gpsDilutionOfPrecisionY)
	{
		m_gpsDilutionOfPrecisionY = gpsDilutionOfPrecisionY;
	}

	//! Set the dilution of precision in z direction.
	void setGpsDilutionOfPrecisionZ(const double gpsDilutionOfPrecisionZ)
	{
		m_gpsDilutionOfPrecisionZ = gpsDilutionOfPrecisionZ;
	}

	//! Set the dilution of precision in vertical direction.
	void setGpsDilutionOfPrecisionVertical(const double gpsDilutionOfPrecisionVertical)
	{
		m_gpsDilutionOfPrecisionVertical = gpsDilutionOfPrecisionVertical;
	}

	//! Set the dilution of precision in position.
	void setGpsDilutionOfPrecisionPosition(const double gpsDilutionOfPrecisionPosition)
	{
		m_gpsDilutionOfPrecisionPosition = gpsDilutionOfPrecisionPosition;
	}

	//! Set the timestamp.
	void setTimestamp(const Timestamp& timestamp) { m_timestamp = timestamp; }

	//! Set the source of the GPS/IMU measurements.
	void setSource(const Source source) { m_source = source; }

	//! Get the quality flag of the GPS/IMU measurements.
	void setINSQuality(const INSQuality insQuality) { m_insQuality = insQuality; }

	//! Get the latitude [rad].
	double getLatitude() const { return m_latitude; }

	//! Get the latitude uncertainty [rad].
	double getLatitudeSigma() const { return m_latitudeSigma; }

	//! Get the longitude [rad].
	double getLongitude() const { return m_longitude; }

	//! Get the longitude uncertainty [rad].
	double getLongitudeSigma() const { return m_longitudeSigma; }

	//! Get the altitude uncertainty [m].
	double getAltitude() const { return m_altitude; }

	//! Get the altitude uncertainty [m].
	double getAltitudeSigma() const { return m_altitudeSigma; }

	/*!
	 * \brief Get the course angle [rad].
	 *
	 * Relative to North. Interval: [0;2*Pi] The course angle is the angle the vehicle is traveling to. If you
	 * drift, it is different to the yaw angle, which is the direction of the vehicle is heading/looking at.
	 * Example: 0=North, Pi/2=West
	 */
	double getCourseAngle() const { return m_courseAngle; }

	//! Get the course angle uncertainty [rad].
	double getCourseAngleSigma() const { return m_courseAngleSigma; }

	/*!
	 * \brief setYawAngle
	 *
	 * Yaw Angle in [rad], Interval [0;2*Pi]. The yaw angle is the angle the vehicle is heading/looking at. If you
	 * drift, it is different to the course angle, which is the direction of travelling or the track angle. Example:
	 * 0=North, Pi/2=West
	 */
	double getYawAngle() const { return m_yawAngle; }

	//! Set the yaw angle uncertainty [rad].
	double getYawAngleSigma() const { return m_yawAngleSigma; }

	//! Get the pitch angle uncertainty [rad].
	double getPitchAngle() const { return m_pitchAngle; }

	//! Get the pitch angle uncertainty [rad].
	double getPitchAngleSigma() const { return m_pitchAngleSigma; }

	//! Get the roll angle [rad].
	double getRollAngle() const { return m_rollAngle; }

	//! Get the roll angle uncertainty [rad].
	double getRollAngleSigma() const { return m_rollAngleSigma; }

	//! Get the cross angle [m/s^2].
	double getCrossAcceleration() const { return m_crossAcceleration; }

	//! Get the cross angle uncertainty [m/s^2].
	double getCrossAccelerationSigma() const { return m_crossAccelerationSigma; }

	//! Get the longitudinal angle  [m/s^2].
	double getLongitudinalAcceleration() const { return m_longitudinalAcceleration; }

	//! Get the longitudinal angle uncertainty [m/s^2].
	double getLongitudinalAccelerationSigma() const { return m_longitudinalAccelerationSigma; }

	//! Get the vertical angle [m/s^2].
	double getVerticalAcceleration() const { return m_verticalAcceleration; }

	//! Get the vertical angle uncertainty [m/s^2].
	double getVerticalAccelerationSigma() const { return m_verticalAccelerationSigma; }

	//! Get the velocity north [m/s].
	double getVelocityNorth() const { return m_velocityNorth; }

	//! Get the velocity north uncertainty [m/s].
	double getmVelocityNorthSigma() const { return m_velocityNorthSigma; }

	//! Get the velocity west [m/s].
	double getVelocityWest() const { return m_velocityWest; }

	//! Get the velocity west uncertainty [m/s].
	double getVelocityWestSigma() const { return m_velocityWestSigma; }

	//! Get the velocity up [m/s].
	double getVelocityUp() const { return m_velocityUp; }

	//! Get the velocity up uncertainty [m/s].
	double getVelocityUpSigma() const { return m_velocityUpSigma; }

	//! Get the velocity in x direction [m/s].
	double getVelocityX() const { return m_velocityX; }

	//! Get the velocity uncertainty in x direction [m/s].
	double getVelocityXSigma() const { return m_velocityXSigma; }

	//! Get the velocity in y direction [m/s].
	double getVelocityY() const { return m_velocityY; }

	//! Get the velocity uncertainty in y direction [m/s].
	double getVelocityYSigma() const { return m_velocityYSigma; }

	//! Get the velocity in z direction [m/s].
	double getVelocityZ() const { return m_velocityZ; }

	//! Get the velocity uncertainty in z direction [m/s].
	double getVelocityZSigma() const { return m_velocityZSigma; }

	//! Get the roll rate [rad/s].
	double getYawRate() const { return m_yawRate; }

	//! Get the roll rate uncertainty [rad/s].
	double getYawRateSigma() const { return m_yawRateSigma; }

	//! Get the yaw rate [rad/s].
	double getPitchRate() const { return m_pitchRate; }

	//! Get the yaw rate uncertainty [rad/s].
	double getPitchRateSigma() const { return m_pitchRateSigma; }

	//! Get the pitch rate [rad/s].
	double getRollRate() const { return m_rollRate; }

	//! Get the pitch rate uncertainty [rad/s].
	double getRollRateSigma() const { return m_rollRateSigma; }

	//! Get the GPS status [none] (tbd).
	double getGpsStatus() const { return m_gpsStatus; }

	//! Get the number of satellites.
	UINT8 getNoOfSatellites() const { return m_noOfSatellites; }

	//! Get the dilution of precision in x direction.
	double getGpsDilutionOfPrecisionX() const { return m_gpsDilutionOfPrecisionX; }

	//! Get the dilution of precision in y direction.
	double getGpsDilutionOfPrecisionY() const { return m_gpsDilutionOfPrecisionY; }

	//! Get the dilution of precision in z direction.
	double getGpsDilutionOfPrecisionZ() const { return m_gpsDilutionOfPrecisionZ; }

	//! Get the dilution of precision in vertical direction.
	double getGpsDilutionOfPrecisionVertical() const { return m_gpsDilutionOfPrecisionVertical; }

	//! Get the dilution of precision in position.
	double getGpsDilutionOfPrecisionPosition() const { return m_gpsDilutionOfPrecisionPosition; }

	//! Get timestamp.
	const Timestamp& getTimestamp() const { return m_timestamp; }

	//! Get the source of the GPS/IMU measurements.
	const Source& getSource() const { return m_source; }

	//! Get the quality flag of the GPS/IMU measurements.
	const INSQuality& getINSQuality() const { return m_insQuality; }

public:
	//! Number of reserved bytes.
	static const int nbOfReserved = 16;

private:
	//                                             Unit            Serialized size
	double m_latitude;                          // [rad]           + 8 Bytes
	double m_latitudeSigma;                     // [m]             + 8 Bytes
	double m_longitude;                         // [rad]           + 8 Bytes
	double m_longitudeSigma;                    // [m]             + 8 Bytes
	double m_altitude;                          // [m]             + 8 Bytes
	double m_altitudeSigma;                     // [m]             + 8 Bytes
	double m_courseAngle;                       // [rad]           + 8 Bytes
	double m_courseAngleSigma;                  // [rad]           + 8 Bytes
	double m_yawAngle;                          // [rad]           + 8 Bytes
	double m_yawAngleSigma;                     // [rad]           + 8 Bytes
	double m_pitchAngle;                        // [rad]           + 8 Bytes
	double m_pitchAngleSigma;                   // [rad]           + 8 Bytes
	double m_rollAngle;                         // [rad]           + 8 Bytes
	double m_rollAngleSigma;                    // [rad]           + 8 Bytes

	double m_crossAcceleration;                 // [ms/s^2]        + 8 Bytes
	double m_crossAccelerationSigma;            // [ms/s^2]        + 8 Bytes
	double m_longitudinalAcceleration;          // [ms/s^2]        + 8 Bytes
	double m_longitudinalAccelerationSigma;     // [ms/s^2]        + 8 Bytes
	double m_verticalAcceleration;              // [ms/s^2]        + 8 Bytes
	double m_verticalAccelerationSigma;         // [ms/s^2]        + 8 Bytes
	double m_velocityNorth;                     // [ms/s]          + 8 Bytes
	double m_velocityNorthSigma;                // [ms/s]          + 8 Bytes
	double m_velocityWest;                      // [ms/s]          + 8 Bytes
	double m_velocityWestSigma;                 // [ms/s]          + 8 Bytes
	double m_velocityUp;                        // [ms/s]          + 8 Bytes
	double m_velocityUpSigma;                   // [ms/s]          + 8 Bytes
	double m_velocityX;                         // [ms/s]          + 8 Bytes
	double m_velocityXSigma;                    // [ms/s]          + 8 Bytes
	double m_velocityY;                         // [ms/s]          + 8 Bytes
	double m_velocityYSigma;                    // [ms/s]          + 8 Bytes
	double m_velocityZ;                         // [ms/s]          + 8 Bytes
	double m_velocityZSigma;                    // [ms/s]          + 8 Bytes

	double m_rollRate;                          // [rad/s]         + 8 Bytes
	double m_rollRateSigma;                     // [rad/s]         + 8 Bytes
	double m_yawRate;                           // [rad/s]         + 8 Bytes
	double m_yawRateSigma;                      // [rad/s]         + 8 Bytes
	double m_pitchRate;                         // [rad/s]         + 8 Bytes
	double m_pitchRateSigma;                    // [rad/s]         + 8 Bytes

	double m_gpsStatus;                         // [none] (tbd)    + 8 Bytes
	UINT8 m_noOfSatellites;                     // [none]          + 1 Bytes

	double m_gpsDilutionOfPrecisionX;           // [none]          + 8 Bytes
	double m_gpsDilutionOfPrecisionY;           // [none]          + 8 Bytes
	double m_gpsDilutionOfPrecisionZ;           // [none]          + 8 Bytes
	double m_gpsDilutionOfPrecisionVertical;    // [none]          + 8 Bytes
	double m_gpsDilutionOfPrecisionPosition;    // [none]          + 8 Bytes

	Timestamp m_timestamp;                      // [none]          + 42 Bytes

	Source m_source;                            // [none]          + 1 Bytes (encoded as UINT8)
	INSQuality m_insQuality;                    // [none]          + 1 Bytes (encoded as UINT8)

	UINT8 m_reserved[nbOfReserved];             // [none]          + 16 Bytes
	//                                                             = 413 Bytes
}; // GpsImu9001

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_GPSIMU9001_HPP_SEEN

//======================================================================
