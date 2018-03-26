//======================================================================
/*! \file Log.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jun 8, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LOG_HPP_SEEN
#define IBEOSDK_LOG_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/Convert.hpp>
#include <ibeosdk/Time.hpp>
#include <ibeosdk/inttypes.hpp>

#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

//======================================================================

typedef struct stat Stat;

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class FileLogStreamCreator
 * \brief Class that provides static methods to create a file log stream.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Feb 27, 2014
 *///-------------------------------------------------------------------
class FileLogStreamCreator {
public:
	//========================================
	/*!\brief Creates an ostream, more precicely an ofstream, connected with
	 *        the file of the given name \a name.
	 * \param[in] name  Name of the file to be opened and an ofstream to be
	 *                  connected with.
	 * \return A pointer to the created ofstream. The caller assumes the
	 *         ownership, i.e. the caller is responsible to destroy the
	 *         ofstream object.
	 * \retval NULL  In case the file couldn't be open, no stream will be
	 *               created, thus NULL will be returned.
	 *///-------------------------------------
	static std::ofstream* create(const std::string& name);
}; // StdLogStream

//======================================================================
//======================================================================
//======================================================================

//======================================================================
/*!\class StdErrLogStreamCreator
 * \brief Class that provides static methods to create a std::cerr log stream.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Feb 27, 2014
 *///-------------------------------------------------------------------
class StdErrLogStreamCreator {
public:
	//========================================
	/*!\brief Return the pointer to the std::cerr ostream.
	 * \return Pointer to the std::cerr ostream.
	 * \attention The ownership of the ostream object the
	 *            returned pointer points to does \b not
	 *            assumed by the caller.
	 *///-------------------------------------
	static std::ostream* create();
}; // StdLogStream

//======================================================================
//======================================================================
//======================================================================

//======================================================================
/*!\class CustomLogStreamCallback
 * \brief Abstract base class for callback objects used in LogStreamBuf.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Feb 27, 2014
 *///-------------------------------------------------------------------
class CustomLogStreamCallback {
public:
	//========================================
	/*!\brief Destructor.
	 *///-------------------------------------
	virtual ~CustomLogStreamCallback() {}

public:
	//========================================
	/*!\brief Callback method that will be called when the LogStreamBuf,
	 *        that uses this object received an endl.
	 * \param[in] s  Content of the log line, as a c-string, terminated
	 *               with '\0'.
	 * \param[in] n  Number of characters inside \a s.
	 *///-------------------------------------
	virtual void onLineEnd(const char* const s, const int n) = 0;
}; // CustomLogStreamCallback

//======================================================================
//======================================================================
//======================================================================

//======================================================================
/*!\class LogStreamBuf
 * \brief streambuf with a callback, that will called on each endl received.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Feb 27, 2014
 *///-------------------------------------------------------------------
class LogStreamBuf : public std::streambuf, private boost::noncopyable {
public:
	//========================================
	/*!\brief Create a LogStreamBuf.
	 * \param[in] bufSize  Size of the buffer used to hold the
	 *                     characters.
	 * \param[in] cb       CustomLogStreamCallback object whose
	 *                     onLineEnd method will be received whenever
	 *                     an endl has been received by the ostream
	 *                     using this buffer.
	 *                     The ownership remains with the caller.
	 *///-------------------------------------
	LogStreamBuf(const std::size_t bufSize, CustomLogStreamCallback* cb);

	//========================================
	/*!\brief Deconstructor.
	 *
	 * The allocated buffer will be deleted.
	 *///-------------------------------------
	virtual ~LogStreamBuf();

protected:
	//========================================
	/*!\brief Reimplentation of std::streambuf::sync.
	 *
	 * Calls the callback's onLineEnd method if the
	 * input inside the buffer ends with '\n'.
	 * After calling the callback, the contents of
	 * the buffer will be removed.
	 *///-------------------------------------
	virtual int sync();

protected:
	const std::size_t m_bufSize; //!< Number of characters to be hold in the streambuf.
	char* const m_bufferStart; //!< Pointer to the buffer of the streambuf.
	CustomLogStreamCallback* m_cb; //!< Callback object
	boost::mutex m_mutex;
}; // LogStreamBuf

