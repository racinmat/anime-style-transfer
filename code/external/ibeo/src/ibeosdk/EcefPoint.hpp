//======================================================================
/*! \file EcefPoint.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Mar 17, 2016
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_ECEFPOINT_HPP_SEEN
#define IBEOSDK_ECEFPOINT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Point3dDouble.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\brief "Earth centered earth fixed" point (aka ECR "earth centered rotational")
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Mar 17, 2016
 *
 * Class to store the ECEF coordinates of a GPS point.
 * For performance reasons it also holds the sine and cosine
 * of the GPS point's latitude and longitude.
 *///-------------------------------------------------------------------
class EcefPoint {
public: // constructor
	EcefPoint();
	EcefPoint(const double x, const double y, const double z, const double latRad, const double lonRad);

	EcefPoint(const Point3dDouble& xyz, const double latRad, const double lonRad);

	EcefPoint(const Point3dDouble& xyz,
	          const double latSin, const double latCos,
	          const double lonSin, const double lonCos);

	EcefPoint(const double x, const double y, const double z,
	          const double latSin, const double latCos,
	          const double lonSin, const double lonCos);

public: // getter
	const Point3dDouble& getXyz() const { return m_xyz; }
	double getX() const { return m_xyz.getX(); }
	double getY() const { return m_xyz.getY(); }
	double getZ() const { return m_xyz.getZ(); }
	double getLatSin() const { return m_latSin; }
	double getLatCos() const { return m_latCos; }
	double getLonSin() const { return m_lonSin; }
	double getLonCos() const { return m_lonCos; }

public: // setter
	void set(const double x, const double y, const double z, const double latRad, const double lonRad);

	void set(const Point3dDouble& xyz, const double latRad, const double lonRad);

	void set(const Point3dDouble& xyz,
	         const double latSin, const double latCos,
	         const double lonSin, const double lonCos);

	void set(const double x, const double y, const double z,
	         const double latSin, const double latCos,
	         const double lonSin, const double lonCos);

protected:
	Point3dDouble m_xyz;
	double m_latSin;
	double m_latCos;
	double m_lonSin;
	double m_lonCos;
}; // EcefPoint

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_ECEFPOINT_HPP_SEEN

//======================================================================
