//======================================================================
/*! \file MountingPositionRaw.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Aug 5, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/MountingPositionRaw.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/io.hpp>

#include <cmath>

//======================================================================

namespace ibeosdk {

//======================================================================

///// Tests if two \c float values are nearly equal
///**
// * \return \c true if the two \c float numbers are equal in
// * terms of the machine precision, which means their difference
// * must be less than 1E-6.
// */
//inline bool fuzzyCompare(float a, float b)
//{
//	return std::abs(a - b) < 1E-6f;
//}

//======================================================================

MountingPositionRaw::MountingPositionRaw()
  : m_yawAngle(0), m_pitchAngle(0), m_rollAngle(0),
    m_posX(0), m_posY(0), m_posZ(0)
{}

//======================================================================

MountingPositionRaw::MountingPositionRaw(const int16_t yaw, const int16_t pitch, const int16_t roll,
                                           const int16_t x, const int16_t y, const int16_t z)
  : m_yawAngle(yaw), m_pitchAngle(pitch), m_rollAngle(roll),
    m_posX(x), m_posY(y), m_posZ(z)
{}

//======================================================================

bool MountingPositionRaw::operator==(const MountingPositionRaw& other) const
{
	return
		m_yawAngle == other.m_yawAngle
		&& m_pitchAngle == other.m_pitchAngle
		&& m_rollAngle == other.m_rollAngle
		&& m_posX == other.m_posX
		&& m_posY == other.m_posY
		&& m_posZ == other.m_posZ;
}

//======================================================================

bool MountingPositionRaw::operator!=(const MountingPositionRaw& other) const
{
	return
		m_yawAngle != other.m_yawAngle
		|| m_pitchAngle != other.m_pitchAngle
		|| m_rollAngle != other.m_rollAngle
		|| m_posX != other.m_posX
		|| m_posY != other.m_posY
		|| m_posZ != other.m_posZ;
}

//======================================================================

bool MountingPositionRaw::deserialize(std::istream &is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readBE(is, m_yawAngle);
	readBE(is, m_pitchAngle);
	readBE(is, m_rollAngle);
	readBE(is, m_posX);
	readBE(is, m_posY);
	readBE(is, m_posZ);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool MountingPositionRaw::serialize(std::ostream &os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, m_yawAngle);
	writeBE(os, m_pitchAngle);
	writeBE(os, m_rollAngle);
	writeBE(os, m_posX);
	writeBE(os, m_posY);
	writeBE(os, m_posZ);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

void MountingPositionRaw::set(const int16_t yaw, const int16_t pitch, const int16_t roll,
                              const int16_t x, const int16_t y, const int16_t z)
{
	m_yawAngle = yaw;
	m_pitchAngle = pitch;
	m_rollAngle = roll;
	m_posX = x;
	m_posY = y;
	m_posZ = z;
}


//======================================================================

} // namespace ibeosdk

//======================================================================
