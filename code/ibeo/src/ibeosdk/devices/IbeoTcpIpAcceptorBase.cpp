//======================================================================
/*! \file IbeoTcpIpAcceptorBase.cpp
 *
 * \copydoc Copyright
 * \author Julia Nitsch (jn)
 * \date May 17, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/IbeoTcpIpAcceptorBase.hpp>

#include <ibeosdk/datablocks/DataBlock.hpp>
#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>
#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/MsgBuffer.hpp>

#include <boost/asio/error.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/system/error_code.hpp>

#include <fstream>
#include <iostream>

//======================================================================

namespace ibeosdk {

//======================================================================

IbeoTcpIpAcceptorBase::SessionBase::SessionBase(IbeoTcpIpAcceptorBase* const parent,
                                                boost::asio::io_service& io_service,
                                                const UINT8 deviceId)
  : m_parent(parent),
    m_socket(io_service),
    m_deviceId(deviceId),
    m_sizePrevMsg(0),
    m_connectionALive(false),
    m_startRange(DataTypeId::DataType_Unknown),
    m_endRange(DataTypeId::DataType_LastId),
    m_sendBuffer()
{}

//======================================================================

IbeoTcpIpAcceptorBase::SessionBase::SessionBase(IbeoTcpIpAcceptorBase* const parent,
                                                boost::asio::io_service& io_service,
                                                const DataTypeId rangeStart,
                                                const DataTypeId rangeEnd,
                                                const UINT8 deviceId)
  : m_parent(parent),
    m_socket(io_service),
    m_deviceId(deviceId),
    m_sizePrevMsg(0),
    m_connectionALive(false),
    m_startRange(rangeStart),
    m_endRange(rangeEnd),
    m_sendBuffer()
{}

//======================================================================

IbeoTcpIpAcceptorBase::SessionBase::~SessionBase()
{
	m_socket.close();
}

//======================================================================

statuscodes::Codes IbeoTcpIpAcceptorBase::SessionBase::sendDatablock(const DataBlock& dataBlock)
{
	// lock -> not only sending thread can write but listen thread can also write replies utilizing that function!
	boost::mutex::scoped_lock lock(m_sendMutex);

	// id must lie within filter start and end range
	if ( !((m_startRange <= dataBlock.getDataType()) && (dataBlock.getDataType() <= m_endRange))) {
		logDebug2 << "Block DataBlock " << toHex(dataBlock.getDataType()) << std::endl;
		return statuscodes::DataBlockBlockedByFilter;
	}

	const uint32_t sizeOfThisMsg = uint32_t(dataBlock.getSerializedSize());
	const uint32_t sizeOfBuffer = sizeOfThisMsg + IbeoDataHeader::getHeaderSize();

	m_sendBuffer.resize(sizeOfBuffer);
	boost::iostreams::stream<boost::iostreams::array> strm(&(m_sendBuffer[0]), sizeOfBuffer);
	std::ostream& s = (std::ostream&)strm;

	logDebug2 << "Sending DataBlock " << toHex(dataBlock.getDataType()) << " with " <<  sizeOfThisMsg+24 << " bytes" << std::endl;

	const IbeoDataHeader header(dataBlock.getDataType(), m_sizePrevMsg, sizeOfThisMsg, m_deviceId, dataBlock.getHeaderNtpTime());
	header.serialize(s);
	dataBlock.serialize(s);

	if ( !s.good()) {
		logError << "Sending failed " <<  statuscodes::FailedToPrepareSendingDataBlock << std::endl;
		return statuscodes::FailedToPrepareSendingDataBlock;
	}

	if ( !m_socket.is_open()) {
		logError << "Socket is not connected" << std::endl;
		return statuscodes::NotConnected;
	}

	try {
		if (m_socket.is_open()) {
			boost::asio::async_write(m_socket, boost::asio::buffer(&(m_sendBuffer[0]), sizeOfBuffer),
				boost::bind(&IbeoTcpIpAcceptorBase::writeDone, m_parent, this, _1, _2));
		}
	}
	catch (const boost::system::system_error& ex) {
		logError << "Sending failed: " << ex.what() << std::endl;
		m_connectionALive = false;
		return statuscodes::SendingDataBlockFailed;
	}

	return statuscodes::EverythingOk;
}

//======================================================================





//======================================================================
//======================================================================
//======================================================================





//======================================================================

IbeoTcpIpAcceptorBase::IbeoTcpIpAcceptorBase(ibeosdk::LogFileManager* const logFileManager, const short unsigned int port)
  : m_sessionPtr(),
    m_logFileManager(logFileManager),
    m_ioService(),
    m_sessionsMutex(),
    m_sessions(),
    m_activeSending(),
    m_sendingStatus(),
    m_writeCondMutex(),
    writeCondition(),
    m_writeExpirationPeriod(boost::posix_time::seconds(10)),
    m_writeExprirationTimer(m_ioService),
    m_writeState(WS_Idle),
    m_acceptor(m_ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    m_acceptIOServiceThreadPtr()
{}

//======================================================================

IbeoTcpIpAcceptorBase::IbeoTcpIpAcceptorBase(ibeosdk::LogFileManager* const logFileManager,
                                             const boost::asio::deadline_timer::duration_type writeExpirationTime,
                                             const short unsigned int port)
  : m_sessionPtr(),
    m_logFileManager(logFileManager),
    m_ioService(),
    m_sessionsMutex(),
    m_sessions(),
    m_activeSending(),
    m_sendingStatus(),
    m_writeCondMutex(),
    writeCondition(),
    m_writeExpirationPeriod(writeExpirationTime),
    m_writeExprirationTimer(m_ioService),
    m_writeState(WS_Idle),
    m_acceptor(m_ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    m_acceptIOServiceThreadPtr()
{}

//======================================================================

IbeoTcpIpAcceptorBase::IbeoTcpIpAcceptorBase(ibeosdk::LogFileManager* const logFileManager,
                                             const boost::optional<boost::asio::deadline_timer::duration_type> writeExpirationTime,
                                             const short unsigned int port)
  : m_sessionPtr(),
    m_logFileManager(logFileManager),
    m_ioService(),
    m_sessionsMutex(),
    m_sessions(),
    m_activeSending(),
    m_sendingStatus(),
    m_writeCondMutex(),
    writeCondition(),
    m_writeExpirationPeriod(writeExpirationTime ? writeExpirationTime.get() :boost::posix_time::seconds(10)),
    m_writeExprirationTimer(m_ioService),
    m_writeState(WS_Idle),
    m_acceptor(m_ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    m_acceptIOServiceThreadPtr()
{}

//======================================================================

void IbeoTcpIpAcceptorBase::init()
{
	m_sessionPtr = getNewSessionPtr(m_ioService);
	m_acceptor.async_accept(m_sessionPtr->getSocket(), boost::bind(&IbeoTcpIpAcceptorBase::handleAccept, this, boost::asio::placeholders::error));

	// starting thread which handles io service
	m_acceptIOServiceThreadPtr.reset(new boost::thread(boost::bind(&IbeoTcpIpAcceptorBase::acceptorIOServiceThread, this)));
}

//======================================================================

IbeoTcpIpAcceptorBase::~IbeoTcpIpAcceptorBase()
{
	closeAcceptor();
}

//======================================================================

void IbeoTcpIpAcceptorBase::issueWriteOperations(const DataBlock& dataBlock)
{
	boost::mutex::scoped_lock lock(m_sessionsMutex);

	m_sendingStatus = statuscodes::EverythingOk;
	if (m_sessions.empty()) {
		logDebug2 << "No session accepted. Skipping DataBlock " << toHex(dataBlock.getDataType()) << std::endl;
		m_writeState = WS_Completed;
		return;
	}

	const uint32_t nbOfBytesToSend = uint32_t(dataBlock.getSerializedSize() + IbeoDataHeader::getHeaderSize());

	std::vector<SessionBase::Ptr>::iterator sessionIt = m_sessions.begin();
	while (sessionIt != m_sessions.end()) {
		// check if connection is still valid otherwise delete it from our sessions

		if ( !((*sessionIt)->isConnectionValid())) {
			sessionIt = m_sessions.erase(sessionIt);
			if (m_sessions.empty()) {
				logDebug2 << "No session accepted. Skipping DataBlock " << toHex(dataBlock.getDataType()) << std::endl;
				m_writeState = WS_Completed;
				return;
			}
			continue;
		}

		if ((*sessionIt)->isConnectionValid()) { // just to be sure (timing)
			boost::mutex::scoped_lock sendLock(m_writeCondMutex);

			const statuscodes::Codes codeCurrent = (*sessionIt)->sendDatablock(dataBlock);
			if (codeCurrent == statuscodes::EverythingOk) {
				m_writeState = WS_InProgress;

				m_activeSending.insert(std::make_pair(&**sessionIt, nbOfBytesToSend));

				logDebug2 << "Adding " << (void*)(&**sessionIt) << " to set. Datatype " << toHex(dataBlock.getDataType())
						<< " Size: " << dataBlock.getSerializedSize()+24 << std::endl;
				logDebug2 << "Size of set: " << m_activeSending.size() << std::endl;
			}

			if (codeCurrent != statuscodes::EverythingOk) {
				m_sendingStatus = codeCurrent;
			}
		}

		++sessionIt;
	}

	return;
}

//======================================================================

void IbeoTcpIpAcceptorBase::waitForWriteOperationsBeCompleted()
{
	// wait for all writes been completed
	boost::mutex::scoped_lock sendLock(m_writeCondMutex);
	if (!m_activeSending.empty()) {
		writeCondition.wait(sendLock);
	}

	if (m_writeState != WS_TimeOut)
		m_writeExprirationTimer.cancel();

}

//======================================================================

statuscodes::Codes IbeoTcpIpAcceptorBase::sendDataBlock(const DataBlock& dataBlock)
{
	issueWriteOperations(dataBlock);

	m_writeExprirationTimer.expires_from_now(m_writeExpirationPeriod);
	m_writeExprirationTimer.async_wait(boost::bind(&IbeoTcpIpAcceptorBase::writeTimeout, this, _1));

	waitForWriteOperationsBeCompleted();

	return m_sendingStatus;
}

//======================================================================

void IbeoTcpIpAcceptorBase::handleAccept(const boost::system::error_code& error)
{
	if (!error) {
		{
			boost::mutex::scoped_lock lock(m_sessionsMutex);
			m_sessions.push_back(getSessionPtr());
			getSessionPtr()->startListening();
			getSessionPtr()->setConnectionALive(true);
		}

		m_sessionPtr = getNewSessionPtr(m_ioService);
		m_acceptor.async_accept(m_sessionPtr->getSocket(),
		                        boost::bind(&IbeoTcpIpAcceptorBase::handleAccept,
		                                    this,
		                                    boost::asio::placeholders::error));
	}
	else {
		logError << "not able to handle accept with error " << error << std::endl;
	}
}


//======================================================================

void IbeoTcpIpAcceptorBase::closeAcceptor()
{
	m_ioService.stop();
	m_acceptIOServiceThreadPtr->join();
}

//======================================================================

void IbeoTcpIpAcceptorBase::acceptorIOServiceThread()
{
	m_ioService.run();
}

//======================================================================

void IbeoTcpIpAcceptorBase::writeDone(SessionBase* const session,
                                      const boost::system::error_code& error,
                                      const std::size_t nbOfBytesTransfered)
{
	if (error) {
		logError << (void*)(session) << " Error: " << error.message() << std::endl;
		m_sendingStatus = statuscodes::SendingDataBlockFailed;

		boost::mutex::scoped_lock sendLock(m_writeCondMutex);

		m_activeSending.erase(session);

		// wakeup receiver
		session->setConnectionALive(false);
		if (m_writeState != WS_TimeOut)
			m_writeState = WS_Error;

		writeCondition.notify_all();
		return;
	}

	session->setSizeOfPrevMsg(uint32_t(nbOfBytesTransfered-ibeosdk::IbeoDataHeader::getHeaderSize()));

	{
		boost::mutex::scoped_lock sendLock(m_writeCondMutex);

		std::map<SessionBase*, uint32_t>::iterator fIter = m_activeSending.find(session);
		if (fIter != m_activeSending.end()) {

			logDebug2 << "Handling " << (void*)(session) << "Size: " << nbOfBytesTransfered
					<< " expected: " << fIter->second
					<< std::endl;

			if (fIter->second > nbOfBytesTransfered) {
				fIter->second -= uint32_t(nbOfBytesTransfered);
			}
			else {
				m_activeSending.erase(fIter);
			}
		}

		if (m_activeSending.empty()) {
			// wakeup receiver
			m_writeState = WS_Completed;

			writeCondition.notify_all();
			m_writeExprirationTimer.cancel();
		}
	}
}

//======================================================================

void IbeoTcpIpAcceptorBase::writeTimeout(const boost::system::error_code& )
{
	boost::mutex::scoped_lock sendLock(m_writeCondMutex);

	if (m_writeState == WS_InProgress) {
		m_writeState = WS_TimeOut;
		cancelWriteOperations();
		writeCondition.notify_all();
	}
	return;
}

//======================================================================

void IbeoTcpIpAcceptorBase::cancelWriteOperations()
{
	std::map<SessionBase*, uint32_t>::iterator sessionIter = m_activeSending.begin();
	for (; sessionIter != m_activeSending.end(); ++sessionIter) {
		sessionIter->first->cancelAsyncIos();
	}
}

////======================================================================
//
//void IbeoTcpIpAcceptorEcu::SessionEcu::readTimeout(const boost::system::error_code& error)
//{
//	if (m_socket.is_open()) {
//		if (!error) {
//			logDebug2 << "Restart read timer" << std::endl;
//			startTimer();
//		}
//		else { // open or error occurred
//			logDebug << error.message() << std::endl;
//			logDebug2 << "Canceling read operation" << std::endl;
//			m_socket.cancel();
//		}
//	}
//}
//======================================================================

}// namespace ibeosdk

//======================================================================
