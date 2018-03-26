//======================================================================
/*! \file ObjectAssociation4001.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 26, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/ObjectAssociation4001.hpp>

#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

ObjectAssociation4001::ObjectAssociation4001()
  : m_refObjId(0),
	m_dutObjId(0),
	m_timestampFirst(),
	m_timestampLast(),
	m_certainty(0.0F)
{}

//======================================================================

bool ObjectAssociation4001::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_refObjId);
	ibeosdk::readBE(is, m_dutObjId);
	ibeosdk::readBE(is, m_timestampFirst);
	ibeosdk::readBE(is, m_timestampLast);
	ibeosdk::readBE(is, m_certainty);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ObjectAssociation4001::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_refObjId);
	ibeosdk::writeBE(os, m_dutObjId);
	ibeosdk::writeBE(os, m_timestampFirst);
	ibeosdk::writeBE(os, m_timestampLast);
	ibeosdk::writeBE(os, m_certainty);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
