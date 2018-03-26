/**
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: FNEUMTH $
 * $Date: 2011-07-20 16:42:26 +0200 (Wed, 20 Jul 2011) $
 * $Revision: 8251 $
 *
 * @remarks
 *
 */
#ifndef _ADTF_MEDIA_DESCRIPTION_HEADER_
#define _ADTF_MEDIA_DESCRIPTION_HEADER_


namespace adtfstreaming
{

/**
 * Enumeration of flags to determine the needed return format of
 * IADTFFileReader::GetStreamMediaDescription method. The enumeration value is
 * used for \c ui32Flags parameter.
 */
typedef enum
{
    /**
     * Returns the name of the Stream Type Structure.
     */
    MDF_StreamDataTypeName       = 0x0004000,

    /**
     * Returns the description of a stream.
     */
    MDF_StreamDescription       = 0x0010000
} tMediaDecriptionFlags;

}   // namespace adtfstreaming

#endif //_ADTF_MEDIA_DESCRIPTION_HEADER_
