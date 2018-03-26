/**
 *
 * ADTF File Access example
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: voigtlpi $
 * $Date: 2014-10-31 14:01:50 +0100 (Fri, 31 Oct 2014) $
 * $Revision: 30555 $
 *
 * @remarks
 *
 */

#include <adtf_streaming.h>

using namespace adtfstreaming;

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <map>

#define WORKING_FILE_PATH_SIZE 256

#ifdef __linux
    #define strcpy_s(dest, size, source) strcpy(dest, source)
#endif


using namespace std;


FILE* _fopen(const tChar* strFileName, const tChar* mode)
{
#ifdef WIN32
    FILE* pFileHandle = NULL;
    if (0 == fopen_s(&pFileHandle, strFileName, mode))
    {
        return pFileHandle;
    }
#else
    return fopen(strFileName, mode);
#endif
    return NULL;
}

tResult QueryFileInfo(const tChar* strFilename)
{
    // open file
    IADTFFileReader::SetLogLevel(eLOG_LVL_ERROR);
    IADTFFileReader* pFileReader = IADTFFileReader::Create();

    if (IS_FAILED(pFileReader->Open(strFilename)))
    {
        IADTFFileReader::Release(pFileReader);
        RETURN_ERROR(ERR_FILE_NOT_FOUND);
    }

    cout << "File: " << strFilename << endl;

    const tADTFMediaDescriptor* pDescriptor = pFileReader->GetMediaDescriptor();

    tInt nStreamCount = 0;
    pFileReader->GetStreamCount(nStreamCount);

    cout << endl << "File Header" << endl;
    cout << "------------------------------------------------------------------------------" << endl;
    cout << "Date              : " << pDescriptor->strDateTime << endl;
    cout << "Duration          : " << pFileReader->GetTimeDuration() << endl;
    cout << "Short description : " << pDescriptor->strShortDesc << endl;
    cout << "Long description  : " << pDescriptor->strLongDesc << endl;
    cout << "Chunk count       : " << pFileReader->GetDataBlockCount() << endl;
    cout << "Extension count   : " << pFileReader->GetExtensionCount() << endl;
    cout << "Stream count      : " << nStreamCount << endl;

    cout << endl << "Streams" << endl;
    cout << "------------------------------------------------------------------------------" << endl;

    for (tInt nIdx = 1;  nIdx <= ADTF_STREAMING_MAX_INDEXED_STREAMS;  ++nIdx)
    {
        const tADTFStreamDescriptor* pStreamDescriptor = pFileReader->GetStreamDescriptor(nIdx);
        if (pStreamDescriptor != NULL)
        {
            std::string strType;
            switch (pStreamDescriptor->nType)
            {
            case tADTFStreamDescriptor::SD_AUDIO:
                strType = "Audio";
                break;
            case tADTFStreamDescriptor::SD_CAN:
                strType = "CAN";
                break;
            case tADTFStreamDescriptor::SD_COMMAND:
                strType = "Command";
                break;
            case tADTFStreamDescriptor::SD_INFO:
                strType = "Info";
                break;
            case tADTFStreamDescriptor::SD_FLEXRAY:
                strType = "Flexray";
                break;
            case tADTFStreamDescriptor::SD_NETWORK_DATA:
                strType = "Network data";
                break;
            case tADTFStreamDescriptor::SD_OTHER:
                strType = "Other";
                break;
            case tADTFStreamDescriptor::SD_REFERENCE:
                strType = "Reference";
                break;
            case tADTFStreamDescriptor::SD_STRUCTURED_DATA:
                strType = "Structured Data";
                break;
            case tADTFStreamDescriptor::SD_USER:
                strType = "User";
                break;
            case tADTFStreamDescriptor::SD_VIDEO:
                strType = "Video";
                break;
            case tADTFStreamDescriptor::SD_COMPRESSED_VIDEO:
            {
                std::ostringstream oStream;
                oStream << "Compressed Video (";
                if (pStreamDescriptor->uData.sCompressedVideoFormat.bValid)
                {
                    oStream << "Version: " << pStreamDescriptor->uData.sCompressedVideoFormat.nVersion
                            << ", Codec: " << pStreamDescriptor->uData.sCompressedVideoFormat.strCodec;
                }
                else
                {
                    oStream << "no codec info avaliable";
                }
                oStream << ")";
                strType = oStream.str();
                break;
            }
            case tADTFStreamDescriptor::SD_NOT_INIT:
                strType = "Not Init";
                break;
            default:
                strType = "Invalid";
                break;
            }

            cout << "Stream #" << nIdx << " : " << pFileReader->GetStreamName(nIdx) << endl;
            cout << "    Type           : " << strType << endl;
            cout << "    Major-/Subtype : " << pStreamDescriptor->nMajorType << "/"
                                            << pStreamDescriptor->nSubType << endl;
            cout << "    Sampletype     : " << pStreamDescriptor->strSampleType << endl;
            cout << "    Mediatype      : " << pStreamDescriptor->strMediaType << endl << endl;
        }
    }

    // close file
    pFileReader->Close();
    IADTFFileReader::Release(pFileReader);

    RETURN_NOERROR;
}



class cLastTimes 
{
    typedef map<tUInt16, tTimeStamp> tLastTimes;

