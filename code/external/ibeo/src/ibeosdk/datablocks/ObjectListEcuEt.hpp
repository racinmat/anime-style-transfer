//======================================================================
/*! \file ObjectListEcuEt.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 5, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_OBJECTLISTECUET_HPP_SEEN
#define IBEOSDK_OBJECTLISTECUET_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/ObjectEcuEt.hpp>
#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/misc/deprecatedwarning.hpp>
#include <ibeosdk/Time.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class ObjectListEcuEt : public ibeosdk::RegisteredDataBlock<ObjectListEcuEt> {
public:
	typedef std::vector<ObjectEcuEt> ObjectVector;
public:
	ObjectListEcuEt();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	IBEOSDK_DEPRECATED NTPTime getScanStartTimestamp() const { return m_timestamp; }
	NTPTime getTimestamp() const { return m_timestamp; }

	int getNbOfObjects() const { return UINT16(m_objects.size()); }
	const ObjectVector& getObjects() const { return m_objects; }
	ObjectVector& getObjects() { return m_objects; }

public: // setter
	void setTimestamp(const NTPTime& timestamp) { m_timestamp = timestamp; }
	void setObjects(const std::vector<ObjectEcuEt>& objects) { m_objects = objects; }

public:
	bool operator==(const ObjectListEcuEt& other) const;
	bool operator!=(const ObjectListEcuEt& other) const { return !((*this) == other); }

protected:
	NTPTime m_timestamp;
	// number of objects uint16_t
	std::vector<ObjectEcuEt> m_objects;
}; // ObjectListEcuET

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTLISTECUET_HPP_SEEN

//======================================================================