//======================================================================
//======================================================================
//======================================================================

//======================================================================
/*!\class CustomLogStreamCreator
 * \brief Class that provides static methods to create a custom log stream.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Feb 27, 2014
 *///-------------------------------------------------------------------
class CustomLogStreamCreator {
public:
	//========================================
	/*!\brief Creates an ostream connected with using the provided callback
	 *        each time a endl has been received.
	 * \param[in] cb          Callback object whose onLineEnd method will be
	 *                        called if the ostream received an endl.
	 * \param[in] bufferSize  Maximal number of characters that can be hold in
	 *                        ostream buffer.
	 * \return A pointer to the created ostream. The caller assumes the
	 *         ownership, i.e. the caller is responsible to destroy the
	 *         ostream object.
	 *///-------------------------------------
	static std::ostream* create(CustomLogStreamCallback* cb, const int bufferSize = 5000);
}; // CustomLogStreamCreator

//======================================================================
//======================================================================
//======================================================================

enum LogLevel {
	LogLevel_Quiet,
	LogLevel_Error,
	LogLevel_Warning,
	LogLevel_Info,
	LogLevel_InfoDetails,
	LogLevel_Debug,
	LogLevel_Debug2
}; // LogLevel

//======================================================================

LogLevel logLevelFromString(const std::string& llStr);

//======================================================================

class LogFile : public std::ofstream {
public:
	static const CustomLogStreamCallback* getCustomLogStreamCallback();
	static const std::string& getCurrentFilename() { return LogFile::m_currentFilename; }
	static std::ostream& getLogFile(bool& ok);

public:
	static bool hasCustomLogStreamCallback();
	static bool hasFileTargetSizeBeenReached();

public:
	static bool setLogFileIndex(const int index);
	static bool increaseIndex() { return LogFile::setLogFileIndex(++LogFile::m_logFileIndex); }

	static bool setLogFile(std::ostream& os);
	static bool setLogFileBaseName(const std::string& baseFilename);
	static void setLogLevel(const LogLevel newLogLevel);
	static void setTargetFileSize(const off_t newTargetFileSize);
	static void setCustomLogStreamCallback(CustomLogStreamCallback* const customLogStreamCallback);

public:
	static void closeLogFile();

public:

	//========================================
	/*!\brief Deprecated log function.
	 *
	 *\deprecated This function is deprecated,
	 *            used the macros logError,
	 *            logWarning, logInfo, logInfoDetails,
	 *            logDebug and logDebug2 instead.
	 *///-------------------------------------
	IBEOSDK_DEPRECATED
	static
	inline std::ostream& log(const std::string& intro,
	                         const char* const file,
	                         const char* const func,
	                         const int line)
	{
		m_logStream->clear();
		return (*m_logStream) << intro << " - " << file << "::" << func << "(" << line << "): ";
	}

	//========================================
	static
	std::ostream& _log(const LogLevel ll,
	                   const std::string& intro,
	                   const char* const file,
	                   const char* const func,
	                   const int line);

	//========================================

	static
	inline std::ostream& _logError(const char* const file,
	                               const char* const func,
	                               const int line)
	{ return _log(LogLevel_Error, "Error ", file, func, line); }

	static
	inline std::ostream& _logWarning(const char* const file,
	                               const char* const func,
	                               const int line)
	{ return _log(LogLevel_Warning, "Warning ", file, func, line); }

	static
	inline std::ostream& _logInfo(const char* const file,
	                               const char* const func,
	                               const int line)
	{ return _log(LogLevel_Info, "Info ", file, func, line); }

	static
	inline std::ostream& _logInfoDetails(const char* const file,
	                                     const char* const func,
	                                     const int line)
	{ return _log(LogLevel_InfoDetails, "InfoDetails ", file, func, line); }

