/**
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
#ifndef STREAMING_CODEC_INTF_H
#define STREAMING_CODEC_INTF_H

/**
 * Interface for codec classes inside ADTF StreamingLib
 */
class IStreamingCodec
{
    public:
        virtual tResult Init(const tBitmapFormat* psInputFormat, tVoid** ppCodecInfo, tUInt32* pCodecInfoSize, tInt* pBufferSize) = 0;
        virtual tResult Compress(const tVoid* pSrc, tInt nSize, tVoid* pDst, tInt* pnCompressedSize, tBool* pbKeyFrame) = 0;
};

#endif