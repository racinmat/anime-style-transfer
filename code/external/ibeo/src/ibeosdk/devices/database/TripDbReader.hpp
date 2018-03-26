//======================================================================
/*! \file TripDbReader.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 18, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef TRIPDBREADER_HPP_SEEN
#define TRIPDBREADER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/database/MongoDbInterface.hpp>
#include <ibeosdk/database/DbInterfaceT.hpp>
#include <ibeosdk/database/DbQueryIterator.hpp>

#include <ibeosdk/database/basedatamodel/Trip.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class TripDbReader
 * \brief Independent Reader for Trip's. This class is not derived
 *        from DbDataBlockReader.
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 18, 2016
 *///-------------------------------------------------------------------
class TripDbReader
{

public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	TripDbReader();

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~TripDbReader() {}

public:
	//========================================
	/*!\brief Connects to database.
	 *
	 * Uses DbConnectionFactory to establish a connection
	 * with a MongoDbInterface.
	 *///-------------------------------------
	virtual void connect();

public:
	//========================================
	/*!\brief Queries information about a Trip object, which
	 *        already has a given Trip name.
	 *
	 * \note It is necessary to call TripDbReader::connect
	 *       function first, to establish a database connection.
	 * \param[in, out]  trip  The Trip to be filled.
	 * \return \c true, if Trip is in database
	 *///-------------------------------------
	virtual bool getTrip(dbaccess::Trip& trip);

	//========================================
	/*!\brief Gets all trip's of the database.
	 *
	 * The returned trip object's just contain the Trip names
	 * and no further information.
	 * To get all information one can call
	 * TripDbReader::getTrip for one Trip or TripDbReader::fillTrips
	 * for a list of Trips.
	 *
	 * \note It is necessary to call TripDbReader::connect
	 *       function first, to establish a database connection.
	 *
	 * \return A TripList which contains all Trip's
	 *         from the database. The list will be empty if
	 *         no Trip can be found.
	 *///-------------------------------------
	virtual dbaccess::TripList getTrips();

	//========================================
	/*!\brief Iterates through a list of Trip's and queries
	 *        information for each.
	 *
	 * \note It is necessary to call TripDbReader::connect
	 *       function first, to establish a database connection.
	 * \param[in, out]  list  List of Trip's.
	 *///-------------------------------------
	virtual void fillTrips(dbaccess::TripList& list);

private:
	//========================================
	/*!\brief General database interface.
	 *///-------------------------------------
	dbaccess::MongoDbInterface::DbIfPtr m_dbInterface;

	//========================================
	/*!\brief Database interface for ProcessingJob's.
	 *///-------------------------------------
	dbaccess::DbInterfaceT<dbaccess::ProcessingJob>::DbIfPtr m_procInterface;

	//========================================
	/*!\brief Result iterator for ProcessingJob's
	 *///-------------------------------------
	dbaccess::DbQueryIterator<dbaccess::ProcessingJob> m_procItr;
}; //TripDbReader

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // TRIPDBREADER_HPP_SEEN

//======================================================================
