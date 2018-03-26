//======================================================================
/*! \file IdcFile.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date April 4, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_IDCFILE_HPP_SEEN
#define IBEOSDK_IDCFILE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>
#include <ibeosdk/listener/DataStreamer.hpp>
#include <ibeosdk/datablocks/FrameIndex.hpp>

#include <ibeosdk/misc/deprecatedwarning.hpp>
#include <ibeosdk/misc/StatusCodes.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <fstream>
#include <list>

//======================================================================

namespace ibeosdk {

class DataBlock;

//======================================================================
/*!\class IdcFile
 * \brief Class to handle the access to an file in ibeo data container (IDC)
 *        format.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Sep 4, 2013
 *
 *///-------------------------------------------------------------------
class IdcFile : boost::noncopyable {
protected:
	//========================================
	/*!\brief List of DataListenerBase (pointer).
	 *///-------------------------------------
	typedef std::list<DataListenerBase*> ListenerList;

	//========================================
	/*!\brief Each map entry is a pair of DataTypeId and
	 *        a list of listener.
	 *///-------------------------------------
	typedef boost::unordered_map<DataTypeId, ListenerList > ListenerListMap;

	//========================================
	/*!\brief List of DataStreamer (pointer).
	 *///-------------------------------------
	typedef std::list<DataStreamer*> StreamerList;

public:
	IdcFile();
	virtual ~IdcFile();

public:
	//========================================
	/*!\brief Open a file for read access.
	 * \return \c true if the file was successfully
	 *         opened. \c false otherwise.
	 *///-------------------------------------
	virtual bool open(const std::string& filename);

	//========================================
	/*!\brief Create/Open a file for write access.
	 * \return \c true if the file was successfully
	 *         opened/created. \c false otherwise.
	 *///-------------------------------------
	virtual bool create(const std::string& filename, const bool append = false);

	//========================================
	/*!\brief Check whether the file is open.
	 *
	 * \return \c true if the file was successfully
	 *         opened/created. \c false otherwise.
	 *///-------------------------------------
	virtual bool isOpen();

	//========================================
	/*!\brief Check whether the eofbit is set
	 * \return \c true if the eofbit is set.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool isEOF();

	//========================================
	/*!\brief Check whether no error bit is set
	 * \return \c true if the not error bit is set.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool isGood();

	//========================================
	/*!\brief Close an IdcFile.
	 *
	 * Is this IdcFile an output file, this method
	 * will write an FrameIndex and a IdcTrailer to
	 * the file. After that it will close the
	 * underlying stream.
	 *///-------------------------------------
	virtual void close();


	//========================================
	/*!\brief Write a DataBlock and its IbeoDataHeader
	 *        to this file.
	 *
	 * \param[in] dh    IbeoDataHeader for the
	 *                  serialized DataBlock in
	 *                  \a body.
	 * \param[in] body  Serialized version of
	 *                  the DataBlock to be
	 *                  written to the IdcFile.
	 *///-------------------------------------
	virtual bool write(const IbeoDataHeader& dh, const char* const body);


	//========================================
	/*!\brief Write a DataBlock and its IbeoDataHeader
	 *        to this file.
	 *
	 * \param[in] dh  IbeoDataHeader for the
	 *                serialized DataBlock in
	 *                \a db.
	 * \param[in] db  DataBlock to be written
	 *                to the IdcFile.
	 *///-------------------------------------
	virtual bool write(const IbeoDataHeader& dh, const DataBlock* const db);


	virtual const DataBlock* getNextDataBlock();
	//========================================
	/*!\brief Read the next Datablock from the file.
	 *
	 * Only the header will be deserialized while the
	 * body will be read in file order.
	 *
	 * The buffer to hold the data will be allocated.
	 * A copy of the data from the file stream will be
	 * performed.
	 * \param[out] dh  IbeoDataHeader of the received
	 *                 message.
	 *
	 * \return The contents of the datablock in raw
	 *         file order.
	 *///-------------------------------------
	boost::shared_ptr<char> getNextDataBlockRaw(IbeoDataHeader& dh);

public:
	//========================================
	/*!\brief Return the open mode the file has
	 *        been opened with.
	 * \return The open mode.
	 *///-------------------------------------
	std::ios_base::openmode getOpenMode() const { return m_openMode; }

	//========================================
	/*!\brief Return the frame index.
	 * \return The frame index.
	 *///-------------------------------------
	const FrameIndex& getFrameIndex() const { return m_frameIndex; }

public: // same interface as in IbeoDeviceBase
	void registerListener(DataListenerBase* const listener);

