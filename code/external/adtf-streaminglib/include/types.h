/**
 *
 * Standard type declarations.
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved.
 *
 * $Author: FNEUMTH $
 * $Date: 2011-07-20 16:42:26 +0200 (Wed, 20 Jul 2011) $
 * $Revision: 8251 $
 * @remarks
 *
 */

#ifndef  BASE_TYPES_H
#define  BASE_TYPES_H

#include "pstdint.h"

/**
 * The tVoid is always the definition for the void (non-type).
 */
typedef void                    tVoid;                  /* .v...    */
/**
 * The tBool defines the type for the Values @ref tTrue and @ref tFalse (platform and compiler dependent).
 */
typedef bool                    tBool;                  /* .b...    */
/**
 * The tChar defines the type for platform character set (platform and compiler dependent type).
 */
typedef char                    tChar;                  /* .c...    */
/**
 * The tWChar defines the type for platform wide character set (platform and compiler dependent type).
 */
typedef signed short int        tWChar;                 /* .wc..    */

/**
 * \typedef tMemoryAddress
 * The tMemoryAddress always defines a type for the platforms memory address pointer (see tVoid*) (32 / 64 bit at the moment)
 * (platform and compiler dependent type).
 */
#ifdef WIN32 
    #ifdef _WIN64
        typedef     tUInt64  tMemoryAddress;
    #else
        typedef     tUInt32  tMemoryAddress; 
    #endif
#else
    #if (__WORDSIZE == 64)
        typedef     tUInt64  tMemoryAddress;
    #else
        #if (__WORDSIZE == 32)
            typedef     tUInt32  tMemoryAddress;
        #else
            #if (__WORDSIZE == 16)
                typedef     tUInt16  tMemoryAddress;
            #else
                typedef     tUInt    tMemoryAddress;
            #endif
        #endif 
    #endif
#endif 


/// type definition for Float32 (32bit float values) (platform and compiler independent type).
typedef float                   tFloat32;               /* .f32...  */
/// type definition for Float64 (64bit double values) (platform and compiler independent type).
typedef double                  tFloat64;               /* .f64...  */
/// type definition for Float128 (128bit double values).(platform and compiler independent type).
typedef long double             tFloat128;              /* .f80..   */

/// type definition for signed integer value (platform and compiler dependent type).
typedef int                     tInt;                   /* .n...    */
/// type definition for unsigned integer value (platform and compiler dependent type).
typedef unsigned int            tUInt;                  /* .n...    */
/// type definition for double value (platform and compiler dependent type).
typedef double                  tFloat;                 /* .f...    */

/// type definition for a file or stream size value (platform and compiler independent type).
typedef tInt64                  tFileSize;              /* .sz...   */
/// type definition for a file or stream position value (platform and compiler independent type).
typedef tInt64                  tFilePos;               /* .pos...  */
/// type definition for a handle value (platform and compiler dependent type).
typedef tVoid*                  tHandle;                /* .h...    */
///the standard result code see @ref error.h.
typedef tInt32                  tResult;                /* .n...    */
///type definition for a time value. This is always in microseconds (if not the declaration of a use needs to mark this explicit). (platform and compiler independent type).
typedef tInt64                  tTimeStamp;             /* .n...    */
/// type definition for a array size values, map size values  etc. (platform and compiler dependent type).
typedef size_t                  tSize;                  /* .sz...   */

#ifndef WIN32
    /// TCHAR for linux platforms
    typedef char                    TCHAR;
#endif // WIN32

#endif /* BASE_TYPES_H */
