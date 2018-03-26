//======================================================================
/*! \file MountingPosition.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 29, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_MOUNTINGPOSITION_HPP_SEEN
#define IBEOSDK_MOUNTINGPOSITION_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class MountingPosition
 * \brief The mounting position of a Device like Laserscanner or Camera
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Sep 29, 2013
 *
 *///-------------------------------------------------------------------
class MountingPosition : public Snippet {
public:
	//========================================
	/*!\brief Default construtor.
	 *
	 * All values are initialized to zero.
	 *///-------------------------------------
	MountingPosition();

	//========================================
	/*!\brief Constructor with all values given.
	 *
	 * \param[in] yaw    Yaw angle of the mounting position.
	 * \param[in] pitch  Pitch angle of the mounting position.
	 * \param[in] roll   Roll angle of the mounting position.
	 * \param[in] x      X position of the mounting position.
	 * \param[in] y      Y position of the mounting position.
	 * \param[in] z      Z position of the mounting position.
	 *
	 * \note (Note: The angles are not yet normalized into the interval
	 * [-pi,pi].  If you require normalized angles here, you must call
	 * normalizeAngles() yourself.)
	 *///-------------------------------------
	MountingPosition(const float yaw, const float pitch, const float roll,
	                 const float x, const float y, const float z);

	//========================================
	/*!\brief Equality predicate
	 * \param[in] other  The other mounting position to be
	 *                   compared with this object.
	 * \return \c true if both MountingPosition objects are
	 *         equal, \c false otherwise.
	 *///-------------------------------------
	bool operator==(const MountingPosition& other) const;

public:
	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization
	 *         of this data class.
	 *///-------------------------------------
	static std::streamsize getSerializedSize_static() { return 6 * std::streamsize(sizeof(float)); }

public:
	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization
	 *         of this data class.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }

	//========================================
	/*!\brief Deserialize data from the given stream \a is into
	 *        this class.
	 * \param[in, out] is  Stream that provides the serialized
	 *                     data to fill this class.
	 *                     On exit the \a is get pointer will
	 *                     be behind the read data.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Reading the data from the stream has failed.
	 *///-------------------------------------
	virtual bool deserialize(std::istream& is);

	//========================================
	/*!\brief Serialize data into the given stream \a os.
	 * \param[out] os  Stream that receive the serialized
	 *                 data from this class.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const;

public:
	//========================================
	/*!\brief Get the yaw angle of the mounting position.
	 * \return Yaw angle of the mounting position given in rad.
	 *///-------------------------------------
	float getYawAngle() const { return m_yawAngle; }

	//========================================
	/*!\brief Get the pitch angle of the mounting position.
	 * \return Pitch angle of the mounting position given in rad.
	 *///-------------------------------------
	float getPitchAngle() const { return m_pitchAngle; }

	//========================================
	/*!\brief Get the roll angle of the mounting position.
	 * \return Roll angle of the mounting position given in rad.
	 *///-------------------------------------
	float getRollAngle() const { return m_rollAngle; }

	//========================================
	/*!\brief Get the x position of the mounting position.
	 * \return X position of the mounting position given in meter.
	 *///-------------------------------------
	float getX() const { return m_posX; }

	//========================================
	/*!\brief Get the y position of the mounting position.
	 * \return Y position of the mounting position given in meter.
	 *///-------------------------------------
	float getY() const { return m_posY; }

	//========================================
	/*!\brief Get the z position of the mounting position.
	 * \return Z position of the mounting position given in meter.
	 *///-------------------------------------
	float getZ() const { return m_posZ; }

public:
	//========================================
	/*!\brief Set a new yaw angle \a y.
	 * \param[in] y  The new yaw angle given in rad.
	 *///-------------------------------------
	void setYawAngle(const float y) { m_yawAngle = y; }

	//========================================
	/*!\brief Set a new pitch angle \a p.
	 * \param[in] p  The new pitch angle given in rad.
	 *///-------------------------------------
	void setPitchAngle(const float p) {m_pitchAngle = p; }

	//========================================
	/*!\brief Set a new roll angle \a r.
	 * \param[in] r  The new roll angle given in rad.
	 *///-------------------------------------
	void setRollAngle(const float r) { m_rollAngle = r; }

	//========================================
	/*!\brief Set a new x position \a x.
	 * \param[in] x  The new x position given in meter.
	 *///-------------------------------------
	void setX(const float x) { m_posX = x; }

	//========================================
	/*!\brief Set a new x position \a y.
	 * \param[in] y  The new y position given in meter.
	 *///-------------------------------------
	void setY(const float y) { m_posY = y; }

	//========================================
	/*!\brief Set a new x position \a z.
	 * \param[in] z  The new z position given in meter.
	 *///-------------------------------------
	void setZ(const float z) { m_posZ  =z; }

public:
	//========================================
	/*!\brief Set all values for a new mounting position.
	 *
	 * \param[in] yaw    Yaw angle of the mounting position.
	 * \param[in] pitch  Pitch angle of the mounting position.
	 * \param[in] roll   Roll angle of the mounting position.
	 * \param[in] x      X position of the mounting position.
	 * \param[in] y      Y position of the mounting position.
	 * \param[in] z      Z position of the mounting position.
	 *
	 * \return A reference to this object after the new values
	 *         have been set.
	 *
	 * \note The angles are not yet normalized into the interval
	 *       [-pi,pi].  If you require normalized angles here, you
	 *       must call normalizeAngles() yourself.
	 *///-------------------------------------
	MountingPosition& set(const float yaw, const float pitch, const float roll,
	                      const float x, const float y, const float z);

	//========================================
	/*!\brief Normalize all three internal angles to the range [-PI..PI]
	 *///-------------------------------------
	void normalizeAngles();

private:
	//! Yaw angle [rad] of the sensor in the vehicle coordinate system
	float m_yawAngle;
	//! Pitch angle [rad] of the sensor in the vehicle coordinate system
	float m_pitchAngle;
	//! Roll angle [rad] of the sensor in the vehicle coordinate system
	float m_rollAngle;

	//! x-coordinate [m] of the sensor in the vehicle coordinate system
	float m_posX;
	//! y-coordinate [m] of the sensor in the vehicle coordinate system
	float m_posY;
	//! z-coordinate [m] of the sensor in the vehicle coordinate system
	float m_posZ;
}; // MountingPosition

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_MOUNTINGPOSITION_HPP_SEEN
