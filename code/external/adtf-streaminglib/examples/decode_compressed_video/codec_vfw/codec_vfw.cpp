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

#include "../stdafx.h"

union tFOURCC 
{
    tUInt32 nValue;                     //< FOURCC of codec as integer
    tChar   strValue[4];                //< FOURCC of codec as string
};

#pragma pack(push, 1)

struct tVfwCodecInfoV1
{
    BITMAPINFO      sBitmapInfo;        //< Structure from VfW Codec, size may vary
};

struct tVfwCodecInfoV2
{
    tUInt32         nMagic;             //< Used to distinguish between V1 and V2
    tBitmapFormat   sOrgBitmapInfo;     //< Original input bitmap info stucture
    BITMAPINFO      sBitmapInfo;        //< Structure from VfW Codec, size may vary
};

#pragma pack(pop)

const static tUInt32 s_nVfwCodecInfoMagicV2 = 0x42424242;

cCodecVfw::cCodecVfw():
    m_hIC(NULL),
    m_pBitmapInfoIn(NULL),
    m_pBitmapInfoOut(NULL)
{
}

cCodecVfw::~cCodecVfw()
{
    if (m_hIC)
    {
        ICClose(m_hIC);
    }

    delete [] (tUInt8*) m_pBitmapInfoIn;
    delete [] (tUInt8*) m_pBitmapInfoOut;
}

tResult cCodecVfw::Init(const tCompressedVideoFormat* pInputFormat, tBitmapFormat* pOutputFormat)
{
    ICINFO  icinfo;
    HIC     hIC;

	
	string strCodecName = pInputFormat->strCodec;
    if (strCodecName.find("vfw:") == 0)
	{
        // in version 2 the codec fourcc is prefixed with vfw:
        strCodecName = strCodecName.substr(4);
	}
	tFOURCC uFourCC;
	uFourCC.strValue[0] = strCodecName.c_str()[0];
	uFourCC.strValue[1] = strCodecName.c_str()[1];
	uFourCC.strValue[2] = strCodecName.c_str()[2];
	uFourCC.strValue[3] = strCodecName.c_str()[3];

	// search for codec
	for (DWORD i=0;  ICInfo(ICTYPE_VIDEO, i, &icinfo);  i++) 
	{ 
	    // opens a given codec for test
	    hIC = ICOpen(icinfo.fccType, icinfo.fccHandler, ICMODE_QUERY); 
	    if (hIC) 
	    { 
		    // Find out the compressor name. 
		    ICGetInfo(hIC, &icinfo, sizeof(icinfo));

            if (icinfo.fccType == ICTYPE_VIDEO &&
                uFourCC.nValue == icinfo.fccHandler)	
            {
                return InitCodec(&icinfo, pInputFormat, pOutputFormat);
            }

            ICClose(hIC); 
        }			
	}

    RETURN_ERROR(ERR_NOT_SUPPORTED);
}

