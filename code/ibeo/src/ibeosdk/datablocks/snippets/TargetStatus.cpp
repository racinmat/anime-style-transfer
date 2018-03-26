//======================================================================
/*! \file TargetStatus.cpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Jun 30, 2014
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/TargetStatus.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

TargetStatus::TargetStatus()
  : m_ipAddress(0),
    m_status(0),
    m_visibilityLaser(0),
    m_occlusionLaser(0),
    m_visibilityDut(0),
    m_occlusionDut(0),
    m_paramImuYaw(0),
    m_paramImuPitch(0),
    m_paramImuRoll(0),
    m_paramImuYawAccuracy(0),
    m_paramImuPitchAccuracy(0),
    m_paramImuRollAccuracy(0),
    m_paramGpsX(0),
    m_paramGpsY(0),
    m_paramGpsZ(0),
    m_paramGpsXAccuracy(0),
    m_paramGpsYAccuracy(0),
    m_paramGpsZAccuracy(0)
{}

//======================================================================

TargetStatus::TargetStatus(const TargetStatus& other)
  : m_ipAddress(other.m_ipAddress),
    m_status(other.m_status),
    m_visibilityLaser(other.m_visibilityLaser),
    m_occlusionLaser(other.m_occlusionLaser),
    m_visibilityDut(other.m_visibilityDut),
    m_occlusionDut(other.m_occlusionDut),
    m_paramImuYaw(other.m_paramImuYaw),
    m_paramImuPitch(other.m_paramImuPitch),
    m_paramImuRoll(other.m_paramImuRoll),
    m_paramImuYawAccuracy(other.m_paramImuYawAccuracy),
    m_paramImuPitchAccuracy(other.m_paramImuPitchAccuracy),
    m_paramImuRollAccuracy(other.m_paramImuRollAccuracy),
    m_paramGpsX(other.m_paramGpsX),
    m_paramGpsY(other.m_paramGpsY),
    m_paramGpsZ(other.m_paramGpsZ),
    m_paramGpsXAccuracy(other.m_paramGpsXAccuracy),
    m_paramGpsYAccuracy(other.m_paramGpsYAccuracy),
    m_paramGpsZAccuracy(other.m_paramGpsZAccuracy)
{}

//======================================================================

TargetStatus& TargetStatus::operator=(const TargetStatus& other)
{
	if (this != &other) {
		m_ipAddress = other.m_ipAddress;
		m_status = other.m_status;
		m_visibilityLaser = other.m_visibilityLaser;
		m_occlusionLaser = other.m_occlusionLaser;
		m_visibilityDut = other.m_visibilityDut;
		m_occlusionDut = other.m_occlusionDut;
		m_paramImuYaw = other.m_paramImuYaw;
		m_paramImuPitch = other.m_paramImuPitch;
		m_paramImuRoll = other.m_paramImuRoll;
		m_paramImuYawAccuracy = other.m_paramImuYawAccuracy;
		m_paramImuPitchAccuracy = other.m_paramImuPitchAccuracy;
		m_paramImuRollAccuracy = other.m_paramImuRollAccuracy;
		m_paramGpsX = other.m_paramGpsX;
		m_paramGpsY = other.m_paramGpsY;
		m_paramGpsZ = other.m_paramGpsZ;
		m_paramGpsXAccuracy = other.m_paramGpsXAccuracy;
		m_paramGpsYAccuracy = other.m_paramGpsYAccuracy;
		m_paramGpsZAccuracy = other.m_paramGpsZAccuracy;
	}

	return *this;
}

//======================================================================

TargetStatus::~TargetStatus() {}

//======================================================================

bool TargetStatus::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_ipAddress);
	ibeosdk::readBE(is, m_status);
	ibeosdk::readBE(is, m_visibilityLaser);
	ibeosdk::readBE(is, m_occlusionLaser);
	ibeosdk::readBE(is, m_visibilityDut);
	ibeosdk::readBE(is, m_occlusionDut);
	ibeosdk::readBE(is, m_paramImuYaw);
	ibeosdk::readBE(is, m_paramImuPitch);
	ibeosdk::readBE(is, m_paramImuRoll);
	ibeosdk::readBE(is, m_paramImuYawAccuracy);
	ibeosdk::readBE(is, m_paramImuPitchAccuracy);
	ibeosdk::readBE(is, m_paramImuRollAccuracy);
	ibeosdk::readBE(is, m_paramGpsX);
	ibeosdk::readBE(is, m_paramGpsY);
	ibeosdk::readBE(is, m_paramGpsZ);
	ibeosdk::readBE(is, m_paramGpsXAccuracy);
	ibeosdk::readBE(is, m_paramGpsYAccuracy);
	ibeosdk::readBE(is, m_paramGpsZAccuracy);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}
//======================================================================

//static
std::streamsize TargetStatus::getSerializedSize_static()
{
	return std::streamsize(sizeof(UINT32)
	                     + sizeof(UINT8)
	                     + (sizeof(float) * 16));
}

//======================================================================

bool TargetStatus::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_ipAddress);
	ibeosdk::writeBE(os, m_status);
	ibeosdk::writeBE(os, m_visibilityLaser);
	ibeosdk::writeBE(os, m_occlusionLaser);
	ibeosdk::writeBE(os, m_visibilityDut);
	ibeosdk::writeBE(os, m_occlusionDut);
	ibeosdk::writeBE(os, m_paramImuYaw);
	ibeosdk::writeBE(os, m_paramImuPitch);
	ibeosdk::writeBE(os, m_paramImuRoll);
	ibeosdk::writeBE(os, m_paramImuYawAccuracy);
	ibeosdk::writeBE(os, m_paramImuPitchAccuracy);
	ibeosdk::writeBE(os, m_paramImuRollAccuracy);
	ibeosdk::writeBE(os, m_paramGpsX);
	ibeosdk::writeBE(os, m_paramGpsY);
	ibeosdk::writeBE(os, m_paramGpsZ);
	ibeosdk::writeBE(os, m_paramGpsXAccuracy);
	ibeosdk::writeBE(os, m_paramGpsYAccuracy);
	ibeosdk::writeBE(os, m_paramGpsZAccuracy);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
