/**
 *
 * ADTF Streaming Library Types.
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: VTW8PSR $
 * $Date: 2014-11-03 13:26:26 +0100 (Mon, 03 Nov 2014) $
 * $Revision: 30631 $
 *
 * @remarks
 *
 */
#ifndef _ADTF_STREAMINGLIB_TYPES_HEADER_
#define _ADTF_STREAMINGLIB_TYPES_HEADER_

/// ID for Version 1 of ADTF/RICE codec
#define ADTF_CODEC_INFO_VERSION_1_x  0x1
/// ID for Version 2 of ADTF/RICE codec
#define ADTF_CODEC_INFO_VERSION_2_0  0x2

/// General namespace of the ADTF StreamingLib
namespace adtfstreaming
{

#pragma pack(push, 1)

    /**
    *
    *   The tADTFMediaDescriptor structure specifies the media descriptor information, that is,
    *   interesting data about the opened dat file.
    *
    */
    typedef struct tagADTFMediaDescriptor
    {
        tDateTime           sDateTime;              //!< creation date of file (local time)
        tTimeStamp          nTimeStamp;             //!< creation date as timestamp (seconds since 1.1.1970, UTC)
        tChar               strDateTime[32];        //!< creation date as string
        tChar               strShortDesc[128];      //!< short description of file
        tChar               strLongDesc[4096];      //!< long description of file
        tChar               strFilename[256];       //!< filename
        tTimeStamp          tsOffset;               //!< minimum of all data block times (data and file time)
        tTimeStamp          tsFirstDataBlock;       //!< minimum of all data block file times.

    } tADTFMediaDescriptor;

    /**
     * Information structure for storing details about the audio format.
     */
    typedef struct 
    {
        tWaveFormat        sWaveFormat;         //!< wave format
        tBool              bWaveFormatValid;    //!< true if sWaveFormat contains valid data, else false
    } tAudioFormat;                             //!< struct containing data for video streams

    /**
     * Information structure for storing details about the video format.
     */
    typedef struct
    {
        /// Enumeration for pixel format configuration
        enum ePixelFormat
        {
            PF_UNKNOWN                  = 0,     //<! unknown / not set
            PF_8BIT                     = 10,    //<! 8 bit
            PF_GREYSCALE_8              = 11,    //<! 8 bit greyscale - default format for 8 bpp
            PF_RGB_8                    = 12,    //<! 8 bit RGB (R: 3 bit, G: 3 bit, B: 2 Bit) - palletized 8 bpp format
            PF_16BIT                    = 20,    //<! 16 bit
            PF_GREYSCALE_10             = 21,    //<! 16 bit greyscale
            PF_GREYSCALE_12             = 22,    //<! 16 bit greyscale
            PF_GREYSCALE_14             = 23,    //<! 16 bit greyscale
            PF_GREYSCALE_16             = 24,    //<! 16 bit greyscale
            PF_RGB_444                  = 25,    //<! 12 bit RGB (R: 4 bit, G: 4 bit, B: 4 bit)
            PF_RGB_555                  = 26,    //<! 15 bit RGB (R: 5 bit, G: 5 bit, B: 5 bit) - default format for 16 bpp
            PF_RGB_565                  = 27,    //<! 16 bit RGB (R: 5 bit, G: 6 bit, B: 5 bit)
            PF_RGBA_4444                = 28,    //<! 16 bit RGBA (R: 4 bit, G: 4 bit, B: 4 bit, A: 4 bit) with alpha value
            PF_ABGR_4444                = 29,    //<! 16 bit ABGR (A: 4 bit, B: 4 bit, G: 4 bit, R: 4 bit) with alpha value - inverted RGBA
            PF_RIII_10                  = 30,
            PF_RIII_12                  = 31,
            PF_RIII_14                  = 32,
            PF_RIII_16                  = 33,
            PF_24BIT                    = 40,    //<! 24 bit
            PF_GREYSCALE_18             = 41,    //<! 24 bit greyscale
            PF_GREYSCALE_20             = 42,    //<! 24 bit greyscale
            PF_GREYSCALE_22             = 43,    //<! 24 bit greyscale
            PF_GREYSCALE_24             = 44,    //<! 24 bit greyscale
            PF_RGB_888                  = 45,    //<! 24 bit RGB (R: 8 bit, G: 8 bit, B: 8 bit) - default format for 24 bpp
            PF_BGR_888                  = 46,    //<! 24 bit BGR (B: 8 bit, G: 8 bit, R: 8 bit) - inverted RGB
            PF_32BIT                    = 50,    //<! 32 bit
            PF_ARGB_8888                = 51,    //<! 32 bit ARGB (A: 8 bit, R: 8 bit, G: 8 bit, B: 8 bit) with alpha value
            PF_ABGR_8888                = 52,    //<! 32 bit ABGR (A: 8 bit, B: 8 bit, G: 8 bit, R: 8 bit) with alpha value - inverted RGBA
            PF_RGBA_8888                = 53,    //<! 32 bit RGBA (R: 8 bit, G: 8 bit, B: 8 bit, A: 8 bit) with alpha value - default format for 32 bpp
            PF_BGRA_8888                = 54,    //<! 16 bit BGRA (B: 8 bit, G: 8 bit, R: 8 bit, A: 8 bit) with alpha value - inverted RGB + A
            PF_GREYSCALE_32             = 55,    //<! 32 bit greyscale
            PF_GREYSCALE_FLOAT32        = 56,    //<! 32 bit greyscale float value
            PF_YUV420P_888              = 60,    //<! 24 bit YUV (Y: 8 bit, U: 8 bit, V: 8 bit) - Y = luminance, U+V = chrominance
            PF_CUSTOM                   = 1000   //<! custom

            // Attention:
            // When you change this enumeration, please change also the corresponding enumeration in the adtf utils (image_intf.h)!!!
            // Thank you.
        } ;

        tColor             sColor;                  //!< color data if palletized image data
        tBitmapFormat      sBitmapFormat;           //!< width, height, bits per pixel etc.
        tBool              bColorValid;             //!< true if sColor contains valid data, else false
        tBool              bBitmapFormatValid;      //!< true if sBitmapFormat contains valid data, else false
    } tVideoFormat;

