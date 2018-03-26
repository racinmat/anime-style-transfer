//======================================================================
/*! \file Point3dFloat.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 14, 2016
 *///-------------------------------------------------------------------

//======================================================================

#ifndef IBEOSDK_POINT3DFLOAT_HPP_SEEN
#define IBEOSDK_POINT3DFLOAT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <ibeosdk/Math.hpp>

#include <cassert>

//======================================================================

namespace ibeosdk {

//======================================================================

class Point3dFloat : public Snippet {
public:
	static std::streamsize getSerializedSize_static();

public:
	Point3dFloat();
	Point3dFloat(const float x, const float y, const float z);
	Point3dFloat(const Point3dFloat& other);
	Point3dFloat& operator=(const Point3dFloat& other);
	virtual ~Point3dFloat();

public:
	Point3dFloat& operator+=(const Point3dFloat& other);
	Point3dFloat& operator*=(const float factor);
	Point3dFloat& operator-=(const Point3dFloat& point);
	Point3dFloat& operator/=(const float divisor);

public:
	float getX() const { return this->m_x; }
	float getY() const { return this->m_y; }
	float getZ() const { return this->m_z; }
	float& getX() { return this->m_x; }
	float& getY() { return this->m_y; }
	float& getZ() { return this->m_z; }

	void setX(const float val) { this->m_x = val; }
	void setY(const float val) { this->m_y = val; }
	void setZ(const float val) { this->m_z = val; }
	void setXYZ(const float x, const float y, const float z) { this->m_x = x; this->m_y = y; this->m_z = z; }

	float getAngleAroundX() const; //!< Returns the rotation angle around x (in the z-y-plane)
	float getAngleAroundY() const; //!< Returns the rotation angle around y (in the z-x-plane)
	float getAngleAroundZ() const; //!< Returns the rotation angle around z (in the x-y-plane)

	void rotateAroundX(const float rollAngle);  //!< Rotate the point around the X-axis ("Roll angle")
	void rotateAroundY(const float pitchAngle); //!< Rotate the point around the Y-axis ("Pitch angle")
	void rotateAroundZ(const float dYawAngle);  //!< Rotate the point around the Z-axis ("Yaw angle")

	bool isZero() const;
	float length() const;
	float distToOrigin() const { return length(); }

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

//protected:
	friend inline bool operator==(const Point3dFloat&, const Point3dFloat&);
	friend inline bool operator!=(const Point3dFloat&, const Point3dFloat&);
	friend inline const Point3dFloat operator+(const Point3dFloat&, const Point3dFloat&);
	friend inline const Point3dFloat operator-(const Point3dFloat&, const Point3dFloat&);
	friend inline float operator*(const Point3dFloat&, const Point3dFloat&);
	friend inline const Point3dFloat operator*(const float, const Point3dFloat&);
	friend inline const Point3dFloat operator*(const Point3dFloat&, float);
	friend inline const Point3dFloat operator-(const Point3dFloat&);
	friend inline const Point3dFloat operator/(const Point3dFloat&, float);

private:
	float m_x;
	float m_y;
	float m_z;
}; //Point3dFloat

//======================================================================
//======================================================================

inline bool operator==(const Point3dFloat& p1, const Point3dFloat& p2)
{
	return    (    ibeosdk::fuzzyCompareT<6>(p1.m_x, p2.m_x)
	           || (std::isnan(p1.m_x) && std::isnan(p2.m_x)))
	       && (    ibeosdk::fuzzyCompareT<6>(p1.m_y, p2.m_y)
	           || (std::isnan(p1.m_y) && std::isnan(p2.m_y)))
	       && (    ibeosdk::fuzzyCompareT<6>(p1.m_z, p2.m_z)
	           || (std::isnan(p1.m_z) && std::isnan(p2.m_z)));
}

//======================================================================

inline bool operator!=(const Point3dFloat& p1, const Point3dFloat& p2)
{
	return !operator==(p1, p2);
}

//======================================================================

inline const Point3dFloat operator+(const Point3dFloat& p1, const Point3dFloat& p2)
{
	return Point3dFloat(p1.m_x + p2.m_x, p1.m_y + p2.m_y, p1.m_z + p2.m_z);
}

//======================================================================

inline const Point3dFloat operator-(const Point3dFloat& p1, const Point3dFloat& p2)
{
	return Point3dFloat(p1.m_x - p2.m_x, p1.m_y - p2.m_y, p1.m_z - p2.m_z);
}

//======================================================================

inline float operator*(const Point3dFloat& p1, const Point3dFloat& p2)
{
	return ((p1.m_x * p2.m_x) + (p1.m_y * p2.m_y) + (p1.m_z * p2.m_z));
}

//======================================================================

inline const Point3dFloat operator*(const Point3dFloat& p, const float factor)
{
	return Point3dFloat(p.m_x * factor, p.m_y * factor, p.m_z * factor);
}

//======================================================================

inline const Point3dFloat operator*(const float factor, const Point3dFloat& p)
{
	return Point3dFloat(p.m_x * factor, p.m_y * factor, p.m_z * factor);
}

//======================================================================

inline const Point3dFloat operator-(const Point3dFloat& p)
{
	return Point3dFloat(-p.m_x, -p.m_y, -p.m_z);
}

//======================================================================

inline const Point3dFloat operator/(const Point3dFloat& p, const float divisor)
{
	assert(ibeosdk::fuzzyCompareT<6>(divisor, 0.0F) == false);
	return Point3dFloat(p.m_x / divisor, p.m_y / divisor, p.m_z / divisor);
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_POINT3DFLOAT_HPP_SEEN

//======================================================================
