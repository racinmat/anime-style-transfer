//======================================================================
/*! \file Point3dFloat.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 14, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/Point3dFloat.hpp>

#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

Point3dFloat::Point3dFloat()
  : m_x(0),
    m_y(0),
    m_z(0)
{}

//======================================================================

Point3dFloat::Point3dFloat(const float x, const float y, const float z)
  : m_x(x),
    m_y(y),
    m_z(z)
{}

//======================================================================

Point3dFloat::~Point3dFloat() {}

//======================================================================

Point3dFloat::Point3dFloat(const Point3dFloat& other)
  : m_x(other.m_x),
    m_y(other.m_y),
    m_z(other.m_z)
{}

//======================================================================

Point3dFloat& Point3dFloat::operator=(const Point3dFloat& other)
{
	this->m_x = other.m_x;
	this->m_y = other.m_y;
	this->m_z = other.m_z;

	return *this;
}

//======================================================================

Point3dFloat& Point3dFloat::operator+=(const Point3dFloat& other)
{
	this->m_x += other.m_x;
	this->m_y += other.m_y;
	this->m_z += other.m_z;
	return *this;
}

//======================================================================

Point3dFloat&  Point3dFloat::operator*=(const float factor)
{
	this->m_x *= factor;
	this->m_y *= factor;
	this->m_z *= factor;
	return *this;
}

//======================================================================

Point3dFloat& Point3dFloat::operator-=(const Point3dFloat& point)
{
	this->m_x -= point.m_x;
	this->m_y -= point.m_y;
	return *this;
}

//======================================================================

Point3dFloat& Point3dFloat::operator/=(const float divisor)
{
	m_x /= divisor;
	m_y /= divisor;
	m_z /= divisor;
	return *this;
}

//======================================================================

float Point3dFloat::getAngleAroundX() const
{
	float angle;

	// atan2(z,y) returns the angle against the y-axis
	angle = std::atan2(m_z, m_y);

	return angle;
}

//======================================================================

float Point3dFloat::getAngleAroundY() const
{
	float angle;

	// atan2(y,x) returns the angle against the x-axis
	angle = std::atan2(m_x, m_z);

	return angle;
}

//======================================================================

float Point3dFloat::getAngleAroundZ() const
{
	float angle;

	// atan2(y,x) returns the angle against the x-axis
	angle = std::atan2(m_y, m_x);

	return angle;
}

//======================================================================

void Point3dFloat::rotateAroundZ(const float yawAngle)
{
	const float dCos = std::cos(yawAngle);
	const float dSin = std::sin(yawAngle);

	const float x = m_x * dCos - m_y * dSin;
	const float y = m_x * dSin + m_y * dCos;

	this->m_x = x;
	this->m_y = y;
}

//======================================================================

void Point3dFloat::rotateAroundX(const float rollAngle)
{
	const float dCos = std::cos(rollAngle);
	const float dSin = std::sin(rollAngle);

	const float y = -m_z * dSin + m_y * dCos;
	const float z = m_z * dCos + m_y * dSin;

	this->m_z = z;
	this->m_y = y;
}

//======================================================================

void Point3dFloat::rotateAroundY(const float pitchAngle)
{
	const float dCos = std::cos(pitchAngle);
	const float dSin = std::sin(pitchAngle);

	const float x = m_z * dSin + m_x * dCos;
	const float z = m_z * dCos - m_x * dSin;

	this->m_z = z;
	this->m_x = x;
}

//======================================================================

bool Point3dFloat::isZero() const
{
	return std::abs(this->m_x) < 1E-11 && std::abs(this->m_y) < 1E-11 && std::abs(this->m_z) < 1E-11;
}

//======================================================================

float Point3dFloat::length() const
{
	return std::sqrt(this->m_x * this->m_x + this->m_y * this->m_y + this->m_z * this->m_z);
}

//======================================================================

std::streamsize Point3dFloat::getSerializedSize_static()
{
	return 3*std::streamsize(sizeof(float));
}


//======================================================================

std::streamsize Point3dFloat::getSerializedSize() const
{
	return getSerializedSize_static();
}

//======================================================================

bool Point3dFloat::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, this->m_x);
	ibeosdk::readBE(is, this->m_y);
	ibeosdk::readBE(is, this->m_z);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool Point3dFloat::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, this->m_x);
	ibeosdk::writeBE(os, this->m_y);
	ibeosdk::writeBE(os, this->m_z);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
