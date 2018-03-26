//======================================================================
/*! \file Matrix3dFloat.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff
 * \date Mar 14, 2016
 * \brief A 2x2 Matrix of type float
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_MATRIX3DFLOAT_HPP_SEEN
#define IBEOSDK_MATRIX3DFLOAT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Point3dFloat.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class Matrix3dFloat {
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
	/*!\brief Constructs a 3x3 identity matrix.
	 *///-------------------------------------
	Matrix3dFloat();

	//========================================
	/*!\brief constructor with entry-wise initialization.
	 *///-------------------------------------
	Matrix3dFloat(const value_type& m11, const value_type& m12, const value_type& m13,
	              const value_type& m21, const value_type& m22, const value_type& m23,
	              const value_type& m31, const value_type& m32, const value_type& m33);

	//========================================
	/*!\brief constructor with columnwise initialization.
	 *///-------------------------------------
	Matrix3dFloat(const Point3dFloat& c0, const Point3dFloat& c1, const Point3dFloat& c2);

	virtual ~Matrix3dFloat() {}

public:
	//========================================
	/*!\brief Adds Matrix3x3 \a other to this one.
	 *///-------------------------------------
	Matrix3dFloat& operator+=(const Matrix3dFloat& other);

	//========================================
	/*!\brief Subtracts Matrix3x3 \a other from this one.
	 *///-------------------------------------
	Matrix3dFloat& operator-=(const Matrix3dFloat& other);

	//========================================
	/*!\brief Multiplies this Matrix3x3 by a factor.
	 *///-------------------------------------
	Matrix3dFloat& operator*=(const value_type& factor);

	//========================================
	/*!\brief Multiplication by another Matrix3x3
	 *///-------------------------------------
	Matrix3dFloat& operator*=(const Matrix3dFloat& other);
	/** divides a Matrix3x3 through a divisor */
	//========================================
	/*!\brief Bivides a Matrix3x3 through a divisor.
	 *///-------------------------------------
	Matrix3dFloat& operator/=(const value_type& divisor);

public:
	//========================================
	/*!\brief Adds Matrix3x3 \a other to this one.
	 *///-------------------------------------
	Matrix3dFloat operator+(const Matrix3dFloat& other) const;


	//========================================
	/*!\brief Subtracts Matrix3x3 \a other from this one.
	 *///-------------------------------------
	Matrix3dFloat operator-(const Matrix3dFloat& other) const;

	//========================================
	/*!\brief Multiplication by a factor
	 *///-------------------------------------
	Matrix3dFloat operator*(const value_type& factor) const;

	/** multiplies this Matrix3x3 by a vector */
	//========================================
	/*!\brief Multiplies this Matrix3x3 by a vector.
	 *///-------------------------------------
	Point3dFloat operator*(const Point3dFloat& point) const;

	//========================================
	/*!\brief Multiplication by another Matrix3x3.
	 *///-------------------------------------
	Matrix3dFloat operator*(const Matrix3dFloat& other) const;

	//========================================
	/*!\brief Bivision through a factor.
	 *///-------------------------------------
	Matrix3dFloat operator/(const value_type& divisor) const;

	//========================================
	/*!\brief negates this Matrix3x3.
	 *///-------------------------------------
	Matrix3dFloat operator-() const;

public:
	//========================================
	/*!\brief comparison of Matrix3x3 \a other to this one
	 *///-------------------------------------
	bool operator==(const Matrix3dFloat& other) const;

	//========================================
	/*!\brief comparison of Matrix3x3 \a other to this one
	 *///-------------------------------------
	bool operator!=(const Matrix3dFloat& other) const;

public:
	//========================================
	/*!\brief returns the transposed Matrix
	 *///-------------------------------------
	Matrix3dFloat transposed() const;

	//========================================
	/*!\brief left side multiplication..
	 *///-------------------------------------
	Point3dFloat mulLeft(const Point3dFloat& point) const;

	//========================================
	/*!\brief Calculation of the determinant of this matrix.
	 *///-------------------------------------
	value_type det() const;

	//========================================
	/*!\brief Calculation of the subdeterminant of this matrix.
	 *///-------------------------------------
	static value_type det2(const value_type &a, const value_type &b,
	            const value_type &c, const value_type &d);

	//========================================
	/*!\brief Calculate the adjoint of this matrix.
	 *///-------------------------------------
	Matrix3dFloat adjointed() const;

	//========================================
	/*!\brief Calculate the inverse of this matrix.
	 *///-------------------------------------
	Matrix3dFloat inverted() const;

	//========================================
	/*!\brief returns the first column as Point3dFloat.
	 *///-------------------------------------
	const Point3dFloat& getColumn1() const;

	//========================================
	/*!\brief returns the second column as Point3dFloat.
	 *///-------------------------------------
	const Point3dFloat& getColumn2() const;

	/*!\brief returns the second column as Point3dFloat.
	 *///-------------------------------------
	const Point3dFloat& getColumn3() const;

	//========================================
	/*!\brief Reset the matrix to unity.
	 *///-------------------------------------
	void setToUnity()
	{
		m_c[0].setXYZ(1.F,0.F,0.F);
		m_c[1].setXYZ(0.F,1.F,0.F);
		m_c[2].setXYZ(0.F,0.F,1.F);
	}

private:
	//========================================
	/*!\brief Array with columns of the matrix.
	 *///-------------------------------------
	Point3dFloat m_c[3];
}; // Matrix3dFloat

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // MATRIX3DFLOAT_HPP_SEEN

//======================================================================
