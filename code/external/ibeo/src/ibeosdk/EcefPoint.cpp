//======================================================================
/*! \file EcefPoint.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Mar 17, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/EcefPoint.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

EcefPoint::EcefPoint()
: m_xyz(0.0, 0.0, 0.0),
  m_latSin(std::sin(0.0)),
  m_latCos(std::cos(0.0)),
  m_lonSin(std::sin(0.0)),
  m_lonCos(std::cos(0.0))
{}

//======================================================================

EcefPoint::EcefPoint(const double x, const double y, const double z, const double latRad, const double lonRad)
  : m_xyz(x, y, z),
    m_latSin(std::sin(latRad)),
    m_latCos(std::cos(latRad)),
    m_lonSin(std::sin(lonRad)),
    m_lonCos(std::cos(lonRad))
{}

//======================================================================

EcefPoint::EcefPoint(const Point3dDouble& xyz, const double latRad, const double lonRad)
  : m_xyz(xyz),
    m_latSin(std::sin(latRad)),
    m_latCos(std::cos(latRad)),
    m_lonSin(std::sin(lonRad)),
    m_lonCos(std::cos(lonRad))
{}

//======================================================================

EcefPoint::EcefPoint(const Point3dDouble& xyz,
                     const double latSin, const double latCos,
                     const double lonSin, const double lonCos)
  : m_xyz(xyz),
    m_latSin(latSin),
    m_latCos(latCos),
    m_lonSin(lonSin),
    m_lonCos(lonCos)
{}

//======================================================================

EcefPoint::EcefPoint(const double x, const double y, const double z,
                     const double latSin, const double latCos,
                     const double lonSin, const double lonCos)
  : m_xyz(x, y, z),
    m_latSin(latSin),
    m_latCos(latCos),
    m_lonSin(lonSin),
    m_lonCos(lonCos)
{}

//======================================================================

void EcefPoint::set(const double x, const double y, const double z, const double latRad, const double lonRad)
{
	m_xyz.setXYZ(x, y, z);
	m_latSin = std::sin(latRad);
	m_latCos = std::cos(latRad);
	m_lonSin = std::sin(lonRad);
	m_lonCos = std::cos(lonRad);
}

//======================================================================

void EcefPoint::set(const Point3dDouble& xyz, const double latRad, const double lonRad)
{
	m_xyz = xyz;
	m_latSin = std::sin(latRad);
	m_latCos = std::cos(latRad);
	m_lonSin = std::sin(lonRad);
	m_lonCos = std::cos(lonRad);
}

//======================================================================

void EcefPoint::set(const Point3dDouble& xyz,
                    const double latSin, const double latCos,
                    const double lonSin, const double lonCos)
{
	m_xyz = xyz;
	m_latSin = latSin;
	m_latCos = latCos;
	m_lonSin = lonSin;
	m_lonCos = lonCos;
}

//======================================================================

void EcefPoint::set(const double x, const double y, const double z,
                    const double latSin, const double latCos,
                    const double lonSin, const double lonCos)
{
	m_xyz.setXYZ(x, y, z);
	m_latSin = latSin;
	m_latCos = latCos;
	m_lonSin = lonSin;
	m_lonCos = lonCos;
}

//======================================================================

}// namespace ibeosdk

//======================================================================
