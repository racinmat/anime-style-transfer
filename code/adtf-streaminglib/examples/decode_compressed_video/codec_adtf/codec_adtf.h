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
#ifndef STREAMING_CODEC_ADTF_H
#define STREAMING_CODEC_ADTF_H



class cCodecADTF : public IStreamingCodec
{
    private:
        tInt32        m_nPredictionMethod;
        tBitmapFormat m_sBitmapFormat;
    public:
        cCodecADTF();
        virtual ~cCodecADTF();

    public:
        virtual tResult Init(const tCompressedVideoFormat* pInputFormat, tBitmapFormat* pOutputFormat);
        virtual tResult Decompress(const tVoid* pInputData, tUInt64 nInputSize, tVoid* pOutput, tSize nOutputSize);

    private:
        template<typename tPixel, typename tPixel_signed, tInt nPlanes>
        tResult DecompressImpl(const tVoid* pSrc, tUInt nSrcSize, tPixel* pDst, tUInt nDstSize);
};

#endif