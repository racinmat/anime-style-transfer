//======================================================================
/*! \file MetaInformationUnsupported.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/MetaInformationUnsupported.hpp>

#include <ibeosdk/io.hpp>

#include <algorithm>

//======================================================================

namespace ibeosdk {

//======================================================================

MetaInformationUnsupported::MetaInformationUnsupported(const uint16_t originalType, const NTPTime timestamp, const uint32_t dataSize)
  : MetaInformation(MetaInformation::MetaInfoType_Unsupported, timestamp),
    m_originalType(originalType),
    m_data(dataSize)
{}

//======================================================================

MetaInformationUnsupported::MetaInformationUnsupported(const uint16_t originalType, const NTPTime timestamp, const DataVector& data)
  : MetaInformation(MetaInformation::MetaInfoType_Unsupported, timestamp),
    m_originalType(originalType),
    m_data(data)
{}

//======================================================================

std::streamsize MetaInformationUnsupported::getSerializedSize() const
{
	return std::streamsize(sizeof(uint16_t) + sizeof(uint64_t) + sizeof(uint32_t)
	                     + this->getSerializedPayloadSize());
}

//======================================================================

bool MetaInformationUnsupported::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readBE(is, m_originalType);
	readBE(is, m_timeStamp);
	uint32_t payloadSize;
	readBE(is, payloadSize);

	deserializePayload(is, payloadSize);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool MetaInformationUnsupported::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, m_originalType);
	writeBE(os, m_timeStamp);
	writeBE(os, this->getSerializedPayloadSize());

	serializePayload(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool MetaInformationUnsupported::deserializePayload(std::istream& is, const uint32_t payloadSize)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	m_data.resize(payloadSize);
	is.read(reinterpret_cast<char*>(&m_data[0]), uint32_t(payloadSize));

//	C++11 way
//	const uint32_t len = m_data.size();
//	m_data.clear();
//	m_data.reserve(len);
//	std::istream_iterator<char> inputStartIt(is);
//	std::copy_n(inputStartIt, len, m_data.begin());

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == payloadSize && payloadSize == getSerializedPayloadSize());
}

//======================================================================

bool MetaInformationUnsupported::serializePayload(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	std::ostream_iterator<char> outputIter(os);
	std::copy(m_data.begin(), m_data.end(), outputIter);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedPayloadSize());
}

//======================================================================

bool MetaInformationUnsupported::isEqual(const MetaInformationUnsupported& other) const
{
	return MetaInformation::isEqual(other)
	    && this->getOriginalType() == other.getOriginalType()
	    && this->getData() == other.getData();
}

//======================================================================

bool MetaInformationUnsupported::isNotEqual(const MetaInformationUnsupported& other) const
{
	return MetaInformation::isNotEqual(other)
	    || this->getOriginalType() != other.getOriginalType()
	    || this->getData() != other.getData();
}

//======================================================================
//======================================================================
//======================================================================

bool operator==(const MetaInformationUnsupported& lhs, const MetaInformationUnsupported& rhs)
{
	return lhs.isEqual(rhs);
}

//======================================================================

bool operator!=(const MetaInformationUnsupported& lhs, const MetaInformationUnsupported& rhs)
{
	return lhs.isNotEqual(rhs);
}

//======================================================================

}// namespace ibeosdk

//======================================================================
