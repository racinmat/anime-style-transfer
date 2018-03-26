//======================================================================
/*! \file RefObjectListEcuEt.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 5, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_REFOBJECTLISTECUET_HPP_SEEN
#define IBEOSDK_REFOBJECTLISTECUET_HPP_SEEN

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

class RefObjectListEcuEt : public ibeosdk::RegisteredDataBlock<RefObjectListEcuEt> {
public:
	RefObjectListEcuEt();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	IBEOSDK_DEPRECATED NTPTime getScanStartTimestamp() const { return m_timestamp; }
	NTPTime getTimestamp() const { return m_timestamp; }

	int getNbOfObjects() const { return UINT16(m_objects.size()); }
	const std::vector<ObjectEcuEt>& getObjects() const { return m_objects; }
	std::vector<ObjectEcuEt>& getObjects() { return m_objects; }

public: // setter
	void setTimestamp(const NTPTime& timestamp) { m_timestamp = timestamp; }

protected:
	NTPTime m_timestamp;
	// number of objects uint16_t
	std::vector<ObjectEcuEt> m_objects;
}; // RefObjectListEcuEt

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_REFOBJECTLISTECUET_HPP_SEEN

//======================================================================
