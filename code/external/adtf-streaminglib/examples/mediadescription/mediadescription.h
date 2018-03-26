/**
*
*  example structs for use with the media description language
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

//*************************************************************************************************
#ifndef _MEDIA_DESCRIPTION_H_
#define _MEDIA_DESCRIPTION_H_

#include <adtf_streaming.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

    /**
     * Structure for demonstration purposes.
     */
    typedef struct {
        tUInt32 nTestVal1;  ///< first test value
        tUInt16 nTestVal2;  ///< second test value
    } tMyType;

    /**
     * Structure for demonstration purposes.
     */
    typedef struct {
        tUInt32 nTestVal1;  ///< first test value
        tUInt16 nTestVal2;  ///< second test value
        tUInt16 nTestVal3;  ///< third test value
        tUInt16 nTestVal4;  ///< forth test value
        tUInt16 nTestVal5;  ///< fifth test value
        tFloat64 fTestVal6; ///< sixth test value
    } tMySecondType;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif //_MEDIA_DESCRIPTION_H_