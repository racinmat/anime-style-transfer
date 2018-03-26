//======================================================================
/*! \file DbConnectionFactory.cpp
 *\verbatim
 * ------------------------------------------
 *  (C) 2012 Ibeo Automotive Systems GmbH, Hamburg, Germany
 * ------------------------------------------
 *
 *  Created on: Jul 24, 2014
 *          by: Raymond Schulz
 *\endverbatim
 *///-------------------------------------------------------------------
//======================================================================

#include "DbConnectionFactory.hpp"

#include <ibeosdk/Log.hpp>
#include <iostream>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

DbConnectionFactory::DbConnectionFactory()
  : m_connectionForCache(),
    m_connProps(),
    m_connRes(),
    m_mutexDBConnFactory(),
    m_threadCounter(),
    m_connectionMap(),
    m_maxNumConnections(5),
    m_numUsedConnections(0)
{}

//======================================================================

DbConnectionFactory::~DbConnectionFactory() {}

//======================================================================

bool DbConnectionFactory::connect(const DbConnection::ConnectionProperties& connProps,
                                  DbConnection::ConnectionResult& res)
{
	{ // mutex block
		traceConnectionStatus();
		if (m_numUsedConnections > 0) {
			logWarning << "Connection properties have been changed, "
					<< " but there are unused db connection." << std::endl;
			// Those connections are not updated.
		}
	} // mutex block
	removeUnusedConnections();

	registerConnectionPropertes(connProps);

	m_connectionForCache = getPooledConnection();
	DbConPtr dbConPtr = m_connectionForCache.second.dbConPtr;
	res = m_connRes;
	return dbConPtr->isConnected();
}

//======================================================================

DbConPtr DbConnectionFactory::createDbInstance()
{
	DbConPtr dbCon(new MongoDbConnection("IbeoPP"));
	return dbCon;
}

//======================================================================

void DbConnectionFactory::traceConnectionStatus()
{
	logDebug << "Number of open connections: " << m_connectionMap.size()
			<< ", used: " << m_numUsedConnections << std::endl;
}

//======================================================================

void DbConnectionFactory::removeUnusedConnections()
{
	{ // mutex block
		Mutex::scoped_lock criticalSection (m_mutexDBConnFactory);

		ConnectionMap::iterator itCh = m_connectionMap.begin();
		std::list<ConnectionMap::iterator> iteratorList;

		for (; itCh != m_connectionMap.end(); ++itCh) {
			if (!itCh->second.isInUse) {
				iteratorList.push_back(itCh);
			}
		}

		std::list< ConnectionMap::iterator >::iterator itit = iteratorList.begin();
		for (; itit != iteratorList.end(); ++itit) {
			m_connectionMap.erase(*itit);
		}
	} // mutex block

	traceConnectionStatus();
}

//======================================================================

DbConnectionFactory::ConnIdHandlePair DbConnectionFactory::getPooledConnection()
{
	using namespace boost::tuples;

	ConnIdHandlePair ret;

	// create always first db connection that is always used and can not be returned.
	std::string sThreadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());

	{ // mutex block
		// Thread safety: lock access to members
		Mutex::scoped_lock criticalSection (m_mutexDBConnFactory);

		// @TODO This function returns the connection based on thread id and highest consecutive number,
		// if a connection with a lower number is released, it may never be re-used.
		int threadIdCount = m_threadCounter[sThreadId];

		ConnectionId connId = std::make_pair(sThreadId, threadIdCount);

		{
			logDebug << "Get pooled connection ID: "
					<< sThreadId << ":" << threadIdCount << ": "
					<< std::endl;
		}

		ConnectionMap::iterator itCh = m_connectionMap.find(connId);
		if (itCh == m_connectionMap.end() || (itCh != m_connectionMap.end() && itCh->second.isInUse)) {
			// connection not yet available, create one and connect
			// or connection already in use
			if (m_connectionMap.size() >= m_maxNumConnections) {
				logDebug << "Maximum number of mongo parallel connections reached. Do not care for now. " << std::endl;
				traceConnectionStatus();
			}

			// increment thread counter
			++threadIdCount;
			m_threadCounter[sThreadId] = threadIdCount;

			ConnectionHandle newConnHandle;
			newConnHandle.isInUse = true;
			newConnHandle.dbConPtr = createDbInstance();


			newConnHandle.dbConPtr->connect(m_connProps, m_connRes);

			// create and insert new connectionId and handle to map
			ConnectionId connIdNew = std::make_pair(sThreadId, threadIdCount);
			m_connectionMap[connIdNew] = newConnHandle;

			ret.first = connIdNew;
			ret.second = newConnHandle;

			{
				logDebug << "Connection created with ConnId="
						<< connIdNew.first << ":" << connIdNew.second << "." << std::endl;
			}
		}
		else if (itCh != m_connectionMap.end() && !itCh->second.isInUse) {
			// ok we found one connection, that is not in use. Now check if it used
			// so return old connection and set to "in use"
			ret.first = connId;
			ret.second = itCh->second;
			itCh->second.isInUse = true;
			{
				logDebug << "Return connection with ConnId="
						<< connId.first << ":" << connId.second << std::endl;
			}
		}

		++m_numUsedConnections;
	} // mutex block

	traceConnectionStatus();

	return ret;
}

//======================================================================

void DbConnectionFactory::returnPooledConnection(const ConnectionId& cid, const bool del)
{
	{
		Mutex::scoped_lock criticalSection (m_mutexDBConnFactory);

		ConnectionMap::iterator it = m_connectionMap.find(cid);
		if (it != m_connectionMap.end()) {
			it->second.isInUse = false;
			if (del) {
				m_connectionMap.erase(it);
				// connection should be closed if erased
			}
			--m_numUsedConnections;

			{
				logDebug << "Connection (" << cid.first << ":" << cid.second
						<< ") returned to pool." << std::endl;
			}
		}
		else {
			logDebug << "Connection to be returned (" << cid.first << ":" << cid.second << ")"
					<< " does not exist." << std::endl;
		}
	} // end mutex block

	traceConnectionStatus();

}
//======================================================================
//======================================================================

DbConnectionFactory::ScopedConnection::ScopedConnection()
{
	DbConnectionFactory& inst = DbConnectionFactory::instance();
	m_chandle = inst.getPooledConnection();
}

//======================================================================

DbConnectionFactory::ScopedConnection::~ScopedConnection()
{
	//TODO causing trouble on windows
	//DbConnectionFactory& inst = DbConnectionFactory::instance();
	//inst.returnPooledConnection(m_chandle.first, true);
}

//======================================================================

MongoDbInterface::DbIfPtr DbConnectionFactory::ScopedConnection::getDbIfPtr()
{
	MongoDbInterface::DbIfPtr p;
	p.reset(new MongoDbInterface(m_chandle.second.dbConPtr));
	return p;
}

//======================================================================

MongoDbObjInterface::DbIfPtr DbConnectionFactory::ScopedConnection::getDbObjIfPtr(const CollectionName& procObjectInvariants,
                                                                                  const CollectionName& procObjectTrajData,
                                                                                  bool& collectionCheck)
{
	MongoDbObjInterface::DbIfPtr p;
	p.reset(new MongoDbObjInterface(m_chandle.second.dbConPtr, procObjectInvariants, procObjectTrajData, collectionCheck));
	return p;
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
