//======================================================================
/*! \file PointBase.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 15, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/PointBase.hpp>

#include <ibeosdk/RotationMatrix3dFloat.hpp>
#include <ibeosdk/io.hpp>

#include <cmath>

//======================================================================

namespace ibeosdk {

//======================================================================

PointProxy::PointProxy(PointBase* const point, PointCloudBase* const cloud)
  : m_point(point),
    m_cloud(cloud)
{}

//==============================================================================

bool PointProxy::hasEpw() const
{
	switch(getType()) {
	case pointType::PointTypeWithEpw:
		return true;
	default:
		return false;
	} // switch
}

//==============================================================================

bool PointProxy::hasFlags() const
{
	switch(getType()) {
	case pointType::PointTypeWithFlags:
		return true;
	default:
		return false;
	} // switch
}

//==============================================================================

pointKind::PointKind PointProxy::getKind() const
{
	return m_cloud->getKind();
}

//==============================================================================

pointType::PointType PointProxy::getType() const
{
	return m_cloud->getType();
}

//==============================================================================

float PointProxy::getEpw() const
{
	return m_point->getEpw();
}

//==============================================================================

void PointProxy::setEpw(const float epw)
{
	m_point->setEpw(epw);
}

//==============================================================================

UINT32 PointProxy::getFlags() const
{
	return m_point->getFlags();
}

//==============================================================================

void PointProxy::setFlags(const UINT32 flags)
{
	m_point->setFlags(flags);
}

//==============================================================================

const ReferencePlane& PointProxy::getReferencePlane() const
{
	return m_cloud->getReferencePlane();
}

//==============================================================================

ReferencePlane& PointProxy::referencePlane()
{
	return m_cloud->referencePlane();
}

//==============================================================================

GpsPoint PointProxy::getReferencePointCoordinates() const
{
	return m_cloud->getReferencePlane().getGpsPoint();
}

//==============================================================================

void PointBase::transformToGlobalCoordinatesP3d(const ReferencePlane& refPlane,
                                                const Point3dFloat& offset,
                                                GpsPoint& globalPoint)
{
	const Point3dFloat transformedOffset = refPlane.getRotationMatrix() * offset;

	double lon, lat;
	PositionWgs84::transformFromTangentialPlane2d(refPlane.getGpsPoint().getLatitudeInRad(),
	                                              refPlane.getGpsPoint().getLongitudeInRad(),
	                                              transformedOffset.getX(),
	                                              transformedOffset.getY(),
	                                              lat, lon);

	globalPoint.setAltitude(refPlane.getGpsPoint().getAltitude());
	globalPoint.setLatitudeInRad(lat);
	globalPoint.setLongitudeInRad(lon);
}

//==============================================================================

void PointBase::transformToGlobalCoordinatesF3d(const ReferencePlane& refPlane,
                                                const Point3dFloat& offset,
                                                GpsPoint& globalPoint)
{
	const Point3dFloat transformedOffset = refPlane.getRotationMatrix() * offset;

	double lon, lat, altitude;
	PositionWgs84::transformFromTangentialPlane3d(refPlane.getGpsPoint().getLatitudeInRad(),
	                                              refPlane.getGpsPoint().getLongitudeInRad(),
	                                              refPlane.getGpsPoint().getAltitude(),
	                                              transformedOffset.getX(),
	                                              transformedOffset.getY(),
	                                              transformedOffset.getZ(),
	                                              lat, lon, altitude);

	globalPoint.setAltitude(float(altitude));
	globalPoint.setLatitudeInRad(lat);
	globalPoint.setLongitudeInRad(lon);
}

//==============================================================================

void PointBase::transformToRelativeCoordinatesP3d(const ReferencePlane& origin,
                                                  const GpsPoint& globalPoint,
                                                  Point3dFloat& offset)
{
	double x, y;

	PositionWgs84::transformToTangentialPlane2d(origin.getGpsPoint().getLatitudeInRad(),
	                                            origin.getGpsPoint().getLongitudeInRad(),
	                                            globalPoint.getLatitudeInRad(),
	                                            globalPoint.getLongitudeInRad(),
	                                            x, y);

	const Point3dFloat p(float(x),
	                     float(y),
	                     std::isnan(globalPoint.getAltitude()) ? origin.getGpsPoint().getAltitude()
	                                                      : globalPoint.getAltitude());
	offset = origin.getRotationMatrix().inverted() * p;
}

//==============================================================================

void PointBase::transformToRelativeCoordinatesF3d(const ReferencePlane& origin,
                                                  const GpsPoint& globalPoint,
                                                  Point3dFloat& offset)
{
	double x, y, z;

	PositionWgs84::transformToTangentialPlane3d(origin.getGpsPoint().getLatitudeInRad(),
	                                            origin.getGpsPoint().getLongitudeInRad(),
	                                            origin.getGpsPoint().getAltitude(),
	                                            globalPoint.getLatitudeInRad(),
	                                            globalPoint.getLongitudeInRad(),
	                                            globalPoint.getAltitude(),
	                                            x, y, z);

	//const Point3dFloat p(float(x), float(y), float(z));
	offset = origin.getRotationMatrix().inverted() *  Point3dFloat(float(x), float(y), float(z));
}

//======================================================================

void PointBase::transformToRelativeCoordinatesP3d(const ReferencePlane& originRefPlane,
                                                  const EcefPoint& originRefPointsEcef,
                                                  const Matrix3dFloat& invTargetRotMatrix,
                                                  const GpsPoint& globalPoint,
                                                  Point3dFloat& offset)
{
	double x, y;

	PositionWgs84::transformToTangentialPlane2d(originRefPointsEcef,
	                                            globalPoint.getLatitudeInRad(),
	                                            globalPoint.getLongitudeInRad(),
	                                            x, y);

	const Point3dFloat p(float(x),
	                    float(y),
	                     std::isnan(globalPoint.getAltitude()) ? originRefPlane.getGpsPoint().getAltitude()
	                                                      : globalPoint.getAltitude());
	offset = invTargetRotMatrix * p;
}

//======================================================================

void PointBase::transformToRelativeCoordinatesF3d(const EcefPoint& originRefPointsEcef,
                                                  const Matrix3dFloat& invTargetRotMatrix,
                                                  const GpsPoint& globalPoint,
                                                  Point3dFloat& offset)
{
	double x, y, z;

	PositionWgs84::transformToTangentialPlaneF3d(originRefPointsEcef,
	                                             globalPoint.getLatitudeInRad(),
	                                             globalPoint.getLongitudeInRad(),
	                                             globalPoint.getAltitude(),
	                                             x, y, z);

	//Point3dFloat p(float(x), float(y), float(z)); konfuses the compiler
	// it is thinging of it as a function prototype.

	offset = invTargetRotMatrix * Point3dFloat(float(x), float(y), float(z));
}

//======================================================================

bool PointBase::operator==(const PointBase& other) const
{
	return ( m_epw    == other.m_epw  )  &&
	       ( m_flags  == other.m_flags);
}

//======================================================================

void PointBase::transformToShiftedReferencePlaneP3d(const ReferencePlane& origin, const ReferencePlane& target,
                                                    const Point3dFloat& originOffset, Point3dFloat& offset)
{
	GpsPoint globalPoint;
	transformToGlobalCoordinatesP3d(origin, originOffset, globalPoint);
	transformToRelativeCoordinatesP3d(target, globalPoint, offset);
}

//==============================================================================

void PointBase::transformToShiftedReferencePlaneP3d(const ReferencePlane& originRefPlane, const ReferencePlane& targetRefPlane,
                                                    const EcefPoint& targetRefPointsEcef, const Matrix3dFloat& invTargetRotMatrix,
                                                    const Point3dFloat& originOffset, Point3dFloat& offsetTarget)
{
	GpsPoint originsGps;
	transformToGlobalCoordinatesP3d(originRefPlane, originOffset, originsGps);
	transformToRelativeCoordinatesP3d(targetRefPlane, targetRefPointsEcef, invTargetRotMatrix, originsGps, offsetTarget);
}

//==============================================================================

void PointBase::transformToShiftedReferencePlaneF3d(const ReferencePlane& originRefPlane,
                                                    const EcefPoint& targetRefPointsEcef,
                                                    const Matrix3dFloat& invTargetRotMatrix,
                                                    const Point3dFloat& originOffset,
                                                    Point3dFloat& offsetTarget)
{
	GpsPoint originsGps;
	transformToGlobalCoordinatesF3d(originRefPlane, originOffset, originsGps);
	transformToRelativeCoordinatesF3d(targetRefPointsEcef, invTargetRotMatrix, originsGps, offsetTarget);
}

//======================================================================

std::streamsize PointBase::getSerializedSizeWithType_static(const pointType::PointType& type)
{
	uint32_t size = 0;

	switch(type) {
	case pointType::PointTypeWithEpw:
		size += 4;
		break;
	case pointType::PointTypeWithFlags:
		size += 4;
		break;
	case pointType::PointTypeWithEpwAndFlags:
		size += 8;
		break;
	default:
		break;
	} // switch

	return std::streamsize(size);
}

//======================================================================

std::streamsize PointBase::getSerializedSizeWithType(const pointType::PointType& type) const
{
	return getSerializedSizeWithType_static(type);
}

//======================================================================

bool PointBase::deserializeWithType(std::istream& is, const pointType::PointType& type)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	switch(type) {
	case pointType::PointTypeWithEpw:
		ibeosdk::readBE(is, m_epw);
		break;
	case pointType::PointTypeWithFlags:
		ibeosdk::readBE(is, m_flags);
		break;
	case pointType::PointTypeWithEpwAndFlags:
		ibeosdk::readBE(is, m_epw);
		ibeosdk::readBE(is, m_flags);
		break;
	default:
		break;
	} // switch

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSizeWithType(type));
}

//======================================================================

bool PointBase::serializeWithType(std::ostream& os, const pointType::PointType& type) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	switch(type) {
	case pointType::PointTypeWithEpw:
		ibeosdk::writeBE(os, m_epw);
		break;
	case pointType::PointTypeWithFlags:
		ibeosdk::writeBE(os, uint32_t(m_flags));
		break;
	case pointType::PointTypeWithEpwAndFlags:
		ibeosdk::writeBE(os, m_epw);
		ibeosdk::writeBE(os, uint32_t(m_flags));
		break;
	default:
		break;
	} // switch

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSizeWithType(type));
}

//======================================================================

std::streamsize PointBase::getSerializedSize() const { return std::streamsize(); }

//======================================================================

bool PointBase::deserialize(std::istream& /*is*/)
{
	return false;
}

//======================================================================

bool PointBase::serialize(std::ostream& /*os*/) const
{
	return false;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
