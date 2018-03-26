//======================================================================
/*! \file Processing.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef PROCESSING_HPP_SEEN
#define PROCESSING_HPP_SEEN

//======================================================================

#include <ibeosdk/database/basedatamodel/BaseEntity.hpp>
#include <ibeosdk/database/CollectionName.hpp>

#include <ibeosdk/Time.hpp>
#include <ibeosdk/inttypes.hpp>

#include <list>
#include <map>

//======================================================================

namespace ibeosdk {
class DbDataBlockReader;

namespace dbaccess {
class Processing;
class ProcessingJob;
class ProcessingUtil;

//========================================
/*!\brief List of ProcessingJobs.
 *///-------------------------------------
typedef std::list<ProcessingJob> ProcJobList;

//========================================
/*!\brief List of Processings.
 *///-------------------------------------
typedef std::list<Processing> ProcList;

//========================================
/*!\brief List of result collections
 *///-------------------------------------
typedef std::vector<std::string> ResultConfig;

//======================================================================
/*!\class ProcessingJob
 * \brief Class to represent a ProcessingJob
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
class ProcessingJob : public BaseEntity
{
public:
	//========================================
	/*!\brief Enumeration of Job types.
	 *///-------------------------------------
	enum JobType {
		JobType_Unknown,
		JobType_None,
		JobType_DynamicObjects,
		JobType_Lanes,
		JobType_Oela,
		JobType_BsdLca,
		JobType_RoadMarkingPoints,
		JobType_DutMatching,
		JobType_VehicleState,
		JobType_JobMax
	};

public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	ProcessingJob();

	//========================================
	/*!\brief Creates ProcessingJob from a given collection name and an id.
	 *///-------------------------------------
	ProcessingJob(const std::string& cn, const std::string& id);

	//========================================
	/*!\brief Default deconstructor.
	 *///-------------------------------------
	virtual ~ProcessingJob(){};

public:
	//========================================
	/*!\brief Returns the JobType.
	 * \return JobType
	 *///-------------------------------------
	const JobType& getJobType() const { return m_type; }

	//========================================
	/*!\brief Returns the collection name.
	 * \return Collection name.
	 *///-------------------------------------
	const CollectionName& getCollectionName() const { return m_collectionName; }

	//========================================
	/*!\brief Returns the JobType as a string.
	 * \return JobType as a string.
	 *///-------------------------------------
	const std::string getTypeAsString() const;

	//========================================
	/*!\brief Returns database id.
	 * \return ID as string.
	 *///-------------------------------------
	const std::string& getDbId() const { return m_dbId; }

	//========================================
	/*!\brief Returns trip name.
	 * \return Trip name as string.
	 *///-------------------------------------
	const std::string& getTripName() const { return m_tripName; }

	//========================================
	/*!\brief Returns start time.
	 * \return Start time as NTPTime.
	 *///-------------------------------------
	const NTPTime& getStartTime() const { return m_startTime; }

	//========================================
	/*!\brief Returns finish time.
	 * \return Finish time as NTPTime.
	 *///-------------------------------------
	const NTPTime& getFinishTime() const { return m_finishTime; }

	//========================================
	/*!\brief Returns result config.
	 * \return Finish time as NTPTime.
	 *///-------------------------------------
	const ResultConfig& getResultConfig() const { return m_resultConfig; }

	//========================================
	/*!\brief Sets a database identifier.
	 * \param[in] key Database identifier as string.
	 *///-------------------------------------
	void setDbId(const std::string& key) { m_dbId = key; }

	//========================================
	/*!\brief Sets a JobType.
	 * \param[in] type The JobType to set.
	 *///-------------------------------------
	void setJobType(const JobType& type) { m_type = type; }

	//========================================
	/*!\brief Sets a Trip name.
	 * \param[in] trip The Trip name to set.
	 *///-------------------------------------
	void setTripName(const std::string& trip) { m_tripName = trip; }

	//========================================
	/*!\brief Sets a start time.
	 * \param[in] time The start time to set.
	 *///-------------------------------------
	void setStartTime(const NTPTime& time) { m_startTime = time; }

	//========================================
	/*!\brief Sets a finish time
	 * \param[in] time The finish time to set.
	 *///-------------------------------------
	void setFinishTime(const NTPTime& time) { m_finishTime = time; }

	//========================================
	/*!\brief Sets a Result config.
	 * \param[in] results The Result config to set.
	 *///-------------------------------------
	void setResultConfig(const ResultConfig& results) { m_resultConfig = results; }

	virtual std::string getFullDbName(const BaseEntityContent& content) const;
	virtual BaseEntityType getType() const { return TypeProcessingJob; };

