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
#ifndef STREAMING_CODEC_VFW_H
#define STREAMING_CODEC_VFW_H

#pragma pack(push, 1)

struct tVfwCodecInfo
{
    BITMAPINFO      sBitmapFormat;        //< Structure from VfW Codec, size may vary
};

#pragma pack(pop)

class cCodecVfw : public IStreamingCodec
{
    private:
        string              m_strCodec;
        ICINFO              m_IcInfo;
        HIC                 m_hIC;               /// contains the video codec for the compressor
        BITMAPINFO          m_oBitmapInfoIn;    /// stores the bitmap information used for compression
        BITMAPINFO*         m_pBitmapInfoOut;   /// stores the output bitmap information used for compression
        tInt                m_nFrame;
        tInt                m_nBufferSize;      /// Size of temporary compression buffer
		tVfwCodecInfo*      m_pCodecInfo;

    public:
        cCodecVfw(const tChar* strCodec);
        virtual ~cCodecVfw();
        virtual tResult Init(const tBitmapFormat* psInputFormat, tVoid** ppCodecInfo, tUInt32* pCodecInfoSize, tInt* pBufferSize);
        virtual tResult Compress(const tVoid* pSrc, tInt nSize, tVoid* pDst, tInt* pnCompressedSize, tBool* pbKeyFrame);
};

#endif