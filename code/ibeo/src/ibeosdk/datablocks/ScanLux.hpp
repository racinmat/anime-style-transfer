//======================================================================
/*! \file ScanLux.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 25, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SCANLUX_HPP_SEEN
#define IBEOSDK_SCANLUX_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>
#include <ibeosdk/datablocks/snippets/ScanPointLux.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class ScanLux : public RegisteredDataBlock<ScanLux> {
public:
	enum ScannerStatus {
		ScSt_MotorOn            = 0x0001, //!< Motor on.
		ScSt_LaserOn            = 0x0002, //!< Laser on.
		ScSt_InternalFeedback   = 0x0004, //!< Internal feedback.
		ScSt_SetFreqReached     = 0x0008, //!< Set frequency reached.
		ScSt_ExtSyncSigDetected = 0x0010, //!< External sync signal detected.
		ScSt_SyncOk             = 0x0020, //!< Sync is ok.
		ScSt_SyncMaster         = 0x0040, //!< Is sync master and not slave.
		// 0x0080 reserved
		ScSt_EpwCompOn          = 0x0100, //!< EPW compensation on.
		ScSt_SystemCompOn       = 0x0200, //!< System compensation on.
		ScSt_StartPulseCompOn   = 0x0400, //!< Start pulse compensation on.
		// 0x0800 reserved
		// 0x1000 reserved
		// 0x2000 reserved
		// 0x4000 reserved
		ScSt_UpsideDown         = 0x8000 //!< Sensor is upside down, FPGA version >= 0x9604.
	}; // ScannerStatus

	//========================================

	enum Flags {
		Flags_GroundLabeled = 0x0001, //!< Ground is labeled.
		Flags_DirtLabeled   = 0x0002, //!< Dirt is labeled.
		Flags_RainLabeled   = 0x0004, //!< Rain is labeled.
		// bit  3, 0x0008, reserved
		// bit  4, 0x0010, internal
		// bit  5, 0x0020, internal
		// bit  6, 0x0040, internal
		// bit  7, 0x0080, reserved
		// bit  8, 0x0100, reserved
		// bit  9, 0x0200, reserved
		Flags_MirrsorSide   = 0x0400 //!< \b 0 = front, \b 1 = rear
		// bit 11, 0x0800, reserved
		// bit 12, 0x1000, reserved
		// bit 13, 0x2000, reserved
		// bit 14, 0x4000, reserved
		// bit 15, 0x8000, reserved
	}; // Flags

	//========================================

public:
	ScanLux();
	virtual ~ScanLux();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	UINT16 getScanNumber() const { return m_scanNumber; }
	UINT16 getScannerStatus() const { return m_scannerStatus; }
	UINT16 getSyncPhaseOffset() const { return m_syncPhaseOffset; }

	NTPTime getStartTimestamp() const { return m_startNTP; }
	NTPTime getEndTimestamp() const { return m_endNTP; }

	UINT16 getAngleTicksPerRotation() const { return m_angleTicksPerRotation; }

	INT16 getStartAngle() const { return m_startAngle; }
	INT16 getEndAngle() const { return m_endAngle; }

	INT16 getMountingPositionYawAngle() const { return m_mountingPosYawAngle; }
	INT16 getMountingPositionPitchAngle() const { return m_mountingPosPitchAngle; }
	INT16 getMountingPositionRollAngle() const { return m_mountingPosRollAngle; }

	INT16 getMountingPositionX() const { return m_mountingPosX; }
	INT16 getMountingPositionY() const { return m_mountingPosY; }
	INT16 getMountingPositionZ() const { return m_mountingPosZ; }

	UINT16 getFlags() const { return m_flags; }
	UINT16 getNumberOfScanPoints() const { return UINT16(m_points.size()); }

	const std::vector<ScanPointLux>& getScanPoints() const { return m_points; }
	std::vector<ScanPointLux>& getScanPoints() { return m_points; }

public: // setter
	void setScanNumber(const UINT16 newScanNumber) { this->m_scanNumber = newScanNumber; }
	void setScannerStatus(const UINT16 newScannerStatus) { this->m_scannerStatus = newScannerStatus; }
	void setSyncPhaseOffset(const UINT16 newSyncPhaseOffset) { this->m_syncPhaseOffset = newSyncPhaseOffset; }

	void setStartTimestamp(const NTPTime newStartTimestamp) { this->m_startNTP = newStartTimestamp; }
	void setEndTimestamp(const NTPTime newEndTimestamp) { this->m_endNTP = newEndTimestamp; }

	void setAngleTicksPerRotation(const UINT16 newAngleTicksPerRotation) { this->m_angleTicksPerRotation = newAngleTicksPerRotation; }

	void setStartAngle(const INT16 newStartAngle) { this->m_startAngle = newStartAngle; }
	void setEndAngle(const INT16 newEndAngle) { this->m_endAngle = newEndAngle; }

	void setMountingPositionYawAngle(const INT16 newMountingPositionYawAngle) { this->m_mountingPosYawAngle = newMountingPositionYawAngle; }
	void setMountingPositionPitchAngle(const INT16 newMountingPositionPitchAngle) { this->m_mountingPosPitchAngle = newMountingPositionPitchAngle; }
	void setMountingPositionRollAngle(const INT16 newMountingPositionRollAngle) { this->m_mountingPosRollAngle = newMountingPositionRollAngle; }

	void setMountingPositionX(const INT16 newMountingPositionX) { this->m_mountingPosX = newMountingPositionX; }
	void setMountingPositionY(const INT16 newMountingPositionY) { this->m_mountingPosY = newMountingPositionY; }
	void setMountingPositionZ(const INT16 newMountingPositionZ) { this->m_mountingPosZ = newMountingPositionZ; }

	void setFlags(const UINT16 newFlags) { this->m_flags = newFlags; }
	void setNumberOfScanPoints(const UINT16 newNumberOfScanPoints)
	{
		this->m_points.resize(newNumberOfScanPoints);
	}
	void setScanPoint(const unsigned int idx, const ScanPointLux& sp)
	{
		this->m_points[idx] = sp;
	}

protected:
	bool timeCheck() const;
	bool nbOfAngleTicksPerRotationCheck() const;
	bool nbOfPtsCheck(const UINT16 nbScanPts) const;

protected:
	static const UINT16 nbOfAngleTicksPerRotation = 11520;
	static const UINT16 tickPerDegree = 32;
	static const INT16 angleRangeMin = -60*tickPerDegree;
	static const INT16 angleRangeMax = 50*tickPerDegree;

protected:
	UINT16 m_scanNumber;
	UINT16 m_scannerStatus;
	UINT16 m_syncPhaseOffset;
	NTPTime m_startNTP;
	NTPTime m_endNTP;
	UINT16 m_angleTicksPerRotation;
	INT16 m_startAngle;
	INT16 m_endAngle;
	// m_points.size() UINT16
	INT16 m_mountingPosYawAngle;
	INT16 m_mountingPosPitchAngle;
	INT16 m_mountingPosRollAngle;
	INT16 m_mountingPosX;
	INT16 m_mountingPosY;
	INT16 m_mountingPosZ;
	UINT16 m_flags;
	std::vector<ScanPointLux> m_points;
}; // ScanLux

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCANLUX_HPP_SEEN

//======================================================================

