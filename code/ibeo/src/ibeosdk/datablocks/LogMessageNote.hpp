//======================================================================
/*! \file LogMessageNote.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 30, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LOGMESSAGENOTE_HPP_SEEN
#define IBEOSDK_LOGMESSAGENOTE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/LogMessageBase.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class LogMessageNote : public LogMessageBase<LogMessageNote> {
public:
	LogMessageNote(const TraceLevel traceLevel = TL_Off, const std::string msg = std::string())
	  : LogMessageBase(traceLevel, msg)
	{}
}; // LogMessageNote

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_LOGMESSAGENOTE_HPP_SEEN

//======================================================================
