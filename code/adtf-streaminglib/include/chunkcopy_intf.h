/**
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: VTW8PSR $
 * $Date: 2014-11-03 13:26:26 +0100 (Mon, 03 Nov 2014) $
 * $Revision: 30631 $
 *
 * @remarks
 *
 */
#ifndef _CHUNK_COPY_INTF_HEADER_
#define _CHUNK_COPY_INTF_HEADER_

namespace adtfstreaming
{

    /**
     * Interface for chunk copying.
     * to use this Interface see example "copychunks".
     */
    class DOEXPORT IChunkCopy
    {
    public:
        /**
         * DTOR
         */
        virtual ~IChunkCopy();

    public: // static methods
        /**
         * Creates an instance of cChunkCopy.
         *
         * @return A pointer to the newly created instance
         */
        static IChunkCopy* Create();

        /**
         * Releases an instance of cChunkCopy.
         *
         * @param [in] pCopier Pointer to the instance to release
         * @retval ERR_POINTER Null-pointer committed
         */
        static tResult Release(IChunkCopy* pCopier);

    public: // access methods
        /**
         * Opens the source ADTF DAT file. This class can open the DAT file only in
         * reading access mode.
         *
         * @param [in] strFileName The filename as null terminated string
         * (relative or absolute path)
         * @retval ERR_POINTER Null-pointer committed
         * @todo Document retvals from adtf_util::cIndexedFileReader::Open()
         * @todo Document retvals from adtf_util::cIndexedFile::GetHeader()
         * @retval ERR_OPEN_FAILED Compatibility file could not be opened
         */
        virtual tResult Open(const tChar* strFileName) = 0;

        /**
         * Creates a destination DAT file.
         *
         * @param [in] strFileName The filename as null terminated string
         * (relative or absolute path)
         * @param[in] ui32Flags Flags which describe the writing behavior (e.g. synchronous or
         * asynchronous). See \c adtfstreaming::tOpenMode to get all available flags
         * @retval ERR_NOERROR File successfully created
         * @retval ERR_FAILED Failed to create
         * @retval ERR_ACCESS_DENIED Could not create file becase of access restrictions.
         */
        virtual tResult CreateFile(const tChar* strFileName, tUInt32 ui32Flags = 0) = 0;

        /**
         * Closes open source and destination files, if they are open.
         *
         */
        virtual tResult Close() = 0;

        /**
         * Copies the next data block of a stream you added by @ref AddStreamToCopy from source to destination file.
         *
         * @retval ERR_EOF      end of source file reached.
         * @retval ERR_NOERROR  data block copied successfully.
         */
        virtual tResult CopyNext() = 0;

        /**
         * Copies the stream information of the ui16StreamIdFrom from the source file to the new stream
         * ui16StreamIdNew (with a new name). Call this function as many streams you want to copy.
         * 
         * @param [in] ui16StreamIdFrom StreamID of the source file.
         * @param [in] ui16StreamIdNew  StreamID of the destination file.
         * @param [in] strStreamName    Name of the destination stream.
         *
         * @retval ERR_NOERROR  stream successfully copied.
         */
        virtual tResult AddStreamToCopy(const tUInt16 ui16StreamIdFrom,
                                        const tUInt16 ui16StreamIdNew,
                                        const tChar* strStreamName) = 0;

        /**
         * Find the stream ID to the corresponding name.
         * The function returns the stream ID for the given name if the stream
         * exists.
         * @param [in]  strName Name of the stream, case sensitive
         * @param [out] ui16StreamId ID of the stream with name strName
         * @retval ERR_POINTER Null-pointer committed
         * @retval ERR_UNKNOWN Stream name was not found
         */
        virtual tResult GetStreamId(const tChar *strName, tUInt16 &ui16StreamId) = 0;

        /**
         * Seeks the stream name.
         * The function seeks the stream name for the given ID if the stream
         * exists.
         * @param [in] ui16StreamId ID of the stream
         * @return the name of the stream with given ID if the stream exists
         * or \c NULL otherwise.
         */
        virtual const tChar * GetStreamName(tUInt16 ui16StreamId) = 0;

