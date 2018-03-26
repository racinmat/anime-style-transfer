//======================================================================
/*! \file ObjectLabelList6503.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 28, 2016
 *///-------------------------------------------------------------------

#ifndef OBJECTLABELLIST6503_HPP_SEEN
#define OBJECTLABELLIST6503_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/ObjectLabel6503.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class ObjectLabelList6503 : public RegisteredDataBlock<ObjectLabelList6503> {
public:
	typedef std::vector<ObjectLabel6503> LabelVector;

public:
	static const uint32_t nbOfReserved = 10;

public:
	ObjectLabelList6503();

public: // interface of RegisteredDataBlock
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	uint32_t getTimeOffsetUs() const { return m_timeOffsetUs; }
	NTPTime getTimestamp() const { return m_timestamp; }
	uint32_t getScanNumber() const { return m_scanNumber; }
	NTPTime getScanMidTimestamp() const { return m_scanMidTimestamp; }
	uint16_t getReserved(const uint32_t idx) const { assert(idx < nbOfReserved); return m_reserved[idx]; }

	const LabelVector& getLabels() const { return m_labels; }
	LabelVector& getLabels() { return m_labels; }

public: // setter
	void setTimeOffsetUs(const uint32_t newTimeOffsetUs) { m_timeOffsetUs = newTimeOffsetUs; }
	void setTimestamp(const NTPTime newTimestamp) { m_timestamp = newTimestamp; }
	void setScanNumber(const uint32_t newScanNumber) { m_scanNumber = newScanNumber; }
	void setScanMidTimestamp(const NTPTime newScanMidTimestamp) { m_scanMidTimestamp = newScanMidTimestamp; }
	void setReserved(const uint32_t idx, const uint16_t newReservedValue) { assert(idx < nbOfReserved); m_reserved[idx] = newReservedValue; }
	// use getLabels

protected:
	uint32_t m_timeOffsetUs;
	NTPTime m_timestamp;
	uint32_t m_scanNumber;
	NTPTime m_scanMidTimestamp;
	uint16_t m_reserved[nbOfReserved];
	//uint16_t m_nbOfLabels
	LabelVector m_labels;
}; // ObjectLabelList6503

//======================================================================

bool operator==(const ObjectLabelList6503& lhs, const ObjectLabelList6503& rhs);
bool operator!=(const ObjectLabelList6503& lhs, const ObjectLabelList6503& rhs);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // OBJECTLABELLIST6503_HPP_SEEN

//======================================================================
