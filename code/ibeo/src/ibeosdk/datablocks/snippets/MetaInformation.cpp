//======================================================================
/*! \file MetaInformation.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/MetaInformation.hpp>

#include <ibeosdk/io.hpp>
#include <ibeosdk/Log.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

template<>
void writeBE<MetaInformation::MetaInformationType>(std::ostream& os, const MetaInformation::MetaInformationType& mit)
{
	writeBE(os, uint16_t(mit));
}

//======================================================================

template<>
void readBE<MetaInformation::MetaInformationType>(std::istream& is, MetaInformation::MetaInformationType& mit)
{
	uint16_t rd16;
	readBE(is, rd16);
	mit = MetaInformation::MetaInformationType(rd16);
}

//======================================================================
//======================================================================
//======================================================================

const uint32_t MetaInformation::maxStringLength = std::numeric_limits<uint16_t>::max();

//======================================================================

MetaInformation::MetaInformation(const MetaInformationType type, const NTPTime timeStamp)
  : m_type(type),
    m_timeStamp(timeStamp)
{}

//======================================================================

MetaInformation::~MetaInformation() {}

//======================================================================

std::streamsize MetaInformation::getSerializedSize() const
{
	return std::streamsize(sizeof(uint16_t) + sizeof(uint64_t) + sizeof(uint32_t)
	                     + this->getSerializedPayloadSize());
}

//======================================================================

bool MetaInformation::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readBE(is, m_type);
	//std::cerr << "D: after type @:" << (streamposToINT64(is.tellg()) - startPos) << "  Type: " << m_type << std::endl;
	readBE(is, m_timeStamp);
	//std::cerr << "D: after tms @:" << (streamposToINT64(is.tellg()) - startPos) << std::endl;
	uint32_t payloadSize;
	readBE(is, payloadSize);
	//std::cerr << "D: after pls @:" << (streamposToINT64(is.tellg()) - startPos) << "  PayloadSz: " << payloadSize << std::endl;

	deserializePayload(is, payloadSize);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool MetaInformation::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, m_type);
	//std::cerr << "S: after type @:" << (streamposToINT64(os.tellp()) - startPos) << "  Type: " << m_type << std::endl;
	writeBE(os, m_timeStamp);
	//std::cerr << "S: after tms @:" << (streamposToINT64(os.tellp()) - startPos) << std::endl;
	writeBE(os, this->getSerializedPayloadSize());
	//std::cerr << "S: after pls @:" << (streamposToINT64(os.tellp()) - startPos)  << "  PayloadSz: " << this->getSerializedPayloadSize() << std::endl;

	serializePayload(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool MetaInformation::isEqual(const MetaInformation& other) const
{
	return this->getType() == other.getType()
	    && this->getTimestamp() == other.getTimestamp();
}

//======================================================================

bool MetaInformation::isNotEqual(const MetaInformation& other) const
{
	return this->getType() != other.getType()
	    || this->getTimestamp() != other.getTimestamp();
}

//======================================================================
//======================================================================
//======================================================================

bool operator==(const MetaInformation& lhs, const MetaInformation& rhs)
{
	return lhs.isEqual(rhs);
}

//======================================================================

bool operator!=(const MetaInformation& lhs, const MetaInformation& rhs)
{
	return lhs.isNotEqual(rhs);
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const MetaInformation::MetaInformationType t)
{
	switch (t) {
	case MetaInformation::MetaInfoType_Unsupported:       os << "MetaInfoType_Unsupported"; break;
	case MetaInformation::MetaInfoType_VersionNumber:     os << "MetaInfoType_VersionNumber"; break;
	case MetaInformation::MetaInfoType_AppBaseConfig:     os << "MetaInfoType_AppBaseConfig"; break;
	case MetaInformation::MetaInfoType_AppBaseSyncMethod: os << "MetaInfoType_AppBaseSyncMethod"; break;
	case MetaInformation::MetaInfoType_EcuId:             os << "MetaInfoType_EcuId"; break;
	case MetaInformation::MetaInfoType_Keywords:          os << "MetaInfoType_Keywords"; break;
	default: os << "Unknown type" << toHex(uint32_t(t));
	}
	return os;
}

//======================================================================

}// namespace ibeosdk

//======================================================================
