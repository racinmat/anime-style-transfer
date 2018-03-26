//======================================================================
/*! \file Box2dF.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date May 30, 2012
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/Box2dF.hpp>
#include <ibeosdk/io.hpp>

#include <algorithm>

//======================================================================

namespace ibeosdk {

//======================================================================

Box2dF::Box2dF()
  : m_center(),
    m_size(),
    m_rotation(.0)
{}

//======================================================================

Box2dF::Box2dF(const Point2dFloat& center,
               const Point2dFloat& size,
               const float rotation)
  : m_center(center),
    m_size(size),
    m_rotation(rotation)
{}

//======================================================================

bool Box2dF::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	m_center.deserialize(is);
	m_size.deserialize(is);
	ibeosdk::readBE(is, this->m_rotation);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool Box2dF::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	m_center.serialize(os);
	m_size.serialize(os);
	ibeosdk::writeBE(os, this->m_rotation);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool Box2dF::containsPoint(const Point2dFloat& point) const
{
	if (this->m_rotation < 1E-6f) {
		// Rotation is zero, hence we can directly check this in
		// cartesian coordinates.
		const float pointX = point.getX() - this->m_center.getX();
		const float pointY = point.getY() - this->m_center.getY();

		// compare position to half size
		return (std::abs(pointX) <=  0.5f * this->m_size.getX())
		       && (std::abs(pointY) <=  0.5f * this->m_size.getY());
	}
	else {
		// 2D coordinate transformation as proposed by Uni Ulm.

		// Move coordinate system to center of the box
		const float deltaX = point.getX() - this->m_center.getX();
		const float deltaY = point.getY() - this->m_center.getY();

		// If any of the X or Y components are outside of the
		// "manhatten" diagonal bounding box, the point cannot be
		// inside the box (and we don't even have to calculate the
		// rotated point).
		const float half_sizeX = 0.5f * this->m_size.getX();
		const float half_sizeY = 0.5f * this->m_size.getY();
		if (std::max(std::abs(deltaX), std::abs(deltaY)) > half_sizeX + half_sizeY)
			return false;

		// Rotate by -psi
		const float dCos = std::cos(this->m_rotation);
		const float dSin = std::sin(this->m_rotation);
		const float pointX =  dCos * deltaX  +  dSin * deltaY;
		const float pointY = -dSin * deltaX  +  dCos * deltaY;

		// compare position to half size
		return ((std::abs(pointX) <=  half_sizeX)
		       && (std::abs(pointY) <=  half_sizeY));
	}
}

//======================================================================

std::pair<float, float> Box2dF::getBoundingAngles() const
{
	// This function calculates a low and a high boundary angle for
	// all edges of the given (rotated) Box2D. The returned FloatPair
	// has the component "first" for the lower bounding angle, and
	// "second" for the upper bounding angle. (Note: This ordering is
	// swapped compared to the scan point ordering!)

	// Need to check whether the origin is inside the box
	if (this->containsPoint(Point2dFloat(0, 0)))
	{
		// Origin is inside. Then return the full interval.
		return std::make_pair<float, float>(float(-M_PI), float(M_PI));
	}

	// The usual case: The box does not contain the origin. Then we
	// look for the min and max angles of the edges.

	std::vector<Point2dFloat> corners = this->getCorners();
	float minangle = corners[0].polarCoordAngle();
	float maxangle = minangle;
	for (unsigned k = 1; k < 4; ++k) {
		float pointangle = corners[k].polarCoordAngle();

		if (pointangle < minangle)
			minangle = pointangle;

		if (pointangle > maxangle)
			maxangle = pointangle;
	}

	return std::pair<float, float>(minangle, maxangle);
}
//======================================================================

std::vector<Point2dFloat> Box2dF::getCorners() const
{
	// This is the delta between center and edges
	const float deltaX = this->m_size.getX() * 0.5f;
	const float deltaY = this->m_size.getY() * 0.5f;

	// The cos/sin rotation factors
	const float dCos = std::cos(this->m_rotation);
	const float dSin = std::sin(this->m_rotation);

	// The dimensions after the rotation
	const float XCos = deltaX * dCos, XSin = deltaX * dSin;
	const float YCos = deltaY * dCos, YSin = deltaY * dSin;

	// Create the resulting points
	std::vector<Point2dFloat> corners(4);
	// Rotate each edge according to the this->rotation
	corners[0] = this->m_center + Point2dFloat(  XCos - YSin,  XSin + YCos);
	corners[1] = this->m_center + Point2dFloat(  XCos + YSin,  XSin - YCos);
	corners[2] = this->m_center + Point2dFloat( -XCos + YSin, -XSin - YCos);
	corners[3] = this->m_center + Point2dFloat( -XCos - YSin, -XSin + YCos);
	return corners;
}

//======================================================================

bool operator==(const Box2dF& lhs, const Box2dF& rhs)
{
	return lhs.getCenter() == rhs.getCenter()
	    && lhs.getSize() == rhs.getSize()
	    && lhs.getRotation() == rhs.getRotation();
}

//======================================================================

bool operator!=(const Box2dF& lhs, const Box2dF& rhs)
{
	return lhs.getCenter() != rhs.getCenter()
	    || lhs.getSize() != rhs.getSize()
	    || lhs.getRotation() != rhs.getRotation();
}

//======================================================================

} // namespace ibeosdk

//======================================================================
