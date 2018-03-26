//======================================================================
/*! \file WinCompatibility.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Oct 2, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_WINCOMPATIBILITY_HPP_SEEN
#define IBEOSDK_WINCOMPATIBILITY_HPP_SEEN

//======================================================================

#ifdef _WIN32
#	ifndef _USE_MATH_DEFINES
#		define _USE_MATH_DEFINES
#	endif // _USE_MATH_DEFINES

#	ifndef NOMINMAX
#		define NOMINMAX
#	endif // NOMINMAX

#	define __func__ __FUNCTION__

#	ifndef __BIG_ENDIAN
#		define __BIG_ENDIAN 1234
#	endif // __BIG_ENDIAN

#	ifndef __LITTLE_ENDIAN
#		define __LITTLE_ENDIAN 3412
#	endif // __LITTLE_ENDIAN

#endif // _WIN32

//======================================================================

#include<ibeosdk/inttypes.hpp>
#include <fstream>

//======================================================================

namespace ibeosdk {

//======================================================================

#if defined(_MSC_VER) & _MSC_VER < 1700 // begin: VS up to 2010
inline
int64_t streamposToInt64(const std::streampos& streampos)
{
	// MSVC has a 64 bit file size, but accessible only through
	// the non-standard std::fpos::seekpos() method.
	// still visual studio 2010 does not calculate right size by itself
	const int64_t pos64 = streampos.seekpos() + std::streamoff(streampos) - _FPOSOFF(streampos.seekpos());

	return pos64;
}
#else // end: VS up to 2010; begin: VS from 2012 and linux
inline
int64_t streamposToInt64(const std::streampos& streampos)
{
	return std::streamoff(streampos);
}
#endif // begin: VS from 2012 and linux

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_WINCOMPATIBILITY_HPP_SEEN

//======================================================================
