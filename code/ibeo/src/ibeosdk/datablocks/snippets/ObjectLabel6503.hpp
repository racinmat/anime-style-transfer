//======================================================================
/*! \file ObjectLabel6503.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 28, 2016
 *///-------------------------------------------------------------------

#ifndef OBJECTLABEL6503_HPP_SEEN
#define OBJECTLABEL6503_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/datablocks/snippets/MeasurementList.hpp>
#include <ibeosdk/datablocks/snippets/Box2d.hpp>

//======================================================================

namespace ibeosdk {

typedef uint64_t Uoid;

//======================================================================

class ObjectLabel6503 : public Snippet {
public:
	static const uint32_t nbOfReserved = 3;

public:
	ObjectLabel6503();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	const Box2d& getObjBox() const { return m_objBox; }
	Box2d& getObjBox() { return m_objBox; }
	uint8_t getWeight() const { return m_weight; }
	uint8_t getClassification() const { return m_classification; }
	uint32_t getId() const { return m_id; }
	Uoid getTrackingId() const { return m_trackingId; }
	float getDisplayMagnificationFactor() const { return m_displayMagnificationFactor; }
	uint16_t getObjectFlags() const { return m_objectFlags; }
	uint16_t getIsInOnKeyframe() const { return m_isInOnKeyframe; }
	uint16_t getReserved(const uint32_t idx) const { assert(idx < nbOfReserved); return m_reserved[idx]; }
	const MeasurementList& getUserData() const { return m_userData; }
	MeasurementList& getUserData() { return m_userData; }

public: // setter
	// use getUserData
	void setWeight(uint8_t newWeight) { m_weight = newWeight; }
	void setClassification(const uint8_t newClassification) { m_classification = newClassification; }
	void setId(const uint32_t newId) { m_id = newId; }
	void setTrackingId(const Uoid newTrackingId) { m_trackingId = newTrackingId; }
	void setDisplayMagnificationFactor(const float newDmf) { m_displayMagnificationFactor = newDmf; }
	void setObjectFlags(const uint16_t newObjectFlags) { m_objectFlags = newObjectFlags; }
	void setIsInOnKeyframe(const uint16_t newIsInOnKeyframe) { m_isInOnKeyframe = newIsInOnKeyframe; }
	void setReserved(const uint32_t idx, const uint16_t newReserved) { assert(idx < nbOfReserved); m_reserved[idx] = newReserved; }
	// use getUserData

protected:
	Box2d m_objBox;
	uint8_t m_weight;
	uint8_t m_classification;
	uint32_t m_id;
	Uoid m_trackingId;
	float m_displayMagnificationFactor;
	uint16_t m_objectFlags;
	uint16_t m_isInOnKeyframe;
	uint16_t m_reserved[nbOfReserved];
	MeasurementList m_userData;
}; // ObjectLabel6503

//======================================================================

bool operator==(const ObjectLabel6503& lhs, const ObjectLabel6503& rhs);
bool operator!=(const ObjectLabel6503& lhs, const ObjectLabel6503& rhs);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // OBJECTLABEL6503_HPP_SEEN

//======================================================================
