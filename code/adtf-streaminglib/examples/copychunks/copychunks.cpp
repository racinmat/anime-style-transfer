/**
*
*  Small example program for adtf streaming lib
*  Copies data chunks from one DAT file to another one.
*
* Demonstrates:
* - creating and releasing a IChunkCopy
* - opening and closing a dat file
*   - copying data chunks
*
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
*
 * $Author: axasil2 $
 * $Date: 2013-03-15 09:38:08 +0100 (Fri, 15 Mar 2013) $
 * $Revision: 19146 $
*
* @remarks
*
*/

#include <adtf_streaming.h>
#include <stdio.h>
#include <iostream>

using namespace adtfstreaming;
using namespace std;

int main(int argc, char* argv[])
{
    //initialize the Copier Instance
    IChunkCopy* pCopier = IChunkCopy::Create();
    if (3 != argc)
    {
        IChunkCopy::Release(pCopier);
        printf("usage: copychunks.exe <source-file> <dest-file>\n");
        return 1;
    }

    //open the file to copy stream parts from
    if (IS_FAILED(pCopier->Open(argv[1])))
    {
        printf("FAILED to open SOURCE FILE\n");
        IChunkCopy::Release(pCopier);
        return 2;
    }

    //create the new where to copy the streams to
    if (IS_FAILED(pCopier->CreateFile(argv[2])))
    {
        printf("FAILED to create DESTINATION FILE\n");
        IChunkCopy::Release(pCopier);
        return 2;
    }

    // this example will only copy a stream that is called "Video"
    const tChar * strStreamName = "video";
    tUInt16 ui16StreamId = 0;
    if (IS_FAILED(pCopier->GetStreamId(strStreamName, ui16StreamId)))
    {
        printf("FAILED to resolve STREAM ID\n");
        IChunkCopy::Release(pCopier);
        return 3;
    }
    
    // copy the Stream Information to the new file with a new stream id and a different name
    // you can also add more than one stream at once!
    if (IS_FAILED(pCopier->AddStreamToCopy(ui16StreamId,
                                           1, 
                                           "new_video_stream")))
    {
        printf("FAILED to create STREAM\n");
        IChunkCopy::Release(pCopier);
        return 4;
    }

    //copy all data blocks of the streams you added with AddStreamToCopy
    // you can also seek to a special position and copy from there
    while (IS_OK(pCopier->CopyNext()))
    {
       printf("current block position: %i\n", static_cast<tInt>(pCopier->GetCurrentBlockPos()));
    }

    //clean up
    if (IS_FAILED(pCopier->Close()))
    {
        printf("FAILED to close files properly\n");
        IChunkCopy::Release(pCopier);
        return 5;
    }
    // destroy the copier instance
    IChunkCopy::Release(pCopier);
    return 0;
}

