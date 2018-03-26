/**
 *
 * Global Error Definitions.
 * @see @ref page_error_codes
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: FNEUMTH $
 * $Date: 2011-07-20 11:13:45 +0200 (Wed, 20 Jul 2011) $
 * $Revision: 8206 $
 *
 * @remarks
 *
 */
#ifndef _ERROR_BASE_HEADER_
#define _ERROR_BASE_HEADER_

/**
 *  \page page_error_codes ADTF Error Codes
 *  This describes how error codes are formed.
 *  \par Values are 32 bit values with layout out as follows:
 *  \code
 *   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 *  +---------------------------------------------------------------+
 *  |E|R|    [Facility Code]    |         Code                      |
 *  +---------------------------------------------------------------+
 *
 *  where
 *
 *      31:1   E - Error Bit (int32 Sign Bit)
 *
 *      30:1   R - is a reserved bit
 *
 *      29:12  Facility - is the facility code (not used, yet)
 *
 *      17:18  Code - is the facility's status code
 *  \endcode
 * 
 * \see For error codes see @ref error.h.
 *
 * \remarks Pre-defined facility codes (not used, yet)
 */

#ifndef AEV_ERRORS_H

/// Facility code zero
#define FAC_CODE_NULL                   0

/**
 * For general error handling functionality see @ref page_error_codes.
 */
#define _FACILITY_MASK_                0x3FFC0000L
/**
 * For general error handling functionality see @ref page_error_codes.
 */
#define _ERROR_CODE_MASK_              0x0003FFFFL
/**
 * For general error handling functionality see @ref page_error_codes.
 */
#define _MAKE_ERROR(s)                 ( - ( ( s ) ) )

/// Check if result is OK.
#define IS_OK(s)                       ( ( s ) >= 0L )

/// Check if result is failed.
#define IS_FAILED(s)                   ( ( s ) < 0L )

/// Extract the error code from a given tResult value.
#define ERROR_CODE(s)                  ( ( - ( s ) ) & _ERROR_CODE_MASK_ )

/// Return status ERR_NOERROR from the current function. This requires the calling function's
/// return type to be tResult.
#define RETURN_NOERROR                 return ERR_NOERROR

/// Return from the current function using a specific error code. This requires the calling function's
/// return type to be tResult.
#if (__UCOM_ASSERT_WHEN_LOGGING_ERRORS == 1)
#define RETURN_ERROR(code)             assert(false); return (code)
#else
#define RETURN_ERROR(code)             return (code)
#endif

/// Return from current function is expression is failed. This requires the calling function's
/// return type to be tResult.
#define RETURN_IF_FAILED(s)            { tResult __result = (s); if (IS_FAILED(__result)) { RETURN_ERROR(__result); } }

/// Return from current function is pointer is NULL. The error code that is returned is ERR_POINTER. 
/// This requires the calling function's return type to be tResult.
#define RETURN_IF_POINTER_NULL(_ptr)    if ( (tVoid*) ( _ptr ) == NULL ) { RETURN_ERROR(ERR_POINTER); }

/// This macro asserts if a pointer is NULL.
/// This assertion is only present in Debug builds and it does nothing on Release builds.
#ifdef _DEBUG
#define ASSERT_IF_POINTER_NULL(_ptr)    __assert((tVoid*) ( _ptr ) != NULL);
#else
#define ASSERT_IF_POINTER_NULL(_ptr)
#endif

/// Return status ERR_FAILED from the current function. This requires the calling function's
/// return type to be tResult.
//#define RETURN_FAILED                  RETURN_ERROR(ERR_FAILED)

/// Return status ERR_UNEXPECTED from the current function. This requires the calling function's
/// return type to be tResult.
//#define RETURN_UNEXPECTED              RETURN_ERROR(ERR_UNEXPECTED)

/**
 *  Error constants
 */
enum
{
    ERR_NOERROR                       = 0L,
    ERR_UNKNOWN                    = _MAKE_ERROR( 2 ), 
    ERR_UNEXPECTED                 = _MAKE_ERROR( 3 ),
    ERR_POINTER                    = _MAKE_ERROR( 4 ),
    ERR_INVALID_ARG                = _MAKE_ERROR( 5 ),
    ERR_INVALID_FUNCTION           = _MAKE_ERROR( 6 ),
    ERR_INVALID_ADDRESS            = _MAKE_ERROR( 7 ),
    ERR_INVALID_HANDLE             = _MAKE_ERROR( 8 ),
    ERR_INVALID_FLAGS              = _MAKE_ERROR( 9 ),
    ERR_INVALID_INDEX              = _MAKE_ERROR( 10),
    ERR_INVALID_FILE               = _MAKE_ERROR( 11 ),
    ERR_MEMORY                     = _MAKE_ERROR( 12 ),
    ERR_TIMEOUT                    = _MAKE_ERROR( 13 ),
    ERR_OUT_OF_SYNC                = _MAKE_ERROR( 14 ),
    ERR_RESSOURCE_IN_USE           = _MAKE_ERROR( 15 ),
    ERR_NOT_IMPL                   = _MAKE_ERROR( 16 ),
    ERR_NO_INTERFACE               = _MAKE_ERROR( 17 ),
    ERR_NO_CLASS                   = _MAKE_ERROR( 18 ),
    ERR_NOT_SUPPORTED              = _MAKE_ERROR( 19 ),
    ERR_NOT_FOUND                  = _MAKE_ERROR( 20 ),
    ERR_CANCELLED                  = _MAKE_ERROR( 21 ),
    ERR_RETRY                      = _MAKE_ERROR( 22 ),
    ERR_FILE_NOT_FOUND             = _MAKE_ERROR( 23 ),
    ERR_PATH_NOT_FOUND             = _MAKE_ERROR( 24 ),
    ERR_ACCESS_DENIED              = _MAKE_ERROR( 25 ),
    ERR_NOT_READY                  = _MAKE_ERROR( 26 ),
    ERR_OPEN_FAILED                = _MAKE_ERROR( 27 ),
    ERR_IO_INCOMPLETE              = _MAKE_ERROR( 28 ),
    ERR_IO_PENDING                 = _MAKE_ERROR( 29 ),
    ERR_NOACCESS                   = _MAKE_ERROR( 30 ),
    ERR_BAD_DEVICE                 = _MAKE_ERROR( 31 ),
    ERR_DEVICE_IO                  = _MAKE_ERROR( 32 ),
    ERR_DEVICE_NOT_READY           = _MAKE_ERROR( 33 ),
    ERR_DEVICE_IN_USE              = _MAKE_ERROR( 34 ),
    ERR_NOT_CONNECTED              = _MAKE_ERROR( 35 ),
    ERR_UNKNOWN_FORMAT             = _MAKE_ERROR( 36 ),
    ERR_NOT_INITIALISED            = _MAKE_ERROR( 37 ),
    ERR_FAILED                     = _MAKE_ERROR( 38 ),
    ERR_END_OF_FILE                = _MAKE_ERROR( 39 ),
    ERR_INVALID_STATE              = _MAKE_ERROR( 40 ),
    ERR_EXCEPTION_RAISED           = _MAKE_ERROR( 41 ),
    ERR_INVALID_TYPE               = _MAKE_ERROR( 42 ),
    ERR_EMPTY                      = _MAKE_ERROR( 43 ),
    ERR_INVALID_VERSION            = _MAKE_ERROR( 44 ),
    ERR_INVALID_LICENSE            = _MAKE_ERROR( 45 ),
};

#endif // AEV_ERRORS_H

#endif // _ERROR_BASE_HEADER_
