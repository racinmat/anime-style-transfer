/**
 *
 * ADTF Streaming Library decompressed video example
 *
 * The streaming library does not perform any kind of compression of decompressed video streams.
 * This example shows how to handle compression seperatly for all currently available codecs
 * in the video compression toolbox.
 *
 * We will open an ADTF DAT File, find a video stream, iterate over all blocks and
 * compress the video data while doing so. The code for the actual compression can be found
 * in the codec_adtf, codec_vfw and codec_ffmpeg subdirectories.
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: VG8D3AW $
 * $Date: 2010-04-16 10:06:05 +0200 (Fr, 16 Apr 2010) $
 * $Revision: 3303 $
 *
 * @remarks
 *
 */

#include "stdafx.h"

#pragma pack(push)
#pragma pack(1)

/** 
 *  This structure defines the file header used to load and save a bitmap.
 */
typedef struct 
{
    tUInt16         ui16Type;       //!< always 0x4D42, for more information have a look at http://msdn.microsoft.com/en-us/library/AA930979.ASPX
    tUInt32         ui32Size;       //!< actual size of data + this header + windows bitmap header + palette
    tUInt16         ui16Reserved1;  //!< Reserved
    tUInt16         ui16Reserved2;  //!< Reserved
    tUInt32         ui32DataOffset; //!< offset from header
} tBitmapFileHeader;

#pragma pack(pop)

tUInt8* ReadBitmap(const tChar* strFilename, tWin32BitmapInfoHeader* pInfo)
{
    ifstream oFile;
    oFile.open(strFilename, ofstream::binary);

    tBitmapFileHeader sHeader;
    oFile.read(reinterpret_cast<tChar*>(&sHeader), sizeof(tBitmapFileHeader));

    if (sHeader.ui16Type != 0x4d42)
    {
        throw "File is not a valid bmp";
    }

    oFile.read(reinterpret_cast<tChar*>(pInfo), sizeof(tWin32BitmapInfoHeader));

    oFile.seekg(sHeader.ui32DataOffset);

    tUInt32 nDataSize = sHeader.ui32Size - sHeader.ui32DataOffset;
    tUInt8* pData = new tUInt8[nDataSize];
    oFile.read(reinterpret_cast<tChar*>(pData), nDataSize);
    oFile.close();

    return pData;
}

#define BMP_START_INDEX 5

int main(int argc, char* argv[])
{
    if (argc < 6 || argv[1] == NULL)
    {
        cerr << "usage: " << argv[0] << " STREAMNAME OUTPUTNAME FPS CODEC BMP1 [BMP2 ...]"  << endl;
        return 1;
    }
    
    string strStream = argv[1];
    string strOutput = argv[2];
    int nFps = atoi(argv[3]);
    string strCodec = argv[4];

    // use information from the first file to initialize our stream
    tWin32BitmapInfoHeader sInitInfo;
    tUInt8* pBitmap = ReadBitmap(argv[BMP_START_INDEX], &sInitInfo);
    delete [] pBitmap;
    cout << "Resolution: " << sInitInfo.nWidth << "x" << sInitInfo.nHeight << endl;
    cout << "Color Depth: " << sInitInfo.nBitCount << endl;

    if (sInitInfo.nBitCount != 24)
    {
        cerr << "Only 24 Bit RGB bitmaps are supported." << endl;
        return 1;
    }
    
    tInt nSizeInBytes = sInitInfo.nWidth * sInitInfo.nHeight * sInitInfo.nBitCount / 8;

    std::auto_ptr<IStreamingCodec> pCodec;
    if (strCodec == "adtf")
    {
        pCodec.reset(new cCodecADTF);
    }
#ifndef WIN32
    #ifdef FFMPEG_AVAILABLE
        else if(strCodec.find("ffmpeg") == 0)
        {
            pCodec.reset(new cCodecFFmpeg(strCodec.c_str()));
        }
    #endif
#else
    else if(strCodec.find("vfw") == 0)
    {
        pCodec.reset(new cCodecVfw(strCodec.c_str()));
    }
#endif

    if (!pCodec.get())
    {
        cerr << "unsupported codec: " << strCodec << endl;
        return 1;
    }

    tCompressedVideoFormat sCompressionFormat;
    sCompressionFormat.sBitmapFormat.nSize = nSizeInBytes;
    sCompressionFormat.sBitmapFormat.nWidth = sInitInfo.nWidth;
    sCompressionFormat.sBitmapFormat.nHeight = sInitInfo.nHeight;
    sCompressionFormat.sBitmapFormat.nBitsPerPixel = sInitInfo.nBitCount;
    sCompressionFormat.sBitmapFormat.nPaletteSize = 0;
    sCompressionFormat.sBitmapFormat.nPixelFormat = tVideoFormat::PF_RGB_888;
    sCompressionFormat.sBitmapFormat.nBytesPerLine = sInitInfo.nWidth * 3;

    sCompressionFormat.nVersion = ADTF_CODEC_INFO_VERSION_2_0;
    memcpy(sCompressionFormat.strCodec, strCodec.c_str(), strCodec.length() + 1);
    
    tInt nBufferSize;
    if (IS_FAILED(pCodec->Init(&sCompressionFormat.sBitmapFormat, &sCompressionFormat.pCodecData, &sCompressionFormat.nSize, &nBufferSize)))
    {
        cerr << "Unable to initialize codec: " << strCodec << endl;
        return 1;
    }

    //create writer
    IADTFFileWriter* pFileWriter = IADTFFileWriter::Create();
    //open file
    if (IS_FAILED(pFileWriter->Open(strOutput.c_str())))
    {
        cerr << "FAILED to open file " << strOutput << endl;
        IADTFFileWriter::Release(pFileWriter);
        return 1;
    }

    tUInt16 nStreamID;
    if (IS_FAILED(pFileWriter->CreateStream(strStream.c_str(), nStreamID,
                                            MEDIA_TYPE_VIDEO , MEDIA_SUBTYPE_VIDEO_COMPRESSED,
                                            STREAMING_MEDIA_TYPE_VIDEO_COMPRESSED, "adtf.core.media_sample",
                                            &sCompressionFormat)))
    {
        cerr << "FAILED to create compressed stream" << endl;
        IADTFFileWriter::Release(pFileWriter);
        return 1;
    }

    tUInt8* pBuffer = new tUInt8[nBufferSize];
    
    for (tInt nFile = BMP_START_INDEX; nFile < argc; ++nFile)
    {
        const tChar* strFile = argv[nFile];
        cout << "processing " << strFile << " ... ";
        tWin32BitmapInfoHeader sInfo;
        pBitmap = ReadBitmap(strFile, &sInfo);
        tInt nCompressedSize = nBufferSize;
        tBool bKeyFrame = tFalse;
        
        if (IS_FAILED(pCodec->Compress(pBitmap, nSizeInBytes, pBuffer, &nCompressedSize, &bKeyFrame)))
        {
            cerr << "FAILED to encode image: " << strFile << endl;
            delete [] pBitmap;
            break;
        }
       
        delete [] pBitmap;
        
        if (IS_FAILED(pFileWriter->Write(nStreamID, pBuffer, nCompressedSize, (tTimeStamp) ((nFile - BMP_START_INDEX) * 1000000.0 / nFps))))
        {
            cerr << "FAILED to write compressed image to stream: " << strFile <<endl;
            break;
        }

        cout << "done" << endl;
    }

    delete [] pBuffer;

    if (IS_FAILED(pFileWriter->Close()))
    {
        cerr << "FAILED to close file" << endl;
    }

    IADTFFileWriter::Release(pFileWriter);    
    return 0;
}

