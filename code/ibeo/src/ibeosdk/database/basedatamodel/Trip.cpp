//======================================================================
/*! \file Trip.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/basedatamodel/Trip.hpp>
#include <ibeosdk/Log.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

Trip::Trip(const std::string& tripname)
  : BaseEntity(),
    m_tripName(tripname),
    m_idcPaths(),
    m_jobList()
{}

//======================================================================

void Trip::reset(const std::string& newTripName)
{
	m_tripName = newTripName;
	m_idcPaths.clear();
	m_jobList.clear();
}

//======================================================================

std::string Trip::getFullDbName(const TripContent& tripContent) const
{
	std::string ret = std::string(Content_Trip).append(getName());
	switch(tripContent) {
	case TripUndefined:
		ret += ";";
		break;
	case TripContentProcessings:
		ret += ContentSuffix_ProcessingJobs;
		break;
	case TripContentScans:
		ret += ContentSuffix_Scans;
		break;
	case TripContentVehicleStates:
		ret += ContentSuffix_VehicleStates;
		break;
	case TripContentImages:
		ret += ContentSuffix_Images;
		break;
	case TripContentMeasurementList:
		ret += ContentSuffix_MeasurementLists;
		break;
	case TripContentCANMessages:
		ret += ContentSuffix_CanMessage;
		break;
	case TripContentInfoValues:
		ret += ContentSuffix_InfoValues;
		break;
	case TripContentCarriageWays:
		ret += ContentSuffix_CarriageWays;
		break;
	default:
		assert(false);
		ret += "__undefined__trip ";
	} // switch
	return ret;
}

//======================================================================

std::string Trip::getFullDbName() const
{
	return std::string(Content_Trip).append(getName());
}

//======================================================================

std::string Trip::getFullDbName(const BaseEntityContent& content) const
{

	std::string ret = getFullDbName();
	switch(content) {
	case BaseEntity::ContentDynamicObjects:
		ret += BaseEntity::ContentSuffix_DynObj;
		break;
	case BaseEntity::ContentDynamicObjectStreams:
		ret += BaseEntity::ContentSuffix_ObjTraj;
		break;
	case BaseEntity::ContentStaticObjects:
		ret += BaseEntity::ContentSuffix_StatObj;
		break;
	default:
		assert(false);
		ret += "__undefined__base__trip";
	} // switch
	return ret;
}

//======================================================================

void Trip::addIdcPath(const std::string& path)
{
	m_idcPaths.push_back(path);
}

//======================================================================

void Trip::addProcessingJob(const ProcessingJob& job)
{
	m_jobList.push_back(job);
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
