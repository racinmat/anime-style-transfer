//======================================================================
/*! \file ImageDbReader.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 22, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef IBEOSDK_IMAGEDBREADER_HPP_SEEN
#define IBEOSDK_IMAGEDBREADER_HPP_SEEN

//======================================================================

#include <ibeosdk/devices/database/DbDataBlockReader.hpp>
#include <ibeosdk/database/DbInterfaceT.hpp>
#include <ibeosdk/database/DbQueryIterator.hpp>
#include <ibeosdk/datablocks/Image.hpp>
#include <ibeosdk/TimeInterval.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class ImageDbReader
 * \brief Derived DbDataBlockReader for DataBlock: Image (0x2403).
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 22, 2016
 *///------------------------------------------------------------------
class ImageDbReader : public DbDataBlockReader
{

public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	ImageDbReader();

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~ImageDbReader(){}

public:
	//========================================
	/*!\brief Connects to given collection.
	 *
	 * Uses DbConnectionFactory to establish a connection with an DbInterfaceT<Image>.
	 *
	 * \param[in] collection  CollectionName to connect to.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::CollectionName& collection);

	//========================================
	/*!\brief Connects to the Image collection which is part of a Trip database.
	 *
	 * Uses ImageDbReader::connectToDbInterface(const dbaccess::CollectionName& collection).
	 *
	 * \param[in] trip  Trip.
	 * \return \c true if connecting was successful, \c false otherwise.
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::Trip& trip);

	//========================================
	/*!\brief Not implemented
	 *
	 * \param[in] proc  ProcessingJob.
	 * \return false
	 *///-------------------------------------
	virtual bool connectToDbInterface(const dbaccess::ProcessingJob& proc) { return false; }

	//========================================
	/*!\brief Notifies a DatabaseReplayer with the current Image.
	 *
	 * The current Image is the object the ImageDbReader::m_itr is pointing to.
	 * This function increments the iterator.
	 *
	 * \note Before calling this, check ImageDbReader::hasValidData if the
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
	 * The ImageDbReader#m_itr will be initialized.
	 * To iterate through the results, either call ImageDbReader::getResultIterator
	 * to get the iterator or use ImageDbReader::notify.
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
	 * \param[out] start  The oldest time stamp.
	 * \param[out] end    The latest time stamp.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryTimeRange(NTPTime& start, NTPTime& end);

	//========================================
	/*!\brief Queries the Image with an specific time stamp.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[in]      time   The time stamp to search for.
	 * \param[out] image  The Image that have been received from the database.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise..
	 *///-------------------------------------
	virtual bool queryImageByTimeStamp(const NTPTime& time, Image& image);

	//========================================
	/*!\brief Queries a range of Images specified by a time range.
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
	virtual bool queryImageRange(const NTPTime& startTime, const NTPTime& endTime,
	                             const StartRangeMode startRangeMode = StartRangeInclusive,
	                             const EndRangeMode endRangeMode = EndRangeInclusive);

	//========================================
	/*!\brief Queries a range of Images specified by a time interval.
	 *
	 *\note It is necessary to call connectToDbInterface function first,
	 *      to establish a database connection.
	 *
	 * \param[in] interval        The time interval with specification about
	 *                            start and end times.
	 * \return \c true if any valid data has been received from the database.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool queryImageRange(const TimeInterval& interval);

	//========================================
	/*!\brief Returns the DbInterfaceT<Image>::DbIfPtr (boost::shared_ptr).
	 *
	 * With this interface, custom queries to the database can be performed.
	 *
	 * \return DbInterfaceT<Image>::DbIfPtr The interface pointer.
	 *///-------------------------------------
	virtual dbaccess::DbInterfaceT<Image>::DbIfPtr getDbInterface() { return m_dbInterface; }

	//========================================
	/*!\brief Returns the DbQueryIterator<Image>.
	 *
	 * \note It is necessary to call connectToDbInterface function first,
	 *       to establish a database connection.
	 *
	 * \return DbQueryIterator<Image> The result iterator.
	 *///-------------------------------------
	virtual dbaccess::DbQueryIterator<Image>& getResultIterator() { return m_itr; }

protected:
	//========================================
	/*!\brief Database interface for Image.
	 *///-------------------------------------
	dbaccess::DbInterfaceT<Image>::DbIfPtr m_dbInterface;

	//========================================
	/*!\brief Result iterator
	 *///-------------------------------------
	dbaccess::DbQueryIterator<Image> m_itr;
}; // ImageDbReader

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IMAGEDBREADER_HPP_SEEN

//======================================================================