private:
	//========================================
	/*!\brief Collection name.
	 *///-------------------------------------
	CollectionName m_collectionName;

	//========================================
	/*!\brief Database identifier.
	 *///-------------------------------------
	std::string m_dbId;

	//========================================
	/*!\brief Jobtype.
	 *///-------------------------------------
	JobType m_type;

	//========================================
	/*!\brief Trip name.
	 *///-------------------------------------
	std::string m_tripName;

	//========================================
	/*!\brief Start time.
	 *///-------------------------------------
	NTPTime m_startTime;

	//========================================
	/*!\brief Finish time.
	 *///-------------------------------------
	NTPTime m_finishTime;

	//========================================
	/*!\brief Result config - describes the result
	 *        collections after processing
	 *///-------------------------------------
	ResultConfig m_resultConfig;

private:
	friend class ::ibeosdk::DbDataBlockReader;
}; //ProcessingJob

//======================================================================
//======================================================================
//======================================================================

//======================================================================
//======================================================================
//======================================================================
/*!\class Processing
 * \brief Class to represent a Processing
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
class Processing : public BaseEntity
{
public:
	//========================================
	/*!\brief Default Constructor.
	 *///-------------------------------------
	Processing();

	//========================================
	/*!\brief Constructor creates Processing with a given name and id.
	 *///-------------------------------------
	Processing(const std::string& cn, const std::string& id);

	//========================================
	/*!\brief Constructor creates Processing with a given name.
	 *///-------------------------------------
	Processing(const std::string& name);

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~Processing() {};

public:
	//========================================
	/*!\brief Returns the collection name.
	 * \return Name as string.
	 *///-------------------------------------
	const CollectionName& getCollectionName() const { return m_collectionName; }

	//========================================
	/*!\brief Returns a list of ProcessingJobs.
	 * \return List of ProcessingJobs.
	 *///-----------------------------------
	const ProcJobList& getJobList() const { return m_jobList; }

	//========================================
	/*!\brief Returns a list of ProcessingJobs.
	 * \return List of ProcessingJobs.
	 *///-----------------------------------
	ProcJobList& jobList() { return m_jobList; }

	//========================================
	/*!\brief Sets a processing name.
	 * \param[in] name Name as string.
	 *///-------------------------------------
	void setName(const std::string& name) { m_name = name; };

	//========================================
	/*!\brief Returns the processing name.
	 * \return Name as string.
	 *///-------------------------------------
	const std::string& getName() const { return m_name; }

	//========================================
	/*!\brief Sets a database identifier.
	 * \param[in] id Database identifier as string.
	 *///-------------------------------------
	void setDbId(const std::string& id) { m_dbId = id; };

	//========================================
	/*!\brief Returns database identifier.
	 * \return ID as string.
	 *///-------------------------------------
	const std::string& getDbId() const { return m_dbId; }

	//========================================
	/*!\brief Adds a ProcessingJob to the list of ProcessingJobs.
	 * \param[in] job The ProcessingJob to add.
	 *///-------------------------------------
	void addProcessingJob(const ProcessingJob& job) { m_jobList.push_back(job);}

	virtual std::string getFullDbName(const BaseEntity::BaseEntityContent& content) const;

	virtual BaseEntityType getType() const { return TypeProcessing; }

private:
	//========================================
	/*!\brief Collection name.
	 *///-------------------------------------
	CollectionName m_collectionName;

	//========================================
	/*!\brief List of ProcessingJobs
	 *///-------------------------------------
	ProcJobList m_jobList;

	//========================================
	/*!\brief Database ID
	 *///-------------------------------------
	std::string m_dbId;

	//========================================
	/*!\brief Processing name.
	 *///-------------------------------------
	std::string m_name;
}; // Processing

//======================================================================
//======================================================================
//======================================================================

//======================================================================
//======================================================================
//======================================================================
/*!\class ProcessingUtil
 * \brief Util class to support the creation of information for Processings
 *        This class follows the Singleton concept
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
class ProcessingUtil
{
public:
	//========================================
	/*!\brief Returns the singleton instance as a pointer.
	 * \return ProcessingUtil instance.
	 *///-------------------------------------
	static ProcessingUtil* getInstance();

	//========================================
	/*!\brief Returns JobType from a given string.
	 *
	 * \return If given type wasn't found JobType_Unknown will be returned
	 *         else the actual JobType.
	 *///-------------------------------------
	const ProcessingJob::JobType getTypeFromStr(const std::string& type);

	//========================================
	/*!\brief Returns string from a given JobType.
	 *
	 * \return If given type wasn't found an empty string will be returned
	 *         else the JobType as string.
	 *///-------------------------------------
	const std::string getStrFromType(const ProcessingJob::JobType& type);

private:
	//========================================
	/*!\brief Private constructor, creates job ids and corresponding map.
	 *///-------------------------------------
	ProcessingUtil();

private:
	//========================================
	/*!\brief Processing name.
	 *///-------------------------------------
	static ProcessingUtil* m_singleton;

	//========================================
	/*!\brief Vector of job ids
	 *///-------------------------------------
	std::vector<std::string> m_jobIds;

	//========================================
	/*!\brief Map from string to JobType
	 *///-------------------------------------
	std::map<std::string, ProcessingJob::JobType> m_map;
}; //ProcessingUtil

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // PROCESSING_HPP_SEEN

//======================================================================
