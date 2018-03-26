//======================================================================
/*! \file PositionWgs84.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 24, 2014
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/PositionWgs84.hpp>
#include <ibeosdk/EcefPoint.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

template<>
inline
void readBE<PositionWgs84::SourceType>(std::istream& is, PositionWgs84::SourceType& value)
{
	uint16_t tmp;
	readBE(is, tmp);
	value = PositionWgs84::SourceType(tmp);
}

//======================================================================

template<>
inline
void writeBE<PositionWgs84::SourceType>(std::ostream& os, const PositionWgs84::SourceType& value)
{
	const uint16_t tmp = value;
	writeBE(os, tmp);
}

//======================================================================

const double PositionWgs84::a = 6378137.0;           // SemiMajorAxis
const double PositionWgs84::b = 6356752.31424518;    // SemiMinorAxis
//const double PositionWgs84::f = 1.0/298.257223563; // ellipsoid flattening
const double PositionWgs84::ea2 = 1.0 - (b*b)/(a*a); // first eccentricity
const double PositionWgs84::eb2 = (a*a)/(b*b) - 1.0; // second eccentricity

//======================================================================

PositionWgs84::PositionWgs84()
  : m_usSinceStartup(0),
    m_timestamp(),
    m_deviceId(0),
    m_latitude(NaN),
    m_latitudeSigma(NaN),
    m_longitude(NaN),
    m_longitudeSigma(NaN),
    m_altitudeMSL(NaN),
    m_altitudeMSLSigma(NaN),

    m_courseAngle(NaN),
    m_courseAngleSigma(NaN),

    m_yawAngle(NaN),
    m_yawAngleSigma(NaN),
    m_pitchAngle(NaN),
    m_pitchAngleSigma(NaN),
    m_rollAngle(NaN),
    m_rollAngleSigma(NaN),

    m_source(SourceType_Unknown)
{}


//======================================================================

PositionWgs84::~PositionWgs84() {}

//======================================================================

void PositionWgs84::transformToTangentialPlane2d(const PositionWgs84& origin, double& pdX, double& pdY) const
{
	const double dOriginLat = origin.getLatitudeInRad();   // (GPS_PI / 180.0);
	const double dOriginLong = origin.getLongitudeInRad(); // (GPS_PI / 180.0);

	this->transformToTangentialPlane2d(dOriginLat, dOriginLong, pdX, pdY);
}

//======================================================================

void PositionWgs84::transformToTangentialPlane2d(const double dOriginLat, const double dOriginLong,
                                                 double& pdX, double& pdY) const
{
	PositionWgs84::transformToTangentialPlane2d(dOriginLat, dOriginLong,
	                                            this->getLatitudeInRad(), this->getLongitudeInRad(), pdX, pdY);
}

//======================================================================

void PositionWgs84::transformToTangentialPlane2d(const double originsLatitudeInRad, const double originsLongitudeInRad,
                                                 const double pointsLatitudeInRad, const double pointsLongitudeInRad,
                                                 double& pointsX, double& pointsY)
{
	const double N = a / sqrt(1.0 - ea2*sin(pointsLatitudeInRad)*sin(pointsLatitudeInRad));
	const double h = 0.0;

	const double pointsEcefX = (N + h)*cos(pointsLatitudeInRad) * cos(pointsLongitudeInRad);
	const double pointsEcefY = (N + h)*cos(pointsLatitudeInRad) * sin(pointsLongitudeInRad);
	const double pointsEcefZ = ((1-ea2)*N + h)*sin(pointsLatitudeInRad); // 1-ea2 = (b*b)/(a*a)


	const double N0 = a / sqrt(1.0 - ea2*sin(originsLatitudeInRad)*sin(originsLatitudeInRad));

	const double pointsEcefX0 = (N0 + h)*cos(originsLatitudeInRad) * cos(originsLongitudeInRad);
	const double pointsEcefY0 = (N0 + h)*cos(originsLatitudeInRad) * sin(originsLongitudeInRad);
	const double pointsEcefZ0 = ((1-ea2)*N0 + h)*sin(originsLatitudeInRad); // 1-ea2 = (b*b)/(a*a)

	// see "Geographic Coordiante Transformation and
	//      Landmark Navigation" (T.Weiss)
	pointsX = - sin(originsLatitudeInRad) * cos(originsLongitudeInRad) * (pointsEcefX - pointsEcefX0)
	          - sin(originsLatitudeInRad) * sin(originsLongitudeInRad) * (pointsEcefY - pointsEcefY0)
	          + cos(originsLatitudeInRad) * (pointsEcefZ - pointsEcefZ0);

	pointsY =   sin(originsLongitudeInRad) * (pointsEcefX - pointsEcefX0)
	          - cos(originsLongitudeInRad) * (pointsEcefY - pointsEcefY0);

	//const double z=   cos(originsLatitudeInRad) * cos(originsLongitudeInRad) * (pointsEcefX - pointsEcefX0)
	//                + cos(originsLatitudeInRad) * sin(originsLongitudeInRad) * (pointsEcefY - pointsEcefY0)
	//                + sin(originsLatitudeInRad) * (pointsEcefZ - pointsEcefZ0);
}

//======================================================================

void PositionWgs84::transformToTangentialPlane3d(const double originsLatitudeInRad,
                                                 const double originsLongitudeInRad,
                                                 const double originsAltitudeInMeter,
                                                 const double pointsLatitudeInRad,
                                                 const double pointsLongitudeInRad,
                                                 const double pointsAltitudeInMeter,
                                                 double& pointsX, double& pointsY, double& pointsZ)
{
	const double N = a / sqrt(1.0 - ea2*sin(pointsLatitudeInRad)*sin(pointsLatitudeInRad));

	const double pointsEcefX = (N + pointsAltitudeInMeter)*cos(pointsLatitudeInRad) * cos(pointsLongitudeInRad);
	const double pointsEcefY = (N + pointsAltitudeInMeter)*cos(pointsLatitudeInRad) * sin(pointsLongitudeInRad);
	const double pointsEcefZ = ((1-ea2)*N + pointsAltitudeInMeter)*sin(pointsLatitudeInRad); // 1-ea2 = (b*b)/(a*a)


	const double N0 = a / sqrt(1.0 - ea2*sin(originsLatitudeInRad)*sin(originsLatitudeInRad));

	const double pointsEcefX0 = (N0 + originsAltitudeInMeter)*cos(originsLatitudeInRad) * cos(originsLongitudeInRad);
	const double pointsEcefY0 = (N0 + originsAltitudeInMeter)*cos(originsLatitudeInRad) * sin(originsLongitudeInRad);
	const double pointsEcefZ0 = ((1-ea2)*N0 + originsAltitudeInMeter)*sin(originsLatitudeInRad); // 1-ea2 = (b*b)/(a*a)

	// see "Geographic Coordiante Transformation and
	//      Landmark Navigation" (T.Weiss)
	pointsX = - sin(originsLatitudeInRad) * cos(originsLongitudeInRad) * (pointsEcefX - pointsEcefX0)
	          - sin(originsLatitudeInRad) * sin(originsLongitudeInRad) * (pointsEcefY - pointsEcefY0)
	          + cos(originsLatitudeInRad) * (pointsEcefZ - pointsEcefZ0);

	pointsY =   sin(originsLongitudeInRad) * (pointsEcefX - pointsEcefX0)
	          - cos(originsLongitudeInRad) * (pointsEcefY - pointsEcefY0);

	pointsZ =   cos(originsLatitudeInRad) * cos(originsLongitudeInRad) * (pointsEcefX - pointsEcefX0)
	          + cos(originsLatitudeInRad) * sin(originsLongitudeInRad) * (pointsEcefY - pointsEcefY0)
	          + sin(originsLatitudeInRad) * (pointsEcefZ - pointsEcefZ0);
}
//======================================================================

void PositionWgs84::transformToTangentialPlane2d(const EcefPoint& originRefPointsEcef,
                                                 const double pointsLatitudeInRad, const double pointsLongitudeInRad,
                                                 double& pointsX, double& pointsY)
{
	const double N = a / sqrt(1.0 - ea2*sin(pointsLatitudeInRad)*sin(pointsLatitudeInRad));
	const double h = 0.0;

	const double pointsEcefX = (N + h)*cos(pointsLatitudeInRad) * cos(pointsLongitudeInRad);
	const double pointsEcefY = (N + h)*cos(pointsLatitudeInRad) * sin(pointsLongitudeInRad);
	const double pointsEcefZ = ((1-ea2)*N + h)*sin(pointsLatitudeInRad); // 1-ea2 = (b*b)/(a*a)

	// see "Geographic Coordiante Transformation and
	//      Landmark Navigation" (T.Weiss)
	pointsX = - originRefPointsEcef.getLatSin() * originRefPointsEcef.getLonCos() * (pointsEcefX - originRefPointsEcef.getX())
	          - originRefPointsEcef.getLatSin() * originRefPointsEcef.getLonSin() * (pointsEcefY - originRefPointsEcef.getY())
	          + originRefPointsEcef.getLatCos() * (pointsEcefZ - originRefPointsEcef.getZ());

	pointsY =   originRefPointsEcef.getLonSin() * (pointsEcefX - originRefPointsEcef.getX())
	          - originRefPointsEcef.getLonCos() * (pointsEcefY - originRefPointsEcef.getY());

	//const double z=   cos(originsLatitudeInRad) * cos(originsLongitudeInRad) * (pointsEcefX - originRefPointsEcef.getX())
	//                + cos(originsLatitudeInRad) * sin(originsLongitudeInRad) * (pointsEcefY - originRefPointsEcef.getY())
	//                + sin(originsLatitudeInRad) * (pointsEcefZ - originRefPointsEcef.getZ());
}

//======================================================================

void PositionWgs84::transformToTangentialPlaneF3d(const EcefPoint& originRefPointsEcef,
                                                  const double pointsLatitudeInRad,
                                                  const double pointsLongitudeInRad,
                                                  const double pointsAltitudeInMeter,
                                                  double& pointsX, double& pointsY, double& pointsZ)
{
	const double N = a / sqrt(1.0 - ea2*sin(pointsLatitudeInRad)*sin(pointsLatitudeInRad));

	const double pointsEcefX = (N + pointsAltitudeInMeter)*cos(pointsLatitudeInRad) * cos(pointsLongitudeInRad);
	const double pointsEcefY = (N + pointsAltitudeInMeter)*cos(pointsLatitudeInRad) * sin(pointsLongitudeInRad);
	const double pointsEcefZ = ((1-ea2)*N + pointsAltitudeInMeter)*sin(pointsLatitudeInRad); // 1-ea2 = (b*b)/(a*a)


	// see "Geographic Coordiante Transformation and
	//      Landmark Navigation" (T.Weiss)
	pointsX = - originRefPointsEcef.getLatSin() * originRefPointsEcef.getLonCos() * (pointsEcefX - originRefPointsEcef.getX())
	         - originRefPointsEcef.getLatSin() * originRefPointsEcef.getLonSin() * (pointsEcefY - originRefPointsEcef.getY())
	         + originRefPointsEcef.getLatCos() * (pointsEcefZ - originRefPointsEcef.getZ());

	pointsY =   originRefPointsEcef.getLonSin() * (pointsEcefX - originRefPointsEcef.getX())
	          - originRefPointsEcef.getLonCos() * (pointsEcefY - originRefPointsEcef.getY());

	pointsZ =   originRefPointsEcef.getLatCos() * originRefPointsEcef.getLonCos() * (pointsEcefX - originRefPointsEcef.getX())
	          + originRefPointsEcef.getLatCos() * originRefPointsEcef.getLonSin() * (pointsEcefY - originRefPointsEcef.getY())
	          + originRefPointsEcef.getLatSin() * (pointsEcefZ - originRefPointsEcef.getZ());

}
//======================================================================

void PositionWgs84::transformFromTangentialPlane2d(const double x, const double y, const PositionWgs84& origin)
{
	double lon, lat;
	PositionWgs84::transformFromTangentialPlane2d(origin.getLatitudeInRad(), origin.getLongitudeInRad(), x, y, lat, lon);
//	resetToDefault();
	setLatitudeInRad(lat);
	setLongitudeInRad(lon);
}

//======================================================================
/*!Inverse function of transformToTangentialPlane
 *
 * Generates this point from x-y-coordinates on a tangential plane defined by the origin.
 * Note that the x-y-coordinates should be somewhat close to the origin to get accurate results.
 *
 * In a first step, the origin given in LLA
 * coordinates (latitude/longitude/altitude) will be transformed into
 * ECEF (Earth centered each fixed) coordinates.
 *
 * In the second step the point given as the x/y offsets with respect to the origin will be
 * transformed in ECEF coordinates. (T.Weiss)
 *
 * In the third step the point now given in ECEF coordinates will be transformed into LLA
 * coordinates. Latitude and longitude will be returned.
 *
 * \note The algorithm that is used is described in https://microem.ru/files/2012/08/GPS.G1-X-00006.pdf
 * \note A correction is used from Geographic Coordinate Transformation and Landmark Navigation (T.Weiss).
 *///-------------------------------------------------------------------
