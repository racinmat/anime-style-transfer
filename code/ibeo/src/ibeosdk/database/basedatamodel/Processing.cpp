//======================================================================
/*! \file Processing.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/basedatamodel/Processing.hpp>
#include <ibeosdk/database/basedatamodel/Trip.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

ProcessingUtil::ProcessingUtil()
{
	m_jobIds = std::vector<std::string>(ProcessingJob::JobType_JobMax);

	m_jobIds[ProcessingJob::JobType_DynamicObjects]    = "JobDynamicObjects";
	m_jobIds[ProcessingJob::JobType_Lanes]             = "JobLanes";
	m_jobIds[ProcessingJob::JobType_Oela]              = "JobOELA";
	m_jobIds[ProcessingJob::JobType_BsdLca]            = "Job_BSD_LCA";
	m_jobIds[ProcessingJob::JobType_RoadMarkingPoints] = "JobRoadmarkingPoints";
	m_jobIds[ProcessingJob::JobType_DutMatching]       = "JobDutMatching";
	m_jobIds[ProcessingJob::JobType_VehicleState]      = "JobVehicleState";

	m_map[m_jobIds[ProcessingJob::JobType_DynamicObjects]]    = ProcessingJob::JobType_DynamicObjects;
	m_map[m_jobIds[ProcessingJob::JobType_Lanes]]             = ProcessingJob::JobType_Lanes;
	m_map[m_jobIds[ProcessingJob::JobType_Oela]]              = ProcessingJob::JobType_Oela;
	m_map[m_jobIds[ProcessingJob::JobType_BsdLca]]            = ProcessingJob::JobType_BsdLca;
	m_map[m_jobIds[ProcessingJob::JobType_RoadMarkingPoints]] = ProcessingJob::JobType_RoadMarkingPoints;
	m_map[m_jobIds[ProcessingJob::JobType_DutMatching]]       = ProcessingJob::JobType_DutMatching;
	m_map[m_jobIds[ProcessingJob::JobType_VehicleState]]      = ProcessingJob::JobType_VehicleState;
}

//======================================================================

ProcessingUtil* ProcessingUtil::m_singleton = NULL;

//======================================================================

ProcessingUtil* ProcessingUtil::getInstance()
{
	if (m_singleton == NULL)
		m_singleton = new ProcessingUtil();
	return m_singleton;
}

//======================================================================

const ProcessingJob::JobType ProcessingUtil::getTypeFromStr(const std::string& type)
{
	std::map<std::string, ProcessingJob::JobType>::iterator it = m_map.find(type);
	return it != m_map.end() ? it->second : ProcessingJob::JobType_Unknown;
}

//======================================================================

const std::string ProcessingUtil::getStrFromType(const ProcessingJob::JobType& type)
{
	try {
		return m_jobIds.at(type);
	} catch(const std::out_of_range &e) {
		return "";
	}
};

//======================================================================
//======================================================================
//======================================================================

//======================================================================
//======================================================================
//======================================================================

ProcessingJob::ProcessingJob()
  : BaseEntity(),
    m_collectionName(""),
    m_dbId(),
    m_type(JobType_Unknown),
    m_tripName(),
    m_startTime(),
    m_finishTime(),
    m_resultConfig()
{}

//======================================================================

ProcessingJob::ProcessingJob(const std::string& cn, const std::string& id)
  : BaseEntity(),
    m_collectionName(cn),
    m_dbId(id),
    m_type(JobType_Unknown),
    m_tripName(),
    m_startTime(),
    m_finishTime(),
    m_resultConfig()
{}

//======================================================================

std::string ProcessingJob::getFullDbName(const BaseEntityContent& content) const
{
	switch(NotUsed) {
	case NotUsed:
		return Content_DataModel + ContentSuffix_JobList;
		break;
	default:
		assert(false);
		return "__undefined__field ";
	} // switch
}

//======================================================================

const std::string ProcessingJob::getTypeAsString() const
{
	return ProcessingUtil::getInstance()->getStrFromType(m_type);
}

//======================================================================
//======================================================================
//======================================================================

//======================================================================
//======================================================================
//======================================================================

Processing::Processing()
  : BaseEntity(),
    m_collectionName(""),
    m_jobList(),
    m_dbId(),
    m_name("")
{}

//======================================================================

Processing::Processing(const std::string& cn, const std::string& id)
  : BaseEntity(),
    m_collectionName(CollectionName(cn)),
    m_jobList(),
    m_dbId(id),
    m_name()
{}

//======================================================================

Processing::Processing(const std::string& name)
  : BaseEntity(),
    m_collectionName(""),
    m_jobList(),
    m_dbId(),
    m_name(name)
{}

//======================================================================

std::string Processing::getFullDbName(const BaseEntity::BaseEntityContent& content) const
{
	switch(NotUsed) {
	case NotUsed:
		return Content_DataModel + ContentSuffix_JobList;
		break;
	default:
		assert(false);
		return "__undefined__field ";
	} // switch
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
