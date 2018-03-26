/**
*
*  can mediasample content
*
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
*
 * $Author: belkera $
 * $Date: 2011-07-01 08:32:18 +0200 (Fri, 01 Jul 2011) $
 * $Revision: 7973 $
*
* @remarks
*
*/

//*************************************************************************************************
#ifndef _CANSTRUCT_H_
#define _CANSTRUCT_H_
#ifdef WIN32
#pragma pack ( push, 1 )
#else
#pragma pack ( push, 1 )
#endif
//*************************************************************************************************
// DEFAULT FORMAT DESCRIPTORS
//*************************************************************************************************

namespace adtfstreaming
{

/**
*
* Struct tCanMessage.
*
*/
typedef struct
{
    /// id of can message
    tUInt16     ui16Id;
    /// channel to send or transmit can data
    tUInt8      ui8Channel;
    /// length of data
    tUInt8      ui8Length;
    /// data field
    tUInt8      pui8Data[8];
} tCanMessage;

/**
*
* Struct tCanMessage.
*
*/
typedef struct
{
    /// id of can message
    tUInt32     ui32Id;
    /// channel to send or transmit can data
    tUInt8      ui8Channel;
    /// length of data
    tUInt8      ui8Length;
    /// data field
    tUInt8      pui8Data[8];
} tCanMessageExt;

}

#ifdef _MSC_VER
#pragma pack(pop)
#else
#pragma pack(pop)
#endif
#endif
