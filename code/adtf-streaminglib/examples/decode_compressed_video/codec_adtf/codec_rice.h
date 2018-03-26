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
#ifndef STREAMING_CODEC_RICE_H
#define STREAMING_CODEC_RICE_H

class cCodecRICE : public IStreamingCodec
{
    private:
        tInt32        m_nPredictionMethod;
        tBitmapFormat m_sBitmapFormat;

    public:
        virtual tResult Init(const tCompressedVideoFormat* pInputFormat, tBitmapFormat* pOutputFormat);
        virtual tResult Decompress(const tVoid* pInputData, tUInt64 nInputSize, tVoid* pOutput, tSize nOutputSize);

    private:
        template<typename tPixel, typename tPixel_signed, tInt nPlanes>
        tResult DecompressImpl(const tVoid* pSrc, tUInt nSrcSize, tPixel_signed* pDst, tUInt nDstSize);
};

#endif