//======================================================================
/*! \file ScalaFpgaRawHeader.hpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SCALAFPGARAWHEADER_HPP_SEEN
#define IBEOSDK_SCALAFPGARAWHEADER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/inttypes.hpp>

#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <istream>
#include <ostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class ScalaFpgaRawHeader : public Snippet {
public:
	static std::streamsize getSerializedSize_static() { return 16; }

public:
	ScalaFpgaRawHeader();
	virtual ~ScalaFpgaRawHeader();

public:
	//! Equality predicate
	bool operator==(const ScalaFpgaRawHeader& other) const;

	bool operator!=(const ScalaFpgaRawHeader& other) const;

public:
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	uint16_t getScanCounter() const { return m_scanCounter; }
	uint16_t getMinApdOffset() const { return m_minApdOffset; }
	uint16_t getMaxApdOffset() const { return m_maxApdOffset; }
	uint16_t getFrequencyInteger() const { return m_frequencyInteger; }
	uint16_t getFrequencyFractional() const { return m_frequencyFractional; }
	IBEOSDK_DEPRECATED uint16_t getFreqencyFractional() const { return m_frequencyFractional; }
	uint16_t getDeviceId() const { return m_deviceId; }

public:
	void setScanCounter(const uint16_t scanCounter) { m_scanCounter = scanCounter; }
	IBEOSDK_DEPRECATED void setMminApdOffset(const uint16_t minApdOffset) {m_minApdOffset = minApdOffset; }
	void setMinApdOffset(const uint16_t minApdOffset) {m_minApdOffset = minApdOffset; }
	void setMaxApdOffset(const uint16_t maxApdOffset) { m_maxApdOffset = maxApdOffset; }
	void setFrequencyInteger(const uint16_t freqInteger) { m_frequencyInteger = freqInteger; }
	void setFrequencyFractional(const uint16_t freqFrac) { m_frequencyFractional = freqFrac; }
	IBEOSDK_DEPRECATED void setFreqencyFractional(const uint16_t freqFrac) { m_frequencyFractional = freqFrac; }
	void setDeviceId(const uint16_t deviceId) { m_deviceId = deviceId; }

public:
	static const uint16_t blockId;

protected:
	uint16_t m_scanCounter;
	uint16_t m_minApdOffset;
	uint16_t m_maxApdOffset;
	uint16_t m_frequencyInteger;
	uint16_t m_frequencyFractional;
	uint16_t m_deviceId;
	uint16_t m_reservedHeader7;
}; // ScalaFpgaRawHeader

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCALAFPGARAWHEADER_HPP_SEEN

//======================================================================

