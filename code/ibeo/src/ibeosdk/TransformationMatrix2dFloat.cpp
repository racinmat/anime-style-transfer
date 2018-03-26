//======================================================================
/*! \file TransformationMatrix2dFloat.cpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief A 2x2 kinematic matrix of type float
 *///-------------------------------------------------------------------

//==============================================================================

#include <ibeosdk/TransformationMatrix2dFloat.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

// inverted matrix
TransformationMatrix2dFloat TransformationMatrix2dFloat::inverted() const
{
	RotationMatrix2dFloat invRot = rotM.inverted();
	Point2dFloat invPos = -invRot * posV;
	return TransformationMatrix2dFloat(invRot, invPos);
}

//======================================================================

// returns the rotated matrix
TransformationMatrix2dFloat TransformationMatrix2dFloat::rot(const value_type alpha) const
{
	return *this * TransformationMatrix2dFloat(RotationMatrix2dFloat(alpha));
}

//======================================================================

// returns the shifted matrix
TransformationMatrix2dFloat TransformationMatrix2dFloat::transX(const TransformationMatrix2dFloat::value_type distance) const
{
	return *this * TransformationMatrix2dFloat(Point2dFloat(distance, 0));
}

//======================================================================

// returns the shifted matrix
TransformationMatrix2dFloat TransformationMatrix2dFloat::transY(const TransformationMatrix2dFloat::value_type distance) const
{
	return *this * TransformationMatrix2dFloat(Point2dFloat(0, distance));
}

//======================================================================

// adds a TransformationMatrix2dFloat to this one
TransformationMatrix2dFloat& TransformationMatrix2dFloat::operator+=(const TransformationMatrix2dFloat& other)
{
	rotM += other.rotM;
	posV += other.posV;
	return *this;
}

//======================================================================

// adds a TransformationMatrix2dFloat to another one
TransformationMatrix2dFloat TransformationMatrix2dFloat::operator+(const TransformationMatrix2dFloat& other) const
{
	return TransformationMatrix2dFloat(*this) += other;
}

//======================================================================

// subtracts a TransformationMatrix2dFloat from this one
TransformationMatrix2dFloat& TransformationMatrix2dFloat::operator-=(const TransformationMatrix2dFloat& other)
{
	rotM -= other.rotM;
	posV -= other.posV;
	return *this;
}

//======================================================================

// subtracts a TransformationMatrix2dFloat from another one
TransformationMatrix2dFloat TransformationMatrix2dFloat::operator-(const TransformationMatrix2dFloat& other) const
{
	return TransformationMatrix2dFloat(*this) -= other;
}

//======================================================================

// multiplies a TransformationMatrix2dFloat to this one
TransformationMatrix2dFloat& TransformationMatrix2dFloat::operator*=(const TransformationMatrix2dFloat& other)
{
	posV += rotM * other.posV;
	rotM *= other.rotM;
	return *this;
}

//======================================================================

// multiplies a TransformationMatrix2dFloat to another one
TransformationMatrix2dFloat TransformationMatrix2dFloat::operator*(const TransformationMatrix2dFloat& other) const
{
	return TransformationMatrix2dFloat(*this) *= other;
}

//======================================================================

// comparison of another TransformationMatrix2dFloat to this one
bool TransformationMatrix2dFloat::operator==(const TransformationMatrix2dFloat& other) const
{
	return (rotM == other.rotM && posV == other.posV );
}

//======================================================================

// comparison of another TransformationMatrix2dFloat to this one
bool TransformationMatrix2dFloat::operator!=(const TransformationMatrix2dFloat& other) const
{
	return !(*this == other);
}

//======================================================================

// multiplication with a Vector3
Point2dFloat TransformationMatrix2dFloat::operator*(const Point2dFloat& position) const
{
	return rotM * position + posV;
}

//======================================================================

// get rotation
const RotationMatrix2dFloat& TransformationMatrix2dFloat::getRotation() const
{
	return rotM;
}

//======================================================================

// get position
const Point2dFloat& TransformationMatrix2dFloat::getPosition() const
{
	return posV;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