void PositionWgs84::transformFromTangentialPlane2d(const double originsLatitudeInRad, const double originsLongitudeInRad,
                                                   const double pointsX, const double pointsY,
                                                   double& pointsLatInRad, double& pointsLonInRad)
{
	// LLA origin to ECEF
	const double N = a / sqrt(1.0 - ea2*sin(originsLatitudeInRad)*sin(originsLatitudeInRad));
	const double h = 0.0;
	const double pointsEcefX0 = (N+h)*cos(originsLatitudeInRad)*cos(originsLongitudeInRad);
	const double pointsEcefY0 = (N+h)*cos(originsLatitudeInRad)*sin(originsLongitudeInRad);
	const double pointsEcefXZ0 = ((b*b)/(a*a) * N + h) * sin(originsLatitudeInRad);

	// points x/y-offset with respect to origin will be transformed into ECEF.
	// see "Geographic Coordinate Transformation and
	//      Landmark Navigation" (T.Weiss)
	const double pointsEcefX = -cos(originsLongitudeInRad) * sin(originsLatitudeInRad) * pointsX
	                          + sin(originsLongitudeInRad) * pointsY
	                       /* + cos(dOriginLong) * cos(originsLattitudeInRad) * 0.0 */
	                          + pointsEcefX0;
	const double pointsEcefY = -sin(originsLongitudeInRad) * sin(originsLatitudeInRad) * pointsX
	                          - cos(originsLongitudeInRad) * pointsY
	                       /* + sin(dOriginLong) * cos(originsLattitudeInRad) * 0.0 */
	                          + pointsEcefY0;
	const double pointsEcefZ =  cos(originsLatitudeInRad) * pointsX
	                       /* + sin(dOriginLat) * 0.0 */
	                          + pointsEcefXZ0;

	// ECEF to LLA
	const double p = sqrt(pointsEcefX*pointsEcefX + pointsEcefY*pointsEcefY);
	const double theta = atan2(pointsEcefZ*a, (p*b));

	pointsLatInRad = atan2(pointsEcefZ + eb2*b*sin(theta) * sin(theta) * sin(theta), (p - ea2*a*cos(theta)*cos(theta)*cos(theta)));
	//const double h = p/cos(phi) - N; // altitude of the result
	pointsLonInRad = atan2(pointsEcefY, pointsEcefX);
}

