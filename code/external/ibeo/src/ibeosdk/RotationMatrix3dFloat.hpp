//======================================================================
//======================================================================
/*! \file RotationMatrix3dFloat.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 14, 2016
 * \brief A 3x3 Rotationmatrix of type float
 *///-------------------------------------------------------------------

//======================================================================

#ifndef IBEOSDK_ROTATIONMATRIX3DFLOAT_HPP_SEEN
#define IBEOSDK_ROTATIONMATRIX3DFLOAT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/Matrix3dFloat.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class RotationMatrix3dFloat : public Matrix3dFloat {
public:
	//========================================
	/*!\brief The type of the stored elements
	 *///-------------------------------------
	typedef Matrix3dFloat::value_type value_type;

	//========================================
	/*!\brief Alias for value_type
	 *///-------------------------------------
	typedef Matrix3dFloat::floatingpoint_type floatingpoint_type;

public:
	//========================================
	/*!\brief Default Constructor
	 *///-------------------------------------
	RotationMatrix3dFloat(): Matrix3dFloat() {}

	//========================================
	/*!\brief Constructor which aligns the x-axies to the given vector.
	 *///-------------------------------------
	RotationMatrix3dFloat(const Point3dFloat& alignment);

	//========================================
	/*!\brief Constructor with columnwise initialization.
	 *///-------------------------------------
	RotationMatrix3dFloat(const Point3dFloat& c0, const Point3dFloat& c1, const Point3dFloat& c2)
	  : Matrix3dFloat(c0, c1, c2)
	{}

	//========================================
	/*!\brief Copy constructor.
	 *///-------------------------------------
	RotationMatrix3dFloat(const RotationMatrix3dFloat& other)
	  : Matrix3dFloat(other)
	{}

	//========================================
	/*!\brief From Matrix3dFloat.
	 *///-------------------------------------
	RotationMatrix3dFloat(const Matrix3dFloat& other)
	  : Matrix3dFloat(other)
	{}

	//========================================
	/*!\brief Constructor with elementwise initialization.
	 *///-------------------------------------
	RotationMatrix3dFloat(const value_type& m11, const value_type& m12, const value_type& m13,
	                      const value_type& m21, const value_type& m22, const value_type& m23,
	                      const value_type& m31, const value_type& m32, const value_type& m33)
	  : Matrix3dFloat(m11, m12, m13, m21, m22, m23, m31, m32, m33)
	{}

	//========================================
	/*!\brief Returns the rotated matrix about the x-axis (roll).
	 *///-------------------------------------
	RotationMatrix3dFloat rotX(const value_type& alpha) const;

	//========================================
	/*!\brief Returns the rotated matrix about the y-axis (pitch).
	 *///-------------------------------------
	RotationMatrix3dFloat rotY(const value_type& alpha) const ;

	//========================================
	/*!\brief Returns the rotated matrix about the z-axis (yaw).
	 *///-------------------------------------
	RotationMatrix3dFloat rotZ(const value_type& alpha) const;

	//========================================
	/*!\brief Returns the inverted RotationMatrix3dFloat.
	 * Note that inverting RotationMatrices.
	 *///-------------------------------------
	RotationMatrix3dFloat invert() const;

public: // in place rotations
	//! rotates the matrix about the x-axis (roll)
	void rotMatX(const value_type& alpha);

	//! rotates the matrix about the y-axis (pitch)
	void rotMatY(const value_type& alpha) ;

	//! rotates the matrix about the z-axis (yaw)
	void rotMatZ(const value_type& alpha);
}; // RotationMatrix3dFloat

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_ROTATIONMATRIX3DFLOAT_HPP_SEEN

//======================================================================
