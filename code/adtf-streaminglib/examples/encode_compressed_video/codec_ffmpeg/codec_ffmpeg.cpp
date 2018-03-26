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

#include "../stdafx.h"
#include "codec_ffmpeg.h"

cCodecFFmpeg::cCodecFFmpeg(const tChar* strCodec, tInt nBitrate)
    :m_pContext(NULL),
     m_pFrame(NULL),
     m_pTmpFrame(NULL),
     m_pBuffer(NULL),
     m_nBufferSize(0),
     m_nBitrate(nBitrate),
     m_pCodecInfo(NULL),
     m_strCodec(strCodec)
{
    av_register_all();
}

cCodecFFmpeg::~cCodecFFmpeg()
{
   if (m_pContext)
    {
        avcodec_close(m_pContext);
        av_free(m_pContext);
    }

    if (m_pFrame)
    {
        avpicture_free((AVPicture*)m_pFrame);
        av_free(m_pFrame);
    }

    if (m_pTmpFrame)
    {
        av_free(m_pTmpFrame);
    }

    if (m_pCodecInfo)
    {
        delete [] reinterpret_cast<tUInt8*>(m_pCodecInfo);
    }
}

tResult cCodecFFmpeg::Init(const tBitmapFormat* psInputFormat, tVoid** ppCodecInfo, tUInt32* pCodecInfoSize, tInt* pBufferSize)
{
    RETURN_IF_POINTER_NULL(psInputFormat);

    if (psInputFormat->nBitsPerPixel != 8
        &&  psInputFormat->nBitsPerPixel != 24
        &&  psInputFormat->nBitsPerPixel != 32)
    {
        cerr << "Bits per pixel not supported!" << endl;
        RETURN_ERROR(ERR_NOT_SUPPORTED);
    }

    if (psInputFormat->nPixelFormat != tVideoFormat::PF_UNKNOWN
        &&  psInputFormat->nPixelFormat != tVideoFormat::PF_GREYSCALE_8
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

    m_sBitmapFormat = *psInputFormat;

    string strSubCodec = m_strCodec.substr(7, m_strCodec.size() - 7);

    m_pCodec = avcodec_find_encoder_by_name(strSubCodec.c_str());

    if (m_pCodec->type != CODEC_TYPE_VIDEO ||
        m_pCodec->encode == NULL)
    {
        cerr << "Codec '" << strSubCodec << "' does not support video encoding." << endl;
        RETURN_ERROR(ERR_NOT_SUPPORTED);
    }

    AVCodecContext* pContext = avcodec_alloc_context();
    pContext->codec_id = m_pCodec->id;
    pContext->codec_type = CODEC_TYPE_VIDEO;
    pContext->width = psInputFormat->nWidth;
    pContext->height = psInputFormat->nHeight;
    pContext->time_base = (AVRational)
    {
        1, 25
    };
    pContext->bit_rate = m_nBitrate;

    //pContext->max_b_frames = 1;
    pContext->sample_fmt = SAMPLE_FMT_NONE;

    if (m_pCodec->pix_fmts != NULL)
    {
        pContext->pix_fmt = m_pCodec->pix_fmts[0];

        for (tInt nIdx = 0;  m_pCodec->pix_fmts[nIdx] != PIX_FMT_NONE;  nIdx++)
        {
            if (m_pCodec->pix_fmts[nIdx] == PIX_FMT_GRAY8
                &&  psInputFormat->nBitsPerPixel == 8)
            {
                pContext->pix_fmt = PIX_FMT_GRAY8;
                break;
            }
            if (m_pCodec->pix_fmts[nIdx] == PIX_FMT_RGB24
                &&  psInputFormat->nBitsPerPixel == 24)
            {
                pContext->pix_fmt = PIX_FMT_RGB24;
                break;
            }
            if (m_pCodec->pix_fmts[nIdx] == PIX_FMT_RGB32
                &&  psInputFormat->nBitsPerPixel == 32)
            {
                pContext->pix_fmt = PIX_FMT_RGB32;
                break;
            }
        }
    }
    else
    {
        pContext->pix_fmt = PIX_FMT_NONE;
    }

    if (avcodec_open(pContext, m_pCodec) < 0)
    {
        cerr << "Failed to init codec!" << endl;
        RETURN_ERROR(ERR_NOT_SUPPORTED);
    }

    m_pContext = pContext;
    m_pTmpFrame = NULL;

    // Init codec information
    tInt nCodecSize = sizeof(tAVCodecInfo) + pContext->extradata_size;
    m_pCodecInfo = reinterpret_cast<tAVCodecInfo*>(new tUInt8[nCodecSize]);
    m_pCodecInfo->av_width = pContext->width;
    m_pCodecInfo->av_height = pContext->height;
    m_pCodecInfo->av_time_base_num = pContext->time_base.num;
    m_pCodecInfo->av_time_base_den = pContext->time_base.den;
    m_pCodecInfo->av_pix_fmt = pContext->pix_fmt;
    m_pCodecInfo->av_extradata_size = pContext->extradata_size;
    memcpy(m_pCodecInfo->av_extradata, pContext->extradata, pContext->extradata_size);

    *ppCodecInfo = m_pCodecInfo;
    *pCodecInfoSize = nCodecSize;

    m_nBufferSize = avpicture_get_size(m_pContext->pix_fmt,
                                       psInputFormat->nWidth,
                                       psInputFormat->nHeight);

    *pBufferSize = m_nBufferSize;

    m_pFrame = avcodec_alloc_frame();
    avpicture_alloc((AVPicture*)m_pFrame,
                    m_pContext->pix_fmt,
                    psInputFormat->nWidth,
                    psInputFormat->nHeight);

    if (psInputFormat->nBitsPerPixel == 8)
    {
        m_nInputPixFmt = PIX_FMT_GRAY8;
    }
    if (psInputFormat->nBitsPerPixel == 24)
    {
        m_nInputPixFmt = PIX_FMT_RGB24;
    }
    if (psInputFormat->nBitsPerPixel == 32)
    {
        m_nInputPixFmt = PIX_FMT_RGB32;
    }

    if (m_pContext->pix_fmt != m_nInputPixFmt)
    {
        m_pTmpFrame = avcodec_alloc_frame();
    }

    RETURN_NOERROR;
}

tResult cCodecFFmpeg::Compress(const tVoid* pSrc, tInt nSize, tVoid* pDst, tInt* pnCompressedSize, tBool* pbKeyFrame)
{
    RETURN_IF_POINTER_NULL(pSrc);
    RETURN_IF_POINTER_NULL(pDst);
    RETURN_IF_POINTER_NULL(pnCompressedSize);
    RETURN_IF_POINTER_NULL(pbKeyFrame);

    AVFrame*  pFrame;

    avpicture_fill((AVPicture*)m_pTmpFrame, (tUInt8*) pSrc, m_nInputPixFmt,
                   m_sBitmapFormat.nWidth,
                   m_sBitmapFormat.nHeight);

    if (m_pContext->pix_fmt != m_nInputPixFmt)
    {
        img_convert((AVPicture*)m_pFrame, m_pContext->pix_fmt,
                    (AVPicture*)m_pTmpFrame, m_nInputPixFmt,
                    m_sBitmapFormat.nWidth,
                    m_sBitmapFormat.nHeight);
        pFrame = m_pFrame;
    }
    else
    {
        pFrame = m_pTmpFrame;
    }

    pFrame->key_frame = *pbKeyFrame;
    tInt nOutSize = avcodec_encode_video(m_pContext, (tUInt8*) pDst, nSize, pFrame);
    if (nOutSize < 0)
    {
        *pnCompressedSize = 0;
        RETURN_ERROR(ERR_FAILED);
    }

    *pnCompressedSize = nOutSize;
    *pbKeyFrame = pFrame->key_frame;
    RETURN_NOERROR;
}
