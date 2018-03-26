//======================================================================
/*! \file MongoDbConnection.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 10, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef MONGODBCONNECTION_HPP_SEEN
#define MONGODBCONNECTION_HPP_SEEN

//======================================================================

#include <ibeosdk/database/DbConnection.hpp>

#ifdef __linux__
#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wextra"
#endif // _linux
#ifdef _WIN32
#include <WinSock2.h>
#include <windows.h>
#endif // _WIN32
#include <mongo/client/dbclient.h>
#ifdef __linux__
#	pragma GCC diagnostic pop
#endif // _linux

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

template<class T> class MongoDbInterfaceT;

//========================================
/*!\brief Boost shared pointer for mongo::DBClientCursor
 *///-------------------------------------
typedef boost::shared_ptr<mongo::DBClientCursor> MongoCursorPtr;

//======================================================================
/*!\class MongoDbConnection
 * \brief Derived DbConnection implementation for connecting to MongoDB databases.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 10, 2016
 *///------------------------------------------------------------------
class MongoDbConnection : public DbConnection {

public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	MongoDbConnection(const std::string& dbname)
	  : DbConnection(dbname),
	    m_dbConnection(true),
	    m_connectionString()
	{}

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~MongoDbConnection() {}

public:
	//========================================
	/*!\brief Returns the number of database connections
	 *        from mongo::DBClientConnection.
	 *
	 * \return Number of connections as integer
	 *///-------------------------------------
	virtual int nbOfConnections() const ;

	//========================================
	/*!\brief Returns MongoDbConnection::m_dbConnection.
	 *
	 * \return mongo::DBClientConnection standard connection object.
	 *///-------------------------------------
	mongo::DBClientConnection& getDbClientConnection() { return m_dbConnection; }

protected:
	//========================================
	/*!\brief Establishes a database connection using the mongo::DBClientConnection
	 *        which is part of the mongo-cxx-driver.
	 *
	 * \param[in, out] res  Result of the connecting action.
	 * \return \c true, if no error occurred
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool connect(ConnectionResult& res);

private:
	//========================================
	/*!\brief DBClientConnection standard connection
	 *///-------------------------------------
	mongo::DBClientConnection m_dbConnection;

	//========================================
	/*!\brief Result iterator
	 *///-------------------------------------
	std::string m_connectionString;

	template<class T> friend class MongoDbInterfaceT;
	friend class MongoDbObjInterface;
	friend class MongoDbInterface;
}; // MongoDbConnection

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // MONGODBCONNECTION_HPP_SEEN

//======================================================================
