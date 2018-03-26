//======================================================================
/*! \file LogMessageWarning.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 30, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LOGMESSAGEWARNING_HPP_SEEN
#define IBEOSDK_LOGMESSAGEWARNING_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/LogMessageBase.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class LogMessageWarning : public LogMessageBase<LogMessageWarning> {
public:
	LogMessageWarning(const TraceLevel traceLevel = TL_Off, const std::string msg = std::string())
	  : LogMessageBase(traceLevel, msg)
	{}
}; // LogMessageWarning

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_LOGMESSAGEWARNING_HPP_SEEN

//======================================================================
