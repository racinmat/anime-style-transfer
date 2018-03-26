//======================================================================
/** \file IbeoSutpInterface.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Aug 31, 2016
 *///-------------------------------------------------------------------

//======================================================================

#ifndef IBEOSDK_IBEOSUTPINTERFACE_HPP
#define IBEOSDK_IBEOSUTPINTERFACE_HPP

//======================================================================

#include <ibeosdk/datablocks/snippets/SutpHeader.hpp>
#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/function.hpp>

#include <istream>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class IbeoSutpInterface
 * \brief Implementation of SUTP (receiving part).
 * \author Kai-Uwe von Deylen (kd)
 * \version 0.1
 * \date Aug 31, 2016
 *
 * Devices that use this interface communicate data using the Ibeo data
 * layer protocol on top of SUTP.
 *///-------------------------------------------------------------------
class IbeoSutpInterface {
public:
	//========================================
	/*!\brief SUTP payload as raw data
	*///--------------------------------------
	typedef std::vector<char> SutpPayload;

	//========================================
	/*!\brief Smart pointer to SUTP payload
	*///--------------------------------------
	typedef boost::shared_ptr<SutpPayload> SutpPayloadPtr;

	//========================================
	/*!\brief Smart pointer to SutpHeader
	*///--------------------------------------
	typedef boost::shared_ptr<SutpHeader> SutpHeaderPtr;

	//========================================
	/*!\brief Single sudp fragment
	*///--------------------------------------
	typedef std::pair<SutpHeaderPtr, SutpPayloadPtr> SudpFragment;

	typedef boost::unordered_map<uint16_t, SudpFragment> CacheType;

	typedef std::pair<uint16_t, SudpFragment> MapEntry;

	//========================================
	/*!\brief Signature of the handler that
	 *        processes received messages.
	 *///-------------------------------------
	typedef boost::function<bool(const char* bodyBuf, size_t len)> MessageHandler;

public:
	static const uint64_t sutpDefaultTimeout;
	static const uint64_t sutpFragmentSize;
	static const uint32_t defaultErrorResetLimit;

public:

	//========================================
	/*!\brief Private Constructor.
	 *
	 * Constructor is private because this
	 * class must only be used through the
	 * boost::shared_ptr. Use create()
	 * to instantiate.
	*///--------------------------------------
	IbeoSutpInterface(MessageHandler msgHandler,
	                  const uint64_t timeout = sutpDefaultTimeout,
	                  const uint32_t errResetLimit = defaultErrorResetLimit);

	//========================================
	/*!\brief Destructor
	 *
	 * Closes the connection if necessary.
	 *///-------------------------------------
	virtual ~IbeoSutpInterface();

private:
	//========================================
	/*!\brief Forbidden default constructor.
	 *///-------------------------------------
	IbeoSutpInterface(); // forbidden

public:
	//========================================
	/*!\brief Sets the callback function of
	 *        the receiver.
	 *///-------------------------------------
	void setMessageHandler(MessageHandler msgHandler);

	//========================================
	/*!\brief Deregister the MessageHandler.
	 *
	 * This method should be called by the
	 * object owning the MessageHandler if it
	 * is being destructed, so that this
	 * IbeoTcpIpInterface does not
	 * accidentally call a handler on a
	 * deleted object.
	 *///-------------------------------------
	void deregisterMessageHandler();

	//========================================
	/*!\brief Processes incoming data.
	 *///-------------------------------------
	bool onReceiveRaw(std::istream& is, const uint32_t messageSize);

	//========================================
	/*!\brief check cache for available
	 *        IbeoDataLayer paket and process.
	 *///-------------------------------------
	void processCache();

	boost::condition& getPacketCompleteCondition() { return m_paketComplete; }
	boost::recursive_mutex& getPacketCompleteMutex() { return m_paketCacheMutex; }

private:
	//========================================
	/*!\brief processes a single IbeoDataLayer
	 *        paket.
	 *///-------------------------------------
	void processPaket();

	bool findSegNo(CacheType::const_iterator& fIter, const uint16_t seqNo);

		//========================================
	/*!\brief checks if the oldest paket is
	 *        complete.
	 *///-------------------------------------
	bool isPacketComplete();

	//========================================
	/*!\brief removes outdated fragments from
	 *        cache.
	 *///-------------------------------------
	void clearOutdatedFragments();

	//========================================
	/*!\brief resets the internal state to
	 *        initial values.
	 *///-------------------------------------
	void reset();


protected:
	//! timeout for SUTP packets
	const uint64_t m_sutpTimeout;

	const uint32_t m_errorResetLimit;

protected:
	//! map containing cached fragments. Key = seqNb
	boost::unordered_map<uint16_t, SudpFragment> m_fragmentCache;

	//! next SUTP sequence number to be processed.
	//! this has to always be the seqNb of the first fragment
	uint16_t m_nextSeqNbProc;

	//! indicates if #m_nextSeqNb contains invalid value.
	//! should be permanently false after receiving first fragment
	bool m_nextSeqNbProcInvalid;

	//! current SUTP time. used to detect outdated pakets
	uint64_t m_sutpTime;

	//! mutex for synchronizing receive thread and callbackThread
	boost::recursive_mutex m_paketCacheMutex;

	//! condition for signaling complete ibeoDataLayer pakets
	boost::condition m_paketComplete;

	//! error counter (reinitialization trigger)
	uint16_t m_numConsecutiveErrors;

	//! Message handler callback for complete ibeo data message
	MessageHandler m_onReceive;

	uint32_t m_nbOfReceivedPackages;
	uint32_t m_nbOfDroppedPackages;
}; // IbeoSutpInterface

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IBEOSUTPINTERFACE_HPP

//======================================================================