        /**
         * Gets the chunk count. The function retrieves the number of chunks
         * the ADTF DAT file contains, when it is opened.
         *
         * @param [in] ui16StreamId ID of the stream (optional, 0 = all streams)
         * @return the number of chunks
         */
        virtual tInt64 GetDataBlockCount(const tUInt16 ui16StreamId = 0) = 0;

        /**
         * Gets the index block count. The function retrieves the number of
         * index entries in the master index table the ADTF DAT file contains,
         * when it is opened.
         *
         * @return the number of index entries
         */
        virtual tInt64 GetIndexBlockCount() = 0;

        /**
         * Gets the current chunk index.
         *
         * @return the current chunk index
         */
        virtual tInt64 GetCurrentBlockPos() = 0;

        /**
         * Gets the time duration. The function retrieves the time the file is
         * lasting.
         *
         * @return the time
         */
        virtual tTimeStamp GetTimeDuration() = 0;

        /**
         * Seeks the DAT file. The seek method jumps to the chunk with the
         * given index.
         * The next IChunkCopy::Copy call will read the specified chunk.
         *
         * @param [in] i64DataBlockPos The index of the chunk (between 0 and
         * chunk count)
         * @return the current chunk index
         */
        virtual tInt64 Seek(tInt64 i64DataBlockPos) = 0;

        /**
         * Seeks the DAT file. The seek method jumps to the chunk in the
         * current stream with the given index.
         * The next IChunkCopy::Copy call will read the specified chunk.
         *
         * @param [in] nStreamId        The ID of the stream to seek
         * @param [in] nDataBlockPos    The index of the chunk (between 0 and
         * chunk count)
         * @return the current chunk index
         */
        virtual tInt64 Seek(tUInt16 nStreamId, tInt64 nDataBlockPos) = 0;

        /**
         * Seeks the DAT file. The seek method jumps to the given time.
         * The next IChunkCopy::Copy() call will read the next chunk at or
         * after the specified time.
         *
         * @warning \c IIndexedFileReader will crash if \c tmTimePos is smaller
         * than the file offset.
         *
         * @param [in] tmTimePos The file time in microseconds
         * @return the current chunk index
         */
        virtual tInt64 SeekTime(tTimeStamp tmTimePos) = 0;

        /**
         * Gets the extension count. The function retrieves the number of
         * extensions the ADTF DAT file contains.
         *
         * @return the number of extensions
         */
        virtual tInt GetExtensionCount() = 0;

        /**
         * Gets the given extension content. The content is contained in a
         * class object named cADTFFileExtension.
         *
         * @param [in]  nIndex      Index of the extension
         * @param [out] ppExtension Content of the extension
         * @retval ERR_NOT_FOUND no extension found for the specified index
         * @retval ERR_POINTER  pointer NULL submitted
         *
         * @remarks The pointer ppExtension is only valid until the next call
         * of GetExtension or FindExtension.
         */
        virtual tResult GetExtension(const tInt nIndex, const cADTFFileExtension** ppExtension) = 0;

        /**
         * Gets the given extension content. The content is contained in a
         * class object named cADTFFileExtension.
         * @param [in]  strIdentifier   Extension name
         * @param [out] ppExtension     Content of the extension
         * @retval ERR_NOT_FOUND given identifier not found in extensions of the source file
         *
         * @remarks The pointer ppExtension is only valid until the next call
         * of GetExtension or FindExtension.
         */
        virtual tResult FindExtension(const tChar* strIdentifier, const cADTFFileExtension** ppExtension) = 0;

    private:    // internal stuff
        /**
         * Init function.
         */
        static tResult Init();

        /**
         * Release function.
         */
        static tResult Release();
    };

}   // namespace adtfstreaming

#endif  // _CHUNK_COPY_INTF_HEADER_