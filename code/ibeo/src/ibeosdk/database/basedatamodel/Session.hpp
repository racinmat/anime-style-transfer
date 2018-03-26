//======================================================================
/*! \file Session.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef SESSION_HPP_SEEN
#define SESSION_HPP_SEEN

//======================================================================

#include <ibeosdk/database/basedatamodel/BaseEntity.hpp>
#include <ibeosdk/database/basedatamodel/Trip.hpp>
#include <ibeosdk/database/basedatamodel/Processing.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {
class Session;

//========================================
/*!\brief List of Sessions.
 *///-------------------------------------
typedef std::list<Session> SessionList;

//======================================================================
/*!\class Session
 * \brief Class to represent a Session, with corresponding trips and processings.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
class Session : public BaseEntity
{
public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	Session();

	//========================================
	/*!\brief Constructor creates Session with a given name.
	 *///-------------------------------------
	Session(const std::string& name);

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~Session() {};

public:
	//========================================
	/*!\brief Resets the name, clears the processing list
	 *        and trip list.
	 *///-------------------------------------
	void reset(const std::string& newName);

	//========================================
	/*!\brief Returns "Trip;" followed by the name of this object.
	 * \return Complete name as string.
	 *///-------------------------------------
	std::string getFullDbName(const BaseEntity::BaseEntityContent& content) const;

	//========================================
	/*!\brief Adds a ProcessingJob to the processing list.
	 * \param[in] trip New Trip to be added.
	 *///-------------------------------------
	void addTrip(const Trip& trip) { m_tripList.push_back(trip); }

	//========================================
	/*!\brief Adds a Processing to the list.
	 * \param[in] proc The Processing to add.
	 *///-------------------------------------
	void addProcessing(const Processing& proc) { m_procList.push_back(proc); }

	//========================================
	/*!\brief Returns the name.
	 * \return Name as string.
	 *///-------------------------------------
	const std::string& getName() const { return m_name; }

	//========================================
	/*!\brief Returns a list of Trips.
	 * \return List of Trips.
	 *///-------------------------------------
	const TripList& getTrips() const { return m_tripList; }

	//========================================
	/*!\brief Returns a list of Trips.
	 * \return List of Trips.
	 *///-------------------------------------
	TripList& getTrips() { return m_tripList; }

	//========================================
	/*!\brief Returns a list of Processings.
	 * \return List of Processings.
	 *///-------------------------------------
	const ProcList& getProcessings() const { return m_procList; }

	//========================================
	/*!\brief Returns a list of Processings.
	 * \return List of Processings.
	 *///-------------------------------------
	ProcList& getProcessings() { return m_procList; }

	//========================================
	/*!\brief Returns just the name from the Trips
	 *        stored in the Trip list.
	 * \return List of names as vector of strings.
	 *///-------------------------------------
	const std::vector<std::string> getTripNames() const;

	//========================================
	/*!\brief Returns just the name from the Processings
	 *        stored in the Processing list.
	 * \return List of names as vector of strings.
	 *///-------------------------------------
	const std::vector<std::string> getProcessingNames() const;

	//========================================
	/*!\brief Removes a Trip from the Session.
	 * \param[in] tripName The name of the Trip to remove.
	 *///-------------------------------------
	void removeTrip(const std::string& tripName);

	//========================================
	/*!\brief Removes a Processing from the Session.
	 * \param[in] procName The name of the Processing to remove.
	 *///-------------------------------------
	void removeProcessing(const std::string& procName);

	//========================================
	/*!\brief Finds a Processing by name and returns a reference to it.
	 * \param[in] procName The name of the Processing.
	 * \param[in, out] proc Processing instance.
	 * \return True if the Session has Processings, false if not
	 *///-------------------------------------
	virtual bool hasProcessing(const std::string& procName, Processing& proc);

	//========================================
	/*!\brief Finds a Trip by name
	 * \param[in] tripName The name of the Trip.
	 * \return True if the Session contains the Trip, false if not
	 *///-------------------------------------
	virtual bool hasTrip(const std::string& tripName);

	virtual BaseEntityType getType() const { return TypeSession; }

private:
	//========================================
	/*!\brief Session name.
	 *///-------------------------------------
	std::string m_name;

	//========================================
	/*!\brief List of Trips
	 *///-------------------------------------
	TripList m_tripList;

	//========================================
	/*!\brief List of Processings
	 *///-------------------------------------
	ProcList m_procList;
}; // Session

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // SESSION_HPP_SEEN

//======================================================================
