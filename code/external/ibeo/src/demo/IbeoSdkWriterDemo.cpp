//======================================================================
/*! \file IbeoSdkWriterDemo.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 21, 2012
 *
 * Demo project connecting to a LUX or Scala and writing the received data
 * blocks to an IDC file.
 *///-------------------------------------------------------------------

#include <ibeosdk/lux.hpp>
#include <ibeosdk/luxhr.hpp>
#include <ibeosdk/scala.hpp>
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>
#include <ibeosdk/devices/IdcFile.hpp>
#include <ibeosdk/IpHelper.hpp>

#include <sstream>
#include <iomanip>

//======================================================================

using namespace ibeosdk;

//======================================================================

const ibeosdk::Version::MajorVersion majorVersion(5);
const ibeosdk::Version::MinorVersion minorVersion(2);
const ibeosdk::Version::Revision revision(2);
const ibeosdk::Version::PatchLevel patchLevel;
const ibeosdk::Version::Build build;
const std::string info = "IbeoSdkWriterDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);
IbeoSDK ibeoSDK;

//======================================================================

class IdcWriter : ibeosdk::IdcFile,
                  public ibeosdk::DataStreamer,
                  public ibeosdk::SplitFile {
public:
	IdcWriter(const std::string filename,
	          IbeoLux* lux,
	          LogFileManager& logFileManager,
	          const off_t fileSizeLimit);

	IdcWriter(const std::string filename,
	          IbeoEcu* ecu,
	          LogFileManager& logFileManager,
	          const off_t fileSizeLimit);

	IdcWriter(const std::string filename,
	          IbeoScala* scala,
	          LogFileManager& logFileManager,
	          const off_t fileSizeLimit);

	IdcWriter(const std::string filename,
	          IbeoMiniLux* miniLux,
	          LogFileManager& logFileManager,
	          const off_t fileSizeLimit);

	IdcWriter(const std::string filename,
	          IbeoLuxHr* luxHr,
	          LogFileManager& logFileManager,
	          const off_t fileSizeLimit);

	virtual ~IdcWriter();

private:
	IdcWriter(const IdcWriter&); // forbidden
	IdcWriter& operator=(const IdcWriter&); // forbidden

public:
	virtual bool create(const std::string& filename, const bool append = false);

	virtual void onData(const IbeoDataHeader& dh, const char* const bodyBuf);
	virtual void split(const int fileIndex);

protected:
	std::string getFileName(const int fileIndex) const;
	void splitFileIfNeeded();

protected:
	LogFileManager& m_logFileManager;
	std::string m_filenameTemplate;
	std::string m_currentFileName;
	off_t m_currentFileSize;
	const off_t m_fileSizeLimit;
	uint32_t m_sizeOfPreviousMessage;
	bool m_isNewFile;
}; // IDCWriter

//======================================================================
//======================================================================
//======================================================================



//======================================================================
//======================================================================
//======================================================================

IdcWriter::IdcWriter(const std::string filename,
                     IbeoLux* lux,
                     LogFileManager& logFileManager,
                     const off_t fileSizeLimit)
  : IdcFile(),
    m_logFileManager(logFileManager),
    m_filenameTemplate(filename),
    m_currentFileName(),
    m_currentFileSize(0),
    m_fileSizeLimit(fileSizeLimit),
    m_sizeOfPreviousMessage(0),
    m_isNewFile(true)
{
	const std::size_t len = m_filenameTemplate.length();
	if (len > 4) {
		if (m_filenameTemplate.substr(len-4)==".idc") {
			m_filenameTemplate = m_filenameTemplate.substr(0, len-4);
		}
	}

	const int idx = this->m_logFileManager.registerSplitFile(this);
	this->create(this->getFileName(idx), false);

	if (lux && this->isOpen()) {
		lux->registerStreamer(this);
	}
}

//======================================================================
//======================================================================
//======================================================================

