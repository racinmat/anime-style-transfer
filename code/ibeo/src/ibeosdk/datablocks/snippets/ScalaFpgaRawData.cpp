//======================================================================
/*! \file ScalaFpgaRawData.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Aug 2, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ScalaFpgaRawData.hpp>
#include <ibeosdk/io.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

ScalaFpgaRawData::ScalaFpgaRawData()
  : m_header(),
    m_info(),
    m_points(),
    m_segInfos(),
    m_diagPulses(),
    m_refScans(),
    m_trailer(),
    m_crc32(0),
    m_reserved00(0),
    m_reserved01(0)
{}

//======================================================================

ScalaFpgaRawData::~ScalaFpgaRawData() {}

//======================================================================

bool ScalaFpgaRawData::operator==(const ScalaFpgaRawData& other) const
{
	return this->m_header == other.m_header
	    && this->m_info == other.m_info
	    && this->m_points == other.m_points
	    && this->m_segInfos == other.m_segInfos
	    && this->m_diagPulses == other.m_diagPulses
	    && this->m_refScans == other.m_refScans
	    && this->m_trailer == other.m_trailer
	    && this->m_crc32 == other.m_crc32;
}

//======================================================================

bool ScalaFpgaRawData::operator!=(const ScalaFpgaRawData& other) const
{
	return ! ((*this) == other);
}

//======================================================================

std::streamsize ScalaFpgaRawData::getSerializedSize() const
{
	return m_header.getSerializedSize()
	     + m_info.getSerializedSize()
	     + (std::streamsize(m_points.size()) * ScalaFpgaRawScanPoint::getSerializedSize_static())
	     + (std::streamsize(m_segInfos.size()) * ScalaFpgaRawSegInfo::getSerializedSize_static())
	     + (std::streamsize(m_diagPulses.size()) * ScalaFpgaRawDiagPulse::getSerializedSize_static())
	     + (std::streamsize(m_refScans.size()) * ScalaFpgaRawRefScan::getSerializedSize_static())
	     + m_trailer.getSerializedSize()
	     + std::streamsize(16);
}

//======================================================================

bool ScalaFpgaRawData::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	bool ok = true;
	bool complete = false;
	uint16_t readBlockId = 0;
	m_points.clear();
	m_segInfos.clear();
	m_diagPulses.clear();
	m_refScans.clear();

	// read header
	readLE(is, readBlockId);
	unget2(is);
	if (ScalaFpgaRawHeader::blockId != readBlockId) {
		return false;
	}

	ok = m_header.deserialize(is);
	if (!ok) {
		return false;
	}
	
	// read info
	readLE(is, readBlockId);
	unget2(is);
	if (ScalaFpgaRawInfo::blockId != readBlockId) {
		return false;
	}

	ok = m_info.deserialize(is);
	if (!ok) {
		return false;
	}

	// read arrays until trailer
	while ((!complete) && is.good() && ok) {
		readLE(is, readBlockId);
		unget2(is);
		if (ScalaFpgaRawScanPoint::blockId == readBlockId) {
			ScalaFpgaRawScanPoint pt;
			ok = pt.deserialize(is);
			m_points.push_back(pt);
		}
		else if (ScalaFpgaRawSegInfo::blockId == readBlockId) {
			ScalaFpgaRawSegInfo si;
			ok = si.deserialize(is);
			m_segInfos.push_back(si);
		}
		else if (ScalaFpgaRawDiagPulse::blockId == readBlockId) {
			ScalaFpgaRawDiagPulse dp;
			ok = dp.deserialize(is);
			m_diagPulses.push_back(dp);
		}
		else if (ScalaFpgaRawRefScan::blockId == readBlockId) {
			ScalaFpgaRawRefScan rs;
			ok = rs.deserialize(is);
			m_refScans.push_back(rs);
		}
		else if (ScalaFpgaRawTrailer::blockId == readBlockId) {
			complete = true;
			ok = m_trailer.deserialize(is);
		}
		else {
			return false;
		}
	} // while !complete, is.good and ok

	if ((!complete)
	  || (ScalaFpgaRawTrailer::blockId != readBlockId)
	  ||  !ok) {
		return false;
	}
	
	readLE(is, m_crc32);
	readLE(is, m_reserved00);
	readLE(is, m_reserved01);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

void ScalaFpgaRawData::unget2(std::istream& is)
{
	is.unget();
	is.unget();
}

//======================================================================

bool ScalaFpgaRawData::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	bool ok = true;

	ok = m_header.serialize(os);
	if (!ok) {
		return false;
	}
	
	ok = m_info.serialize(os);
	if (!ok) {
		return false;
	}
	
	std::vector<ScalaFpgaRawScanPoint>::const_iterator pt = m_points.begin();
	for (; pt != m_points.end(); ++pt) {
		ok = pt->serialize(os);
		if (!ok) {
			return false;
		}
	} // for

	std::vector<ScalaFpgaRawSegInfo>::const_iterator si = m_segInfos.begin();
	for (; si != m_segInfos.end(); ++si) {
		ok = si->serialize(os);
		if (!ok) {
			return false;
		}
	} // for

	std::vector<ScalaFpgaRawDiagPulse>::const_iterator dp = m_diagPulses.begin();
	for (; dp != m_diagPulses.end(); ++dp) {
		ok = dp->serialize(os);
		if (!ok) {
			return false;
		}
	} // for

	std::vector<ScalaFpgaRawRefScan>::const_iterator rs = m_refScans.begin();
	for (; rs != m_refScans.end(); ++rs) {
		ok = rs->serialize(os);
		if (!ok) {
			return false;
		}
	}

	ok = m_trailer.serialize(os);
	if (!ok) {
		return false;
	}
	
	writeLE(os, m_crc32);
	writeLE(os, m_reserved00);
	writeLE(os, m_reserved01);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize()) && ok;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
