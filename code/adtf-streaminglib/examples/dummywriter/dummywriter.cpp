/**
*
*  Small example program for adtf streaming lib
*  Creates a dat file with two streams
*
* Demonstrates:
* - creating and releasing a IADTFFileWriter
* - opening and closing a dat file
*   - creating a stream with basic sample type
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
    if (IS_FAILED(pFileWriter->Open("test.dat")))
    {
        printf("FAILED to open FILE\n");
        IADTFFileWriter::Release(pFileWriter);
        return 0;
    }

    //create stream for std messages
    tUInt16 u16StreamIdU32=0;
    RETURN_IF_FAILED(pFileWriter->CreateStream("struct uint32",u16StreamIdU32,MEDIA_TYPE_STRUCTURED_DATA,MEDIA_SUBTYPE_STRUCT_UINT32));

    //create stream for ext message
    tUInt16 u16StreamIdF64=0;
    RETURN_IF_FAILED(pFileWriter->CreateStream("struct float64",u16StreamIdF64,MEDIA_TYPE_STRUCTURED_DATA,MEDIA_SUBTYPE_STRUCT_FLOAT64));

    //set title
    tDateTime oDateTime;
    oDateTime.ui16Year          = 2009;
    oDateTime.ui16Month         =    4;
    oDateTime.ui16Day           =   29;
    oDateTime.ui16Hour          =   18;
    oDateTime.ui16Minute        =   30;
    oDateTime.ui16Second        =   10;
    oDateTime.ui32Microseconds  =    0;
    pFileWriter->SetMediaDescription("can test", "file with two structured streams",&oDateTime);
    tUInt32 u32Value = 0;
    tFloat64 f64Value = 0;
    for (tInt nCounter = 0; nCounter < 99;  ++nCounter)
    {
        RETURN_IF_FAILED(pFileWriter->Write(u16StreamIdU32,&u32Value,sizeof(tUInt32),
            nCounter*10000));
        RETURN_IF_FAILED(pFileWriter->Write(u16StreamIdF64,&f64Value,sizeof(tFloat64),
            nCounter*10000+500));
        ++u32Value;
        ++f64Value;
    }


    RETURN_IF_FAILED(pFileWriter->Close());

    IADTFFileWriter::Release(pFileWriter);

    return 0;
}
