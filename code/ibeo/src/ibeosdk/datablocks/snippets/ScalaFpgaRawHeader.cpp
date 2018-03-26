//======================================================================
/*! \file ScalaFpgaRawHeader.cpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ScalaFpgaRawHeader.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

const uint16_t ScalaFpgaRawHeader::blockId = 0xF000U;

//======================================================================

ScalaFpgaRawHeader::ScalaFpgaRawHeader()
  : m_scanCounter(0),
    m_minApdOffset(0),
    m_maxApdOffset(0),
    m_frequencyInteger(0),
    m_frequencyFractional(0),
    m_deviceId(0),
    m_reservedHeader7(0)
{}

//======================================================================

ScalaFpgaRawHeader::~ScalaFpgaRawHeader() {}

//======================================================================

bool ScalaFpgaRawHeader::operator==(const ScalaFpgaRawHeader& other) const
{
	return this->m_scanCounter == other.m_scanCounter
	    && this->m_minApdOffset == other.m_minApdOffset
	    && this->m_maxApdOffset == other.m_maxApdOffset
	    && this->m_frequencyInteger == other.m_frequencyInteger
	    && this->m_frequencyFractional == other.m_frequencyFractional
	    && this->m_deviceId == other.m_deviceId
	    && this->m_reservedHeader7 == other.m_reservedHeader7;
}

//======================================================================

bool ScalaFpgaRawHeader::operator!=(const ScalaFpgaRawHeader& other) const
{
	return !((*this) == other);
}

//======================================================================

bool ScalaFpgaRawHeader::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	uint16_t readBlockId;
	readLE(is, readBlockId);
	if (readBlockId != ScalaFpgaRawHeader::blockId)
		return false;

	readLE(is, m_scanCounter);
	readLE(is, m_minApdOffset);
	readLE(is, m_maxApdOffset);
	readLE(is, m_frequencyInteger);
	readLE(is, m_frequencyFractional);
	readLE(is, m_deviceId);
	readLE(is, m_reservedHeader7);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == (this->getSerializedSize()));
}

//======================================================================

bool ScalaFpgaRawHeader::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeLE(os, blockId);
	writeLE(os, m_scanCounter);
	writeLE(os, m_minApdOffset);
	writeLE(os, m_maxApdOffset);
	writeLE(os, m_frequencyInteger);
	writeLE(os, m_frequencyFractional);
	writeLE(os, m_deviceId);
	writeLE(os, m_reservedHeader7);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
