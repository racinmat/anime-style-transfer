//======================================================================
/*! \file LogMessageError.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 30, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LOGMESSAGEERROR_HPP_SEEN
#define IBEOSDK_LOGMESSAGEERROR_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/LogMessageBase.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class LogMessageError : public LogMessageBase<LogMessageError> {
public:
	LogMessageError(const TraceLevel traceLevel = TL_Off, const std::string msg = std::string())
	  : LogMessageBase(traceLevel, msg)
	{}
}; // LogMessageError

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_LOGMESSAGEERROR_HPP_SEEN

//======================================================================