IdcWriter::IdcWriter(const std::string filename,
                     IbeoEcu* ecu,
                     LogFileManager& logFileManager,
                     const off_t fileSizeLimit)
  : IdcFile(),
    m_logFileManager(logFileManager),
    m_filenameTemplate(filename),
    m_currentFileName(),
    m_currentFileSize(0),
    m_fileSizeLimit(fileSizeLimit),
    m_sizeOfPreviousMessage(0),
    m_isNewFile(true)
{
	const std::size_t len = m_filenameTemplate.length();
	if (len > 4) {
		if (m_filenameTemplate.substr(len-4)==".idc") {
			m_filenameTemplate = m_filenameTemplate.substr(0, len-4);
		}
	}

	const int idx = this->m_logFileManager.registerSplitFile(this);
	this->create(this->getFileName(idx), false);

	if (ecu && this->isOpen()) {
		ecu->registerStreamer(this);
	}
}

//======================================================================
//======================================================================
//======================================================================

IdcWriter::IdcWriter(const std::string filename,
                     IbeoScala* scala,
                     LogFileManager& logFileManager,
                     const off_t fileSizeLimit)
  : IdcFile(),
    m_logFileManager(logFileManager),
    m_filenameTemplate(filename),
    m_currentFileName(),
    m_currentFileSize(0),
    m_fileSizeLimit(fileSizeLimit),
    m_sizeOfPreviousMessage(0),
    m_isNewFile(true)
{
	const std::size_t len = m_filenameTemplate.length();
	if (len > 4) {
		if (m_filenameTemplate.substr(len-4)==".idc") {
			m_filenameTemplate = m_filenameTemplate.substr(0, len-4);
		}
	}

	const int idx = this->m_logFileManager.registerSplitFile(this);
	this->create(this->getFileName(idx), false);

	if (scala && this->isOpen()) {
		scala->registerStreamer(this);
	}
}

//======================================================================

IdcWriter::IdcWriter(const std::string filename,
                     IbeoMiniLux* miniLux,
                     LogFileManager& logFileManager,
                     const off_t fileSizeLimit)
  : IdcFile(),
    m_logFileManager(logFileManager),
    m_filenameTemplate(filename),
    m_currentFileName(),
    m_currentFileSize(0),
    m_fileSizeLimit(fileSizeLimit),
    m_sizeOfPreviousMessage(0),
    m_isNewFile(true)
{
	const std::size_t len = m_filenameTemplate.length();
	if (len > 4) {
		if (m_filenameTemplate.substr(len-4)==".idc") {
			m_filenameTemplate = m_filenameTemplate.substr(0, len-4);
		}
	}

	const int idx = this->m_logFileManager.registerSplitFile(this);
	this->create(this->getFileName(idx), false);

	if (miniLux && this->isOpen()) {
		miniLux->registerStreamer(this);
	}
}

//======================================================================
//======================================================================
//======================================================================

IdcWriter::IdcWriter(const std::string filename,
                     IbeoLuxHr* luxHr,
                     LogFileManager& logFileManager,
                     const off_t fileSizeLimit)
  : IdcFile(),
    m_logFileManager(logFileManager),
    m_filenameTemplate(filename),
    m_currentFileName(),
    m_currentFileSize(0),
    m_fileSizeLimit(fileSizeLimit),
    m_sizeOfPreviousMessage(0),
    m_isNewFile(true)
{
	const std::size_t len = m_filenameTemplate.length();
	if (len > 4) {
		if (m_filenameTemplate.substr(len-4)==".idc") {
			m_filenameTemplate = m_filenameTemplate.substr(0, len-4);
		}
	}

	const int idx = this->m_logFileManager.registerSplitFile(this);
	this->create(this->getFileName(idx), false);

	if (luxHr && this->isOpen()) {
		luxHr->registerStreamer(this);
	}
}

//======================================================================

IdcWriter::~IdcWriter()
{
	this->m_logFileManager.unregisterSplitFile(this);

	// Calling this close method, the trailer will be written
	// to finalize the IDC file.
	this->close();
}

//======================================================================

bool IdcWriter::create(const std::string& filename, const bool append)
{
	const bool result = IdcFile::create(filename, append);

	if (result) {
		logInfo << "Successfully open IDC file '" << filename << "'." << std::endl;
	}
	else {
		logError << "Failed to open IDC file '" << filename << "'." << std::endl;
	}
	return result;
}

//======================================================================