    public:
        cLastTimes(){}
        ~cLastTimes(){}
        tTimeStamp GetDiffToLastChunkTime(const tUInt16& ui16Stream, const tTimeStamp& tmCurrentTime)
        {
            return GetLastTimeStamp(m_mapLastChunkTime, ui16Stream, tmCurrentTime);
        }
        tTimeStamp GetDiffToLastStreamTime(const tUInt16& ui16Stream, const tTimeStamp& tmCurrentTime)
        {
            return GetLastTimeStamp(m_mapLastStreamTime, ui16Stream, tmCurrentTime);
        }
    private:
        tTimeStamp GetLastTimeStamp(tLastTimes& mapLastTimes,
                                    const tUInt16& ui16Stream,
                                    const tTimeStamp& tmCurrentTime)
        {
            tTimeStamp tmResult = -1;
            tLastTimes::iterator it = mapLastTimes.find(ui16Stream);
            if (it != mapLastTimes.end())
            {
                tmResult = tmCurrentTime - it->second;
                it->second = tmCurrentTime;
            }
            else
            {
                mapLastTimes[ui16Stream] = tmCurrentTime;
            }
            return tmResult;
        }
        tLastTimes m_mapLastChunkTime;
        tLastTimes m_mapLastStreamTime;
};


/// The Hardware Time ./ Device Time.
/// While using this a microseconds-value-resolution should be used.
#define     Additional_DeviceOriginalTime       0
/// A Counter.
#define     Additional_Counter                  1

tResult AccessFileData(const tChar* strFilename, const tChar* strCSVFilePath)
{
    IADTFFileReader* pFileReader = IADTFFileReader::Create();
    cLastTimes oRememberTimes;
    if (IS_FAILED(pFileReader->Open(strFilename)))
    {
        IADTFFileReader::Release(pFileReader);
        RETURN_ERROR(ERR_FILE_NOT_FOUND);
    }

    cout << endl << "File data" << endl;
    cout << "------------------------------------------------------------------------------" << endl;


    FILE* hCSVFile = _fopen(strCSVFilePath, "w+");
    if (NULL == hCSVFile)
    {
        cout << endl << "Please close the csv file!" << endl;
        RETURN_ERROR(ERR_NOACCESS);
    }

    // set the labels
    fprintf(hCSVFile, "%s", "stream;stream_name;chunk_time;mediasample_time;device_time;chunk_time_delta_to_lastofstream;media_time_delta_to_lastofstream\n");

    cADTFDataBlock *pDataBlock = NULL;
    tResult nRet = ERR_NOERROR;
    while (nRet == ERR_NOERROR)
    {
        nRet = pFileReader->Read(&pDataBlock);
        if (IS_OK(nRet))
        {
            fprintf(hCSVFile, "%d;%s;%lld;%lld;%lld;%lld;%lld\n",
                                pDataBlock->GetStreamId(),
                                pFileReader->GetStreamName(pDataBlock->GetStreamId()),
                                (long long int) pDataBlock->GetFileTime(),
                                (long long int) pDataBlock->GetTime(),
                                (long long int) pDataBlock->GetAdditionalTimestamp(Additional_DeviceOriginalTime, -1),
                                (long long int) oRememberTimes.GetDiffToLastChunkTime(pDataBlock->GetStreamId(), pDataBlock->GetFileTime()),
                                (long long int) oRememberTimes.GetDiffToLastStreamTime(pDataBlock->GetStreamId(), pDataBlock->GetTime()));
        }
    }

    fclose(hCSVFile);
    hCSVFile = NULL;

    // close file
    pFileReader->Close();
    IADTFFileReader::Release(pFileReader);
    RETURN_NOERROR;
}

tResult Decompress(const tChar* strFilename, tChar* strOutputFile)
{
    std::string strWork = strFilename;
    tResult nResult = ERR_NOERROR;

    // '.bz2'
    if (strWork.length() < 5)
    {
        strcpy_s((tChar *)strOutputFile, WORKING_FILE_PATH_SIZE, strWork.c_str());
        return nResult;
    }
    std::string strExtension = strWork.substr(strWork.length() - 4, 4);
    if (0 == _stricmp(strExtension.c_str(), ".bz2"))
    {
        // file ends with '.bz2'
        strWork = strWork.substr(0, strWork.length() - 4);
        nResult = IADTFFileCompression::Decompress(strFilename, strWork.c_str(), IADTFFileCompression::CA_Bzip2);
    }

    strcpy_s((tChar *)strOutputFile, WORKING_FILE_PATH_SIZE, strWork.c_str());
    return nResult;
}

int main(int argc, char* argv[])
{
    if (argc < 3 || argv[1] == NULL || argv[2] == NULL)
    {
        cout << "usage: " << argv[0] << " DAT-File CSV-File"  << endl;
        return -1;
    }

    tChar strWorkingFile[WORKING_FILE_PATH_SIZE];
    if (IS_FAILED(Decompress(argv[1], strWorkingFile)))
    {
        cout << "could not decompress the file " << argv[1] << endl;
        return -2;
    }

    if (IS_FAILED(QueryFileInfo(strWorkingFile)))
    {
        cout << "could not query info of file " << argv[1] << endl;
        return -2;
    }

    if (IS_FAILED(AccessFileData(strWorkingFile, argv[2])))
    {
        cout << "could not access data of file " << argv[1] << endl;
        return -2;
    }
    return 0;
}
