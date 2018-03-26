//======================================================================
/*! \file ObjectListLux.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_OBJECTLISTLUX_HPP_SEEN
#define IBEOSDK_OBJECTLISTLUX_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/ObjectLux.hpp>
#include <ibeosdk/Time.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class ObjectListLux : public RegisteredDataBlock<ObjectListLux> {
public:
	ObjectListLux();
	ObjectListLux(const NTPTime scanStartTimeStamp);
	virtual ~ObjectListLux();


public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	UINT16 getNumberOfObjects() const { return UINT16(m_objects.size()); }
	const std::vector<ObjectLux>& getObjects() const { return m_objects; }
	std::vector<ObjectLux>& getObjects() { return m_objects; }
	NTPTime getScanStartTimestamp() const { return m_scanStartTimestamp; }

	void addObject(const ObjectLux&);
	const ObjectLux* findObject(const UINT16 id) const;

public:
	void setScanStartTimeStamp(const NTPTime newScanStartTimeStamp) { m_scanStartTimestamp = newScanStartTimeStamp; }

protected:
	// this is also the maximum number of objects the LUX3/LUX4
	// is able to send. The number can be exceed by the Lux8L.
	// There it is just a hint and not a hard code limit for the
	// maximal number of objects int the object list.
	static const UINT16 nbOfObjsToBeReserveInVector = 63;

protected:
	NTPTime m_scanStartTimestamp;
	std::vector<ObjectLux> m_objects;
}; // ObjectListLux

//======================================================================

std::string getObjectIdsAsString(const ObjectListLux& objList);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTLISTLUX_HPP_SEEN

//======================================================================

