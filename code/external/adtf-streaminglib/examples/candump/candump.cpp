/**
*
*  Small example program for adtf streaming lib
*  Displays information about file and stream, then
*  prints out can data from stream "can_raw"
*
* Demonstrates:
* - creating and releasing a IADTFDileReader
* - opening and closing a dat file
*   - retrieving file information
*   - selecting a stream
*   - retrieving data from stream (in this case, can messages)
*
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
*
 * $Author: voigtlpi $
 * $Date: 2012-11-09 21:04:54 +0100 (Fri, 09 Nov 2012) $
 * $Revision: 17457 $
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

    IADTFFileReader* pFileReader = IADTFFileReader::Create();
    if (2 != argc)
    {
        IADTFFileReader::Release(pFileReader);
        printf("usage: can_dump.exe datfile\n");
        return 0;
    }
    if (IS_FAILED(pFileReader->Open(argv[1])))
    {
        printf("FAILED to open FILE\n");
        IADTFFileReader::Release(pFileReader);
        return 0;
    }
    //Get media descriptor of file
    // the media descriptor contains the descriptions set during record as well as the creation date
    const tADTFMediaDescriptor *pMediaDescriptor = pFileReader->GetMediaDescriptor();
    cout << "[name] " << pMediaDescriptor->strFilename << endl;
    cout << "[short desc.] " << pMediaDescriptor->strShortDesc << endl;
    cout << "[long desc.] " << pMediaDescriptor->strLongDesc << endl;
    cout << "[date and time]" << pMediaDescriptor->strDateTime << endl;


    //Get file extensions if available
    int nExtensionCount = pFileReader->GetExtensionCount();
    const cADTFFileExtension *pExtension = NULL;
    for (tInt nCounter = 0; nCounter < nExtensionCount; nCounter++)
    {
        pFileReader->GetExtension(nCounter, &pExtension);
        if (NULL != pExtension)
        {
            cout << "extension identifier: " << pExtension->GetIdentifier() << endl;
        }

    }

    //Get streams
    tInt nCanStream = -1;
    tInt nStreamCount = 0;

    pFileReader->GetStreamCount(nStreamCount);
    //block infos
    tUInt64 n64DataBlockCount = pFileReader->GetDataBlockCount();
    tUInt64 n64IndexBlockCount = pFileReader->GetIndexBlockCount();

    cout << argv[1] << " contains " << nStreamCount << " streams" << endl;
    cout << argv[1] << " contains " << n64DataBlockCount << " data blocks" << endl;
    cout << argv[1] << " contains " << n64IndexBlockCount << " index blocks" << endl;

    for (int nCounter = 1; nCounter <= ADTF_STREAMING_MAX_INDEXED_STREAMS; nCounter++)
    {
        // note that streams start at 1, since stream 0 is used as "all streams"
        const tChar *strName = pFileReader->GetStreamName(nCounter);
        const tADTFStreamDescriptor *pStreamDescriptor = pFileReader->GetStreamDescriptor(nCounter);

        if (NULL != pStreamDescriptor)
        {
            switch (pStreamDescriptor->nType)
            {
                case tADTFStreamDescriptor::SD_CAN:
                {
                    cout << "CAN Stream: ";
                    break;
                }
                case tADTFStreamDescriptor::SD_VIDEO:
                {
                    cout << "Video Stream: ";
                    cout << "width = " << pStreamDescriptor->uData.sVideoFormat.sBitmapFormat.nWidth;
                    cout << "height = " << pStreamDescriptor->uData.sVideoFormat.sBitmapFormat.nHeight;
                    cout << "bpp = " << pStreamDescriptor->uData.sVideoFormat.sBitmapFormat.nBitsPerPixel;
                    cout << " : ";
                    break;
                }
                default:
                    break;
            }
        }

        if (NULL != strName)
        {
            cout << strName << endl;
            // we search for a can string with the name raw_can
            if (0 == strcmp(strName, "rawcan")  ||  0 == strcmp(strName, "can_raw"))
            {
                nCanStream = nCounter;
            }
        }
    }
    
    //check wether we found our stream
    if (-1 != nCanStream)
    {
        //now wait for user to push a button
        cout << "press any key to start processing...";
        getchar();


        cADTFDataBlock *pDataBlock = NULL;
        tInt nMessageCounter = 0;
        //read in datablocks
        while (IS_OK(pFileReader->Read(&pDataBlock)))
        {
            //if it is a canblock, show on console
            if (pDataBlock->GetStreamId() == nCanStream)
            {
                nMessageCounter++;
                //get the time the block was received as a sample
                tTimeStamp tsReceived = pDataBlock->GetTime();
                //get data from datablock
                tCanMessage *pCanMessage = NULL;
                //get data from block
                pDataBlock->GetData((const tVoid**)&pCanMessage);
                //show can message
                cout << "#" << nMessageCounter << " -- " << tsReceived << " : C" << (int)pCanMessage->ui8Channel << " ID=" << hex << pCanMessage->ui16Id << dec << "[";
                for (int nCounter = 0; nCounter < pCanMessage->ui8Length; nCounter++)
                {
                    cout << " " << hex << (int)pCanMessage->pui8Data[nCounter] << dec;
                }
                cout << "]" << endl;
            }
            cout << "current block position: " << pFileReader->GetCurrentBlockPos() << endl;
        }
    }
    else
    {
        //we did not find the can stream so we leave
        cout << "stream rawcan not found, aborting..." << endl;
    }
    //clean up
    pFileReader->Close();
    IADTFFileReader::Release(pFileReader);
    return 0;
}