	//========================================
	/*!\brief Unregister a listener.
	 * \param[in] listener  Address of the listener object to
	 *                      be unregistered.
	 * \return Result of the operation.
	 *///-------------------------------------
	statuscodes::Codes unregisterListener(DataListenerBase* const listener);

	//========================================
	/*!\brief Register a DataStreamer to this device.
	 *
	 * Each time a message has been received by the
	 * this object, the registered streamer will be
	 * call which are listening to the received message
	 * type (DataType).
	 * \param[in] streamer  Streamer to be registered.
	 * \note There is (currently) no way to unregister a
	 *       streamer, so a registered DataStreamer must
	 *       not be destroyed before this IbeoDevice
	 *       has been destroyed.
	 * \sa IbeoDeviceBase::registerStreamer
	 *///-------------------------------------
	void registerStreamer(DataStreamer* const streamer);

	//========================================
	/*!\brief Unregister a streamer.
	 * \param[in] streamer  Address of the streamer object to
	 *                      be unregistered.
	 * \return Result of the operation.
	 *///-------------------------------------
	statuscodes::Codes unregisterStreamer(DataStreamer* const streamer);

public:
	//========================================
	/*!\brief Call all registered Listener listening to the
	 *        received type of DataBlock.
	 * \param[in] data  DataBlock that has been received.
	 * \sa IbeoDeviceBase::notifyListeners
	 *///-------------------------------------
	void notifyListeners(const DataBlock* const data);

	//========================================
	/*!\brief Call all registered Streamers listening to the
	 *        received type of DataBlock.
	 * \param[in] dh       Header that came along with that
	 *                     DataBlock.
	 * \param[in] bodyBuf  Buffer that contains the
	 *                     (still serialized) body of
	 *                     the received DataBlock.
	 * \sa IbeoDeviceBase::notifyStreamers
	 *///-------------------------------------
	void notifyStreamers(const IbeoDataHeader& dh, const boost::shared_ptr<char> bodyBuf);

	//========================================
	/*!\brief Call all registered Streamers listening to the
	 *        received type of DataBlock.
	 * \param[in] dh    Header that came along with that
	 *                  DataBlock.
	 * \param[in] data  Buffer that contains the
	 *                  (still serialized) body of
	 *                  the received DataBlock.
	 * \warning This method is deprecated. Use notifyStreamers instead.
	 *///-------------------------------------
	IBEOSDK_DEPRECATED void notifyStreamer(const IbeoDataHeader& dh, const boost::shared_ptr<char> data)
	{
		notifyStreamers(dh, data);
	}

public:
	std::list<DataBlock::DataBlockRegisterId::Key>
	getIdsOfSupportedDataBlocks() const;

protected:
	//========================================
	/*!\brief Deserialize a received DataBlock into \a db.
	 * \param[in] is  Stream that contains the body of the
	 *                DataBlock.
	 * \param[in] db  The DataBlock object the received data
	 *                will be deserialized into.
	 * \param[in] dh  IbeoDataHeader that came along with
	 *                the DataBlock.
	 * \sa IbeoDeviceBase::deserialize
	 *///-------------------------------------
	const DataBlock* deserialize(std::istream& is,
	                             DataBlock& db,
	                             const IbeoDataHeader& dh);

	//========================================
	/*!\brief Unregister all streamer and listener.
	 *///-------------------------------------
	void unregisterAll();

private:
	virtual bool writeHeader(const IbeoDataHeader& dh);
	virtual bool write(const char* const data, const uint32_t length);

protected:
	//========================================
	/*!\brief Map that holds all listener that are registrated.
	 *
	 * Holds for each DataTypeId for which listener have
	 * been registrated a list of those listener.
	 *///-------------------------------------
	ListenerListMap m_listeners;

	//========================================
	/*!\brief The list of registrated streamer.
	 *///-------------------------------------
	StreamerList m_streamers;

	std::ios_base::openmode m_openMode;
	FrameIndex m_frameIndex;

	DataBlockBufferProvider<IdcFile> dbbp;

private:
	//========================================
	/*!\brief File stream for io access to the file.
	 *///-------------------------------------
	std::fstream m_strm;

	uint32_t m_latestMsgSize;
	NTPTime m_latestHeaderTimestamp;
	NTPTime m_firstHeaderTimestamp;

	//========================================
	/*!\brief Variable to hold the deserialization
	 *        of a IbeoDataHeader.
	 *///-------------------------------------
	IbeoDataHeader m_dh;

	bool m_firstDataBlockToBeWritten;
}; // IdcFile

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IDCFILE_HPP_SEEN
