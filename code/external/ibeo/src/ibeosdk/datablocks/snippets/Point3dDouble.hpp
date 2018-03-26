//======================================================================
/*! \file Point3dDouble.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer
 * \date Mar 17, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef IBEOSDK_POINT3DDOUBLE_HPP_SEEN
#define IBEOSDK_POINT3DDOUBLE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <ibeosdk/Math.hpp>

#include <cassert>

//======================================================================

namespace ibeosdk {

//======================================================================

class Point3dDouble : public Snippet {
public:
	static std::streamsize getSerializedSize_static();

public:
	Point3dDouble();
	Point3dDouble(const double x, const double y, const double z);
	Point3dDouble(const Point3dDouble& other);
	Point3dDouble& operator=(const Point3dDouble& other);
	virtual ~Point3dDouble();

public:
	Point3dDouble& operator+=(const Point3dDouble& other);
	Point3dDouble& operator*=(const double factor);
	Point3dDouble& operator-=(const Point3dDouble& point);
	Point3dDouble& operator/=(const double divisor);

public:
	double getX() const { return this->m_x; }
	double getY() const { return this->m_y; }
	double getZ() const { return this->m_z; }
	double& getX() { return this->m_x; }
	double& getY() { return this->m_y; }
	double& getZ() { return this->m_z; }

	void setX(const double val) { this->m_x = val; }
	void setY(const double val) { this->m_y = val; }
	void setZ(const double val) { this->m_z = val; }
	void setXYZ(const double x,const  double y, const double z) { this->m_x = x; this->m_y = y; this->m_z = z; }

	double getAngleAroundX() const; //!< Returns the rotation angle around x (in the z-y-plane)
	double getAngleAroundY() const; //!< Returns the rotation angle around y (in the z-x-plane)
	double getAngleAroundZ() const; //!< Returns the rotation angle around z (in the x-y-plane)

	void rotateAroundX(const double rollAngle);  //!< Rotate the point around the X-axis ("Roll angle")
	void rotateAroundY(const double pitchAngle); //!< Rotate the point around the Y-axis ("Pitch angle")
	void rotateAroundZ(const double dYawAngle);  //!< Rotate the point around the Z-axis ("Yaw angle")

	bool isZero() const;
	double length() const;
	double distToOrigin() const { return length(); }

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

//protected:
	friend inline bool operator==(const Point3dDouble&, const Point3dDouble&);
	friend inline bool operator!=(const Point3dDouble&, const Point3dDouble&);
	friend inline const Point3dDouble operator+(const Point3dDouble&, const Point3dDouble&);
	friend inline const Point3dDouble operator-(const Point3dDouble&, const Point3dDouble&);
	friend inline double operator*(const Point3dDouble&, const Point3dDouble&);
	friend inline const Point3dDouble operator*(const double, const Point3dDouble&);
	friend inline const Point3dDouble operator*(const Point3dDouble&, double);
	friend inline const Point3dDouble operator-(const Point3dDouble&);
	friend inline const Point3dDouble operator/(const Point3dDouble&, double);

private:
	double m_x;
	double m_y;
	double m_z;
}; //Point3dDouble

//======================================================================
//======================================================================

inline bool operator==(const Point3dDouble& p1, const Point3dDouble& p2)
{
	return    (    ibeosdk::fuzzyCompareT<11>(p1.m_x, p2.m_x)
	           || (std::isnan(p1.m_x) && std::isnan(p2.m_x)))
	       && (    ibeosdk::fuzzyCompareT<11>(p1.m_y, p2.m_y)
	           || (std::isnan(p1.m_y) && std::isnan(p2.m_y)))
	       && (    ibeosdk::fuzzyCompareT<11>(p1.m_z, p2.m_z)
	           || (std::isnan(p1.m_z) && std::isnan(p2.m_z)));
}

//======================================================================

inline bool operator!=(const Point3dDouble& p1, const Point3dDouble& p2)
{
	return !operator==(p1, p2);
}

//======================================================================

inline const Point3dDouble operator+(const Point3dDouble& p1, const Point3dDouble& p2)
{
	return Point3dDouble(p1.m_x + p2.m_x, p1.m_y + p2.m_y, p1.m_z + p2.m_z);
}

//======================================================================

inline const Point3dDouble operator-(const Point3dDouble& p1, const Point3dDouble& p2)
{
	return Point3dDouble(p1.m_x - p2.m_x, p1.m_y - p2.m_y, p1.m_z - p2.m_z);
}

//======================================================================

inline double operator*(const Point3dDouble& p1, const Point3dDouble& p2)
{
	return ((p1.m_x * p2.m_x) + (p1.m_y * p2.m_y) + (p1.m_z * p2.m_z));
}

//======================================================================

inline const Point3dDouble operator*(const Point3dDouble& p, const double factor)
{
	return Point3dDouble(p.m_x * factor, p.m_y * factor, p.m_z * factor);
}

//======================================================================

inline const Point3dDouble operator*(const double factor, const Point3dDouble& p)
{
	return Point3dDouble(p.m_x * factor, p.m_y * factor, p.m_z * factor);
}

//======================================================================

inline const Point3dDouble operator-(const Point3dDouble& p)
{
	return Point3dDouble(-p.m_x, -p.m_y, -p.m_z);
}

//======================================================================

inline const Point3dDouble operator/(const Point3dDouble& p, const double divisor)
{
	assert(ibeosdk::fuzzyCompareT<11>(divisor, 0.0) == false);
	return Point3dDouble(p.m_x / divisor, p.m_y / divisor, p.m_z / divisor);
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_Point3dDouble_HPP_SEEN

//======================================================================
