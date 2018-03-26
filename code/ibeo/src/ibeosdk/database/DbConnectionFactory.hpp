//======================================================================
/*! \file DbConnectionFactory.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef DBCONNECTIONFACTORY_HPP_SEEN
#define DBCONNECTIONFACTORY_HPP_SEEN

//======================================================================

#include <ibeosdk/database/CollectionName.hpp>
#include <ibeosdk/database/DbInterfaceT.hpp>
#include <ibeosdk/database/MongoDbConnection.hpp>
#include <ibeosdk/database/MongoDbInterface.hpp>
#include <ibeosdk/database/MongoDbInterfaceT.hpp>
#include <ibeosdk/database/MongoDbObjInterface.hpp>

#include <boost/tuple/tuple.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================
/*!\class DbConnectionFactory
 * \brief Factory class to connect to MongoDB databases and create
 *        interface classes to access the data.
 *
 * This class follows the Singleton design pattern.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 9, 2016
 *///------------------------------------------------------------------
class DbConnectionFactory {
private:
	//========================================
	/*!\brief Define connection id, combined by a unique string (could be a thread id,
	 *  and a running number)
	 *///-------------------------------------
	typedef std::pair<std::string, int> ConnectionId;

	//========================================
	/*!\brief Shared pointer for DbConnection
	 *///-------------------------------------
	typedef boost::shared_ptr<DbConnection> DbIfPtr;

	//========================================
	/*!\brief Define handle of connection: Pointer to connection and a bool
	 *  if this connection is currently in use and not yet given back to the pool.
	 *///-------------------------------------
	struct ConnectionHandle {
		DbConPtr dbConPtr;
		bool isInUse;
	};

	//========================================
	/*!\brief Count number of connection for each thread represented by
	 *        string with boost::thread_id
	 *///-------------------------------------
	typedef std::map<std::string, int> ThreadCountingMap;

	//========================================
	/*!\brief Define connection id, combined with a ConnectionHandle
	 *///-------------------------------------
	typedef std::pair<ConnectionId, ConnectionHandle> ConnIdHandlePair;

	//========================================
	/*!\brief Map connection id to actual connection.
	 *///-------------------------------------
	typedef std::map<ConnectionId, ConnectionHandle> ConnectionMap;

	//========================================
	/*!\brief The type of mutex that is used internally by the Manager.
	 *///-------------------------------------
	typedef boost::recursive_mutex Mutex;

public:
	//========================================
	/*! ScopedConnection create connection from connection pool in ConnectionFactory
	 *  that is release after the instance goes out of scope.
	 *  If destructor is called the connection is returned to ConnectionFactory.
	 *
	 * Hint: Prerequisites: Before using the scoped connection the connection properties
	 * need to be registered.
	 *///-------------------------------------
	class ScopedConnection {
	public:
		//========================================
		/*!\brief Default constructor.
		 *///-------------------------------------
		ScopedConnection();

		//========================================
		/*!\brief Default destructor.
		 *///-------------------------------------
		~ScopedConnection();

	public:
		//========================================
		/*!\brief Returns DbInterfaceT (shared pointer) from type DataType
		 * \param[in]  collectionName   Collection this interface should point to.
		 * \param[out] collectionCheck  Tells if the collection is present in the database.
		 * \tparam D Type of the data interfaced by this function.
		 * \return Created interface.
		 *///-------------------------------------
		template<class D>
		typename DbInterfaceT<D>::DbIfPtr getDbIfPtr(const CollectionName& collectionName, bool& collectionCheck);

		//========================================
		/*!\brief Returns MongoDbObjInterface (shared pointer).
		 * \param[in]  collectionNameProcObjectInvariants  Collection to connect to.
		 *                                                 In this collection the invariant object
		 *                                                 data is stored.
		 * \param[in]  collectionNameProcObjectTrajData    Collection to connect to.
	 *                                                     In this collection the object trajectories
	 *                                                     are stored.
		 * \param[out] collectionCheck  Tells if the collection is present in the database.
		 * \return Created interface.
		 *///-------------------------------------
		MongoDbObjInterface::DbIfPtr getDbObjIfPtr(const CollectionName& collectionNameProcObjectInvariants,
		                                           const CollectionName& collectionNameProcObjectTrajData,
		                                           bool& collectionCheck);

		//========================================
		/*!\brief Returns MongoDbInterface (shared pointer).
		 * \return Created interface.
		 *///-------------------------------------
		MongoDbInterface::DbIfPtr getDbIfPtr();

