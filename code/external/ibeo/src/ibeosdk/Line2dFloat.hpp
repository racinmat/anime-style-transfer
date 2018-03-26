//======================================================================
/*! \file Line2dFloat.hpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Mar 11, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LINE2DFLOAT_HPP
#define IBEOSDK_LINE2DFLOAT_HPP

//======================================================================

#include <ibeosdk/datablocks/snippets/Point2dFloat.hpp>
#include <ibeosdk/datablocks/snippets/Polygon2dFloat.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*! \brief  A line in the two-dimensional plane, composed out of two points.
 * This class is a line composed out of two points and offers some
 * helper methods for simple geometrical calculations.
 *
 * \sa Point2dFloat, Polygon2dFloat
 *///-------------------------------------------------------------------
class Line2dFloat {
public:
	/*! \brief Describes how two lines can be intersecting. \sa isIntersecting() */
	enum IntersectingType {
		NotIntersecting, ///< The lines are not intersecting, i.e. they are parallel or zero
		LineIntersecting, ///< The lines are intersecting within their line segments
		OutsideIntersecting ///< The lines are intersecting, but outside of their line segments
	};

public:
	//========================================
	/*!\brief Empty constructor
	 *///-------------------------------------
	Line2dFloat()
	  : first(),
	    second()
	{}

	//========================================
	/*!\brief Constructor with two points
	 *///-------------------------------------
	Line2dFloat(const Point2dFloat&a, const Point2dFloat& b)
	  : first(a),
	    second(b)
	{}

	//========================================
	/*!\brief Constructor with x/y coordinates of the two points given explicitly
	 *///-------------------------------------
	Line2dFloat(float x1, float y1, float x2, float y2)
	  : first(x1, y1),
	    second(x2, y2)
	{}

	//========================================
	/*!\brief  Constructor from a std::pair
	 *///-------------------------------------
	Line2dFloat(const std::pair<Point2dFloat, Point2dFloat>& p)
	  : first(p.first),
	    second(p.second)
	{}

public:
	//========================================
	/*!\name Accessor methods for information about this line
	 *///-------------------------------------
	 //\{

	//========================================
	/*!\brief Returns the first point
	 *///-------------------------------------
	const Point2dFloat& getP1() const { return first; }

	//========================================
	/*!\brief Returns the second point
	 *///-------------------------------------
	const Point2dFloat& getP2() const { return second; }

	//========================================
	/*!\brief Returns the length of this line, i.e. the distance of the two points.
	 *///-------------------------------------
	float length() const { return getDiff().dist(); }

	//========================================
	/*!\brief Returns the difference between line end and line start as a Point2dFloat.
	 *
	 * The returned value is getP2() - getP1().
	 *///-------------------------------------
	Point2dFloat getDiff() const { return second - first; }

	//========================================
	/*!\brief Checks if the line is vertical (both x components are 0)
	 *///-------------------------------------
	bool isVertical() const { return std::abs(this->second.getX() - this->first.getX() ) < 1E-11; }

	//========================================
	/*!\brief Returns true if both points are zero
	 *///-------------------------------------
	bool isZero() const { return first.isZero() && second.isZero(); }

	//========================================
	/*!\brief Returns the normal direction (orthogonal vector) to this line.
	 *
	 * The returned direction is the 90 degree rotated unit vector of the line.
	 *///-------------------------------------
	Point2dFloat getNormalDirection() const;

	//\}

public:
	//========================================
	/*!\name Setter methods for changing this line
	 *///-------------------------------------
	//\{

	//========================================
	/*!\brief Sets the first point.
	 *///-------------------------------------
	void setP1(const Point2dFloat& p1) { first = p1; }

	//========================================
	/*!\brief Sets the second point.
	 *///-------------------------------------
	void setP2(const Point2dFloat& p2) { second = p2; }

	//\}

public:
	//========================================
	/*!\name Geometrical relations to other objects.
	 *///-------------------------------------
	//\{

	//========================================
	/*!\brief Calculates the intersection point between two lines.
	 * The returned value describes whether the two lines intersect
	 * within their line segments, or outside of their line segments,
	 * or not at all. The actual intersection point is written to
	 * intersectingPoint if that pointer is non-NULL.
	 *///-------------------------------------
	IntersectingType isIntersecting(const Line2dFloat& other,
	                                Point2dFloat* intersectingPoint = NULL) const;


	//========================================
	/*!\brief Calculates and returns the point that results when projecting
	 *        the given point onto this line orthogonally.
	 *///-------------------------------------
	Point2dFloat projectOntoLine(const Point2dFloat& point) const;

	//========================================
	/*!\brief Returns true if this line "contains" the given point.
	 *
	 * Computes the distance from the point to the line, and if it is
	 * zero, the point lies on the line and this method returns true.
	 *///-------------------------------------
	bool containsPoint(const Point2dFloat& point) const;

	//========================================
	/*!\brief Returns the distance of a point to this line segment.
	 *
	 * If the point's projection onto the line is between the end
	 * points of this line, the distance to the projected point is
	 * returnd. If the projection is not on the line segment, the
	 * distance to the closest end point is returned. Alternatively,
	 * distanceToPoint() returns the distance to the point's
	 * projection in all cases.
	 *///-------------------------------------
	float distanceFromLineSegment(const Point2dFloat& point) const;

	//\}

private:
	bool line2d_intersect(const Line2dFloat& firstLine, const Line2dFloat& secondLine) const;

private:
	//========================================
	/*!\brief The first point of this line.
	 *///-------------------------------------
	Point2dFloat first;

	//========================================
	/*!\brief The second point of this line.
	 *///-------------------------------------
	Point2dFloat second;
}; // Line2dFloat

//======================================================================

// Text output for debugging
std::ostream& operator<<(std::ostream& os, const Line2dFloat& l);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_LINE2DFLOAT_HPP

//======================================================================