    /**
     * Codec information structure used to store codec specific configuration data
     */
    struct tCompressedVideoFormat
    {
        tBool           bValid;             //!< true if data is valid
        tUInt32         nSize;              //!< Size of pCodecData
        tUInt32         nVersion;           //!< Version information (see defines above)
        tBitmapFormat   sBitmapFormat;      //!< Format of original video
        tChar           strCodec[128];      //!< either a FOURCC or codecgroup:codec (i.e. vfw:DIVX)
        tVoid*          pCodecData;         //!< codec specific configuration data
    };

    /**
     *  This struct contains information about a specific stream
     */
    typedef struct tagADTFStreamDescriptor
    {
        /// Enumeration for stream types
        enum eType 
        {
            SD_AUDIO,
            SD_CAN,
            SD_COMMAND,
            SD_INFO,
            SD_FLEXRAY,
            SD_NETWORK_DATA,
            SD_OTHER,
            SD_REFERENCE,
            SD_STRUCTURED_DATA,
            SD_USER,
            SD_VIDEO,
            SD_NOT_INIT,
            SD_COMPRESSED_VIDEO,
            SD_MEDIA_DESCRIPTION
        };
        tUInt32             nType;              //!< type of stream , only major type, see \link #eType \c eType \endlink
        tInt32              nMajorType;         //!< major type
        tInt32              nSubType;           //!< subtype
        tChar               strSampleType[512]; //!< sample type
        tChar               strMediaType[512];  //!< media type
        /// Video/Compressed video/Audio data union
        union {
            tAudioFormat           sAudioFormat;            //!< struct containing data for audio streams
            tVideoFormat           sVideoFormat;            //!< struct containing data for video streams
            tCompressedVideoFormat sCompressedVideoFormat;  //!< struct containing data for compressed video streams
        }uData ;    //!< union containing data of all built-in media types (audio and video

    } tADTFStreamDescriptor;
    
/**
 *  The CAN data structure is used by CAN MediaSamples of type OID_ADTF_MEDIA_SAMPLE_CAN.
 */
struct tADTFCANData
{
    /**
     *  This enum specifies the different kinds of messages
     *  that may be contained in the union
     */
    enum eMessageTag
    {
        MT_Data = 0,        //!< Data
        MT_Status = 1,      //!< Status
    };

