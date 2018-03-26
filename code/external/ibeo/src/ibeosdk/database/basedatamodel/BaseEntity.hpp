//======================================================================
/*! \file BaseEntity.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 4, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef BASEENTITY_HPP_SEEN
#define BASEENTITY_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <boost/date_time.hpp>

#include <string>
#include <vector>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================
/*!\class BaseEntity
 * \brief Class to abstract the database structure to entity objects.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 4, 2016
 *///-------------------------------------------------------------------
class BaseEntity
{
public:
	//========================================
	/*!\brief Enumeration BaseEntity content.
	 *///-------------------------------------
	enum BaseEntityContent {
		ContentDynamicObjects,
		ContentDynamicObjectStreams,
		ContentStaticObjects,
		NotUsed
	};

	//========================================
	/*!\brief Enumeration BaseEntity types.
	 *///-------------------------------------
	enum BaseEntityType {
		TypeTrip,
		TypeProcessing,
		TypeProcessingJob,
		TypeSession,
		TypeUnknown
	};

protected:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	BaseEntity()
	  : m_keywords(),
	    m_description(),
	    m_date(boost::posix_time::microsec_clock::local_time())
	{}

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~BaseEntity() {}

public:
	//========================================
	/*!\brief Returns the keywords.
	 * \return List of keywords as vector of strings.
	 *///-------------------------------------
	const std::vector<std::string>& getKeywords() const {return m_keywords; }

	//========================================
	/*!\brief Adds a new keyword to the keyword list.
	 * \param[in] keyword New keyword.
	 *///-------------------------------------
	void addKeyword(const std::string& keyword) { m_keywords.push_back(keyword); }

	//========================================
	/*!\brief Sets keywords list.
	 * \param[in] keywords Vector of keywords.
	 *///-------------------------------------
	void setKeywords(const std::vector<std::string>& keywords) { m_keywords = keywords; }

	//========================================
	/*!\brief Clears the keyword list.
	 *///-------------------------------------
	void clearKeywords() { m_keywords.clear(); }

	//========================================
	/*!\brief Returns the description.
	 * \return Description as string.
	 *///-------------------------------------
	const std::string& getDescription() const { return m_description; }

	//========================================
	/*!\brief Sets the description.
	 * \param[in] description The description to set.
	 *///-------------------------------------
	void setDescription(const std::string& description) { m_description = description; }

	//========================================
	/*!\brief Returns the date.
	 * \return Description as boost ptime.
	 *///-------------------------------------
	const boost::posix_time::ptime& getDate() const { return m_date; }

	//========================================
	/*!\brief Sets the date.
	 * \param[in] date The date to set.
	 *///-------------------------------------
	void setDate(const boost::posix_time::ptime& date) { m_date = date; }

public:
	//========================================
	/*!\brief Returns the complete database/collection name for this entity.
	 * \param[in] content BaseEntity content to use
	 * \return Complete name as string
	 *///-------------------------------------
	virtual std::string getFullDbName(const BaseEntityContent& content) const = 0;

	//========================================
	/*!\brief Returns entity type.
	 * \return Type as BaseEntityType
	 *///-------------------------------------
	virtual BaseEntityType getType() const = 0;

public:
	//========================================
	/*!\brief Database name for data model.
	 *///-------------------------------------
	static const std::string Content_DataModel;

	//========================================
	/*!\brief Database name for trip.
	 *///-------------------------------------
	static const std::string Content_Trip;

	//========================================
	/*!\brief Collection prefix for processing.
	 *///-------------------------------------
	static const std::string ContentPrefix_Processing;

	//========================================
	/*!\brief Collection name for CanMessages.
	 *///-------------------------------------
	static const std::string ContentSuffix_CanMessage;

	//========================================
	/*!\brief Collection name for CarriageWays.
	 *///-------------------------------------
	static const std::string ContentSuffix_CarriageWays;

	//========================================
	/*!\brief Collection name for DutObj.
	 *///-------------------------------------
	static const std::string ContentSuffix_DutObj;

	//========================================
	/*!\brief Collection name for DutObjList.
	 *///-------------------------------------
	static const std::string ContentSuffix_DutObjList;

	//========================================
	/*!\brief Collection name for DynObj.
	 *///-------------------------------------
	static const std::string ContentSuffix_DynObj;

	//========================================
	/*!\brief Collection name for ObjTraj.
	 *///-------------------------------------
	static const std::string ContentSuffix_ObjTraj;

	//========================================
	/*!\brief Collection name for Images.
	 *///-------------------------------------
	static const std::string ContentSuffix_Images;

	//========================================
	/*!\brief Collection name for InfoValues.
	 *///-------------------------------------
	static const std::string ContentSuffix_InfoValues;

	//========================================
	/*!\brief Collection name for JobBase.
	 *///-------------------------------------
	static const std::string ContentSuffix_JobBase;

	//========================================
	/*!\brief Collection name for JobList.
	 *///-------------------------------------
	static const std::string ContentSuffix_JobList;

	//========================================
	/*!\brief Collection name for LanePoints.
	 *///-------------------------------------
	static const std::string ContentSuffix_LanePoints;

	//========================================
	/*!\brief Collection name for MeasurementLists.
	 *///-------------------------------------
	static const std::string ContentSuffix_MeasurementLists;

	//========================================
	/*!\brief Collection name for static objects.
	 *///-------------------------------------
	static const std::string ContentSuffix_StatObj;

	//========================================
	/*!\brief Collection name for ObjectAssociations.
	 *///-------------------------------------
	static const std::string ContentSuffix_ObjectAssociations;

	//========================================
	/*!\brief Collection name for ObjectFlags.
	 *///-------------------------------------
	static const std::string ContentSuffix_ObjectFlags;

	//========================================
	/*!\brief Collection name for OelaMeasurement.
	 *///-------------------------------------
	static const std::string ContentSuffix_OelaMeasurement;

	//========================================
	/*!\brief Collection name for ProcessingJobs.
	 *///-------------------------------------
	static const std::string ContentSuffix_ProcessingJobs;

	//========================================
	/*!\brief Collection name for Scans.
	 *///-------------------------------------
	static const std::string ContentSuffix_Scans;

	//========================================
	/*!\brief Collection name for Sessions.
	 *///-------------------------------------
	static const std::string ContentSuffix_Sessions;

	//========================================
	/*!\brief Collection name for VehicleStates.
	 *///-------------------------------------
	static const std::string ContentSuffix_VehicleStates;

private:
	//========================================
	/*!\brief Associated keywords.
	 *///-------------------------------------
	std::vector<std::string> m_keywords;

	//========================================
	/*!\brief Associated description.
	 *///-------------------------------------
	std::string m_description;

	//========================================
	/*!\brief Corresponding date.
	 *///-------------------------------------
	boost::posix_time::ptime m_date;
}; // BaseEntity

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // BASEENTITY_HPP_SEEN

//======================================================================
