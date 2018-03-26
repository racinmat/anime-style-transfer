//======================================================================
/*! \file GlobalPoint.cpp
 *\verbatim
 * ------------------------------------------
 *  (C) 2016 Ibeo Automotive Systems GmbH, Hamburg, Germany
 * ------------------------------------------
 *
 *  Created on: Mar 16, 2016
 *          by: Kristian Bischoff
 *\endverbatim
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/GlobalPoint.hpp>
#include <ibeosdk/datablocks/PointCloudGlobal7500.hpp>

#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

GlobalPointProxy::GlobalPointProxy(GlobalPoint* const point, PointCloudGlobal7500* const cloud)
         : PointProxy(point, cloud)
{}

//======================================================================

GpsPoint GlobalPointProxy::getGlobalCoordinates() const
{
	return static_cast<GlobalPoint*>(m_point)->getGpsPoint();
}

//======================================================================

Point3dFloat GlobalPointProxy::getOffsetFromReferencePoint() const
{
	if (m_cloud->referencePlane().getGpsPoint() == static_cast<GlobalPoint*>(m_point)->getGpsPoint())
		return Point3dFloat();

	else
	{
		Point3dFloat ret;
		PointBase::transformToRelativeCoordinatesF3d(m_cloud->getReferencePlane(), static_cast<GlobalPoint*>(m_point)->getGpsPoint(), ret);
		return ret;
	}
}

//======================================================================

//======================================================================
//======================================================================

bool GlobalPoint::operator==(const GlobalPoint& other) const
{
	return PointBase::operator==(other) && ( m_gpsPoint == other.m_gpsPoint );
}

//======================================================================

std::streamsize GlobalPoint::getSerializedSize_static()
{
	return GpsPoint::getSerializedSize_static();
}

//======================================================================

std::streamsize GlobalPoint::getSerializedSize() const
{
	return getSerializedSize_static();
}

//======================================================================

bool GlobalPoint::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	m_gpsPoint.deserialize(is);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool GlobalPoint::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	m_gpsPoint.serialize(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} //namespace ibeosdk

//======================================================================