	static
	inline std::ostream& _logDebug(const char* const file,
	                               const char* const func,
	                               const int line)
	{ return _log(LogLevel_Debug, "Debug ", file, func, line); }

	static
	inline std::ostream& _logDebug2(const char* const file,
	                               const char* const func,
	                               const int line)
	{ return _log(LogLevel_Debug2, "Debug2 ", file, func, line); }

public:
	static std::ostream* m_logStream;
	static std::string m_filenameTemplate;
	static std::string m_currentFilename;
	static int m_logFileIndex;
	static off_t m_logFileTargetSize;
	static LogLevel m_logLevel;
	static TimeConversion timeConversion;

protected:
	static CustomLogStreamCallback* m_customLogStreamCallback;
	static Stat m_fileStat;
	static boost::mutex m_mutex;
}; // LogFile


//======================================================================

//========================================
/*!\brief Deprecated log function.
 *
 *\deprecated This function is deprecated,
 *            used the macros logError,
 *            logWarning, logInfo, logInfoDetails,
 *            logDebug and logDebug2 instead.
 *///-------------------------------------
IBEOSDK_DEPRECATED
inline std::ostream& log(const std::string& intro,
                         const char* const file,
                         const char* const func,
                         const int line)
{
	std::cerr.clear();
	return std::cerr << intro << " - " << file << "::" << func << "(" << line << "): ";
}

//======================================================================

#define logError LogFile::_logError(__FILE__, __func__, __LINE__)
#define logInfo LogFile::_logInfo(__FILE__, __func__, __LINE__)
#define logInfoDetails LogFile::_logInfoDetails(__FILE__, __func__, __LINE__)
#define logWarning LogFile::_logWarning(__FILE__, __func__, __LINE__)
#define logDebug LogFile::_logDebug(__FILE__, __func__, __LINE__)
#define logDebug2 LogFile::_logDebug2(__FILE__, __func__, __LINE__)

//======================================================================

inline std::stringstream& toHex(std::stringstream& ss, const unsigned char t)
{
	ss << std::hex << std::setfill('0') << std::setw(2) << uint32_t(uint8_t(t));
	return ss;
}

//======================================================================

template<typename T>
std::string toHex(const T t);

//======================================================================

template<>
inline
std::string toHex<unsigned char>(const unsigned char t)
{
	std::stringstream ss;
	ss << "0x";
	toHex(ss, t);
	return ss.str();
}

//======================================================================

template<>
inline
std::string toHex<char>(const char t)
{
	return toHex(uint8_t(t));
}

//======================================================================

template<>
inline
std::string toHex<uint16_t>(const uint16_t t)
{
	std::stringstream ss;
	ss << "0x";
	ss << std::hex << std::setfill('0') << std::setw(4) << t;
//	toHex(ss, uint8_t(t>>8));
//	toHex(ss, uint8_t(t & 0xFF));
	return ss.str();
}

//======================================================================

template<>
inline
std::string toHex<int16_t>(const int16_t t) { return toHex(uint16_t(t)); }

//======================================================================

template<>
inline
std::string toHex<uint32_t>(const uint32_t t)
{
	std::stringstream ss;
	ss << "0x";
	ss << std::hex << std::setfill('0') << std::setw(8) << t;

//	toHex(ss, (unsigned char)((t>>24) & 0xFF));
//	toHex(ss, (unsigned char)((t>>16) & 0xFF));
//	toHex(ss, (unsigned char)((t>> 8) & 0xFF));
//	toHex(ss, (unsigned char)((t>> 0) & 0xFF));
	return ss.str();
}

//======================================================================

template<>
inline
std::string toHex<int32_t>(const int32_t t) { return toHex(uint32_t(t)); }

//======================================================================

} // namespace ibeosdk

//======================================================================

#include <ibeosdk/SdkAbort.hpp>

//======================================================================

#endif // IBEOSDK_LOG_HPP_SEEN

//======================================================================
