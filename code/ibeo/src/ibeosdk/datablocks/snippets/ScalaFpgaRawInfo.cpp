//======================================================================
/*! \file ScalaFpgaRawInfo.cpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ScalaFpgaRawInfo.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

const uint16_t ScalaFpgaRawInfo::blockId = 0xF010U;

//======================================================================

ScalaFpgaRawInfo::ScalaFpgaRawInfo()
  : m_ApdActualValue(0),
    m_ApdTableValue(0),
    m_temperature(0),
    m_referenceNoise(0),
    m_controlNoise(0),
    m_noiseMeasurementThreshold(0),
    m_reservedInfo7(0)
{}

//======================================================================

ScalaFpgaRawInfo::~ScalaFpgaRawInfo() {}

//======================================================================

bool ScalaFpgaRawInfo::operator==(const ScalaFpgaRawInfo& other) const
{
	return this->m_ApdActualValue == other.m_ApdActualValue
	    && this->m_ApdTableValue == other.m_ApdTableValue
	    && this->m_temperature == other.m_temperature
	    && this->m_referenceNoise == other.m_referenceNoise
	    && this->m_controlNoise == other.m_controlNoise
	    && this->m_noiseMeasurementThreshold == other.m_noiseMeasurementThreshold
	    && this->m_reservedInfo7 == other.m_reservedInfo7;
}

//======================================================================

bool ScalaFpgaRawInfo::operator!=(const ScalaFpgaRawInfo& other) const
{
	return ! ((*this) == other);
}

//======================================================================

bool ScalaFpgaRawInfo::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	uint16_t readBlockId;
	readLE(is, readBlockId);
	if (readBlockId != ScalaFpgaRawInfo::blockId)
		return false;

	readLE(is, m_ApdActualValue);
	readLE(is, m_ApdTableValue);
	readLE(is, m_temperature);
	readLE(is, m_referenceNoise);
	readLE(is, m_controlNoise);
	readLE(is, m_noiseMeasurementThreshold);
	readLE(is, m_reservedInfo7);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == (this->getSerializedSize()));
}

//======================================================================

bool ScalaFpgaRawInfo::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());
	
	writeLE(os, blockId);
	writeLE(os, m_ApdActualValue);
	writeLE(os, m_ApdTableValue);
	writeLE(os, m_temperature);
	writeLE(os, m_referenceNoise);
	writeLE(os, m_controlNoise);
	writeLE(os, m_noiseMeasurementThreshold);
	writeLE(os, m_reservedInfo7);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
