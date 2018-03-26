//======================================================================
/*! \file IbeoTcpIpAcceptorEcu.cpp
 *
 * \copydoc Copyright
 * \author Julia Nitsch (jn)
 * \date May 17, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/devices/IbeoTcpIpAcceptorEcu.hpp>
#include <ibeosdk/CommandId.hpp>
#include <ibeosdk/datablocks/commands/CommandEcuSetFilter.hpp>
#include <ibeosdk/datablocks/commands/ReplyEcuSetFilter.hpp>
#include <ibeosdk/MsgBuffer.hpp>

#include <boost/iostreams/stream.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

IbeoTcpIpAcceptorEcu::SessionEcu::SessionEcu(IbeoTcpIpAcceptorBase* const parent,
                                             boost::asio::io_service& io_service,
                                             const boost::optional<boost::asio::deadline_timer::duration_type> readExpirationTime,
                                             const UINT8 deviceId)
  : IbeoTcpIpAcceptorBase::SessionBase(parent, io_service, DataTypeId::DataType_LastId, DataTypeId::DataType_LastId, deviceId),
    m_expiryTime(readExpirationTime),
    m_timer(io_service),
    m_msgBuffer(msgBufferSize)
{}

//======================================================================

void IbeoTcpIpAcceptorEcu::SessionEcu::startListen()
{
	logDebug2 << "Start listening  " << (void*)(this) << std::endl;
	if (m_socket.is_open()) {
		startTimer();
		startRead();
	}
}

//======================================================================

void IbeoTcpIpAcceptorEcu::SessionEcu::startTimer()
{
	if (m_expiryTime)
		m_timer.expires_from_now(m_expiryTime.get());
	else
		m_timer.expires_from_now(boost::posix_time::seconds(1));
	m_timer.async_wait(boost::bind(&IbeoTcpIpAcceptorEcu::timeout, this, _1));
}

//======================================================================

void IbeoTcpIpAcceptorEcu::SessionEcu::startRead()
{
	char* const recvBuf = m_msgBuffer.getRecvBuf();
	const int bytesToRead = m_msgBuffer.getBytesToRead();
	const boost::asio::mutable_buffers_1& buffers = boost::asio::buffer(recvBuf, std::size_t(bytesToRead));

	if (m_socket.is_open()) {
		m_socket.async_read_some(buffers, boost::bind(&IbeoTcpIpAcceptorEcu::listen, this, _1, _2));
	}
}

//======================================================================

void IbeoTcpIpAcceptorEcu::SessionEcu::listen(const boost::system::error_code& error, const size_t nbOfBytesReceived)
{
	if (!error) {
		logDebug2 << "Received " << nbOfBytesReceived << " byte." << std::endl;
		//m_msgBuffer.increaseInsPos(int(nbOfBytesReceived));

		const IbeoDataHeader* recvDataHeader;
		const char* startOfMsgBody;

		// process all received commands
		while (m_msgBuffer.getMessage(recvDataHeader, startOfMsgBody, int(nbOfBytesReceived))) {

			logDebug2 << "Received data block " << toHex(recvDataHeader->getDataType()) << std::endl;

			if (recvDataHeader && (recvDataHeader->getDataType() == DataTypeId::DataType_Command)) {
				onReceiveCommand(*recvDataHeader, startOfMsgBody);
				continue;
			}
		}

		if (m_socket.is_open()) {
			startRead();
		}
	}
	else {
		logError << error.message() << std::endl;
		m_timer.cancel();
	}
}

//======================================================================

void IbeoTcpIpAcceptorEcu::SessionEcu::readTimeout(const boost::system::error_code& error)
{
	if (m_socket.is_open()) {
		if (!error) {
			logDebug2 << "Restart read timer" << std::endl;
			startTimer();
		}
		else { // open or error occurred
			logDebug << error.message() << std::endl;
			logDebug2 << "Canceling read operation" << std::endl;
			m_socket.cancel();
		}
	}
}

//======================================================================

IbeoTcpIpAcceptorEcu::IbeoTcpIpAcceptorEcu(ibeosdk::LogFileManager* const logFileManager,
                                           const unsigned short port)
  : IbeoTcpIpAcceptorBase(logFileManager, port),
    m_readExpirationTime()
{}

//======================================================================

IbeoTcpIpAcceptorEcu::IbeoTcpIpAcceptorEcu(LogFileManager* const logFileManager,
                                           const boost::optional<boost::asio::deadline_timer::duration_type> expirationTime,
                                           const unsigned short port)
  : IbeoTcpIpAcceptorBase(logFileManager, expirationTime, port),
    m_readExpirationTime(expirationTime)
{}

//======================================================================

IbeoTcpIpAcceptorEcu::IbeoTcpIpAcceptorEcu(LogFileManager* const logFileManager,
                                           const boost::optional<boost::asio::deadline_timer::duration_type> readExpirationTime,
                                           const boost::asio::deadline_timer::duration_type writeExpirationTime,
                                           const unsigned short port)
  : IbeoTcpIpAcceptorBase(logFileManager, writeExpirationTime, port),
    m_readExpirationTime(readExpirationTime)
{}

//======================================================================

IbeoTcpIpAcceptorBase::SessionBase::Ptr IbeoTcpIpAcceptorEcu::getNewSessionPtr(boost::asio::io_service& io_service)
{
	m_sessionPtr.reset(new SessionEcu(this, io_service, m_readExpirationTime));
	return  m_sessionPtr;
}

//======================================================================

void IbeoTcpIpAcceptorEcu::SessionEcu::onReceiveCommand(const IbeoDataHeader& dh, const char* bodyBuf)
{
	{
		const uint8_t* bb = reinterpret_cast<const uint8_t*>(bodyBuf);
		logDebug2 << "Incomming Command: " << toHex(uint16_t(*(bb+1) + ((*(bb))<<8)))
				<< "  Size: " << dh.getMessageSize() << std::endl;
	}

	ibeosdk::CommandId cmd_id(0); // set dummy id
	boost::iostreams::stream<boost::iostreams::array_source> stream(bodyBuf, dh.getMessageSize());
	std::istream& s = (std::istream&)stream;
	cmd_id.readBE(s);

	if (cmd_id == ibeosdk::CommandId::CmdManagerSetFilter) {
		// serialize CmdSetFilter because we want to apply that filter
		// dummy construct, will be filled with stream
		ibeosdk::CommandEcuSetFilter cmdEcuSetFilter(ibeosdk::DataTypeId::DataType_Unknown, ibeosdk::DataTypeId::DataType_Unknown);
		boost::iostreams::stream<boost::iostreams::array_source> cmd_stream(bodyBuf, dh.getMessageSize());
		cmdEcuSetFilter.deserialize(cmd_stream, dh);

		setRange(cmdEcuSetFilter.getRangeStart(), cmdEcuSetFilter.getRangeEnd());

		ibeosdk::ReplyEcuSetFilter rplyEcuSetFilter;
		sendDatablock(rplyEcuSetFilter);
	}
}

//======================================================================

} // namespace ibeosdk

//======================================================================
