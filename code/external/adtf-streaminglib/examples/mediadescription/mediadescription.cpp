/**
 *
 *  Small example program for adtf streaming lib
 *  Writes a file using the media description language
 *  Reads this file using the media description language
 *
 * Demonstrates:
 * - creating and releasing a IADTFDileReader/IADTFDileWriter
 * - opening and closing a dat file
 * - creating streams with a media description
 * - getting the media description for a stream
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: axasil2 $
 * $Date: 2013-03-14 10:47:58 +0100 (Thu, 14 Mar 2013) $
 * $Revision: 19131 $
 *
 * @remarks
 *
 */

#include "mediadescription.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <adtf_streaming.h>

using namespace adtfstreaming;
using namespace std;


tResult ReadStreamDescriptionFromFile(const string& strFile, string& strDescription)
{
    strDescription = "";

    ifstream oDescFile(strFile.c_str());
    if (oDescFile.is_open())
    {
        while (!oDescFile.eof())
        {
            string strLine;
            getline(oDescFile, strLine);
            strDescription += strLine;
        }
        oDescFile.close();
        RETURN_NOERROR;
    }
    else
    {
        RETURN_ERROR(ERR_INVALID_ARG);
    }
}


tResult WriteDatFile(const string& strPath, const string& strDatFilename)
{
    string strDatFile = strPath + "/" + strDatFilename;

    IADTFFileWriter* pFileWriter = IADTFFileWriter::Create();
    RETURN_IF_POINTER_NULL(pFileWriter);

    RETURN_IF_FAILED(pFileWriter->Open(strDatFile.c_str()));

    tUInt16 nStreamId1 = 0;
    RETURN_IF_FAILED(pFileWriter->CreateStream("test_serialize_basic", nStreamId1, 0, 0));

    string strDescription1;
    RETURN_IF_FAILED(ReadStreamDescriptionFromFile(strPath + "/stream1_media_desc.xml", strDescription1));
    RETURN_IF_FAILED(pFileWriter->SetStreamMediaDescription(nStreamId1, strDescription1.c_str()));

    tUInt16 nStreamId2 = 0;
    RETURN_IF_FAILED(pFileWriter->CreateStream("test_serialize_complex", nStreamId2, 0, 0));

    string strDescription2;
    RETURN_IF_FAILED(ReadStreamDescriptionFromFile(strPath + "/stream2_media_desc.xml", strDescription2));
    RETURN_IF_FAILED(pFileWriter->SetStreamMediaDescription(nStreamId2, strDescription2.c_str()));

    tUInt16 nStreamId3 = 0;
    RETURN_IF_FAILED(pFileWriter->CreateStream("no_description", nStreamId3, 0, 0));

    for (tUInt16 nIdx = 0; nIdx < 2500; nIdx++)
    {
        tMyType sType1;
        sType1.nTestVal1 = nIdx;
        sType1.nTestVal2 = nIdx + 1;
        RETURN_IF_FAILED(pFileWriter->Write(nStreamId1, (tVoid*)&sType1, sizeof(sType1), 15000 * nIdx));

        tMySecondType sType2;
        sType2.nTestVal1 = nIdx;
        sType2.nTestVal2 = nIdx + 1;
        sType2.nTestVal3 = nIdx + 2;
        sType2.nTestVal4 = nIdx + 3;
        sType2.nTestVal5 = nIdx + 4;
        sType2.fTestVal6 = nIdx * 0.375;
        RETURN_IF_FAILED(pFileWriter->Write(nStreamId2, (tVoid*)&sType2, sizeof(sType2), 15000 * nIdx + 7500));
    }

    //Close also writes a description file
    RETURN_IF_FAILED(pFileWriter->Close());
    IADTFFileWriter::Release(pFileWriter);

    RETURN_NOERROR;
}