//======================================================================

void PositionWgs84::transformFromTangentialPlane3d(const double originsLatitudeInRad, const double originsLongitudeInRad,
                                                   const double originsAltitutde,
                                                   const double pointsX, const double pointsY, const double pointsZ,
                                                   double& pointsLatInRad, double& pointsLonInRad,
                                                   double& pointsAltInMeter)
{
	// LLA origin to ECEF
	const double N = a / sqrt(1.0 - ea2*sin(originsLatitudeInRad)*sin(originsLatitudeInRad));
	const double h = originsAltitutde;
	const double pointsEcefX0 = (N+h)*cos(originsLatitudeInRad)*cos(originsLongitudeInRad);
	const double pointsEcefY0 = (N+h)*cos(originsLatitudeInRad)*sin(originsLongitudeInRad);
	const double pointsEcefZ0 = ((b*b)/(a*a) * N + h) * sin(originsLatitudeInRad);

	// points x/y-offset with respect to origin will be transformed into ECEF
	// and the origin's ECEF will be added.
	// see "Geographic Coordinate Transformation and
	//      Landmark Navigation" (T.Weiss)
	const double pointsEcefX = -cos(originsLongitudeInRad) * sin(originsLatitudeInRad) * pointsX
	                          + sin(originsLongitudeInRad) * pointsY
	                          + cos(originsLongitudeInRad) * cos(originsLatitudeInRad) * pointsZ
	                          + pointsEcefX0;
	const double pointsEcefY = -sin(originsLongitudeInRad) * sin(originsLatitudeInRad) * pointsX
	                          - cos(originsLongitudeInRad) * pointsY
	                          + sin(originsLongitudeInRad) * cos(originsLatitudeInRad) * pointsZ
	                          + pointsEcefY0;
	const double pointsEcefZ =  cos(originsLatitudeInRad) * pointsX
	                          + sin(originsLatitudeInRad) * pointsZ
	                          + pointsEcefZ0;

	// ECEF to LLA
	const double p = sqrt(pointsEcefX*pointsEcefX + pointsEcefY*pointsEcefY);
	const double theta = atan2(pointsEcefZ*a, (p*b));

	pointsLatInRad = atan2(pointsEcefZ + eb2*b*sin(theta) * sin(theta) * sin(theta), (p - ea2*a*cos(theta)*cos(theta)*cos(theta)));
	pointsLonInRad = atan2(pointsEcefY, pointsEcefX);
	pointsAltInMeter = p/cos(pointsLatInRad) - N;
}

