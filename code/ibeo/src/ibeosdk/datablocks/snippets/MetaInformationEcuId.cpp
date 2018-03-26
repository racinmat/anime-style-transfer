//======================================================================
/*! \file MetaInformationEcuId.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/MetaInformationEcuId.hpp>

#include <ibeosdk/io.hpp>

#include <algorithm>

//======================================================================

namespace ibeosdk {

//======================================================================

MetaInformationEcuId::MetaInformationEcuId()
  : MetaInformation(MetaInformation::MetaInfoType_EcuId),
    m_ecuId()
{}

//======================================================================

uint32_t MetaInformationEcuId::getSerializedPayloadSize() const
{
	return uint32_t(sizeof(uint16_t) + m_ecuId.size());
}

//======================================================================

void MetaInformationEcuId::setEcuId(const std::string& newEcuId)
{
	m_ecuId = newEcuId;
	if (m_ecuId.size() > maxStringLength) {
		m_ecuId.resize(maxStringLength);
	}
}

//======================================================================

bool MetaInformationEcuId::deserializePayload(std::istream& is, const uint32_t payloadSize)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	uint16_t len;
	ibeosdk::readBE(is, len);

	m_ecuId.resize(len);
	is.read(&m_ecuId[0], len);

//	C++11 way
//	m_ecuId.clear();
//	std::istream_iterator<char> inputStartIt(is);
//	std::copy_n(inputStartIt, len, m_ecuId.begin());

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == payloadSize && payloadSize == getSerializedPayloadSize());
}

//======================================================================

bool MetaInformationEcuId::serializePayload(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, uint16_t(m_ecuId.size()));

	std::ostream_iterator<char> outputIter(os);
	std::copy(m_ecuId.begin(), m_ecuId.end(), outputIter);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool MetaInformationEcuId::isEqual(const MetaInformationEcuId& other) const
{
	return MetaInformation::isEqual(other)
	    && this->getEcuId() == other.getEcuId();
}

//======================================================================

bool MetaInformationEcuId::isNotEqual(const MetaInformationEcuId& other) const
{
	return MetaInformation::isNotEqual(other)
	    || this->getEcuId() != other.getEcuId();
}

//======================================================================
//======================================================================
//======================================================================

bool operator==(const MetaInformationEcuId& lhs, const MetaInformationEcuId& rhs)
{
	return lhs.isEqual(rhs);
}

//======================================================================

bool operator!=(const MetaInformationEcuId& lhs, const MetaInformationEcuId& rhs)
{
	return lhs.isNotEqual(rhs);
}

//======================================================================

}// namespace ibeosdk

//======================================================================
