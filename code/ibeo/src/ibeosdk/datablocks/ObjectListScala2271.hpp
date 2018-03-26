//======================================================================
/*! \file ObjectListScala2271.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 23, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_OBJECTLISTSCALA2271_HPP_SEEN
#define IBEOSDK_OBJECTLISTSCALA2271_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/ObjectScala2271.hpp>
#include <ibeosdk/Time.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ObjectListScala2271 : public ibeosdk::RegisteredDataBlock<ObjectListScala2271> {
public:
	typedef std::vector<ObjectScala2271> ObjectVector;

public:
	virtual ~ObjectListScala2271();

	ObjectListScala2271(const NTPTime scanStartTimeStamp);
	ObjectListScala2271();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	NTPTime getScanStartTimestamp() const { return m_scanStartTimestamp; }
	uint16_t getScanNumber() const { return m_scanNumber; }
	uint8_t getObjectListId() const { return m_objectListId; }
	uint8_t getDeviceId() const { return m_deviceId; }
	uint16_t getDeviceInterfaceVersion() const { return m_deviceInterfaceVersion; }

	uint32_t getReserved() const { return m_reserved; }

	uint16_t getNumberOfObjects() const { return UINT16(m_objects.size()); }
	const ObjectVector& getObjects() const { return m_objects; }
	ObjectVector& getObjects() { return m_objects; }

public:
	void setScanStartTimestamp(const NTPTime& newNTPTime) { m_scanStartTimestamp = newNTPTime; }
	void setScanNumber(const uint16_t newScanNumber) { m_scanNumber = newScanNumber; }
	void setObjectListId(const uint8_t newObjectListId) { m_objectListId = newObjectListId; }
	void setDeviceId(const uint8_t newDeviceId) { m_deviceId = newDeviceId; }
	//========================================
	/*!\brief Set the object list id. (14 bit value).
	 * \attention Only the lower 14 bits are used, i.e. the
	 *            device interface version is between 0 an 16383.
	 *///-------------------------------------
	void setDeviceInterfaceVersion(const uint16_t newDeviceInterfaceVersion)
	{
		assert ((newDeviceInterfaceVersion & 0x3FFF) == newDeviceInterfaceVersion);
		m_deviceInterfaceVersion = (newDeviceInterfaceVersion & 0x3FFF);
	}

	void setReserved(const uint32_t newReserved) { m_reserved = newReserved; }

protected:
	NTPTime m_scanStartTimestamp;
	uint16_t m_scanNumber;
	uint8_t m_objectListId;
	uint8_t m_deviceId;
	uint16_t m_deviceInterfaceVersion;
	uint32_t m_reserved;
	// uint16 number of objects
	std::vector<ObjectScala2271> m_objects;
}; // ObjectListScala2271

//======================================================================

bool operator==(const ObjectListScala2271& lhs, const ObjectListScala2271& rhs);
bool operator!=(const ObjectListScala2271& lhs, const ObjectListScala2271& rhs);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTLISTSCALA2271_HPP_SEEN

//======================================================================
