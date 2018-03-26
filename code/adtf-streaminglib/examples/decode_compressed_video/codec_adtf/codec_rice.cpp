/**
 *
 * ADTF File Access example
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: A87AWGT $
 * $Date: 2011-07-18 14:39:35 +0200 (Mon, 18 Jul 2011) $
 * $Revision: 8152 $
 *
 * @remarks
 *
 */

#include "../stdafx.h"

#include "rice.h"

#pragma pack(push, 1)

// struct for the additional codec data of version 1 RICE
struct tRiceCodecInfo_ADTF1
{
    tInt32              nPredictionMethod;
    tWin32BitmapInfo    sBitmapInfo;
};

#pragma pack(pop)

#if defined(_MSC_VER)
    #define INLINE  __forceinline
#else
    #define INLINE  inline
#endif

#define PREDICTION_METHOD_NONE       0
#define PREDICTION_METHOD_TOP        1
#define PREDICTION_METHOD_LEFT       2
#define PREDICTION_METHOD_TOPLEFT    3

template<typename tPixel> 
class cPredictionFilterTop
{
public:
    static tVoid backward(tPixel* pDst, const tPixel* pSrc,
                          tInt nWidth, tInt nHeight, tInt nBytesPerLine)
    {
        // Handle first scanline special
        memcpy(pDst, pSrc, nBytesPerLine);

        pDst = (tPixel*)(((tUInt8*) pDst) + nBytesPerLine);

        for (tInt nY = 1;  nY < nHeight;  nY++)
        {
            for (tInt nX = 0;  nX < nWidth;  nX++)
            {
                pDst[nX] = pSrc[nX] - pDst[nX];
            }

            pSrc = (const tPixel*)(((const tUInt8*) pSrc) + nBytesPerLine);
            pDst =       (tPixel*)((      (tUInt8*) pDst) + nBytesPerLine);

        }
    }
};

template<typename tPixel, tInt nPlanes> 
class cPredictionFilterLeft
{
public:
    static tVoid backward(tPixel* pDst, const tPixel* pSrc,
                          tInt nWidth, tInt nHeight, tInt nBytesPerLine)
    {
        tInt nPixelsPerLine = nBytesPerLine / sizeof(tPixel);
        tInt nMaxX = nWidth * nPlanes;

        for (tInt nY = 0;  nY < nHeight;  nY++)
        {
            // Handle first scanline special
            if (nY == 0)
            {
                pDst[0] = pSrc[0];
            }
            else
            {
                pDst[0] = pDst[-nPixelsPerLine] - pSrc[0];
            }

            for (tInt nX = 1;  nX < nMaxX;  nX++)
            {
                pDst[nX] = pDst[nX-nPlanes] - pSrc[nX];
            }

            pSrc = (const tPixel*)(((const tUInt8*) pSrc) + nBytesPerLine);
            pDst =       (tPixel*)((      (tUInt8*) pDst) + nBytesPerLine);
        }
    }
};

template<typename tPixel, tInt nPlanes> 
class cPredictionFilterTopLeft
{
public:
    static tVoid backward(tPixel* pDst, const tPixel* pSrc,
                          tInt nWidth, tInt nHeight, tInt nBytesPerLine)
    {
        tInt nLineOffset = nBytesPerLine / sizeof(tPixel);
        tInt nMaxX = nWidth * nPlanes;

        // Handle first scanline special
        for (tInt nX = 0;  nX < nPlanes;  nX++)
        {
            pDst[nX] = pSrc[nX];
        }
        for (tInt nX = nPlanes;  nX < nMaxX;  nX++)
        {
            pDst[nX] = pDst[nX-nPlanes] - pSrc[nX];
        }

        for (tInt nY = 1;  nY < nHeight;  nY++)
        {
            pSrc = (const tPixel*)(((const tUInt8*) pSrc) + nBytesPerLine);
            pDst =       (tPixel*)((      (tUInt8*) pDst) + nBytesPerLine);

            for (tInt nX = 0;  nX < nPlanes;  nX++)
            {
                pDst[nX] = pDst[nX - nLineOffset] - pSrc[nX];
            }
            for (tInt nX = nPlanes;  nX < nMaxX;  nX++)
            {
                pDst[nX] = (pDst[nX-nPlanes] + pDst[nX - nLineOffset]) / 2 - pSrc[nX];
            }
        }
    }
};

