//======================================================================
/*! \file Trip.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef TRIP_HPP_SEEN
#define TRIP_HPP_SEEN

//======================================================================

#include <ibeosdk/database/basedatamodel/BaseEntity.hpp>
#include <ibeosdk/database/basedatamodel/Processing.hpp>

#include <boost/date_time.hpp>

#include <string>
#include <list>
#include <vector>

//======================================================================

namespace ibeosdk {
namespace dbaccess {
class Processing;

//======================================================================
/*!\class Trip
 * \brief Class to represent a Trip, with corresponding files and jobs.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
class Trip : public BaseEntity
{
public:
	//========================================
	/*!\brief Enumeration Trip content.
	 *///-------------------------------------
	enum TripContent {
		TripUndefined,
		TripContentInfoValues,
		TripContentProcessings,
		TripContentScans,
		TripContentVehicleStates,
		TripContentImages,
		TripContentTrajectories,
		TripContentMeasurementList,
		TripContentCANMessages,
		TripContentCarriageWays,
		TripContentPointCloudMap
	};

public:
	//========================================
	/*!\brief Constructor creates Trip with a given name.
	 *///-------------------------------------
	Trip(const std::string& tripname);

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~Trip(){};

public:
	//========================================
	/*!\brief Resets the name, clears the processing list
	 *        and idc paths.
	 *///-------------------------------------
	void reset(const std::string& newTripName);

	//========================================
	/*!\brief Returns the name.
	 * \return Name as string.
	 *///-------------------------------------
	const std::string& getName() const { return m_tripName; }

	//========================================
	/*!\brief Adds an IDC paths to the IDC list.
	 * \param[in] path New path to IDC file.
	 *///-------------------------------------
	void addIdcPath(const std::string& path);

	//========================================
	/*!\brief Adds a ProcessingJob to the list of ProcessingJobs.
	 * \param[in] job The ProcessingJob to add.
	 *///-------------------------------------
	void addProcessingJob(const ProcessingJob& job);

	//========================================
	/*!\brief Returns list of IDC paths.
	 * \return List of paths as vector of strings
	 *///-------------------------------------
	const std::vector<std::string>& getIdcPaths() const {return m_idcPaths; }

	//========================================
	/*!\brief Returns "Trip;" followed by the name of this object.
	 * \return Complete name as string
	 *///-------------------------------------
	virtual std::string getFullDbName() const;

	//========================================
	/*!\brief Returns "Trip;" followed by the name and the given content.
	 * \param[in] content BaseEntity content to use
	 * \return Complete name as string
	 *///-------------------------------------
	virtual std::string getFullDbName(const BaseEntityContent& content) const;

	//========================================
	/*!\brief Returns "Trip;" followed by the name and the given Trip content.
	 * \param[in] tripContent Trip content to use
	 * \return Complete name as string
	 *///-------------------------------------
	virtual std::string getFullDbName(const TripContent& tripContent) const;

	//========================================
	/*!\brief Returns a list of ProcessingJobs.
	 * \return List of ProcessingJobs.
	 *///-------------------------------------
	const ProcJobList& getProcessingJobs() const { return m_jobList; }

	//========================================
	/*!\brief Returns a list of ProcessingJobs.
	 * \return List of ProcessingJobs.
	 *///-------------------------------------
	ProcJobList& getProcessingJobs() { return m_jobList; }

	virtual BaseEntityType getType() const { return TypeTrip; }

private:
	//========================================
	/*!\brief Trip name.
	 *///-------------------------------------
	std::string m_tripName;

	//========================================
	/*!\brief Associated IDC file paths.
	 *///-------------------------------------
	std::vector<std::string> m_idcPaths;

	//========================================
	/*!\brief List of executed ProcessingJobs.
	 *///-------------------------------------
	ProcJobList m_jobList;
}; // Trip
//======================================================================

//========================================
/*!\brief List of multiple Trips.
 *///-------------------------------------
typedef std::list<Trip> TripList;

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // TRIP_HPP_SEEN

//======================================================================
