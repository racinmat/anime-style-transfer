//======================================================================
/*! \file DbConnection.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef DBCONNECTION_HPP_SEEN
#define DBCONNECTION_HPP_SEEN

//======================================================================

#include <ibeosdk/inttypes.hpp>

#include <boost/date_time.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>
#include <list>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================
/*!\class DbConnection
 * \brief Virtual class to represent a database connection with connection status,
 *        properties, result and the actual database name.
 *
 *        A derived class needs to implement DbConnection::nbOfConnections() and
 *        DbConnection::connect(ConnectionResult& result).
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 9, 2016
 *///-------------------------------------------------------------------
class DbConnection {
public:
	//======================================================================
	/*!\class ConnectionResult
	 * \brief Connection result
	 * \author Kristian Bischoff (kb)
	 * \version 0.1
	 * \date Feb 9, 2016
	 *///-------------------------------------------------------------------
	class ConnectionResult {
	public:
		//========================================
		/*!\brief Enumeration for connection errors.
		 *///-------------------------------------
		enum RfDbErr {
			RfDb_NoError = 0,
			RfDb_ExceptionRaised = 1, // see error message
			RfDb_ConnectionError = 2  // see db specific error code or error message string
		};

	public:
		//========================================
		/*!\brief Default constructor.
		 *///-------------------------------------
		ConnectionResult()
		  : m_ibeoRfdbErrCode(RfDb_NoError),
		    m_dbErrorCode(0),
		    m_dbErrMsg()
		{}

	public:
		//========================================
		/*!\brief RfDbErr code.
		 *///-------------------------------------
		RfDbErr m_ibeoRfdbErrCode;

		//========================================
		/*!\brief General error code
		 *///-------------------------------------
		int m_dbErrorCode;

		//========================================
		/*!\brief Holds detailed information about the error code.
		 *///-------------------------------------
		std::string m_dbErrMsg;
	}; // ConnectionResult
	//========================================

	//========================================
	/*!\class ConnectionProperties
	 * \brief Connection properties
	 * \author Kristian Bischoff (kb)
	 * \version 0.1
	 * \date Apr 22, 2016
	 *///-------------------------------------
	struct ConnectionProperties {
	public:
		//========================================
		/*!\brief Default constructor.
		 *///-------------------------------------
		ConnectionProperties()
		  : m_isConnected(false),
		    m_dbUser(""),
		    m_dbPassword(""),
		    m_dbIp(""),
		    m_dbPort(0)
		{}

		//========================================
		/*!\brief Creates ConnectionProperties with given parameters
		 *
		 * \param[in] dbuser  User name for authentication
		 * \param[in] dbpwd   Password for authentication
		 * \param[in] dbip    Database ip
		 * \param[in] dbport  Database port
		 *///-------------------------------------
		ConnectionProperties(const std::string& dbuser,
		                     const std::string dbpwd,
		                     const std::string dbip,
		                     const unsigned short dbport)
		  : m_isConnected(false),
		    m_dbUser(dbuser),
		    m_dbPassword(dbpwd),
		    m_dbIp(dbip),
		    m_dbPort(dbport)
		{}

	public:
		//========================================
		/*!\brief Tells if a connection is established.
		 *///-------------------------------------
		bool m_isConnected;

		//========================================
		/*!\brief Holds the login user name.
		 *///-------------------------------------
		std::string m_dbUser;

		//========================================
		/*!\brief Holds the login password.
		 *///-------------------------------------
		std::string m_dbPassword;

		//========================================
		/*!\brief Holds the database ip address
		 *///------------------------------------
		std::string m_dbIp;

		//========================================
		/*!\brief Holds the database port.
		 *///------------------------------------
		unsigned short m_dbPort;
	}; // ConnectionProperties
	//========================================

protected:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	DbConnection(const std::string& dbname)
	  : m_dbname(dbname),
	    m_connProps()
	{
		m_connProps.m_isConnected = false;
	}

public:
	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~DbConnection() {}

public:
	//========================================
	/*!\brief Sets the connection properties and calls
	 *        DbConnection::connect(ConnectionResult).
	 *
	 * \param[in]      connProps  The connection properties
	 * \param[in, out] res        Result of the connecting action.
	 * \return \c true, if no error occurred
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool connect(const ConnectionProperties& connProps, ConnectionResult& res)
	{
		m_connProps = connProps;
		return connect(res);
	}

	//========================================
	/*!\brief Returns status of connection properties.
	 *
	 * \return \c true if connection to database is established
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool isConnected() const { return m_connProps.m_isConnected; }

	//========================================
	/*!\brief Pure virtual function for returning the number
	 *        of established connections.
	 *
	 * \return Number of connections as integer
	 *///-------------------------------------
	virtual int nbOfConnections() const = 0;

public:
	//========================================
	/*!\brief Returns associated database name.
	 * \return Database name as string
	 *///-------------------------------------
	const std::string& getDbName() const { return m_dbname; }

	//========================================
	/*!\brief Returns connection properties.
	 * \return ConnectionProperties
	 *///-------------------------------------
	ConnectionProperties& getConnectionProperties() { return m_connProps; }

	//========================================
	/*!\brief Returns connection properties.
	 * \return ConnectionProperties
	 *///-------------------------------------
	const ConnectionProperties& getConnectionProperties() const { return m_connProps; }

protected:
	//========================================
	/*!\brief Sets status of connection properties to connected
	 * \param[in] connected Status if connection is established
	 *///-------------------------------------
	void setConnected(bool connected = true) { m_connProps.m_isConnected = connected; }

	//========================================
	/*!\brief Pure virtual function to establish a database connection.
	 *
	 * \param[in, out] result  Result of the connecting action.
	 * \return \c true if connection to database is established
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool connect(ConnectionResult& result) = 0;

protected:
	//========================================
	/*!\brief Holds the current database name.
	 *///-------------------------------------
	std::string m_dbname;

	//========================================
	/*!\brief Holds the current connection properties.
	 *///-------------------------------------
	ConnectionProperties m_connProps;
}; // DbConnection

//======================================================================

//========================================
/*!\brief Boost shared pointer for DbConnection.
 *///-------------------------------------
typedef boost::shared_ptr<DbConnection> DbConPtr;

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // DBCONNECTION_HPP_SEEN

//======================================================================
