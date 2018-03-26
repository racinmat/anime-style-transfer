//======================================================================
/*! \file PointCloudDbReader.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 22, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/database/PointCloudDbReader.hpp>
#include <ibeosdk/database/DbConnectionFactory.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

using namespace dbaccess;
using namespace dbField;

//======================================================================

PointCloudDbReader::PointCloudDbReader()
  : DbDataBlockReader(ProcessingJob::JobType_RoadMarkingPoints),
    m_ppDbInterface(),
    m_ppDbCursor(),
    m_type(pointType::PointTypeWithEpwAndFlags),
    m_kind(pointKind::PointKindLanePoint)
{}

//======================================================================

bool PointCloudDbReader::connectToDbInterface(const dbaccess::CollectionName& collection)
{
	bool collectionCheck = false;

	DbConnectionFactory::ScopedConnection sc;
	m_ppDbInterface = sc.getDbIfPtr<PlanePoint>(collection, collectionCheck);

	return collectionCheck;
}

//======================================================================

bool PointCloudDbReader::connectToDbInterface(const dbaccess::ProcessingJob& proc)
{
	std::string ns = "";

	const ResultConfig& resultConf = proc.getResultConfig();

	if (   (resultConf.size() < 1)
	    || (proc.getJobType() != ProcessingJob::JobType_RoadMarkingPoints)) {
		return false;
	}

	return connectToDbInterface(CollectionName(resultConf[0]));
}

//======================================================================

void PointCloudDbReader::setPointCloudMapToBeRead()
{
	m_type = pointType::PointTypeWithEpw;
	m_kind = pointKind::PointKindScanPoint;
}

//======================================================================

void PointCloudDbReader::setRoadMarkingPointCloudToBeRead()
{
	m_type = pointType::PointTypeWithEpwAndFlags;
	m_kind = pointKind::PointKindLanePoint;
}

//======================================================================

void PointCloudDbReader::queryPointCloudPlane(PointCloudPlane7510& cloud)
{
	DbQueryOptions opt;
	queryPointCloudPlaneInternal(cloud, opt);
}

//======================================================================

void PointCloudDbReader::queryPointCloudPlane(PointCloudPlane7510& cloud,
                                              const GpsPoint& from,
                                              const GpsPoint& to)
{
	ReferencePlane refPlane;

	m_ppDbCursor = m_ppDbInterface->fastQuery(DbQueryOptions().limit(1));

	if (m_ppDbCursor->more()) {
		const mongo::BSONObj bo = m_ppDbCursor->next();

		if (!bo.isEmpty()) {
			GpsPoint refPoint;
			BsonT<PointProxy>::createGpsPoint(refPoint, bo);
			BsonT<PointProxy>::createReferencePlane(refPlane, bo);

			refPlane.setGpsPoint(refPoint);
		}
	}

	Point3dFloat fromOffset;
	Point3dFloat toOffset;

	PointBase::transformToRelativeCoordinatesF3d(refPlane, from, fromOffset);
	PointBase::transformToRelativeCoordinatesF3d(refPlane, to, toOffset);

	DbQueryOptions opt = DbQueryOptions().minEquals(DbField_X, fromOffset.getX()).maxEquals(DbField_X, toOffset.getX())
	                                     .minEquals(DbField_Y, fromOffset.getY()).maxEquals(DbField_Y, toOffset.getY())
	                                     .minEquals(DbField_Z, fromOffset.getZ()).maxEquals(DbField_Z, toOffset.getZ());

	queryPointCloudPlaneInternal(cloud, opt);
}

//======================================================================

void PointCloudDbReader::queryPointCloudPlane(PointCloudPlane7510& cloud,
                                              const double& firstBegin, const double& firstEnd,
                                              const double& secondBegin, const double& secondEnd,
                                              const double& thirdBegin, const double& thirdEnd)
{
	DbQueryOptions opt = DbQueryOptions().minEquals(DbField_X, firstBegin).maxEquals(DbField_X, firstEnd)
	                                     .minEquals(DbField_Y, secondBegin).maxEquals(DbField_Y, secondEnd)
	                                     .minEquals(DbField_Z, thirdBegin).maxEquals(DbField_Z, thirdEnd);

	queryPointCloudPlaneInternal(cloud, opt);
}

//======================================================================

void PointCloudDbReader::queryPointCloudPlaneInternal(PointCloudPlane7510& cloud,
                                                      const DbQueryOptions& opt)
{
	cloud.setType(m_type);
	cloud.setKind(m_kind);

	m_ppDbCursor = m_ppDbInterface->fastQuery(opt);

	bool isFirstPoint = true;
	while (m_ppDbCursor->more()) {
		mongo::BSONObj bo = m_ppDbCursor->next();

		if (!bo.isEmpty()) {
			std::string bsonobj = bo.toString();
			BsonT<PointProxy>::addPoint(cloud, bo, isFirstPoint);
			isFirstPoint = false;
		}
	}
}

//======================================================================

} // namespace ibeosdk

//======================================================================
