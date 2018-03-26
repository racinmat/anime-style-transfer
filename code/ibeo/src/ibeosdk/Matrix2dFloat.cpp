//======================================================================
/*! \file Matrix2dFloat.cpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief A 2x2 Matrix of type float
 *///-------------------------------------------------------------------

#include <ibeosdk/Matrix2dFloat.hpp>

//==============================================================================

namespace ibeosdk {

//==============================================================================

// default constructor, creates a 2x2 identity matrix
Matrix2dFloat::Matrix2dFloat()
{
	c[0] = Point2dFloat(1,0);
	c[1] = Point2dFloat(0,1);
}

//==============================================================================

// copy-constructor
Matrix2dFloat::Matrix2dFloat(const Matrix2dFloat& other)
{
	*this = other;
}

// constructor with elementwise initialization
Matrix2dFloat::Matrix2dFloat(const value_type m11, const value_type m12,
                             const value_type m21, const value_type m22)
{
	c[0].setXY(m11, m21);
	c[1].setXY(m12, m22);
}

//==============================================================================

// constructor with columnwise initialization
Matrix2dFloat::Matrix2dFloat(const Point2dFloat& c0, const Point2dFloat& c1)
{
	c[0]=c0;
	c[1]=c1;
}

//==============================================================================

// returns the transposed Matrix
Matrix2dFloat Matrix2dFloat::transposed() const
{
	return Matrix2dFloat(Point2dFloat(c[0].getX(), c[1].getX()),
	                     Point2dFloat(c[0].getY(), c[1].getY()));
}

//==============================================================================

// adds a Matrix2dFloat to this one
Matrix2dFloat& Matrix2dFloat::operator+=(const Matrix2dFloat& other)
{
	c[0] += other.c[0];
	c[1] += other.c[1];
	return *this;
}

//==============================================================================

// adds a Matrix2dFloat to this one
Matrix2dFloat Matrix2dFloat::operator+(const Matrix2dFloat& other) const
{
	return Matrix2dFloat(*this) += other;
}

//==============================================================================

// subtracts a Matrix2dFloat from this one
Matrix2dFloat& Matrix2dFloat::operator-=(const Matrix2dFloat& other)
{
	c[0] -= other.c[0];
	c[1] -= other.c[1];
	return *this;
}

//==============================================================================

// subtracts a Matrix2dFloat from this one
Matrix2dFloat Matrix2dFloat::operator-(const Matrix2dFloat& other) const
{
	return Matrix2dFloat(*this) -= other;
}

//==============================================================================

// multiplies this Matrix2dFloat by a factor
Matrix2dFloat& Matrix2dFloat::operator*=(const value_type factor)
{
	c[0] *= factor;
	c[1] *= factor;
	return *this;
}

//==============================================================================

// multiplication by a factor
Matrix2dFloat Matrix2dFloat::operator*(const value_type factor) const
{
	return Matrix2dFloat(*this) *= factor;
}

//==============================================================================

// left side multiplication
Point2dFloat Matrix2dFloat::mulLeft(const Point2dFloat& point) const
{
	return Point2dFloat(
		point.getX()*c[0].getX() + point.getY()*c[0].getY(),
		point.getX()*c[1].getX() + point.getY()*c[1].getY()
	);
}

//==============================================================================

// multiplies this Matrix2dFloat by a vector
Point2dFloat Matrix2dFloat::operator*(const Point2dFloat& point) const
{
	return (c[0]*point.getX() + c[1]*point.getY());
}

//==============================================================================

// multiplication by another Matrix2dFloat
Matrix2dFloat Matrix2dFloat::operator*(const Matrix2dFloat& other) const
{
	Point2dFloat p1 = *this*other.c[0];
	Point2dFloat p2 = *this*other.c[1];
	return Matrix2dFloat(p1,p2);
}

//==============================================================================

// multiplication by another Matrix2dFloat
Matrix2dFloat& Matrix2dFloat::operator*=(const Matrix2dFloat& other)
{
	return *this = *this * other;
}

//==============================================================================

// divides a Matrix2dFloat through a factor
Matrix2dFloat& Matrix2dFloat::operator/=(const value_type divisor)
{
	c[0] /= divisor;
	c[1] /= divisor;
	return *this;
}

//==============================================================================

// division through a factor
Matrix2dFloat Matrix2dFloat::operator/(const value_type divisor) const
{
	return Matrix2dFloat(*this) /= divisor;
}

//==============================================================================

// negates the Matrix2dFloat
Matrix2dFloat Matrix2dFloat::operator-() const
{
	return Matrix2dFloat(Point2dFloat(), Point2dFloat()) -= *this;
}

//==============================================================================

// comparison of another Matrix2dFloat to this one
bool Matrix2dFloat::operator==(const Matrix2dFloat& other) const
{
	return ((c[0] == other.c[0]) && (c[1] == other.c[1]));
}

//==============================================================================

// comparison of another Matrix2dFloat to this one
bool Matrix2dFloat::operator!=(const Matrix2dFloat& other) const
{
	return !(*this == other);
}

//==============================================================================

// Calculation of the determinant of this matrix.
Matrix2dFloat::value_type Matrix2dFloat::det() const
{
	return c[0].getX() * c[1].getY() - c[1].getX() * c[0].getY();
}

//==============================================================================

// Calculate the inverse of this matrix.
Matrix2dFloat Matrix2dFloat::inverted() const
{
	return Matrix2dFloat(c[1].getY(), -c[1].getX(),
	                    -c[0].getY(),  c[0].getX()) / det();
}

//==============================================================================

// get the first column
Point2dFloat Matrix2dFloat::getColumn1() const
{
	return c[0];
}

//==============================================================================

// get the second column
Point2dFloat Matrix2dFloat::getColumn2() const
{
	return c[1];
}

//==============================================================================

} // namespace ibeosdk

//==============================================================================

