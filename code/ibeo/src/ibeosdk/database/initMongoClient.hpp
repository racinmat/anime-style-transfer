//======================================================================
/*! \file initMongoClient.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Jun 11, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef INIT_HPP
#define INIT_HPP

//======================================================================

#ifdef _WIN32
#include <WinSock2.h>
#include <windows.h>
#endif // _WIN32
#include <mongo/client/dbclient.h>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================
/*!\brief Initialize the mongo client driver with default options.
 *
 * Call this function exactly once inside the 'main' function.
 * See documentation mongo::client::initialize().
 *///-------------------------------------------------------------------
void initMongoClient()
{
	mongo::Status status = mongo::client::initialize();
	logInfo << "MongoDB client initialization returned status: "
			<< status.toString() << std::endl;
}

//======================================================================
/*!\brief Shutdown the mongo client driver.
 *
 * See documentation mongo::client::shutdown().
 *///-------------------------------------------------------------------
void shutdownMongoClient()
{
	mongo::Status status = mongo::client::shutdown();
	logInfo << "MongoDB client shutdown returned status: "
			<< status.toString() << std::endl;
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // INIT_HPP

//======================================================================
