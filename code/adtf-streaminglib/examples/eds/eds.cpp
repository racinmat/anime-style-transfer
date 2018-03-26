/**
 *
 *  Small example program for adtf streaming lib
 *  Extract EDS Data from a DAT File to a directory.
 *  Writes EDS Data to a DAT File.
 *  Checks if the EDS Data exists in the DAT File.
 *
 * Demonstrates:
 * - creating and releasing a IADTFDileReader/IADTFDileWriter
 * - opening and closing a dat file
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


tResult AddEDSData(const tChar* strEDSPath, const tChar* strEDSFilename, const tChar* strFile, const tChar* strFileDir, const tChar* strDatFileName)
{
    IADTFFileWriter* pFileWriter = IADTFFileWriter::Create();

    // open dat file
    if (IS_FAILED(pFileWriter->Open(strDatFileName)))
    {
        IADTFFileWriter::Release(pFileWriter);
        RETURN_ERROR(ERR_OPEN_FAILED);
    }    
    
    // write stream to dat-file
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
    pFileWriter->SetMediaDescription("EDS test", "file with two streams and eds extension data",&oDateTime);
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
    
    // add a single file
    tResult nRes1 = pFileWriter->AddExtendedData(strEDSPath, strEDSFilename, strFile, "testfile comment", "testfile version");
    if (IS_FAILED(nRes1))
    {
        IADTFFileWriter::Release(pFileWriter);
        RETURN_ERROR(nRes1);
    }

    // add a complete directory
    tResult nRes2 = pFileWriter->AddExtendedDataDir(strEDSPath, strFileDir, "testdir comment", "testdir version");
    if (IS_FAILED(nRes2))
    {
        IADTFFileWriter::Release(pFileWriter);
        RETURN_ERROR(nRes2);
    }

    //save data to file
    tResult nRes3 = pFileWriter->AddEDSExtension(0);
    if (IS_FAILED(nRes3))
    {
        IADTFFileWriter::Release(pFileWriter);
        RETURN_ERROR(nRes3);
    }

    // close file
    pFileWriter->Close();
    IADTFFileWriter::Release(pFileWriter);

    RETURN_NOERROR;
}

tResult ExtractEDS(const tChar* strFilename, const tChar* strDesDir)
{
    // create file reader
    IADTFFileReader* pFileReader = IADTFFileReader::Create();

    // open dat file
    if (IS_FAILED(pFileReader->Open(strFilename)))
    {
        IADTFFileReader::Release(pFileReader);
        RETURN_ERROR(ERR_OPEN_FAILED);
    }

    cout << "File: " << strFilename << endl;

    if (IS_FAILED(pFileReader->ExtractEDSDataFromDAT(strFilename, strDesDir, 0)))
    {
        IADTFFileReader::Release(pFileReader);
        RETURN_ERROR(ERR_INVALID_FILE);
    }

    // close file
    pFileReader->Close();
    IADTFFileReader::Release(pFileReader);

    RETURN_NOERROR;
}


int main(int argc, char* argv[])
{
    tChar* strUsage(argv[1]);
    tInt nUsage = 0;

    if (strUsage != NULL && strcmp(strUsage, "extract") == 0)
    {
        if (4 != argc)
        {
            cout << "usage: eds.exe <use: extract> <datfilename> <destinationFolder>\n" << endl;
            return 0;
        }

        nUsage = 1;
    }
    else if (strUsage != NULL && strcmp(strUsage, "add") == 0)
    {
        if (7 != argc)
        {
            cout << "usage: eds.exe <use: add> <edsPath> <edsFilename> <strFile to add> <strFileDir> <datfilename to save the eds>\n" << endl;
            return 0;
        }

        nUsage = 2;
    }
    else
    {
        cout << "you have to define a usage to use this example: \n" << endl;
        cout << "extract or add\n" << endl;
        cout << "extract: eds.exe <use: extract> <datfilename> <destinationFolder>\n" << endl;
        cout << "add: eds.exe <use: add> <edsPath> <edsFilename> <strFile to add> <strFileDir> <datfilename to save the eds>\n" << endl;
        return 0;
    }

    if (nUsage == 1)
    {
        tChar* strFileName(argv[2]);
        tChar* strDestDir(argv[3]);

        tResult nRes = ExtractEDS(strFileName, strDestDir);

        if (nRes == ERR_INVALID_FILE)
        {
            cout << "file is invalid: " << argv[2] << endl;
            return -2;
        }
        else if (nRes < 0)
        {
            cout << "unknown error" << endl;
            return -2;
        }
    }
    else if (nUsage == 2)
    {
        tChar* strEDSPath(argv[2]);
        tChar* strEDSFilename(argv[3]);
        tChar* strFile(argv[4]);
        tChar* strFileDir(argv[5]);
        tChar* strDatfilename(argv[6]);

        tResult nRes = AddEDSData(strEDSPath, strEDSFilename, strFile, strFileDir, strDatfilename);

        if (nRes == ERR_INVALID_ARG)
        {
            cout << "not all arguments were correct" << endl;
            return -2;
        }
        else if (nRes == ERR_OPEN_FAILED)
        {
            cout << "could not open dat file" << endl;
            return -2;
        }
        else if (nRes < 0)
        {
            cout << "unknown error" << endl;
            return -2;
        }
    }

    cout << "Extracting or Adding the DAT File for EDS Data was successfully.\n";
    return 0;
}

