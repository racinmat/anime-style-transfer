//======================================================================
/*! \file EventTag.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Mar 18, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_EVENTTAG_HPP_SEEN
#define IBEOSDK_EVENTTAG_HPP_SEEN

//======================================================================

#include <ibeosdk/Time.hpp>
#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/PositionWgs84.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class EventTag : public ibeosdk::RegisteredDataBlock<EventTag> {
public:
	enum TagClass {
		Unknown    = 0x0000,
		Pedestrian = 0x0001
	};

	enum Flags {
		IsEnd   = 0x01,
		IsPulse = 0x02
	};

public:
	EventTag();
	virtual ~EventTag();

public:
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	NTPTime getTagStart() const { return m_tagStart; }
	NTPTime getTagEnd() const { return m_tagEnd; }
	uint8_t getFlags() const { return m_flags; }
	TagClass getTagClass() const { return m_tagClass; }
	uint32_t getTagId() const { return m_tagId; }
	const std::string& getTagString() const { return  m_tagString; }
	std::string& getTagString() { return  m_tagString; }
	const PositionWgs84& getWgs84() const { return m_wgs84; }
	PositionWgs84& getWgs84() { return m_wgs84; }
	uint64_t getRoiWidth() const { return m_roiWidth; }
	uint64_t getRoiLength() const { return m_roiLength; }

	bool isEndOfEvent () const { return ((m_flags & IsEnd) != 0); }
	bool isPulseEvent () const { return ((m_flags & IsPulse) != 0); }

public:
	void setTagStart(const NTPTime newTagStart) { m_tagStart = newTagStart; }
	void setTagEnd(const NTPTime newTagEnd) { m_tagEnd = newTagEnd; }
	void setFlags(const uint8_t newFlags) { m_flags = newFlags; }
	void setTagClass(const TagClass newTagClass) { m_tagClass = newTagClass; }
	void setTagId(const uint32_t newTagId) { m_tagId = newTagId; }
	void setTagString(const std::string& newTagString) { m_tagString = newTagString; }
	void setWgs84(const PositionWgs84& newWgs84) { m_wgs84 = newWgs84; }
	void setRoiWidth(const uint64_t newRoiWidth) { m_roiWidth = newRoiWidth; }
	void setRoiLength(const uint64_t newRoiLength) { m_roiLength = newRoiLength; }

	void setIsEndOfEvent() { m_flags |= uint8_t(IsEnd); }
	void unsetIsEndOfEvent() { m_flags &= uint8_t(~IsEnd); }

	void setIsPulseEvent() { m_flags |= uint8_t(IsPulse); }
	void unsetIsPulseEvent() { m_flags &= uint8_t(~IsPulse); }
public:
	static const int nbOfReserved = 16;

protected:
	NTPTime m_tagStart;
	NTPTime m_tagEnd;
	uint8_t m_flags;
	TagClass m_tagClass; // as uint16_t
	uint32_t m_tagId;
	std::string m_tagString;
	PositionWgs84 m_wgs84;
	uint64_t m_roiWidth;
	uint64_t m_roiLength;
	char m_reserved[nbOfReserved];
}; // EventTag

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_EVENTTAG_HPP_SEEN

//======================================================================
