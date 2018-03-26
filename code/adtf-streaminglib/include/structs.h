/**
 *
 * Standard type declarations.
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved.
 *
 * $Author: belkera $
 * $Date: 2011-07-01 08:32:18 +0200 (Fri, 01 Jul 2011) $
 * $Revision: 7973 $
 * @remarks
 *
 */

#ifndef  BASE_STRUCTS_H
#define  BASE_STRUCTS_H

#ifdef _MSC_VER
    #pragma pack(push)
    #pragma pack(1)
#else
    #pragma pack(push)
    #pragma pack(1)
#endif

    /**
     *
     * Date Structure to define dates.
     *
     */
    typedef struct tag_Date
    {
        /// The Year
        tUInt16 ui16Year;
        /// The Month
        tUInt16 ui16Month;
        /// The Day
        tUInt16 ui16Day;
    } tDate;

    /**
     *
     * Time Structure to define Time.
     *
     */
    typedef struct tag_Time
    {
        /// The Hour
        tUInt16 ui16Hour;
        /// The Minute
        tUInt16 ui16Minute;
        /// The Second
        tUInt16 ui16Second;
        /// The Microsecond
        tUInt32 ui32Microseconds;
    } tTime;

    /**
     *
     * Struct to handle date and time together.
     *
     */
    typedef struct tag_DateTime
    {
		/// The Year
        tUInt16 ui16Year;
		/// The Month
        tUInt16 ui16Month;  
		/// The Day
        tUInt16 ui16Day;     
		/// The Hour
        tUInt16 ui16Hour; 
		/// The Minute
        tUInt16 ui16Minute;   
		/// The Second
        tUInt16 ui16Second;  
		/// The Microsenconds
        tUInt32 ui32Microseconds;
    } tDateTime;

    /**
     *
     * Struct to specifie a bitmap.
     *
     */
    typedef struct tag_BitmapFormat
    {
        /// Specifies the width (in pixels) of the image
        tInt32        nWidth;
        /// Specifies the width (in pixels) of the image
        tInt32        nHeight;
        /// Specifies the number of bits used to represent the color of a single pixel
        tInt16        nBitsPerPixel;
		/// Pixel format specified in tPixelFormat. More information ca be found at @see adtf::cImage::tPixelFormat
        tInt16        nPixelFormat;
        /// Specifies the number of bytes used per line
        tInt32        nBytesPerLine;
        /// Size of bitmap
        tInt32        nSize;
        /// Size of colour palette
        tInt32        nPaletteSize;
    } tBitmapFormat;

    /**
     *
     * The Common Wave format describes a sound Smaple stream. 
     *
     */
    typedef struct tag_WaveFormat
    {
        /// Format type
        tInt32        nFormatType;
        /// Number of channels (1=mono, 2=stereo)
        tInt32        nChannels;
        /// Samples per second
        tInt32        nSamplesPerSec;
        /// Bits per sample
        tInt32        nBitsPerSample;
        /// Number of samples
        tInt32        nNumSamples;
        /// Size of samples
        tInt32        nSize;
    } tWaveFormat;

    /**
     *
     * Struct to split the color in the categories: blue, green, red an alpha value.
     *
     */
    typedef struct tag_Color
    {
		/// The Blue value
        tUInt8          nBlue;
		/// The Green value
        tUInt8          nGreen;
		/// The Red value
        tUInt8          nRed;       
		/// The Alpha value
        tUInt8          nAlpha;  
    } tColor;

    /**
     *
     * Struct to get informations about a bitmap.
     *
     */
    typedef struct tag_Win32BitmapInfoHeader
    {
		/// Specifies the number of bytes required by the structure
        tUInt32         nSize;              
		/// Specifies the width of the bitmap, in pixels
        tInt32          nWidth; 
		/// Specifies the height of the bitmap, in pixels
        tInt32          nHeight;
		/// Specifies the number of planes for the target device. This value must be set to 1
        tUInt16         nPlanes;   
		/// Specifies the number of bits in a bitmap
        tUInt16         nBitCount;          
		/// Specifies the compression rate of the bitmap
        tUInt32         nCompression;       
		/// Specifies the size, in bytes, of the image. This may be set to zero for uncompressed bitmaps
        tUInt32         nSizeImage;         
		/// Specifies the horizontal resolution, in pixels-per-meter, of the target device for the bitmap. An application can use this value to select a bitmap from a resource group that best matches the characteristics of the current device 
        tInt32          nXPelsPerMeter;     
		/// Specifies the vertical resolution, in pixels-per-meter, of the target device for the bitmap
        tInt32          nYPelsPerMeter;      
		/// Specifies the number of color indexes in the color table that are actually used by the bitmap. If this value is zero, the bitmap uses the maximum number of colors corresponding to the value of the nBitCount member for the compression mode specified by nCompression
        tUInt32         nClrUsed;         
		/// Specifies the number of color indexes that are required for displaying the bitmap. If this value is zero, all colors are required
        tUInt32         nClrImportant;      
    } tWin32BitmapInfoHeader;

    /**
     *
     * Struct to get the informations and the color about a bitmap.
     *
     */
    typedef struct tag_Win32BitmapInfo
    {
		/// @ref tWin32BitmapInfoHeader
        tWin32BitmapInfoHeader      sHeader; 
		/// @ref tColor
        tColor                      aColors[1]; 
    } tWin32BitmapInfo;

#ifdef _MSC_VER
    #pragma pack(pop)
#else
    #pragma pack(pop)
#endif

#endif /* BASE_STRUCTS_H */
