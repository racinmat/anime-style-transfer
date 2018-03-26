/**
 *
 * ADTF File Access example
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

#include "../stdafx.h"

union tFOURCC 
{
    tUInt32 nValue;                     //< FOURCC of codec as integer
    tChar   strValue[4];                //< FOURCC of codec as string
};

cCodecVfw::cCodecVfw(const tChar* strCodec):
    m_strCodec(strCodec),
    m_hIC(NULL),
    m_pBitmapInfoOut(NULL),
    m_nFrame(0)
{
}

cCodecVfw::~cCodecVfw()
{
    if (m_hIC)
    {
        ICCompressEnd(m_hIC);
        ICClose(m_hIC);
    }

    delete [] reinterpret_cast<tUInt8*>(m_pCodecInfo);
}

tResult cCodecVfw::Init(const tBitmapFormat* psInputFormat, tVoid** ppCodecInfo, tUInt32* pCodecInfoSize, tInt* pBufferSize)
{
    RETURN_IF_POINTER_NULL(psInputFormat);

    
    if (m_strCodec.size() < 8)
    {
        cerr << "Invalid codec id: " << m_strCodec << endl;
        RETURN_ERROR(ERR_NOT_SUPPORTED);
    }

    tFOURCC uFourCC;
    uFourCC.strValue[0] = m_strCodec.c_str()[4];
    uFourCC.strValue[1] = m_strCodec.c_str()[5];
    uFourCC.strValue[2] = m_strCodec.c_str()[6];
    uFourCC.strValue[3] = m_strCodec.c_str()[7];

    // search for it
    HIC     hIC = 0;
    for (DWORD i = 0;  ICInfo(ICTYPE_VIDEO, i, &m_IcInfo);  ++i)
    {
        // opens a given codec for test
        hIC = ICOpen(m_IcInfo.fccType, m_IcInfo.fccHandler, ICMODE_QUERY);
        if (hIC)
        {
            // Find out the compressor name.
            ICGetInfo(hIC, &m_IcInfo, sizeof(m_IcInfo));

            if (m_IcInfo.fccType == ICTYPE_VIDEO &&
                (uFourCC.nValue == m_IcInfo.fccHandler))
            {
                ICClose(hIC);
                break;
            }
        }

        ICClose(hIC);
        hIC = 0;
    }

    if (!hIC)
    {
        cerr << "Codec '" << m_strCodec << "' not supported!" << endl;
        RETURN_ERROR(ERR_NOT_SUPPORTED);
    }

    // Test input format
    if (psInputFormat->nBitsPerPixel != 24)
    {
        cerr << "Bits per pixel not supported!" << endl;
        RETURN_ERROR(ERR_NOT_SUPPORTED);
    }

    tInt nBytesPerLine = (psInputFormat->nWidth * 24 / 8 + 3) & (-4);

    BITMAPINFO  sBitmapIn;
    sBitmapIn.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
    sBitmapIn.bmiHeader.biWidth         = psInputFormat->nWidth;
    sBitmapIn.bmiHeader.biHeight        = psInputFormat->nHeight;
    sBitmapIn.bmiHeader.biPlanes        = 1;
    sBitmapIn.bmiHeader.biBitCount      = 24;
    sBitmapIn.bmiHeader.biCompression   = 0;
    sBitmapIn.bmiHeader.biSizeImage     = nBytesPerLine * psInputFormat->nHeight;
    sBitmapIn.bmiHeader.biXPelsPerMeter = 0;
    sBitmapIn.bmiHeader.biYPelsPerMeter = 0;
    sBitmapIn.bmiHeader.biClrUsed       = 0;
    sBitmapIn.bmiHeader.biClrImportant  = 0;


    memcpy(&m_oBitmapInfoIn, &sBitmapIn, sizeof(BITMAPINFO));

    try
    {
        // opens a given codec for compression
        m_hIC = ICOpen(m_IcInfo.fccType, m_IcInfo.fccHandler, ICMODE_COMPRESS);
        if (m_hIC == NULL)
        {
            RETURN_ERROR(ERR_UNEXPECTED);
        }

        DWORD dwFormatSize = ICCompressGetFormatSize(m_hIC, &m_oBitmapInfoIn);
        tInt nCodecInfoSize = sizeof(tVfwCodecInfo) - sizeof(BITMAPINFO) + dwFormatSize;

        // Init codec V2 information
        m_pCodecInfo = reinterpret_cast<tVfwCodecInfo*>(new tUInt8[nCodecInfoSize]);

        *ppCodecInfo = m_pCodecInfo;
        *pCodecInfoSize = nCodecInfoSize;

        memcpy(&m_pCodecInfo->sBitmapFormat, psInputFormat, sizeof(tBitmapFormat));

        m_pBitmapInfoOut = &m_pCodecInfo->sBitmapFormat;

        if (ICERR_OK != ICCompressGetFormat(m_hIC, &m_oBitmapInfoIn, m_pBitmapInfoOut))
        {
            RETURN_ERROR(ERR_UNEXPECTED);
        }

        // Here you could set your compression settings.
        //{
        //    tInt nError = ICSetState(m_hIC, m_oCodecConfigData.GetPtr(), m_oCodecConfigData.GetSize());
        //    if (ICERR_OK != nError && m_oCodecConfigData.GetSize() != nError)
        //    {
        //        RETURN_ERROR(ERR_UNEXPECTED);
        //    }
        //}

        // make the buffer for compressed image
        // Find the worst-case buffer size.
        *pBufferSize = m_nBufferSize = ICCompressGetSize(m_hIC, &m_oBitmapInfoIn, m_pBitmapInfoOut);

        if (ICERR_OK != ICCompressBegin(m_hIC, &m_oBitmapInfoIn, m_pBitmapInfoOut))
        {
            cerr << "Error while initializing compression with codec '" << m_strCodec << "'." << endl;
            RETURN_ERROR(ERR_UNEXPECTED);
        }
    }
    catch (...)
    {
        cerr << "An exception occured while initializing the codec '" << m_strCodec << "'." << endl;
        RETURN_ERROR(ERR_UNEXPECTED);
    }
    RETURN_NOERROR;
}

tResult cCodecVfw::Compress(const tVoid* pSrc, tInt nSize, tVoid* pDst, tInt* pnCompressedSize, tBool* pbKeyFrame)
{
    DWORD nChunkId = 0;
    DWORD nFlagsOut = 0;

    m_pBitmapInfoOut->bmiHeader.biSizeImage = *pnCompressedSize;

    DWORD err = ICCompress(m_hIC,
                           0,
                           &m_pBitmapInfoOut->bmiHeader,
                           pDst,
                           &m_oBitmapInfoIn.bmiHeader,
                           (LPVOID)pSrc,
                           &nChunkId,
                           &nFlagsOut,
                           m_nFrame,
                           m_nFrame ? 0 : 0xFFFFFF,
                           0,
                           NULL,
                           NULL);
    
    // Handle compressor error.
    if (err != ICERR_OK)
    {
        cerr << "error while encoding frame" << endl;
        RETURN_ERROR(ERR_FAILED);
    }

    *pnCompressedSize = m_pBitmapInfoOut->bmiHeader.biSizeImage;
    *pbKeyFrame = (nFlagsOut == AVIIF_KEYFRAME);

    ++m_nFrame;

    RETURN_NOERROR;
}