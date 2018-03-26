//======================================================================
/*! \file ScalaFpgaRawSegInfo.cpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ScalaFpgaRawSegInfo.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

const uint16_t ScalaFpgaRawSegInfo::blockId = 0xF040U;

//======================================================================

ScalaFpgaRawSegInfo::ScalaFpgaRawSegInfo()
  : m_segmentIndex(0),
    m_ApdVoltage(0),
    m_noise(0),
    m_reservedSegmentInfo4(0),
    m_reservedSegmentInfo5(0),
    m_reservedSegmentInfo6(0),
    m_reservedSegmentInfo7(0)
{}

//======================================================================

ScalaFpgaRawSegInfo::~ScalaFpgaRawSegInfo() {}

//======================================================================

bool ScalaFpgaRawSegInfo::operator==(const ScalaFpgaRawSegInfo& other) const
{
	return this->m_segmentIndex == other.m_segmentIndex
	    && this->m_ApdVoltage == other.m_ApdVoltage
	    && this->m_noise == other.m_noise
	    && this->m_reservedSegmentInfo4 == other.m_reservedSegmentInfo4
	    && this->m_reservedSegmentInfo5 == other.m_reservedSegmentInfo5
	    && this->m_reservedSegmentInfo6 == other.m_reservedSegmentInfo6
	    && this->m_reservedSegmentInfo7 == other.m_reservedSegmentInfo7;
}

//======================================================================

bool ScalaFpgaRawSegInfo::operator!=(const ScalaFpgaRawSegInfo& other) const
{
	return !((*this) == other);
}

//======================================================================

bool ScalaFpgaRawSegInfo::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	uint16_t readBlockId;
	readLE(is, readBlockId);
	if (readBlockId != ScalaFpgaRawSegInfo::blockId)
		return false;

	readLE(is, m_segmentIndex);
	readLE(is, m_ApdVoltage);
	readLE(is, m_noise);
	readLE(is, m_reservedSegmentInfo4);
	readLE(is, m_reservedSegmentInfo5);
	readLE(is, m_reservedSegmentInfo6);
	readLE(is, m_reservedSegmentInfo7);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == (this->getSerializedSize()));
}

//======================================================================

bool ScalaFpgaRawSegInfo::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeLE(os, m_segmentIndex);
	writeLE(os, m_ApdVoltage);
	writeLE(os, m_noise);
	writeLE(os, m_reservedSegmentInfo4);
	writeLE(os, m_reservedSegmentInfo5);
	writeLE(os, m_reservedSegmentInfo6);
	writeLE(os, m_reservedSegmentInfo7);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
