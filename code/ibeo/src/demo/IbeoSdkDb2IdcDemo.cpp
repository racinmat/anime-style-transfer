//======================================================================
/*! \file IbeoSdkDb2IdcDemo.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 01, 2016
 *
 * Demo project reading data from a Mongo database and writing
 * the received data blocks to an IDC file.
 *
 * Example parameters for info mode
 * -I "127.0.0.1" -U "" -C "" -S "" -T "" -P ""
 *
 * Example parameters for writing an IDC file
 * -I "127.0.0.1" -U "" -C "" -S "SessionName" -T "Tripname" -P "ProcessingName" -O FileName
 *
 *///-------------------------------------------------------------------

#include <ibeosdk/lux.hpp>
#include <ibeosdk/scala.hpp>
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>

#include <ibeosdk/database/initMongoClient.hpp>
#include <ibeosdk/devices/DatabaseReplayer.hpp>
#include <ibeosdk/devices/database/ObjectListDbReader.hpp>
#include <ibeosdk/devices/database/ImageDbReader.hpp>
#include <ibeosdk/devices/database/MeasurementList2821DbReader.hpp>
#include <ibeosdk/devices/database/ScanDbReader.hpp>
#include <ibeosdk/devices/database/VehicleStateDbReader.hpp>
#include <ibeosdk/devices/database/CanMessageDbReader.hpp>

#include <ibeosdk/devices/IdcFile.hpp>
#include <ibeosdk/IpHelper.hpp>

#include <boost/program_options.hpp>

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
const std::string info = "IbeoSdkDb2IdcDemo";


ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);
IbeoSDK ibeoSDK;

//======================================================================

class IdcWriter : ibeosdk::IdcFile,
                  public ibeosdk::DataListener<ScanEcu>,
                  public ibeosdk::DataListener<ObjectListEcuEtDyn>,
                  public ibeosdk::DataListener<Image>,
                  public ibeosdk::DataListener<MeasurementList2821>,
                  public ibeosdk::DataListener<VehicleStateBasicEcu>,
                  public ibeosdk::DataListener<VehicleStateBasicEcu2808>,
                  public ibeosdk::DataListener<CanMessage>,
                  public ibeosdk::SplitFile {
public:
	IdcWriter(const std::string filename,
	          DatabaseReplayer* dbReader,
	          LogFileManager& logFileManager,
	          const off_t fileSizeLimit);

	virtual ~IdcWriter();

private:
	IdcWriter(const IdcWriter&); // forbidden
	IdcWriter& operator=(const IdcWriter&); // forbidden

public:
	virtual bool create(const std::string& filename, const bool append = false);

	virtual void onData(const ScanEcu* const scan);
	virtual void onData(const ObjectListEcuEtDyn* const objList);
	virtual void onData(const Image* const image);
	virtual void onData(const VehicleStateBasicEcu2808* const vsb);
	virtual void onData(const VehicleStateBasicEcu* const vsb);
	virtual void onData(const MeasurementList2821* const ml);
	virtual void onData(const CanMessage* const cm);

	virtual void split(const int fileIndex);

protected:
	std::string getFileName(const int fileIndex) const;
	void splitFileIfNeeded();
	void writeDataBlock(const DataBlock* const db);
	void writeDataBlock(const DataBlock* const db, const uint8_t deviceId, const NTPTime ntpTime);

protected:
	LogFileManager& m_logFileManager;
	std::string m_filenameTemplate;
	std::string m_currentFileName;
	off_t m_currentFileSize;
	const off_t m_fileSizeLimit;
	uint8_t m_deviceId;

	ScanEcu m_lastScan;
	bool m_scanSeen;
	bool m_isNewFile;
}; // IDCWriter

//======================================================================
//======================================================================
//======================================================================

IdcWriter::IdcWriter(const std::string filename,
                     DatabaseReplayer* dbReader,
                     LogFileManager& logFileManager,
                     const off_t fileSizeLimit)
  : IdcFile(),
    m_logFileManager(logFileManager),
    m_filenameTemplate(filename),
    m_currentFileName(),
    m_currentFileSize(0),
    m_fileSizeLimit(fileSizeLimit),
    m_deviceId(0),
	m_scanSeen(false),
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
}

//======================================================================

