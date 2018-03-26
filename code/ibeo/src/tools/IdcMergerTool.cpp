//======================================================================
/*! \file IdcMergerTool.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Mar 20, 2014
 *
 * Demo project to merge two IDC files on base on their IbeoDataHeader
 * time stamps into a comman file.
 *///-------------------------------------------------------------------

#include <ibeosdk/lux.hpp>
#include <ibeosdk/ecu.hpp>
#include <ibeosdk/minilux.hpp>
#include <ibeosdk/scala.hpp>

#include <ibeosdk/devices/IdcFile.hpp>

#include <boost/program_options.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

//======================================================================

using namespace ibeosdk;
using namespace std;

//======================================================================

const ibeosdk::Version::MajorVersion majorVersion(5);
const ibeosdk::Version::MinorVersion minorVersion(2);
const ibeosdk::Version::Revision revision(2);
const ibeosdk::Version::PatchLevel patchLevel;
const ibeosdk::Version::Build build;
const std::string info = "IdcMergerTool";

ibeosdk::Version appVersion(majorVersion, minorVersion, revision, patchLevel, build, info);

IbeoSDK ibeoSDK;

//======================================================================
/*!\class IdcMerger
 * \brief
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Apr 8, 2014
 *
 * This class will merge several IDC sources into a single IDC file.
 * After creating an IdcMerger instance the process will be started
 * calling #merge. The method #selectInput will decide from which
 * package the next IDC datablock will be stored into the output file.
 * The standard implementation will decide on the time stamp of the
 * datablock's headers which comes first. If the time stamp is equal,
 * the datablock from the first file given will be take first.
 *
 * Reimplement #selectInput to implement your own way to merge the
 * input files.
 *
 * \attention IdcMerger expect the IDC datablocks inside the file to
 *            be in chronicle order. I.e. you cannot change the order
 *            of the datablocks from a file.
 *///-------------------------------------------------------------------
class IdcMerger : private IdcFile {
protected:
	//==================================================================
	/*!\class BlockDescr
	 * \brief Holds the contents of a datablock.
	 *
	 * The header is deserialized, the body is still serialized inside
	 * the buffer belonging to this BlockDescr.
	 * \author Jan Christian Dittmer (jcd)
	 * \version 0.1
	 * \date Apr 8, 2014
	 *///---------------------------------------------------------------
	class BlockDescr {
	public:
		BlockDescr() : m_dh(), m_data() {}

	public:
		IbeoDataHeader m_dh;
		boost::shared_ptr<char> m_data;
	}; // BlockDescr

public:
	//========================================
	/*!\brief Create an IdcMerger instance.
	 *
	 * \param[in] outName         Name of the output file.
	 * \param[in] inNames         Vector of filenames of
	 *                            the input files.
	 * \param[in] logFileManager  LogFileManager used
	 *                            for log output.
	 *///-------------------------------------
	IdcMerger(const string& outName,
	          const vector<string>& inNames,
	          LogFileManager& logFileManager);

	~IdcMerger();

private:
	IdcMerger(const IdcMerger&); // forbidden
	IdcMerger& operator=(const IdcMerger&); // forbidden

public:
	//========================================
	/*!\brief Start the merge process.
	 *
	 * \return Returns whether the merge process
	 *         was successfull or not.
	 *///-------------------------------------
	bool merge();

protected:
	//========================================
	/*!\brief Open all files.
	 * \return Returns whether opening the files
	 *         was successfull or not.
	 *///-------------------------------------
	bool openFiles();

	//========================================
	/*!\brief Select the file from which to take the
	 *        next datablock.
	 *
	 * \param[out] idx  Index of the file from which
	 *                  to take the next datablock.
	 * \return Returns whether there is any datablock
	 *         left to write.
	 *///-------------------------------------
	virtual bool selectInput(size_t& idx);

	//========================================
	/*!\brief Write the given datablock to the output file.
	 *
	 * \param[in, out] bd  The datablock to write to the
	 *                     output file. On output the
	 *                     BlockDescr::m_dh::m_sizeOfPrevMessage
	 *                     attribute has been updated.
	 * \return Returns whether the output stream is still good.
	 *///-------------------------------------
	bool onData(BlockDescr& bd);

protected:
	LogFileManager& m_logFileManager;
	string m_outName;
	vector<string> m_inNames;

	vector<boost::shared_ptr<IdcFile> > m_inFiles;
	vector<BlockDescr> m_currBlocks;
	uint32_t m_previousMsgSize;
}; // IDCWriter

//======================================================================

IdcMerger::IdcMerger(const string& outName,
	                   const vector<string>& inNames,
	                   LogFileManager& logFileManager)
  : IdcFile(),
    m_logFileManager(logFileManager),
    m_outName(outName),
		m_inNames(inNames),
		m_inFiles(inNames.size()),
		m_currBlocks(inNames.size()),
		m_previousMsgSize(0)
{}

//======================================================================

