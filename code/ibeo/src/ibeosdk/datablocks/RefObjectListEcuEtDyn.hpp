//======================================================================
/*! \file RefObjectListEcuEtDyn.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Mar 05, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_REFOBJECTLISTECUETDYN_HPP_SEEN
#define IBEOSDK_REFOBJECTLISTECUETDYN_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/ObjectEcuEtDyn.hpp>
#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/Time.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class RefObjectListEcuEtDyn : public ibeosdk::RegisteredDataBlock<RefObjectListEcuEtDyn> {
public:
	enum Flags {
		CoordSystem_MiddleRearAxisISO70000 = 0x00,
		CoordSystem_MiddleFrontAxis        = 0x01,
		CoordSystem_Mask                   = 0x0F,
		Flags_ReservedMask                 = 0xF0
	};

public:
	RefObjectListEcuEtDyn();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	NTPTime getTimestamp() const { return m_timestamp; }
	UINT8 getObjectListId() const { return m_objectListId; }
	UINT8 getDeviceType() const { return m_deviceType; }
	UINT16 getDeviceInterfaceVersion() const { return m_deviceInterfaceVersion; }

	bool coordSystemIsMiddleRearAxis() const { return (m_flags & CoordSystem_Mask) == CoordSystem_MiddleRearAxisISO70000; }
	bool coordSystemIsMiddleFrontAxis() const { return (m_flags & CoordSystem_Mask) == CoordSystem_MiddleFrontAxis; }

	UINT8 getFlags() const { return m_flags; }

	UINT16 getNbOfObjects() const { return UINT16(m_objects.size()); }
	const std::vector<ObjectEcuEtDyn>& getObjects() const { return m_objects; }
	std::vector<ObjectEcuEtDyn>& getObjects() { return m_objects; }

public: // setter
	void setTimestamp(const NTPTime& newTimestamp) { m_timestamp = newTimestamp; }
	void setObjectListId(const UINT8 newObjectListId) { m_objectListId = newObjectListId; }
	void setDeviceType(const UINT8 newDeviceType) { m_deviceType = newDeviceType; }
	void setDeviceInterfaceVerstion(const UINT16 newDeviceInterfaceVersion) { m_deviceInterfaceVersion = newDeviceInterfaceVersion; }

	void setCoordSystemToMiddleRearAxis() { m_flags = uint8_t(m_flags & ~uint8_t(CoordSystem_Mask)) | uint8_t(CoordSystem_MiddleRearAxisISO70000); }
	void setCoordSystemToISO70000() { this->setCoordSystemToMiddleRearAxis(); }
	void setCoordSystemToMiddleFrontAxis(){ m_flags = uint8_t(uint8_t(m_flags & ~uint8_t(CoordSystem_Mask)) | uint8_t(CoordSystem_MiddleFrontAxis)); }

	void setFlags(const UINT8 newFlags) { m_flags = newFlags; }

protected:
	NTPTime m_timestamp;
	UINT8 m_objectListId;
	UINT8 m_deviceType;
	UINT16 m_deviceInterfaceVersion;
	UINT8 m_flags;
	UINT8 m_reserved1;
	std::vector<ObjectEcuEtDyn> m_objects;
}; // RefObjectListEcuEtDyn

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_REFOBJECTLISTECUETDYN_HPP_SEEN

//======================================================================