void IdcWriter::onData(const IbeoDataHeader& dh, const char* const bodyBuf)
{
	logDebug << "Received message 0x" << std::hex << dh.getDataType() << std::dec << std::endl;

	switch (dh.getDataType()) {
	case DataTypeId::DataType_LuxScan:
	case DataTypeId::DataType_EcuScan:
	case DataTypeId::DataType_Scan2208:
		this->splitFileIfNeeded();
		break;
	case DataTypeId::DataType_IdcTrailer:
	case DataTypeId::DataType_FrameIndex:
		logDebug << "Skip writing datatype " << toHex(dh.getDataType()) << std::endl;
		return;
		break;
	default:
		break;
	}

	//dh.serialize(this->m_strm);
	const uint32_t sz = dh.getMessageSize();
	this->m_currentFileSize += sz;

	IbeoDataHeader ndh(dh);
	ndh.setPreviousMessageSize(m_isNewFile ? 0 : m_sizeOfPreviousMessage);

	if (m_isNewFile) {
		m_isNewFile = false;
	}
	write(ndh, bodyBuf);
	m_sizeOfPreviousMessage = ndh.getMessageSize();


	logDebug << "Total file size: " << this->m_currentFileSize << std::endl;
}

//======================================================================

std::string IdcWriter::getFileName(const int fileIndex) const
{
	std::stringstream ss;
	ss << m_filenameTemplate;
	ss << "_" << std::setfill('0') << std::setw(4) << fileIndex;
	ss << ".idc";
	return ss.str();
}

//======================================================================

void IdcWriter::split(const int fileIndex)
{
	//this->m_strm.flush();
	this->close();
	this->create(this->getFileName(fileIndex), false);
	m_currentFileSize = 0;
	m_isNewFile = true;
}

//======================================================================

void IdcWriter::splitFileIfNeeded()
{
	if (m_currentFileSize >= m_fileSizeLimit) {
		this->m_logFileManager.splitRequest();
	}
}

//======================================================================
//======================================================================
//======================================================================











//======================================================================
//======================================================================
//======================================================================

void idcWriterLux_demo(LogFileManager& logFileManager,
                       const std::string& ip,
                       const std::string& outFilename);

//======================================================================

void idcWriterEcu_demo(LogFileManager& logFileManager,
                       const std::string& ip,
                       const std::string& outFilename);

//======================================================================

void idcWriterScala_demo(LogFileManager& logFileManager,
                         const std::string& ip,
                         const std::string& outFilename);

//======================================================================

void idcWriterMiniLux_demo(LogFileManager& logFileManager,
                           const std::string& ip,
                           const std::string& outFilename);

//======================================================================

void idcWriterLuxHr_demo(LogFileManager& logFileManager,
                         const std::string& ip,
                         const std::string& outFilename);

//======================================================================
//======================================================================
//======================================================================

int checkArguments(const int argc, const char** argv, bool& useEcu, bool& hasLogFile)
{
	useEcu = false;
	if (argc>1 && argv[1]==std::string("-e"))
		useEcu = true;
	const int minNbOfNeededArguments = 3 + (useEcu ? 1 : 0);
	const int maxNbOfNeededArguments = 4 + (useEcu ? 1 : 0);

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
		std::cerr << argv[0] << " " << "[-e] IP OUTPUTFILENAME [LOGFILE]" << std::endl;
		std::cerr << "\t-e:\tIf given and the given port is 12002: Sensor is an ECU instead of a LUX."<< std::endl;
		std::cerr << "\tIP is the ip address of the sensor and its port, e.g. 192.168.0.1:12004." << std::endl
				<< "\t\tIf the port is omitted, port 12002 will be used." << std::endl
				<< "\t\tPort 12002: Lux or Ecu" << std::endl
				<< "\t\tPort 12004: Scala" << std::endl
				<< "\t\tPort 12006: MiniLux" << std::endl
				<< "\t\tPort 12008: LuxHR" << std::endl;
		std::cerr << "\tOUTPUTFILENAME name of the output file." << std::endl;
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

	bool useEcu;
	bool hasLogFile;
	const int checkResult = checkArguments(argc, argv, useEcu, hasLogFile);
	if (checkResult != 0)
		exit(checkResult);

	int currArg = 1 + (useEcu ? 1 : 0);

	std::string ip = argv[currArg++];
	const uint16_t port = getPort(ip, 12002);
	const std::string outFilename = argv[currArg++];

	const off_t maxLogFileSize = 1000000000; // VS2008/VS2010 allows only 32bit values here

	LogFileManager logFileManager;
	ibeosdk::LogFile::setTargetFileSize(maxLogFileSize);

	if (hasLogFile) {
		ibeosdk::LogFile::setLogFileBaseName(argv[currArg++]);
	}
	const ibeosdk::LogLevel ll = ibeosdk::logLevelFromString("Debug");
	ibeosdk::LogFile::setLogLevel(ll);

	logFileManager.start();

	if (hasLogFile) {
		logInfo << argv[0] << " Version " << appVersion.toString()
		        << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;
	}

	switch (port) {
	case 12002:
		if (useEcu)
			idcWriterEcu_demo(logFileManager, ip, outFilename);
		else
			idcWriterLux_demo(logFileManager, ip, outFilename);
		break;
	case 12004:
		idcWriterScala_demo(logFileManager, ip, outFilename);
		break;
	case 12006:
		idcWriterMiniLux_demo(logFileManager, ip, outFilename);
		break;
	case 12008:
		idcWriterLuxHr_demo(logFileManager, ip, outFilename);
		break;
	default:
		logError << "Port " << port << " is not supported by any known device." << std::endl;
		break;
	}

	exit(0);
}

