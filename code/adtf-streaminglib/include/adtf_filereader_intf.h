/**
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: VTW8PSR $
 * $Date: 2014-10-28 09:23:42 +0100 (Tue, 28 Oct 2014) $
 * $Revision: 30385 $
 *
 * @remarks
 *
 */
#ifndef _ADTF_STREAMING_READER_INTF_HEADER_
#define _ADTF_STREAMING_READER_INTF_HEADER_

namespace adtfstreaming
{
/**
 * Reader Interface.
 */
class DOEXPORT IADTFFileReader
{
    public:
        virtual ~IADTFFileReader();

    public: //support for additional classes
        /**
        *  Add an additional class and its classfactory to the reader, e.g. in order to add more known sample types
        *  Call before the first call to create!!!
        *  @param strID     [in]        Identifier of class
        *  @param pFactory  [in]        pointer to factory that creates the class
        *  @return standard error code
        */
        static tResult AddClass(const tChar* strID, tVoid* pFactory);
        /**
         *  Add an additional service to be loaded, e.g. in order to add more known sample types
         *  Call before the first call to create!!!
         *  @param strServiceFilename   [in]    filename of service to be loaded
         *  @return standard error code
         */
        static tResult AddTypeService(const tChar* strServiceFilename);

    public:
        /**
         * Creates a dat file reader. The function creates a dat file reader to access the data
         * content of a dat file. 
         *
         * @return the interface to a dat file reader.
         */
        static IADTFFileReader* Create();

        /**
         * Releases a dat file reader. This function releases a dat file reader and deletes it.
         *
         * @param pReader   [in,out] Pointer to reader object to release
         * @return the standard result code.
         */
        static tResult  Release(IADTFFileReader* pReader);

        /**
         *  Get the number of valid streams contained in the dat file.
         *  THIS DOES NOT IMPLY that these streams are tightly packed.
         *  It is possible that stream 1 exists, 2 does not, but 3 and 4 do.
         *  @param nStreamCount    [out]    pointer to int that will receive the number of streams
         *  @return                         standard error code
         */
        virtual tResult GetStreamCount(tInt &nStreamCount) = 0;

        /**
         * Opens the ADTF file. The adtf reader can open the dat file only in reading access mode.
         * If a correspoding media description file (e.g. 'strFileName'.description) exits, this is also openend.
         *
         * @param strFileName   [in] The Filename as null terminated string (relative or absolute path).
         *
         * @return the standard result code.
         */
        virtual tResult Open(const tChar* strFileName) = 0;

        /**
         * Closes the ADTF file. The close method closes the dat file. 
         * Do not forget to call @ref Release.
         *
         * @return the standard result code.
         */
        virtual tResult Close() = 0;

        /**
         * Gets the chunk count. The function retrieves the number of chunks the
         * adtf file contains, when it is opened.
         *
         * @return the number of chunks.
         */
        virtual tInt64  GetDataBlockCount() = 0;

        /**
         * Gets the chunk count. The function retrieves the number of chunks the
         * adtf file contains, when it is opened.
         *
         * @param nStreamId [in] ID of the stream
         * @return the number of chunks.
         */
        virtual tInt64 GetDataBlockCount(tUInt16 nStreamId) = 0;

        /**
         * Gets the index block count. The function retrieves the number of index entries in the master index table the
         * adtf file contains, when it is opened.
         *
         * @return the number of index entries.
         */
        virtual tInt64  GetIndexBlockCount() = 0;

        /**
         * Gets the next index entry in the master index table.
         *
         * @return the next index entry in the master index table.
         */
        virtual tInt64  GetNextIndexBlockPos()       = 0;

        /**
         * Gets the current chunk index of the current master index entry.
         *
         * @return the current chunk index of the current master index entry.
         */
        virtual tInt64  GetCurrentIndexBlockPos()    = 0;

        /**
         * Gets the current chunk index.
         *
         * @return the current chunk index.
         */
        virtual tInt64  GetCurrentBlockPos() = 0;

