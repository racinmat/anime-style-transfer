//======================================================================
/*! \file MountingPositionRaw.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Aug 5, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_MOUNTINGPOSITIONRAW_HPP_SEEN
#define IBEOSDK_MOUNTINGPOSITIONRAW_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <istream>
#include <ostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class MountingPositionRaw : public Snippet {
public:
	//========================================
	/*!\brief Default construtor.
	 *
	 * All values are initialized to zero.
	 *///-------------------------------------
	MountingPositionRaw();
	//========================================

	/*!\brief Constructor with all values given.
	 *
	 * \param[in] yaw    Yaw angle of the mounting position [AngleTicks].
	 * \param[in] pitch  Pitch angle of the mounting position [AngleTicks].
	 * \param[in] roll   Roll angle of the mounting position [AngleTicks].
	 * \param[in] x      X position of the mounting position [cm].
	 * \param[in] y      Y position of the mounting position [cm].
	 * \param[in] z      Z position of the mounting position [cm].
	 *
	 * \note (Note: The angles are not yet normalized into the interval
	 * [-pi,pi].  If you require normalized angles here, you must call
	 * normalizeAngles() yourself.)
	 *///-------------------------------------
	MountingPositionRaw(const int16_t yaw, const int16_t pitch, const int16_t roll,
	                     const int16_t x, const int16_t y, const int16_t z);

public:
	//========================================
	/*!\brief Equality predicate
	 * \param[in] other  The other mounting position to be
	 *                   compared with this object.
	 * \return \c true if both MountingPosition objects are
	 *         equal, \c false otherwise.
	 *///-------------------------------------
	bool operator==(const MountingPositionRaw& other) const;

	//========================================
	/*!\brief NotEquality predicate
	 * \param[in] other  The other mounting position to be
	 *                   compared with this object.
	 * \return \c true if both MountingPosition objects are
	 *         equal, \c false otherwise.
	 *///-------------------------------------
	bool operator!=(const MountingPositionRaw& other) const;

public:
	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization
	 *         of this data class.
	 *///-------------------------------------
	static std::streamsize getSerializedSize_static() { return 6 * std::streamsize(sizeof(int16_t)); }

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
	 * \return Yaw angle of the mounting position given in AngleTicks.
	 *///-------------------------------------
	int16_t getYawAngle() const { return m_yawAngle; }

	//========================================
	/*!\brief Get the pitch angle of the mounting position.
	 * \return Pitch angle of the mounting position given in AngleTicks.
	 *///-------------------------------------
	int16_t getPitchAngle() const { return m_pitchAngle; }

	//========================================
	/*!\brief Get the roll angle of the mounting position.
	 * \return Roll angle of the mounting position given in AngleTicks.
	 *///-------------------------------------
	int16_t getRollAngle() const { return m_rollAngle; }

	//========================================
	/*!\brief Get the x position of the mounting position.
	 * \return X position of the mounting position given in cm.
	 *///-------------------------------------
	int16_t getX() const { return m_posX; }

	//========================================
	/*!\brief Get the y position of the mounting position.
	 * \return Y position of the mounting position given in cm.
	 *///-------------------------------------
	int16_t getY() const { return m_posY; }
	//========================================
	/*!\brief Get the z position of the mounting position.
	 * \return Z position of the mounting position given in cm.
	 *///-------------------------------------
	int16_t getZ() const { return m_posZ; }

public:
	//========================================
	/*!\brief Set a new yaw angle \a y.
	 * \param[in] y  The new yaw angle given in AngleTicks.
	 *///-------------------------------------
	void setYawAngle(const int16_t y) { m_yawAngle = y; }

	//========================================
	/*!\brief Set a new pitch angle \a p.
	 * \param[in] p  The new pitch angle given in AngleTicks.
	 *///-------------------------------------
	void setPitchAngle(const int16_t p) {m_pitchAngle = p; }

	//========================================
	/*!\brief Set a new roll angle \a r.
	 * \param[in] r  The new roll angle given in AngleTicks.
	 *///-------------------------------------
	void setRollAngle(const int16_t r) { m_rollAngle = r; }

	//========================================
	/*!\brief Set a new x position \a x.
	 * \param[in] x  The new x position given in cm.
	 *///-------------------------------------
	void setX(const int16_t x) { m_posX = x; }

	//========================================
	/*!\brief Set a new x position \a y.
	 * \param[in] y  The new y position given in cm.
	 *///-------------------------------------

	void setY(const int16_t y) { m_posY = y; }
	//========================================
	/*!\brief Set a new x position \a z.
	 * \param[in] z  The new z position given in cm.
	 *///-------------------------------------
	void setZ(const int16_t z) { m_posZ  =z; }

	//========================================
	/*!\brief Set all values for a new mounting position.
	 *
	 * \param[in] yaw    Yaw angle of the mounting position [AngleTicks].
	 * \param[in] pitch  Pitch angle of the mounting position [AngleTicks].
	 * \param[in] roll   Roll angle of the mounting position [AngleTicks].
	 * \param[in] x      X position of the mounting position [cm].
	 * \param[in] y      Y position of the mounting position [cm].
	 * \param[in] z      Z position of the mounting position [cm].
	 *
	 * \return A reference to this object after the new values
	 *         have been set.
	 *///-------------------------------------
	void set(const int16_t yaw, const int16_t pitch, const int16_t roll,
	         const int16_t x, const int16_t y, const int16_t z);

protected:
	//! Yaw angle [AngleTicks] of the sensor in the vehicle coordinate system
	int16_t m_yawAngle;
	//! Pitch angle [AngleTicks] of the sensor in the vehicle coordinate system
	int16_t m_pitchAngle;
	//! Roll angle [AngleTicks] of the sensor in the vehicle coordinate system
	int16_t m_rollAngle;

	//! x-coordinate [cm] of the sensor in the vehicle coordinate system
	int16_t m_posX;
	//! y-coordinate [cm] of the sensor in the vehicle coordinate system
	int16_t m_posY;
	//! z-coordinate [cm] of the sensor in the vehicle coordinate system
	int16_t m_posZ;
}; // MountingPositionRaw

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_MOUNTINGPOSITIONRAW_HPP_SEEN

//======================================================================

