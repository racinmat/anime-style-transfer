//======================================================================
/*! \file CanMessageDbReader.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 22, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef IBEOSDK_CANMESSAGEDBREADER_HPP_SEEN
#define IBEOSDK_CANMESSAGEDBREADER_HPP_SEEN

//======================================================================

#include <ibeosdk/devices/database/DbDataBlockReader.hpp>
#include <ibeosdk/database/DbInterfaceT.hpp>
#include <ibeosdk/database/DbQueryIterator.hpp>
#include <ibeosdk/datablocks/CanMessage.hpp>
#include <ibeosdk/TimeInterval.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class CanMessageDbReader
 * \brief Derived DbDataBlockReader for DataBlock: CanMessage (0x1002).
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 22, 2016
 *///------------------------------------------------------------------
class CanMessageDbReader : public DbDataBlockReader {
public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	CanMessageDbReader();

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~CanMessageDbReader() {};

public:
	//========================================
	/*!\brief Connects to given collection.
	 *
	 * Uses DbConnectionFactory to establish a connection with a DbInterfaceT<CanMessage>.
	 *
	 * \param[in] collection  CollectionName to connect to.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::CollectionName& collection);

	//========================================
	/*!\brief Connects to the CanMessages collection which is part of a Trip database.
	 *
	 * Uses CanMessageDbReader::connectToDbInterface(const dbaccess::CollectionName& collection).
	 *
	 * \param[in] trip  Trip.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::Trip& trip);

	//========================================
	/*!\brief Not implemented
	 *
	 * \param[in] proc  ProcessingJob.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::ProcessingJob& proc) { return false; }

	//========================================
	/*!\brief Notifies a DatabaseReplayer with the current CanMessage.
	 *
	 * The current CanMessage is the object the CanMessageDbReader::m_itr is pointing to.
	 * This function increments the iterator.
	 *
	 * \note Before calling this, check CanMessageDbReader::hasValidData if the
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
	 * The CanMessageDbReader#m_itr will be initialized.
	 * To iterate through the results, either call CanMessageDbReader::getResultIterator
	 * to get the iterator or use CanMessageDbReader::notify.
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
	 * \param[in] start  The oldest time stamp.
	 * \param[in] end    The latest time stamp.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryTimeRange(NTPTime& start, NTPTime& end);

	//========================================
	/*!\brief Queries the CanMessage with an specific time stamp.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[in]  time    The time stamp to search for.
	 * \param[out] canMsg  The CanMessage that have been received from the database.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryCanMessageByTimeStamp(const NTPTime& time, CanMessage& canMsg);

	//========================================
	/*!\brief Queries a range of CanMessages specified by a time range.
	 *
	 * The CanMessageDbReader#m_itr will be reseted.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[in] startTime       The begin of the time range.
	 * \param[in] endTime         The end of the time range.
	 * \param[in] startRangeMode  Tells the query whether to include
	 *                            or exclude the start time in or from the
	 *                            time interval to query for.
	 * \param[in] endRangeMode    Tells the query whether to include
	 *                            or exclude the end time in or from the
	 *                            time interval to query for.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryCanMessageRange(const NTPTime& startTime, const NTPTime& endTime,
	                                  const StartRangeMode startRangeMode = StartRangeInclusive,
	                                  const EndRangeMode endRangeMode = EndRangeInclusive);

	//========================================
	/*!\brief Queries a range of CanMessages specified by a time interval.
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 *
	 * \param[in] interval  The time interval with specification about
	 *                      start and end times.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryCanMessageRange(const TimeInterval& interval);

	//========================================
	/*!\brief Returns the DbInterfaceT<CanMessage>::DbIfPtr (boost::shared_ptr).
	 *
	 * With this interface, custom queries to the database can be performed.
	 *
	 * \return DbInterfaceT<CanMessage>::DbIfPtr The interface pointer.
	 *///-------------------------------------
	virtual dbaccess::DbInterfaceT<CanMessage>::DbIfPtr getDbInterface(){ return m_dbInterface; };

	//========================================
	/*!\brief Returns the DbQueryIterator<CanMessage>.
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 *
	 * \return DbQueryIterator<CanMessage> The result iterator.
	 *///-------------------------------------
	virtual dbaccess::DbQueryIterator<CanMessage>& getResultIterator(){ return m_itr; };

protected:
	//========================================
	/*!\brief Database interface for CanMessages.
	 *///-------------------------------------
	dbaccess::DbInterfaceT<CanMessage>::DbIfPtr m_dbInterface;

	//========================================
	/*!\brief Result iterator
	 *///-------------------------------------
	dbaccess::DbQueryIterator<CanMessage> m_itr;
}; // CanMessageDbReader

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_CANMESSAGEDBREADER_HPP_SEEN

//======================================================================
