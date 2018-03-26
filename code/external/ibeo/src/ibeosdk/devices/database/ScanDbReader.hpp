//======================================================================
/*! \file ScanDbReader.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 17, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef IBEOSDK_SCANDBREADER_HPP_SEEN
#define IBEOSDK_SCANDBREADER_HPP_SEEN

//======================================================================

#include <ibeosdk/devices/database/DbDataBlockReader.hpp>
#include <ibeosdk/database/DbInterfaceT.hpp>
#include <ibeosdk/database/DbQueryIterator.hpp>
#include <ibeosdk/datablocks/ScanEcu.hpp>
#include <ibeosdk/TimeInterval.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class ScanDbReader
 * \brief Derived DbDataBlockReader for DataBlock: ScanEcu (0x2205).
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 17, 2016
 *///-------------------------------------------------------------------
class ScanDbReader : public DbDataBlockReader
{

public:

	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	ScanDbReader();

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~ScanDbReader() {}

public:
	//========================================
	/*!\brief Connects to given collection.
	 *
	 * Uses DbConnectionFactory to establish a connection with a DbInterfaceT<ScanEcu>.
	 *
	 * \param[in] collection  CollectionName to connect to.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///------------------------------------
	virtual bool connectToDbInterface(const dbaccess::CollectionName& collection);

	//========================================
	/*!\brief Connects to the Scan collection which is part of a Trip database.
	 *
	 * Calls ScanDbReader::connectToDbInterface(const dbaccess::CollectionName& collection).
	 *
	 * \param[in] trip  Trip.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::Trip& trip);

	//========================================
	/*!\brief Not implemented
	 *
	 * \param[in] proc  Processing.
	 * \return false.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::ProcessingJob& proc) { return false; }

	//========================================
	/*!\brief Notifies a DatabaseReplayer with the current Scan.
	 *
	 * The current Scan is the object the ScanDbReader::m_itr is pointing to.
	 * This function increments the iterator.
	 *
	 * \note Before calling this, check ScanDbReader::hasValidData if the
	 * iterator pasts the end.
	 *
	 * \param[in] dbReader  DatabaseReplayer object (IbeoDeviceBase).
	 *///-------------------------------------
	virtual void notify(DatabaseReplayer& dbReader);

	virtual const NTPTime getCurrentDataBlockTimestamp() const;
	virtual const bool hasValidData() const;

	//========================================
	/*!\brief Queries all entries ordered by time stamp.
	 *
	 * The ScanDbReader#m_itr will be initialized.
	 * To iterate through the results, either call ScanDbReader::getResultIterator
	 * to get the iterator or use ScanDbReader::notify.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *///-------------------------------------
	virtual void queryData();

	//========================================
	/*!\brief Queries the oldest and latest time stamp.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[out] start The oldest time stamp.
	 * \param[out] end The latest time stamp.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryTimeRange(NTPTime& start, NTPTime& end);

	//========================================
	/*!\brief Queries a single ScanEcu with an specific time stamp.
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 *
	 * \param[in] time The time stamp to search for.
	 * \param[in, out] scan The scan that have been received from to database.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryScanByTimeStamp(const NTPTime& time, ScanEcu& scan);

	//========================================
	/*!\brief Queries a range of ScanEcu specified by a time range.
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 * \note One gets the scans one by one, using the internal iterator
	 *       which is accessible via getResultIterator.
	 *
	 * \param[in] startTime The begin of the time range.
	 * \param[in] endTime The end of the time range.
	 * \param[in] startRangeMode  Tells the query whether to include
	 *                            or exclude the start time in or from the
	 *                            time interval to query for.
	 * \param[in] endRangeMode    Tells the query whether to include
	 *                            or exclude the end time in or from the
	 *                            time interval to query for.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryScanRange(const NTPTime& startTime, const NTPTime& endTime,
	                            const StartRangeMode startRangeMode = StartRangeInclusive,
	                            const EndRangeMode endRangeMode = EndRangeInclusive);

	//========================================
	/*!\brief Queries a range of ScanEcu specified by a time interval.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[in] interval        The time interval with specification about
	 *                            start and end times.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryScanRange(const TimeInterval& interval);

	//========================================
	/*!\brief Returns the DbInterfaceT<ScanEcu>::DbIfPtr (boost::shared_ptr).
	 *
	 * With this interface, custom queries to the database can performed.
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 * \return DbInterfaceT<ScanEcu>::DbIfPtr The interface pointer.
	 *///-------------------------------------
	virtual dbaccess::DbInterfaceT<ScanEcu>::DbIfPtr getDbInterface(){ return m_dbInterface; }

	//========================================
	/*!\brief Returns the DbQueryIterator<ScanEcu>.
	 *
	 * \return DbQueryIterator<ScanEcu> The result iterator.
	 *///-------------------------------------
	virtual dbaccess::DbQueryIterator<ScanEcu>& getResultIterator(){ return m_itr; }

protected:
	//========================================
	/*!\brief Database interface for ScanEcu.
	 *///-------------------------------------
	dbaccess::DbInterfaceT<ScanEcu>::DbIfPtr m_dbInterface;

	//========================================
	/*!\brief Result iterator
	 *///-------------------------------------
	dbaccess::DbQueryIterator<ScanEcu> m_itr;
}; //ScanDbReader

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCANDBREADER_HPP_SEEN

//======================================================================
