//======================================================================
/*! \file IbeoSdkDataLoggerDemo.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 21, 2012
 *
 * Demo project for data logging. This tool is connecting
 * with a LUX sensor and saves the data into an IDC file.
 * This is basically an IbeoSdkWriterDemo with more elaborated
 * disk management.
 *///-------------------------------------------------------------------

#include <ibeosdk/lux.hpp>
#include <ibeosdk/scala.hpp>
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>
#include <ibeosdk/devices/IdcFile.hpp>
#include <ibeosdk/SplitFile.hpp>
#include <ibeosdk/IpHelper.hpp>

#include <boost/program_options.hpp>
#include <boost/asio.hpp>

#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/vfs.h>

//======================================================================

using namespace ibeosdk;

//======================================================================

const ibeosdk::Version::MajorVersion majorVersion(5);
const ibeosdk::Version::MinorVersion minorVersion(2);
const ibeosdk::Version::Revision revision(2);
const ibeosdk::Version::PatchLevel patchLevel;
const ibeosdk::Version::Build build;
const std::string info = "IbeoSdkDataLoggerDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);
IbeoSDK ibeoSDK;

//======================================================================

class IdcWriter : ibeosdk::IdcFile,
                  public ibeosdk::DataStreamer,
                  public ibeosdk::SplitFile {
public:
	IdcWriter(const std::string filename,
	          IbeoEthDevice* device,
	          LogFileManager& logFileManager,
	          const off_t fileSizeLimit,
	          const double memoryConsumptionLimit);

	virtual ~IdcWriter();

private:
	IdcWriter(const IdcWriter&); // forbidden
	IdcWriter& operator=(const IdcWriter&); // forbidden

public:
	virtual bool create(const std::string& filename, const bool append = false);
	virtual void onData(const IbeoDataHeader& dh, const char* const bodyBuf);
	virtual void split(const int fileIndex);

	void checkFilesystemUsage(const std::string& devPath, uint64_t& diskSize, uint64_t& totalFreeBytes);
	bool isFileSystemUsageExceeded() const { return m_isFilesystemUsageExceeded; }

protected:
	std::string getFileName(const int fileIndex) const;
	void splitFileIfNeeded();

protected:
	LogFileManager& m_logFileManager;
	std::string m_filenameTemplate;
	std::string m_currentFileName;
	off_t m_currentFileSize;
	const off_t m_fileSizeLimit;

	const double m_maxFilesystemUsage;
	bool m_isFilesystemUsageExceeded;
	bool m_isNewFile;
}; // IDCWriter

//======================================================================
//======================================================================
//======================================================================



//======================================================================
//======================================================================
//======================================================================

IdcWriter::IdcWriter(const std::string filename,
                     IbeoEthDevice* device,
                     LogFileManager& logFileManager,
                     const off_t fileSizeLimit,
                     const double memoryConsumptionLimit)
  : IdcFile(),
    m_logFileManager(logFileManager),
    m_filenameTemplate(filename),
    m_currentFileName(),
    m_currentFileSize(0),
    m_fileSizeLimit(fileSizeLimit),
    m_maxFilesystemUsage(memoryConsumptionLimit),
    m_isFilesystemUsageExceeded(false),
    m_isNewFile(true)
{
	const std::size_t len = m_filenameTemplate.length();
	if (len > 4) {
		// in case the provided filename ends with the .idc suffix
		// this will be stripped from the filename template.
		if (m_filenameTemplate.substr(len-4)==".idc") {
			m_filenameTemplate = m_filenameTemplate.substr(0, len-4);
		}
	}

	const int idx = this->m_logFileManager.registerSplitFile(this);
	this->create(this->getFileName(idx), false);

	if (device && this->isOpen()) {
		device->registerStreamer(this);
	}
}

//======================================================================

IdcWriter::~IdcWriter()
{
	this->m_logFileManager.unregisterSplitFile(this);
	this->close();
}

//======================================================================

void IdcWriter::checkFilesystemUsage(const std::string& devPath, uint64_t& diskSize, uint64_t& totalFreeBytes)
{
	struct statfs fs;

	if ( ( statfs( devPath.c_str(), &fs ) ) < 0 ) {
		printf( "Failed to stat %s: %s\n", devPath.c_str() ,strerror( errno ) );
		diskSize = 0;
		totalFreeBytes = 0;
	}
	else {
		diskSize = uint64_t(fs.f_blocks) * uint64_t(fs.f_bsize);
		totalFreeBytes = uint64_t(fs.f_bfree) * uint64_t(fs.f_bsize);
	}
}

//======================================================================

