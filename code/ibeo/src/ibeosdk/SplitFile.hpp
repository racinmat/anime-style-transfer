//======================================================================
/*! \file SplitFile.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 24, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SPLITFILE_HPP_SEEN
#define IBEOSDK_SPLITFILE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class SplitFile {
public:
	virtual ~SplitFile();
public:
	virtual void split(const int fileIndex) = 0;
}; // SplitFile

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SPLITFILE_HPP_SEEN

//======================================================================
