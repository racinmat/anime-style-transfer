//======================================================================
/*! \file ObjectListEcuEtDyn.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Mar 14, 2014
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_OBJECTLISTECUETDYN_HPP_SEEN
#define IBEOSDK_OBJECTLISTECUETDYN_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/ObjectEcuEtDyn.hpp>
#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/Time.hpp>

#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class ObjectListEcuEtDyn : public ibeosdk::RegisteredDataBlock<ObjectListEcuEtDyn> {
public:
	typedef ObjectEcuEtDyn ObjectType;
	typedef std::vector<ObjectType> ObjectVector;

public:
	static const uint8_t flagBits_MiddleRearAxisISO70000 = 0x00;
	static const uint8_t flagBits_MiddleFrontAxis        = 0x01;
	static const uint8_t flagBits_CoordianteSystemMask   = 0x0F;

	static const uint8_t flagBits_isRefObjList           = 0x10;

	static const uint8_t flagBits_reservedMask           = 0xE0;

	enum DeviceType {
		DeviceType_Undefined    =   0,
		DeviceType_Laserscanner =   1,
		DeviceType_Radar        =  50,
		DeviceType_Camera       =  60,
		DeviceType_Reference    =  90,
		DeviceType_DuT          = 120
	};

public:
	ObjectListEcuEtDyn();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	NTPTime getTimestamp() const { return m_timestamp; }
	UINT8 getObjectListId() const { return m_objectListId; }
	DeviceType getDeviceType() const { return m_deviceType; }
	UINT16 getDeviceInterfaceVersion() const { return m_deviceInterfaceVersion; }

	bool coordSystemIsMiddleRearAxis() const { return (m_flags & flagBits_CoordianteSystemMask) == flagBits_MiddleRearAxisISO70000; }
	bool coordSystemIsMiddleFrontAxis() const { return (m_flags & flagBits_CoordianteSystemMask) == flagBits_MiddleFrontAxis; }
	bool isRefObjList() const { return (m_flags & flagBits_isRefObjList) == flagBits_isRefObjList; }

	UINT8 getFlags() const { return m_flags; }
	UINT8 getReserved1() const { return m_reserved1;}

	UINT16 getNbOfObjects() const { return UINT16(m_objects.size()); }
	const ObjectVector& getObjects() const { return m_objects; }
	ObjectVector& getObjects() { return m_objects; }

public: // setter
	void setTimestamp(const NTPTime& newTimestamp) { m_timestamp = newTimestamp; }
	void setObjectListId(const UINT8 newObjectListId) { m_objectListId = newObjectListId; }
	IBEOSDK_DEPRECATED void setDeviceType(const uint8_t newDeviceType) { m_deviceType = DeviceType(newDeviceType); }
	void setDeviceType(const DeviceType newDeviceType) { m_deviceType = newDeviceType; }
	void setDeviceInterfaceVerstion(const UINT16 newDeviceInterfaceVersion) { m_deviceInterfaceVersion = newDeviceInterfaceVersion; }

	void setCoordSystemToMiddleRearAxis() { m_flags = uint8_t(m_flags & ~flagBits_CoordianteSystemMask) | flagBits_MiddleRearAxisISO70000; }
	void setCoordSystemToISO70000() { this->setCoordSystemToMiddleRearAxis(); }
	void setCoordSystemToMiddleFrontAxis(){ m_flags = uint8_t(uint8_t(m_flags & ~flagBits_CoordianteSystemMask) | flagBits_MiddleFrontAxis); }

	void setToBeRefObjList(const bool toBe)
	{
		if (toBe) m_flags = uint8_t(m_flags | flagBits_isRefObjList);
		else m_flags = uint8_t(m_flags & ~flagBits_isRefObjList);
	}

	void setFlags(const UINT8 newFlags) { m_flags = newFlags; }
	void setReserved1(const UINT8 newReserved1) {m_reserved1 = newReserved1;}

	void setObjects(const ObjectListEcuEtDyn::ObjectVector& newObjects) {m_objects = newObjects;}

public:
	bool operator==(const ObjectListEcuEtDyn& other) const;
	bool operator!=(const ObjectListEcuEtDyn& other) const { return !((*this) == other); }

protected:
	NTPTime m_timestamp;
	UINT8 m_objectListId;
	DeviceType m_deviceType;
	UINT16 m_deviceInterfaceVersion;
	UINT8 m_flags;
	UINT8 m_reserved1;
	ObjectVector m_objects;
}; // ObjectListEcuEtDyn

//======================================================================

template<>
void readBE<ObjectListEcuEtDyn::DeviceType>(std::istream& is, ObjectListEcuEtDyn::DeviceType& dt);

template<>
void writeBE<ObjectListEcuEtDyn::DeviceType>(std::ostream& os, const ObjectListEcuEtDyn::DeviceType& dt);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTLISTECUETDYN_HPP_SEEN

//======================================================================