std::string split1000000(const unsigned long sz, const char sep)
{
	std::stringstream ss;
	if (sz < 1000000) {
		ss << '0';
	}
	else {
		ss << sz/1000000;
	}
	ss << sep;
	ss << std::setfill('0');
	ss << std::setw(3) << (sz%1000000);
	return ss.str();
}

//======================================================================

bool IdcWriter::create(const std::string& filename, const bool append)
{
	if (filename.empty())
		return false;

	logInfo << "========================================" << std::endl;

	std::string absPath;

	if (filename.at(0)=='/') {
		const size_t position = filename.rfind("/");
		absPath = filename.substr(0,position > 0 ? position : 1);
	}
	else {
		char tempPath[200];
		absPath = getcwd(tempPath,200);
	}

	bool result = false;

	uint64_t diskSize;
	uint64_t totalFreeBytes;
	this->checkFilesystemUsage(absPath, diskSize, totalFreeBytes);

	logInfo << "Main path of the executable file: " << absPath << std::endl;

	logInfo << "Size of filesystem:       " << split1000000(diskSize,'.') << " MByte" << std::endl;
	logInfo << "Free space on filesystem: " << split1000000(totalFreeBytes,'.') << " MByte" << std::endl;

	const double actualMemConsumptionPercent = 100.0 - ((double)totalFreeBytes*100)/(double)diskSize;
	if (m_maxFilesystemUsage < actualMemConsumptionPercent) {
		m_isFilesystemUsageExceeded = true;
		this->close();
		result = false;
	}
	else {
		m_isFilesystemUsageExceeded = false;
		result = IdcFile::create(filename, append);
	}

	LogFile::m_logStream->setf(std::ios_base::fixed, std::ios_base::floatfield);
	LogFile::m_logStream->precision(3);
	logInfo << "Current harddrive usage: " << actualMemConsumptionPercent << "%" << std::endl;
	logInfo << "Max harddrive usage:     " << m_maxFilesystemUsage << "%" << std::endl;
	LogFile::m_logStream->precision(-1);
	LogFile::m_logStream->unsetf(std::ios_base::floatfield);

	if (m_isFilesystemUsageExceeded)
		logInfo << "==> Harddrive usage exceeds the limit." << std::endl;
	else
		logInfo << "==> Enough space on harddrive." << std::endl;

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
	default:
		break;
	}

	//dh.serialize(this->m_strm);
	const uint32_t sz = dh.getMessageSize();
	this->m_currentFileSize += sz;
	if (m_isNewFile) {
		IbeoDataHeader ndh(dh);
		ndh.setPreviousMessageSize(0);
		write(ndh, bodyBuf);
		m_isNewFile = false;
	}
	else {
		write(dh, bodyBuf);
	}
//	this->write(bodyBuf, sz);
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