IdcMerger::~IdcMerger() {}

//======================================================================

bool IdcMerger::merge()
{
	if (this->m_inNames.empty())
		return false;

	if (!openFiles())
		return false;

	int validInputs = 0;
	for (size_t i = 0; i < m_inFiles.size(); ++i) {
		m_currBlocks[i].m_data = m_inFiles[i]->getNextDataBlockRaw(m_currBlocks[i].m_dh);
		if (m_currBlocks.at(i).m_data.get())
			++validInputs;
	}

	bool hasValidInputs = validInputs > 0;
	if (!hasValidInputs)
		return true;

	size_t i = 0;
	while ((hasValidInputs = selectInput(i))) {
		logInfo << "Selected " << i << " - "
				<< "0x" << std::hex << setfill('0') << setw(4) << m_currBlocks[i].m_dh.getDataType() << std::dec << setfill(' ')
				<< "  time: " << m_currBlocks[i].m_dh.getTimestamp().toPtime() << endl;
		bool ok = this->onData(m_currBlocks[i]);
		if (!ok)
			return false;
		m_currBlocks[i].m_data = m_inFiles[i]->getNextDataBlockRaw(m_currBlocks[i].m_dh);
	}

	return true;
}

//======================================================================

bool IdcMerger::openFiles()
{
	for (size_t i = 0; i < m_inNames.size(); ++i) {
		m_inFiles[i].reset(new IdcFile);
		if (!m_inFiles[i]->open(m_inNames.at(i))) {
			logError << "Could not open idc file '" << m_inNames.at(i) << "' for input" << std::endl;
			return false;
		}
	} // for i

	if (!IdcFile::create(m_outName, false)) {
		logError << "Could not open idc file '" << m_outName << "' for writing" << std::endl;
		return false;
	}

	return true;
}

//======================================================================

bool IdcMerger::selectInput(size_t& idx)
{
	bool hasValid = false;
	NTPTime minTime;
	for (size_t i = 0; i < this->m_inFiles.size(); ++i) {
		if (this->m_currBlocks.at(i).m_data.get()) {
			const NTPTime time = this->m_currBlocks.at(i).m_dh.getTimestamp();
			if (!hasValid || time < minTime) {
				minTime = time;
				idx = i;
				hasValid = true;
			}
		} // if valid
	} // for i

	return hasValid;
}

//======================================================================

bool IdcMerger::onData(BlockDescr& bd)
{
	if (bd.m_dh.getDataType()==DataTypeId::DataType_IdcTrailer
		|| bd.m_dh.getDataType()==DataTypeId(0x6130)) {
		logError << "Skip datatype " << "0x" <<  std::hex << bd.m_dh.getDataType() << std::dec << std::endl;
		return true;
	}

	// adjust the size of the previous message
	bd.m_dh.setPreviousMessageSize(this->m_previousMsgSize);
	this->m_previousMsgSize = bd.m_dh.getMessageSize();
	//bd.m_dh.serialize(this->m_strm);

	return this->write(bd.m_dh, bd.m_data.get());
}

//======================================================================
//======================================================================
//======================================================================

class CommandLineArguments {
public:
	CommandLineArguments(int argc, const char** argv)
	  : m_inputFile(2),
	    m_outputFile(),
	    m_hasLogFile(false),
	    m_logFilename()
	{
		using namespace boost::program_options;
		variables_map cmdVars;

		options_description desc("IbeoSdkFileMergerDemo");
		desc.add_options()
			("input1,1", value<std::string>(&m_inputFile[0])->required(), "Name of the first IDC input file")
			("input2,2", value<std::string>(&m_inputFile[1])->required(), "Name of the second IDC input file")
			("output,O", value<std::string>(&m_outputFile)->required(), "Name of the output IDC file")
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

int main(const int argc, const char** argv)
{
	std::cerr << argv[0] << " Version " << appVersion.toString();
	std::cerr << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;

	CommandLineArguments cmdargs(argc, argv);
	//if (!cmdargs.isValid())
	//	exit(1);

	LogFileManager logFileManager;
	if (cmdargs.hasLogFile()) {
		ibeosdk::LogFile::setLogFileBaseName(cmdargs.getLogFileName());
	}
//	ibeosdk::LogFile::setTargetFileSize(cmdargs.getMaxFileSize());

	const ibeosdk::LogLevel ll = ibeosdk::logLevelFromString("Debug2");
	ibeosdk::LogFile::setLogLevel(ll);

	logFileManager.start();

	if (cmdargs.hasLogFile()) {
		logInfo << argv[0] << " Version " << appVersion.toString()
		        << "  using IbeoSDK " << ibeoSDK.getVersion().toString() << std::endl;
	}

	IdcMerger merger(cmdargs.getOutputFile(), cmdargs.getInputFiles(), logFileManager);
	if (!merger.merge()) {
		logError << "Merging failed" << endl;
		exit(1);
	}
	exit(0);
}

//======================================================================
