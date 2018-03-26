//======================================================================
/*! \file IdcCompare.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date May 25, 2016
 *
 * This tool is comparing the two given IDC files byte by byte.
 * Only the timestamps in the IbeoDataHeader will be ignored and
 * allowed to be different.
 *///-------------------------------------------------------------------


//======================================================================

#include <ibeosdk/lux.hpp>
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>
#include <ibeosdk/scala.hpp>

#include <ibeosdk/devices/IdcFile.hpp>

#include <boost/program_options.hpp>

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

//======================================================================

using namespace ibeosdk;
using namespace std;

//======================================================================

const ibeosdk::Version::MajorVersion majorVersion(5);
const ibeosdk::Version::MinorVersion minorVersion(2);
const ibeosdk::Version::Revision revision(2);
const ibeosdk::Version::PatchLevel patchLevel;
const ibeosdk::Version::Build build;
const std::string info = "IdcCompare";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);

IbeoSDK ibeoSDK;

//======================================================================

class IdcComparator {
public:
	//========================================
	/*!\brief Create an IdcComparator instance.
	 *
	 * \param[in] filename1         Name of the first file to be compared.
	 * \param[in] filename2         Name of the second file to be compared.
	 * \param[in] logFileManager    LogFileManager used for log output.
	 *///-------------------------------------
	IdcComparator(const string& filename1,
	              const string& filename2,
	              LogFileManager& logFileManager);
	~IdcComparator();

private:
	IdcComparator(const IdcComparator&); // forbidden
	IdcComparator& operator=(const IdcComparator&); // forbidden

public:
	//========================================
	/*!\brief Start the compare process.
	 *
	 * \return Returns whether the two files are
	 *         equal (\c true) or not (\c false).
	 *///-------------------------------------
	bool compare();

protected:
	//========================================
	/*!\brief Open all files.
	 * \return Returns whether opening the files
	 *         was successfull or not.
	 *///-------------------------------------
	bool openFiles();

	//========================================
	/*!\brief Checks whether all input files are open.
	 * \return \c true if both input files are open.
	 *         \c false otherwise.
	 *///-------------------------------------
	bool areFilesOpen() const;

	//========================================
	/*!\brief Compare the current datablocks
	 *        from the two files.
	 * \return \c true if the data block from
	 *         file1 (\a dh1, \a rawData1) is
	 *         identically to file2 (\a dh2, \a rawData2).
	 *         Only the header time stamps may
	 *         be different.
	 *         \c false otherwise.
	 *///-------------------------------------
	bool onData(const IbeoDataHeader& dh1,
	            const boost::shared_ptr<char> rawData1,
	            const IbeoDataHeader& dh2,
	            const boost::shared_ptr<char> rawData2);

protected:
	LogFileManager& m_logFileManager;
	string m_filename1;
	string m_filename2;

	boost::shared_ptr<IdcFile> m_inFile1;
	boost::shared_ptr<IdcFile> m_inFile2;
}; // IdcComparator

//======================================================================

IdcComparator::IdcComparator(const string& filename1,
                             const string& filename2,
                             LogFileManager& logFileManager)
  : m_logFileManager(logFileManager),
    m_filename1(filename1),
    m_filename2(filename2),
    m_inFile1(),
    m_inFile2()
{}

//======================================================================

IdcComparator::~IdcComparator() {}

//======================================================================

bool IdcComparator::compare()
{
	openFiles();

	if (!areFilesOpen()) {
		logError << "Input files are not open" << endl;
		return false;
	}

	while (m_inFile1->isGood() && m_inFile2->isGood()) {
		IbeoDataHeader dh1;
		boost::shared_ptr<char> data1 = m_inFile1->getNextDataBlockRaw(dh1);
		IbeoDataHeader dh2;
		boost::shared_ptr<char> data2 = m_inFile2->getNextDataBlockRaw(dh2);

		if (!onData(dh1, data1, dh2, data2))
			return false;
	} // while

	return true;
}

//======================================================================

