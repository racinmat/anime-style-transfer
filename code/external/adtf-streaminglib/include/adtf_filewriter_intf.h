/**
*
* ADTF File Writer Interface.
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
#ifndef _ADTF_STREAMING_WRITER_INTF_HEADER_
#define _ADTF_STREAMING_WRITER_INTF_HEADER_

namespace adtfstreaming
{

/**
 *  Interface and factory for dat-file writing.
 *  This class is a factory for dat-file writers. Furthermore it is the
 *  interface used by said writers. Use this class if you want to write 
 *  ADTF 2 compatible dat files.
 */
class DOEXPORT IADTFFileWriter
{
    public:
        virtual ~IADTFFileWriter();

    public: //support for additional classes
        /**
         * Add an additional class and its classfactory to the writer, 
         * e.g. in order to add more known sample types
         * Call before the first call to create!!!
         * @param strID     [in]        Identifier of class
         * @param pFactory  [in]        pointer to factory that creates the class
         *
         * @retval ERR_NOERROR if all went well
         * @retval ERR_POINTER if strId or pFactory was NULL
         * @retval ERR_UNEXPECTED if @ref IADTFFileWriter::Create() has been called before
         *
         */
        static tResult AddClass(const tChar* strID, tVoid* pFactory);
        /**
         * Add an additional service to be loaded, e.g. in order to add more known sample types
         * Call before the first call to create!!!
         *
         * @param strServiceFilename   [in]    filename of service to be loaded
         *
         * @retval ERR_NOERRROR if all went well
         * @retval ERR_POINTER if strServiceFilename was NULL
         * @retval ERR_UNEXPECTED if @ref IADTFFileWriter::Create() has been called before
         *
         */
        static tResult AddTypeService(const tChar* strServiceFilename);

    public: //factory methods
        /**
         * Creates a dat file writer. The function creates a dat file writer to create and
         * populate a new dat-file.
         *
         * @retval Interface Returns the interface to a dat file writer.
         * @retval NULL if something went wrong.
         *
         */
        static IADTFFileWriter* Create();

        /**
         *
         * Releases a dat file writer. This function releases a dat file writer and deletes it.
         *
         * @param pWriter [in] writer to be released
         *
         * @retval ERR_NOERROR if writer could be released.
         * @retval ERR_POINTER if pWriter is NULL
         *
         */
        static tResult  Release(IADTFFileWriter* pWriter);

    public: //access methods
        /**
         * Opens the ADTF file. 
         *
         * @param strFileName   [in] The Filename as null terminated string (relative or absolute
         * path)
         * @param ui32Flags     [in] Flags which describe the writing behavior (e.g. synchronous or
         * asynchronous). See \c adtfstreaming::tOpenMode to get all available flags.
         *
         * @retval ERR_NOERROR if all went well.
         * @retval ERR_POINTER if strFileName is NULL
         * @retval ERR_INVALID_ARG internal error
         * @retval ERR_MEMORY if a problem occurred while reserving memory for index table
         *
         * @remark The adtf writer can open the dat file only in writing access mode.
         *
         */
        virtual tResult Open(const tChar* strFileName, tUInt32 ui32Flags=0) = 0;

        /**
         *
         * Closes the ADTF file. The close method closes the dat file.
         * If there is a media description set for any stream this also writes the media description file for the DAT.
         * Don't forget to call @ref Release.
         *
         * @retval ERR_NOERROR if all went well
         * @retval ERR_UNEXPECTED if no file was open
         *
         */
        virtual tResult Close() = 0;

