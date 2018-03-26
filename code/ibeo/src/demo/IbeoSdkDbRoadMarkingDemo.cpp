//======================================================================
/*! \file IbeoSdkDbRoadMarkingDemo.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 01, 2016
 *
 * Demo project for reading and writing RoadMarkings from database to an idc file.
 *
 *
 * Example parameter
 * -I "127.0.0.1" -U "" -C "" -T "TripName" -O FileName
 *
 *
 * Following collections must be present in the database ('id' stands for a processing identifier):
 *       Trip;'TripName'.Scans
 *       Trip;'TripName'.Proc;'id'.VehicleStates
 *       Trip;'TripName'.Proc;'id'.LanePoints
 *
 *
 *///-------------------------------------------------------------------

#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>

#include <ibeosdk/devices/database/TripDbReader.hpp>
#include <ibeosdk/devices/database/ScanDbReader.hpp>
#include <ibeosdk/devices/database/PointCloudDbReader.hpp>
#include <ibeosdk/devices/database/VehicleStateDbReader.hpp>

#include <ibeosdk/database/initMongoClient.hpp>
#include <ibeosdk/database/DbConnectionFactory.hpp>

#include <ibeosdk/devices/IdcFile.hpp>

#include <boost/program_options.hpp>

//======================================================================

using namespace ibeosdk;

//======================================================================

const ibeosdk::Version::MajorVersion majorVersion(5);
const ibeosdk::Version::MinorVersion minorVersion(2);
const ibeosdk::Version::Revision revision(2);
const ibeosdk::Version::PatchLevel patchLevel;
const ibeosdk::Version::Build build;
const std::string info = "IbeoSdkDbRoadMarkingDemo";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);
IbeoSDK ibeoSDK;

void getCollectionToConnect(const dbaccess::ProcessingJob::JobType type,
                                   const dbaccess::ProcJobList& jobList,
                                   dbaccess::CollectionName& cn)
{

	using namespace dbaccess;

	ProcJobList::const_iterator itr = jobList.begin();
	for( ; itr != jobList.end(); ++itr) {

		ProcessingJob::JobType tmpType = itr->getJobType();
		if(tmpType == type) {
			switch(type) {
				case ProcessingJob::JobType_VehicleState: {
					if(!itr->getResultConfig().empty()) {
						cn = itr->getResultConfig()[0];
					}
					break;
				}
				case ProcessingJob::JobType_RoadMarkingPoints: {
					if(!itr->getResultConfig().empty()) {
						cn = itr->getResultConfig()[0];
					}
					break;
				}
				default:
					break;//nothing to do;
			}
		}
	}
}

//======================================================================

class IdcWriter : ibeosdk::IdcFile,
                  public ibeosdk::DataListener<ScanEcu>,
                  public ibeosdk::DataListener<PointCloudPlane7510>,
                  public ibeosdk::DataListener<VehicleStateBasicEcu2808>,
                  public ibeosdk::SplitFile {
public:
	IdcWriter(const std::string filename,
	          LogFileManager& logFileManager,
	          const off_t fileSizeLimit);

	virtual ~IdcWriter();

private:
	IdcWriter(const IdcWriter&); // forbidden
	IdcWriter& operator=(const IdcWriter&); // forbidden

public:
	virtual bool create(const std::string& filename, const bool append = false);

	virtual void onData(const ScanEcu* const scan);
	virtual void onData(const PointCloudPlane7510* const pc);
	virtual void onData(const VehicleStateBasicEcu2808* const pc);

	virtual void split(const int fileIndex);

protected:
	std::string getFileName(const int fileIndex) const;
	void splitFileIfNeeded();
	void writeDataBlock(const DataBlock* const db, const uint8_t deviceId, const NTPTime ntpTime);

protected:
	LogFileManager& m_logFileManager;
	std::string m_filenameTemplate;
	std::string m_currentFileName;
	off_t m_currentFileSize;
	const off_t m_fileSizeLimit;
	uint8_t m_deviceId;

	ScanEcu lastScan;
}; // IDCWriter

//======================================================================
//======================================================================
//======================================================================

IdcWriter::IdcWriter(const std::string filename,
                     LogFileManager& logFileManager,
                     const off_t fileSizeLimit)
  : IdcFile(),
    m_logFileManager(logFileManager),
    m_filenameTemplate(filename),
    m_currentFileName(),
    m_currentFileSize(0),
    m_fileSizeLimit(fileSizeLimit),
    m_deviceId(0)
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
	writeDataBlock(&end, 0, lastScan.getStartTimestamp());
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
	const uint32_t sizeOfThisMessage = db->getSerializedSize();
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
	this->splitFileIfNeeded();

	writeDataBlock(scan, scan->getHeaderDeviceId(), scan->getStartTimestamp());
	lastScan = *scan;
}

//======================================================================

void IdcWriter::onData(const PointCloudPlane7510* const pc)
{
	writeDataBlock(pc, 0, pc->getHeaderNtpTime());
}

//======================================================================