class CommandLineArguments {
public:
	CommandLineArguments(int argc, const char** argv)
	  : m_useEcu(false),
	    m_hasLogFile(false),
	    m_valid(false),
	    m_ip(),
	    m_ipAsStr(),
	    m_port(0),
	    m_outputFile(),
	    m_maxFileSize(100000000), // 100 mByte
	    m_maxHarddriveUsage(80.0),
	    m_logFilename()
	{
		using namespace boost::program_options;
		variables_map cmdVars;

		options_description desc("IbeoSdkDataLoggerDemo\n"
			"\tPort = 12002 for Lux or ECU (see parameter --ecu)\n"
			"\tPort = 12004 for Scala\n"
			"\tPort = 12006 for MiniLux");
		desc.add_options()
			("ip,I", value<std::string>(&m_ipAsStr)->required(), "Ip address of the sensor/ecu.")
			("port,P", value<uint16_t>(&m_port)->default_value(12002), "Port of the device.")
			("ecu,e", "Device is an ECU not a LUX sensor. (Only if port=12002)")
			("output,O", value<std::string>(&m_outputFile)->required(), "Name of the output IDC file")
			("maxfilesize,s", value<off_t>(&m_maxFileSize)->default_value(100000000),
					"Exceeding this size will trigger splitting the output file.")
			("maxharddriveusage,u", value<double>(&m_maxHarddriveUsage)->default_value(80.0),
					"If more space than this given percentage is occupied on the destination file system "
					"opening a new file will fail. The program will terminate."
					"This criteria will only be checked on program start and whenever the output file "
					"is split.")
			("logfile,l", value<std::string>(&m_logFilename),
					"Name of the log file. If omitted, std::cerr will be used.")
			("help,h", "Show this message")
			;

		positional_options_description pos_opts;
		pos_opts.add("ip",1);
		pos_opts.add("output",1);
		pos_opts.add("logfile",1);

		try {
			store(command_line_parser(argc, argv).options(desc).positional(pos_opts).run(), cmdVars);
		}
		catch (const boost::program_options::multiple_occurrences& e) {
			std::cerr << "*** Command line option has given more than once or too many parameters provided." << std::endl;
			desc.print(std::cerr);
			exit(1);
		}
		catch (const boost::program_options::error& e) {
			std::cerr << "*** Error in command line arguments: " << e.what() << std::endl;
			desc.print(std::cerr);
			exit(1);
		}

		try {
			notify(cmdVars);
		}
		catch (const boost::program_options::error& e) {
			std::cerr << "*** Error in command line arguments: " << e.what() << std::endl;
			desc.print(std::cerr);
			exit(1);
		}

		if (cmdVars.count("help")) {
			desc.print(std::cerr);
			exit(0);
		}

		m_useEcu = cmdVars.count("ecu");
		if (m_useEcu && m_port!=12002) {
			std::cerr << "Option --ecu can only be used if port is set to 12002." << std::endl;
			exit(1);
		}


		m_hasLogFile = cmdVars.count("logfile");

		boost::system::error_code ec;
		m_ip = boost::asio::ip::address::from_string(m_ipAsStr, ec);
		if (ec) {
			std::cerr << "IP address '" << m_ipAsStr << "' is not valid: " << ec.message() << std::endl;
		}
		else {
			m_valid = true;
		}
	}

public:
	bool useEcu() const { return m_useEcu; }
	bool hasLogFile() const { return m_hasLogFile; }
	bool isValid() const { return m_valid; }
	boost::asio::ip::address getIp() const { return m_ip; }
	std::string getIpAsString() const { return m_ipAsStr; }
	uint16_t getPort() const { return m_port; }
	const std::string& getOutputFile() const { return m_outputFile; }
	off_t getMaxFileSize() const { return m_maxFileSize; }
	double getMaxHarddriveUsage() const { return m_maxHarddriveUsage; }
	const std::string& getLogFileName() const { return m_logFilename; }

protected:
	bool m_useEcu;
	bool m_hasLogFile;
	bool m_valid;
	boost::asio::ip::address m_ip;
	std::string m_ipAsStr;
	uint16_t m_port;
	std::string m_outputFile;
	off_t m_maxFileSize;
	double m_maxHarddriveUsage;
	std::string m_logFilename;
}; // CommandLineArguments

//======================================================================

void idcwriter_demo(LogFileManager& logFileManager, const CommandLineArguments& cmdargs);

//======================================================================

int main(const int argc, const char** argv)
{
	std::cerr << argv[0] << " Version " << appVersion.toString();
	std::cerr << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;

	CommandLineArguments cmdargs(argc, argv);
	if (!cmdargs.isValid())
		exit(1);

	LogFileManager logFileManager;
	if (cmdargs.hasLogFile()) {
		ibeosdk::LogFile::setLogFileBaseName(cmdargs.getLogFileName());
	}
	ibeosdk::LogFile::setTargetFileSize(cmdargs.getMaxFileSize());

	const ibeosdk::LogLevel ll = ibeosdk::logLevelFromString("Debug");
	ibeosdk::LogFile::setLogLevel(ll);

	logFileManager.start();

	if (cmdargs.hasLogFile()) {
		logInfo << argv[0] << " Version " << appVersion.toString()
		        << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;
	}

	idcwriter_demo(logFileManager, cmdargs);

	exit(0);
}

//======================================================================

void idcwriter_demo(LogFileManager& logFileManager, const CommandLineArguments& cmdargs)
{
	IbeoEthDevice* device = NULL;

	switch(cmdargs.getPort()) {
	case 12002:
		if (cmdargs.useEcu())
			device = new IbeoEcu(cmdargs.getIpAsString());
		else
			device = new IbeoLux(cmdargs.getIpAsString());
		break;
	case 12004:
		device = new IbeoScala(cmdargs.getIpAsString());
		break;
	case 12006:
		device = new IbeoMiniLux(cmdargs.getIpAsString());
		break;
	default:
		logError << "Port " << cmdargs.getPort() << " is not supported by any known device." << std::endl;
		exit(1);
	}

	device->setLogFileManager(&logFileManager);
	IdcWriter idcWriter(cmdargs.getOutputFile(),
	                    &*device,
	                    logFileManager,
	                    cmdargs.getMaxFileSize(),
	                    cmdargs.getMaxHarddriveUsage());
	device->getConnected();

	while (true) {
		if (!device->isConnected() || idcWriter.isFileSystemUsageExceeded())
			return;
#		ifdef _WIN32
			::Sleep(1);
#		else // _WIN32
			sleep(1);
#		endif // _WIN32
	}
}

//======================================================================

