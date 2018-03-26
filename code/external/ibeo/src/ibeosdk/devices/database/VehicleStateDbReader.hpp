//======================================================================
/*! \file VehicleStateDbReader.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 17, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef IBEOSDK_VEHICLESTATEDBREADER_HPP_SEEN
#define IBEOSDK_VEHICLESTATEDBREADER_HPP_SEEN

//======================================================================

#include <ibeosdk/devices/database/DbDataBlockReader.hpp>
#include <ibeosdk/database/DbInterfaceT.hpp>
#include <ibeosdk/database/DbQueryIterator.hpp>
#include <ibeosdk/datablocks/VehicleStateBasicEcu.hpp>
#include <ibeosdk/datablocks/VehicleStateBasicEcu2808.hpp>
#include <ibeosdk/TimeInterval.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class VehicleStateDbReader
 * \brief Derived DbDataBlockReader for DataBlocks:
 *         - VehicleStateBasicEcu (0x2807)
 *         - VehicleStateBasicEcu2808 (0x2808)
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 17, 2016
 *///-------------------------------------------------------------------
class VehicleStateDbReader : public DbDataBlockReader
{

public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	VehicleStateDbReader();

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~VehicleStateDbReader() {}

public:
	//========================================
	/*!\brief Connects to given collection.
	 *
	 * Uses DbConnectionFactory to establish a connection with a DbInterfaceT<VehicleStateBasicEcu>
	 * or DbInterfaceT<VehicleStateBasicEcu2808>. The VehicleStateBasicEcu2808 will be used, if
	 * VehicleStateDbReader::isProcessedDataActive() returns true.
	 * In this case #m_dbInfOnlineVhs will not connect to the database and the
	 * #m_itrOnlineVhs iterator is empty. The same behavior applies to the use with VehicleStateBasicEcu.
	 *
	 * \note If processed data is desired, call VehicleStateDbReader::useProcessedData(const bool& value)
	 *       before this function.
	 *
	 * \param[in] collection  CollectionName to connect to.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///------------------------------------
	virtual bool connectToDbInterface(const dbaccess::CollectionName& collection);

	//========================================
	/*!\brief Connects to the VehicleState collection which is part of a Trip database.
	 *
	 * Uses VehicleStateDbReader::connectToDbInterface(const dbaccess::CollectionName& collection).
	 *
	 * \param[in] trip  Trip.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::Trip& trip);

	//========================================
	/*!\brief Connects to database with given processing job.
	 *
	 * Uses VehicleStateDbReader::connectToDbInterface(const dbaccess::CollectionName& collection).
	 *
	 * \param[in] proc  ProcessingJob.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::ProcessingJob& proc);

	//========================================
	/*!\brief Notifies a DatabaseReplayer with the current VehicleState.
	 *
	 * The current Scan is the object the VehicleStateDbReader::m_itr is pointing to.
	 * This function increments the iterator.
	 *
	 * \note Before calling this, check VehicleStateDbReader::hasValidData if the
	 * iterator pasts the end.
	 *
	 * \param[in] dbReader  DatabaseReplayer object (IbeoDeviceBase).
	 *///-------------------------------------
	void notify(DatabaseReplayer& dbReader);

	const NTPTime getCurrentDataBlockTimestamp() const;
	virtual const bool hasValidData() const;

	//========================================
	/*!\brief Queries all entries ordered by time stamp.
	 *
	 * The VehicleStateDbReader#m_itr will be initialized.
	 * To iterate through the results, either call VehicleStateDbReader::getResultIterator
	 * to get the iterator or use VehicleStateDbReader::notify.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *///-------------------------------------
	void queryData();