        /**
         *
         * This method writes the specified data to a dat file.
         *
         * @param ui16StreamId      [in] id of stream to write to. Stream must have been
         *                               created previously using @ref IADTFFileWriter::CreateStream
         * @param pvData            [in] pointer to data to write
         * @param i64DataSize       [in] size of data in byte (mind the real maximum of data 
         *                               size depends on compiler and system architecture using)
         * @param tmTimeStampData   [in] timestamp used both for media sample as for location in
         *                               dat file
         * @param ui16FlagsDataType [in] data type flag of written data
         * @param tmFileTimeStamp   [in] timestamp of chunk inside of dat-file. If -1, 
         *                               timestamp is calculated as rising time
         * @param ui32FlagsInternal [in] Sample flags @see ADTF MediaSample Flags Internals
         *
         * @note It is <b>legal</b> to set pvData to NULL <b>if and only if</b>
         *        u32DataSize is 0. This creates empty media samples with a timestamp.
         *
         * @retval ERR_NOERROR if all went well
         * @retval ERR_POINTER if pData is NULL and u32DataSize is greater than 0
         * @retval ERR_INVALID_ARG if nStreamId is 0
         * @retval ERR_UNKNOWN     if nStreamId does not belong to a known stream
         * @retval ERR_UNEXPECTED if no datfile created 
         * @retval ERR_OUT_OF_SYNC if nFileTimeStamp is specified and is less than the last
         *                             file time stamp. This is a sanity check: time does not flow 
         *                             backwards
         *
         */
        virtual tResult Write(tUInt16 ui16StreamId, 
                              const tVoid* pvData,
                              tInt64 i64DataSize, 
                              tTimeStamp tmTimeStampData,
                              tUInt16 ui16FlagsDataType=cADTFDataBlock::DTF_DATA,
                              tTimeStamp tmFileTimeStamp=-1,
                              tUInt32 ui32FlagsInternal=0) = 0;

 
        /**
         *
         * This method writes the specified data to a dat file.
         *
         * @param pBlock [in] reference to the block to write
         *
         * @note Additional information (like the file timestamp, flags, etc.)
         *       will be taken from the given data block \c pBlock.
         *
         * @retval ERR_NOERROR if all went well
         * @retval ERR_POINTER if pData is NULL and u32DataSize is greater than 0
         * @retval ERR_INVALID_ARG if nStreamId is 0
         * @retval ERR_UNKNOWN     if nStreamId does not belong to a known stream
         * @retval ERR_UNEXPECTED if no datfile created 
         * @retval ERR_OUT_OF_SYNC if nFileTimeStamp is specified and is less than the last
         *                         file time stamp. This is a sanity check: time does not flow 
         *                         backwards
         *
         */
        virtual tResult Write(cADTFDataBlock* pBlock) = 0;

        /** 
         *
         * This method creates a named stream inside the dat file. It returns a
         * stream handle which can be used to write to this stream using
         * @ref IADTFFileWriter::Write()
         *
         * @param strName       [in] name of stream, must be unique inside of file
         * @param ui16StreamId  [out] id of created stream. Invalid if return
         *                           value is not equal to ERR_NOERROR 
         * @param ui32MajorType [in]Major media type identificator of this stream
         * @param ui32SubType   [in]sub media type identificator of this stream
         * @param strMediaType  [in] string that identifies the mediatype used.
         *                          For a list of default mediatypes see
         *                          adtf_mediatypes.h
         * @param strSampleType [in] string specifying the type of mediasmaple to use
         *                          For a list of default mediatsampleypes see
         *                          adtf_mediatypes.h
         * @param pFormatData   [in] format data for audio or video pins. Use tAudioFormat or tVideoFormat here.
         * @param pAdditionalData [in] Pointer to additional data for the MediaType.
         * @param nAdditionalDataSize [in] The Size of the additional data.
         * 
         * @remark Most filters use the default media type and sample type but set
         *         a specific major and sub type. See the canwriter example, since
         *         can data uses a special media sample
         *
         *
         * @retval ERR_NOERROR if all went well
         * @retval ERR_INVALID_ARG if strName,strMediaType or strSampleType was empty
         * @retval ERR_POINTER if strName, strMediaType or strSampleType was NULL
         * @retval ERR_UNEXPECTED if a stream with this name already exists
         * @retval ERR_FAILED if either the sampletype or the mediatype could not be resolved.
         * @remarks If you are using non-standard types, load the appropiate class factories first.
         */
        virtual tResult CreateStream(const tChar* strName, 
                                     tUInt16& ui16StreamId,
                                     tUInt32 ui32MajorType, 
                                     tUInt32 ui32SubType,
                                     const tChar* strMediaType=STREAMING_MEDIA_TYPE_GENERIC, 
                                     const tChar* strSampleType=STREAMING_SAMPLE_TYPE_GENERIC,
                                     tVoid* pFormatData=NULL,
                                     const tVoid* pAdditionalData = NULL,
                                     tInt nAdditionalDataSize = 0) = 0;

