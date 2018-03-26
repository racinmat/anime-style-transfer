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
    public:
        cCodecADTF();
        virtual ~cCodecADTF();

    private:
        tInt              m_nBufferSize;
        tInt32            m_nPredictionMethod;
        tUInt8*           m_pMemoryBuffer;
        tSize             m_szMemoryBufferSize;
        tBitmapFormat     m_sInputFormat;

    public:
        virtual tResult Init(const tBitmapFormat* psInputFormat, tVoid** ppCodecInfo, tUInt32* pCodecInfoSize, tInt* pBufferSize);
        virtual tResult Compress(const tVoid* pSrc, tInt nSize, tVoid* pDst, tInt* pnCompressedSize, tBool* pbKeyFrame);

    private:
        template<typename tPixel, tInt nPlanes>
        tResult CompressImpl(const tPixel* pSrc, tInt nSize, tVoid* pDst, tInt* nCompressedSize);
};

#endif