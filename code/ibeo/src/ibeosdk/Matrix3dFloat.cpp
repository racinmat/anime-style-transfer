//======================================================================
/*! \file Matrix3dFloat.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff
 * \date Mar 14, 2016
 * \brief A 2x2 Matrix of type float
 *///-------------------------------------------------------------------

#include <ibeosdk/Matrix3dFloat.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

Matrix3dFloat::Matrix3dFloat()
{
	m_c[0].setXYZ(1.0F, 0.0F, 0.0F);
	m_c[1].setXYZ(0.0F, 1.0F, 0.0F);
	m_c[2].setXYZ(0.0F ,0.0F, 1.0F);
}

//======================================================================

Matrix3dFloat
::Matrix3dFloat(const value_type& m11, const value_type& m12, const value_type& m13,
                const value_type& m21, const value_type& m22, const value_type& m23,
                const value_type& m31, const value_type& m32, const value_type& m33)
{
	m_c[0].setXYZ(m11, m21, m31);
	m_c[1].setXYZ(m12, m22, m32);
	m_c[2].setXYZ(m13, m23, m33);
}

//======================================================================

Matrix3dFloat::Matrix3dFloat(const Point3dFloat& c0, const Point3dFloat& c1, const Point3dFloat& c2)
{
	m_c[0] = c0;
	m_c[1] = c1;
	m_c[2] = c2;
}

//======================================================================

Matrix3dFloat& Matrix3dFloat::operator+=(const Matrix3dFloat& other)
{
	m_c[0] += other.m_c[0];
	m_c[1] += other.m_c[1];
	m_c[2] += other.m_c[2];
	return *this;
}

//======================================================================

Matrix3dFloat& Matrix3dFloat::operator-=(const Matrix3dFloat& other)
{
	m_c[0] -= other.m_c[0];
	m_c[1] -= other.m_c[1];
	m_c[2] -= other.m_c[2];
	return *this;
}

//======================================================================

Matrix3dFloat& Matrix3dFloat::operator*=(const value_type& factor)
{
	m_c[0] *= factor;
	m_c[1] *= factor;
	m_c[2] *= factor;
	return *this;
}

//======================================================================

Matrix3dFloat& Matrix3dFloat::operator*=(const Matrix3dFloat& other)
{
	// use c0, c1 and c2 to hold intermediate results.
	const Point3dFloat c0 = (*this)*other.m_c[0];
	const Point3dFloat c1 = (*this)*other.m_c[1];
	const Point3dFloat c2 = (*this)*other.m_c[2];

	m_c[0] = c0;
	m_c[1] = c1;
	m_c[2] = c2;
	return *this;
}

//======================================================================

Matrix3dFloat& Matrix3dFloat::operator/=(const value_type& divisor)
{
	m_c[0] /= divisor;
	m_c[1] /= divisor;
	m_c[2] /= divisor;
	return *this;
}

//======================================================================

Matrix3dFloat Matrix3dFloat::operator+(const Matrix3dFloat& other) const
{
	return Matrix3dFloat(*this) += other;
}

//======================================================================

Matrix3dFloat Matrix3dFloat::operator-(const Matrix3dFloat& other) const
{
	return Matrix3dFloat(*this) -= other;
}
//======================================================================

Matrix3dFloat Matrix3dFloat::operator*(const value_type& factor) const
{
	return Matrix3dFloat(*this) *= factor;
}

//======================================================================

Point3dFloat Matrix3dFloat::operator*(const Point3dFloat& point) const
{
	return (m_c[0]*point.getX() + m_c[1]*point.getY() + m_c[2]*point.getZ());
}

//======================================================================

Matrix3dFloat Matrix3dFloat::operator*(const Matrix3dFloat& other) const
{
	const Point3dFloat p1 = (*this)*other.m_c[0];
	const Point3dFloat p2 = (*this)*other.m_c[1];
	const Point3dFloat p3 = (*this)*other.m_c[2];
	return Matrix3dFloat(p1, p2, p3);
}

//======================================================================

