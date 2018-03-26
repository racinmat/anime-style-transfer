//======================================================================
/*! \file ReferencePlane.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 15, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef REFERENCEPLANE_HPP_SEEN
#define REFERENCEPLANE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/datablocks/snippets/GpsPoint.hpp>
#include <ibeosdk/datablocks/VehicleStateBasicEcu2808.hpp>

#include <ibeosdk/RotationMatrix3dFloat.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ReferencePlane: public Snippet {
public:
	ReferencePlane()
	  : m_gpsPoint(),
	    m_yaw(0.f),
	    m_pitch(0.f),
	    m_roll(0.f),
	    m_rotationMatrixIsValid(false),
	    m_rotationMatrix()
	{}

	ReferencePlane(const GpsPoint& point,
	               const float yaw = 0.f,
	               const float pitch = 0.f,
	               const float roll = 0.f)
	  : m_gpsPoint(point),
	    m_yaw(yaw),
	    m_pitch(pitch),
	    m_roll(roll),
	    m_rotationMatrixIsValid(false),
	    m_rotationMatrix()
	{}

	ReferencePlane(const VehicleStateBasicEcu2808& vsb);

public:
	bool operator==(const ReferencePlane& other) const;
	bool operator!=(const ReferencePlane& other) const;

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	GpsPoint& gpsPoint() { return m_gpsPoint; }
	const GpsPoint& getGpsPoint() const { return m_gpsPoint; }
	void setGpsPoint(const GpsPoint& point) { m_gpsPoint = point; }

	float getYaw() const { return m_yaw; }
	void setYaw(const float yaw) { m_rotationMatrixIsValid = false; m_yaw = yaw; }

	float getPitch() const { return m_pitch; }
	void setPitch(const float pitch) { m_rotationMatrixIsValid = false; m_pitch = pitch; }

	float getRoll() const { return m_roll; }
	void setRoll(const float roll) { m_rotationMatrixIsValid = false; m_roll = roll; }

	const RotationMatrix3dFloat& getRotationMatrix() const
	{
		if (!m_rotationMatrixIsValid) {
			m_rotationMatrix.setToUnity();
			m_rotationMatrix.rotMatZ(m_yaw);
			m_rotationMatrix.rotMatY(m_pitch);
			m_rotationMatrix.rotMatX(m_roll);
		}
		return m_rotationMatrix;
	}


	//========================================
private:
	GpsPoint m_gpsPoint;

	float m_yaw;
	float m_pitch;
	float m_roll;

private: // rotation matrix cache
	bool m_rotationMatrixIsValid;
	mutable RotationMatrix3dFloat m_rotationMatrix;
}; // ReferencePlane

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // REFERENCEPLANE_HPP_SEEN

//======================================================================
