//======================================================================
/*! \file VersionDbReader.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 4, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef VERSIONDBREADER_HPP_SEEN
#define VERSIONDBREADER_HPP_SEEN

//======================================================================

#include <ibeosdk/database/MongoDbInterface.hpp>
#include <ibeosdk/database/basedatamodel/DatabaseVersion.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

//======================================================================
/*!\class VersionDbReader
 * \brief Class to read the database model version.
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 22, 2016
 *///------------------------------------------------------------------
class VersionDbReader
{

public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	VersionDbReader();

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~VersionDbReader() {}

public:
	//========================================
	/*!\brief Connects to database.
	 *
	 * Uses DbConnectionFactory to establish a connection
	 * with a MongoDbInterface.
	 *///-------------------------------------
	virtual void connect();

	//========================================
	/*!\brief  Queries the database model version.
	 *
	 * \note It is necessary to call VersionDbReader::connect
	 *       function first, to establish a database connection.
	 *
	 * \return DatabaseVersion which contains information
	 *         about Major, Minor and Patch value
	 * \throw DatabaseException To signalize the non-existence
	 *                          of the model version a DatabaseException
	 *                          will be thrown.
	 *///-------------------------------------
	virtual dbaccess::DatabaseVersion getModelVersion();

private:
	//========================================
	/*!\brief General database interface.
	 *///-------------------------------------
	dbaccess::MongoDbInterface::DbIfPtr m_dbInterface;
}; //VersionDbReader

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // VERSIONDBREADER_HPP_SEEN

//======================================================================
