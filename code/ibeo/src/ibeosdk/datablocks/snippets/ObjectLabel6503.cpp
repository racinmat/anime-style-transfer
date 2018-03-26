//======================================================================
/*! \file ObjectLabel6503.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 28, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/ObjectLabel6503.hpp>
#include <ibeosdk/ObjectBasic.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

ObjectLabel6503::ObjectLabel6503()
  : m_objBox(),
    m_weight(0),
    m_classification(ObjectClass_Unclassified),
    m_id(0),
    m_trackingId(0),
    m_displayMagnificationFactor(1.0F),
    m_objectFlags(0),
    m_isInOnKeyframe(0),
    // m_reserved
    m_userData()
{
	memset(&m_reserved[0], 0, sizeof(m_reserved));
}

//======================================================================

std::streamsize ObjectLabel6503::getSerializedSize() const
{
	return Box2d::getSerializedSize_static()
	     + std::streamsize(2*sizeof(uint8_t))
	     + std::streamsize(sizeof(uint32_t))
	     + std::streamsize(sizeof(uint64_t))
	     + std::streamsize(sizeof(float))
	     + std::streamsize((2+nbOfReserved)*sizeof(uint16_t))
	     + m_userData.getSerializedSize();
}

//======================================================================

bool ObjectLabel6503::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	m_objBox.deserialize(is);
	ibeosdk::readBE(is, this->m_weight);
	ibeosdk::readBE(is, this->m_classification);
	ibeosdk::readBE(is, this->m_id);
	ibeosdk::readBE(is, this->m_trackingId);
	ibeosdk::readBE(is, this->m_displayMagnificationFactor);
	ibeosdk::readBE(is, this->m_objectFlags);
	ibeosdk::readBE(is, this->m_isInOnKeyframe);
	for (uint16_t r = 0; r < nbOfReserved; ++r) {
		ibeosdk::readBE(is, this->m_reserved[r]);
	}

	m_userData.deserialize(is);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ObjectLabel6503::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	m_objBox.serialize(os);

	ibeosdk::writeBE(os, this->m_weight);
	ibeosdk::writeBE(os, this->m_classification);
	ibeosdk::writeBE(os, this->m_id);
	ibeosdk::writeBE(os, this->m_trackingId);
	ibeosdk::writeBE(os, this->m_displayMagnificationFactor);
	ibeosdk::writeBE(os, this->m_objectFlags);
	ibeosdk::writeBE(os, this->m_isInOnKeyframe);
	for (uint16_t r = 0; r < nbOfReserved; ++r) {
		ibeosdk::writeBE(os, this->m_reserved[r]);
	}

	m_userData.serialize(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool operator==(const ObjectLabel6503& lhs, const ObjectLabel6503& rhs)
{
	if ( lhs.getObjBox() != rhs.getObjBox()
	  || lhs.getWeight() != rhs.getWeight()
	  || lhs.getClassification() != rhs.getClassification()
	  || lhs.getId() != rhs.getId()
	  || lhs.getTrackingId() != rhs.getTrackingId()
	  || lhs.getDisplayMagnificationFactor() != rhs.getDisplayMagnificationFactor()
	  || lhs.getObjectFlags() != rhs.getObjectFlags()
	  || lhs.getIsInOnKeyframe() != rhs.getIsInOnKeyframe())
		return false;

	if (lhs.getUserData() != rhs.getUserData())
		return false;

	for (uint16_t r = 0; r < ObjectLabel6503::nbOfReserved; ++r) {
		if (lhs.getReserved(r) != rhs.getReserved(r))
			return false;
	}
	return true;
}

//======================================================================

bool operator!=(const ObjectLabel6503& lhs, const ObjectLabel6503& rhs)
{
	if ( lhs.getObjBox() != rhs.getObjBox()
	  || lhs.getWeight() != rhs.getWeight()
	  || lhs.getClassification() != rhs.getClassification()
	  || lhs.getId() != rhs.getId()
	  || lhs.getTrackingId() != rhs.getTrackingId()
	  || lhs.getDisplayMagnificationFactor() != rhs.getDisplayMagnificationFactor()
	  || lhs.getObjectFlags() != rhs.getObjectFlags()
	  || lhs.getIsInOnKeyframe() != rhs.getIsInOnKeyframe()
	  || lhs.getUserData() != rhs.getUserData())
		return true;

	for (uint16_t r = 0; r < ObjectLabel6503::nbOfReserved; ++r) {
		if (lhs.getReserved(r) != rhs.getReserved(r))
			return true;
	}
	return false;
}

//======================================================================

}// namespace ibeosdk

//======================================================================