        /**
         *
         * This function sets the media description.
         *
         * @param ui16StreamId        [in] id to look up.
         * @param strMediaDescription [in] media description to connect with the stream
         *
         * @retval Standard error result code
         * @retval ERR_UNEXPECTED media type does not support media description or invalid XML tag
         * @retval ERR_INVALID_ARG no description for this stream available
         * @retval ERR_POINTER Null-pointer committed
         * @retval ERR_NOT_INITIALISED DDLImporter was not initialized yet
         * @retval ERR_UNKNOWN_FORMAT Expected XML hierarchy not found
         * @retval ERR_NO_CLASS Cross reference not resolvable (e.g. refUnit)
         * @retval ERR_UNKNOWN Cross reference has not been resolved
         * @retval ERR_INVALID_VERSION Incompatible DDL descriptions detected (not the same DDL version) 
         * @retval ERR_NOT_FOUND At least one mandatory element (e.g. header) was not found
         */
        virtual tResult SetStreamMediaDescription(tUInt16 ui16StreamId, const tChar* strMediaDescription) = 0;

        /**
         *
         * This function returns the stream name for the given handle if exists.
         *
         * @param ui16StreamId [in] id to look up.
         *
         * @retval Returns the stream name.
         * @retval If stream is not found, returns NULL.
         */
        virtual const tChar* GetStreamName(tUInt16 ui16StreamId) = 0;

        /**
         *
         * This function returns the stream id for the given name if exists.
         *
         * @param strName [in] name of stream, case sensitive.
         * @param ui16StreamId [out] id of stream with name strName.
         *
         * @retval ERR_NOERROR if strName was found.
         * @retval ERR_UNKNOWN if strName was not found.
         * @retval ERR_POINTER if strname is NULL.
         *
         */
        virtual tResult GetStreamId(const tChar* strName, tUInt16& ui16StreamId) = 0;

        /**
         *
         *  This function sets the media description of the DAT File.
         *
         *  @param strShortDescription [in] short description of DAT File content. May be NULL.
         *  @param strLongDescription [in] short description of DAT File content May be NULL.
         *  @param pDateTime [in] date and time of DAT File creation. May be NULL.
         *
         *  @note If either strShortDescription or strLongDescription is NULL, the
         *        Description of the file is not modified.
         *
         *  @note The short description must not contain the EOL (end of line) character,
         *        because EOL is used as separator between the short and long description.
         *        ERR_UNEXPECTED is returned if the short description contains the EOL character.
         *
         *  @note The length of the short description is limited to 128 characters.
         *        The text after the 128th character will be cut.
         *
         *  @note If pDateTime is NULL, the creation dat and time of the file are not modified.
         *
         *  @note call this after successfully calling Open.
         *  
         *  @retval ERR_NOERROR if all went well.
         *  @retval ERR_UNEXPECTED if call on non open file
         */
        virtual tResult SetMediaDescription(const tChar* strShortDescription, 
                                            const tChar* strLongDescription,
                                            const tDateTime* pDateTime=NULL) = 0;

