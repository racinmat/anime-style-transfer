//======================================================================
/*! \file MongoDbInterface.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 4, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/MongoDbInterface.hpp>
#include <ibeosdk/database/MongoDbUtils.hpp>
#include <ibeosdk/database/DatabaseException.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

MongoDbInterface::MongoDbInterface(const DbConPtr dbConnection)
  : m_mDbConnection(dynamic_cast<MongoDbConnection*>(dbConnection.get()))
{}

//======================================================================

mongo::BSONObj MongoDbInterface::queryOne(const CollectionName& ns, const DbQueryOptions& options)
{
	const mongo::Query query = MongoDbUtils::convertDbQueryOptionsToFullQuery(options);

	const mongo::BSONObj fieldsToReturn = MongoDbUtils::convertReturnFields(options);
	int queryOptions = 0; // TODO not yet implemented

	{
		logDebug << "queryData(" << ns << "): nToReturn(ignored): " << options.getLimit()
		         << ", nToSkip(ignored): " << options.getSkip() << ", batchSize(ignored): " << options.getBatchSize()
		         << " query: " << query.toString() << ", fields: " << fieldsToReturn.toString() << std::endl;
				// TODO, queryOptions
	}

	return queryOne(ns, query, &fieldsToReturn, queryOptions);
}

//======================================================================

mongo::BSONObj MongoDbInterface::queryOne(const CollectionName& ns,
                                          const mongo::Query& query,
                                          const mongo::BSONObj* fieldsToReturn,
                                          const int queryOptions)
{
	try {
		return m_mDbConnection->m_dbConnection.findOne(ns, query, fieldsToReturn, queryOptions);
	}
	catch (const mongo::AssertionException& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	}
}

//======================================================================

MongoCursorPtr MongoDbInterface::query(const CollectionName& ns, const DbQueryOptions& options)
{
	const mongo::Query query = MongoDbUtils::convertDbQueryOptionsToFullQuery(options);

	const int nToReturn = options.getLimit();
	const int nToSkip = options.getSkip();
	const mongo::BSONObj fieldsToReturn = MongoDbUtils::convertReturnFields(options);
	const int batchSize = options.getBatchSize();
	const int queryOptions = 0; // TODO not yet implemented

	{
		logDebug << "queryData(" << ns << "): nToReturn: " << nToReturn
				<< ", nToSkip: " << nToSkip << ", batchSize: " << batchSize
				<< " query: " << query.toString() << ", fields: " << fieldsToReturn.toString() << std::endl;
				// TODO, queryOptions
	}

	try {
		return MongoCursorPtr(m_mDbConnection->m_dbConnection.query(ns,
		                                                            query,
		                                                            nToReturn,
		                                                            nToSkip,
		                                                            &fieldsToReturn,
		                                                            queryOptions,
		                                                            batchSize));
	}
	catch (const mongo::AssertionException& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	}
}

//======================================================================

MongoCursorPtr MongoDbInterface::query(const CollectionName& ns,
                                       const mongo::Query& query,
                                       const int nToReturn,
                                       const int nToSkip,
                                       const mongo::BSONObj* fieldsToReturn,
                                       const int queryOptions,
                                       const int batchSize)
{
	try {
		return MongoCursorPtr(m_mDbConnection->m_dbConnection.query(ns,
		                                                            query,
		                                                            nToReturn,
		                                                            nToSkip,
		                                                            fieldsToReturn,
		                                                            queryOptions,
		                                                            batchSize));
	}
	catch (const mongo::AssertionException& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	}
}

//======================================================================

UINT64 MongoDbInterface::count(const CollectionName& ns, const DbQueryOptions& options)
{
	const mongo::Query query = MongoDbUtils::convertDbQueryOptionsToQuery(options);

	const int nToReturn = options.getLimit();
	const int nToSkip = options.getSkip();
	const int queryOptions = 0; // TODO

	{
		logDebug << "countData(" << ns << "): nToReturn: " << nToReturn << ", nToSkip: " << nToSkip
		         << " query: " << query.obj.toString() << std::endl;
		// TODO queryOptions
	}

	UINT64 result = count(ns, query.obj, queryOptions, nToReturn, nToSkip);

	logDebug << "Mongo count: " << result << std::endl;

	return result;
}

//======================================================================

UINT64 MongoDbInterface::count(const CollectionName& ns,
                               const mongo::Query& query,
                               const int queryOptions,
                               const int nToReturn,
                               const int nToSkip)
{
	try {
		return m_mDbConnection->m_dbConnection.count(ns, query.obj, queryOptions, nToReturn, nToSkip);
	}
	catch (const mongo::UserException& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	}
}

//======================================================================

std::list<std::string> MongoDbInterface::getDatabaseNames()
{
	try {
		return m_mDbConnection->m_dbConnection.getDatabaseNames();
	}
	catch(const std::exception& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	}
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
