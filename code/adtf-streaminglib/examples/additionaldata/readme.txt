/**
 *
 * Demo to read or write additional data to a stream
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved.
 *
 * @author               $Author: voigtlpi $
 * @date                 $Date: 2009-07-16 15:36:37 +0200 (Do, 16 Jul 2009) $
 * @version              $Revision: 10093 $
 *
 * @remarks
 *
 */
 
/**
 * \page page_additionaldata Additional Data Example
 *
 * Implements an example program that writes and reads additional data to a stream of a DAT File 
 * with the help of the media type description.
 * With the service 'media_type_service' you can see, how to implement a own mediatype with additional data.
 * BUT: The code of the service can only be uses with ADTF.
 * 
 * Also XCP Data uses this Additional Data.
 * How to copy the XCP Data stream with the help of IADTFFileWriter and IADTFFileReader can be seen
 * in the following code snippet.
 * \code 
 *  //...
    const tChar* strName = pFileReader->GetStreamName(uiStreamIdx);
    if (strName != NULL)
    {
        cString strStreamName = strName;
        if (strStreamName.IsNotEmpty())
        {
            const tVoid* pData = NULL;
            tSize szData = 0;
            //this can return with error return value if no additional data exists, we do not check
            pFileReader->GetStreamAdditionalData(uiStreamIdx, &pData, &szData);
            const tADTFStreamDescriptor* pDesc = pFileReader->GetStreamDescriptor(uiStreamIdx);
            //this call works for every stream type .. also video, audio and video compressed
            __adtf_test_result(pFileWriter->CreateStream(strStreamName.GetPtr(),
                                                         uiStreamIdx,
                                                         pDesc->nMajorType,
                                                         pDesc->nSubType,
                                                         pDesc->strMediaType,
                                                         pDesc->strSampleType,                                                                
                                                         NULL,
                                                         pData,
                                                         tInt(szData)));
        }
    }
   //...
 * \endcode  
 * 
 * Remember to run this example on windows the streaming lib dll must be in your path.
 * 
 * \par Location
 * \code
 *    ./examples/additionaldata/
 * \endcode
 *
 * \par This example shows:
 * \li how to read additional data from a DAT File for the streams.
 * \li how to write additional data to a stream of a DAT File.
 * \li how to write streams with mediatypes of a created media type
 * 
 * \par The Implementation for the Additional Data Example
 * \include additionaldata.cpp
 *
 */