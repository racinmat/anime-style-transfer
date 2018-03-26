//======================================================================
/*! \file LaneSupportPoint.cpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief Support point for a LaneSegment
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/LaneSupportPoint.hpp>

//======================================================================

namespace ibeosdk {
namespace lanes {

//======================================================================

std::string const LaneSupportPoint::VERSION = "1.0.0";

//======================================================================

LaneSupportPoint::LaneSupportPoint(const PositionWgs84& point,
                                   const Point2dFloat& lineOffsetLeft,
                                   const Point2dFloat& lineOffsetRight)
  : m_point(point),
    m_lineOffsetLeft(lineOffsetLeft),
    m_lineOffsetRight(lineOffsetRight)
{}

//======================================================================

LaneSupportPoint::LaneSupportPoint()
{
	m_point = PositionWgs84();
	m_lineOffsetLeft = Point2dFloat();
	m_lineOffsetRight = Point2dFloat();
}

//======================================================================

PositionWgs84 LaneSupportPoint::getPoint() const
{
	return m_point;
}

//======================================================================

Point2dFloat LaneSupportPoint::getLeftOffset() const
{
	return m_lineOffsetLeft;
}

//======================================================================

Point2dFloat LaneSupportPoint::getRightOffset() const
{
	return m_lineOffsetRight;
}

//======================================================================

std::streamsize LaneSupportPoint::getSerializedSize() const
{
	return 8 +                                          // double getLongitudeInDeg
	       8 +                                          // double getLatitudeInDeg
	       8 +                                          // double getCourseAngleInDeg
	       m_lineOffsetRight.getSerializedSize() +      // Point2DFloat
	       m_lineOffsetLeft.getSerializedSize();        // Point2DFloat
}

//======================================================================

bool LaneSupportPoint::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_point.getLongitudeInDeg());
	ibeosdk::writeBE(os, m_point.getLatitudeInDeg());
	ibeosdk::writeBE(os, m_point.getCourseAngleInDeg());
	m_lineOffsetLeft.serialize(os);
	m_lineOffsetRight.serialize(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool LaneSupportPoint::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	double lon, lat, course;

	ibeosdk::readBE(is, lon);
	ibeosdk::readBE(is, lat);
	ibeosdk::readBE(is, course);

	m_lineOffsetLeft.deserialize(is);
	m_lineOffsetRight.deserialize(is);

	m_point.setLongitudeInDeg(lon);
	m_point.setLatitudeInDeg(lat);
	m_point.setCourseAngleInDeg(course);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace lanes
} // namespace ibeo

//======================================================================

