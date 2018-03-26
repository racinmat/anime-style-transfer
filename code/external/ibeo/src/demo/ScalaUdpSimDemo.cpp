//======================================================================
/*! \file ScalaUdpSimDemo.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd))
 * \date Apr 27, 2017
 *
 * Demo project for simulating an Scala device sendings its data
 * via UDP
 *///-------------------------------------------------------------------

#include <ibeosdk/lux.hpp>
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>
#include <ibeosdk/scala.hpp>

#include <ibeosdk/devices/IdcFile.hpp>

#include <ibeosdk/devices/sutp/IbeoSutpSendInterface.hpp>

#include <boost/asio.hpp>

#include <iostream>
#include <cstdlib>

//======================================================================

using namespace ibeosdk;

//======================================================================

const ibeosdk::Version::MajorVersion majorVersion(5);
const ibeosdk::Version::MinorVersion minorVersion(2);
const ibeosdk::Version::Revision revision(2);
const ibeosdk::Version::PatchLevel patchLevel;
const ibeosdk::Version::Build build;
const std::string info = "ScalaUdpSimDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);

IbeoSDK ibeoSDK;

//======================================================================

TimeConversion tc;

//======================================================================

class ScalaSim {
public:
	ScalaSim(const std::string filename,
	         ibeosdk::LogFileManager* const logFileManager,
	         const unsigned short port = 2000);
	~ScalaSim();
public:
	void run();

protected:
	void sendDataBlock(const IbeoDataHeader& dh, const boost::shared_ptr<char> body);
	bool sendData(boost::asio::streambuf& outBuf);

protected:
	unsigned short m_port;
	ibeosdk::LogFileManager* const m_logManager;
	boost::asio::io_service m_io_service;
	boost::asio::ip::udp::socket m_socket;
	boost::asio::ip::udp::endpoint m_destinationEndpoint;
	std::string m_idcFileName;
	IdcFile m_idcFile;
	boost::shared_ptr<IbeoSutpSendInterface> m_sutpSendInterface;
	uint32_t m_nbOfSendDatablocks;
}; // ScalaSim

//======================================================================

ScalaSim::ScalaSim(const std::string filename,
                   ibeosdk::LogFileManager* const logFileManager,
                   const unsigned short port)
  : m_port(port),
    m_logManager(logFileManager),
    m_io_service(),
    m_socket(m_io_service),
    m_destinationEndpoint(),
    m_idcFileName(filename),
    m_idcFile(),
    m_sutpSendInterface(),
    m_nbOfSendDatablocks(0)
{}

//======================================================================

ScalaSim::~ScalaSim()
{
	boost::system::error_code error;
	m_socket.close(error);

	logInfo << m_nbOfSendDatablocks << " send packages" << std::endl;

}

//======================================================================

void ScalaSim::run()
{
	m_sutpSendInterface.reset(new IbeoSutpSendInterface(boost::bind(&ScalaSim::sendData, this, _1)));

	boost::system::error_code error;
	m_socket.open(boost::asio::ip::udp::v4(), error);

	m_socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
//	m_socket.set_option(boost::asio::socket_base::broadcast(true));

	boost::asio::ip::address destination_address = boost::asio::ip::address::from_string("127.0.0.1");
	m_destinationEndpoint = boost::asio::ip::udp::endpoint(destination_address, m_port);


//	m_socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::any(), m_port));
//	m_socket.set_option(boost::asio::ip::multicast::join_group(listen_address));

	m_idcFile.open(m_idcFileName);
	if (!m_idcFile.isOpen()) {
		logError << "Cannot open IDC file '" << m_idcFileName << "'" << std::endl;
		return;
	}

	logInfo << "Start sending" << std::endl;

	IbeoDataHeader dh;
	while (m_idcFile.isGood() && (m_nbOfSendDatablocks < 1000000)) {
		//usleep(40);

		boost::shared_ptr<char> body = m_idcFile.getNextDataBlockRaw(dh);

		if (body) {
			logDebug << "Send data block: " << toHex(dh.getDataType()) << std::endl;
			sendDataBlock(dh, body);
		}
	} // while

	logInfo << "Finished sending" << std::endl;
	logInfo << m_nbOfSendDatablocks << " send datablocks" << std::endl;
	m_sutpSendInterface.reset();
}


