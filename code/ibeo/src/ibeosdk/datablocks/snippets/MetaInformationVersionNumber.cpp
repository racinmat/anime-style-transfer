//======================================================================
/*! \file MetaInformationVersionNumber.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/MetaInformationVersionNumber.hpp>

#include <ibeosdk/io.hpp>

#include <algorithm>

//======================================================================

namespace ibeosdk {

//======================================================================
//======================================================================
//======================================================================

template<>
void writeBE<MetaInformationVersionNumber::SoftwareType>(std::ostream& os, const MetaInformationVersionNumber::SoftwareType& cst)
{
	writeBE(os, uint16_t(cst));
}

//======================================================================

template<>
void readBE<MetaInformationVersionNumber::SoftwareType>(std::istream& is, MetaInformationVersionNumber::SoftwareType& cst)
{
	uint16_t rd16;
	readBE(is, rd16);
	cst = MetaInformationVersionNumber::SoftwareType(rd16);
}

//======================================================================

MetaInformationVersionNumber::MetaInformationVersionNumber()
  : MetaInformation(MetaInformation::MetaInfoType_VersionNumber),
    m_softwareType(SoftwareType_Undefined),
    m_versionNumber(0),
    m_extraString("")
{}

//==============================================================================

void MetaInformationVersionNumber::setVersionNumber(const UINT16 major, const UINT16 minor, const UINT16 patch, const std::string extra)
{
	m_versionNumber = VersionNumberType(major * 1000000 + minor * 1000 + patch);
	m_extraString = extra;
}

//==============================================================================

MetaInformationVersionNumber::VersionPartType MetaInformationVersionNumber::getMajorVersion() const
{
	return VersionPartType(m_versionNumber/1000000);
}

//==============================================================================

MetaInformationVersionNumber::VersionPartType MetaInformationVersionNumber::getMinorVersion() const
{
	return VersionPartType((m_versionNumber%1000000)/1000);
}

//==============================================================================

MetaInformationVersionNumber::VersionPartType MetaInformationVersionNumber::getPatchVersion() const
{
	return VersionPartType(m_versionNumber%1000);
}

//======================================================================

uint32_t MetaInformationVersionNumber::getSerializedPayloadSize() const
{
	return uint32_t(sizeof(uint16_t) + sizeof(uint32_t) + sizeof(uint16_t) + m_extraString.size());
}

//======================================================================

bool MetaInformationVersionNumber::deserializePayload(std::istream& is, const uint32_t payloadSize)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readBE(is, m_softwareType);
	readBE(is, m_versionNumber);

	uint16_t len;
	ibeosdk::readBE(is, len);

	m_extraString.resize(len);
	is.read(&m_extraString[0], len);

//	C++11 way
//	m_extraString.clear();
//	std::istream_iterator<char> inputStartIt(is);
//	std::copy_n(inputStartIt, len, m_extraString.begin());

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == payloadSize && payloadSize == getSerializedPayloadSize());
}

//======================================================================

bool MetaInformationVersionNumber::serializePayload(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, m_softwareType);
	writeBE(os, m_versionNumber);

	writeBE(os, uint16_t(m_extraString.size()));
	std::ostream_iterator<char> outputIter(os);
	std::copy(m_extraString.begin(), m_extraString.end(), outputIter);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedPayloadSize());
}

//======================================================================

bool MetaInformationVersionNumber::isEqual(const MetaInformationVersionNumber& other) const
{
	return MetaInformation::isEqual(other)
	    && this->getSoftwareType() == other.getSoftwareType()
	    && this->getVersionNumber() == other.getVersionNumber()
	    && this->getExtraString() == other.getExtraString();

}

//======================================================================

bool MetaInformationVersionNumber::isNotEqual(const MetaInformationVersionNumber& other) const
{
	return MetaInformation::isNotEqual(other)
	    || this->getSoftwareType() != other.getSoftwareType()
	    || this->getVersionNumber() != other.getVersionNumber()
	    || this->getExtraString() != other.getExtraString();

}

//======================================================================
//======================================================================
//======================================================================

bool operator==(const MetaInformationVersionNumber& lhs, const MetaInformationVersionNumber& rhs)
{
	return lhs.isEqual(rhs);
}

//======================================================================

bool operator!=(const MetaInformationVersionNumber& lhs, const MetaInformationVersionNumber& rhs)
{
	return lhs.isNotEqual(rhs);
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const MetaInformationVersionNumber::SoftwareType st)
{
	switch (st) {
	case MetaInformationVersionNumber::SoftwareType_Undefined: os << "SoftwareType_Undefined"; break;
	case MetaInformationVersionNumber::SoftwareType_Custom:    os << "SoftwareType_Custom"; break;
	case MetaInformationVersionNumber::SoftwareType_AppBase:   os << "SoftwareType_AppBase"; break;
	case MetaInformationVersionNumber::SoftwareType_EvS:       os << "SoftwareType_EvS"; break;
	case MetaInformationVersionNumber::SoftwareType_Ilv:       os << "SoftwareType_Ilv"; break;
	default: os << "Unknown software type" << uint32_t(st);
	}
	return os;
}

//======================================================================

}// namespace ibeosdk

//======================================================================
