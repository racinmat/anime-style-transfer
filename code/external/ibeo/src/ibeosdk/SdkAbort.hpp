//======================================================================
/*! \file SdkAbort.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 5, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SDKABORT_HPP_SEEN
#define IBEOSDK_SDKABORT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <stdlib.h>

//======================================================================

#ifndef IBEOSDK_LOG_HPP_SEEN
#error "Has to be included by Log.hpp"
#endif // IBEOSDK_LOG_HPP_SEEN

//======================================================================

namespace ibeosdk {

//======================================================================

inline
void sdkAbort()
{
	LogFile::closeLogFile();
	abort();
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SDKABORT_HPP_SEEN

//======================================================================

