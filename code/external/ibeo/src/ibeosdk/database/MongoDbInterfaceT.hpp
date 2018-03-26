//======================================================================
/*! \file MongoDbInterfaceT.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef MONGODBINTERFACET_HPP_SEEN
#define MONGODBINTERFACET_HPP_SEEN

//======================================================================

#include <ibeosdk/database/DbInterfaceT.hpp>
#include <ibeosdk/database/DbQueryIterator.hpp>
#include <ibeosdk/database/DbQueryOptions.hpp>
#include <ibeosdk/database/MongoDbConnection.hpp>
#include <ibeosdk/database/MongoDbUtils.hpp>
#include <ibeosdk/database/datamodel/BsonT.hpp>

#include <ibeosdk/inttypes.hpp>

#include <ibeosdk/Log.hpp>

#include <mongo/client/exceptions.h>

//======================================================================

namespace ibeosdk {
namespace dbaccess {;

//======================================================================
/*!\class MongoQueryIteratorImpl
 * \brief Derived DbQueryIteratorImpl implementation to access query results
 *        received from a MongoDB database.
 * \tparam DataType Type of the data interfaced by this class.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 8, 2016
 *///------------------------------------------------------------------
template<class DataType>
class MongoQueryIteratorImpl : public DbQueryIteratorImpl<DataType> {
public:
	//========================================
	/*!\brief Constructor.
	 *
	 * Created instance calls DbQueryIteratorImpl::next() initially.
	 * \param[in] cursor mongo::DBClientCursor which holds the query results.
	 *///-------------------------------------
	MongoQueryIteratorImpl(MongoCursorPtr cursor)
	  : m_cursor(cursor)
	{
		m_end = false;
		next();
	}

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~MongoQueryIteratorImpl() {}

	//========================================
	/*!\brief Creates a new DataType object from existing result cursor.
	 *
	 * A new object is only created if the cursor hasn't reached his end.
	 *///-------------------------------------
	virtual void next()
	{
		if (!m_end) {
			m_end = !m_cursor->more();
			if (!m_end) {
				mongo::BSONObj bo = m_cursor->next();
				if (!bo.isEmpty()) {
					BsonT<DataType>::createDataType(m_data, bo);
				}
			}
		}
	}

	virtual bool end() const { return m_end; }
	virtual DataType const& get() const { return m_data; }

private:
	//========================================
	/*!\brief Hidden default constructor.
	 *///-------------------------------------
	MongoQueryIteratorImpl() : m_end(true) {}

	//========================================
	/*!\brief Hidden copy constructor.
	 *///-------------------------------------
	MongoQueryIteratorImpl(MongoQueryIteratorImpl const&) : m_end(true) {}

private:
	//========================================
	/*!\brief Indicates if the end of the mongo::DBClientCursor was reached.
	 *///-------------------------------------
	bool m_end;

	//========================================
	/*!\brief Holds the created object from type Datatype.
	 *///-------------------------------------
	DataType m_data;

	//========================================
	/*!\brief mongo::DBClientCursor which holds the query results.
	 *///-------------------------------------
	MongoCursorPtr m_cursor;
}; //MongoQueryIteratorImpl

//======================================================================
//======================================================================
//======================================================================

//======================================================================
//======================================================================
//======================================================================
/*!\class MongoDbInterfaceT
 * \brief Derived implementation of DbInterfaceT to support queries
 *        to MongoDB databases.
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \tparam DataType Type of the data interfaced by this class.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 4, 2016
 *///------------------------------------------------------------------
