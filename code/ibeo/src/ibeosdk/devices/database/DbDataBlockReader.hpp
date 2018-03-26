//======================================================================
/*! \file DbDataBlockReader.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 22, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef IBEOSDK_DBDATABLOCKREADER_HPP_SEEN
#define IBEOSDK_DBDATABLOCKREADER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/database/CollectionName.hpp>
#include <ibeosdk/database/basedatamodel/Trip.hpp>
#include <ibeosdk/database/basedatamodel/Processing.hpp>

#include <ibeosdk/Time.hpp>

//======================================================================

namespace ibeosdk {
class DataBlock;
class DatabaseReplayer;

//======================================================================
/*!\class DbDataBlockReader
 * \brief Base class for all DbReader. Derived classes will be responsible
 *        to connect to a database collection and perform queries for
 *        their assigned Datablocks.
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 22, 2016
 *///------------------------------------------------------------------
class DbDataBlockReader
{

public:
	//========================================
	/*!\brief Enumeration for start ranges.
	 *///-------------------------------------
	enum StartRangeMode {
		StartRangeInclusive,
		StartRangeExclusive
	};

	//========================================
	/*!\brief Enumeration for end ranges.
	 *///-------------------------------------
	enum EndRangeMode {
		EndRangeInclusive,
		EndRangeExclusive
	};

public:
	//========================================
	/*!\brief Creates an DbDataBlockReader linked with a ProcessingJob type.
	 *///-------------------------------------
	DbDataBlockReader(const dbaccess::ProcessingJob::JobType& type);

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~DbDataBlockReader() {};

public:
	//========================================
	/*!\brief Connects to given collection.
	 *
	 * \param[in] collection  CollectionName to connect to.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::CollectionName& collection) = 0;

	//========================================
	/*!\brief Connects to database with given Trip.
	 *
	 * \param[in] trip  Trip.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::Trip& trip) = 0;

	//========================================
	/*!\brief Connects to database with given ProcessingJob.
	 *
	 * \param[in] proc  ProcessingJob.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::ProcessingJob& proc) = 0;

	//========================================
	/*!\brief Standard query, to get all items.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *///-------------------------------------
	virtual void queryData() = 0;

	//========================================
	/*!\brief Notifies a DatabaseReplayer.
	 *
	 * \param[in] dbReader  DatabaseReplayer object (IbeoDeviceBase).
	 *///-------------------------------------
	virtual void notify(DatabaseReplayer& dbReader) = 0;

	//========================================
	/*!\brief Returns NTPTime from a DataBlock object.
	 *
	 *\return NTPTime The time of the object.
	 *///-------------------------------------
	virtual const NTPTime getCurrentDataBlockTimestamp() const = 0;

	//========================================
	/*!\brief Checks if a DataBlock exists.
	 *
	 *\return \c true if there is a DataBlocks, \c false otherwise.
	 *///-------------------------------------
	virtual const bool hasValidData() const = 0;

	//========================================
	/*!\brief Checks if the given string contains another.
	 *
	 * \param[in] str String to search in.
	 * \param[in] pattern String of interest.
	 * \return Result.
	 * \retval true Pattern is part of str.
	 * \retval false Pattern isn't a part of str.
	 *///-------------------------------------
	virtual bool findSubStr(const std::string& str, const std::string& pattern);

	//========================================
	/*!\brief Returns the supported processing job type.
	 *
	 * \return dbaccess::ProcessingJob::JobType The type.
	 *///-------------------------------------
	virtual const dbaccess::ProcessingJob::JobType getSupportedJobType() const { return m_jobType; }

	//========================================
	/*!\brief Sets DbDataBlockReader in a different reading mode for processed data.
	 *
	 * \param[in] value True activates reading mode for processed data.
	 * \return dbaccess::ProcessingJob::JobType The type.
	 *///-------------------------------------
	virtual void useProcessedData(const bool& value = true) { m_useProcessedData = value; }

	//========================================
	/*!\brief Returns if DbDataBlockReader is in processed reading mode.
	 * e
	 * \return \c true if processed reading mode is active, \c false otherwise.
	 *///-------------------------------------
	virtual const bool isProcessedDataActive() const { return m_useProcessedData; }

protected:
	//========================================
	/*!\brief Forwards a DataBlock to the DatabaseReplayer
	 * \param[in] dbReader  DatabaseReplayer to call.
	 * \param[in] db        DataBlock that has been received.
	 *///-------------------------------------
	void notifyListeners(DatabaseReplayer& dbReader, const DataBlock* const db);

protected:
	//========================================
	/*!\brief Job type this reader corresponds
	 *///------------------------------------
	dbaccess::ProcessingJob::JobType m_jobType;

	//========================================
	/*!\brief Flag to signalize a reading mode for processed data
	 *///------------------------------------
	bool m_useProcessedData;
}; // DbDataBlockReader

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_DBDATABLOCKREADER_HPP_SEEN

//======================================================================
