//======================================================================
/*! \file IbeoTcpIpAcceptorScala.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jun 02 2016
 *///-------------------------------------------------------------------

//======================================================================

#ifndef IBEOSDK_IBEOTCPIPACCEPTORSCALA_HPP_SEEN
#define IBEOSDK_IBEOTCPIPACCEPTORSCALA_HPP_SEEN

//======================================================================

#include <ibeosdk/devices/IbeoTcpIpAcceptorBase.hpp>

#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class IbeoTcpIpAcceptorScala
 * \brief Class for accepting connections via TCP/IP and sending data
 *        via this connection, behaving like a Scala.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Jun 02, 2016
 *///-------------------------------------------------------------------
class IbeoTcpIpAcceptorScala : public IbeoTcpIpAcceptorBase {
public:
	//========================================
	/*!\brief Convenience typedef for boost::shared_ptr.
	 *///-------------------------------------
	typedef boost::shared_ptr<IbeoTcpIpAcceptorScala> Ptr;

private:
	//======================================================================
	/*!\brief Handles connections and act like a Scala sensor.
	 * \author Jan Christian Dittmer (jcd)
	 * \version 0.1
	 * \date Jun 02, 2016
	 *///-------------------------------------------------------------------
	class SessionScala : public IbeoTcpIpAcceptorBase::SessionBase {
	public:
		//========================================
		/*!\brief Convenience typedef for boost::shared_ptr.
		 *///-------------------------------------
		typedef boost::shared_ptr<SessionScala> Ptr;

	public:
		//========================================
		/*!\brief Creates a SessionScala
		 *
		 * \param[in] io_service      Service which
		 *                            established connection.
		 * \param[in] deviceId        Id of our simulated device
		 *                            needed for ibeo data header.
		 *///-------------------------------------
		SessionScala(IbeoTcpIpAcceptorBase* const parent,
		             boost::asio::io_service& io_service,
		             const UINT8 deviceId = 1);

		//========================================
		/*!\brief Destructs a SessionScala.
		 *///-------------------------------------
		virtual ~SessionScala(){}

	protected:
		//========================================
		/*!\brief Worker function for m_ListenThreadPtr.
		 *
		 * Doing nothing. A Scala sensor does not
		 * listen to ethernet commands.
		 *///-------------------------------------
		virtual void startListen();
	}; //IbeoTcpIpAcceptorScala::SessionScala

protected:
	static const int msgBufferSize = 4*65536;

public:
	//========================================
	/*!\brief Creates an IbeoTcpIpAcceptorScala.
	 *
	 * \param[in] logFileManager  LogFileManager which is
	 *                            handle the splitting of
	 *                            output files and log files.
	 * \param[in] port            Port number for the
	 *                            connection.
	 *///-------------------------------------
	IbeoTcpIpAcceptorScala(ibeosdk::LogFileManager* const logFileManager, const unsigned short port = 12004);

	//========================================
	/*!\brief Creates an IbeoTcpIpAcceptorScala.
	 *
	 * \param[in] logFileManager       LogFileManager which is
	 *                                 handle the splitting of
	 *                                 output files and log files.
	 * \param[in  writeExpirationTime  Timeout limit for async
	 *                                 write operations.
	 * \param[in] port                 Port number for the
	 *                                 connection.
	 *///-------------------------------------
	IbeoTcpIpAcceptorScala(ibeosdk::LogFileManager* const logFileManager,
	                       const boost::asio::deadline_timer::duration_type writeExpirationTime,
	                       const unsigned short port = 12004);

	//========================================
	/*!\brief Destructs an IbeoTcpIpAcceptorScala.
	 *///-------------------------------------
	virtual ~IbeoTcpIpAcceptorScala(){}

protected:
	//========================================
	/*!\brief Gets current session ptr.
	 * \return Session ptr casted to sessionBase.
	 *///-------------------------------------
	virtual IbeoTcpIpAcceptorBase::SessionBase::Ptr getSessionPtr() { return m_sessionPtr; }

	//========================================
	/*!\brief Gets new session ptr initialized
	 *        with io_service.
	 * \param[in] io_service  Service which handles
	 *                        connections.
	 * \return Session ptr casted to sessionBase.
	 *///-------------------------------------
	virtual IbeoTcpIpAcceptorBase::SessionBase::Ptr getNewSessionPtr(boost::asio::io_service& io_service);

private:
	//========================================
	/*!\brief Session ptr for next session which
	 *        will be established.
	 *///-------------------------------------
	SessionScala::Ptr m_sessionPtr;
}; // IbeoTcpIpAcceptorScala

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IBEOTCPIPACCEPTORSCALA_HPP_SEEN

//======================================================================