//*************************************************************************************************
tResult cCodecRICE::Init(const tCompressedVideoFormat* pInputFormat, tBitmapFormat* pOutputFormat)
{
    if (string(pInputFormat->strCodec) == "ADTF" || pInputFormat->nVersion == ADTF_CODEC_INFO_VERSION_2_0)
    {
        // the ADTF codec and the version 2 RICE codec only store the prediction method as additional data.
        m_nPredictionMethod = *reinterpret_cast<tInt32*>(pInputFormat->pCodecData);
        memcpy(pOutputFormat, &pInputFormat->sBitmapFormat, sizeof(tBitmapFormat));
        memcpy(&m_sBitmapFormat, &pInputFormat->sBitmapFormat, sizeof(tBitmapFormat));
    }
    else if (pInputFormat->nVersion == ADTF_CODEC_INFO_VERSION_1_x)
    {
        // version 1 of RICE also stored the output format in the additional codec data
        const tRiceCodecInfo_ADTF1* pCodecInfoADTF1 = reinterpret_cast<const tRiceCodecInfo_ADTF1*>(pInputFormat->pCodecData);

        m_nPredictionMethod = pCodecInfoADTF1->nPredictionMethod;

        m_sBitmapFormat.nWidth          = pCodecInfoADTF1->sBitmapInfo.sHeader.nWidth;
        m_sBitmapFormat.nHeight         = pCodecInfoADTF1->sBitmapInfo.sHeader.nHeight;
        m_sBitmapFormat.nBitsPerPixel   = pCodecInfoADTF1->sBitmapInfo.sHeader.nBitCount;
        m_sBitmapFormat.nPixelFormat    = m_sBitmapFormat.nBitsPerPixel == 24 ? 45 :
                                          m_sBitmapFormat.nBitsPerPixel == 16 ? 24 :
                                          m_sBitmapFormat.nBitsPerPixel == 8  ? 11 : 
                                          1000;
        m_sBitmapFormat.nBytesPerLine   = m_sBitmapFormat.nWidth * m_sBitmapFormat.nBitsPerPixel / 8;
        m_sBitmapFormat.nBytesPerLine  += 4 - 1;
        m_sBitmapFormat.nBytesPerLine  -= m_sBitmapFormat.nBytesPerLine % 4;
        m_sBitmapFormat.nSize           = m_sBitmapFormat.nBytesPerLine * m_sBitmapFormat.nHeight;
        m_sBitmapFormat.nPaletteSize    = pCodecInfoADTF1->sBitmapInfo.sHeader.nClrImportant;

        memcpy(pOutputFormat, &m_sBitmapFormat, sizeof(tBitmapFormat));
    }

    RETURN_NOERROR;
}

template<typename tPixel, typename tPixel_signed, tInt nPlanes>
tResult cCodecRICE::DecompressImpl(const tVoid* pSrc, tUInt nSrcSize, tPixel_signed* pDst, tUInt nDstSize)
{
    // decompress the data
    rice_Uncompress((tVoid*)pSrc, pDst, nSrcSize, nDstSize);

    tInt nWidth = m_sBitmapFormat.nWidth;
    tInt nHeight = m_sBitmapFormat.nHeight;
    tInt nBytesPerLine = m_sBitmapFormat.nBytesPerLine;

    // and then predict the remaining pixels
    if (m_nPredictionMethod == PREDICTION_METHOD_NONE)
    {
    }
    else if (m_nPredictionMethod == PREDICTION_METHOD_TOP)
    {
        cPredictionFilterTop<tPixel_signed>::backward(pDst, pDst,
                                               nWidth, nHeight, nBytesPerLine);
    }
    else if (m_nPredictionMethod == PREDICTION_METHOD_LEFT)
    {
        cPredictionFilterLeft<tPixel_signed,nPlanes>::backward(pDst, pDst, 
                                                        nWidth, nHeight, nBytesPerLine);
    }
    else if (m_nPredictionMethod == PREDICTION_METHOD_TOPLEFT)
    {
        cPredictionFilterTopLeft<tPixel_signed,nPlanes>::backward(pDst, pDst, 
                                                           nWidth, nHeight, nBytesPerLine);
    }
    else
    {
        RETURN_ERROR(ERR_NOT_IMPL);
    }

    RETURN_NOERROR;
}


tResult cCodecRICE::Decompress(const tVoid* pInputData, tUInt64 nInputSize, tVoid* pOutput, tSize nOutputSize)
{
    if (nOutputSize != m_sBitmapFormat.nSize)
    {
        RETURN_ERROR(ERR_INVALID_ARG);
    }

    // select the corresponding decompression template
    switch (m_sBitmapFormat.nBitsPerPixel)
    {
        case 8:
        {
            return DecompressImpl<tUInt8,tInt8,1>(pInputData, (tUInt) nInputSize, (tInt8*) pOutput, (tUInt) nOutputSize);
        }
        case 16:
        {
            return DecompressImpl<tUInt16,tInt16,1>(pInputData, (tUInt) nInputSize, (tInt16*) pOutput, (tUInt) nOutputSize);
        }
        case 24:
        {
            return DecompressImpl<tUInt8,tInt8,3>(pInputData, (tUInt) nInputSize, (tInt8*) pOutput, (tUInt) nOutputSize);
        }
        case 32:
        {
            return DecompressImpl<tUInt8,tInt8,4>(pInputData, (tUInt) nInputSize, (tInt8*) pOutput, (tUInt) nOutputSize);
        }
        default:
        {
            RETURN_ERROR(ERR_NOT_SUPPORTED);
        }
    }

    RETURN_NOERROR;
}