        /**
         * Gets the time duration. The function retrieves the time the file is lasting.
         *
         * @return the time.
         */
        virtual tTimeStamp  GetTimeDuration() = 0;

        /**
         * Seeks the DAT File. The seek method jumps chunk with the given index.
         * The next IADTFFileReader::Read call will read the specified chunk.
         *
         * @param nDataBlockPos [in] The index of the chunk (between 0 and chunk count).
         *
         * @return the current chunk index.
         */
        virtual tInt64 Seek(tInt64 nDataBlockPos) = 0;

        /**
         * Seeks the DAT File. The seek method jumps chunk in the current stream with the given index.
         * The next IADTFFileReader::Read call will read the specified chunk.
         *
         * @param nStreamId     [in] The ID of the stream to seek
         * @param nDataBlockPos [in] The index of the chunk (between 0 and chunk count).
         *
         * @return the current chunk index.
         */
        virtual tInt64 Seek(tUInt16 nStreamId, tInt64 nDataBlockPos) = 0;


        /**
         * Seeks the DAT File. The seek method jumps to the given time.
         * The next IADTFFileReader::Read call will read the next chunk at or after the
         * specified time.
         *
         * @warning \c IIndexedFileReader will crash if \c tmTimePos is smaller
         * than the file offset.
         * @param tmTimePos [in] The file time in micro seconds.
         *
         * @return the current chunk index.
         */
        virtual tInt64 SeekTime(tTimeStamp tmTimePos) = 0;

        //virtual tResult GetNextSample(IMediaSample** ppSample, tUInt16* pnStreamId, tTimeStamp* pnTime) = 0;

        /**
         * Reads the next dat file content. The content is contained in a class object named cADTFDataBlock.
         * This function will increase the current dat file position. And return an internal pointer.
         *
         * @param ppDataBlock [in, out] Address of an pointer to an data block.
         *                              The Pointer will be valid until the next fileoperation.
         *
         * @return the standard result code.
         */
        virtual tResult Read(cADTFDataBlock** ppDataBlock) = 0;

        /**
         * Reads the next dat file content. The content is contained in a class object named cADTFDataBlock.
         * This function will increase the current dat file position.
         *
         * @param pDataBlock [inout] Address of an user allocated data block.
         *
         * @return the standard result code.
         */
        virtual tResult Read(cADTFDataBlock* pDataBlock) = 0;

        /**
         * Seeks the stream name. The function seek the stream name for the given handle if exists.
         *
         * @param nStreamId [in] ID of the stream
         * @return the stream name. if not found the return value will be NULL.
         */
        virtual const tChar* GetStreamName(tUInt16 nStreamId) = 0;

        /**
         * Find the stream id. 
         * This function returns the stream id for the given name if exists.
         *
         * @param strName [in] name of stream, case sensitive.
         * @param ui16StreamId [out] id of stream with name strName.
         *
         * @return ERR_NOERROR if strName was found.
         * @return ERR_UNKNOWN if strName was not found.
         * @return ERR_POINTER if strname is NULL.
         */
        virtual tResult GetStreamId(const tChar* strName, tUInt16& ui16StreamId) = 0;

        /**
         * Gets the Stream Description that describes the selected stream.
         *   @param  ui16StreamId [in]    the id of the selected stream, must be bigger than 0
         *   @return the StreamDescription
         */
        virtual const tADTFStreamDescriptor* GetStreamDescriptor(tUInt16 ui16StreamId) = 0;


        /**
         * Gets the Media Description. The function retrieves the media description of the file.
         *
         * @return the MediaDescription
         */
        virtual const tADTFMediaDescriptor* GetMediaDescriptor() = 0;

