//======================================================================
/*! \file LogMessageDebug.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 30, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LOGMESSAGEDEBUG_HPP_SEEN
#define IBEOSDK_LOGMESSAGEDEBUG_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/LogMessageBase.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class LogMessageDebug : public LogMessageBase<LogMessageDebug> {
public:
	LogMessageDebug(const TraceLevel traceLevel = TL_Off, const std::string msg = std::string())
	  : LogMessageBase(traceLevel, msg)
	{}
}; // LogMessageDebug

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_LOGMESSAGEDEBUG_HPP_SEEN

//======================================================================
