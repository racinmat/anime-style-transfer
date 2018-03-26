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

#define PREDICTION_METHOD_INVALID   0
#define PREDICTION_METHOD_TOPLEFT   1

static tInt INLINE decode_signbit(tUInt nCode)
{
    tInt nx = nCode;
    return (- (nx & 1)) ^ (nx >> 1);
}

template<typename tPixel>
static tInt INLINE decode_signbit(tPixel nCode, tPixel nPred)
{
    const tInt nBitCount = sizeof(tPixel)*8;
    const tInt nMax = (1 << nBitCount) - 1;

    tInt nx = nCode;

    if (nPred <= nMax/2)
    {
        if (nx <= nPred*2)
        {
            return (- (nx & 1)) ^ (nx >> 1);
        }
        else
        {
            tInt nRest = nx - nPred*2;
            nx = nPred*2;
            return ((- (nx & 1)) ^ (nx >> 1)) + nRest;
        }
    }
    else /* nPred > nMax/2 */
    {
        if (nx <= (nMax - nPred)*2)
        {
            return (- (nx & 1)) ^ (nx >> 1);
        }
        else
        {
            tInt nRest = nx - (nMax - nPred)*2;
            nx = (nMax - nPred)*2;
            return - ((- (nx & 1)) ^ (nx >> 1)) - nRest;
        }
    }
}

/**
 * Template class for pixel prediction algorithm
 */
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
            tInt nPred = pDst[nX-nPlanes];
            tInt nDiff = decode_signbit<tPixel>(pSrc[nX], nPred);
            pDst[nX] = nPred + nDiff;
        }

        for (tInt nY = 1;  nY < nHeight;  nY++)
        {
            pSrc = (const tPixel*)(((const tUInt8*) pSrc) + nBytesPerLine);
            pDst =       (tPixel*)((      (tUInt8*) pDst) + nBytesPerLine);

            for (tInt nX = 0;  nX < nPlanes;  nX++)
            {
                tInt nPred = pDst[nX-nLineOffset];
                tInt nDiff = decode_signbit<tPixel>(pSrc[nX], nPred);
                pDst[nX] = nPred + nDiff;
            }
            for (tInt nX = nPlanes;  nX < nMaxX;  nX++)
            {
                tInt nPred = (pDst[nX-nPlanes] + pDst[nX-nLineOffset]) / 2;
                tInt nDiff = decode_signbit<tPixel>(pSrc[nX], nPred);
                pDst[nX] = nPred + nDiff;
            }
        }
    }
};

cCodecADTF::cCodecADTF()
{
}

cCodecADTF::~cCodecADTF()
{
}

//*************************************************************************************************
tResult cCodecADTF::Init(const tCompressedVideoFormat* pInputFormat, tBitmapFormat* pOutputFormat)
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
tResult cCodecADTF::DecompressImpl(const tVoid* pSrc, tUInt nSrcSize, tPixel* pDst, tUInt nDstSize)
{
    // decompress the data
    rice_Uncompress((tVoid*)pSrc, pDst, nSrcSize, nDstSize);

    tInt nWidth = m_sBitmapFormat.nWidth;
    tInt nHeight = m_sBitmapFormat.nHeight;
    tInt nBytesPerLine = m_sBitmapFormat.nBytesPerLine;

    // and then predict the remaining pixels
    if (m_nPredictionMethod == PREDICTION_METHOD_TOPLEFT)
    {
        cPredictionFilterTopLeft<tPixel,nPlanes>::backward(pDst, pDst, nWidth, nHeight, nBytesPerLine);
    }
    else
    {
        RETURN_ERROR(ERR_NOT_IMPL);
    }

    RETURN_NOERROR;
}


tResult cCodecADTF::Decompress(const tVoid* pInputData, tUInt64 nInputSize, tVoid* pOutput, tSize nOutputSize)
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
            return DecompressImpl<tUInt8,tInt8,1>(pInputData, (tUInt) nInputSize, (tUInt8*) pOutput, (tUInt) nOutputSize);
        }
        case 16:
        {
            return DecompressImpl<tUInt16,tInt16,1>(pInputData, (tUInt) nInputSize, (tUInt16*) pOutput, (tUInt) nOutputSize);
        }
        case 24:
        {
            return DecompressImpl<tUInt8,tInt8,3>(pInputData, (tUInt) nInputSize, (tUInt8*) pOutput, (tUInt) nOutputSize);
        }
        case 32:
        {
            return DecompressImpl<tUInt8,tInt8,4>(pInputData, (tUInt) nInputSize, (tUInt8*) pOutput, (tUInt) nOutputSize);
        }
        default:
        {
            RETURN_ERROR(ERR_NOT_SUPPORTED);
        }
    }

    RETURN_NOERROR;
}

