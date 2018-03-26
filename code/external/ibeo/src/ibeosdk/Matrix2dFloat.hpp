//======================================================================
/*! \file Matrix2dFloat.hpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief A 2x2 Matrix of type float
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_MATRIX2DFLOAT_HPP
#define IBEOSDK_MATRIX2DFLOAT_HPP

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Point2dFloat.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class Matrix2dFloat {
public:
	//========================================
	/*!\brief The type of the stored elements.
	 *///-------------------------------------
	typedef float value_type;

	//========================================
	/*!\brief Alias for value_type.
	 *///-------------------------------------
	typedef value_type floatingpoint_type;

public:
	//========================================
	/*!\brief Constructs a 2x2 identity matrix.
	 *///-------------------------------------
	Matrix2dFloat();

	//========================================
	/*!\brief copy-constructor
	 *///-------------------------------------
	Matrix2dFloat(const Matrix2dFloat& other);

	//========================================
	/*!\brief constructor with entry-wise initialization.
	 *///-------------------------------------
	Matrix2dFloat(const value_type m11, const value_type m12,
	              const value_type m21, const value_type m22);

	//========================================
	/*!\brief constructor with columnwise initialization.
	 *///-------------------------------------
	Matrix2dFloat(const Point2dFloat& c0, const Point2dFloat& c1);

public:
	//========================================
	/*!\brief Adds Matrix2x2 \a other to this one.
	 *///-------------------------------------
	Matrix2dFloat& operator+=(const Matrix2dFloat& other);

	//========================================
	/*!\brief Adds Matrix2x2 \a other to this one.
	 *///-------------------------------------
	Matrix2dFloat operator+(const Matrix2dFloat& other) const;

	//========================================
	/*!\brief Subtracts Matrix2x2 \a other from this one.
	 *///-------------------------------------
	Matrix2dFloat& operator-=(const Matrix2dFloat& other);

	//========================================
	/*!\brief Subtracts Matrix2x2 \a other from this one.
	 *///-------------------------------------
	Matrix2dFloat operator-(const Matrix2dFloat& other) const;

	//========================================
	/*!\brief Multiplies this Matrix2x2 by a factor.
	 *///-------------------------------------
	Matrix2dFloat& operator*=(const value_type factor);

	//========================================
	/*!\brief Multiplication by a factor
	 *///-------------------------------------
	Matrix2dFloat operator*(const value_type factor) const;

	/** multiplies this Matrix2x2 by a vector */
	//========================================
	/*!\brief Multiplies this Matrix2x2 by a vector.
	 *///-------------------------------------
	Point2dFloat operator*(const Point2dFloat& point) const;

	//========================================
	/*!\brief Multiplication by another Matrix2x2.
	 *///-------------------------------------
	Matrix2dFloat operator*(const Matrix2dFloat& other) const;

	//========================================
	/*!\brief Multiplication by another Matrix2x2
	 *///-------------------------------------
	Matrix2dFloat& operator*=(const Matrix2dFloat& other);

	/** divides a Matrix2x2 through a divisor */
	//========================================
	/*!\brief Bivides a Matrix2x2 through a divisor.
	 *///-------------------------------------
	Matrix2dFloat& operator/=(const value_type divisor);

	//========================================
	/*!\brief Bivision through a factor.
	 *///-------------------------------------
	Matrix2dFloat operator/(const value_type divisor) const;

	//========================================
	/*!\brief negates this Matrix2x2.
	 *///-------------------------------------
	Matrix2dFloat operator-() const;

	//========================================
	/*!\brief comparison of Matrix2x2 \a other to this one
	 *///-------------------------------------
	bool operator==(const Matrix2dFloat& other) const;

	//========================================
	/*!\brief comparison of Matrix2x2 \a other to this one
	 *///-------------------------------------
	bool operator!=(const Matrix2dFloat& other) const;

public:
	//========================================
	/*!\brief returns the transposed Matrix
	 *///-------------------------------------
	Matrix2dFloat transposed() const;

	//========================================
	/*!\brief left side multiplication..
	 *///-------------------------------------
	Point2dFloat mulLeft(const Point2dFloat& point) const;

	//========================================
	/*!\brief Calculation of the determinant of this matrix.
	 *///-------------------------------------
	value_type det() const;

	//========================================
	/*!\brief Calculate the inverse of this matrix
	 *///-------------------------------------
	Matrix2dFloat inverted() const;

	//========================================
	/*!\brief returns the first column as Point2dFloat.
	 *///-------------------------------------
	Point2dFloat getColumn1() const;

	//========================================
	/*!\brief returns the second column as Point2dFloat.
	 *///-------------------------------------
	Point2dFloat getColumn2() const;

private:
	//========================================
	/*!\brief Array with columns of the matrix.
	 *///-------------------------------------
	Point2dFloat c[2];
}; // class Matrix2dFloat

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif //IBEOSDK_MATRIX2DFLOAT_HPP

//======================================================================