	private:
		ConnIdHandlePair m_chandle;
	}; // ScopedConnection

public:
	//========================================
	/*!\brief Returns the Singleton instance
	 * \return DbConnectionFactory instance
	 *///-------------------------------------
	static DbConnectionFactory& instance()
	{
		static DbConnectionFactory theInstance;
		return theInstance;
	}

private:
	DbConnectionFactory();
	DbConnectionFactory(const DbConnectionFactory&);
	DbConnectionFactory& operator=(const DbConnectionFactory&);

public:
	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~DbConnectionFactory();

public:
	//========================================
	/*!\brief Sets the connection properties
	 * \param[in] cp  Existing connection properties.
	 *///-------------------------------------
	void registerConnectionPropertes(const DbConnection::ConnectionProperties& cp) { m_connProps = cp; }

	//========================================
	/*!\brief Establishes a database connection.
	 *
	 * This function needs to be called once before performing any query.
	 *
	 * \param[in]  connProps  Existing connection properties.
	 * \param[out] res        Connection result.
	 * \return \c true if DbConnectionFactory was connected successfully to the database,
	 *         \c false otherwise.
	 *///-------------------------------------
	bool connect(const DbConnection::ConnectionProperties& connProps, DbConnection::ConnectionResult& res);

	//========================================
	/*!\brief Returns the last connection result
	 * \return ConnectionResult with status about the last performed connection
	 *///-------------------------------------
	const DbConnection::ConnectionResult& getLastConnectionResult() { return m_connRes; }

public:
	//========================================
	/*!\brief Creates a new MongoDbConnection
	 * \return Created MongoDbConnection
	 *///-------------------------------------
	void removeUnusedConnections();

private:
	//========================================
	/*!\brief Creates a new MongoDbConnection
	 * \return Created MongoDbConnection
	 *///-------------------------------------
	DbConPtr createDbInstance();

	//========================================
	/*!Return connection from pool (map). The connection has an identifier
	 * <code>ConnectionID</code>. This function creates the ConnectionID based on
	 * thread id a sequential number. If the connection is already created it is given to
	 * the caller within <code>ConnIdHandlePair</code> that contains a shared pointer of the connection.
	 * If the connection is not created it is created within this function and returned.
	 *
	 * \note Hint: The connection needs to be returned to the pool after us, otherwise
	 *       it is kept open till the end of the program. Best practice is to use the
	 *       class <code>ScopedConnection</code> that handles getting and returning the
	 *       connection.
	 *///-------------------------------------
	ConnIdHandlePair getPooledConnection();

	//========================================
	/*!\brief Connection from pool is returned back.
	 * \param[out] cid  Connection ID of the connection to be returned.
	 * \param[in]  del  Iff \c true the connection is also closed and deleted from pool
	 *                  Otherwise it is kept open.
	 *///-------------------------------------
	void returnPooledConnection(const ConnectionId& cid, const bool del = false);

	void traceConnectionStatus();

private:
	ConnIdHandlePair m_connectionForCache;

private:
	DbConnection::ConnectionProperties m_connProps;
	DbConnection::ConnectionResult m_connRes;

private:
	//========================================
	/*!\brief Mutex for thread-safe access
	 * \note The keyword \c mutable is necessary here to make the
	 * mutex work in the const member functions, see FAQ #7 in the
	 * documentation of Boost.threads: "How can you lock a mutex member
	 * in a const member function?" See
	 * - offline (needs package \c libboost-doc installed):
	 *   /usr/share/doc/libboost-doc/HTML/doc/html/threads/faq.html
	 * - online: http://www.boost.org/doc/html/thread/faq.html
	 *///-------------------------------------
	mutable Mutex m_mutexDBConnFactory;

	//========================================
	/*!\brief Count number of connection for each thread represented by
	 *        string with boost::thread_id.
	 *///-------------------------------------
	ThreadCountingMap m_threadCounter;

	//========================================
	/*!\brief Map to store different connection identifier and associated
	 *        ConnectionHandle's
	 *///-------------------------------------
	ConnectionMap m_connectionMap;

	//========================================
	/*!\brief Holds the maximum value of allowed connections.
	 *///-------------------------------------
	UINT16 m_maxNumConnections;

	//========================================
	/*!\brief Holds the number of unused connections.
	 *///-------------------------------------
	int m_numUsedConnections;
}; // DbConnectionFactory

//======================================================================

template<class T>
inline
typename DbInterfaceT<T>::DbIfPtr DbConnectionFactory::ScopedConnection::getDbIfPtr(const CollectionName& collectionName,
                                                                                    bool& collectionCheck)
{
	typename DbInterfaceT<T>::DbIfPtr p;
	p.reset(new MongoDbInterfaceT<T>(m_chandle.second.dbConPtr, collectionName, collectionCheck));
	return p;
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // DBCONNECTIONFACTORY_HPP_SEEN

//======================================================================
