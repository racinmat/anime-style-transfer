//======================================================================
/*! \file RotationMatrix3dFloat.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 14, 2016
 * \brief A 3x3 Rotationmatrix of type float
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/RotationMatrix3dFloat.hpp>
#include <cmath>

//======================================================================

namespace ibeosdk {

//======================================================================

RotationMatrix3dFloat RotationMatrix3dFloat::rotX(const RotationMatrix3dFloat::value_type &alpha) const
{
	return (*this) * RotationMatrix3dFloat(1.0f,            0.0f,             0.0f,
	                                       0.0f, std::cos(alpha), -std::sin(alpha),
	                                       0.0f, std::sin(alpha),  std::cos(alpha));
}

//======================================================================

RotationMatrix3dFloat RotationMatrix3dFloat::rotY(const RotationMatrix3dFloat::value_type &alpha) const
{
	return (*this) * RotationMatrix3dFloat( std::cos(alpha),    0.0, std::sin(alpha),
	                                                   0.0f,    1.0f,           0.0f,
	                                       -std::sin(alpha),    0.0f, std::cos(alpha) );
}

//======================================================================

RotationMatrix3dFloat RotationMatrix3dFloat::rotZ(const RotationMatrix3dFloat::value_type &alpha) const
{
	return (*this) * RotationMatrix3dFloat(std::cos(alpha), -std::sin(alpha),    0.0f,
	                                       std::sin(alpha),  std::cos(alpha),    0.0f,
	                                                  0.0f,             0.0f,    1.0f );
}

//======================================================================

RotationMatrix3dFloat RotationMatrix3dFloat::invert() const
{
	return this->transposed();
}

//======================================================================

void RotationMatrix3dFloat::rotMatX(const RotationMatrix3dFloat::value_type &alpha)
{
	(*this) *= RotationMatrix3dFloat(1.0f,            0.0f,             0.0f,
	                                 0.0f, std::cos(alpha), -std::sin(alpha),
	                                 0.0f, std::sin(alpha),  std::cos(alpha));
}

//======================================================================

void RotationMatrix3dFloat::rotMatY(const RotationMatrix3dFloat::value_type &alpha)
{
	(*this) *= RotationMatrix3dFloat( std::cos(alpha),    0.0, std::sin(alpha),
	                                             0.0f,    1.0f,           0.0f,
	                                 -std::sin(alpha),    0.0f, std::cos(alpha) );
}

//======================================================================

void RotationMatrix3dFloat::rotMatZ(const RotationMatrix3dFloat::value_type &alpha)
{
	(*this) *= RotationMatrix3dFloat(std::cos(alpha), -std::sin(alpha),    0.0f,
	                                 std::sin(alpha),  std::cos(alpha),    0.0f,
	                                            0.0f,             0.0f,    1.0f );
}

//==============================================================================

RotationMatrix3dFloat::RotationMatrix3dFloat(const Point3dFloat& alignment)
{
	if (alignment.isZero()) {
		*this = RotationMatrix3dFloat();
	}
	else {
		const float l = alignment.length();
		if (l == 0) {
			*this =  RotationMatrix3dFloat::rotY(float(M_PI_2));
		}
		else {
			RotationMatrix3dFloat ret;
			ret = ret.rotZ(alignment.getAngleAroundZ());
			ret = ret.rotY(-std::atan2(alignment.getZ(), l));
			*this = ret;
		}
	}
}

//======================================================================

} //namespace ibeosdk

//======================================================================