void IdcWriter::onData(const VehicleStateBasicEcu2808* const vsb)
{
	writeDataBlock(vsb, 0, vsb->getTimestamp());
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
	    m_outputFile(),
	    m_maxFileSize(100000000)
	{
		using namespace boost::program_options;
		variables_map cmdVars;

		options_description desc("IbeoSdkDbRoadMarkingDemo\n");
		desc.add_options()
			("ip,I", value<std::string>(&m_ipAsStr)->default_value("127.0.0.1"), "Ip address of the database.")
			("username,U", value<std::string>(&m_userName)->default_value(""), "User name for the database.")
			("password,C", value<std::string>(&m_password)->default_value(""), "Password for the database.")
			("trip,T", value<std::string>(&m_tripName), "Trip name")
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

		m_hasLogFile = cmdVars.count("logfile");

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
	std::string getTripName() const { return m_tripName; }

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

	std::string m_tripName;

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
	using namespace dbaccess;

	initMongoClient();

	//=====================================
	// Connect to database
	//=====================================
	DbConnection::ConnectionResult m_result;
	DbConnection::ConnectionProperties prop(cmdargs.getUserName(), cmdargs.getPassword(),cmdargs.getIpAsString(), 27017);

	try {
		DbConnectionFactory::instance().connect(prop, m_result);
		if (m_result.m_dbErrorCode != DbConnection::ConnectionResult::RfDb_NoError)
			throw m_result;
	}
	catch (const DbConnection::ConnectionResult& error) {
		logError << "Connection failed with error code: '" << error.m_dbErrorCode << std::endl;
		logError << "Error message: " << error.m_dbErrMsg << std::endl;
		return;
	}

	IdcWriter idcWriter(cmdargs.getOutputFile(), logFileManager, 10000000);

	//=====================================
	// Read data from database
	//=====================================
	try {
		CollectionName tmpCollection("");
		TripDbReader tripReader;
		tripReader.connect();

		Trip trip(cmdargs.getTripName());
		if (!tripReader.getTrip(trip)) {
			logError << "Trip '" << trip.getName() << "' not found." << std::endl;
			return;
		}

		NTPTime start, end; //start and end time stamp, used to query the  time range of the trip

		//=====================================
		// Connect ScanDbReader to database and query first scan
		//=====================================
		ScanEcu ecuScan;

		ScanDbReader scanReader;
		tmpCollection = trip.getFullDbName(Trip::TripContentScans);
		if(!scanReader.connectToDbInterface(tmpCollection)) {
			logError << "Connecting to collection '" << tmpCollection
					 << "' failed - check if collection exists." << std::endl;
			shutdownMongoClient();
			return;
		}

		scanReader.queryTimeRange(start, end);
		scanReader.queryScanByTimeStamp(start, ecuScan);

		ecuScan.getScanPoints().clear();

		//=====================================
		// Connect VehicleStateDbReader to database and query one vehicle state
		//=====================================
		VehicleStateBasicEcu2808 vsb;

		VehicleStateDbReader vhsReader;
		vhsReader.useProcessedData(); //use processed vehicle state

		ProcJobList& list = trip.getProcessingJobs();
		getCollectionToConnect(ProcessingJob::JobType_VehicleState, list, tmpCollection);
		if(!vhsReader.connectToDbInterface(tmpCollection)) {
			logError << "Connecting to collection '" << tmpCollection
					 << "' failed - check if collection exists." << std::endl;
			shutdownMongoClient();
			return;
		}

		vhsReader.queryVehicleStateByTimeStamp(start, vsb);
		vsb.setCourseAngle(0);
		vsb.setXPosition(0);
		vsb.setYPosition(0);
		vsb.setZPosition(0);


		//=====================================
		//  Connect PointCloudDbReader to database and query RoadMarkingPoints
		//=====================================
		PointCloudPlane7510 rmPointCloud;
		rmPointCloud.setReferencePlane(ReferencePlane(vsb.getReferencePoint()));

		PointCloudDbReader pcReader;
		pcReader.setRoadMarkingPointCloudToBeRead();
		getCollectionToConnect(ProcessingJob::JobType_RoadMarkingPoints, list, tmpCollection);

		if(!pcReader.connectToDbInterface(tmpCollection)) {
			logError << "Connecting to collection '" << tmpCollection
					 << "' failed - check if collection exists." << std::endl;
			shutdownMongoClient();
			return;
		}

		pcReader.queryPointCloudPlane(rmPointCloud); // get complete pointcloud
	//	pcReader.queryPointCloudPlane(rmPointCloud, -60, 50, -900, -620, -1, 1); // get section specified by x,y,z

		//=====================================
		// Write data to idc file
		//=====================================
		idcWriter.onData(&ecuScan);
		idcWriter.onData(&vsb);
		idcWriter.onData(&rmPointCloud);
	}
	catch (const DatabaseException& e) {
		logError << "Reception failed: '" << e.what() << "'" << std::endl;
	}

	shutdownMongoClient();
}

//======================================================================