//======================================================================

void PositionWgs84::transformToTangentialPlaneWithHeading2d(const PositionWgs84& origin, double& x, double& y) const
{
	double rotatedX;
	double rotatedY;
	transformToTangentialPlane2d(origin, rotatedX, rotatedY);
	x = cos(-origin.getCourseAngleInRad()) * rotatedX - sin(-origin.getCourseAngleInRad()) * rotatedY;
	y = sin(-origin.getCourseAngleInRad()) * rotatedX + cos(-origin.getCourseAngleInRad()) * rotatedY;
}

//======================================================================

void PositionWgs84::transformFromTangentialPlaneWithHeading2d(const double x, const double y, const PositionWgs84& origin)
{
	const double rotatedX = cos(origin.getCourseAngleInRad()) * x - sin(origin.getCourseAngleInRad()) * y;
	const double rotatedY = sin(origin.getCourseAngleInRad()) * x + cos(origin.getCourseAngleInRad()) * y;

	transformFromTangentialPlane2d(rotatedX, rotatedY, origin);
	setCourseAngleInRad(origin.getCourseAngleInRad());
}

//======================================================================

void PositionWgs84::llaToEcef(const double pointsLatitudeInRad, const double pointsLongitudeInRad,
                              EcefPoint& pointsEcef)
{
	const double N = a / sqrt(1.0 - ea2*sin(pointsLatitudeInRad)*sin(pointsLatitudeInRad));
	const double h = 0.0;

	pointsEcef = EcefPoint((N + h)*cos(pointsLatitudeInRad) * cos(pointsLongitudeInRad),
	                       (N + h)*cos(pointsLatitudeInRad) * sin(pointsLongitudeInRad),
	                       ((1-ea2)*N + h)*sin(pointsLatitudeInRad),
	                       sin(pointsLatitudeInRad),
	                       cos(pointsLatitudeInRad),
	                       sin(pointsLongitudeInRad),
	                       cos(pointsLongitudeInRad));
}

