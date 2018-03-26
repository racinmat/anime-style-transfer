//======================================================================
/*! \file ObjectScala2271.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 24, 2014
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/ObjectScala2271.hpp>
#include <ibeosdk/Log.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

ObjectScala2271::ObjectScala2271()
  : m_objectId(0),
    m_internal(0),
    m_interfaceFlags(0),
    m_unfilteredAttrib(),
    m_filteredAttrib(),
    m_reserved(0)
{}

//======================================================================

ObjectScala2271::~ObjectScala2271() {}

//======================================================================

std::streamsize ObjectScala2271::getSerializedSize() const
{
	return std::streamsize(sizeof(uint32_t))
		+ 3* std::streamsize(sizeof(uint8_t))
		+ m_unfilteredAttrib.getSerializedSize()
		+ m_filteredAttrib.getSerializedSize()
		+ std::streamsize(sizeof(uint32_t));
}

//======================================================================

bool ObjectScala2271::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_objectId);
	ibeosdk::readBE(is, m_internal);
	ibeosdk::readBE(is, m_interfaceFlags);
	uint8_t attrFlags;
	ibeosdk::readBE(is, attrFlags);


	// attention has(Un)FilteredAttributes/Contours methods are not
	// to be used here since their data source has not be read yet.
	m_unfilteredAttrib.setIsValid((attrFlags & ObjectScala2271::UnfAvailable) == ObjectScala2271::UnfAvailable);
	m_unfilteredAttrib.setHasContourPoints((attrFlags & ObjectScala2271::UnfContAvail) == ObjectScala2271::UnfContAvail);
	if (!m_unfilteredAttrib.deserialize(is))
		return false;

	m_filteredAttrib.setIsValid((attrFlags & ObjectScala2271::FilAvailable) == ObjectScala2271::FilAvailable);
	m_filteredAttrib.setHasContourPoints((attrFlags & ObjectScala2271::FilContAvail) == ObjectScala2271::FilContAvail);
	m_filteredAttrib.deserialize(is);

	ibeosdk::readBE(is, m_reserved);
	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ObjectScala2271::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_objectId);
	ibeosdk::writeBE(os, m_internal);
	ibeosdk::writeBE(os, m_interfaceFlags);
	ibeosdk::writeBE(os, getAttributeFlags());

	m_unfilteredAttrib.serialize(os);
	m_filteredAttrib.serialize(os);
	ibeosdk::writeBE(os, m_reserved);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

uint8_t ObjectScala2271::getAttributeFlags() const
{
	uint8_t attrFlags = ObjectScala2271::NoAttributes;
	attrFlags = uint8_t(attrFlags | (hasUnfilteredAttributes() ? ObjectScala2271::UnfAvailable : ObjectScala2271::NoAttributes));
	attrFlags = uint8_t(attrFlags | (hasUnfilteredContours()   ? ObjectScala2271::UnfContAvail : ObjectScala2271::NoAttributes));
	attrFlags = uint8_t(attrFlags | (hasFilteredAttributes()   ? ObjectScala2271::FilAvailable : ObjectScala2271::NoAttributes));
	attrFlags = uint8_t(attrFlags | (hasFilteredContours()     ? ObjectScala2271::FilContAvail: ObjectScala2271::NoAttributes));

	return attrFlags;
}

//======================================================================

void ObjectScala2271::setAttributeFlags(const uint8_t attrFlags)
{
	m_unfilteredAttrib.setIsValid((attrFlags & ObjectScala2271::UnfAvailable) == ObjectScala2271::UnfAvailable);
	m_unfilteredAttrib.setHasContourPoints((attrFlags & ObjectScala2271::UnfContAvail) == ObjectScala2271::UnfContAvail);
	m_filteredAttrib.setIsValid((attrFlags & ObjectScala2271::FilAvailable) == ObjectScala2271::FilAvailable);
	m_filteredAttrib.setHasContourPoints((attrFlags & ObjectScala2271::FilContAvail) == ObjectScala2271::FilContAvail);
}

//======================================================================

bool operator==(const ObjectScala2271& lhs, const ObjectScala2271& rhs)
{
	if (lhs.getObjectId() != rhs.getObjectId()) return false;
	if (lhs.getInternal() != rhs.getInternal()) return false;
	if (lhs.getInterfaceFlags() != rhs.getInterfaceFlags()) return false;
	if (lhs.getReserved() != rhs.getReserved()) return false;

	if (lhs.getAttributeFlags() == rhs.getAttributeFlags()) {
		if (lhs.getUnfilteredObjectAttributes() == rhs.getUnfilteredObjectAttributes())
			if (lhs.getFilteredObjectAttributes() == rhs.getFilteredObjectAttributes())
				return true;
	}

	return false;
}

//======================================================================

bool operator!=(const ObjectScala2271& lhs, const ObjectScala2271& rhs)
{
	return !(lhs == rhs);
}

//======================================================================

}// namespace ibeosdk

//======================================================================
