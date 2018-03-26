//======================================================================
/*! \file Point3dDouble.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer
 * \date Mar 17, 2016
 *///-------------------------------------------------------------------
//======================================================================


#include <ibeosdk/datablocks/snippets/Point3dDouble.hpp>

#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

Point3dDouble::Point3dDouble()
  : m_x(0),
    m_y(0),
    m_z(0)
{}

//======================================================================

Point3dDouble::Point3dDouble(const double x, const double y, const double z)
  : m_x(x),
    m_y(y),
    m_z(z)
{}

//======================================================================

Point3dDouble::~Point3dDouble() {}

//======================================================================

Point3dDouble::Point3dDouble(const Point3dDouble& other)
  : m_x(other.m_x),
    m_y(other.m_y),
    m_z(other.m_z)
{}

//======================================================================

Point3dDouble& Point3dDouble::operator=(const Point3dDouble& other)
{
	this->m_x = other.m_x;
	this->m_y = other.m_y;
	this->m_z = other.m_z;

	return *this;
}

//======================================================================

Point3dDouble& Point3dDouble::operator+=(const Point3dDouble& other)
{
	this->m_x += other.m_x;
	this->m_y += other.m_y;
	this->m_z += other.m_z;
	return *this;
}

//======================================================================

Point3dDouble&  Point3dDouble::operator*=(const double factor)
{
	this->m_x *= factor;
	this->m_y *= factor;
	this->m_z *= factor;
	return *this;
}

//======================================================================

Point3dDouble& Point3dDouble::operator-=(const Point3dDouble& point)
{
	this->m_x -= point.m_x;
	this->m_y -= point.m_y;
	return *this;
}

//======================================================================

Point3dDouble& Point3dDouble::operator/=(const double divisor)
{
	m_x /= divisor;
	m_y /= divisor;
	m_z /= divisor;
	return *this;
}

//======================================================================

double Point3dDouble::getAngleAroundX() const
{
	double angle;

	// atan2(z,y) returns the angle against the y-axis
	angle = std::atan2(m_z, m_y);

	return angle;
}

//======================================================================

double Point3dDouble::getAngleAroundY() const
{
	double angle;

	// atan2(y,x) returns the angle against the x-axis
	angle = std::atan2(m_x, m_z);

	return angle;
}

//======================================================================

double Point3dDouble::getAngleAroundZ() const
{
	double angle;

	// atan2(y,x) returns the angle against the x-axis
	angle = std::atan2(m_y, m_x);

	return angle;
}

//======================================================================

void Point3dDouble::rotateAroundZ(const double yawAngle)
{
	const double dCos = std::cos(yawAngle);
	const double dSin = std::sin(yawAngle);

	const double x = m_x * dCos - m_y * dSin;
	const double y = m_x * dSin + m_y * dCos;

	this->m_x = x;
	this->m_y = y;
}

//======================================================================

void Point3dDouble::rotateAroundX(const double rollAngle)
{
	const double dCos = std::cos(rollAngle);
	const double dSin = std::sin(rollAngle);

	const double y = -m_z * dSin + m_y * dCos;
	const double z = m_z * dCos + m_y * dSin;

	this->m_z = z;
	this->m_y = y;
}

//======================================================================

void Point3dDouble::rotateAroundY(const double pitchAngle)
{
	const double dCos = std::cos(pitchAngle);
	const double dSin = std::sin(pitchAngle);

	const double x = m_z * dSin + m_x * dCos;
	const double z = m_z * dCos - m_x * dSin;

	this->m_z = z;
	this->m_x = x;
}

//======================================================================

bool Point3dDouble::isZero() const
{
	return std::abs(this->m_x) < 1E-11 && std::abs(this->m_y) < 1E-11 && std::abs(this->m_z) < 1E-11;
}

//======================================================================

double Point3dDouble::length() const
{
	return std::sqrt(this->m_x * this->m_x + this->m_y * this->m_y + this->m_z * this->m_z);
}

//======================================================================

std::streamsize Point3dDouble::getSerializedSize_static()
{
	return 3*std::streamsize(sizeof(double));
}


//======================================================================

std::streamsize Point3dDouble::getSerializedSize() const
{
	return getSerializedSize_static();
}

//======================================================================

bool Point3dDouble::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, this->m_x);
	ibeosdk::readBE(is, this->m_y);
	ibeosdk::readBE(is, this->m_z);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool Point3dDouble::serialize(std::ostream& os) const
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