//======================================================================

std::streamsize PositionWgs84::getSerializedSize_static()
{
	return std::streamsize(sizeof(uint32_t))
	     + std::streamsize(sizeof(uint64_t))
	     + std::streamsize(sizeof(uint8_t))
	     + 14 * std::streamsize(sizeof(double))
	     + std::streamsize(sizeof(uint16_t));
}

//======================================================================

std::streamsize PositionWgs84::getSerializedSize() const
{
	return getSerializedSize_static();
}

//======================================================================

bool PositionWgs84::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, this->m_usSinceStartup);
	ibeosdk::readBE(is, this->m_timestamp);

	ibeosdk::readBE(is, this->m_deviceId);

	ibeosdk::readBE(is, this->m_latitude);
	ibeosdk::readBE(is, this->m_longitude);
	ibeosdk::readBE(is, this->m_altitudeMSL);
	ibeosdk::readBE(is, this->m_latitudeSigma);
	ibeosdk::readBE(is, this->m_longitudeSigma);
	ibeosdk::readBE(is, this->m_altitudeMSLSigma);

	ibeosdk::readBE(is, this->m_courseAngle);
	ibeosdk::readBE(is, this->m_courseAngleSigma);

	ibeosdk::readBE(is, this->m_yawAngle);
	ibeosdk::readBE(is, this->m_yawAngleSigma);
	ibeosdk::readBE(is, this->m_pitchAngle);
	ibeosdk::readBE(is, this->m_pitchAngleSigma);
	ibeosdk::readBE(is, this->m_rollAngle);
	ibeosdk::readBE(is, this->m_rollAngleSigma);

	ibeosdk::readBE(is, this->m_source);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool PositionWgs84::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, this->m_usSinceStartup);
	ibeosdk::writeBE(os, this->m_timestamp);

	ibeosdk::writeBE(os, this->m_deviceId);

	ibeosdk::writeBE(os, this->m_latitude);
	ibeosdk::writeBE(os, this->m_longitude);
	ibeosdk::writeBE(os, this->m_altitudeMSL);
	ibeosdk::writeBE(os, this->m_latitudeSigma);
	ibeosdk::writeBE(os, this->m_longitudeSigma);
	ibeosdk::writeBE(os, this->m_altitudeMSLSigma);

	ibeosdk::writeBE(os, this->m_courseAngle);
	ibeosdk::writeBE(os, this->m_courseAngleSigma);

	ibeosdk::writeBE(os, this->m_yawAngle);
	ibeosdk::writeBE(os, this->m_yawAngleSigma);
	ibeosdk::writeBE(os, this->m_pitchAngle);
	ibeosdk::writeBE(os, this->m_pitchAngleSigma);
	ibeosdk::writeBE(os, this->m_rollAngle);
	ibeosdk::writeBE(os, this->m_rollAngleSigma);

	ibeosdk::writeBE(os, this->m_source);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
