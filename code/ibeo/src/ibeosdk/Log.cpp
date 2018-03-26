//======================================================================
/*! \file Log.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jun 8, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/Log.hpp>

#include <iostream>
#include <sys/stat.h>
using namespace std;

//======================================================================

namespace ibeosdk {

//======================================================================

std::ofstream*
FileLogStreamCreator::create(const std::string& name)
{
	std::ofstream* ofs = new ofstream(name.c_str(),
	                                  ios_base::out | ios_base::app);
	if (ofs->is_open())
		return ofs;

	delete ofs;
	return NULL;
}

//======================================================================

std::ostream*
StdErrLogStreamCreator::create()
{
	return &std::cerr;
}

//======================================================================

LogStreamBuf::LogStreamBuf(const std::size_t bufSize, CustomLogStreamCallback* cb)
  : std::streambuf(),
    m_bufSize(bufSize),
    m_bufferStart(new char[m_bufSize]),
    m_cb(cb),
    m_mutex()
{
	setp(m_bufferStart, m_bufferStart+m_bufSize);
	setg(m_bufferStart, m_bufferStart, m_bufferStart+m_bufSize);
}

//======================================================================

LogStreamBuf::~LogStreamBuf()
{
	delete m_bufferStart;
}

//======================================================================

int LogStreamBuf::sync()
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);

	const int nb = int(pptr()-pbase());
	if (nb == 0)
		return 0;

	//TODO: Under windows: remove also \r?
	if (*(pptr()-1) == '\n') {
		*(pptr()-1) = '\0';
		//TODO: correct nb, that is passed to endLineEnd
		//TODO: check m_cb for NULL?
		m_cb->onLineEnd(pbase(), nb);
		pbump(-nb);
		return nb;
	}
	return 0;
}

//======================================================================

std::ostream*
CustomLogStreamCreator::create(CustomLogStreamCallback* cb, const int bufferSize)
{
	std::streambuf* logStreamBuf = new LogStreamBuf(std::size_t(bufferSize), cb);
	return new std::ostream(logStreamBuf);
}

//======================================================================

LogLevel logLevelFromString(const std::string& llStr)
{
	if (llStr == "Error") return LogLevel_Error;
	if (llStr == "Warning") return LogLevel_Warning;
	if (llStr == "Info") return LogLevel_Info;
	if (llStr == "InfoDetails") return LogLevel_InfoDetails;
	if (llStr == "Debug") return LogLevel_Debug;
	if (llStr == "Debug2") return LogLevel_Debug2;

	std::cerr << "Unknown log level '" << llStr << "' given. "
			<< "Defaulting to LogLevel Info" << std::endl;

	return LogLevel_Info;
}

//======================================================================

//static
std::ostream* LogFile::m_logStream = NULL;
std::string LogFile::m_filenameTemplate = std::string();
std::string LogFile::m_currentFilename = std::string();
int LogFile::m_logFileIndex = 0;
off_t LogFile::m_logFileTargetSize = off_t(10000000);
LogLevel LogFile::m_logLevel = LogLevel_Error;
TimeConversion LogFile::timeConversion;

CustomLogStreamCallback* LogFile::m_customLogStreamCallback = NULL;
Stat LogFile::m_fileStat;
boost::mutex LogFile::m_mutex;

//======================================================================

//static
bool LogFile::setLogFileIndex(const int index)
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);
	LogFile::closeLogFile();
	LogFile::m_logFileIndex = index;

	bool isOpen;
	LogFile::m_logStream = &ibeosdk::LogFile::getLogFile(isOpen);

	if (isOpen && !LogFile::m_logStream->fail() && !LogFile::m_logStream->bad()) {
		std::cerr << "Successfully opened log file '" << m_currentFilename << "'" << std::endl;
		return true;
	}
	else {
		std::cerr << "Failed to open log file '" << m_currentFilename << "' - using std::cerr" << std::endl;
		return false;
	}
}

//======================================================================


//static
ostream& LogFile::getLogFile(bool& ok)
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);
	if (LogFile::m_logStream) {
		ok = true;
		return *(LogFile::m_logStream);
	}

	if (LogFile::m_customLogStreamCallback) {
		LogFile::m_logStream = CustomLogStreamCreator::create(LogFile::m_customLogStreamCallback);
	}
	else if (LogFile::m_filenameTemplate.empty() || LogFile::m_filenameTemplate == "cerr") {
		LogFile::m_logStream = StdErrLogStreamCreator::create();
	}
	else {
		stringstream ss;
		ss << LogFile::m_filenameTemplate;
		ss << "_" << setfill('0') << setw(4) << LogFile::m_logFileIndex;
		ss << ".log";

		LogFile::m_currentFilename = ss.str();

		LogFile::m_logStream = FileLogStreamCreator::create(LogFile::m_currentFilename);
	}
	ok = (LogFile::m_logStream != NULL);
	return *(LogFile::m_logStream);
}

//======================================================================

//static
bool LogFile::setLogFile(std::ostream& os)
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);
	if (LogFile::m_logStream)
		return false;

	LogFile::m_logStream = &os;
	return true;
}

//======================================================================

//static
bool LogFile::setLogFileBaseName(const std::string& baseFilename)
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);
	if (!LogFile::m_filenameTemplate.empty())
		return false;

	const std::string::size_type len = std::string::size_type(baseFilename.length());
	if (len > 4 && baseFilename.substr(len-4)==".log")
		LogFile::m_filenameTemplate = baseFilename.substr(0,len-4);
	else
		LogFile::m_filenameTemplate = baseFilename;
	return true;
}

//======================================================================

//static
void LogFile::setLogLevel(const LogLevel newLogLevel)
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);
	LogFile::m_logLevel = newLogLevel;
}


//======================================================================

//static
void LogFile::setTargetFileSize(const off_t newTargetFileSize)
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);
	LogFile::m_logFileTargetSize = newTargetFileSize;
}

//======================================================================

//static
void LogFile::setCustomLogStreamCallback(CustomLogStreamCallback* const clsc)
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);
	LogFile::m_customLogStreamCallback = clsc;
}

//======================================================================

const CustomLogStreamCallback* LogFile::getCustomLogStreamCallback()
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);
	return LogFile::m_customLogStreamCallback;
}

//======================================================================

bool LogFile::hasCustomLogStreamCallback()
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);
	return LogFile::m_customLogStreamCallback != NULL;
}

//======================================================================

//static
void LogFile::closeLogFile()
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);
	if (LogFile::m_logStream) {
		if (std::ofstream* ofs = dynamic_cast<std::ofstream*>(LogFile::m_logStream)) {
			ofs->close();
			delete ofs;
		}
		LogFile::m_logStream = NULL;
	}
}

//======================================================================

//static
bool LogFile::hasFileTargetSizeBeenReached()
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);
	if (LogFile::m_logStream) {
		if (LogFile::m_currentFilename.empty())
			return false;

		if (stat(LogFile::m_currentFilename.c_str(), &LogFile::m_fileStat) == 0) {
//			std::cerr << "File Pos: " << LogFile::fileStat.st_size << std::endl;
			return (LogFile::m_fileStat.st_size >= LogFile::m_logFileTargetSize);
		}
		else {
			logError << "Cannot stat the the file size of current log file '"
					<< LogFile::m_currentFilename << "'" << endl;
			return false;
		}
	}
	return false;
}

//======================================================================

std::ostream& LogFile::_log(const LogLevel ll,
                            const std::string& intro,
                            const char* const file,
                            const char* const func,
                            const int line)
{
	boost::lock_guard<boost::mutex> lockGuard(m_mutex);

	using namespace boost::posix_time;

	if (m_logLevel < ll) {
		m_logStream->clear(std::ios::failbit);
		return (*m_logStream);
	}
	else
		m_logStream->clear();

# ifdef _WIN32
	ptime ts = microsec_clock::local_time();

	return (*m_logStream) << timeConversion.toString(ts, 6) << " "
	                    << intro
	                    << " - " << file << "::" << func << "(" << line << "): ";

# else // _WIN32
	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	tm* ltime = localtime(&ts.tv_sec);
	char timeStr[32];
	strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S.", ltime);

	return (*m_logStream) //<< "[" << boost::this_thread::get_id() << "] "
	                    << timeStr
	                    << std::setfill('0') << std::setw(6) << ts.tv_nsec/1000 << std::setfill(' ') << " "
	                    << intro
	                    << " - " << file << "::" << func << "(" << line << "): ";
# endif // _WIN32
}


//======================================================================

} // namespace ibeosdk

//======================================================================