        /**
         * Get the media description of a stream (for further information about the media description,
         *  see the data definition language documentation)
         *
         * @param nStreamId      [in]  ID of the stream
         * @param strDescBuffer  [out] optional buffer to copy the media description into
         * @param szSizeOfBuffer [in]  size of strDescBuffer
         * @param ui32Flags      [in]  flags to control what part to get from the media description
         *
         * @return the media description for the corresponding stream but see remark
         * @retval If the stream is not found, it returns NULL.
         * 
         * @remark The return value is only valid until the next call of GetStreamMediaDescription !
         */
        virtual const tChar* GetStreamMediaDescription(tUInt16 nStreamId,
                                                       tChar* strDescBuffer = NULL,
                                                       tInt szSizeOfBuffer = 0,
                                                       tUInt32 ui32Flags = MDF_StreamDescription) = 0;

        /**
         * Get the additional data of a stream
         *
         * @param ui16StreamId  [in]  ID of the stream
         * @param ppData        [out] Address of a pointer to the additional data
         * @param pszSize       [in]  Size of ppData
         * @param ui32Flags     [in]  currently unused.
         *
         * @retval ERR_NOT_FOUND There is no additional data for the specified stream.
         */
        virtual tResult GetStreamAdditionalData(tUInt16 ui16StreamId, const tVoid** ppData, tSize* pszSize, tUInt32 ui32Flags = 0) = 0;
        
        /**
         * Gets the extension count. The function retrieves the number of extensions the
         * adtf file contains.
         *
         * @return the number of extensions.
         */
        virtual tInt GetExtensionCount() = 0;

        /**
         * Gets the given extension content. The content is contained in a class object named cADTFFileExtension.
         *
         * @param nIndex            [in] Index of the extension.
         * @param ppExtension       [out] Content of the extension. This pointer is only valid until the next call of GetExtension or FindExtension.
         *
         * @return the standard result code.
         */
        virtual tResult GetExtension(const tInt nIndex, const cADTFFileExtension** ppExtension) = 0;

        /**
         * Gets the given extension content. The content is contained in a class object named cADTFFileExtension.
         *
         * @param strIdentifier      [in] Extension name.
         * @param ppExtension        [out] Content of the extension. This pointer is only valid until the next call of GetExtension or FindExtension.
         *
         * @return the standard result code.
         */
        virtual tResult FindExtension(const tChar* strIdentifier, const cADTFFileExtension** ppExtension) = 0;
        
        /**
         * Extracts the EDS Data from a given DAT File to a Destination Dir
         *
         * @param i_strDatFile         [in] Full path with name of the containing DAT-file ('\\0' terminated !)
         * @param i_strDestPath        [in] Destination folder on a local file system, if the path is
         *                                  NULL the $TEMP$ macro will be used ('\\0' terminated !)
         * @param i_nFlags             [in] Additional flags
         *                                  \li 0x00 default, nothing is deleted, files will be overwritten
         *                                  \li 0x01 deletes the strDestPath before extracting \n
         *                                      handle with care if you set the value to 0x01. There may be
         *                                      occur some problems if the directory of 
         *                                      i_strDestPath is otherwise in use.
         *
         *
         * @retval ERR_INVALID_FILE i_strDatFile doesn't exist or the extraction of the file(s) failed
         * @retval ERR_ACCESS_DENIED i_strDatFile can't be opened
         * @retval ERR_UNEXPECTED no extension found
         * @retval ERR_INVALID_ARG if one of the pointer is NULL (expect i_strDestPath)
         * @retval OK if everything is OK
         */
        virtual tResult ExtractEDSDataFromDAT(const tChar* i_strDatFile, const tChar* i_strDestPath, tInt32 i_nFlags = 0) = 0;

        /**
         * @brief Get the GUID of the current file.
         * @return Either pointer to the beginning of 0-terminated string representing the GUID of
         * the currently handled file or NULL in case no GUID exists.
         */
        virtual const tChar* GetGUID() = 0;

    public:
        /**
         * Sets The global Loglevel.
         * By default the Loglevel is set to eLOG_LVL_DUMP
         * 
         * @param [in] nLogLevel    see @ref tLogLevel
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
}; //class end

} //namespace adtfstreaming

#endif //_ADTF_STREAMING_READER_INTF_HEADER_
