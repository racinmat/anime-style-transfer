//======================================================================
/*! \file MongoDbConnection.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 10, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include "MongoDbConnection.hpp"

#include <ibeosdk/Log.hpp>
#include <iostream>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

bool MongoDbConnection::connect(ConnectionResult& res)
{
	try {
		std::stringstream connection_ss;
		connection_ss << m_connProps.m_dbIp << ":" << m_connProps.m_dbPort;
		m_connectionString = connection_ss.str();
		m_dbConnection.connect(m_connectionString, res.m_dbErrMsg);

		if (res.m_dbErrMsg.empty()) {
			res.m_dbErrorCode = DbConnection::ConnectionResult::RfDb_NoError;
			m_dbConnection.resetError();
		}
		else
			res.m_dbErrorCode = DbConnection::ConnectionResult::RfDb_ConnectionError;

		if (res.m_dbErrorCode == 0) {
			if (!m_connProps.m_dbPassword.empty() && !m_connProps.m_dbUser.empty()) {
				m_dbConnection.auth("admin", m_connProps.m_dbUser, m_connProps.m_dbPassword, res.m_dbErrMsg);
			}
		}
		if (res.m_dbErrMsg.empty()) {
			res.m_dbErrorCode = DbConnection::ConnectionResult::RfDb_NoError;
			m_dbConnection.resetError();
		}
		else
			res.m_dbErrorCode = DbConnection::ConnectionResult::RfDb_ConnectionError;

	}
	catch (const std::exception& e) {
		res.m_dbErrorCode = DbConnection::ConnectionResult::RfDb_ExceptionRaised;
		res.m_dbErrMsg = e.what();
		logError << "Exception caught: " << e.what() << std::endl;
		return false;
	}

	logDebug<< "ErrorCode " << res.m_dbErrorCode << std::endl;

	if (res.m_dbErrorCode == 2) {
		logDebug<< res.m_dbErrMsg << std::endl;
	}

	setConnected(res.m_dbErrorCode == 0);

	return isConnected();
}

//======================================================================

int MongoDbConnection::nbOfConnections() const
{
	return m_dbConnection.getNumConnections();
}

//======================================================================

}// namespace dbaccess
} // namespace ibeosdk

//======================================================================
