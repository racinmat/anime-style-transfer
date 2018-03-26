//======================================================================
/*! \file Point2dFloat.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 27, 2012
 * \date May 30, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/Point2dFloat.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

Point2dFloat::Point2dFloat()
  : m_x(0.0),
    m_y(0.0)
{}

//======================================================================

Point2dFloat::Point2dFloat(const float x, const float y)
  : m_x(x),
    m_y(y)
{}

//======================================================================

Point2dFloat::Point2dFloat(const Point2dFloat& other)
  : m_x(other.m_x),
    m_y(other.m_y)
{}

//======================================================================

Point2dFloat& Point2dFloat::operator=(const Point2dFloat& other)
{
	this->m_x = other.m_x;
	this->m_y = other.m_y;

	return *this;
}

//======================================================================

Point2dFloat::~Point2dFloat() {}

//======================================================================

Point2dFloat& Point2dFloat::operator+=(const Point2dFloat& other)
{
	this->m_x += other.m_x;
	this->m_y += other.m_y;
	return *this;
}

//======================================================================

Point2dFloat Point2dFloat::operator+(const Point2dFloat& other) const
{
	return Point2dFloat(this->m_x + other.m_x,
	                    this->m_y + other.m_y);
}

//======================================================================

Point2dFloat Point2dFloat::operator-(const Point2dFloat& other) const
{
	return Point2dFloat(this->m_x - other.m_x,
	                    this->m_y - other.m_y);
}

//======================================================================

Point2dFloat& Point2dFloat::operator-=(const Point2dFloat& point)
{
	m_x -= point.m_x;
	m_y -= point.m_y;
	return *this;
}

//======================================================================

Point2dFloat& Point2dFloat::operator/=(const float divisor)
{
	m_x /= divisor;
	m_y /= divisor;
	return *this;
}

//======================================================================

float Point2dFloat::operator*(const Point2dFloat& other) const
{
	return this->m_x * other.m_x + this->m_y * other.m_y;
}

//======================================================================

Point2dFloat& Point2dFloat::operator*=(const float factor)
{
	m_x *= factor;
	m_y *= factor;
	return *this;
}

//======================================================================

Point2dFloat Point2dFloat::operator/(const float other) const
{
	return Point2dFloat(this->m_x / other, this->m_y / other);
}

//======================================================================

bool Point2dFloat::operator==(const Point2dFloat& other) const
{
	return this->m_x == other.m_x && this->m_y == other.m_y;
}

//======================================================================

bool Point2dFloat::operator!=(const Point2dFloat& other) const
{
	return !(*this == other);
}

//======================================================================

bool Point2dFloat::isZero() const
{
	return std::abs(this->m_x) < 1E-11 && std::abs(this->m_y) < 1E-11;
}

//======================================================================

Point2dFloat Point2dFloat::normalized() const
{
	// return, if zero point
	if (std::abs(this->m_x) < 1E-11 && std::abs(this->m_y) < 1E-11)
		return *this;

	return *this / this->dist();
}

//======================================================================

void Point2dFloat::rotate(float angle)
{
	*this = rotated(angle);
}

//======================================================================

Point2dFloat Point2dFloat::rotated(const float angle_rad) const
{
	const float dCos = float(cos(angle_rad));
	const float dSin = float(sin(angle_rad));

	return Point2dFloat(m_x * dCos - m_y * dSin,
	                    m_x * dSin + m_y * dCos);
}


//======================================================================

bool Point2dFloat::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, this->m_x);
	ibeosdk::readBE(is, this->m_y);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool Point2dFloat::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, this->m_x);
	ibeosdk::writeBE(os, this->m_y);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const Point2dFloat& p)
{
	os << "(" << p.getX() << ", " << p.getY() << ")";
	return os;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