tResult ReadDatFileWithStreamingLib(const string& strPath, const string& strFilename, tBool& bIsCorrect)
{
    bIsCorrect = tTrue;
    string strDatFile(strPath + "/" + strFilename);

    IADTFFileReader* pFileReader = IADTFFileReader::Create();
    RETURN_IF_POINTER_NULL(pFileReader);

    //Open also loads corresponding a media description (e.g. <dat_file_name.dat>.description)
    RETURN_IF_FAILED(pFileReader->Open(strDatFile.c_str()));

    cADTFDataBlock* pDataBlock;
    while(IS_OK(pFileReader->Read(&pDataBlock)))
    {
        tVoid* pData = NULL;
        tUInt64 szData = pDataBlock->GetData((const tVoid**)&pData);
        if (sizeof(tMyType) == szData)
        {
            tMyType* pMyType = (tMyType*)pData;
            bIsCorrect = bIsCorrect && (pMyType->nTestVal1 + 1 == pMyType->nTestVal2);
        }
        else if (sizeof(tMySecondType) == szData)
        {
            tMySecondType* pMySecondType = (tMySecondType*)pData;
            bIsCorrect = bIsCorrect && (pMySecondType->nTestVal1 + 1 == pMySecondType->nTestVal2);
            bIsCorrect = bIsCorrect && (pMySecondType->nTestVal1 + 2 == pMySecondType->nTestVal3);
            bIsCorrect = bIsCorrect && (pMySecondType->nTestVal1 + 3 == pMySecondType->nTestVal4);
            bIsCorrect = bIsCorrect && (pMySecondType->nTestVal1 + 4 == pMySecondType->nTestVal5);
            bIsCorrect = bIsCorrect && (pMySecondType->nTestVal1 * 0.375 == pMySecondType->fTestVal6);
        }
        else
        {
            bIsCorrect = tFalse;
        }
    }

    RETURN_IF_FAILED(pFileReader->Close());
    IADTFFileReader::Release(pFileReader);

    RETURN_NOERROR;
}

tResult ReadStreamMediaDescription(const string& strPath, const string& strFilename)
{
    string strDatFile(strPath + "/" + strFilename);

    IADTFFileReader* pFileReader = IADTFFileReader::Create();
    RETURN_IF_POINTER_NULL(pFileReader);

    //Open also loads corresponding a media description (e.g. <dat_file_name.dat>.description)
    RETURN_IF_FAILED(pFileReader->Open(strDatFile.c_str()));

    tInt nStreamCount = 0;
    RETURN_IF_FAILED(pFileReader->GetStreamCount(nStreamCount));

    for (tInt nIdx = 0; nIdx < nStreamCount; nIdx++)
    {
        const tChar* pDesc = pFileReader->GetStreamMediaDescription(nIdx);
        if (NULL != pDesc)
        {
            stringstream oStringStream;
            oStringStream << strPath.c_str() << "output_media_desc_stream_" << nIdx << ".xml";
            
            ofstream oOutputFile(oStringStream.str().c_str());
            oOutputFile << pDesc;
            oOutputFile.close();
        }
    }


    RETURN_IF_FAILED(pFileReader->Close());
    IADTFFileReader::Release(pFileReader);

    RETURN_NOERROR;
}


int main(int argc, char* argv[])
{
    if (3 != argc)
    {
        cout << "usage: mediadescription.exe <working_path> <datfile>\n";
        return 0;
    }

    string strPath(argv[1]);
    string strDatFilename(argv[2]);

    if (IS_OK(WriteDatFile(strPath, strDatFilename)))
    {
        tBool bCorrect = tTrue;
        if (IS_OK(ReadDatFileWithStreamingLib(strPath, strDatFilename, bCorrect)))
        {
            if (bCorrect)
            {
                cout << "Data in DAT file is correct.\n";
            }
            else
            {
                cout << "Data in DAT file is incorrect.\n";
            }

            if (IS_FAILED(ReadStreamMediaDescription(strPath, strDatFilename)))
            {
                cout << "Error while reading media descriptions.";
                return -1;
            }
        }
        else
        {
            cout << "Error while reading DAT file.\n";
            return -1;
        }
    }
    else
    {
        cout << "Error while writing DAT file.\n";
        return -1;
    }

    cout << "Writing and Reading DAT file was processed correctly.\n";
    return 0;
}

