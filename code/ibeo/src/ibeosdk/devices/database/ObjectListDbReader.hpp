//======================================================================
/*! \file ObjectListDbReader.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 22, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef IBEOSDK_OBJECTLISTDBREADER_HPP_SEEN
#define IBEOSDK_OBJECTLISTDBREADER_HPP_SEEN

//======================================================================

#include <ibeosdk/devices/database/DbDataBlockReader.hpp>

#include <ibeosdk/database/DbInterfaceT.hpp>
#include <ibeosdk/database/DbQueryIterator.hpp>
#include <ibeosdk/database/MongoDbObjInterface.hpp>

#include <ibeosdk/datablocks/ScanEcu.hpp>
#include <ibeosdk/datablocks/ObjectListEcuEt.hpp>
#include <ibeosdk/datablocks/snippets/ObjectEcuEt.hpp>
#include <ibeosdk/TimeInterval.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class ObjectListDbReader
 * \brief Derived DbDataBlockReader for DataBlock: ObjectListEcuEt (0x2281).
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 22, 2016
 *///------------------------------------------------------------------
class ObjectListDbReader : public DbDataBlockReader
{

public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	ObjectListDbReader();

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~ObjectListDbReader() {}

public:
	//========================================
	/*!\brief Not implemented.
	 *
	 * \param[in] collection  CollectionName to connect to.
	 * \return false
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::CollectionName& collection) { return false; }

	//========================================
	/*!\brief Connects to given collections.
	 *
	 * Uses DbConnectionFactory to establish a connection with a DbInterfaceT<ScanEcu>
	 * and a MongoDbObjInterface.
	 *
	 * \param[in] scanCollection  ScanEcu collection name.
	 * \param[in] objTrajCollection  Collection name for object trajectories.
	 * \param[in] objInvCollection  Collection name for object invariants.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::CollectionName& scanCollection,
	                                  const dbaccess::CollectionName& objTrajCollection,
	                                  const dbaccess::CollectionName& objInvCollection);

	//========================================
	/*!\brief Connects to the collections for unprocessed dynamic objects.
	 *
	 * Calls ObjectListDbReader::connectToDbInterface(CollectionName, CollectionName, CollectionName)
	 *
	 * \param[in] trip  The Trip with specified name.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::Trip& trip);

	//========================================
	/*!\brief Connects to the collections for processed dynamic objects.
	 *
	 * Calls ObjectListDbReader::connectToDbInterface(CollectionName, CollectionName, CollectionName)
	 *
	 * \param[in] proc  Expects a ProcessingJob with type JobType::DYNAMIC_OBJECTS and two entries
	 *                 in the ResultConfig.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::ProcessingJob& proc);

	//========================================
	/*!\brief Queries all entries ordered by time stamp.
	 *
	 * The ObjectListDbReader#m_scanDbCursor will be initialized and
	 * a first ObjectListEcuEtDyn is generated.
	 * One can get the actual ObjectListEcuEtDyn by calling ObjectListDbReader::getCurrentObjectList.
	 * To create the next ObjectListEcuEtDyn use ObjectListDbReader::next;
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
	/*!\brief Queries the ObjectListEcuEtDyn with an specific time stamp.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[in]  time    The time stamp to search for.
	 * \param[out] objList The object list that have been received from to database.
	 * \return \c true if the given ObjectListEcuEtDyn is not empty.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryObjectListByTimeStamp(const NTPTime& time, ObjectListEcuEtDyn& objList);

	//========================================
	/*!\brief Queries a range of ObjectListEcuEtDyn specified by a time range.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[in] startTime The begin of the time range.
	 * \param[in] endTime   The end of the time range.
	 * \param[in] startRangeMode  Tells the query whether to include
	 *                            or exclude the start time in or from the
	 *                            time interval to query for.
	 * \param[in] endRangeMode    Tells the query whether to include
	 *                            or exclude the end time in or from the
	 *                            time interval to query for.
	 * \return \c true if the given ObjectListEcuEtDyn is not empty.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryObjectListRange(const NTPTime& startTime, const NTPTime& endTime,
	                                  const StartRangeMode startRangeMode = StartRangeInclusive,
	                                  const EndRangeMode endRangeMode = EndRangeInclusive);

	//========================================
	/*!\brief Queries a range of ObjectListEcuEtDyn specified by a time interval.
	 *
	 * Calls ObjectListDbReader::queryObjectListRange(NTPTime, NTPTime,
	                                                  StartRangeMode, EndRangeMod);
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[in] interval        The time interval with specification about
	 *                            start and end times.
	 * \return \c true if query was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool queryObjectListRange(const TimeInterval& interval);

	//========================================
	/*!\brief Returns pointer to a ObjectListEcuEtDyn if valid data are present.
	 *
	 * \note The pointer can be a NULL pointer
	 * \return  Pointer to ObjectListEcuEtDyn with valid data or NULL.
	 *///-------------------------------------
	const ObjectListEcuEtDyn* getCurrentObjectList() const
	{
		if (hasValidData())
			return &m_currentList;
		else
			return NULL;
	}

	//========================================
	/*!\brief Generates a new ObjectListEcuEtDyn.
	 *
	 * Uses ObjectListDbReader::getObjectList(ObjectListEcuEtDyn& objList)
//	 * \note The pointer can be a NULL pointer
	 *
	 *///-------------------------------------
	void next() { getObjectList(m_currentList); }

	virtual void notify(DatabaseReplayer& dbReader);
	virtual const NTPTime getCurrentDataBlockTimestamp() const;

