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

#include <windows.h>
#include <Vfw.h>

class cCodecVfw : public IStreamingCodec
{
    private:
        HIC                 m_hIC;              /// contains the video codec for the decompressor
	    BITMAPINFO*         m_pBitmapInfoIn;    /// stores the bitmap information used for decompression
        BITMAPINFO*         m_pBitmapInfoOut;   /// stores the output bitmap information used for decompression 

    public:
        cCodecVfw();
        virtual ~cCodecVfw();
        virtual tResult Init(const tCompressedVideoFormat* pInputFormat, tBitmapFormat* pOutputFormat);
        virtual tResult Decompress(const tVoid* pInputData, tUInt64 nInputSize, tVoid* pOutput, tSize nOutputSize);

    private:
        tResult InitCodec(ICINFO* pICInfo, const tCompressedVideoFormat* psCodecInfo, tBitmapFormat* pOutputFormat);
};

#endif