//======================================================================
/*! \file ScalaFpgaRawScanPointBase.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Oct 23, 2015
 *///-------------------------------------------------------------------

#ifndef SCALAFPGARAWSCANPOINTBASE_HPP_SEEN
#define SCALAFPGARAWSCANPOINTBASE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/inttypes.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ScalaFpgaRawScanPointBase : public Snippet {
public:
	enum RawScanPointFlags {
		RSPF_Transparent        = 0x0001,
		RSPF_Rain               = 0x0002,
		RSPF_Ground             = 0x0004, // not used
		RSPF_Dirt               = 0x0008, // always unset

		RSPF_HighTresholdH1     = 0x0010,
		// invalid 0x0020
		RSPF_NearRange           = 0x0040,
		RSPF_HighTresholdH2     = 0x0080, // ==> MARKER

		RSPF_Noise              = 0x0100,
		RSPF_CandidateInvalid   = 0x0200, // unused
		RSPF_RainStep1Done      = 0x0400, // unused
		RSPF_RainStep2Done      = 0x0800,

		RSPF_GroundStep1Done    = 0x1000, // unused
		RSPF_GroundStep2Done    = 0x2000, // unused
		RSPF_BlueValidCaculated = 0x4000, // unused
		RSPF_Flushed            = 0x8000 //!< Scan point has completely preprocessed.
	};

public:
	static std::streamsize getSerializedSize_static() { return 16; }

public:
	ScalaFpgaRawScanPointBase();
	virtual ~ScalaFpgaRawScanPointBase();

public:
	//! Equality predicate
	bool operator==(const ScalaFpgaRawScanPointBase& other) const;
	bool operator!=(const ScalaFpgaRawScanPointBase& other) const;

public:
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	virtual uint16_t getBlockId() const = 0;

public:
	uint16_t getRadialDistance() const { return m_radialDistance; }
	uint16_t getEchoPulseWidth() const { return m_echoPulseWidth; }
	int16_t getAngle() const { return m_angle; }
	uint16_t getResolution() const { return m_resolution; }
	uint8_t getChannelId() const { return m_channelId; }
	uint8_t getEchoId() const { return m_echoId; }
	uint8_t getFlagsHigh() const { return m_flagsHigh; }
	uint8_t getFlagsLow() const { return m_flagsLow; }

public:
	void setRadialDistance(const uint16_t dist) { m_radialDistance = dist; }
	void setEchoPulseWidth(const uint16_t width) { m_echoPulseWidth = width; }
	void setAngle(const int16_t angle) { m_angle = angle; }
	void setResolution(const uint16_t res) { m_resolution = res; }
	void setChannelId(const uint8_t id) { m_channelId = id; }
	void setEchoId(const uint8_t id) { m_echoId = id; }
	void setFlagsHigh(const uint8_t flags) { m_flagsHigh = flags; }
	void setFlagsLow(const uint8_t flags) { m_flagsLow = flags; }

protected:
	uint16_t m_radialDistance;
	uint16_t m_echoPulseWidth;
	int16_t m_angle;
	uint16_t m_reserved;
	uint16_t m_resolution;
	uint8_t m_channelId;
	uint8_t m_echoId;
	uint8_t m_flagsHigh;
	uint8_t m_flagsLow;
}; // ScalaFpgaRawScanPointBase

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // SCALAFPGARAWSCANPOINTBASE_HPP_SEEN

//======================================================================
