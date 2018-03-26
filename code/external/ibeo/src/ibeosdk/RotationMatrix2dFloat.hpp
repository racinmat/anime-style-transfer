//======================================================================
/*! \file RotationMatrix2dFloat.hpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief A 2x2 Rotationmatrix of type float
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_ROTATIONMATRIX2DFLOAT_HPP
#define IBEOSDK_ROTATIONMATRIX2DFLOAT_HPP

//==============================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/Matrix2dFloat.hpp>

//==============================================================================

namespace ibeosdk {

//==============================================================================

class RotationMatrix2dFloat : public Matrix2dFloat {
public:
	//========================================
	/*!\brief The type of the stored elements
	 *///-------------------------------------
	typedef Matrix2dFloat::value_type value_type;

	//========================================
	/*!\brief Alias for value_type
	 *///-------------------------------------
	typedef Matrix2dFloat::floatingpoint_type floatingpoint_type;

public:
	//========================================
	/*!\brief default Constructor
	 *///-------------------------------------
	RotationMatrix2dFloat()
	  : Matrix2dFloat()
	{}

	//========================================
	/*!\brief columnwise constructor
	 *///-------------------------------------
	RotationMatrix2dFloat(const Point2dFloat& c0, const Point2dFloat& c1)
	  : Matrix2dFloat(c0, c1)
	{}

	//========================================
	/*!\brief Copy constructor.
	 *///-------------------------------------
	RotationMatrix2dFloat(const RotationMatrix2dFloat& other)
	  : Matrix2dFloat(other)
	{}

	//========================================
	/*!\brief Copy constructor.
	 *///-------------------------------------
	RotationMatrix2dFloat(const Matrix2dFloat& other)
	  : Matrix2dFloat(other)
	{}

	//========================================
	/*!\brief constructor with entry-wise initialization
	 *///-------------------------------------
	RotationMatrix2dFloat(const value_type m11, const value_type m12,
	                      const value_type m21, const value_type m22)
	  : Matrix2dFloat(m11,m12,m21,m22)
	{}

	//========================================
	/*!\brief rotation constructor
	 *///-------------------------------------
	RotationMatrix2dFloat(const float alpha)
	  : Matrix2dFloat(cosf(alpha), -sinf(alpha), sinf(alpha), cosf(alpha))
	{}

public:
	///
	//========================================
	/*!\brief returns the inverted RotationMatrix2dFloat.
	 *
	 * \note Note that inverting RotationMatrices
	 * \todo Check this comment.
	 *///-------------------------------------
	RotationMatrix2dFloat inverted() const
	{
		return RotationMatrix2dFloat(this->transposed());
	}

	//========================================
	/*!\brief Get the angle of the rotation.
	 *///-------------------------------------
	value_type getAngle() const
	{
		return atan2f(getColumn1().getY(), getColumn1().getX());
	}
}; // class RotationMatrix2dFloat

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_ROTATIONMATRIX2DFLOAT_HPP

//======================================================================