bool IdcComparator::openFiles()
{
	m_inFile1.reset(new IdcFile);
	if (!m_inFile1->open(m_filename1)) {
		logError << "Could not open idc file '" << m_filename1 << "' for input" << std::endl;
		return false;
	}
	m_inFile2.reset(new IdcFile);
	if (!m_inFile2->open(m_filename2)) {
		logError << "Could not open idc file '" << m_filename2 << "' for input" << std::endl;
		return false;
	}
	return true;
}

//======================================================================

bool IdcComparator::areFilesOpen() const
{
	if (!m_inFile1.get())
		return false;
	if (!m_inFile1->isOpen())
		return false;
	if (!m_inFile2.get())
		return false;
	if (!m_inFile2->isOpen())
		return false;

	return true;
}

//======================================================================

bool IdcComparator::onData(const IbeoDataHeader& dh1,
                           const boost::shared_ptr<char> rawData1,
                           const IbeoDataHeader& dh2,
                           const boost::shared_ptr<char> rawData2)
{

	if (rawData1.get() && rawData2.get()) {
		logDebug << "Compare " << toHex(dh1.getDataType()) << "  and  " << toHex(dh2.getDataType()) << std::endl;


		if (!ibeosdk::compareIbeoDataHeaderWithoutDate(dh1, dh2)) {
			logInfo << "Header where different." << std::endl;
			return false;
		}

		if (0 != (strncmp(rawData1.get(), rawData2.get(), dh1.getMessageSize()))) {
			logInfo << "Body where different." << std::endl;
			return false;
		}
	}
	else if (rawData1.get()) {
		logInfo << "File '" << m_filename2 << "' is shorter than '" << m_filename1 << "'" << std::endl;
		return false;
	}
	else if (rawData2.get()) {
		logInfo << "File '" << m_filename1 << "' is shorter than '" << m_filename2 << "'" << std::endl;
		return false;
	}


	return true;
}

//======================================================================





//======================================================================
//======================================================================
//======================================================================





//======================================================================

class CommandLineArguments {
public:
	CommandLineArguments(int argc, const char** argv)
	  : m_inputFile(2),
	    m_hasLogFile(false),
	    m_logFilename()
	{
		using namespace boost::program_options;
		variables_map cmdVars;

		options_description desc("IdcComparator");
		desc.add_options()
			("input1,1", value<std::string>(&m_inputFile[0])->required(), "Name of the first IDC input file")
			("input2,2", value<std::string>(&m_inputFile[1])->required(), "Name of the second IDC input file")
			("logfile,l", value<std::string>(&m_logFilename),
					"Name of the log file. If omitted, std::cerr will be used.")
			("help,h", "Show this message")
			;

		positional_options_description pos_opts;
		pos_opts.add("input1",1);
		pos_opts.add("input2",1);
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

		m_hasLogFile = cmdVars.count("logfile") >= 1;
	}

public:
	const std::vector<std::string> getInputFiles() { return m_inputFile; }
	const std::string& getOutputFile() const { return m_outputFile; }
	const std::string& getLogFileName() const { return m_logFilename; }
	bool hasLogFile() const { return m_hasLogFile; }
protected:
	std::vector<std::string> m_inputFile;
	std::string m_outputFile;
	bool m_hasLogFile;
	std::string m_logFilename;
}; // CommandLineArguments

//======================================================================





//======================================================================
//======================================================================
//======================================================================





//======================================================================

int main(const int argc, const char** argv)
{
	std::cerr << argv[0] << " Version " << appVersion.toString();
	std::cerr << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;

	CommandLineArguments cmdargs(argc, argv);

	LogFileManager logFileManager;
	if (cmdargs.hasLogFile()) {
		ibeosdk::LogFile::setLogFileBaseName(cmdargs.getLogFileName());
	}

	const ibeosdk::LogLevel ll = ibeosdk::logLevelFromString("Debug2");
	ibeosdk::LogFile::setLogLevel(ll);

	logFileManager.start();

	if (cmdargs.hasLogFile()) {
		logInfo << argv[0] << " Version " << appVersion.toString()
		        << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;
	}

	IdcComparator comparator(cmdargs.getInputFiles().at(0), cmdargs.getInputFiles().at(1), logFileManager);

	if (comparator.compare()) {
		logInfo << "Files are equal." << endl;
	}
	else {
		logInfo << "Files are not equal." << endl;
	}
	exit(0);
}

//======================================================================


