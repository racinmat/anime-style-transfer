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

#include "rice.h"
#include "prediction.h"

#pragma pack(push, 1)

// struct for the additional codec data of version 1 RICE
struct tRiceCodecInfo_ADTF1
{
    tInt32              nPredictionMethod;
    tWin32BitmapInfo    sBitmapInfo;
};

#pragma pack(pop)

cCodecADTF::cCodecADTF()
{
    m_pMemoryBuffer      = NULL;
    m_szMemoryBufferSize = 0;
}

cCodecADTF::~cCodecADTF()
{
    if (m_pMemoryBuffer)
    {
        delete [] m_pMemoryBuffer;
        m_pMemoryBuffer = NULL;
    }
    m_szMemoryBufferSize = 0;
}

//*************************************************************************************************
tResult cCodecADTF::Init(const tBitmapFormat* psInputFormat, tVoid** ppCodecInfo, tUInt32* pCodecInfoSize, tInt* pBufferSize)
{
    RETURN_IF_POINTER_NULL(psInputFormat);

    if (psInputFormat->nBitsPerPixel != 8
        &&  psInputFormat->nBitsPerPixel != 16
        &&  psInputFormat->nBitsPerPixel != 24
        &&  psInputFormat->nBitsPerPixel != 32)
    {
        cerr << "Bits per pixel not supported!" << endl;
        RETURN_ERROR(ERR_NOT_SUPPORTED);
    }

    if (psInputFormat->nPixelFormat != tVideoFormat::PF_UNKNOWN
        &&  psInputFormat->nPixelFormat != tVideoFormat::PF_GREYSCALE_8
        &&  psInputFormat->nPixelFormat != tVideoFormat::PF_GREYSCALE_10
        &&  psInputFormat->nPixelFormat != tVideoFormat::PF_GREYSCALE_12
        &&  psInputFormat->nPixelFormat != tVideoFormat::PF_GREYSCALE_14
        &&  psInputFormat->nPixelFormat != tVideoFormat::PF_GREYSCALE_16
        &&  psInputFormat->nPixelFormat != tVideoFormat::PF_RGB_888
        &&  psInputFormat->nPixelFormat != tVideoFormat::PF_BGR_888
        &&  psInputFormat->nPixelFormat != tVideoFormat::PF_ARGB_8888
        &&  psInputFormat->nPixelFormat != tVideoFormat::PF_ABGR_8888
        &&  psInputFormat->nPixelFormat != tVideoFormat::PF_RGBA_8888
        &&  psInputFormat->nPixelFormat != tVideoFormat::PF_BGRA_8888)
    {
        cerr << "Pixel format not supported!" << endl;
        RETURN_ERROR(ERR_NOT_SUPPORTED);
    }

    // Find the worst-case buffer size.
    *pBufferSize = m_nBufferSize = (psInputFormat->nBytesPerLine * psInputFormat->nHeight) * 2;
    
    m_szMemoryBufferSize = psInputFormat->nBytesPerLine * psInputFormat->nHeight;
    if (m_pMemoryBuffer)
    {
        delete [] m_pMemoryBuffer;
        m_pMemoryBuffer = NULL;
    }
    m_pMemoryBuffer = new tUInt8[m_szMemoryBufferSize];

    m_sInputFormat = *psInputFormat;

    m_nPredictionMethod = PREDICTION_METHOD_TOPLEFT;
    *ppCodecInfo = &m_nPredictionMethod;
    *pCodecInfoSize = sizeof(m_nPredictionMethod);

    RETURN_NOERROR;
}


template<typename tPixel, tInt nPlanes>
tResult cCodecADTF::CompressImpl(const tPixel* pSrc, tInt nSize, tVoid* pDst, tInt* pnCompressedSize)
{
    tInt nWidth =  m_sInputFormat.nWidth;
    tInt nHeight = m_sInputFormat.nHeight;
    tInt nBytesPerLine = m_sInputFormat.nBytesPerLine;

    tPixel*  pBuffer = (tPixel*) m_pMemoryBuffer;

    if (m_nPredictionMethod == PREDICTION_METHOD_TOPLEFT)
    {
        cPredictionFilterTopLeft<tPixel, nPlanes>::forward(pBuffer, pSrc, nWidth, nHeight, nBytesPerLine);
    }
    else
    {
        RETURN_ERROR(ERR_NOT_IMPL);
    }

    *pnCompressedSize = rice_Compress<tPixel>(pBuffer, pDst, nSize);

    RETURN_NOERROR;
}


tResult cCodecADTF::Compress(const tVoid* pSrc, tInt nSize, tVoid* pDst, tInt* pnCompressedSize, tBool* pbKeyFrame)
{
    RETURN_IF_POINTER_NULL(pSrc);
    RETURN_IF_POINTER_NULL(pDst);
    RETURN_IF_POINTER_NULL(pnCompressedSize);
    RETURN_IF_POINTER_NULL(pbKeyFrame);

    if (*pnCompressedSize < m_nBufferSize)
    {
        cerr << "BufferSize is greater or equal then CompressedSize" << endl;
        RETURN_ERROR(ERR_UNKNOWN);
    }

    // Each frame is a key frame
    *pbKeyFrame = tTrue;

    if (m_sInputFormat.nBitsPerPixel == 8)
    {
        return CompressImpl<tUInt8,1>((const tUInt8*) pSrc, nSize, pDst, pnCompressedSize);
    }
    else if (m_sInputFormat.nBitsPerPixel == 16)
    {
        return CompressImpl<tUInt16,1>((const tUInt16*) pSrc, nSize, pDst, pnCompressedSize);
    }
    else if (m_sInputFormat.nBitsPerPixel == 24)
    {
        return CompressImpl<tUInt8,3>((const tUInt8*) pSrc, nSize, pDst, pnCompressedSize);
    }
    else if (m_sInputFormat.nBitsPerPixel == 32)
    {
        return CompressImpl<tUInt8,4>((const tUInt8*) pSrc, nSize, pDst, pnCompressedSize);
    }
    else
    {
        RETURN_ERROR(ERR_NOT_IMPL);
    }
}

