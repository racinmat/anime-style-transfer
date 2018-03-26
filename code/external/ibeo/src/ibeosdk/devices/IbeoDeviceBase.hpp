//======================================================================
/*! \file IbeoDeviceBase.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 7, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_IBEODEVICEBASE_HPP_SEEN
#define IBEOSDK_IBEODEVICEBASE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/DataTypeId.hpp>
#include <ibeosdk/EventMonitor.hpp>
#include <ibeosdk/datablocks/commands/Command.hpp>
#include <ibeosdk/CommandId.hpp>
#include <ibeosdk/misc/StatusCodes.hpp>


#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/unordered_map.hpp>

#include <iostream>
#include <list>

//======================================================================

namespace ibeosdk {

class IbeoDataHeader;
class DataBlock;
class DataListenerBase;
class DataStreamer;
class LogFileManager;

//======================================================================
/*!\class IbeoDeviceBase
 * \brief Base class for all Ibeo devices.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Apr 07, 2015
 *///-------------------------------------------------------------------
class IbeoDeviceBase : private boost::noncopyable {
protected:
	enum ThreadState {
		TS_NotRunning,
		TS_Starting,
		TS_Running,
		TS_Stopping,
		TS_StartFailed,
		TS_RunFailed
	}; // ThreadState
	typedef boost::mutex::scoped_lock Lock;
	typedef boost::mutex Mutex;
	typedef boost::condition Condition;

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
	IbeoDeviceBase();
	virtual ~IbeoDeviceBase();

public:
	//========================================
	/*!\brief Establish the connection to the
	 *        hardware.
	 *
	 * Starting the receiving thread.
	 *///-------------------------------------
	virtual void getConnected() = 0;

	//========================================
	/*!\brief Disconnect the connection
	 *        to the hardware device.
	 *///-------------------------------------
	virtual void disconnect();

	//========================================
	/*!\brief Checks whether the connection
	 *        to the hardware device is there and
	 *        the receiving thread is still running.
	 * \return \c true if messages from the hardware
	 *         can still be received. \c false otherwise.
	 *///-------------------------------------
	virtual bool isConnected() = 0;

	//========================================
	/*!\brief Register a DataListener to this device.
	 *
	 * Each time a message has been received by
	 * this object, the registered listener will be
	 * called which are listening to the received message
	 * type (DataType).
	 * \param[in] listener  Listener to be registered.
	 * \note There is (currently) no way to unregister a
	 *       listener, so a registered DataListener must
	 *       not be destroyed before this IbeoDevice
	 *       has been destroyed.
	 * \warning the methodology of this method have changed
	 * the user do not need to register DataListeners for
	 * each DataType invidually
	 *///-------------------------------------
	virtual void registerListener(DataListenerBase* const listener);


	//========================================
	/*!\brief Unregister a listener.
	 * \param[in] listener  Address of the listener object to
	 *                      be unregistered.
	 * \return Result of the operation.
	 *///-------------------------------------
	virtual statuscodes::Codes unregisterListener(DataListenerBase* const listener);

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
	 *///-------------------------------------
	virtual void registerStreamer(DataStreamer* const streamer);

	//========================================
	/*!\brief Unregister a streamer.
	 * \param[in] streamer  Address of the streamer object to
	 *                      be unregistered.
	 * \return Result of the operation.
	 *///-------------------------------------
	virtual statuscodes::Codes unregisterStreamer(DataStreamer* const streamer);

public:
	//========================================
	/*!\brief Set the LogFileManager.
	 *
	 * \param[in] logFileManager  LogFileManager which is
	 *                            handle the splitting of
	 *                            output files and log files.
	 *///-------------------------------------
	void setLogFileManager(LogFileManager* const logFileManager);

protected:
	//========================================
	/*!\brief Unregister all streamer and listener.
	 *///-------------------------------------
	void unregisterAll();

	//========================================
	/*!\brief This method will be called by the receive thread
	 *        when a new DataBlock has been received completely.
	 *
	 * This class will call notifyListeners and notifyStreamers.
	 *
	 * \param[in]       dh  Header that came along with that
	 *                      DataBlock.
	 * \param[in, out]  is  Stream that contains the contents of
	 *                      the DataBlock that has been received.
	 *///-------------------------------------
	virtual const DataBlock* onData(const IbeoDataHeader& dh, std::istream& is) = 0;

	//========================================
	/*!\brief Send a command which expects no reply.
	 * \param[in] cmd  Command to be sent.
	 * \return The result of the operation.
	 *///-------------------------------------
	virtual statuscodes::Codes sendCommand(const CommandBase& cmd) = 0;

	//========================================
	/*!\brief Send a command and wait for a reply.
	 *
	 * The command will be sent. The calling thread
	 * will sleep until a reply has been received
	 * but not longer than the number of milliseconds
	 * given in \a timeOut.
	 *
	 * \param[in]       cmd    Command to be sent.
	 * \param[in, out]  reply  The reply container for
	 *                         the reply to be stored into.
	 * \param[in]       timeOut  Number of milliseconds to
	 *                           wait for a reply.
	 * \return The result of the operation.
	 *///-------------------------------------
	virtual statuscodes::Codes sendCommand(const CommandBase& cmd,
	                                       CommandReplyBase& reply,
	                                       const boost::posix_time::time_duration timeOut = boost::posix_time::milliseconds(500)) = 0;

	//========================================
	/*!\brief Call all registered Listener listening to the
	 *        received type of DataBlock.
	 * \param[in] data  DataBlock that has been received.
	 *///-------------------------------------
	virtual void notifyListeners(const DataBlock* const data);

	//========================================
	/*!\brief Call all registered Streamers listening to the
	 *        received type of DataBlock.
	 * \param[in] dh       Header that came along with that
	 *                     DataBlock.
	 * \param[in] bodyBuf  Buffer that contains the
	 *                     (still serialized) body of
	 *                     the received DataBlock.
	 *///-------------------------------------
	virtual void notifyStreamers(const IbeoDataHeader& dh, const char* const bodyBuf);

	//========================================
	/*!\brief Deserialize a received DataBlock into \a db.
	 * \param[in] is  Stream that contains the body of the
	 *                DataBlock.
	 * \param[in] db  The DataBlock object the received data
	 *                will be deserialized into.
	 * \param[in] dh  IbeoDataHeader that came along with
	 *                the DataBlock.
	 *///-------------------------------------
	virtual const DataBlock* deserialize(std::istream& is,
	                                     DataBlock& db,
	                                     const IbeoDataHeader& dh);

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

	LogFileManager* m_logFileManager;

}; // IbeoDeviceBase

//======================================================================

std::ostream& operator<<(std::ostream& os, const statuscodes::Codes ec);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IBEODEVICEBASE_HPP_SEEN

//======================================================================
