//======================================================================
/*! \file TransformationMatrix2dFloat.hpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief A 2x2 kinematic matrix of type float
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_TRANSFORMATIONMATRIX2DFLOAT_HPP
#define IBEOSDK_TRANSFORMATIONMATRIX2DFLOAT_HPP

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/RotationMatrix2dFloat.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class TransformationMatrix2dFloat {
public:
	//========================================
	/*!\brief The type of the stored elements
	 *///-------------------------------------
	typedef RotationMatrix2dFloat::value_type value_type;

	///
	//========================================
	/*!\brief Alias for value_type.
	 *///-------------------------------------
	typedef value_type floatingpoint_type;

public:
	//========================================
	/*!\brief default constructor (creates Identity Matrix)
	 *///-------------------------------------
	TransformationMatrix2dFloat()
	  : rotM(),
	    posV()
	{}

	//========================================
	/*!\brief constructor with initialization of the RotationMatrix
	 *///-------------------------------------
	TransformationMatrix2dFloat(const RotationMatrix2dFloat& rm)
	  : rotM(rm),
	    posV()
	{}

	//========================================
	/*!\brief constructor with initialization of the position-vector
	 *///-------------------------------------
	TransformationMatrix2dFloat(const Point2dFloat& point)
	  : rotM(),
	    posV(point)
	{}

	//========================================
	/*!\brief constructor with initialization of the RotationMatrix and the position-vector
	 *///-------------------------------------
	TransformationMatrix2dFloat(const RotationMatrix2dFloat& rm,
	                           const Point2dFloat& point)
	  : rotM(rm),
	    posV(point)
	{}

	//========================================
	/*!\brief constructor via angle and position vector
	 *///-------------------------------------
	TransformationMatrix2dFloat(const value_type& angle, const Point2dFloat& point)
	  : rotM(angle),
	    posV(point)
	{}

	//========================================
	/*!\brief constructor via angle only
	 *///-------------------------------------
	TransformationMatrix2dFloat(const value_type& angle)
	  : rotM(angle),
	    posV()
	{}

	//========================================
	/*!\brief copy constructor
	 *///-------------------------------------
	TransformationMatrix2dFloat(const TransformationMatrix2dFloat& other)
	  : rotM(other.rotM),
	    posV(other.posV)
	{}

	//========================================
	/*!\brief Adds a TransformationMatrix2dFloat to this one.
	 *///-------------------------------------
	TransformationMatrix2dFloat& operator+=(const TransformationMatrix2dFloat& other);

	//========================================
	/*!\brief Subtracts a TransformationMatrix2dFloat from this one.
	 *///-------------------------------------
	TransformationMatrix2dFloat& operator-=(const TransformationMatrix2dFloat& other);

	//========================================
	/*!\brief Multiplies a TransformationMatrix2dFloat to this one.
	 *///-------------------------------------
	TransformationMatrix2dFloat& operator*=(const TransformationMatrix2dFloat& other);

public:
	//========================================
	/*!\brief Adds a TransformationMatrix2dFloat to another one.
	 *///-------------------------------------
	TransformationMatrix2dFloat operator+(const TransformationMatrix2dFloat& other) const;

	//========================================
	/*!\brief Subtracts a TransformationMatrix2dFloat from another one.
	 *///-------------------------------------
	TransformationMatrix2dFloat operator-(const TransformationMatrix2dFloat& other) const;

	//========================================
	/*!\brief Multiplies a TransformationMatrix2dFloat to another one.
	 *///-------------------------------------
	TransformationMatrix2dFloat operator*(const TransformationMatrix2dFloat& other) const;

	//========================================
	/*!\brief multiplication with a vector
	 *///-------------------------------------
	Point2dFloat operator*(const Point2dFloat& position) const;

public:
	//========================================
	/*!\brief Comparison of another TransformationMatrix2dFloat to this one.
	 *///-------------------------------------
	bool operator==(const TransformationMatrix2dFloat& other) const;

	//========================================
	/*!\brief Comparison of another TransformationMatrix2dFloat to this one.
	 *///-------------------------------------
	bool operator!=(const TransformationMatrix2dFloat& other) const;

public:
	//========================================
	/*!\brief Inverts the TransformationMatrix2dFloat.
	 *///-------------------------------------
	TransformationMatrix2dFloat inverted() const;

	//========================================
	/*!\brief around z-axis rotated matrix
	 *///-------------------------------------
	TransformationMatrix2dFloat rot(const value_type alpha) const;

	//========================================
	/*!\brief along x-axis shifted matrix
	 *///-------------------------------------
	TransformationMatrix2dFloat transX(const value_type distance) const;

	//========================================
	/*!\brief along y-axis shifted matrix
	 *///-------------------------------------
	TransformationMatrix2dFloat transY(const value_type distance) const;

public:
	//========================================
	/*!\brief get the rotation matrix
	 *///-------------------------------------
	const RotationMatrix2dFloat& getRotation() const;

	//========================================
	/*!\brief get the position
	 *///-------------------------------------
	const Point2dFloat& getPosition() const;

private:
	//========================================
	/*!\brief The RotationMatrix.
	 *///-------------------------------------
	RotationMatrix2dFloat rotM;

	//========================================
	/*!\brief The position vector.
	 *///-------------------------------------
	Point2dFloat posV;
}; // class TransformationMatrix2dFloat

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_TRANSFORMATIONMATRIX2DFLOAT_HPP

//======================================================================
