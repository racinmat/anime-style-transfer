//======================================================================
/*! \file MongoDbInterface.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 4, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef MONGODBINTERFACE_HPP_SEEN
#define MONGODBINTERFACE_HPP_SEEN

//======================================================================

#include <ibeosdk/database/MongoDbConnection.hpp>
#include <ibeosdk/database/MongoDbUtils.hpp>
#include <ibeosdk/database/DbQueryOptions.hpp>
#include <ibeosdk/database/CollectionName.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================
/*!\class MongoDbInterface
 * \brief General class for wrapping some query functionality from the mongo-cxx-driver.
 *
 * All queries will be performed with the mongo::DBClientConnection,
 * which is a member of the MongoDbConnection class.
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 4, 2016
 *///------------------------------------------------------------------
class MongoDbInterface {
public:
	//========================================
	/*!\brief Boost shared pointer for MongoDbInterface
	 *///-------------------------------------
	typedef boost::shared_ptr<MongoDbInterface> DbIfPtr;

public:
	//========================================
	/*!\brief Creates MongoDbInterface with given DbConnection.
	 *///-------------------------------------
	MongoDbInterface(const DbConPtr dbConnection);

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~MongoDbInterface() {};

public:
	//========================================
	/*!\brief Executes a query against a CollectionName and given DbQueryOptions.
	 *
	 * Return first item matching query.
	 *
	 * Uses MongoDbInterface::queryOne(const CollectionName& ns, const mongo::Query& query,
	 *                                 const mongo::BSONObj* fieldsToReturn,
	 *                                 const int queryOptions) internally.
	 *
	 * \param[in] ns       The CollectionName, where the query should be executed.
	 * \param[in] options  Specified DbQueryOptions.
	 * \return The result of the query as a mongo::BSONObj.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	mongo::BSONObj queryOne(const CollectionName& ns, const DbQueryOptions& options = DbQueryOptions());

	//========================================
	/*!\brief Executes a query against a CollectionName and given parameters
	 *
	 * Return first item matching query.
	 *
	 * \param[in] ns              The CollectionName, where the query should be executed.
	 * \param[in] query           Holds the actual MongoDB query.
	 * \param[in] fieldsToReturn  Describes the fields, the database will add
	 *                            to the resulting mongo::BSONObj.
	 * \param[in] queryOptions    QueryOptions - see mongo::QueryOptions
	 *                            in dbclientinterface.h
	 * \return The result of the query as a mongo::BSONObj.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	mongo::BSONObj queryOne(const CollectionName& ns,
	                        const mongo::Query& query,
	                        const mongo::BSONObj* fieldsToReturn = 0,
	                        const int queryOptions = 0);

	//========================================
	/*!\brief Executes a query against a CollectionName and given DbQueryOptions.
	 *
	 * Uses MongoDbInterface::query(const CollectionName& ns, const mongo::Query& query,
	 *                              const int nToReturn, const int nToSkip,
	 *                              const mongo::BSONObj* fieldsToReturn,
	 *                              const int queryOptions, const int batchSize)
	 *
	 * \param[in] ns       The CollectionName, where the query should be executed.
	 * \param[in] options  Specified DbQueryOptions.
	 * \return mongo::DBClientCursor as a MongoCursorPtr, which holds the query results.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	MongoCursorPtr query(const CollectionName& ns, const DbQueryOptions& options = DbQueryOptions());

	//========================================
	/*!\brief Executes a query against a CollectionName and given DbQueryOptions.
	 *
	 * \param[in] ns              The CollectionName, where the query should be executed.
	 * \param[in] query           Holds the actual MongoDB query.
	 * \param[in] nToReturn       How many results should be returned.
	 * \param[in] nToSkip         How many results should be skipped.
	 * \param[in] fieldsToReturn  Describes the fields, the database will add
	 *                            to the resulting mongo::BSONObj.
	 * \param[in] queryOptions    QueryOptions - see mongo::QueryOptions
	 *                            in dbclientinterface.h
	 * \param[in] batchSize       Specifies the number of documents to return
	 *                            in each batch of the response .
	 * \return mongo::DBClientCursor as a MongoCursorPtr, which holds the query results.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	MongoCursorPtr query(const CollectionName& ns,
	                     const mongo::Query& query,
	                     const int nToReturn = 0,
	                     const int nToSkip = 0,
	                     const mongo::BSONObj* fieldsToReturn = 0,
	                     const int queryOptions = 0,
	                     const int batchSize = 0);

	//========================================
	/*!\brief Executes a query and counts the number of result elements.
	 *
	 * Uses MongoDbInterface::count(const CollectionName& ns, const mongo::Query& query,
	 *                              const int queryOptions, const int nToReturn,
	 *                              const int nToSkip)
	 *
	 * \param[in] ns       The CollectionName, where the query should be executed.
	 * \param[in] options  Specified DbQueryOptions.
	 * \return Number of matching elements as UINT64.
	 *///-------------------------------------
	UINT64 count(const CollectionName& ns, const DbQueryOptions& options = DbQueryOptions());

	//========================================
	/*!\brief Executes a query and counts the number of result elements.
	 *
	 * \param[in] ns              The CollectionName, where the query should be executed.
	 * \param[in] query           Holds the actual MongoDB query.
	 * \param[in] queryOptions    QueryOptions - see mongo::QueryOptions
	 *                            in dbclientinterface.h
	 * \param[in] nToReturn       How many results should be returned.
	 * \param[in] nToSkip         How many results should be skipped.
	 * \return Number of matching elements as UINT64.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	UINT64 count(const CollectionName& ns,
	             const mongo::Query& query,
	             const int queryOptions = 0,
	             const int nToReturn = 0,
	             const int nToSkip = 0);

	//========================================
	/*!\brief Returns a list of all database names inside one MongoDB database.
	 *
	 * \return List of database names.
	 * \throw DatabaseException if an error occurs
	 *///-------------------------------------
	std::list<std::string> getDatabaseNames();

	//========================================
	/*!\brief Returns a constant reference to MongoDbInterface::m_mDbConnection.
	 *
	 * \return MongoDbConnection reference.
	 *///-------------------------------------
	const MongoDbConnection& getDbConnection() const { return *m_mDbConnection; }

public:
	//========================================
	/*!\brief Pointer to MongoDbConnection, for query execution.
	 *///-------------------------------------
	MongoDbConnection* const m_mDbConnection;
}; // MongoDbInterface

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // MONGODBINTERFACE_HPP_SEEN

//======================================================================
