/**
 *
 *  Small example program for adtf streaming lib
 *  Writes additional data to a stream of a DAT File
 *  Reads additional data from a stream of a DAT File
 *
 * Demonstrates:
 * - creating and releasing a IADTFDileReader/IADTFDileWriter
 * - opening and closing a dat file
 * - creating streams with a media description
 * - writing the additional data to this stream
 * - reading the additional data from a stream
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

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <adtf_streaming.h>

using namespace adtfstreaming;
using namespace std;

tResult ReadAdditionalData(const tChar* strFilename)
{
    // create file reader
    IADTFFileReader* pFileReader = IADTFFileReader::Create();

    // open dat file
    if (IS_FAILED(pFileReader->Open(strFilename)))
    {
        IADTFFileReader::Release(pFileReader);
        RETURN_ERROR(ERR_FILE_NOT_FOUND);
    }

    tInt nStreamCount;
    pFileReader->GetStreamCount(nStreamCount);

    cout << "File: " << strFilename << endl;
    cout << "Stream count: " << nStreamCount << endl;

    for (tInt nIdx = 1;  nIdx <= nStreamCount;  ++nIdx)
    {
        string strStreamName;
        strStreamName = pFileReader->GetStreamName(nIdx);

        tUInt16 ui16StreamID;
        pFileReader->GetStreamId(strStreamName.c_str(), ui16StreamID);
        const tADTFStreamDescriptor* pDesc = pFileReader->GetStreamDescriptor(ui16StreamID);

        cout << "Stream #" << nIdx << " : " << strStreamName << endl;
        cout << "StreamID: " << ui16StreamID << endl;

        const tVoid* pAdditionalData;
        tSize szDataSize;
        
        if (IS_FAILED(pFileReader->GetStreamAdditionalData(ui16StreamID, &pAdditionalData, &szDataSize)))
        {
            cout << "No Additional Data found!" << endl;
        }
        else
        {
            cout << "Additional Data found: " << endl;

            if (strcmp(pDesc->strMediaType, "adtf.type.versioned") == 0)
            {
                const tUInt32* pui32Version = reinterpret_cast<const tUInt32*>(pAdditionalData);

                cout << "Version: " << *pui32Version << endl;
            }
            else if (strcmp(pDesc->strMediaType,"adtf.type.video") == 0)
            {
                const tBitmapFormat* psFormat = reinterpret_cast<const tBitmapFormat*>(pAdditionalData);

                cout << "PixelFormat: " << psFormat->nPixelFormat << endl;
            }
			else if (strcmp(pDesc->strMediaType,"adtf.type.xcp") == 0)
            {
                cout << "Found XCP Configuration: " << endl;
            }
        }
    }

    // close file
    pFileReader->Close();
    IADTFFileReader::Release(pFileReader);

    RETURN_NOERROR;
}


tResult WriteAdditionalData(const tChar* strFilename)
{
    // create file reader
    IADTFFileWriter* pFileWriter = IADTFFileWriter::Create();
    if (!pFileWriter)
    {
        RETURN_ERROR(ERR_UNEXPECTED);
    }

    // open dat file
    if (IS_FAILED(pFileWriter->Open(strFilename)))
    {
        IADTFFileWriter::Release(pFileWriter);
        RETURN_ERROR(ERR_OPEN_FAILED);
    }

    cout << "File: " << strFilename << endl;

    // this value is just for demonstration, use whatever you like
    tUInt32 nDataAdditional = 1234;

    // create stream with versioned media type and additional data.
    tUInt16 u16StreamId = 0;
    RETURN_IF_FAILED(pFileWriter->CreateStream("additional", u16StreamId, MEDIA_TYPE_USER + 0xabcd, 0, "adtf.type.versioned", STREAMING_SAMPLE_TYPE_GENERIC, 0,
                     &nDataAdditional, sizeof(nDataAdditional)));

    // close file
    pFileWriter->Close();
    IADTFFileWriter::Release(pFileWriter);

    RETURN_NOERROR;
}

int main(int argc, char* argv[])
{
    if (4 > argc)
    {
        cerr << "usage: " << argv[0] << " read|write <datfileName> [serviceName]\n";
        return -1;
    }

    string strUsage(argv[1]);
    string strDatFilename(argv[2]);

    if (0 == strcmp(strUsage.c_str(), "read"))
    {
        if (argc == 4)
        {
            // register service
            if (IS_FAILED(IADTFFileReader::AddTypeService(argv[3])))
            {
                cerr << "could not register service " << argv[3] << endl;
                return -1;
            }
        }
        
        //open datfile and read additional informations
        tResult strRes = ReadAdditionalData(strDatFilename.c_str());
        if (strRes == ERR_UNKNOWN)
        {

        }
        else if (strRes == ERR_FILE_NOT_FOUND)
        {
            cerr << "file not found" << argv[2] << endl;
            return -1;
        }
        else if (strRes < 0)
        {
            cerr << "unknown error" << endl;
            return -1;
        }
    }
    else if (0 == strcmp(strUsage.c_str(), "write"))
    {
        if (argc == 4)
        {
            // register service
            if (IS_FAILED(IADTFFileWriter::AddTypeService(argv[3])))
            {
                cerr << "could not register service " << argv[3] << endl;
                return -1;
            }
        }
        
        // open datfile and write additional data
        tResult strRes = WriteAdditionalData(strDatFilename.c_str());
        if (strRes == ERR_OPEN_FAILED)
        {
            cerr << "could not open file " << argv[2] << endl;
            return -1;
        }
        else if (strRes < 0)
        {
            cerr << "unknown error" << endl;
            return -1;
        }
    }
    else
    {
        cerr << "Unsupported operation" << endl;
        return -1;
    }

    cout << "Writing or Reading DAT file was processed correctly.\n";
    return 0;
}

