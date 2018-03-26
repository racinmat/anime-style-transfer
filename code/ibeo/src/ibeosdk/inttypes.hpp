//======================================================================
/*! \file inttypes.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 4, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_INTTYPES_HPP_SEEN
#define IBEOSDK_INTTYPES_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <boost/cstdint.hpp>

#if _MSC_VER == 1500
#	ifdef WCHAR_MIN
#		undef WCHAR_MIN
#		undef INT8_C
#		undef UINT8_C
#		undef INT16_C
#		undef UINT16_C
#		undef INT32_C
#		undef UINT32_C
#		undef INT64_C
#		undef UINT64_C
#		undef INTMAX_C
#		undef UINTMAX_C
#	endif // WCHAR_MIN
#	include <ibeosdk/stdintForVS2008.h> // using a copy of VS2010 stdint.h here
#else
#	include <stdint.h>
#endif



//======================================================================

namespace ibeosdk {

//======================================================================

typedef boost::uint8_t          BYTE;      ///<  8 bit wide unsigned integer
typedef boost::int8_t           INT8;      ///<  8 bit wide   signed integer
typedef boost::uint8_t          UINT8;     ///<  8 bit wide unsigned integer
typedef boost::int16_t          INT16;     ///< 16 bit wide   signed integer
typedef boost::uint16_t         UINT16;     ///< 16 bit wide unsigned integer
#ifndef _BASETSD_H_     // avoid naming conflict with Windows <basetsd.h>
typedef boost::int32_t          INT32;     ///< 32 bit wide   signed integer
typedef boost::uint32_t         UINT32;     ///< 32 bit wide unsigned integer
typedef boost::int64_t          INT64;     ///< 64 bit wide   signed integer
typedef boost::uint64_t         UINT64;     ///< 64 bit wide unsigned integer
#endif

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_INTTYPES_HPP_SEEN

//======================================================================
