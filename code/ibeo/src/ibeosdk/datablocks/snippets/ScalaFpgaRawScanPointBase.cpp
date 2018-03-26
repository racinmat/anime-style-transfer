//======================================================================
/*! \file ScalaFpgaRawScanPointBase.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Oct 23, 2015
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/ScalaFpgaRawScanPointBase.hpp>

#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

ScalaFpgaRawScanPointBase::ScalaFpgaRawScanPointBase()
  : m_radialDistance(0),
    m_echoPulseWidth(0),
    m_angle(0),
    m_reserved(0),
    m_resolution(0),
    m_channelId(0),
    m_echoId(0),
    m_flagsHigh(0),
    m_flagsLow(0)
{}

//======================================================================

ScalaFpgaRawScanPointBase::~ScalaFpgaRawScanPointBase() {}

//======================================================================

bool ScalaFpgaRawScanPointBase::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	uint16_t readBlockId;
	readLE(is, readBlockId);
	if (readBlockId != this->getBlockId())
		return false;

	readLE(is, m_radialDistance);
	readLE(is, m_echoPulseWidth);
	readLE(is, m_angle);
	readLE(is, m_reserved); // not used
	readLE(is, m_resolution);

	uint16_t u16;
	readLE(is, u16);
	m_channelId = uint8_t(u16 >> 8);
	m_echoId = uint8_t(u16);

	readLE(is, u16);
	m_flagsHigh = uint8_t(u16 >> 8);
	m_flagsLow = uint8_t(u16);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == (this->getSerializedSize()));
}

//======================================================================

bool ScalaFpgaRawScanPointBase::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeLE(os, this->getBlockId());
	writeLE(os, m_radialDistance);
	writeLE(os, m_echoPulseWidth);
	writeLE(os, m_angle);
	writeLE(os, m_reserved); // not used
	writeLE(os, m_resolution);

	const uint16_t echoLayer = uint16_t(m_echoId | (m_channelId << 8));
	writeLE(os, echoLayer);

	const uint16_t flags = uint16_t(m_flagsLow | (m_flagsHigh << 8));
	writeLE(os, flags);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ScalaFpgaRawScanPointBase::operator==(const ScalaFpgaRawScanPointBase& other) const
{
	return this->getBlockId() == other.getBlockId()
		&& this->m_radialDistance == other.m_radialDistance
	    && this->m_echoPulseWidth == other.m_echoPulseWidth
	    && this->m_angle == other.m_angle
	    && this->m_reserved == other.m_reserved
	    && this->m_resolution == other.m_resolution
	    && this->m_channelId == other.m_channelId
	    && this->m_echoId == other.m_echoId
	    && this->m_flagsHigh == other.m_flagsHigh
	    && this->m_flagsLow == other.m_flagsLow;
}

//======================================================================

bool ScalaFpgaRawScanPointBase::operator!=(const ScalaFpgaRawScanPointBase& other) const
{
	return !((*this) == other);
}

//======================================================================

}// namespace ibeosdk

//======================================================================
