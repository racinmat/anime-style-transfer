//======================================================================
/*! \file SubScan2208.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Aug 2, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SUBSCAN2208_HPP_SEEN
#define IBEOSDK_SUBSCAN2208_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/ScanPoint2208.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/Time.hpp>

#include <vector>
#include <istream>
#include <ostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class SubScan2208 : public Snippet {
public:
	static const int nbOfReserved = 6;
	static const int maxNbOfPoints = 9*581;

public:
	typedef std::vector<ScanPoint2208> PointVector;
	enum SubScanFlags {
		SSF_Laser = 0x01 //!< id of the used laser
		// reserved
	};
public:
	SubScan2208();
	virtual ~SubScan2208();

public:
	//! Equality predicate
	bool operator==(const SubScan2208& other) const;

	bool operator!=(const SubScan2208& other) const;

public:
	virtual std::streamsize getSerializedSize() const { return 36 + ScanPoint2208::getSerializedSize_static() * std::streamsize(scanPoints.size()); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	NTPTime getStartScanTimestamp() const { return startScanTimestamp; }
	NTPTime getEndScanTimestamp() const { return endScanTimestamp; }
	int16_t getStartScanAngle() const { return startScanAngle; }
	int16_t getEndScanAngle() const { return endScanAngle; }
	uint8_t getFlags() const { return flags; }
	uint8_t getMirrorSide() const { return mirrorSide; }
	float getMirrorTiltDeprecated() const { return mirrorTiltDeprecated; }
	uint16_t getMirrorTilt() const { return mirrorTilt; }
	uint8_t getReserved(const int idx) const { return reserved[idx]; }
	uint32_t getNbOfPoints() const { return UINT16(scanPoints.size()); }

	const PointVector& getScanPoints() const { return scanPoints; }
	PointVector& getScanPoints() { return scanPoints; }

public:
	void setStartScanTimestamp(const NTPTime newStartScanTimestamp) { startScanTimestamp = newStartScanTimestamp; }
	void setEndScanTimestamp(const NTPTime newEndScanTimestamp) { endScanTimestamp = newEndScanTimestamp; }
	void setStartScanAngle(const int16_t newStartScanAngle) { startScanAngle = newStartScanAngle; }
	void setEndScanAngle(const int16_t newEndScanAngle) { endScanAngle = newEndScanAngle; }
	void setFlags(const uint8_t newFlags) { flags = newFlags; }
	void setMirrorSide(const uint8_t newMirrorSide) { mirrorSide = newMirrorSide; }
	void setMirrorTiltDeprecated(const float newMirrorTiltDeprecated) { mirrorTiltDeprecated = newMirrorTiltDeprecated; }
	void setMirrorTilt(const uint16_t newMirrorTilt) { mirrorTilt = newMirrorTilt; }
	void setReserved(const int idx, const uint8_t newReserved) { reserved[idx] = newReserved; }

protected:
	NTPTime startScanTimestamp;
	NTPTime endScanTimestamp;
	int16_t startScanAngle;
	int16_t endScanAngle;
	uint8_t flags;
	uint8_t mirrorSide;
	float mirrorTiltDeprecated; //!< deg
	uint16_t mirrorTilt; //!< 1/500 deg
	uint8_t reserved[nbOfReserved];
	// uint16_t nbOfPoints
	PointVector scanPoints;
}; // SubScan2208

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SUBSCAN2208_HPP_SEEN#

//======================================================================

