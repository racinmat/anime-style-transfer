/**
 * ADTF Video Compression Codec / Prediction filters
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: VG8D3AW $
 * $Date: 2012-08-27 09:03:00 +0200 (Mon, 27 Aug 2012) $
 * $Revision: 15393 $
 *
 * @remarks
 *
 */
#ifndef _CODEC_ADTF_PREDICTION_HEADER_
#define _CODEC_ADTF_PREDICTION_HEADER_

#if defined(_MSC_VER)
    #define INLINE  __forceinline
#else
    #define INLINE  inline
#endif

#define PREDICTION_METHOD_INVALID   0
#define PREDICTION_METHOD_TOPLEFT   1

/**
 * Template function for coding/decoding sign bit
 */
static tUInt INLINE code_signbit(tInt nDiff)
{
    return (nDiff << 1) ^ (nDiff >> (sizeof(tInt)*8-1));
}

static tInt INLINE decode_signbit(tUInt nCode)
{
    tInt nx = nCode;
    return (- (nx & 1)) ^ (nx >> 1);
}

template<typename tPixel>
static tPixel INLINE code_signbit(tInt nDiff, tPixel nPred)
{
    const tInt nBitCount = sizeof(tPixel)*8;
    const tInt nMax = (1 << nBitCount) - 1;

    if (nPred <= nMax/2)
    {
        if (nDiff <= nPred)
        {
            return (nDiff << 1) ^ (nDiff >> (nBitCount-1));
        }
        else
        {
            tInt nRest = nDiff - nPred;
            nDiff = nPred;
            return ((nDiff << 1) ^ (nDiff >> (nBitCount-1))) + nRest;
        }
    }
    else /* nPred > nMax/2 */
    {
        if (nDiff >= -(nMax - nPred))
        {
            return (nDiff << 1) ^ (nDiff >> (nBitCount-1));
        }
        else
        {
            tInt nRest = nPred - nDiff - nMax;
            nDiff = (nMax - nPred);
            return ((nDiff << 1) ^ (nDiff >> (nBitCount-1))) + nRest;
        }
    }
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

//template<typename tPixel>
//static tPixel INLINE code_signbit(tInt nDiff, tPixel nPred)
//{
//    return (nDiff << 1) ^ (nDiff >> (sizeof(tInt)*8-1));
//}
//
//template<typename tPixel>
//static tInt INLINE decode_signbit(tPixel nCode, tPixel nPred)
//{
//    tInt nx = nCode;
//    return (- (nx & 1)) ^ (nx >> 1);
//}


/**
 * Template class for pixel prediction algorithm
 */
template<typename tPixel, tInt nPlanes>
class cPredictionFilterTopLeft
{
public:
    static tVoid forward(tPixel* pDst, const tPixel* pSrc,
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
            tInt nPred = pSrc[nX-nPlanes];
            tInt nDiff = pSrc[nX] - nPred;
            pDst[nX] = code_signbit<tPixel>(nDiff, nPred);
        }

        for (tInt nY = 1;  nY < nHeight;  nY++)
        {
            pSrc = (const tPixel*)(((const tUInt8*) pSrc) + nBytesPerLine);
            pDst =       (tPixel*)((      (tUInt8*) pDst) + nBytesPerLine);

            for (tInt nX = 0;  nX < nPlanes;  nX++)
            {
                tInt nPred = pSrc[nX-nLineOffset];
                tInt nDiff = pSrc[nX] - nPred;
                pDst[nX] = code_signbit<tPixel>(nDiff, nPred);
            }

            for (tInt nX = nPlanes;  nX < nMaxX;  nX++)
            {
                tInt nPred = (pSrc[nX-nPlanes] + pSrc[nX-nLineOffset]) / 2;
                tInt nDiff = pSrc[nX] - nPred;
                pDst[nX] = code_signbit<tPixel>(nDiff, nPred);
            }
        }
    }

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

#endif // _CODEC_ADTF_PREDICTION_HEADER_
