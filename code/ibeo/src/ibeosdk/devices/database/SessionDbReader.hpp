//======================================================================
/*! \file SessionDbReader.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 22, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef IBEOSDK_SESSIONDBREADER_HPP_SEEN
#define IBEOSDK_SESSIONDBREADER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/devices/database/DbDataBlockReader.hpp>

#include <ibeosdk/database/DbInterfaceT.hpp>
#include <ibeosdk/database/DbQueryIterator.hpp>
#include <ibeosdk/database/basedatamodel/Session.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class SessionDbReader
 * \brief Independent Reader for Session's. This class is not derived
 *        from DbDataBlockReader.
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 22, 2016
 *///------------------------------------------------------------------
class SessionDbReader
{

public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	SessionDbReader();

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~SessionDbReader() {}

public:
	//========================================
	/*!\brief Connects to given collection.
	 *
	 * Uses DbConnectionFactory to establish a connection with a DbInterfaceT<Session>.
	 *
	 * \param[in] collection  Collection name.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///------------------------------------
	virtual bool connect(const dbaccess::CollectionName& collection);

	//========================================
	/*!\brief Queries all entries from the database
	 * and stores the results to SessionList#m_list
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *///-------------------------------------
	virtual void queryData();

	//========================================
	/*!\brief Queries information for one session.
	 *
	 * The session will not contain all information. It is necessary to call
	 * TripDbReader::fillTrips and ProcessingDbReader::fillProcessings to get detailed
	 * informations about Trip's and executed Processing's.
	 *
	 * \param[in, out] session The Session with specified name.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *///-------------------------------------
	virtual bool querySessionByName(dbaccess::Session& session);

public:
	//========================================
	/*!\brief Returns the DbInterfaceT<Session>::DbIfPtr (boost::shared_ptr).
	 *
	 * With this interface, custom queries to the database can created.
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 *
	 * \return DbInterfaceT<Session>::DbIfPtr The interface pointer.
	 *///-------------------------------------
	virtual dbaccess::DbInterfaceT<dbaccess::Session>::DbIfPtr getDbInterface(){ return m_dbInterface; }

	//========================================
	/*!\brief Returns the DbQueryIterator<Session>.
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 *
	 * \return DbQueryIterator<Session> The result iterator.
	 *///-------------------------------------
	virtual dbaccess::DbQueryIterator<dbaccess::Session>& getResultIterator(){ return m_itr; }

	//========================================
	/*!\brief Returns SessionList#m_list.
	 *
	 * \note By default SessionList#m_list is empty.
	 * Call first SessionList::queryData, to fill the list.
	 *
	 * \return dbaccess::SessionList Constant reference to the list.
	 *///-------------------------------------
	virtual const dbaccess::SessionList&  getSessionList() const { return m_list; }

	//========================================
	/*!\brief Returns SessionList#m_list.
	 *
	 * \note By default SessionList#m_list is empty.
	 * Call first SessionList::queryData, to fill the list.
	 *
	 * \return dbaccess::SessionList Reference to the list.
	 *///-------------------------------------
	virtual dbaccess::SessionList&  getSessionList() { return m_list; }

protected:
	//========================================
	/*!\brief Database interface for Session's.
	 *///-------------------------------------
	dbaccess::DbInterfaceT<dbaccess::Session>::DbIfPtr m_dbInterface;

	//========================================
	/*!\brief Result iterator for Session's
	 *///-------------------------------------
	dbaccess::DbQueryIterator<dbaccess::Session> m_itr;

	//========================================
	/*!\brief List to hold received Session's
	 *///-------------------------------------
	dbaccess::SessionList m_list;
};// SessionDbReader

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SESSIONDBREADER_HPP_SEEN

//======================================================================
