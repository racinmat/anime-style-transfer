//======================================================================
/*! \file MountingPosition.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 29, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/MountingPosition.hpp>
#include <ibeosdk/Math.hpp>
#include <ibeosdk/io.hpp>

#include <cassert>
#include <cmath>

//======================================================================

namespace ibeosdk {

//======================================================================

MountingPosition::MountingPosition()
  : m_yawAngle(0), m_pitchAngle(0), m_rollAngle(0),
	m_posX(0), m_posY(0), m_posZ(0)
{}


//======================================================================

MountingPosition::MountingPosition(const float yaw, const float pitch, const float roll,
                                   const float x, const float y, const float z)
  : m_yawAngle(yaw), m_pitchAngle(pitch), m_rollAngle(roll),
	m_posX(x), m_posY(y), m_posZ(z)
{}

//======================================================================

bool MountingPosition::operator==(const MountingPosition& other) const
{
	bool ret =
		ibeosdk::fuzzyCompareT<6>(m_yawAngle, other.m_yawAngle)
		&& ibeosdk::fuzzyCompareT<6>(m_pitchAngle, other.m_pitchAngle)
		&& ibeosdk::fuzzyCompareT<6>(m_rollAngle, other.m_rollAngle)
		&& m_posX == other.m_posX
		&& m_posY == other.m_posY
		&& m_posZ == other.m_posZ
		;

	return ret;
}

//======================================================================

MountingPosition& MountingPosition::set(const float yaw, const float pitch, const float roll,
                                        const float x, const float y, const float z)
{
	m_yawAngle = yaw;
	m_pitchAngle = pitch;
	m_rollAngle = roll;
	m_posX = x;
	m_posY = y;
	m_posZ = z;

	return *this;
}

//======================================================================

bool MountingPosition::deserialize(std::istream &is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_yawAngle);
	ibeosdk::readBE(is, m_pitchAngle);
	ibeosdk::readBE(is, m_rollAngle);
	ibeosdk::readBE(is, m_posX);
	ibeosdk::readBE(is, m_posY);
	ibeosdk::readBE(is, m_posZ);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool MountingPosition::serialize(std::ostream &os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_yawAngle);
	ibeosdk::writeBE(os, m_pitchAngle);
	ibeosdk::writeBE(os, m_rollAngle);
	ibeosdk::writeBE(os, m_posX);
	ibeosdk::writeBE(os, m_posY);
	ibeosdk::writeBE(os, m_posZ);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

void MountingPosition::normalizeAngles()
{
	m_yawAngle = ibeosdk::normalizeRadians(m_yawAngle);
	m_pitchAngle = ibeosdk::normalizeRadians(m_pitchAngle);
	m_rollAngle = ibeosdk::normalizeRadians(m_rollAngle);
}

//======================================================================

} // namespace ibeosdk

//======================================================================
