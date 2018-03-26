//======================================================================
/*! \file BsonT_Point.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 21, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef BSONT_POINT_HPP_SEEN
#define BSONT_POINT_HPP_SEEN

//======================================================================

#ifndef BSONT_HPP_SEEN
#	error "Must be include by BsonT.hpp"
#endif // BSONT_HPP_SEEN

//======================================================================

#include <ibeosdk/datablocks/PointCloudGlobal7500.hpp>
#include <ibeosdk/datablocks/PointCloudPlane7510.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================
/*!\class BsonT<PointProxy>
 * \brief BsonT representation for a PointProxy.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 21, 2016
 *///-------------------------------------------------------------------
template<>
class BsonT<PointProxy> : public BsonBase
{
public:
	//========================================
	/*!\brief Converts a BSON object to a GpsPoint.
	 *
	 * \param[in,out] gpsPoint The GpsPoint to fill.
	 * \param[in] bsonObj The BSON object from the database query.
	 *///-------------------------------------
	static void createGpsPoint(GpsPoint& gpsPoint, const mongo::BSONObj& bsonObj);

	//========================================
	/*!\brief Converts a BSON object to a ReferencePlane.
	 *
	 * \param[in,out] refPlane The ReferencePlane to fill.
	 * \param[in] bsonObj The BSON object from the database query.
	 *///-------------------------------------
	static void createReferencePlane(ReferencePlane& refPlane, const mongo::BSONObj& bsonObj);

	//========================================
	/*!\brief Converts a BSON object to a Point3dFloat.
	 *
	 * \param[in,out] point The Point3dFloat to fill.
	 * \param[in] bsonObj The BSON object from the database query.
	 *///-------------------------------------
	static void createPoint3dFloat(Point3dFloat& point, const mongo::BSONObj& bsonObj);

	//========================================
	/*!\brief Converts a BSON object to a GlobalPoint and adding it
	 *        to a PointCloudGlobal7500.
	 *
	 * \param[in,out] cloud The PointCloudGlobal7500 the GlobalPoint will be added to.
	 * \param[in] bsonObj The BSON object from the database query.
	 *///-------------------------------------
	static bool addPoint(PointCloudGlobal7500& cloud, const mongo::BSONObj& bsonObj);

	//========================================
	/*!\brief Converts a BSON object to a PlanePoint and adding it
	 *        to a PointCloudPlane7510.
	 *
	 * \param[in,out] cloud The PointCloudPlane7510 the PlanePoint will be added to.
	 * \param[in] bsonObj The BSON object from the database query.
	 * \param[in] init Indicates if the PlanePoint is the first one
	 *///-------------------------------------
	static bool addPoint(PointCloudPlane7510& cloud, const mongo::BSONObj& bsonObj,
	                     const bool& init);

	//========================================
	/*!\brief Database field for reflectivity.
	 *///-------------------------------------
	static const std::string bsonFtReflectivity;

	//========================================
	/*!\brief Database field for flags.
	 *///-------------------------------------
	static const std::string bsonFtFlags;

	//========================================
	/*!\brief Database field for RefPlaneYaw.
	 *///-------------------------------------
	static const std::string bsonFtRefPlaneYaw;

	//========================================
	/*!\brief Database field for RefPlanePitch.
	 *///-------------------------------------
	static const std::string bsonFtRefPlanePitch;

	//========================================
	/*!\brief Database field for RefPlaneRoll.
	 *///-------------------------------------
	static const std::string bsonFtRefPlaneRoll;
}; // BsonT<PointProxy>

//======================================================================
//======================================================================
//======================================================================

//======================================================================
//======================================================================
//======================================================================
/*!\class BsonT<GlobalPoint>
 * \brief BsonT representation for a GlobalPoint.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 21, 2016
 *///-------------------------------------------------------------------
template<>
class BsonT<GlobalPoint> : public BsonBase
{
public:
	//========================================
	/*!\brief Converts a BSON object to a GlobalPoint object.
	 *
	 * \param[in,out] data The GlobalPoint to fill.
	 * \param[in] bsonObj The BSON object from the database query.
	 *///-------------------------------------
	static void createDataType(GlobalPoint& data, const mongo::BSONObj& bsonObj);
}; // BsonT<GlobalPoint>

//======================================================================
//======================================================================
//======================================================================

//======================================================================
//======================================================================
//======================================================================
/*!\class BsonT<PlanePoint>
 * \brief BsonT representation for a PlanePoint.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 21, 2016
 *///-------------------------------------------------------------------
template<>
class BsonT<PlanePoint> : public BsonBase
{
public:
	//========================================
	/*!\brief Converts a BSON object to a PlanePoint object.
	 *
	 * \param[in,out] data The PlanePoint to fill.
	 * \param[in] bsonObj The BSON object from the database query.
	 *///-------------------------------------
	static void createDataType(PlanePoint& data, const mongo::BSONObj& bsonObj);
}; // BsonT<PlanePoint>

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // POINT_HPP_SEEN

//======================================================================