	//========================================
	/*!\brief Tells whether a ObjectListEcuEtDyn with valid data is present
	 * \return \c true if valid data is present, \c false otherwise.
	 *///-------------------------------------
	virtual const bool hasValidData() const { return m_hasValidData; }

public:
	//========================================
	/*!\brief Returns the first invariant object data found in the database.
	 *
	 * The ObjectListDbReader#m_invariantCursor will be initialized.
	 * Uses ObjectListDbReader::getNextObjectInvariant().
	 *
	 *\note It is necessary to call connectToDbInterface functions first,
	 *      to establish a database connection.
	 *
	 * \return ObjectEcuEtDyn pointer filled with invariant data, or a NULL pointer.
	 *///-------------------------------------
	const ibeosdk::ObjectEcuEtDyn* getFirstObjectInvariant();

	//========================================
	/*!\brief Returns invariant object data kept by ObjectListDbReader#m_invariantCursor.
	 *
	 *\note It is necessary to call ObjectListDbReader:getFirstObjectInvariant()
	 *      function first, to get valid data for the ObjectListDbReader#m_invariantCursor
	 *      result cursor.
	 *
	 * \return ObjectEcuEtDyn pointer filled with invariant data, or a NULL pointer.
	 *///-------------------------------------
	const ibeosdk::ObjectEcuEtDyn* getNextObjectInvariant();

private:
	//========================================
	/*!\brief Identifies with given start/end time a range of ScanEcu's.
	 *
	 *\note It is necessary to call connectToDbInterface functions first,
	 *      to establish a database connection.
	 *
	 * \param[out] objList   ObjectListEcuEtDyn reference.
	 *///-------------------------------------
	virtual void prepareScanDbCursor(const NTPTime& startTime, const NTPTime& endTime,
	                                 const StartRangeMode startRangeMode = StartRangeInclusive,
	                                 const EndRangeMode endRangeMode = EndRangeInclusive);

	//========================================
	/*!\brief Fills ObjectListEcuEtDyn reference with data.
	 *
	 *\note It is necessary to call connectToDbInterface and queryData functions first,
	 *      to establish a database connection and to get valid data.
	 *
	 * \param[out] objList   ObjectListEcuEtDyn reference.
	 *///-------------------------------------
	virtual void getObjectList(ObjectListEcuEtDyn& objList);

	//========================================
	/*!\brief Get the next scan time stamp queried for.
	 * \param[out] time The time from the next scan
	 * \sa m_scanDbCursor
	 *///-------------------------------------
	void getNextScanTimestamp(boost::posix_time::ptime& time);

	//========================================
	/*!\brief Get the next time stamp queried for.
	 * \param[in]  cursor  Result cursor, holding the query results.
	 * \param[out] time    The time stamp from the next item.
	 *                     Iff the current cursor result hasn't a
	 *                     'Timestamp' field, this parameter will be
	 *                     boost::posix_time::not_a_date_time.
	 *///-------------------------------------
	static void getNextTimestampFromCursor(dbaccess::MongoCursorPtr cursor, boost::posix_time::ptime& time);

	//========================================
	/*!\brief Resets ObjectListDbReader::m_currentScansStartTs
	 *        and ObjectListDbReader::m_currentList .
	 *///-------------------------------------
	void resetCurrentValues();

protected:
	//========================================
	/*!\brief Tells whether the data in m_currentList
	 *        are valid.
	 *///-------------------------------------
	bool m_hasValidData;

	//========================================
	/*!\brief Hold the current object list if
	 *        m_hasValidData is \c true.
	 *///-------------------------------------
	ObjectListEcuEtDyn m_currentList;

	//========================================
	/*!\brief The start timestamp of the currently
	 *        next to be read scan.
	 *///-------------------------------------
	boost::posix_time::ptime m_currentScansStartTs;

	//========================================
	/*!\brief Holds the MongoDbObjInterface pointer to
	 *        query for the objects.
	 *///-------------------------------------
	dbaccess::MongoDbObjInterface::DbIfPtr m_dbInterface;

	//========================================
	/*!\brief Holds the MongoDbInterfaceT<ScanEcu> pointer to
	 *        query for the scans' time stamps
	 *///-------------------------------------
	dbaccess::DbInterfaceT<ScanEcu>::DbIfPtr m_scanDbInterface;

	//========================================
	/*!\brief The cursor to the query result when querying for
	 *        all scan time stamps within the given range.
	 * \sa queryObjectListRange
	 *///-------------------------------------
	dbaccess::MongoCursorPtr m_scanDbCursor;

	//========================================
	/*!\brief Tells whether the last scans time stamp is in the
	 *        requested range or the following scan.
	 *
	 * For an object lists all object updates (DynamicStream) in
	 * the time range from the scan's time stamp till but exclusive
	 * the next scan's time stamp.
	 *
	 * If queryObjectListRange is used the \a end time stamp is given
	 * as the last scan's time stamp for which an object list shall be
	 * generated.
	 *
	 * To collect all the object updates for this last scan, the time range
	 * has to be defined and hence the time stamp for the following scan
	 * is also needed.
	 *
	 * This flag will be set if such a following scan exists. This scan itself
	 * has to be excluded from the iterator range.
	 *///-------------------------------------
	bool m_lastScanIsBeyondRange;

	//========================================
	/*!\brief Standard mongo::DBClientCursor for invariant object data
	 *///-------------------------------------
	ibeosdk::dbaccess::MongoCursorPtr m_invariantCursor;

	//========================================
	/*!\brief Holds the invariant data from the last getNextObjectInvariant() call.
	 *///-------------------------------------
	ibeosdk::ObjectEcuEtDyn m_objInvariant;
}; // ObjectListDbReader

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTLISTDBREADER_HPP_SEEN

//======================================================================
