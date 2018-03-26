//======================================================================
/*! \file PointCloudDbReader.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 22, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef POINTCLOUDDBREADER_HPP_SEEN
#define POINTCLOUDDBREADER_HPP_SEEN

//======================================================================

#include <ibeosdk/devices/database/DbDataBlockReader.hpp>

#include <ibeosdk/database/DbInterfaceT.hpp>
#include <ibeosdk/database/DbQueryIterator.hpp>
#include <ibeosdk/database/DbQueryOptions.hpp>

#include <ibeosdk/datablocks/PointCloudPlane7510.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class PointCloudDbReader
 * \brief Derived DbDataBlockReader for DataBlock: PointCloudPlane7510 (0x7510).
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 22, 2016
 *///------------------------------------------------------------------
class PointCloudDbReader : public DbDataBlockReader {
public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	PointCloudDbReader();

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~PointCloudDbReader() {};

public:
	//========================================
	/*!\brief Connects to given collection.
	 *
	 * Uses DbConnectionFactory to establish a connection with a DbInterfaceT<GlobalPoint>
	 * and a DbInterfaceT<PlanePoint>.
	 *
	 * \param[in] collection  CollectionName to connect to.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::CollectionName& collection);

	//========================================
	/*!\brief Not implemented.
	 *
	 * \param[in] trip  Trip.
	 * \return false
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::Trip& trip) { return false; }

	//========================================
	/*!\brief Connects to the collection for processed RoadMarkings.
	 *
	 * Uses DbConnectionFactory to establish connections with a DbInterfaceT<ScanEcu>.
	 *
	 * \param[in] proc  Expects a ProcessingJob from type JobType_RoadMarkingPoints and
	 *                  a non empty ResultConfig.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::ProcessingJob& proc);

	//========================================
	/*!\brief Not implemented.
	 *///-------------------------------------
	virtual void queryData() {};

	//========================================
	/*!\brief Not implemented.
	 *///-------------------------------------
	virtual void notify(DatabaseReplayer& dbReader) {}

	//========================================
	/*!\brief Not implemented.
	 * \return Default NTPTime.
	 *///-------------------------------------
	virtual const NTPTime getCurrentDataBlockTimestamp() const { return NTPTime(); }

	//========================================
	/*!\brief Not implemented.
	 * \return false
	 *///-------------------------------------
	virtual const bool hasValidData() const { return false; }

public:
	//========================================
	/*!\brief Sets the point type and kind suitable for
	 *        PointCloud to be accessed by this reader.
	 *
	 * Type will be pointType::PointTypeWithEpw.
	 * Kind will be pointKind::PointKindScanPoint;
	 *///-------------------------------------
	virtual void setPointCloudMapToBeRead();

	//========================================
	/*!\brief Sets the point type and kind suitable for
	 *        PointCloud
	 *
	 * Type will be pointType::PointTypeWithEpwAndFlags.
	 * Kind will be pointKind::PointKindLanePoint;
	 *///-------------------------------------
	virtual void setRoadMarkingPointCloudToBeRead();

	//========================================
	/*!\brief Queries a complete PointCloudPlane7510.
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 *
	 * \param[out] cloud The PointCloudPlane7510 to fill.
	 *///-------------------------------------
	virtual void queryPointCloudPlane(PointCloudPlane7510& cloud);

	//========================================
	/*!\brief Queries PointCloudPlane7510 specified by double values
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 *
	 * \param[out] cloud        The PointCloudPlane7510 to fill.
	 * \param[in]  firstBegin   Begin range for x.
	 * \param[in]  firstEnd     End range for x.
	 * \param[in]  secondBegin  Begin range for y.
	 * \param[in]  secondEnd    End range for y.
	 * \param[in]  thirdBegin   Begin range for z.
	 * \param[in]  thirdEnd     End range for z.
	 *///-------------------------------------
	virtual void queryPointCloudPlane(PointCloudPlane7510& cloud,
	                                  const double& firstBegin, const double& firstEnd,
	                                  const double& secondBegin, const double& secondEnd,
	                                  const double& thirdBegin, const double& thirdEnd);

	//========================================
	/*!\brief Queries PointCloudPlane7510 specified by GpsPoint's
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 *
	 * \param[out] cloud  The PointCloudPlane7510 to fill.
	 * \param[in]  from   GpsPoint which marks the begin.
	 * \param[in]  to     GpsPoint which marks the end.
	 *///-------------------------------------
	virtual void queryPointCloudPlane(PointCloudPlane7510& cloud,
	                                  const GpsPoint& from, const GpsPoint& to);

	//========================================
	/*!\brief Returns the DbInterfaceT<PlanePoint>::DbIfPtr (boost::shared_ptr).
	 *
	 * With this interface, custom queries to the database can created.
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 *
	 * \return DbInterfaceT<PlanePoint>::DbIfPtr The interface pointer.
	 *///-------------------------------------
	virtual dbaccess::DbInterfaceT<PlanePoint>::DbIfPtr getPpInterface() { return m_ppDbInterface; }

private:
	//========================================
	/*!\brief Queries PointCloudPlane7510 depending on dbaccess::DbQueryOptions.
	 *
	 * \param[out] cloud  The PointCloudPlane7510 to fill.
	 * \param[in]  opt    dbaccess::DbQueryOptions with section specification
	 *///-------------------------------------
	virtual void queryPointCloudPlaneInternal(PointCloudPlane7510& cloud,
	                                          const dbaccess::DbQueryOptions& opt);

protected:
	//========================================
	/*!\brief Database interface for PlanePoint's.
	 *///-------------------------------------
	dbaccess::DbInterfaceT<PlanePoint>::DbIfPtr m_ppDbInterface;

	//========================================
	/*!\brief Standard mongo::DBClientCursor as a dbaccess::MongoCursorPtr
	 *///-------------------------------------
	dbaccess::MongoCursorPtr m_ppDbCursor;

	//========================================
	/*!\brief Describes the point type the PointCloudDbReader
	 *        sets to PointCloudPlane7510.
	 *///-------------------------------------
	pointType::PointType m_type;

	//========================================
	/*!\brief Describes the point kind the PointCloudDbReader
	 *        sets to PointCloudPlane7510.
	 *///-------------------------------------
	pointKind::PointKind m_kind;
}; //PointCloudDbReader

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // POINTCLOUDDBREADER_HPP_SEEN

//======================================================================
