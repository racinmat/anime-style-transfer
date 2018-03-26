//======================================================================
/*! \file ScanPointEcu.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ScanPointEcu.hpp>
#include <ibeosdk/io.hpp>
#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

ScanPointEcu::ScanPointEcu()
  : m_posX(0.),
    m_posY(0.),
    m_posZ(0.),
    m_epw(0.),
    m_deviceId(0),
    m_layer(0),
    m_echo(0),
    m_reserved(0),
    m_timeOffset(0),
    m_flags(0),
    m_segmentId(0)
{}

//======================================================================

ScanPointEcu::ScanPointEcu(const ScanPointEcu& other)
  : m_posX(other.m_posX),
    m_posY(other.m_posY),
    m_posZ(other.m_posZ),
    m_epw(other.m_epw),
    m_deviceId(other.m_deviceId),
    m_layer(other.m_layer),
    m_echo(other.m_echo),
    m_reserved(other.m_reserved),
    m_timeOffset(other.m_timeOffset),
    m_flags(other.m_flags),
    m_segmentId(other.m_segmentId)
{}

//======================================================================

ScanPointEcu& ScanPointEcu::operator=(const ScanPointEcu& other)
{
	m_posX = other.m_posX;
	m_posY = other.m_posY;
	m_posZ = other.m_posZ;
	m_epw = other.m_epw;
	m_deviceId = other.m_deviceId;
	m_layer = other.m_layer;
	m_echo = other.m_echo;
	m_reserved = other.m_reserved;
	m_timeOffset = other.m_timeOffset;
	m_flags = other.m_flags;
	m_segmentId = other.m_segmentId;

  return *this;
}

//======================================================================

ScanPointEcu::~ScanPointEcu() {}

//======================================================================

bool ScanPointEcu::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_posX);
	ibeosdk::readBE(is, m_posY);
	ibeosdk::readBE(is, m_posZ);

	ibeosdk::readBE(is, m_epw);

	ibeosdk::readBE(is, m_deviceId);
	ibeosdk::readBE(is, m_layer);
	ibeosdk::readBE(is, m_echo);

	ibeosdk::readBE(is, m_reserved);

	ibeosdk::readBE(is, m_timeOffset);
	ibeosdk::readBE(is, m_flags);
	ibeosdk::readBE(is, m_segmentId);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

//static
std::streamsize ScanPointEcu::getSerializedSize_static()
{
	return
		4*std::streamsize(sizeof(float))
		+ 3*std::streamsize(sizeof(UINT8)) + 1
		+ std::streamsize(sizeof(UINT32))
		+ std::streamsize(sizeof(UINT16)) + 2;
}

//======================================================================

bool ScanPointEcu::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_posX);
	ibeosdk::writeBE(os, m_posY);
	ibeosdk::writeBE(os, m_posZ);

	ibeosdk::writeBE(os, m_epw);

	ibeosdk::writeBE(os, m_deviceId);
	ibeosdk::writeBE(os, m_layer);
	ibeosdk::writeBE(os, m_echo);

	ibeosdk::writeBE(os, m_reserved);

	ibeosdk::writeBE(os, m_timeOffset);
	ibeosdk::writeBE(os, m_flags);

	ibeosdk::writeBE(os, m_segmentId);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ScanPointEcu::operator==( const ScanPointEcu& other ) const
{
	if ( !(fuzzyCompareT<7>(m_posX, other.getPositionX())))
		return false;

	if ( !(fuzzyCompareT<7>(m_posY, other.getPositionY())))
		return false;

	if ( !(fuzzyCompareT<7>(m_posZ, other.getPositionZ())))
		return false;

	if ( !(fuzzyCompareT<7>(m_epw, other.getEchoPulseWidth())))
		return false;
	
	if (m_deviceId != other.getDeviceId())
		return false;

	if (m_layer != other.getLayer())
		return false;

	if (m_echo != other.getEcho())
		return false;

	if (m_reserved != other.getReserved())
		return false;

	if (m_timeOffset != other.getTimeOffset())
		return false;

	if (m_flags != other.getFlags())
		return false;

	if (m_segmentId != other.getSegmentId())
		return false;	
	
	return true;
}


//======================================================================

} // namespace ibeosdk

//======================================================================

