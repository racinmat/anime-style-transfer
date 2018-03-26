/**
 *
 * Global constant definitions.
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

#ifndef  BASE_CONSTANTS_H
#define  BASE_CONSTANTS_H

/*
definitions of prefix
*/

/// prefix for global variables / objects
#define  GLOBAL         /* */               /* g_... */
/// prefix for members variables / objects
#define  MEMBER         /* */               /* m_... */
/// prefix for local variables / objects
#define  LOCAL          auto                /* l_... */
/// prefix for static variables / objects
#define  STATIC         static              /* s_... */
/// prefix for extern variables / objects
#define  EXTERN         extern              /* g_... */
/// prefix for register variables / objects
#define  REGIST         register            /* r_... */

/*
definitions for function paramters
*/
#ifndef IN
/// definition for an incoming paramater
#define IN             /* in */
#endif

#ifndef OUT
/// definition for an outcoming paramater
#define OUT            /* out */
#endif

#ifndef INOUT
/// definition for an in and outcoming paramater
#define INOUT          /* in,out */
#endif

#ifndef OPTIONAL
/// definition for an optional paramater
#define OPTIONAL       /* optional */
#endif

#ifndef UNUSED
/// definition for an in and outcoming paramater to functions
#define UNUSED         /* unused */
#endif

/*
generic constants
*/
/// Value for @ref tBool.
#define tFalse          false
/// Value for @ref tBool.
#define tTrue           true

/// default value for invalid size
#define INVALID_SIZE    0xFFFFFFFF
/// default value for invalid positions
#define INVALID_POS     0xFFFFFFFF
/// default value for invalid handles
#define INVALID_HANDLE  0xFFFFFFFF

/*
general bit declarations
*/

#define  BIT0           0x0001          //!< Define for the value of bit 0
#define  BIT1           0x0002          //!< Define for the value of bit 1
#define  BIT2           0x0004          //!< Define for the value of bit 2
#define  BIT3           0x0008          //!< Define for the value of bit 3
#define  BIT4           0x0010          //!< Define for the value of bit 4
#define  BIT5           0x0020          //!< Define for the value of bit 5
#define  BIT6           0x0040          //!< Define for the value of bit 6
#define  BIT7           0x0080          //!< Define for the value of bit 7
#define  BIT8           0x0100          //!< Define for the value of bit 8
#define  BIT9           0x0200          //!< Define for the value of bit 9
#define  BIT10          0x0400          //!< Define for the value of bit 10
#define  BIT11          0x0800          //!< Define for the value of bit 11
#define  BIT12          0x1000          //!< Define for the value of bit 12
#define  BIT13          0x2000          //!< Define for the value of bit 13
#define  BIT14          0x4000          //!< Define for the value of bit 14
#define  BIT15          0x8000          //!< Define for the value of bit 15
#define  BIT16          0x00010000      //!< Define for the value of bit 16
#define  BIT17          0x00020000      //!< Define for the value of bit 17
#define  BIT18          0x00040000      //!< Define for the value of bit 18
#define  BIT19          0x00080000      //!< Define for the value of bit 19
#define  BIT20          0x00100000      //!< Define for the value of bit 20
#define  BIT21          0x00200000      //!< Define for the value of bit 21
#define  BIT22          0x00400000      //!< Define for the value of bit 22
#define  BIT23          0x00800000      //!< Define for the value of bit 23
#define  BIT24          0x01000000      //!< Define for the value of bit 24
#define  BIT25          0x02000000      //!< Define for the value of bit 25
#define  BIT26          0x04000000      //!< Define for the value of bit 26
#define  BIT27          0x08000000      //!< Define for the value of bit 27
#define  BIT28          0x10000000      //!< Define for the value of bit 28
#define  BIT29          0x20000000      //!< Define for the value of bit 29
#define  BIT30          0x40000000      //!< Define for the value of bit 30
#define  BIT31          0x80000000      //!< Define for the value of bit 31

/// Helper macro to get the highbyte of the argument
#define  fHIGHBYTE(x)   ( (( x ) & 0xFF00) >> 8 )
/// Helper macro to get the lowbyte of the argument
#define  fLOWBYTE(x)    ( ( x ) & 0x00FF )          

/// Helper macro to get the highword of the argument
#define  fHIGHWORD(x)   ( (( x ) & 0xFFFF0000) >> 16 )
/// Helper macro to get the lowword of the argument
#define  fLOWWORD(x)    ( ( x ) & 0x0000FFFF )  

///defines a common value for unsupportes platform
#define PLATFORM_NOT_SUPPORTED        0x00

///defines the little endianess value, that will be retrieved by @see __get_platform_byteorder.
#define PLATFORM_LITTLE_ENDIAN_8      0x01

