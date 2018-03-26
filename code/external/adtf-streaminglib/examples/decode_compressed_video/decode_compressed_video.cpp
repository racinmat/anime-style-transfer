/**
 *
 * ADTF Streaming Library compressed video example
 *
 * The streaming library does not perform any kind of decompression of compressed video streams.
 * This example shows how to handle decompression seperatly for all currently available codecs
 * in the video compression toolbox.
 *
 * We will open an ADTF DAT File, find a compressed video stream, iterate over all blocks and
 * decompress the video data while doing so. The code for the actual decompression can be found
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

// #ifdef _MSC_VER
#pragma pack(push)
#pragma pack(1)
// #endif

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

// #ifdef _MSC_VER
#pragma pack(pop)
// #endif




tInt CreateCompressedStreamDatFileCopy(const tADTFStreamDescriptor* pStreamDescriptor,
                                       tUInt16& nStreamId,
                                       string& strStream,
                                       IADTFFileReader* pFileReader,
                                       string& strNewFile)
{
    //we only want to export the compressed video to a new dat file
    IADTFFileWriter* pWriter = IADTFFileWriter::Create();


    if (IS_FAILED(pWriter->Open(strNewFile.c_str())))
    {
        cerr << "unable to initialize dat file writer" << endl;
        IADTFFileWriter::Release(pWriter);
        pWriter = NULL;
        IADTFFileReader::Release(pFileReader);
        return 1;
    }
    tUInt16 ui16NewStreamId = 0;
    if (IS_FAILED(pWriter->CreateStream(strStream.c_str(),
                                        ui16NewStreamId,
                                        pStreamDescriptor->nMajorType, 
                                        pStreamDescriptor->nSubType,
                                        pStreamDescriptor->strMediaType,
                                        pStreamDescriptor->strSampleType,
                                        (tVoid*)&pStreamDescriptor->uData)))
    {
        cerr << "unable to initialize dat file writer with given codec" << endl;
        pWriter->Close();
        IADTFFileWriter::Release(pWriter);
        pWriter = NULL;
        IADTFFileReader::Release(pFileReader);
        return 1;
    }
    cADTFDataBlock *pDataBlock = NULL;
    while (IS_OK(pFileReader->Read(&pDataBlock)))
    {
        // we are only interested in blocks from our stream
        if (pDataBlock->GetStreamId() == nStreamId)
        {
            pDataBlock->SetStreamId(ui16NewStreamId);
            if (IS_FAILED(pWriter->Write(pDataBlock)))
            {
                //something went wrong but we go ahead
            }
        }
    }
    pWriter->Close();
    IADTFFileWriter::Release(pWriter);
    pWriter = NULL;
    return 0;
}


tInt CreateVideo(const tADTFStreamDescriptor* pStreamDescriptor,
                 tUInt16& nStreamId,
                 IADTFFileReader* pFileReader,
                 string& strNewFile)
{
    string strCodec = pStreamDescriptor->uData.sCompressedVideoFormat.strCodec;
    cout << "found codec '" << strCodec << "'" << endl;

    IStreamingCodec* pCodec = NULL;
    if (strCodec == "adtf")
    {
        pCodec = new cCodecADTF;
    }
    else if (strCodec == "rice")
    {
        pCodec = new cCodecRICE();
    }
    else
    {
#ifdef WIN32
        // try vfw codec
        pCodec = new cCodecVfw;
#else
    #ifdef FFMPEG_AVAILABLE
        // try ffmpeg
        pCodec = new cCodecFFmpeg;
    #endif
#endif
    }

    if (!pCodec)
    {
        cerr << "no codec implementation found" << endl;
        return 1;
    }

    // initialize the codec
    tBitmapFormat sOutputFormat;
    if (IS_FAILED(pCodec->Init(&pStreamDescriptor->uData.sCompressedVideoFormat, &sOutputFormat)))
    {
        cerr << "unable to initialize codec" << endl;
        delete pCodec;
        return 1;
    }

    tBool bConvert12to8Bit = tFalse;

    // 8-Bit images needs greyscale palette
    if (sOutputFormat.nBitsPerPixel == 8)
    {
        sOutputFormat.nPaletteSize = 256;
    }
    else if (sOutputFormat.nBitsPerPixel == 16  &&  sOutputFormat.nPaletteSize == 4096)
    {
        sOutputFormat.nPaletteSize = 256;
        sOutputFormat.nBitsPerPixel = 8;
        bConvert12to8Bit = tTrue;
    }

    // create a temporary output bitmap
    tInt nOutputSize = sOutputFormat.nSize;
    tUInt8* pOutputData = new tUInt8[nOutputSize];

    tInt    nBitmapInfoSize = sizeof(tWin32BitmapInfo) + sOutputFormat.nPaletteSize*sizeof(tColor);
    tUInt8* pBitmapInfo = new tUInt8[nBitmapInfoSize];

    tWin32BitmapInfo& sBitmapInfo = *(tWin32BitmapInfo*)pBitmapInfo;
    sBitmapInfo.sHeader.nSize           = sizeof(tWin32BitmapInfoHeader);
    sBitmapInfo.sHeader.nWidth          = sOutputFormat.nWidth;
    sBitmapInfo.sHeader.nHeight         = -sOutputFormat.nHeight;
    sBitmapInfo.sHeader.nPlanes         = 1;
    sBitmapInfo.sHeader.nBitCount       = (tUInt8) sOutputFormat.nBitsPerPixel;
    sBitmapInfo.sHeader.nCompression    = 0;
    sBitmapInfo.sHeader.nSizeImage      = ((sOutputFormat.nWidth + 3) & (-4))
        * sOutputFormat.nHeight 
        * sOutputFormat.nBitsPerPixel/8;
    sBitmapInfo.sHeader.nXPelsPerMeter  = 0;
    sBitmapInfo.sHeader.nYPelsPerMeter  = 0;
    sBitmapInfo.sHeader.nClrUsed        = sOutputFormat.nPaletteSize;
    sBitmapInfo.sHeader.nClrImportant   = 0;
    for (tInt nIdx = 0;  nIdx < sOutputFormat.nPaletteSize;  ++nIdx)
    {
        sBitmapInfo.aColors[nIdx].nRed = nIdx;
        sBitmapInfo.aColors[nIdx].nGreen = nIdx;
        sBitmapInfo.aColors[nIdx].nBlue = nIdx;
        sBitmapInfo.aColors[nIdx].nAlpha = 0xff;
    }

    tBitmapFileHeader sFileHeader;
    sFileHeader.ui16Type        = 0x4D42;
    sFileHeader.ui32Size        = sizeof(tBitmapFileHeader) + nBitmapInfoSize + sBitmapInfo.sHeader.nSizeImage;
    sFileHeader.ui16Reserved1   = 0;
    sFileHeader.ui16Reserved2   = 0;
    sFileHeader.ui32DataOffset  = sizeof(tBitmapFileHeader) + nBitmapInfoSize;

    // now iterate over all data blocks
    tInt nFrame = -1;
    cADTFDataBlock *pDataBlock = NULL;
    while (IS_OK(pFileReader->Read(&pDataBlock)))
    {
        // we are only interested in blocks from our stream
        if (pDataBlock->GetStreamId() == nStreamId)
        {
            ++nFrame;

            // access the block data
            const tVoid* pData;
            tUInt64 nSize = pDataBlock->GetData(&pData);

            // and let the codec decompress it
            if (IS_FAILED(pCodec->Decompress(pData, nSize, pOutputData, nOutputSize)))
            {
                cerr << "decompression of frame " << nFrame << " failed" << endl;
                continue;
                // we do not exit because the codec might be able to recover on later (key)frames.
            }

            if (bConvert12to8Bit)
            {
                for (tInt nY = 0;  nY < sOutputFormat.nHeight;  nY++)
                {
                    tUInt8* pui8Out = pOutputData + ((sOutputFormat.nWidth + 3) & (-4)) * nY;
                    tUInt16* pui16In = ((tUInt16*)pOutputData) + sBitmapInfo.sHeader.nWidth * nY;

                    for (tInt nX = 0;  nX < sOutputFormat.nWidth;  nX++)
                    {
                        pui8Out[nX] = pui16In[nX] >> 4;
                    }
                }
            }

            // find a new filename
            ostringstream strFilename;
            strFilename << strNewFile << "_" << nFrame << ".bmp";
            cout << "writing " << strFilename.str() << endl;

            // and dump the bitmap to the file
            ofstream oFile;
            oFile.open(strFilename.str().c_str(), ofstream::binary);
            oFile.write(reinterpret_cast<tChar*>(&sFileHeader), sizeof(sFileHeader));
            oFile.write(reinterpret_cast<tChar*>(pBitmapInfo), nBitmapInfoSize);
            oFile.write(reinterpret_cast<tChar*>(pOutputData), sBitmapInfo.sHeader.nSizeImage);
            oFile.close();
        }
    }

    // cleanup
    delete [] pBitmapInfo;
    delete [] pOutputData;
    delete pCodec;
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc < 4 || argv[1] == NULL)
    {
        cerr << "usage: " << argv[0] << " FILENAME STREAMNAME OUTPUTNAME"  << endl;
        return 1;
    }

    string strDatFile = argv[1];
    string strStream = argv[2];
    string strOutput = argv[3];

    // open the dat file
    IADTFFileReader* pFileReader = IADTFFileReader::Create();
    if (IS_FAILED(pFileReader->Open(strDatFile.c_str())))
    {
        cerr << "unable to open file " << strDatFile << endl;
        IADTFFileReader::Release(pFileReader);
        return 1;
    }

    // find our stream
    tUInt16 nStreamId;
    if (IS_FAILED(pFileReader->GetStreamId(strStream.c_str(), nStreamId)))
    {
        cerr << "unable to file stream '" << strStream << "'" << endl;
        IADTFFileReader::Release(pFileReader);
        return 1;
    }

    // get the stream description
    const tADTFStreamDescriptor* pStreamDescriptor = pFileReader->GetStreamDescriptor(nStreamId);
    if (!pStreamDescriptor)
    {
        cerr << "unable to get stream info" << endl;
        IADTFFileReader::Release(pFileReader);
        return 1;
    }

    // and check if it really is a compressed stream
    if (pStreamDescriptor->nType != tADTFStreamDescriptor::SD_COMPRESSED_VIDEO)
    {
        cerr << "stream '" << argv[2] << "' is no compressed video stream" << endl;
        IADTFFileReader::Release(pFileReader);
        return 1;
    }

    // check if format info is avaliable (media type errors)
    if (!pStreamDescriptor->uData.sCompressedVideoFormat.bValid)
    {
        cerr << "stream '" << argv[2] << "' has no valid codec information" << endl;
        IADTFFileReader::Release(pFileReader);
        return 1;
    }

    

    // now handle the different codecs
    tInt nError = 0;
    if ((strOutput.length() - 4) == strOutput.rfind(string(".dat"), strOutput.length() - 4))
    {
        nError = CreateCompressedStreamDatFileCopy(pStreamDescriptor, nStreamId, strStream, pFileReader, strOutput);
    }
    else
    {
        nError = CreateVideo(pStreamDescriptor, nStreamId, pFileReader, strOutput);
    }

    IADTFFileReader::Release(pFileReader);
    pFileReader = NULL;

    if (nError != 0)
    {
        cout << "finished with error" << endl;
    }
    else
    {
        cout << "finished" << endl;
    }
    
    return nError;
}

