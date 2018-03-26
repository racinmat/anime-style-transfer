//======================================================================
/*! \file Box2d.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 13, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/Box2d.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

Box2d::Box2d()
  : m_center(),
    m_size(),
    m_compressedRotation(0)
{}

//======================================================================

Box2d::Box2d(const CompressedPoint2d& center,
             const CompressedPoint2d& size,
             const int16_t compressedRotation)
  : m_center(center),
    m_size(size),
    m_compressedRotation(compressedRotation)
{}

//======================================================================

bool Box2d::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_center);
	ibeosdk::readBE(is, m_size);
	ibeosdk::readBE(is, this->m_compressedRotation);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool Box2d::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_center);
	ibeosdk::writeBE(os, m_size);
	ibeosdk::writeBE(os, this->m_compressedRotation);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool operator==(const Box2d& lhs, const Box2d& rhs)
{
	return lhs.getCenter() == rhs.getCenter()
	    && lhs.getSize() == rhs.getSize()
	    && lhs.getRotation() == rhs.getRotation();
}

//======================================================================

bool operator!=(const Box2d& lhs, const Box2d& rhs)
{
	return lhs.getCenter() != rhs.getCenter()
	    || lhs.getSize() != rhs.getSize()
	    || lhs.getRotation() != rhs.getRotation();
}

//======================================================================

} // namespace ibeosdk

//======================================================================
