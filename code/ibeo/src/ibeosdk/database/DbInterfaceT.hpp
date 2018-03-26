//======================================================================
/*! \file DbInterfaceT.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef DBINTERFACET_HPP_SEEN
#define DBINTERFACET_HPP_SEEN

//======================================================================

#include <ibeosdk/database/DbConnection.hpp>
#include <ibeosdk/database/DbQueryOptions.hpp>
#include <ibeosdk/database/DbQueryIterator.hpp>
#include <ibeosdk/database/CollectionName.hpp>
#include <ibeosdk/database/MongoDbConnection.hpp>
#include <ibeosdk/database/DatabaseException.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================
/*!\class DbInterfaceT
 * \brief Templated virtual class to warp some query functionality from the mongo-cxx-driver
 *        with different object types.
 * \tparam DataType Type of the data interfaced by this class.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 9, 2016
 *///------------------------------------------------------------------
template<class DataType>
class DbInterfaceT {
public:
	//========================================
	/*!\brief Boost shared pointer for DbInterfaceT
	 *///-------------------------------------
	typedef boost::shared_ptr<DbInterfaceT> DbIfPtr;

public:
	DbInterfaceT(const DbConPtr dbConnection, const CollectionName& collectionName)
	  : m_dbConnection(dbConnection),
	    m_collectionName(collectionName)
	{}

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~DbInterfaceT() {}

public:
	virtual DbQueryIterator<DataType> queryData()
	{
		DbQueryOptions options;
		return queryData(options);
	}

	//========================================
	/*!\brief Perform a query on a database.
	 *
	 * Return first item matching query.
	 *
	 * \param[in]  options  Specified DbQueryOptions.
	 * \param[out] data     Resulting DataType with information received
	 *                      from the database.
	 * \return \c true if query was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool queryOne(const DbQueryOptions& options, DataType& data) = 0;

	//========================================
	/*!\brief Perform a query on a database.
	 *
	 * Return first item matching query.
	 *
	 * \param[in]  query    Query string.
	 * \param[out] data     Resulting DataType with information received
	 *                      from the database.
	 * \return \c true if query was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool queryOne(const std::string& query, DataType& data) = 0;

	//========================================
	/*!\brief Perform a query on a database.
	 *
	 * \param[in]  options  Specified DbQueryOptions.
	 * \return Iterator to all items matching specified query.
	 *///-------------------------------------
	virtual DbQueryIterator<DataType> queryData(const DbQueryOptions& options) = 0;

	//========================================
	/*!\brief Perform a query on a database.
	 *
	 * \param[in]  options  Specified DbQueryOptions.
	 * \return mongo::DBClientcursor (boost shared pointer) to all items
	 *         matching specified query.
	 *///-------------------------------------
	virtual MongoCursorPtr fastQuery(const DbQueryOptions& options = DbQueryOptions()) = 0;

	//========================================
	/*!\brief Perform a query on a database to count the number of result elements.
	 *
	 * \param[in]  options  Specified DbQueryOptions.
	 * \return Number of items matching specified query.
	 *///-------------------------------------
	virtual UINT64 countData(const DbQueryOptions& options) = 0;

//public:
//	virtual bool createIndex(const std::string& fullDBName, const std::string& fieldname) { return false; };

public:
	//========================================
	/*!\brief Returns status of the database connection.
	 *
	 * \return \c true if connection to database is established
	 *         \c false otherwise.
	 *///-------------------------------------
	bool isConnected() const { return m_dbConnection->isConnected(); }

	//========================================
	/*!\brief Returns connection properties.
	 *
	 * \return DbConnection::ConnectionProperties
	 *///-------------------------------------
	DbConnection::ConnectionProperties& getConnectionProperties() { return m_dbConnection->getConnectionProperties(); }

	//========================================
	/*!\brief Returns connection properties.
	 *
	 * \return DbConnection::ConnectionProperties
	 *///-------------------------------------
	const DbConnection::ConnectionProperties& getConnectionProperties() const { return m_dbConnection->getConnectionProperties(); }

	//========================================
	/*!\brief Returns the database connection.
	 *
	 * \return DbConPtr shared pointer to DbConnection.
	 *///-------------------------------------
	const DbConPtr getDbConnection() const { return m_dbConnection; }

	//========================================
	/*!\brief Returns the collection name this interface points to.
	 *
	 * \return Collection as a CollectionName.
	 *///-------------------------------------
	const CollectionName& getCollectionName() const { return m_collectionName; }


protected:
	//========================================
	/*!\brief Holds the database connection.
	 *///-------------------------------------
	const DbConPtr m_dbConnection;

	//========================================
	/*!\brief Holds the collection this interface points to.
	 *///-------------------------------------
	CollectionName m_collectionName;
}; // class DbInterfaceT

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // DBINTERFACET_HPP_SEEN

//======================================================================
