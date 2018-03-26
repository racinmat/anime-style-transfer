//======================================================================
/*! \file BsonT_Point.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 21, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/datamodel/BsonT.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

const std::string BsonT<PointProxy>::bsonFtReflectivity = "Reflectivity";
const std::string BsonT<PointProxy>::bsonFtFlags = "Flags";
const std::string BsonT<PointProxy>::bsonFtRefPlaneYaw = "RefPlaneYaw";
const std::string BsonT<PointProxy>::bsonFtRefPlanePitch = "RefPlanePitch";
const std::string BsonT<PointProxy>::bsonFtRefPlaneRoll = "RefPlaneRoll";

//======================================================================

void BsonT<PointProxy>::createGpsPoint(GpsPoint& gpsPoint, const mongo::BSONObj& bsonObj)
{
	gpsPoint.setAltitude(float(bsonObj.getField(bsonFtAlt).Double()));
	gpsPoint.setLatitudeInDeg(bsonObj.getField(bsonFtLat).Double());
	gpsPoint.setLongitudeInDeg(bsonObj.getField(bsonFtLon).Double());
}

//======================================================================

void BsonT<PointProxy>::createReferencePlane(ReferencePlane& refPlane, const mongo::BSONObj& bsonObj)
{
	if (bsonObj.hasField(bsonFtRefPlaneYaw))
		refPlane.setYaw(float(bsonObj.getField(bsonFtRefPlaneYaw).Double()));

	if (bsonObj.hasField(bsonFtRefPlanePitch))
		refPlane.setPitch(float(bsonObj.getField(bsonFtRefPlanePitch).Double()));

	if (bsonObj.hasField(bsonFtRefPlaneRoll))
		refPlane.setRoll(float(bsonObj.getField(bsonFtRefPlaneRoll).Double()));
}

//======================================================================

void BsonT<PointProxy>::createPoint3dFloat(Point3dFloat& point, const mongo::BSONObj& bsonObj)
{
	const float x = float(bsonObj.getField(bsonFtX).Double());
	const float y = float(bsonObj.getField(bsonFtY).Double());
	const float z = float(bsonObj.getField(bsonFtZ).Double());

	point.setXYZ(x,y,z);
}

//======================================================================

bool BsonT< PointProxy >::addPoint(PointCloudGlobal7500& cloud, const mongo::BSONObj& bsonObj)
{
	const pointKind::PointKind kind = PointCloudBase::stringToKind(bsonObj.getField(BsonBase::bsonFtType).String());

	if (kind != cloud.getKind())
		return false;

	GlobalPoint point;
	BsonT<GlobalPoint>::createDataType(point, bsonObj);

	cloud.push_back(point);

	return true;
}

//======================================================================

bool BsonT< PointProxy >::addPoint(PointCloudPlane7510& cloud, const mongo::BSONObj& bsonObj, const bool& init)
{
	const pointKind::PointKind kind = PointCloudBase::stringToKind(bsonObj.getField(BsonBase::bsonFtType).String());

	if (kind != cloud.getKind())
		return false;

	GpsPoint refPoint;
	createGpsPoint(refPoint, bsonObj);

	ReferencePlane refPlane;
	createReferencePlane(refPlane, bsonObj);
	refPlane.setGpsPoint(refPoint);

//	if (init)
//		cloud.setReferencePlane(refPlane);

	PlanePoint point;
	BsonT<PlanePoint>::createDataType(point, bsonObj);

	// check if reference is same as for the cloud
//	if (refPlane != cloud.getReferencePlane())
//	{
//		Point3dFloat newOffset;
//		PointBase::transformToShiftedReferencePlane(refPlane, cloud.getReferencePlane(), point.getPoint3D(), newOffset);
//		point.setPoint3D(newOffset);
//	}

	cloud.push_back(point);

	return true;
}

//======================================================================

void BsonT<GlobalPoint>::createDataType(GlobalPoint& data, const mongo::BSONObj& bsonObj)
{
	GpsPoint refPoint;
	Point3dFloat offset;
	ReferencePlane refPlane;

	BsonT<PointProxy>::createGpsPoint(refPoint, bsonObj);
	BsonT<PointProxy>::createPoint3dFloat(offset, bsonObj);
	BsonT<PointProxy>::createReferencePlane(refPlane, bsonObj);

	refPlane.setGpsPoint(refPoint);

	GpsPoint gpsPoint;

	// transform point if necessary
	if (offset.isZero())
		gpsPoint = refPoint;
	else
		PointBase::transformToGlobalCoordinatesF3d(refPlane, offset, gpsPoint );

	data.setGpsPoint(gpsPoint);

	if (bsonObj.hasField(BsonT<PointProxy>::bsonFtReflectivity))
		data.setEpw(float(bsonObj.getField(BsonT<PointProxy>::bsonFtReflectivity).Double()));

	// type dependend fields
	if (bsonObj.hasField(BsonT<PointProxy>::bsonFtFlags))
		data.setFlags(bsonObj.getField(BsonT<PointProxy>::bsonFtFlags).Int());
}

//======================================================================

void BsonT<PlanePoint>::createDataType(PlanePoint& data, const mongo::BSONObj& bsonObj)
{
	Point3dFloat offset;
	BsonT<PointProxy>::createPoint3dFloat(offset, bsonObj);

	data.setPoint3D(offset);

	// type dependend fields
	if (bsonObj.hasField(BsonT<PointProxy>::bsonFtReflectivity))
		data.setEpw(float(bsonObj.getField(BsonT<PointProxy>::bsonFtReflectivity).Double()));

	// type dependend fields
	if (bsonObj.hasField(BsonT<PointProxy>::bsonFtFlags))
		data.setFlags(bsonObj.getField(BsonT<PointProxy>::bsonFtFlags).Int());
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
