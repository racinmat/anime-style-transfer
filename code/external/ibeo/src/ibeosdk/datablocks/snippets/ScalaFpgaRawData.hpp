//======================================================================
/*! \file ScalaFpgaRawData.hpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SCALAFPGARAWDATA_HPP_SEEN
#define IBEOSDK_SCALAFPGARAWDATA_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/datablocks/snippets/ScalaFpgaRawHeader.hpp>
#include <ibeosdk/datablocks/snippets/ScalaFpgaRawInfo.hpp>
#include <ibeosdk/datablocks/snippets/ScalaFpgaRawScanPoint.hpp>
#include <ibeosdk/datablocks/snippets/ScalaFpgaRawSegInfo.hpp>
#include <ibeosdk/datablocks/snippets/ScalaFpgaRawDiagPulse.hpp>
#include <ibeosdk/datablocks/snippets/ScalaFpgaRawRefScan.hpp>
#include <ibeosdk/datablocks/snippets/ScalaFpgaRawTrailer.hpp>

#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <vector>
#include <istream>
#include <ostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class ScalaFpgaRawData : public Snippet {
public:
	ScalaFpgaRawData();
	virtual ~ScalaFpgaRawData();

public:
	//! Equality predicate
	bool operator==(const ScalaFpgaRawData& other) const;
	bool operator!=(const ScalaFpgaRawData& other) const;

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	const ScalaFpgaRawHeader& getHeader() const { return m_header; }
	const ScalaFpgaRawInfo& getInfo() const { return m_info; }
	const std::vector<ScalaFpgaRawScanPoint>& getPoints() const { return m_points; }
	std::vector<ScalaFpgaRawScanPoint>& getPoints()	{ return m_points; }
	const std::vector<ScalaFpgaRawSegInfo>& getSegInfos() const { return m_segInfos; }
	std::vector<ScalaFpgaRawSegInfo>& getSegInfos() { return m_segInfos; }
	const std::vector<ScalaFpgaRawDiagPulse>& getDiagPulses() const { return m_diagPulses; }
	std::vector<ScalaFpgaRawDiagPulse>& getDiagPulses() { return m_diagPulses; }
	const std::vector<ScalaFpgaRawRefScan>& getRefScans() const { return m_refScans; }
	std::vector<ScalaFpgaRawRefScan>& getRefScans() { return m_refScans; }
	const ScalaFpgaRawTrailer& getTrailer() const { return m_trailer; }
	uint32_t getCrc32() const { return m_crc32; }
	IBEOSDK_DEPRECATED uint32_t getCRC32() const { return m_crc32; }

public:
	void setHeader(const ScalaFpgaRawHeader& header) { m_header = header; }
	void setInfo(const ScalaFpgaRawInfo& info) { m_info = info; }
	void setTrailer(const ScalaFpgaRawTrailer& trailer) { m_trailer = trailer; }
	void setCrc32(const uint32_t crc) { m_crc32 = crc; }
	IBEOSDK_DEPRECATED void setCRC32(const uint32_t crc) { m_crc32 = crc; }

protected:
	static void unget2(std::istream& is);

protected:
	ScalaFpgaRawHeader m_header;
	ScalaFpgaRawInfo m_info;
	std::vector<ScalaFpgaRawScanPoint> m_points;
	std::vector<ScalaFpgaRawSegInfo> m_segInfos;
	std::vector<ScalaFpgaRawDiagPulse> m_diagPulses;
	std::vector<ScalaFpgaRawRefScan> m_refScans;
	ScalaFpgaRawTrailer m_trailer;
	uint32_t m_crc32;
	uint32_t m_reserved00;
	uint64_t m_reserved01;
}; // ScalaFpgaRawData

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCALAFPGARAWDATA_HPP_SEEN

//======================================================================