Matrix3dFloat Matrix3dFloat::operator/(const value_type& divisor) const
{
	return Matrix3dFloat(*this) /= divisor;
}

//======================================================================

Matrix3dFloat Matrix3dFloat::operator-() const
{
	return Matrix3dFloat(Point3dFloat(), Point3dFloat(), Point3dFloat()) -= *this;
}

//======================================================================

bool Matrix3dFloat::operator==(const Matrix3dFloat& other) const
{
	return  (m_c[0] == other.m_c[0]
	      && m_c[1] == other.m_c[1]
	      && m_c[2] == other.m_c[2]);
}

//======================================================================

bool Matrix3dFloat::operator!=(const Matrix3dFloat& other) const
{
	return !(*this == other);
}

//======================================================================

Matrix3dFloat Matrix3dFloat::transposed() const
{
	return Matrix3dFloat(Point3dFloat(m_c[0].getX(), m_c[1].getX(), m_c[2].getX()),
	                     Point3dFloat(m_c[0].getY(), m_c[1].getY(), m_c[2].getY()),
	                     Point3dFloat(m_c[0].getZ(), m_c[1].getZ(), m_c[2].getZ()));
}

//======================================================================

Point3dFloat Matrix3dFloat::mulLeft(const Point3dFloat& point) const
{
	return Point3dFloat(point.getX()*m_c[0].getX() + point.getY()*m_c[0].getY(),
	                    point.getX()*m_c[1].getX() + point.getY()*m_c[1].getY(),
	                    point.getZ()*m_c[2].getZ() + point.getZ()*m_c[2].getZ());
}

//======================================================================

Matrix3dFloat::value_type Matrix3dFloat::det() const
{
	return
		m_c[0].getX() * (m_c[1].getY() * m_c[2].getZ() - m_c[1].getZ() * m_c[2].getY()) +
		m_c[0].getY() * (m_c[1].getZ() * m_c[2].getX() - m_c[1].getX() * m_c[2].getZ()) +
		m_c[0].getZ() * (m_c[1].getX() * m_c[2].getY() - m_c[1].getY() * m_c[2].getX());
}

//======================================================================

Matrix3dFloat::value_type Matrix3dFloat::det2(const value_type &a, const value_type &b,
          const value_type &c, const value_type &d)
{
	return a*d - b*c;
}

//======================================================================

Matrix3dFloat Matrix3dFloat::adjointed() const
{
	return Matrix3dFloat(
		Point3dFloat(det2(m_c[1].getY(), m_c[2].getY(), m_c[1].getZ(), m_c[2].getZ()),
		             det2(m_c[2].getX(), m_c[1].getX(), m_c[2].getZ(), m_c[1].getZ()),
		             det2(m_c[1].getX(), m_c[2].getX(), m_c[1].getY(), m_c[2].getY())),
		Point3dFloat(det2(m_c[2].getY(), m_c[0].getY(), m_c[2].getZ(), m_c[0].getZ()),
		             det2(m_c[0].getX(), m_c[2].getX(), m_c[0].getZ(), m_c[2].getZ()),
		             det2(m_c[2].getX(), m_c[0].getX(), m_c[2].getY(), m_c[0].getY())),
		Point3dFloat(det2(m_c[0].getY(), m_c[1].getY(), m_c[0].getZ(), m_c[1].getZ()),
		             det2(m_c[1].getX(), m_c[0].getX(), m_c[1].getZ(), m_c[0].getZ()),
		             det2(m_c[0].getX(), m_c[1].getX(), m_c[0].getY(), m_c[1].getY()))
	);
}

//======================================================================

Matrix3dFloat Matrix3dFloat::inverted() const
{
	return adjointed().transposed() / det();
}

//======================================================================

const Point3dFloat& Matrix3dFloat::getColumn1() const
{
	return m_c[0];
}

//======================================================================

const Point3dFloat& Matrix3dFloat::getColumn2() const
{
	return m_c[1];
}

//======================================================================

const Point3dFloat& Matrix3dFloat::getColumn3() const
{
	return m_c[2];
}

//======================================================================

} // namespace ibeosdk

//======================================================================
