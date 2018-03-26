//======================================================================
/*! \file Scan2208.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Aug 2, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SCAN2208_HPP_SEEN
#define IBEOSDK_SCAN2208_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>

#include <ibeosdk/datablocks/snippets/SubScan2208.hpp>
#include <ibeosdk/datablocks/snippets/MountingPositionRaw.hpp>
#include <ibeosdk/datablocks/snippets/ScannerInfo.hpp>

#include <ibeosdk/inttypes.hpp>

#include <ibeosdk/Time.hpp>

#include <vector>
#include <cassert>

//======================================================================

namespace ibeosdk {

//======================================================================

class Scan2208 : public RegisteredDataBlock<Scan2208> {
public:
	static const int nbOfThresholds = 7;
	static const int nbOfReserved = 12;
	static const int maxSubScans = 2;

public:
	enum ScannerStatusBits {
		SS_MotorOn = 0x0001,
		SS_Laser1On = 0x0002,
		SS_DemoModeOn = 0x0004,
		SS_FrequencyLocked = 0x0008,
		// reserved 0x0010
		// reserved 0x0020
		// reserved 0x0040
		SS_Laser2On = 0x0080,
		SS_MotorDirectionConterClockwise = 0x0100
		// rest is reserved
	}; // ScannerStatusBits

public:
	typedef std::vector<SubScan2208> SubScanVector;

public:
	Scan2208();
	virtual ~Scan2208();

public:
	//! Equality predicate
	bool operator==(const Scan2208& other) const;
	bool operator!=(const Scan2208& other) const;

public:
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	uint16_t getScanNumber() const { return scanNumber; }
	ibeosdk::ScannerType getScannerType() const { return scannerType; }
	uint16_t getScannerStatus() const { return scannerStatus; }
	uint32_t getProcessingFlags() const { return processingFlags; }
	uint16_t getAngleTicksPerRotation() const { return angleTicksPerRotation; }
	const MountingPositionRaw& getMountingPosition() const { return mountingPosition; }
	MountingPositionRaw& getMountingPosition() { return mountingPosition; }
	uint16_t getThreshold(const int idx) const { return thresholds[idx]; }
	uint8_t getReserved(const int idx) const { return reserved[idx]; }
	uint16_t getDeviceId() const { return deviceId; }
	uint8_t getNbOfSubScans() const { return uint8_t(subScans.size()); }

	const SubScanVector& getSubScans() const { return this->subScans; }
	SubScanVector& getSubScans() { return this->subScans; }

public:
	void setScanNumber(const uint16_t newScanNumber) { scanNumber = newScanNumber; }
	void setScannerType(const ibeosdk::ScannerType newScannerType) { scannerType = newScannerType; }
	void setScannerStatus(const uint16_t newScannerStatus) { scannerStatus = newScannerStatus; }
	void setProcessingFlags(const uint32_t newProcessingFlags) { processingFlags = newProcessingFlags; }
	void setAngleTicksPerRotation(const uint16_t newAngleTicksPerRotation) { angleTicksPerRotation = newAngleTicksPerRotation; }
	void setThreshold(const int idx, const uint16_t newThresholds) { thresholds[idx] = newThresholds; }
	void setReserved(const int idx, const uint8_t newReserved) { reserved[idx] = newReserved; }
	void setDeviceId(const uint8_t newDeviceId) { deviceId = newDeviceId; }


protected:
	uint16_t scanNumber;
	ibeosdk::ScannerType scannerType;
	uint16_t scannerStatus;
	uint16_t angleTicksPerRotation;
	uint32_t processingFlags;
	MountingPositionRaw mountingPosition;
	uint16_t thresholds[nbOfThresholds];
	uint8_t reserved[nbOfReserved];
	uint8_t deviceId;
	// uint8_t number of sub scans.
	SubScanVector subScans;
}; // Scan2208

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCAN2208_HPP_SEEN

//======================================================================

