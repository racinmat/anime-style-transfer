//======================================================================
/*! \file Point2dFloat.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 27, 2012
 * \date May 30, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_POINT2DFLOAT_HPP_SEEN
#define IBEOSDK_POINT2DFLOAT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/Point2d.hpp>

#include <cmath>

//======================================================================

namespace ibeosdk {

//======================================================================

class Point2dFloat : public Snippet {
public:
	static std::streamsize getSerializedSize_static() { return 2*std::streamsize(sizeof(float)); }
public:
	Point2dFloat();
	Point2dFloat(const float x, const float y);
	virtual ~Point2dFloat();

public:
	Point2dFloat(const Point2dFloat& src);
	Point2dFloat& operator=(const Point2dFloat& src);

	Point2dFloat& operator+=(const Point2dFloat& other);
	Point2dFloat& operator*=(const float factor);
	Point2dFloat& operator-=(const Point2dFloat& point);
	Point2dFloat& operator/=(const float divisor);

public:
	Point2dFloat operator+(const Point2dFloat& other) const;
	Point2dFloat operator-(const Point2dFloat& other) const;
	Point2dFloat operator/(const float other) const;

	float operator*(const Point2dFloat& other) const;

	bool operator==(const Point2dFloat& other) const;
	bool operator!=(const Point2dFloat& other) const;

public:
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	float getX() const { return this->m_x; }
	float getY() const { return this->m_y; }
	float& getX() { return this->m_x; }
	float& getY() { return this->m_y; }

	void setX(const float val) { this->m_x = val; }
	void setY(const float val) { this->m_y = val; }
	void setXY(const float newX, const float newY) { this->m_x = newX; this->m_y = newY; }

	float dist() const { return std::sqrt(this->m_x * this->m_x + this->m_y * this->m_y); }

	/*! \brief Returns true if this point is zero in terms of the machine
	 * precision, that is, its value is exactly zero or "almost zero". */
	bool isZero() const;

	float polarCoordAngle() const { return std::atan2(this->m_y, this->m_x); }

	/*! \brief Calculates the normalized form of this vector (point is
	 * considered as a vector here) and returns it.  If the vector has
	 * zero length (isZero() returns true), it will be left unchanged.
	 */
	Point2dFloat normalized() const;

	/**
	 * Rotates this point around the orign (0,0)
	 * Same as rotated, but modifies the content.
	 */
	void rotate(float angle);

	/** Rotate the given point by the given angle around (0,0) and returns the
	 * rotated point.
	 *
	 * This method implements an "active rotation matrix".
	 *
	 * In other words: A positive rotation of the point (1,0) by the
	 * angle +pi/2 results in the point (0,1). Watch out: If the
	 * rotation is defined differently, another point (0,-1) might
	 * have been expected, but this function is implemented to return
	 * (0,1) here. */
	Point2dFloat rotated(const float angle_rad) const;

protected:
	float m_x;
	float m_y;
}; // Point2Dfloat

//======================================================================

inline Point2dFloat operator*(const float factor, const Point2dFloat pt)
{
	return Point2dFloat(factor * pt.getX(), factor * pt.getY());
}

//======================================================================

inline Point2dFloat operator*(const Point2dFloat pt, const float factor)
{
	return Point2dFloat(pt.getX() * factor, pt.getY() * factor);
}

//======================================================================

inline Point2dFloat operator*(const float factor, const Point2d pt)
{
	return Point2dFloat(factor * pt.getX(), factor * pt.getY());
}

//======================================================================

inline Point2dFloat operator*(const Point2d pt, const float factor)
{
	return Point2dFloat(pt.getX() * factor, pt.getY() * factor);
}

//======================================================================

inline
bool operator<(const Point2dFloat& l, const Point2dFloat& r)
{
	return ((l.getX() < r.getX()) && (l.getY() < r.getY()));
}

//======================================================================

inline
bool operator<=(const Point2dFloat& l, const Point2dFloat& r)
{
	return ((l.getX() <= r.getX()) && (l.getY() <= r.getY()));
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const Point2dFloat& p);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_POINT2DFLOAT_HPP_SEEN

//======================================================================
