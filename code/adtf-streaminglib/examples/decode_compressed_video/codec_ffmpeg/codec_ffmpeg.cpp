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

#include "../stdafx.h"

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

cCodecFFmpeg::cCodecFFmpeg()
    :m_pContext(NULL),
     m_pFrame(NULL),
     m_pTmpFrame(NULL),
     m_pBuffer(NULL),
     m_nBufferSize(0)
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
        av_free(m_pFrame);
    }

    if (m_pTmpFrame)
    {
        av_free(m_pTmpFrame);
    }

    delete [] m_pBuffer;
}

tResult cCodecFFmpeg::Init(const tCompressedVideoFormat* pInputFormat, tBitmapFormat* pOutputFormat)
{
    string strCodec = pInputFormat->strCodec;
    if (strCodec.find("ffmpeg:") != 0)
    {
        RETURN_ERROR(ERR_NOT_SUPPORTED);
    }

    strCodec = strCodec.substr(7);

    // find a decoder
    AVCodec* pAVCodec = avcodec_find_decoder_by_name(strCodec.c_str());
    if (!pAVCodec)
    {
        RETURN_ERROR(ERR_NOT_SUPPORTED);
    }

    const tAVCodecInfo* pAVCodecInfo = (const tAVCodecInfo*) pInputFormat->pCodecData;
    AVCodecContext* pContext = avcodec_alloc_context3(pAVCodec);

    // set up the codec
    pContext->codec_id = pAVCodec->id;
    pContext->codec_type = MEDIA_TYPE_VIDEO;
    pContext->width = pAVCodecInfo->av_width;
    pContext->height = pAVCodecInfo->av_height;
    pContext->pix_fmt = (AVPixelFormat)(pAVCodecInfo->av_pix_fmt);
    pContext->time_base.num = pAVCodecInfo->av_time_base_num;
    pContext->time_base.den = pAVCodecInfo->av_time_base_den;
    pContext->extradata_size = pAVCodecInfo->av_extradata_size;
    pContext->extradata = (tUInt8*) av_malloc(pAVCodecInfo->av_extradata_size);
    memcpy(pContext->extradata, pAVCodecInfo->av_extradata, pAVCodecInfo->av_extradata_size);

    if (avcodec_open2(pContext, pAVCodec) != 0)
    {
        avcodec_close(pContext);
        av_free(pContext);
        RETURN_ERROR(ERR_UNEXPECTED);
    }

    memcpy(&m_sBitmapFormat, &pInputFormat->sBitmapFormat, sizeof(tBitmapFormat));
    memcpy(pOutputFormat, &pInputFormat->sBitmapFormat, sizeof(tBitmapFormat));

    m_pContext = pContext;
    m_pFrame = avcodec_alloc_frame();

    switch(m_sBitmapFormat.nBitsPerPixel)
    {
        case 8:
        {
            m_nOutputPixFmt = AV_PIX_FMT_GRAY8;
            break;
        }
        case 24:
        {
            m_nOutputPixFmt = AV_PIX_FMT_RGB24;
            break;
        }
        case 32:
        {
            m_nOutputPixFmt = AV_PIX_FMT_RGB32;
            break;
        }
        default:
        {
            RETURN_ERROR(ERR_NOT_SUPPORTED);
        }
    }

    // if the output format does not match the decoders output format we perform a conversion
    if (m_pContext->pix_fmt != m_nOutputPixFmt)
    {
        m_pTmpFrame = avcodec_alloc_frame();
    }

    RETURN_NOERROR;
}


tResult cCodecFFmpeg::Decompress(const tVoid* pInputData, tUInt64 nInputSize, tVoid* pOutput, tSize nOutputSize)
{
    RETURN_IF_POINTER_NULL(pInputData);
    RETURN_IF_POINTER_NULL(pOutput);
    if (nOutputSize < m_sBitmapFormat.nSize)
    {
        RETURN_ERROR(ERR_INVALID_ARG);
    }

    // setup a temporary buffer
    if (m_nBufferSize < nInputSize + FF_INPUT_BUFFER_PADDING_SIZE)
    {
        if (m_pBuffer)
        {
            delete [] m_pBuffer;
        }

        m_nBufferSize = nInputSize + FF_INPUT_BUFFER_PADDING_SIZE;
        m_pBuffer = new tUInt8[m_nBufferSize];
    }

    // we have to copy because the buffer needs to be FF_INPUT_BUFFER_PADDING_SIZE bigger than the data
    memcpy(m_pBuffer, pInputData, nInputSize);

    tInt nGotPicture = 0;
    avcodec_decode_video2(m_pContext, m_pFrame, &nGotPicture,
                         (const tUInt8*) m_pBuffer, nInputSize);
    if (nGotPicture == 0)
    {
        RETURN_ERROR(ERR_FAILED);
    }

    if (m_pContext->pix_fmt == m_nOutputPixFmt)
    {
        // if the format matches just copy
        memcpy(pOutput, m_pFrame->data[0], m_sBitmapFormat.nSize);
    }
    else
    {
        // otherwise convert
        avpicture_fill((AVPicture*) m_pTmpFrame, (tUInt8*) pOutput, m_nOutputPixFmt,
                       m_sBitmapFormat.nWidth,
                       m_sBitmapFormat.nHeight);
        img_convert((AVPicture*)m_pTmpFrame, m_nOutputPixFmt,
                    (AVPicture*)m_pFrame, m_pContext->pix_fmt,
                    m_sBitmapFormat.nWidth,
                    m_sBitmapFormat.nHeight);
    }

    RETURN_NOERROR;
}

