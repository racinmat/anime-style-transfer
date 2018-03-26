//======================================================================
/*! \file ProcessingDbReader.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 7, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef PROCESSINGDBREADER_HPP_SEEN
#define PROCESSINGDBREADER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/database/MongoDbInterface.hpp>
#include <ibeosdk/database/MongoDbInterfaceT.hpp>
#include <ibeosdk/database/DbQueryIterator.hpp>

#include <ibeosdk/database/basedatamodel/Processing.hpp>
#include <ibeosdk/database/basedatamodel/Trip.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class ProcessingDbReader
 * \brief Independent Reader for Processing's. This class is not derived
 *        from DbDataBlockReader.
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 7, 2016
 *///------------------------------------------------------------------
class ProcessingDbReader
{

public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	ProcessingDbReader();

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~ProcessingDbReader() {};

public:
	//========================================
	/*!\brief Connects to the database.
	 *
	 * Uses DbConnectionFactory to establish a connection
	 * with a MongoDbInterface.
	 *///-------------------------------------
	virtual void connect();

public:
	//========================================
	/*!\brief Queries information about one ProcessingJob
	 *
	 * The given ProcessingJob needs to contain the collection name and
	 * it's database identifier.
	 *
	 * \note It is necessary to call ProcessingDbReader::connect
	 *       function first, to establish a database connection.
	 *
	 * \param[in] procJob  ProcessingJob to fill.
	 * \return \c true, if Processing is in database.
	 *///-------------------------------------
	virtual bool getProcessingJob(dbaccess::ProcessingJob& procJob);

	//========================================
	/*!\brief Returns a list of ProcessingsJob's processed for the given Trip.
	 *
	 * \note It is necessary to call ProcessingDbReader::connect
	 *       function first, to establish a database connection.
	 *
	 * \param[in] trip  Trip from the database which contains possibly ProcessingJob's.
	 * \return dbaccess::ProcJobList List with ProcessingsJob's
	 *         Is empty when no Processing was started.
	 *///-------------------------------------
	virtual dbaccess::ProcJobList getProcessingJobs(const dbaccess::Trip& trip);

	//========================================
	/*!\brief Iterates through a list of ProcessingJob's and queries
	 *        information for each.
	 *
	 * Uses ProcessingDbReader::getProcessingJob(dbaccess::ProcessingJob& procJob)
	 *
	 * \note It is necessary to call ProcessingDbReader::connect
	 *       function first, to establish a database connection.
	 *
	 * \param[in, out]  jobList  List of ProcessingJob's.
	 *///-------------------------------------
	virtual void fillProcessingJobs(dbaccess::ProcJobList& jobList);

	//========================================
	/*!\brief Gets information about one Processing.
	 *
	 * The given Processing needs to contain the collection name and
	 * it's database identifier
	 *
	 * \note It is necessary to call ProcessingDbReader::connect
	 *       function first, to establish a database connection.
	 *
	 * \return \c true, if Processing is in database
	 *///-------------------------------------
	virtual bool getProcessing(dbaccess::Processing& proc);

	//========================================
	/*!\brief Gets all processings's from the database.
	 *
	 * \note It is necessary to call ProcessingDbReader::connect
	 *       function first, to establish a database connection.
	 *
	 * \return A ProcList which contains all Processing's
	 *         from the database. The list will be empty if
	 *         no Processing can be found.
	 *///-------------------------------------
	virtual dbaccess::ProcList getProcessings();

	//========================================
	/*!\brief Iterates through a list of Processings's and queries
	 *        information for each.
	 *
	 * Uses ProcessingDbReader::getProcessing(dbaccess::Processing& proc).
	 *
	 * \note It is necessary to call ProcessingDbReader::connect
	 *       function first, to establish a database connection.
	 * \param[in, out]  list  List of Processings's.
	 *///-------------------------------------
	virtual void fillProcessings(dbaccess::ProcList& list);

private:
	//========================================
	/*!\brief General database interface.
	 *///-------------------------------------
	dbaccess::MongoDbInterface::DbIfPtr m_dbInterface;

	//========================================
	/*!\brief Database interface for ProcessingJob's.
	 *///-------------------------------------
	dbaccess::DbInterfaceT<dbaccess::ProcessingJob>::DbIfPtr m_dbProcJobInterface;

	//========================================
	/*!\brief Result iterator for ProcessingJob's
	 *///-------------------------------------
	dbaccess::DbQueryIterator<dbaccess::ProcessingJob> m_itr;
}; // ProcessingDbReader

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // PROCESSINGDBREADER_HPP_SEEN

//======================================================================
