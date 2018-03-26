//======================================================================
/*! \file ObjectListScala.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 17, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_OBJECTLISTSCALA_HPP_SEEN
#define IBEOSDK_OBJECTLISTSCALA_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/ObjectScala.hpp>
#include <ibeosdk/Time.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ObjectListScala : public ibeosdk::RegisteredDataBlock<ObjectListScala> {
public:
	virtual ~ObjectListScala();
	ObjectListScala(const NTPTime scanStartTimeStamp);
	ObjectListScala();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	NTPTime getScanStartTimestamp() const { return m_scanStartTimestamp; }
	UINT16 getScanNumber() const { return m_scanNumber; }
	UINT16 getNumberOfObjects() const { return UINT16(m_objects.size()); }
	const std::vector<ObjectScala>& getObjects() const { return m_objects; }
	std::vector<ObjectScala>& getObjects() { return m_objects; }

	const ObjectScala* findObject(const UINT16 id) const;

public:
	void setScanStartTimeStamp(const NTPTime newScanStartTimeStamp) { m_scanStartTimestamp = newScanStartTimeStamp; }
	void setScanNumber(const uint16_t newScanNumber) { m_scanNumber = newScanNumber; }

	void addObject(const ObjectScala& newObj);

protected:
	NTPTime m_scanStartTimestamp;
	uint16_t m_scanNumber;
	// uint16 number of objects
	std::vector<ObjectScala> m_objects;
}; // ObjectListScala

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTLISTSCALA_HPP_SEEN

//======================================================================