//======================================================================

void idcWriterLux_demo(LogFileManager& logFileManager,
                       const std::string& ip,
                       const std::string& outFilename)
{
	IbeoLux lux(ip);
	lux.setLogFileManager(&logFileManager);
	IdcWriter idcWriter(outFilename, &lux, logFileManager, 10000000);
	lux.getConnected();

	while (true) {
		if (!lux.isConnected())
			return;
#		ifdef _WIN32
			::Sleep(1);
#		else // _WIN32
			sleep(1);
#		endif // _WIN32
	}
}

//======================================================================

void idcWriterEcu_demo(LogFileManager& logFileManager,
                       const std::string& ip,
                       const std::string& outFilename)
{
	IbeoEcu ecu(ip);
	ecu.setLogFileManager(&logFileManager);
	IdcWriter idcWriter(outFilename, &ecu, logFileManager, 1000000000);
	ecu.getConnected();

	while (true) {
		if (!ecu.isConnected())
			return;
#		ifdef _WIN32
			::Sleep(1);
#		else // _WIN32
			sleep(1);
#		endif // _WIN32
	}
}

//======================================================================

void idcWriterScala_demo(LogFileManager& logFileManager,
                         const std::string& ip,
                         const std::string& outFilename)
{
	IbeoScala scala(ip);
	scala.setLogFileManager(&logFileManager);
	IdcWriter idcWriter(outFilename, &scala, logFileManager, 10000000);
	scala.getConnected();

	while (true) {
		if (!scala.isConnected())
			return;
#		ifdef _WIN32
			::Sleep(1);
#		else // _WIN32
			sleep(1);
#		endif // _WIN32
	}
}

//======================================================================

void idcWriterMiniLux_demo(LogFileManager& logFileManager,
                           const std::string& ip,
                           const std::string& outFilename)
{
	IbeoMiniLux miniLux(ip);
	miniLux.setLogFileManager(&logFileManager);
	IdcWriter idcWriter(outFilename, &miniLux, logFileManager, 10000000);
	miniLux.getConnected();

	while (true) {
		if (!miniLux.isConnected())
			return;
#		ifdef _WIN32
			::Sleep(1);
#		else // _WIN32
			sleep(1);
#		endif // _WIN32
	}
}

//======================================================================

void idcWriterLuxHr_demo(LogFileManager& logFileManager,
                         const std::string& ip,
                         const std::string& outFilename)
{
	IbeoLuxHr luxHr(ip);
	luxHr.setLogFileManager(&logFileManager);
	IdcWriter idcWriter(outFilename, &luxHr, logFileManager, 10000000);
	luxHr.getConnected();

	while (true) {
		if (!luxHr.isConnected())
			return;
#		ifdef _WIN32
			::Sleep(1);
#		else // _WIN32
			sleep(1);
#		endif // _WIN32
	}
}

//======================================================================