template<class DataType>
class MongoDbInterfaceT : public DbInterfaceT<DataType> {
public:
	//========================================
	/*!\brief Constructor
	 *
	 * Created MongoDbInterfaceT will initially check if given collection is present.
	 * \param[in] dbConnection      Already existing database connection.
	 * \param[in] collectionName    Collection to connect to.
	 * \param[out] collectionCheck  Tells if the collection is present in the database.
	 *///-------------------------------------
	MongoDbInterfaceT(const DbConPtr dbConnection, const CollectionName& collectionName, bool& collectionCheck)
	  : DbInterfaceT<DataType>(dbConnection, collectionName),
	    m_mdbConnection(dynamic_cast<MongoDbConnection*>(dbConnection.get()))
	{
		collectionCheck = MongoDbUtils::collectionExists(m_mdbConnection->m_dbConnection, collectionName);
	}

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~MongoDbInterfaceT() {}

public:
	//========================================
	/*!\brief Perform a query on a database.
	 *
	 * From the query options, the limit and batch size parameters are ignored
	 * (both are set to 1 instead). All other query options are considered.
	 *
	 * \param[in]  options  Specified DbQueryOptions.
	 * \param[out] data     Resulting DataType with information received
	 *                      from the database.
	 * \return \c true if query was successful, \c false otherwise.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	virtual bool queryOne(const DbQueryOptions& options, DataType& data);

	//========================================
	/*!\brief Perform a query on a database.
	 *
	 * Return first item matching query.
	 *
	 * \note This is a workaround to support MongoDB versions < 3.0.
	 *
	 * \param[in]  query    Query string.
	 * \param[out] data     Resulting DataType with information received
	 *                      from the database.
	 * \return \c true if query was successful, \c false otherwise.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	virtual bool queryOne(const std::string& query, DataType& data);

	//========================================
	/*!\brief Perform a query on a database.
	 *
	 * All query options are considered, including match criteria, order-by,
	 * limit number, skip number, batch size.
	 *
	 * \param[in]  options  Specified DbQueryOptions.
	 * \return Iterator to all items matching specified query.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	virtual DbQueryIterator<DataType> queryData(const DbQueryOptions& options);

	//========================================
	/*!\brief Perform a fast query on a database.
	 *
	 * \param[in]  options  Specified DbQueryOptions.
	 * \return mongo::DBClientcursor (boost shared pointer) to all items
	 *         matching specified query.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	virtual MongoCursorPtr fastQuery(const DbQueryOptions& options);

	//========================================
	/*!\brief Perform a query on a database to count the number of result elements.
	 *
	 * From the query options, the order-by and batch size parameters are ignored.
	 *
	 * \param[in]  options  Specified DbQueryOptions.
	 * \return Number of items matching specified query.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	virtual UINT64 countData(const DbQueryOptions& options);

//public:
//	virtual bool createIndex(const std::string& fullDBName, const std::string& fieldname)
//	{
//		bool success = false;
//
//		try{
//			m_mdbConnection->m_dbConnection.createIndex(fullDBName, BSON(fieldname << 1));
//			 success = true;
//		} catch(mongo::OperationException& e){
//			logError << e.what() << std::endl;
//		}
//
//		return success;
//	}

protected:
	//========================================
	/*!\brief Holds the database connection.
	 *///-------------------------------------
	MongoDbConnection* const m_mdbConnection;
}; // MongoDbInterfaceT

//======================================================================
//======================================================================
//======================================================================

template<class DataType>
bool MongoDbInterfaceT<DataType>::queryOne(const DbQueryOptions& options, DataType& data)
{
	try {
		const mongo::Query dBqueryOptions = MongoDbUtils::convertDbQueryOptionsToFullQuery(options);

		const int nToReturn = 1;
		const int nToSkip = options.getSkip();
		const mongo::BSONObj* fieldsToReturn = NULL;
		const int queryOptions = 0;

		{
			logDebug << "queryOne(" << this->m_collectionName << "): nToSkip: " << nToSkip << " query: " << dBqueryOptions.toString() << std::endl;
		}

		MongoCursorPtr cursor = MongoCursorPtr(m_mdbConnection->m_dbConnection.query(this->m_collectionName,
		                                                                             dBqueryOptions, nToReturn,
		                                                                             nToSkip, fieldsToReturn,
		                                                                             queryOptions));

		if (cursor->more()) {
			mongo::BSONObj bsonObj = cursor->next();

			if ( !bsonObj.isEmpty()) {
				BsonT<DataType>::createDataType(data, bsonObj);
				return true;
			}
		}
	}
	catch (const mongo::AssertionException& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	}
	return false;
}

