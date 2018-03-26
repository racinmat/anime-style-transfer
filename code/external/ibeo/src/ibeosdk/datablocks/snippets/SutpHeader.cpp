//======================================================================
/*! \file SutpHeader.cpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen
 * \date Aug 31, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/SutpHeader.hpp>

#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

#include <ostream>

//======================================================================

namespace ibeosdk {

//======================================================================

SutpHeader::SutpHeader()
  : m_timestamp(0),
    m_version(0),
    m_seqNo(0),
    m_flags(0),
    m_scannerId(0),
    m_dataType(0),
    m_fwVersion(0),
    m_scanNo(0),
    m_fragsTotal(0),
    m_fragNo(0)
{}

//======================================================================

SutpHeader::SutpHeader(const uint64_t timestamp,
                       const uint16_t version,
                       const uint16_t seqNo,
                       const uint8_t  flags,
                       const uint8_t  scannerId,
                       const uint16_t dataType,
                       const uint16_t fwVersion,
                       const uint16_t scanNo,
                       const uint16_t fragsTotal,
                       const uint16_t fragNo)
 :  m_timestamp(timestamp),
    m_version(version),
    m_seqNo(seqNo),
    m_flags(flags),
    m_scannerId(scannerId),
    m_dataType(dataType),
    m_fwVersion(fwVersion),
    m_scanNo(scanNo),
    m_fragsTotal(fragsTotal),
    m_fragNo(fragNo)
{}

//======================================================================

SutpHeader::~SutpHeader() {}

//======================================================================

bool SutpHeader::operator==(const SutpHeader& other) const
{
	return m_timestamp == other.m_timestamp
	   &&  m_version == other.m_version
	   &&  m_seqNo == other.m_seqNo
	   &&  m_flags == other.m_flags
	   &&  m_scannerId == other.m_scannerId
	   &&  m_dataType == other.m_dataType
	   &&  m_fwVersion == other.m_fwVersion
	   &&  m_scanNo == other.m_scanNo
	   &&  m_fragsTotal == other.m_fragsTotal
	   &&  m_fragNo == other.m_fragNo;
}

//======================================================================

bool SutpHeader::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_timestamp);
	ibeosdk::readBE(is, m_version);
	if (m_version != ProtocolVersion_01) {
		logError << "SUTP header magic word mismatch." << std::endl;
		is.clear(std::ios::failbit);
		return !is.fail();
	}
	ibeosdk::readBE(is, m_seqNo);
	ibeosdk::readBE(is, m_flags);
	ibeosdk::readBE(is, m_scannerId);
	ibeosdk::readBE(is, m_dataType);
	ibeosdk::readBE(is, m_fwVersion);
	ibeosdk::readBE(is, m_scanNo);
	ibeosdk::readBE(is, m_fragsTotal);
	ibeosdk::readBE(is, m_fragNo);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool SutpHeader::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());


	if (m_version != ProtocolVersion_01)
	{
		logError << "SUTP header magic word mismatch." << std::endl;
		return false;
	}

	ibeosdk::writeBE(os, m_timestamp);
	ibeosdk::writeBE(os, m_version);
	ibeosdk::writeBE(os, m_seqNo);
	ibeosdk::writeBE(os, m_flags);
	ibeosdk::writeBE(os, m_scannerId);
	ibeosdk::writeBE(os, m_dataType);
	ibeosdk::writeBE(os, m_fwVersion);
	ibeosdk::writeBE(os, m_scanNo);
	ibeosdk::writeBE(os, m_fragsTotal);
	ibeosdk::writeBE(os, m_fragNo);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::string SutpHeader::prettyPrint() const
{
	std::stringstream os;
	os << std::hex
	   << "[ibeo::SutpHeader:"
	   << " m_timestamp=" << m_timestamp
	   << " m_version=" << m_version
	   << " m_seqNo=" << m_seqNo
	   << " m_flags=" << uint16_t(m_flags)
	   << " m_scannerId=" << uint16_t(m_scannerId)
	   << " m_dataType=" << m_dataType
	   << " m_fwVersion=" << m_fwVersion
	   << " m_scanNo=" << m_scanNo
	   << " m_fragsTotal=" << m_fragsTotal
	   << " m_fragNo=" << m_fragNo
	   << "]";
	return os.str();
}

//======================================================================

} // namespace ibeosdk

//======================================================================