    /**
     *  CAN message flags
     */
    enum eDataFlags
    {
        DF_NONE = 0,            //!< Standard flags
        DF_ERROR_FRAME = 1,     //!< Indicates an error frame
        DF_REMOTE_FRAME = 2,    //!< Indicates a remote frame
        DF_TX_COMPLETED = 4     //!< Notification for successful message transmission
    };

    /**
     *  CAN bus state flags. 
     */
    enum eBusStatus
    {
        BS_OFFLINE = 1,         //!< Bus is offline
        BS_ERROR_PASSIVE = 2,   //!< One of the error counters has reached the error level.
        BS_ERROR_WARNING = 4,   //!< One of the error counters has reached the warning level.
        BS_ERROR_ACTIVE = 8     //!< Bus is online
    };

    /**
     *  CAN message id masks
     */
    enum eMsgId
    {
        MSG_IDMASK_BASE  = 0x000007FF,      //!< Message IDs for base frame format use 11 bit identifiers
        MSG_IDMASK_EXTENDED  = 0x1FFFFFFF,  //!< Message IDs for extended frame format use 29 bit identifiers
        MSG_EXTENDED_FLAG = 0x80000000      //!< Extended CAN messages are marked by bit 31
    };

    /**
     *  CAN message header structure
     */
    struct tMessageHeader
    {
        tUInt8      ui8Tag;             //!< Type of contained message (see eMessageTag)
        tUInt8      ui8Channel;         //!< Channel that received this message
        tTimeStamp  tmTimeStamp;        //!< Hardware timestamp in micro seconds 
    };

    /**
     *  CAN message data structure
     */
    struct tData
    {
        tUInt32     ui32Id;             //!< id of can message. For extended CAN messages bit 31 is set.
        tUInt8      ui8Length;          //!< length of data [0..8]
        tUInt8      ui8Reserved;        //!< reserved, should be zero
        tUInt16     ui16Flags;          //!< Flags @see eDataFlags
        tUInt16     ui16Reserved;       //!< reserved, should be zero
        tUInt32     ui32Reserved;       //!< reserved, should be zero
        tUInt8      aui8Data[8];        //!< data field
    };

    /**
     *  CAN bus status structure
     */
    struct tStatus
    {
        tUInt32     ui32BitRate;        //!< CAN bus bitrate
        tUInt32     ui32RxBitCount;     //!< Count of received bits
        tUInt32     ui32TxBitCount;     //!< Count of transmitted bits
        tUInt16     ui16RxErrorCounter; //!< Error counter for the receive section of the CAN controller.
        tUInt16     ui16TxErrorCounter; //!< Error counter for the transmit section of the CAN controller.
        tUInt8      ui8BusStatus;       //!< Flags @see eBusStatus
        tUInt8      ui8Reserved;        //!< reserved, should be zero
        tUInt32     ui32Reserved;       //!< reserved, should be zero
    };

    tMessageHeader  sHeader;            //!< CAN message header structure

    union
    {
        tData   sData;                  //!< used when ui8Tag == MT_Data
        tStatus sStatus;                //!< used when ui8Tag == MT_Status
    };
};


 /**
 *  Available log levels
 */
typedef enum
{
    ///Log level None  (This log level can only be reached if you set that twice)!
    eLOG_LVL_NONE      = 0,
    ///Log level Exceptions
    eLOG_LVL_EXCEPTION = 1,
    ///Log level Error (only error and exception will be logged)
    eLOG_LVL_ERROR     = 2,
    ///Log level Warning (only Warning, error and exception will be logged)
    eLOG_LVL_WARNING   = 3,
    ///Log level Info (only info Warning, error and exception will be logged)
    eLOG_LVL_INFO      = 4,
    ///Log level Dump (only in debug dump message will be logged)
    eLOG_LVL_DUMP      = 5
} tLogLevel;

///Available open modes for file writing
typedef enum
{
    ///No flag set
    OM_None      = 0x00,
    ///Disable asynchronous internal cache, any write operation results in a direct file operation
    OM_SyncWrite = 0x02
} tOpenMode;

#pragma pack(pop)
} //namespace adtfstreaming

#endif //_ADTF_STREAMINGLIB_TYPES_HEADER_
