//======================================================================
/*! \file MetaInformationAppBaseConfig.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/MetaInformationAppBaseConfig.hpp>

#include <ibeosdk/io.hpp>

#include <algorithm>

//======================================================================

namespace ibeosdk {

//======================================================================

MetaInformationAppBaseConfig::MetaInformationAppBaseConfig()
  : MetaInformation(MetaInformation::MetaInfoType_AppBaseConfig),
    m_appBaseConfig()
{}

//======================================================================

uint32_t MetaInformationAppBaseConfig::getSerializedPayloadSize() const
{
	return uint32_t(sizeof(uint16_t) + m_appBaseConfig.size());
}

//======================================================================

void MetaInformationAppBaseConfig::setAppBaseConfig(const std::string& newAppBaseConfig)
{
	m_appBaseConfig = newAppBaseConfig;
	if (m_appBaseConfig.size() > maxStringLength) {
		m_appBaseConfig.resize(maxStringLength);
	}
}

//======================================================================

bool MetaInformationAppBaseConfig::deserializePayload(std::istream& is, const uint32_t payloadSize)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	uint16_t len;
	ibeosdk::readBE(is, len);

	m_appBaseConfig.resize(len);
	if (len>0)
		is.read(&m_appBaseConfig[0], len);

//	C++11 way
//	m_ecuId.clear();
//	std::istream_iterator<char> inputStartIt(is);
//	std::copy_n(inputStartIt, len, m_appBaseConfig.begin());

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == payloadSize) && (payloadSize == getSerializedPayloadSize());
}

//======================================================================

bool MetaInformationAppBaseConfig::serializePayload(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, uint16_t(m_appBaseConfig.size()));

	std::ostream_iterator<char> outputIter(os);
	std::copy(m_appBaseConfig.begin(), m_appBaseConfig.end(), outputIter);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedPayloadSize());
}

//======================================================================

bool MetaInformationAppBaseConfig::isEqual(const MetaInformationAppBaseConfig& other) const
{
	return MetaInformation::isEqual(other)
	    && this->getAppBaseConfig() == other.getAppBaseConfig();
}

//======================================================================

bool MetaInformationAppBaseConfig::isNotEqual(const MetaInformationAppBaseConfig& other) const
{
	return MetaInformation::isNotEqual(other)
	    || this->getAppBaseConfig() != other.getAppBaseConfig();
}

//======================================================================
//======================================================================
//======================================================================

bool operator==(const MetaInformationAppBaseConfig& lhs, const MetaInformationAppBaseConfig& rhs)
{
	return lhs.isEqual(rhs);
}

//======================================================================

bool operator!=(const MetaInformationAppBaseConfig& lhs, const MetaInformationAppBaseConfig& rhs)
{
	return lhs.isNotEqual(rhs);
}

//======================================================================

}// namespace ibeosdk

//======================================================================
