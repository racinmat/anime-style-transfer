//======================================================================
/*! \file ObjectListEcu.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_OBJECTLISTECU_HPP_SEEN
#define IBEOSDK_OBJECTLISTECU_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/misc/deprecatedwarning.hpp>
#include <ibeosdk/datablocks/snippets/ObjectEcu.hpp>
#include <ibeosdk/Time.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class ObjectListEcu : public RegisteredDataBlock<ObjectListEcu> {
public:
	ObjectListEcu();

public: // getter
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	IBEOSDK_DEPRECATED NTPTime getScanStartTimestamp() const { return m_timestamp; }
	NTPTime getTimestamp() const { return m_timestamp; }

	UINT16 getNumberOfObjects() const { return UINT16(m_objects.size()); }
	const std::vector<ObjectEcu>& getObjects() const { return m_objects; }
	std::vector<ObjectEcu>& getObjects() { return m_objects; }

public: // setter
	IBEOSDK_DEPRECATED void setScanStartTimestamp(const NTPTime newScanStartTimeStamp) { m_timestamp = newScanStartTimeStamp; }
	void setTimestamp(const NTPTime newTimeStamp) { m_timestamp = newTimeStamp; }
	void setObjects(const std::vector<ObjectEcu>& newObjects) { m_objects = newObjects; }
	
public:
	bool operator==(const ObjectListEcu& other) const;
	bool operator!=(const ObjectListEcu& other) const { return !((*this) == other); }

protected:
	NTPTime m_timestamp;
	std::vector<ObjectEcu> m_objects;
}; // ObjectListEcu

//======================================================================

std::string getObjectIdsAsString(const ObjectListEcu& objList);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTLISTECU_HPP_SEEN

//======================================================================

