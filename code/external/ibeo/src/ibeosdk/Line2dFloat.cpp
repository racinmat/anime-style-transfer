//======================================================================
/*! \file Line2dFloat.hpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Mar 11, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/Line2dFloat.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

bool Line2dFloat::line2d_intersect(const Line2dFloat& firstLine, const Line2dFloat& secondLine) const
{
	float a1, a2, b1, b2, c1, c2; // Coefficients of line eqns.
	float r1, r2, r3, r4;         // 'Sign' values

	float x1 = firstLine.first.getX();
	float y1 = firstLine.first.getY();
	float x2 = firstLine.second.getX();
	float y2 = firstLine.second.getY();
	float x3 = secondLine.first.getX();
	float y3 = secondLine.first.getY();
	float x4 = secondLine.second.getX();
	float y4 = secondLine.second.getY();

	a1 = y2 - y1;
	b1 = x1 - x2;
	c1 = x2 * y1 - x1 * y2;

	r3 = a1 * x3 + b1 * y3 + c1; // step 1
	r4 = a1 * x4 + b1 * y4 + c1; // step 2

	// step 3
	if ( r3 != 0 && r4 != 0 &&  r3*r4 >= 0 )
		return false;

	a2 = y4 - y3;
	b2 = x3 - x4;
	c2 = x4 * y3 - x3 * y4;

	r1 = a2 * x1 + b2 * y1 + c2; // step 4
	r2 = a2 * x2 + b2 * y2 + c2; // step 5

	// step 6
	if ( r1 != 0 && r2 != 0 && r1*r2 >= 0)
		return false;

	return true;
}

//======================================================================

Line2dFloat::IntersectingType Line2dFloat::isIntersecting(const Line2dFloat& other, Point2dFloat* point) const
{
	if (this->isZero() || other.isZero())
		return NotIntersecting;

	const bool has_inside_intersection =
		line2d_intersect(*this, other);

	IntersectingType result =
		has_inside_intersection ? LineIntersecting : OutsideIntersecting;
	Point2dFloat intersectPoint;

	Point2dFloat V = this->getDiff().normalized();
	Point2dFloat N = other.getNormalDirection();

	// check parallel lines
	// TODO no check if lanes are overlapping, but NotIntersecting is
	// probably correct
	if ( std::abs(V * N) < 1E-11 )
	{
		result = NotIntersecting;
	}
	else
	{
		float d = N * other.first;
		float u = ( d - first.getX() * N.getX() - first.getY() * N.getY() ) / (V * N);

		intersectPoint = first + V *u ;
	}

	if (point)
		*point = intersectPoint;
	return result;
}

//======================================================================

Point2dFloat Line2dFloat::projectOntoLine(const Point2dFloat& point) const
{
	Point2dFloat line_vec = getDiff();
	float line_length = length();
	if (std::abs(line_length) >= 1E-11)
	{
		Point2dFloat line_unitvec = line_vec / line_length;
		Point2dFloat vec_to_point = point - first;
		float length_of_projection = line_unitvec * vec_to_point;
		return first + line_unitvec * length_of_projection;
	}
	else
	{
		// Zero-length line; what should we do?
		return point;
	}
}

//======================================================================

bool Line2dFloat::containsPoint(const Point2dFloat& point) const
{
	float d = distanceFromLineSegment(point);
	return d < 1E-11;
}

//======================================================================

Point2dFloat Line2dFloat::getNormalDirection() const
{
	Point2dFloat diff = getDiff().normalized();
	return Point2dFloat(-diff.getY(), diff.getX());
}

//======================================================================

float Line2dFloat::distanceFromLineSegment(const Point2dFloat& point) const
{
	const float cx = point.getX();
	const float cy = point.getY();

	const float ax = getP1().getX();
	const float ay = getP1().getY();
	const float bx = getP2().getX();
	const float by = getP2().getY();

	const float r_numerator = (cx - ax) * (bx - ax) + (cy - ay) * (by - ay);
	const float r_denomenator = (bx - ax) * (bx - ax) + (by - ay) * (by - ay);
	const float r = r_numerator / r_denomenator;

	const float s =  ((ay - cy) * (bx - ax) - (ax - cx) * (by - ay) ) / r_denomenator;

	const float distanceLine = std::abs(s) * float(sqrt(r_denomenator));
	float distanceSegment = -1.0;

	if ( (r >= 0) && (r <= 1) )
	{
		distanceSegment = distanceLine;
	}
	else
	{
		float dist1 = (cx - ax) * (cx - ax) + (cy - ay) * (cy - ay);
		float dist2 = (cx - bx) * (cx - bx) + (cy - by) * (cy - by);
		if (dist1 < dist2)
			distanceSegment = float(sqrt(dist1));
		else
			distanceSegment = float(sqrt(dist2));
	}

	return distanceSegment;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
