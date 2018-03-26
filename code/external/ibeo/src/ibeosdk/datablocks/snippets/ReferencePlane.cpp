//======================================================================
/*! \file ReferencePlane.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 15, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/ReferencePlane.hpp>
#include <ibeosdk/datablocks/snippets/PointBase.hpp>

//======================================================================

namespace ibeosdk {

//==============================================================================

ReferencePlane::ReferencePlane(const VehicleStateBasicEcu2808& vsb)
  : m_rotationMatrixIsValid(false),
    m_rotationMatrix()
{
	const GpsPoint vsRef(vsb.getLongitude(), vsb.getLatitude(), vsb.getAltitude());
	GpsPoint vsGlobal;
	const Point3dFloat vsOffset = Point3dFloat((float)vsb.getXPosition(), (float)vsb.getYPosition(), vsb.getZPosition());
	PointBase::transformToGlobalCoordinatesF3d(ReferencePlane(vsRef), vsOffset, vsGlobal );

	setGpsPoint(vsGlobal);
	// set angle to 0 if the corresponding vsb entry is NaN
	(std::isnan(vsb.getCourseAngle()))        ? setYaw(.0)   : setYaw(vsb.getCourseAngle());
	(std::isnan(vsb.getVehiclePitchAngle()))  ? setPitch(.0) : setPitch(vsb.getVehiclePitchAngle());
	(std::isnan(vsb.getVehicleRollAngle()))   ? setRoll(.0)  : setRoll(vsb.getVehicleRollAngle());
}

//==============================================================================

bool ReferencePlane::operator==(const ReferencePlane& other) const
{
	return m_gpsPoint == other.m_gpsPoint
	    && m_yaw == other.m_yaw
	    && m_pitch == other.m_pitch
	    && m_roll == other.m_roll;
}

//==============================================================================

bool ReferencePlane::operator!=(const ReferencePlane& other) const
{
	return !(*this == other);
}

//======================================================================

std::streamsize ReferencePlane::getSerializedSize() const
{
	return std::streamsize(3* sizeof(float)) + m_gpsPoint.getSerializedSize();
}

//======================================================================

bool ReferencePlane::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	m_gpsPoint.deserialize(is);
	ibeosdk::readBE(is, m_yaw);
	ibeosdk::readBE(is, m_pitch);
	ibeosdk::readBE(is, m_roll);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ReferencePlane::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	m_gpsPoint.serialize(os);
	ibeosdk::writeBE(os, m_yaw);
	ibeosdk::writeBE(os, m_pitch);
	ibeosdk::writeBE(os, m_roll);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