	//========================================
	/*!\brief Queries the oldest and latest time stamp.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[out] start  The oldest time stamp.
	 * \param[out] end    The latest time stamp.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryTimeRange(NTPTime& start, NTPTime& end);

	//========================================
	/*!\brief Queries the VehicleStateBasicEcu with an specific time stamp.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[in] time  The time stamp to search for.
	 * \param[out] vhs  The VehicleState that have been received from to database.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryVehicleStateByTimeStamp(const NTPTime& time, VehicleStateBasicEcu& vhs);

	//========================================
	/*!\brief Queries the VehicleStateBasicEcu2806 with an specific time stamp.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[in] time  The time stamp to search for.
	 * \param[out] vhs  The verhicle state that have been received from to database.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryVehicleStateByTimeStamp(const NTPTime& time, VehicleStateBasicEcu2808& vhs);

	//========================================
	/*!\brief Queries a range of VehicleStateBasicEcu specified by a time range.
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
	virtual bool queryVehicleStateRange(const NTPTime& startTime, const NTPTime& endTime,
	                                    const StartRangeMode startRangeMode = StartRangeInclusive,
	                                    const EndRangeMode endRangeMode = EndRangeInclusive);

	//========================================
	/*!\brief Queries a range of VehicleStateBasicEcu specified by a time interval.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[in] interval        The time interval with specification about
	 *                            start and end times.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryVehicleStateRange(const TimeInterval& interval);

	virtual bool queryVehicleStateRangeExclusiveEnd(const NTPTime& start, const NTPTime& end);

	//========================================
	/*!\brief Returns the DbInterfaceT<VehicleStateBasicEcu>::DbIfPtr (boost::shared_ptr).
	 *
	 * With this interface, custom queries to the database can be performed.
	 *
	 * \return DbInterfaceT<VehicleStateBasicEcu>::DbIfPtr The interface pointer.
	 *///-------------------------------------
	virtual dbaccess::DbInterfaceT<VehicleStateBasicEcu>::DbIfPtr getOnlineDbInterface(){ return m_dbInfOnlineVhs; }

	//========================================
	/*!\brief Returns the DbQueryIterator<VehicleStateBasicEcu>.
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 *
	 * \return DbQueryIterator<VehicleStateBasicEcu> The result iterator.
	 *///-------------------------------------
	virtual dbaccess::DbQueryIterator<VehicleStateBasicEcu>& getOnlineResultIterator(){ return m_itrOnlineVhs; }

	//========================================
	/*!\brief Returns the DbInterfaceT<VehicleStateBasicEcu2808>::DbIfPtr (boost::shared_ptr).
	 *
	 * With this interface, custom queries to the database can be performed.
	 *
	 * \return DbInterfaceT<VehicleStateBasicEcu2808>::DbIfPtr The interface pointer.
	 *///-------------------------------------
	virtual dbaccess::DbInterfaceT<VehicleStateBasicEcu2808>::DbIfPtr getProcDbInterface(){ return m_dbInfProcVhs; }

	//========================================
	/*!\brief Returns the DbQueryIterator<VehicleStateBasicEcu2808>.
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 *
	 * \return DbQueryIterator<VehicleStateBasicEcu2808> The result iterator.
	 *///-------------------------------------
	virtual dbaccess::DbQueryIterator<VehicleStateBasicEcu2808>& getProcResultIterator(){ return m_itrProcVhs; }

protected:
	//========================================
	/*!\brief Database interface for VehicleStateBasicEcu.
	 *///-------------------------------------
	dbaccess::DbInterfaceT<VehicleStateBasicEcu>::DbIfPtr m_dbInfOnlineVhs;

	//========================================
	/*!\brief Database interface for VehicleStateBasicEcu2808.
	 *///-------------------------------------
	dbaccess::DbInterfaceT<VehicleStateBasicEcu2808>::DbIfPtr m_dbInfProcVhs;

	//========================================
	/*!\brief Result iterator for VehicleStateBasicEcu objects
	 *///-------------------------------------
	dbaccess::DbQueryIterator<VehicleStateBasicEcu> m_itrOnlineVhs;

	//========================================
	/*!\brief Result iterator for VehicleStateBasicEcu2808 objects
	 *///-------------------------------------
	dbaccess::DbQueryIterator<VehicleStateBasicEcu2808> m_itrProcVhs;
}; //VehicleStateDbReader

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_VEHICLESTATEDBREADER_HPP_SEEN

//======================================================================