///defines the big endianess value, that will be retrieved by @see __get_platform_byteorder.
#define PLATFORM_BIG_ENDIAN_8         0x02
       
///defines a link to @ref __get_platform_byteorder.
#define PLATFORM_BYTEORDER          __get_platform_byteorder()

/**
 * This function retrieves the platform dependent byte order.
 * @return @ref PLATFORM_NOT_SUPPORTED, @ref PLATFORM_LITTLE_ENDIAN_8 or @ref PLATFORM_BIG_ENDIAN_8.
 */
static inline unsigned char __get_platform_byteorder()
{
    tUInt32 ui32Value = 0x01020304;
    if (((unsigned char*)&ui32Value)[0] == 0x04 &&
        ((unsigned char*)&ui32Value)[2] == 0x02 )
    {
        return PLATFORM_LITTLE_ENDIAN_8;
    }
    else if (((unsigned char*)&ui32Value)[0] == 0x01 &&
            ((unsigned char*)&ui32Value)[2] == 0x03 )
    {
        return PLATFORM_BIG_ENDIAN_8;
    }
    else
    {
        return PLATFORM_NOT_SUPPORTED;
    }
}

///defines the MS VC++ Compiler 7 release value (VS 2003)
#define PLATFORM_COMPILER_MSVC7_RELEASE  1300
///defines the MS VC++ Compiler 7 debug value (VS 2003)
#define PLATFORM_COMPILER_MSVC7_DEBUG   11300

///defines the MS VC++ Compiler 8 release value (VS 2005)
#define PLATFORM_COMPILER_MSVC8_RELEASE  1400
///defines the MS VC++ Compiler 8 debug value (VS 2005)
#define PLATFORM_COMPILER_MSVC8_DEBUG   11400

///defines the MS VC++ Compiler 9 release value (VS 2008)
#define PLATFORM_COMPILER_MSVC9_RELEASE  1500
///defines the MS VC++ Compiler 9 debug value (VS 2008)
#define PLATFORM_COMPILER_MSVC9_DEBUG   11500

//System versions (this can be move to another file if necessary)

///Major Version for Vista
#define WINDOWS_SYSTEM_MAJOR_VERSION_VISTA 6
///Minor Version for Vista
#define WINDOWS_SYSTEM_MINOR_VERSION_VISTA 0

///Major Version for WinXP
#define WINDOWS_SYSTEM_MAJOR_VERSION_XP 5
///Minor Version for WinXP
#define WINDOWS_SYSTEM_MINOR_VERSION_XP 1

///Major Version for WinXP System
#define WINDOWS_SYSTEM_MAJOR_VERSION_2000 5
///Minor Version for WinXP System
#define WINDOWS_SYSTEM_MINOR_VERSION_2000 0

/**
 * \def PLATFORM_COMPILER_CURRENT
 * On Windows this define will be set to the current compiler version of MS VC++.
 * On Linux-Gcc it is set to 0.
 * @see @ref PLATFORM_COMPILER_MSVC9_RELEASE,\n
 *      @ref PLATFORM_COMPILER_MSVC9_DEBUG,\n
 *      @ref PLATFORM_COMPILER_MSVC8_RELEASE,\n
 *      @ref PLATFORM_COMPILER_MSVC8_DEBUG.
 */
#ifdef _MSC_VER 
    #ifdef _DEBUG
        #define PLATFORM_COMPILER_CURRENT   ((_MSC_VER / 100) * 100 + 10000)
    #else
        #define PLATFORM_COMPILER_CURRENT   ((_MSC_VER / 100) * 100)
    #endif
#else
    #define     PLATFORM_COMPILER_CURRENT   0
#endif

#ifndef WIN32
    /// maps the _stricmp to the unix strcasecmp
    #define _stricmp strcasecmp
    /// maps the _strnicmp to the unix strncasecmp
    #define _strnicmp strncasecmp
#endif


   #ifdef _ADTF_NO_DEPRECATED_WARNING
       #define ADTF_DEPRECATED(_func_) _func_
   #else
       #ifndef WIN32 
           #define ADTF_DEPRECATED(_func_) __attribute__ ((deprecated)) _func_
       #else
          #define ADTF_DEPRECATED(_func_) __declspec( deprecated("This is deprecated. See documentation for further information!")) _func_
       #endif
    #endif


/**
 * \def ADTF_DEPRECATED
 *   Mark a function or variable as deprecated.
 *   To mark  the function   \code void foo(...); \endcode as deprecated declare
 *   \code void ADTF_DEPRECATED(foo)(...); \endcode
 *   @param _func_   The function or variable
 */


#endif /* BASE_CONSTANTS_H */