tResult cCodecVfw::InitCodec(ICINFO* pICInfo, const tCompressedVideoFormat* psCodecInfo, tBitmapFormat* pOutputFormat)
{
    // open the given codec
	m_hIC = ICOpen(pICInfo->fccType, pICInfo->fccHandler, ICMODE_DECOMPRESS); 
	if (m_hIC == NULL)
    {
        RETURN_ERROR(ERR_UNEXPECTED);
    }

    // setup the codec infos depending on the version
    if (psCodecInfo->nVersion == ADTF_CODEC_INFO_VERSION_1_x)
    {
        const tVfwCodecInfoV1* psCodecInfo_V1 = (const tVfwCodecInfoV1*) psCodecInfo->pCodecData;
        const tVfwCodecInfoV2* psCodecInfo_V2 = (const tVfwCodecInfoV2*) psCodecInfo->pCodecData;

        if (psCodecInfo_V2->nMagic == s_nVfwCodecInfoMagicV2)
        {
            m_pBitmapInfoIn = (BITMAPINFO*) new tUInt8[psCodecInfo_V2->sBitmapInfo.bmiHeader.biSize];

            // Initialize the bitmap structure. 
            memcpy(m_pBitmapInfoIn, &psCodecInfo_V2->sBitmapInfo, psCodecInfo_V2->sBitmapInfo.bmiHeader.biSize);
        }
        else /* Vfw_CODEC_INFO_V1 */
        {
            m_pBitmapInfoIn = (BITMAPINFO*) new tUInt8[psCodecInfo_V1->sBitmapInfo.bmiHeader.biSize];

            // Initialize the bitmap structure. 
            memcpy(m_pBitmapInfoIn, &psCodecInfo_V1->sBitmapInfo, psCodecInfo_V1->sBitmapInfo.bmiHeader.biSize);
        }
    }
    //
    else
    {
        BITMAPINFO* pBitmapInfo = static_cast<BITMAPINFO*>(psCodecInfo->pCodecData);
        m_pBitmapInfoIn = (BITMAPINFO*) new tUInt8[pBitmapInfo->bmiHeader.biSize];

        // Initialize the bitmap structure. 
        memcpy(m_pBitmapInfoIn, pBitmapInfo, pBitmapInfo->bmiHeader.biSize);
    }

    // Get the output format
    DWORD dwFormatSize = ICDecompressGetFormatSize(m_hIC, m_pBitmapInfoIn);
    m_pBitmapInfoOut = (BITMAPINFO*) new tUInt8[dwFormatSize];
    memset(m_pBitmapInfoOut, 0x00, dwFormatSize);
    m_pBitmapInfoOut->bmiHeader.biSize = dwFormatSize;

    if (ICDecompressGetFormat(m_hIC, m_pBitmapInfoIn, m_pBitmapInfoOut) != ICERR_OK)
    {
        ICClose(m_hIC);
        m_hIC = NULL;
        RETURN_ERROR(ERR_UNEXPECTED);
    }

    if (ICDecompressBegin(m_hIC, m_pBitmapInfoIn, m_pBitmapInfoOut) != ICERR_OK)
    { 
        // Handle the error identifying the unsupported format. 
        // this should not happen because we checked the format
        ICClose(m_hIC); 
        m_hIC = NULL;
        RETURN_ERROR(ERR_UNEXPECTED);
    } 

    // setup the output format structure
    pOutputFormat->nBitsPerPixel = m_pBitmapInfoOut->bmiHeader.biBitCount;
    pOutputFormat->nSize = m_pBitmapInfoOut->bmiHeader.biSizeImage;
    pOutputFormat->nWidth = m_pBitmapInfoOut->bmiHeader.biWidth;
    pOutputFormat->nHeight = m_pBitmapInfoOut->bmiHeader.biHeight;
    pOutputFormat->nBytesPerLine = pOutputFormat->nBitsPerPixel / 8 * pOutputFormat->nWidth;
    pOutputFormat->nPaletteSize = 0;
    switch (pOutputFormat->nBitsPerPixel)
    {
        case 8:
        {
            pOutputFormat->nPixelFormat = 11; //PF_GRAYSCALE_8
            break;
        }
        case 16:
        {
            pOutputFormat->nPixelFormat = 24; //PF_GRAYSCALE_16
            break;
        }
        case 24:
        {
            pOutputFormat->nPixelFormat = 45; //PF_RGB_888
            break;
        }
        case 32:
        {
            pOutputFormat->nPixelFormat = 53; //PF_RGBA_8888
            break;
        }
    }

    RETURN_NOERROR;
}

tResult cCodecVfw::Decompress(const tVoid* pInputData, tUInt64 nInputSize, tVoid* pOutput, tSize nOutputSize)
{
    if (m_hIC == NULL)
    {
        RETURN_ERROR(ERR_NOT_INITIALISED);
    }

    if (m_pBitmapInfoOut->bmiHeader.biSizeImage != nOutputSize)
    {
        RETURN_ERROR(ERR_INVALID_ARG);
    }

    m_pBitmapInfoIn->bmiHeader.biSizeImage = (DWORD) nInputSize;
    DWORD nResult = ICDecompress(m_hIC, 0,
                                 &m_pBitmapInfoIn->bmiHeader, (LPVOID) pInputData,
                                 &m_pBitmapInfoOut->bmiHeader, (LPVOID) pOutput);
    if (nResult != ICERR_OK)
    { 
        // Handle compressor error.
        RETURN_ERROR(ERR_UNEXPECTED);
    } 

    RETURN_NOERROR;
}