//======================================================================

template<class DataType>
bool MongoDbInterfaceT<DataType>::queryOne(const std::string& query, DataType& data)
{
	try {
		mongo::BSONObj bsonObj = m_mdbConnection->m_dbConnection.findOne(this->m_collectionName, query);

		if ( !bsonObj.isEmpty()) {
			BsonT<DataType>::createDataType(data, bsonObj);
			return true;
		}
	}
	catch (const mongo::AssertionException& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	}
	return false;
}

//======================================================================

template<class DataType>
DbQueryIterator<DataType> MongoDbInterfaceT<DataType>::queryData(const DbQueryOptions& options)
{
	const mongo::Query query = MongoDbUtils::convertDbQueryOptionsToFullQuery(options);

	const int nToReturn = options.getLimit();
	const int nToSkip = options.getSkip();
	const mongo::BSONObj fieldsToReturn = MongoDbUtils::convertReturnFields(options);
	const int batchSize = options.getBatchSize();
	const int queryOptions = 0;

	{
		logDebug << "queryData(" << this->m_collectionName << "): nToReturn: " << nToReturn
				<< ", nToSkip: " << nToSkip << ", batchSize: " << batchSize
				<< " query: " << query.toString() << ", fields: " << fieldsToReturn.toString() << std::endl;
	}

	try {
		MongoCursorPtr cursor = MongoCursorPtr(m_mdbConnection->m_dbConnection.query(this->m_collectionName,
		                                                                             query,
		                                                                             nToReturn,
		                                                                             nToSkip,
		                                                                             &fieldsToReturn,
		                                                                             queryOptions,
		                                                                             batchSize));

		MongoQueryIteratorImpl<DataType>* iteratorImpl = new MongoQueryIteratorImpl<DataType>(cursor);
		DbQueryIterator<DataType> iterator = DbQueryIterator<DataType>(iteratorImpl);
		return iterator;
	}
	catch (const mongo::AssertionException& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	}
}

//======================================================================

template<class DataType>
MongoCursorPtr MongoDbInterfaceT<DataType>::fastQuery(const DbQueryOptions& options)
{
	const mongo::Query query = MongoDbUtils::convertDbQueryOptionsToFullQuery(options);

	const int nToReturn = options.getLimit();
	const int nToSkip = options.getSkip();
	const mongo::BSONObj fieldsToReturn = MongoDbUtils::convertReturnFields(options);
	const int batchSize = options.getBatchSize();
	const int queryOptions = 0;

	try {
		return MongoCursorPtr(m_mdbConnection->m_dbConnection.query(this->m_collectionName,
		                                                            query, nToReturn,
		                                                            nToSkip, &fieldsToReturn,
		                                                            queryOptions, batchSize));
	}
	catch (const mongo::AssertionException& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	}
}

//======================================================================

template<class DataType>
UINT64 MongoDbInterfaceT<DataType>::countData(const DbQueryOptions& options)
{
	const mongo::Query query = MongoDbUtils::convertDbQueryOptionsToQuery(options);

	const int nToReturn = options.getLimit();
	const int nToSkip = options.getSkip();
	const int queryOptions = 0;

	{
		logDebug << "countData(" << this->m_collectionName << "): nToReturn: " << nToReturn << ", nToSkip: " << nToSkip
				<< " query: " << query.obj.toString() << std::endl;
	}

	try {
		const UINT64 result = m_mdbConnection->m_dbConnection.count(this->m_collectionName, query.obj, queryOptions, nToReturn, nToSkip);
		return result;
	}
	catch (const mongo::UserException& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	}
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // MONGODBINTERFACET_HPP_SEEN

//======================================================================
