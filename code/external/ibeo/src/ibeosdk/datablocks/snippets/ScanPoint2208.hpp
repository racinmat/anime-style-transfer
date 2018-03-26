//======================================================================
/*! \file ScanPoint2208.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Aug 2, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SCANPOINT2208_HPP_SEEN
#define IBEOSDK_SCANPOINT2208_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <istream>
#include <ostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class ScanPoint2208 : public Snippet {
public:
	enum ScanPoint2208Flags {
		SPFML_Transparent   = 0x0001,
		SPFML_Rain          = 0x0002,
		SPFML_Ground        = 0x0004,
		SPFML_Dirt          = 0x0008,

		SPFML_HighThreshold = 0x0010,
		SPFML_Noise         = 0x0020,
		SPFML_NearRange     = 0x0040,
		SPFML_Marker        = 0x0080,

		SPFML_Blooming      = 0x0100,
		SPFML_Background    = 0x0200,
		SPFML_Reserved3     = 0x0400,
		SPFML_Reserved4     = 0x0800,

		SPFML_Reflector     = 0x1000,
		SPFML_Reserved5     = 0x2000,
		SPFML_InterlacingPt = 0x4000, // only for A1 prototype
		SPFML_Reserved7     = 0x8000
	};

public:
	ScanPoint2208();
	virtual ~ScanPoint2208();

public:
	//! Equality predicate
	bool operator==(const ScanPoint2208& other) const;

	bool operator!=(const ScanPoint2208& other) const;

public:
	static std::streamsize getSerializedSize_static() { return 11; }

public:
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	uint8_t getThresholdId() const { return thresholdId; }
	uint8_t getEchoId() const { return echoId; }
	uint8_t getReserved() const { return reserved; }
	uint8_t getLayerId() const { return layerId; }
	uint16_t getFlags() const { return flags; }
	int16_t getHorizontalAngle() const { return horizontalAngle; }
	uint16_t getRadialDistance() const { return radialDistance; }
	uint16_t getEchoPulseWidth() const { return echoPulseWidth; }
	uint8_t getPfValue() const { return pfValue; }

public:
	void setThresholdId(const uint8_t newThresholdId) { this->thresholdId=newThresholdId & 0x0f; } // 4 bit
	void setEchoId(const uint8_t newEchoId) { this->echoId=newEchoId & 0x03; } // 2 bit
	void setReserved(const uint8_t newReserved) { this->reserved = newReserved & 0x03; } // 2 bit
	void setLayerId(const uint8_t newLayerId) { this->layerId = newLayerId; }
	void setFlags(const uint16_t newFlags) { this->flags=newFlags; }
	void setHorizontalAngle(const int16_t newHorizontalAngle) { this->horizontalAngle=newHorizontalAngle; }
	void setRadialDistance(const uint16_t newRadialDistance) { this->radialDistance=newRadialDistance; }
	void setEchoPulseWidth(const uint16_t newEchoPulseWidth) { this->echoPulseWidth=newEchoPulseWidth; }
	void setPfValue(const uint8_t newPfValue) { this->pfValue=newPfValue; }

public:
//	static const uint8_t undefinedThresholdId = 0xF; // 0xE
protected:
	uint8_t thresholdId; // 4 bit
	uint8_t echoId; // 2 bit
	uint8_t reserved; // 2bit

	uint8_t layerId;
	uint16_t flags;
	int16_t horizontalAngle;
	uint16_t radialDistance;
	uint16_t echoPulseWidth;
	uint8_t pfValue;
}; // ScanPoint2208

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCANPOINT2208_HPP_SEEN

//======================================================================