        /**
         *
         * Gets the extension count. 
         * This function retrieves the number of extensions the
         * dat file contains.
         *
         * @note your written dat-file will contain a few more extensions than this
         *       function returns since straminfos are stored as extension, too,
         *       <b>at the time the file is closed</b>
         *
         * @return Returns the number of extensions.
         *
         */
        virtual tInt GetExtensionCount() = 0;

        
        /**
         *
         *  This function adds an extension to the dat file.
         *
         *  @param pExtension [in] pointer to strcuture defining the extension
         *
         *  @retval ERR_NOERROR if all went well
         *  @retval ERR_NOT_IMPL if the size of the extension was bigger than a tInt could hold
         *  @retval ERR_UNEXPECTED if no file is open
         *
         */
        virtual tResult AddExtension(const cADTFFileExtension* pExtension) = 0;
        
        /**
            *
            * Adds a complete directory to EDS (without recursion)
            *
            * @param [in] i_strEDSPath path inside the DAT-file ('\\0' terminated !)
            * @param [in] i_strFileDir path of the source files at your filesystem ('\\0' terminated !).
            *                          May contain regular expressions and macros.
            * @param [in] i_strComment additional comment for the file ('\\0' terminated !)
            * @param [in] i_strVersion additional version string for the file ('\\0' terminated !)
            *
            * @retval OK everything is OK
            * @retval ERR_PATH_NOT_FOUND i_strSourcePath is invalid
            * @retval ERR_INVALID_ARG i_strEDSPath is invalid
            * @retval ERR_POINTER internal error
            *
            */
        virtual tResult AddExtendedDataDir(const tChar* i_strEDSPath,  
                                           const tChar* i_strFileDir, 
                                           const tChar* i_strComment, 
                                           const tChar* i_strVersion) = 0;

            
           /**
            * Adds a single file to EDS
            *
            * @param [in] i_strEDSPath path inside EDS ('\\0' terminated !).
            *                          May contain macros.
            * @param [in] i_strEDSFilename NULL if the filename of i_strFile should be used  ('\\0' terminated !)
            * @param [in] i_strFile path + name of the source file ('\\0' terminated!)
            * @param [in] i_strComment additional comment for the file ('\\0' terminated !)
            * @param [in] i_strVersion additional version string for the file ('\\0' terminated !)
            *
            * @retval OK everything is OK
            * @retval ERR_PATH_NOT_FOUND i_strSourcePath is invalid
            * @retval ERR_INVALID_ARG i_strEDSPath is invalid
            *
            */
        virtual tResult AddExtendedData(const tChar* i_strEDSPath, 
                                        const tChar* i_strEDSFilename, 
                                        const tChar* i_strFile, 
                                        const tChar* i_strComment, 
                                        const tChar* i_strVersion) = 0;

           /**
            *
            * Adds the EDSExtension to the DAT-file
            * 
            * @param [in] i_nFlags additional flags, 0x1 indicates, that the file should be compressed
            *              
            * @retval OK                 everything is OK
            * @retval ERR_UNKNOWN        Extension can't be added
            * @retval ERR_UNEXPECTED if  no file is open
            */
        virtual tResult AddEDSExtension(tInt32 i_nFlags = 0) = 0;

    public:
        /**
         * Sets The global Loglevel.
         * By default the Loglevel is set to eLOG_LVL_DUMP
         * 
         * @param[in] nLogLevel    see @ref tLogLevel
         * @return Standard error code (see @ref page_error_codes)
         * @retval @ref ERR_NOERROR Log level reached.
         * @retval @ref ERR_FAILED LogLevel could not be set
         */
        static tResult SetLogLevel(tInt nLogLevel);

    private:    //internal stuff
        /**
         * Initialization function.
         */
        static tResult Init();
        /**
         * Release function.
         */
        static tResult Release();

}; //end of class

} //namespace adtfstreaming

#endif //_ADTF_STREAMING_WRITER_INTF_HEADER_
