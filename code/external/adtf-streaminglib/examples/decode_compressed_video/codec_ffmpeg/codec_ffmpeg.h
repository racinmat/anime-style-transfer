/**
 *
 * ADTF File Access example
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: belkera $
 * $Date: 2011-07-01 08:32:18 +0200 (Fri, 01 Jul 2011) $
 * $Revision: 7973 $
 *
 * @remarks
 *
 */
#ifndef STREAMING_CODEC_VFW_H
#define STREAMING_CODEC_VFW_H

extern "C"
{
#include "libavformat/avformat.h"
}


class cCodecFFmpeg : public IStreamingCodec
{
    private:
        AVCodecContext* m_pContext;
        AVFrame*        m_pFrame;
        AVFrame*        m_pTmpFrame;
        AVPixelFormat     m_nOutputPixFmt;
        tUInt8*         m_pBuffer;
        tUInt64         m_nBufferSize;

        tBitmapFormat   m_sBitmapFormat;

    public:
        cCodecFFmpeg();
        virtual ~cCodecFFmpeg();
        virtual tResult Init(const tCompressedVideoFormat* pInputFormat, tBitmapFormat* pOutputFormat);
        virtual tResult Decompress(const tVoid* pInputData, tUInt64 nInputSize, tVoid* pOutput, tSize nOutputSize);
};

#endif
