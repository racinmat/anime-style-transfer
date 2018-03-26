/**
 *
 * Default media types
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved.
 *
 * $Author: mathias.mueller $
 * $Date: 2009-01-21 16:43:51 +0100 (Mi, 21 Jan 2009) $
 * $Revision: 6234 $
 *
 */

#ifndef _ADTF_STREAMING_MEDIATYPES_HEADER
#define _ADTF_STREAMING_MEDIATYPES_HEADER
//*************************************************************************************************
// SAMPLE TYPES for ADTF BASIC
//*************************************************************************************************
#define  STREAMING_SAMPLE_TYPE_CAN_DATA "adtf.sample.can_message"           //!< For the data area of this sample only the type adtf_devicetb::tCANData is possible.
#define  STREAMING_SAMPLE_TYPE_CAN      "adtf.sample.can_message_raw"       //!< raw can sample
#define  STREAMING_SAMPLE_TYPE_CAN_EXT  "adtf.sample.can_message_ext_raw"   //!< raw ext Can sample
#define  STREAMING_SAMPLE_TYPE_GENERIC  "adtf.core.media_sample"            //!< generic sample

//*************************************************************************************************
// MEDIA TYPES for ADTF BASIC
//*************************************************************************************************
#define  STREAMING_MEDIA_TYPE_GENERIC    "adtf.core.media_type"             //!< generic media type
#define  STREAMING_MEDIA_TYPE_AUDIO      "adtf.type.audio"                  //!< audio media type
#define  STREAMING_MEDIA_TYPE_VIDEO      "adtf.type.video"                  //!< video media type
#define  STREAMING_MEDIA_TYPE_VIDEO_COMPRESSED "adtf.type.video_compressed" //!< compressed video media type

#ifndef _DEFAULT_MEDIA_TYPES_HEADER_
/// Multiple Inclusion Protection for the default media types header
#define _DEFAULT_MEDIA_TYPES_HEADER_
//*************************************************************************************************
// COMMON TYPES and SUBTYPES FOR ADTF BASIC
//*************************************************************************************************

/// Major type code for video types
#define MEDIA_TYPE_VIDEO                            0x0100
/// Minor type code for uncompressed video types
#define MEDIA_SUBTYPE_VIDEO_UNCOMPRESSED            0x0000
/// Minor type code for compressed video types
#define MEDIA_SUBTYPE_VIDEO_COMPRESSED              0xffff

/// Major type code for CAN types
#define MEDIA_TYPE_CAN                              0x0200
/// Minor type code for raw CAN message types with old format
#define MEDIA_SUBTYPE_CAN_RAW_MESSAGEOLD            0x0000
/// Minor type code for extended raw CAN message types
#define MEDIA_SUBTYPE_CAN_RAW_MESSAGE_EXT           0x0001
/// Minor type code for raw CAN message types
#define MEDIA_SUBTYPE_CAN_RAW_MESSAGE               0x0002
/// sub type for new can data
#define MEDIA_SUBTYPE_CAN_DATA                      0x0003

/// Major type code for structured data types
#define MEDIA_TYPE_STRUCTURED_DATA                  0x0300  // structured data
/// Minor type code for structured data types inside structured data types
#define MEDIA_SUBTYPE_STRUCT_STRUCTURED             0x0000
/// Minor type code for tFloat64 data types inside structured data types
#define MEDIA_SUBTYPE_STRUCT_FLOAT64                0x0001
/// Minor type code for tFloat32 data types inside structured data types
#define MEDIA_SUBTYPE_STRUCT_FLOAT32                0x0002
/// Minor type code for tUInt32 data types inside structured data types
#define MEDIA_SUBTYPE_STRUCT_UINT32                 0x0003
/// Minor type code for tUInt16 data types inside structured data types
#define MEDIA_SUBTYPE_STRUCT_UINT16                 0x0004
/// Minor type code for tUInt8 data types inside structured data types
#define MEDIA_SUBTYPE_STRUCT_UINT8                  0x0005
/// Minor type code for generic object data types inside structured data types
#define MEDIA_SUBTYPE_STRUCT_OBJECT_GENERIC         0x0006
/// Minor type code for tUInt64 data types inside structured data types
#define MEDIA_SUBTYPE_STRUCT_UINT64                 0x0007

/// Major type code for FlexRay types
#define MEDIA_TYPE_FLEXRAY                          0x0500
//#define MEDIA_SUBTYPE_FLEXRAY_RAW_MESSAGE           0x0001  //please use now ...RAW_FRAME
/// Minor type code for raw FlexRay frame types
#define MEDIA_SUBTYPE_FLEXRAY_RAW_FRAME             0x0001
/// Minor type code for cycling FlexRay types
#define MEDIA_SUBTYPE_FLEXRAY_CYCLE                 0x0002

#define MEDIA_TYPE_INFO                             0x0600  ///< information data
#define MEDIA_SUBTYPE_INFO_MARKER                   0x0001  ///< description marker within data stream

/// Major type code for audio types
#define MEDIA_TYPE_AUDIO                            0x0800
/// Minor type code for uncompressed audio types
#define MEDIA_SUBTYPE_AUDIO_UNCOMPRESSED            0x0000

/// Major type code for command types
#define MEDIA_TYPE_COMMAND                          0x0900
/// Minor type code for GCL command types
#define MEDIA_SUBTYPE_COMMAND_GCL                   0x0001  // graphics command list

/// Major type code for reference types
#define MEDIA_TYPE_REFERENCE                        0x1000
/// Minor type code for time reference types
#define MEDIA_SUBTYPE_REF_TIME                      0x0001
/// Minor type code for position reference types
#define MEDIA_SUBTYPE_REF_POSITION                  0x0010

/// Major type code for network data types
#define MEDIA_TYPE_NETWORK_DATA                     0x2000
/// Minor type code for IP network data types
#define MEDIA_SUBTYPE_NETWORK_DATA_IP               0x0001

//*************************************************************************************************
#define MEDIA_TYPE_USER                             0x00010000  ///< offset for user-defined types

//*************************************************************************************************
#endif // _DEFAULT_MEDIA_TYPES_HEADER_

#endif // _ADTF_STREAMING_MEDIATYPES_HEADER
