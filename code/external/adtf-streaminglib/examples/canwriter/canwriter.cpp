/**
*
*  Small example program for adtf streaming lib
*  Creates a dat file with two can streams, one
*  of them suitable for the candump example
*
* Demonstrates:
* - creating and releasing a IADTFFileWriter
* - opening and closing a dat file
*   - creating a stream with non-basic sample type
*   - writing data to said stream
*
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
*
 * $Author:  $
 * $Date:  $
 * $Revision:  $
*
* @remarks
*
*/

#include <adtf_streaming.h>
#include <stdio.h>
#include <iostream>
#include "canstruct.h"
using namespace adtfstreaming;
using namespace std;

int main(int argc, char* argv[])
{
    //create writer
    IADTFFileWriter* pFileWriter = IADTFFileWriter::Create();
    //open file
    if (IS_FAILED(pFileWriter->Open("test_rawcan.dat")))
    {
        printf("FAILED to open FILE\n");
        IADTFFileWriter::Release(pFileWriter);
        return 0;
    }
    //can data structures
    tCanMessage oMessage;
    memset(&oMessage,0x0,sizeof(tCanMessage));
    oMessage.ui8Channel = 01;
    oMessage.ui8Length  = 8;

    tCanMessageExt oMessageExt;
    memset(&oMessageExt,0x0,sizeof(tCanMessageExt));
    oMessageExt.ui8Channel = 02;
    oMessageExt.ui8Length  = 8;

    tADTFCANData oMessageData;
    {
        memset(&oMessageData, 0x0, sizeof(tADTFCANData));
        oMessageData.sHeader.ui8Tag = tADTFCANData::MT_Data;
        oMessageData.sHeader.ui8Channel = 03;
        for (tUInt8 nData = 0; nData < 8; ++nData)
        {
            oMessageData.sData.aui8Data[nData] = nData;
        }
    }

    //create stream for std messages
    tUInt16 u16StreamIdStd=0;
    if (IS_FAILED(pFileWriter->CreateStream("rawcan",
                                            u16StreamIdStd,
                                            MEDIA_TYPE_CAN,
                                            MEDIA_SUBTYPE_CAN_RAW_MESSAGE,
                                            STREAMING_MEDIA_TYPE_GENERIC,
                                            STREAMING_SAMPLE_TYPE_CAN)))
    {
        pFileWriter->Close();
        IADTFFileWriter::Release(pFileWriter);
        return 0;
    }

    //create stream for ext message
    tUInt16 u16StreamIdExt=0;
    if (IS_FAILED(pFileWriter->CreateStream("rawcan_ext",
                                            u16StreamIdExt,
                                            MEDIA_TYPE_CAN,
                                            MEDIA_SUBTYPE_CAN_RAW_MESSAGE_EXT,
                                            STREAMING_MEDIA_TYPE_GENERIC,
                                            STREAMING_SAMPLE_TYPE_CAN_EXT)))
    {
        pFileWriter->Close();
        IADTFFileWriter::Release(pFileWriter);
        return 0;
    }

    //create stream for tADTFCANData
    tUInt16 u16StreamIdCANData=0;
    if (IS_FAILED(pFileWriter->CreateStream("candata",
                                            u16StreamIdCANData,
                                            MEDIA_TYPE_CAN,
                                            MEDIA_SUBTYPE_CAN_RAW_MESSAGE,
                                            STREAMING_MEDIA_TYPE_GENERIC,
                                            STREAMING_SAMPLE_TYPE_CAN_DATA)))
    {
        pFileWriter->Close();
        IADTFFileWriter::Release(pFileWriter);
        return 0;
    }

    //set title
    tDateTime oDateTime;
    oDateTime.ui16Year          = 2009;
    oDateTime.ui16Month         =    4;
    oDateTime.ui16Day           =   29;
    oDateTime.ui16Hour          =   18;
    oDateTime.ui16Minute        =   30;
    oDateTime.ui16Second        =   10;
    oDateTime.ui32Microseconds  =    0;
    pFileWriter->SetMediaDescription("can test", "file with two can streams",&oDateTime);
    for (tInt nCounter = 0; nCounter < 99;  ++nCounter)
    {
        oMessage.ui16Id = nCounter;
        if (IS_FAILED(pFileWriter->Write(u16StreamIdStd,
                                         &oMessage,
                                         sizeof(tCanMessage),
                                         nCounter*10000)))
        {
            pFileWriter->Close();
            IADTFFileWriter::Release(pFileWriter);
            return 0;
        }
    }

    for (tInt nCounter = 99; nCounter < 199;  ++nCounter)
    {
        oMessageExt.ui32Id = nCounter;
        if (IS_FAILED(pFileWriter->Write(u16StreamIdExt,
                                         &oMessageExt,
                                         sizeof(tCanMessageExt),
                                         nCounter*10000)))
        {
            pFileWriter->Close();
            IADTFFileWriter::Release(pFileWriter);
            return 0;
        }
    }

    for (tInt nCounter = 199; nCounter < 299;  ++nCounter)
    {
        if (IS_FAILED(pFileWriter->Write(u16StreamIdCANData,
                                         &oMessageData,
                                         sizeof(tADTFCANData),
                                         nCounter*10000)))
        {
            pFileWriter->Close();
            IADTFFileWriter::Release(pFileWriter);
            return 0;
        }
    }

    pFileWriter->Close();
    IADTFFileWriter::Release(pFileWriter);

    return 0;
}

