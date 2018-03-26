//======================================================================
/*! \file ObjectAssociation4001.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 26, 2016
 *///-------------------------------------------------------------------

#ifndef OBJECTASSOCIATION4001_HPP_SEEN
#define OBJECTASSOCIATION4001_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/Time.hpp>
#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ObjectAssociation4001 : public Snippet {
public:
	ObjectAssociation4001();

public: // getter
	uint32_t getReferenceObjId() const { return m_refObjId; }
	uint32_t getDutObjId() const { return m_dutObjId; }
	NTPTime getTimestampFirst() const { return m_timestampFirst; }
	NTPTime getTimestampLast() const { return m_timestampLast; }
	float getCertainty() const { return m_certainty; }

public: // setter
	void setRefObjId(const uint32_t newReferenceObjectId) { m_refObjId = newReferenceObjectId; }
	void setDutObjId(const uint32_t newDutObjectId) { m_dutObjId = newDutObjectId; }
	void setTimestampFirst(const NTPTime newTimestampFirst) { m_timestampFirst = newTimestampFirst; }
	void setTimestampLast(const NTPTime newTimestampLast) { m_timestampLast = newTimestampLast; }
	void setCertainty(const float newCertainty) { m_certainty = newCertainty; }

public:
	static std::streamsize getSerializedSize_static() { return 28; }

public:
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

protected:
	uint32_t m_refObjId;
	uint32_t m_dutObjId;
	NTPTime m_timestampFirst;
	NTPTime m_timestampLast;
	float m_certainty;
}; // ObjectAssociation4001Entry

//======================================================================

inline
bool operator==(const ObjectAssociation4001& lhs, const ObjectAssociation4001& rhs)
{
	return lhs.getReferenceObjId() == rhs.getReferenceObjId()
	    && lhs.getDutObjId() == rhs.getDutObjId()
	    && lhs.getTimestampFirst() == rhs.getTimestampFirst()
	    && lhs.getTimestampLast() == rhs.getTimestampLast()
	    && fuzzyFloatEqualT<6>(lhs.getCertainty(), rhs.getCertainty());
}

//======================================================================

inline
bool operator!=(const ObjectAssociation4001& lhs, const ObjectAssociation4001& rhs)
{
	return lhs.getReferenceObjId() != rhs.getReferenceObjId()
	    || lhs.getDutObjId() != rhs.getDutObjId()
	    || lhs.getTimestampFirst() != rhs.getTimestampFirst()
	    || lhs.getTimestampLast() != rhs.getTimestampLast()
	    || fuzzyFloatInequalT<6>(lhs.getCertainty(), rhs.getCertainty());
}

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // OBJECTASSOCIATION4001_HPP_SEEN

//======================================================================
