//======================================================================
/*! \file ScanPointLux.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 25, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ScanPointLux.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

ScanPointLux::ScanPointLux() {}

//======================================================================

ScanPointLux::ScanPointLux(const ScanPointLux& src)
  : m_layer(src.m_layer),
    m_echo(src.m_echo),
    m_flags(src.m_flags),
    m_angle(src.m_angle),
    m_distance(src.m_distance),
    m_epw(src.m_epw),
    m_reserved(src.m_reserved)
{}

//======================================================================

ScanPointLux& ScanPointLux::operator= (const ScanPointLux& src)
{
	if (&src != this) {
		m_layer = src.m_layer;
		m_echo = src.m_echo;
		m_flags = src.m_flags;
		m_angle = src.m_angle;
		m_distance = src.m_distance;
		m_epw = src.m_epw;
		m_reserved = src.m_reserved;
	}

	return *this;
}

//======================================================================

ScanPointLux::~ScanPointLux() {}

//======================================================================

// static
std::streamsize ScanPointLux::getSerializedSize_static()
{
	return 2 * std::streamsize(sizeof(UINT8)) + 3 * std::streamsize(sizeof(UINT16)) + 2;
}

//======================================================================

bool ScanPointLux::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	BYTE layerAndEcho;
	ibeosdk::readLE(is,layerAndEcho);
	m_layer = layerAndEcho & 0xF;
	m_echo = (layerAndEcho >> 4) & 0xF;
	ibeosdk::readLE(is, m_flags);
	ibeosdk::readLE(is, m_angle);
	ibeosdk::readLE(is, m_distance);

	ibeosdk::readLE(is, m_epw);
	ibeosdk::readLE(is,m_reserved);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ScanPointLux::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	const UINT8 layerAndEcho = UINT8((m_echo << 4) | (m_layer & 0xF));
	ibeosdk::writeLE(os, layerAndEcho);
	ibeosdk::writeLE(os, m_flags);
	ibeosdk::writeLE(os, m_angle);
	ibeosdk::writeLE(os, m_distance);
	ibeosdk::writeLE(os, m_epw);
	ibeosdk::writeLE(os, m_reserved);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================