IdcWriter::~IdcWriter()
{
	FrameEndSeparator end;
	writeDataBlock(&end, 0, m_lastScan.getStartTimestamp());
	this->m_logFileManager.unregisterSplitFile(this);

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

void IdcWriter::writeDataBlock(const DataBlock* const db, const uint8_t deviceId, const NTPTime ntpTime)
{
	const uint32_t sizeOfThisMessage = uint32_t(db->getSerializedSize());
	const IbeoDataHeader dh(db->getDataType(),
	                        0,
	                        sizeOfThisMessage,
	                        deviceId,
	                        ntpTime);

	logInfo << "=================" << std::endl;
	logInfo << "Type " << db->getDataType() << " Size " << sizeOfThisMessage
			<< " HeaderDeviceId " << db->getHeaderDeviceId()
			<< " HeaderTime " << db->getHeaderNtpTime() << std::endl;

	this->write(dh, db);

	this->m_currentFileSize += IbeoDataHeader::getHeaderSize() + sizeOfThisMessage;
}

//======================================================================

void IdcWriter::onData(const ScanEcu* const scan)
{
	logDebug << "Received message 0x" << std::hex << scan->getDataType() << std::dec << std::endl;
	this->splitFileIfNeeded();

	if (m_scanSeen) {
		FrameEndSeparator end;
		writeDataBlock(&end, 0, scan->getStartTimestamp());
	}

	writeDataBlock(scan, scan->getHeaderDeviceId(), scan->getStartTimestamp());
	m_lastScan = *scan;
	m_scanSeen = true;
}

//======================================================================

void IdcWriter::onData(const ObjectListEcuEtDyn* const objList)
{
	logInfo << "OBJECTLIST received" << std::endl;
	if (m_scanSeen)
		writeDataBlock(objList, objList->getHeaderDeviceId(), objList->getHeaderNtpTime());
}

//======================================================================

void IdcWriter::onData(const Image* const image)
{
	if (m_scanSeen)
		writeDataBlock(image, image->getDeviceId(), image->getTimestamp());
}

//======================================================================

void IdcWriter::onData(const VehicleStateBasicEcu* const vsb)
{
	if (m_scanSeen)
		writeDataBlock(vsb, 0, vsb->getTimestamp());
}

//======================================================================

void IdcWriter::onData(const VehicleStateBasicEcu2808* const vsb)
{
	if (m_scanSeen)
		writeDataBlock(vsb, 0, vsb->getTimestamp());
}

//======================================================================

void IdcWriter::onData(const MeasurementList2821* const ml)
{
	if (m_scanSeen && !ml->getTimestamp().is_not_a_date_time())
		writeDataBlock(ml, 0, ml->getTimestamp());
}

//======================================================================

void IdcWriter::onData(const CanMessage* const cm)
{
	if (m_scanSeen)
		writeDataBlock(cm, 0, cm->getTimestamp());
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
	this->close();
	this->create(this->getFileName(fileIndex), false);
	m_currentFileSize = 0;
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

class CommandLineArguments {
public:
	CommandLineArguments(int argc, const char** argv)
	  : m_hasLogFile(false),
	    m_logFilename(),
	    m_valid(false),
	    m_databaseIp(),
	    m_ipAsStr(),
	    m_userName(),
	    m_password(),
	    m_sessionName(),
	    m_tripName(),
	    m_processing(),
	    m_outputFile(),
	    m_maxFileSize(100000000)
	{
		using namespace boost::program_options;
		variables_map cmdVars;

		options_description desc("IbeoSdkDb2IdcDemo\n");
		desc.add_options()
			("ip,I", value<std::string>(&m_ipAsStr)->default_value("127.0.0.1"), "Ip address of the database.")
			("username,U", value<std::string>(&m_userName)->default_value(""), "User name for the database.")
			("password,C", value<std::string>(&m_password)->default_value(""), "Password for the database.")
			("sessionname,S", value<std::string>(&m_sessionName)->required(), "Name of the session in the database.")
			("tripname,T", value<std::string>(&m_tripName)->required(), "Name of the trip in the database.")
			("processing,P", value<std::string>(&m_processing)->required(), "Name of processing in the database.")
			("output,O", value<std::string>(&m_outputFile)->required(), "Name of the output IDC file")
			("maxfilesize,s", value<off_t>(&m_maxFileSize)->default_value(100000000),
					"Exceeding this size will trigger splitting the output file.")
			("logfile,l", value<std::string>(&m_logFilename),
					"Name of the log file. If omitted, std::cerr will be used.")
			("help,h", "Show this message")
			;

		positional_options_description pos_opts;
		pos_opts.add("ip",1);
		pos_opts.add("username",1);
		pos_opts.add("password",1);
		pos_opts.add("tripname",1);
		pos_opts.add("processing",1);
		pos_opts.add("output",1);
		pos_opts.add("logfile",1);

		try {
			store(command_line_parser(argc, argv).options(desc).positional(pos_opts).run(), cmdVars);
		}
		catch (const boost::program_options::multiple_occurrences&) {
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

		m_hasLogFile = (cmdVars.count("logfile") > 0);

		boost::system::error_code ec;
		m_databaseIp = boost::asio::ip::address::from_string(m_ipAsStr, ec);
		if (ec) {
			std::cerr << "IP address '" << m_ipAsStr << "' is not valid: " << ec.message() << std::endl;
		}
		else {
			m_valid = true;
		}
	}

public:
	bool isValid() const { return m_valid; }

	bool hasLogFile() const { return m_hasLogFile; }
	const std::string& getLogFileName() const { return m_logFilename; }

	boost::asio::ip::address getDatabaseIp() const { return m_databaseIp; }
	std::string getIpAsString() const { return m_ipAsStr; }

	std::string getUserName() const { return m_userName; }
	std::string getPassword() const { return m_password; }
	std::string getSessionName() const { return m_sessionName; }
	std::string getTripName() const { return m_tripName; }
	std::string getProcessing() const { return m_processing; }

	const std::string& getOutputFile() const { return m_outputFile; }
	off_t getMaxFileSize() const { return m_maxFileSize; }

protected:
	bool m_hasLogFile;
	std::string m_logFilename;
	bool m_valid;

	boost::asio::ip::address m_databaseIp;
	std::string m_ipAsStr;

	std::string m_userName;
	std::string m_password;
	std::string m_sessionName;
	std::string m_tripName;
	std::string m_processing;

	std::string m_outputFile;
	off_t m_maxFileSize;
}; // CommandLineArguments

//======================================================================
//======================================================================
//======================================================================

void idcWriterDb_demo(LogFileManager& logFileManager, const CommandLineArguments& cmdargs);

//======================================================================
//======================================================================
//======================================================================

int main(const int argc, const char** argv)
{
	std::cerr << argv[0] << " Version " << appVersion.toString();
	std::cerr << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;

	CommandLineArguments cmdargs(argc, argv);
	if (!cmdargs.isValid())
		exit(1);

	LogFileManager logFileManager;
	ibeosdk::LogFile::setTargetFileSize(cmdargs.getMaxFileSize());

	if (cmdargs.hasLogFile()) {
		ibeosdk::LogFile::setLogFileBaseName(cmdargs.getLogFileName());
	}
	ibeosdk::LogFile::setTargetFileSize(cmdargs.getMaxFileSize());
	const ibeosdk::LogLevel ll = ibeosdk::logLevelFromString("Info");
	ibeosdk::LogFile::setLogLevel(ll);

	logFileManager.start();

	if (cmdargs.hasLogFile()) {
		logInfo << argv[0] << " Version " << appVersion.toString()
		        << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;
	}

	idcWriterDb_demo(logFileManager, cmdargs);

	exit(0);
}

//======================================================================

void idcWriterDb_demo(LogFileManager& logFileManager, const CommandLineArguments& cmdargs)
{
	dbaccess::initMongoClient();

	dbaccess::DbConnection::ConnectionProperties prop(cmdargs.getUserName(), cmdargs.getPassword(),cmdargs.getIpAsString(), 27017);
	DatabaseReplayer dbReader(prop, cmdargs.getSessionName(), cmdargs.getTripName(), cmdargs.getProcessing());

	ScanDbReader scanReader;
	VehicleStateDbReader vhsReader;
	vhsReader.useProcessedData();

	ImageDbReader imgReader;
	CanMessageDbReader canMsgReader;
	MeasurementList2821DbReader measListReader;
	ObjectListDbReader objListReader;
	objListReader.useProcessedData();

	dbReader.registerDbDataBlockReader(scanReader);
	dbReader.registerDbDataBlockReader(vhsReader);
	dbReader.registerDbDataBlockReader(imgReader);
	dbReader.registerDbDataBlockReader(canMsgReader);
	dbReader.registerDbDataBlockReader(measListReader);
	dbReader.registerDbDataBlockReader(objListReader);

	IdcWriter idcWriter(cmdargs.getOutputFile(), &dbReader, logFileManager, 10000000);

	dbReader.registerListener(&idcWriter);
	dbReader.getConnected();

	while (true) {
		if (!dbReader.isConnected())
			return;
#		ifdef _WIN32
			::Sleep(1);
#		else // _WIN32
			sleep(1);
#		endif // _WIN32
	}

	dbaccess::shutdownMongoClient();
}

//======================================================================
