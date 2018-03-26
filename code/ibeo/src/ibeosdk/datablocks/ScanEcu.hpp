//======================================================================
/*! \file ScanEcu.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 25, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SCANECU_HPP_SEEN
#define IBEOSDK_SCANECU_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>
#include <ibeosdk/datablocks/snippets/ScannerInfo.hpp>
#include <ibeosdk/datablocks/snippets/ScanPointEcu.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class ScanEcu : public RegisteredDataBlock<ScanEcu> {
public:
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
		Flags_FusedScan     = 0x0200, //!< Scan is the result of a fusion.
		Flags_MirrsorSide   = 0x0400, //!< \b 0 = front, \b 1 = rear
		Flags_CoordSystem   = 0x0800, //!< \b 0 = scanner coordinates, \b 1 = vehicle coordinates
		Flags_ObjIdAsSegId  = 0x1000  ///< SegmentID of scan points is use as ObjID
		// bit 13, 0x2000, reserved
		// bit 14, 0x4000, reserved
		// bit 15, 0x8000, reserved
	}; // Flags

public:
	ScanEcu();
	virtual ~ScanEcu();

public:
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;
	virtual std::streamsize getSerializedSize() const;

public:
	NTPTime getStartTimestamp() const { return m_scanStartTime; }
	UINT32 getEndTimeOffset() const { return m_endTimeOffset; }
	UINT32 getFlags() const { return m_flags; }
	UINT16 getScanNumber() const { return m_scanNumber; }
	UINT8 getNumberOfScannerInfos() const { return UINT8(m_scannerInfos.size()); }
	const std::vector<ScannerInfo>& getScannerInfos() const { return m_scannerInfos; }
	std::vector<ScannerInfo>& getScannerInfos() { return m_scannerInfos; }

	UINT16 getNumberOfScanPoints() const { return UINT16(m_scanPoints.size()); }

	UINT8 getReserved0() const { return m_reserved0; }
	UINT16 getReserved1() const { return m_reserved1; }

	const std::vector<ScanPointEcu>& getScanPoints() const { return m_scanPoints; }
	std::vector<ScanPointEcu>& getScanPoints() { return m_scanPoints; }

public:
	void setStartTimestamp(const NTPTime newScanStartTime) { m_scanStartTime = newScanStartTime; }
	void setEndTimeOffset(const UINT32 newEndTimeOffset) { m_endTimeOffset = newEndTimeOffset; }
	void setFlags(const UINT32 newFlags) { m_flags = newFlags; }
	void setScanNumber(const UINT16 newScanNumber) { m_scanNumber = newScanNumber; }
	void setReserved0(const UINT8 newReserved0) { m_reserved0 = newReserved0; }
	void setReserved1(const UINT16 newReserved1) { m_reserved1 = newReserved1; }
	void setScannerInfos(const std::vector<ScannerInfo> newScannerInfos) { m_scannerInfos = newScannerInfos; }
	void setScanPoints(const std::vector<ScanPointEcu> newScanPoints) { m_scanPoints = newScanPoints; }

public:
	bool operator==(const ScanEcu& other) const;
	bool operator!=(const ScanEcu& other) const { return !( (*this) == other );}

protected:
	NTPTime m_scanStartTime;
	UINT32 m_endTimeOffset;
	UINT32 m_flags;
	UINT16 m_scanNumber;
//	UINT16 number of scan points;
//	UINT8 number of scan infos;
	UINT8 m_reserved0;
	UINT16 m_reserved1;
	std::vector<ScannerInfo> m_scannerInfos;
	std::vector<ScanPointEcu> m_scanPoints;
}; // ScanEcu

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCANECU_HPP_SEEN

//======================================================================

