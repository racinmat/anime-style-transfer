//======================================================================
/*! \file ScalaFpgaRawSegInfo.hpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SCALAFPGARAWSEGINFO_HPP_SEEN
#define IBEOSDK_SCALAFPGARAWSEGINFO_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/inttypes.hpp>

#include <istream>
#include <ostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class ScalaFpgaRawSegInfo : public Snippet {
public:
	static std::streamsize getSerializedSize_static() { return 16; }

public:
	ScalaFpgaRawSegInfo();
	virtual ~ScalaFpgaRawSegInfo();

public:
	//! Equality predicate
	bool operator==(const ScalaFpgaRawSegInfo& other) const;
	bool operator!=(const ScalaFpgaRawSegInfo& other) const;

public:
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	uint16_t getSegmentIndex() const { return m_segmentIndex; }
	uint16_t getApdVoltage() const { return m_ApdVoltage; }
	uint16_t getNoise() const { return m_noise; }
	uint16_t getReservedSegmentInfo4() const { return m_reservedSegmentInfo4; }
	uint16_t getReservedSegmentInfo5() const { return m_reservedSegmentInfo4; }
	uint16_t getReservedSegmentInfo6() const { return m_reservedSegmentInfo4; }
	uint16_t getReservedSegmentInfo7() const { return m_reservedSegmentInfo4; }

public:
	void setSegmentIndex(const uint16_t idx) { m_segmentIndex = idx; }
	void setApdVoltage(const uint16_t voltage) { m_ApdVoltage = voltage; }
	void setNoise(const uint16_t noise) { m_noise = noise; }

public:
	static const uint16_t blockId;

protected:
	uint16_t m_segmentIndex;
	uint16_t m_ApdVoltage;
	uint16_t m_noise;
	uint16_t m_reservedSegmentInfo4;
	uint16_t m_reservedSegmentInfo5;
	uint16_t m_reservedSegmentInfo6;
	uint16_t m_reservedSegmentInfo7;
}; // ScalaFpgaRawSegInfo

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCALAFPGARAWSEGINFO_HPP_SEEN

//======================================================================