//======================================================================

void ScalaSim::sendDataBlock(const IbeoDataHeader& dh, const boost::shared_ptr<char> body)
{
	logInfo << "DataBlock: " << toHex(dh.getDataType()) << "  size: " << dh.getMessageSize() << std::endl;
	if (dh.getDataType() != DataTypeId::DataType_Image) {
		m_sutpSendInterface->send(dh, body.get());
		++m_nbOfSendDatablocks;
	}
}

//======================================================================

bool ScalaSim::sendData(boost::asio::streambuf& outBuf)
{
	logInfo << "Actually sending" << std::endl;
//	// serialize dh
//	boost::array<char, 24> dhs;
//	boost::iostreams::stream<boost::iostreams::array_sink> strm(dhs.begin(), dhs.end());
//	std::ostream& s = (std::ostream&)strm;
//	dh.serialize(s);
//
//	// combine header and body to a buffer sequence
//	std::vector<boost::asio::const_buffer> data;
//	data.push_back(boost::asio::buffer(dhs));
//	data.push_back(boost::asio::buffer(body.get(), dh.getMessageSize()));

	// send data via UDP
	logInfo << "Buffers size bytes: " << outBuf.size() << std::endl;

	const uint32_t len = uint32_t(m_socket.send_to(outBuf.data(), m_destinationEndpoint));
	logInfo << "Send bytes: " << len << std::endl;
	return true;
}

//======================================================================
//======================================================================
//======================================================================

int checkArguments(const int argc, const char** argv, bool& hasLogFile)
{
	const int minNbOfNeededArguments = 2;
	const int maxNbOfNeededArguments = 3;

	bool wrongNbOfArguments = false;
	if (argc < minNbOfNeededArguments) {
		std::cerr << "Missing argument" << std::endl;
		wrongNbOfArguments = true;
	}
	else if (argc > maxNbOfNeededArguments) {
		std::cerr << "Too many argument" << std::endl;
		wrongNbOfArguments = true;
	}

	if (wrongNbOfArguments) {
		std::cerr << argv[0] << " " << " INPUTFILENAME [LOGFILE]" << std::endl;
		std::cerr << "\tINPUTFILENAME Name of the file to use as input." << std::endl;
		std::cerr << "\tLOGFILE name of the log file. If ommitted, the log output will be performed to stderr." << std::endl;
		return 1;
	}

	hasLogFile = (argc == maxNbOfNeededArguments);
	return 0;
}

//======================================================================

int main(const int argc, const char** argv)
{
	std::cerr << argv[0] << " Version " << appVersion.toString();
	std::cerr << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;

	bool hasLogFile;
	const int checkResult = checkArguments(argc, argv, hasLogFile);
	if (checkResult != 0)
		exit(checkResult);
	int currArg = 1;

	std::string filename = argv[currArg++];

	const off_t maxLogFileSize = 1000000;

	LogFileManager logFileManager;
	ibeosdk::LogFile::setTargetFileSize(maxLogFileSize);

	if (hasLogFile) {
		ibeosdk::LogFile::setLogFileBaseName(argv[currArg++]);
	}
	const ibeosdk::LogLevel ll = ibeosdk::logLevelFromString("Debug2");
	ibeosdk::LogFile::setLogLevel(ll);
	logFileManager.start();

	if (hasLogFile) {
		logInfo << argv[0] << " Version " << appVersion.toString()
		        << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;
	}

	ScalaSim sim(filename, &logFileManager, 2000);
	sim.run();

	exit(0);
}

//======================================================================
