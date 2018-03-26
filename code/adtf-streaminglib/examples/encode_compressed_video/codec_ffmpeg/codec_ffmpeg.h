/**
 *
 * ADTF File Access example
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: belkera $
 * $Date: 2011-07-01 08:32:18 +0200 (Fr, 01 Jul 2011) $
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

#pragma pack(push, 1)

struct tAVCodecInfo
{
    tInt32 av_width;
    tInt32 av_height;
    tInt32 av_pix_fmt;
    tInt32 av_time_base_num;
    tInt32 av_time_base_den;
    tInt32 av_extradata_size;
    tUInt8 av_extradata[0];
};

#pragma pack(pop)


class cCodecFFmpeg : public IStreamingCodec
{
    private:
        tBitmapFormat   m_sBitmapFormat;
        AVCodec*        m_pCodec;
        AVCodecContext* m_pContext;
        AVFrame*        m_pFrame;
        AVFrame*        m_pTmpFrame;
        AVPixelFormat     m_nInputPixFmt;
        tUInt64         m_nBufferSize;
        tUInt8*         m_pBuffer;

        tInt            m_nBitrate;
        tAVCodecInfo*   m_pCodecInfo;
        string          m_strCodec;

    public:
        cCodecFFmpeg(const tChar* strCodec = "ffmpeg:mpeg4", tInt nBitrate = 800000);
        virtual ~cCodecFFmpeg();
        
        virtual tResult Init(const tBitmapFormat* psInputFormat, tVoid** ppCodecInfo, tUInt32* pCodecInfoSize, tInt* pBufferSize);
        virtual tResult Compress(const tVoid* pSrc, tInt nSize, tVoid* pDst, tInt* pnCompressedSize, tBool* pbKeyFrame);
};

#endif
