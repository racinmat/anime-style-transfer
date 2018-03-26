/**
 *
 * DataBlock container for one mediasample.
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved.
 *
 * $Author: voigtlpi $
 * $Date: 2014-02-25 21:03:17 +0100 (Tue, 25 Feb 2014) $
 * $Revision: 26648 $
 *
 * @remarks
 *
 */
#ifndef _ADTF_FILEBLOCK_HEADER_ 
#define _ADTF_FILEBLOCK_HEADER_

namespace adtfstreaming 
{

/**
 * This class is a helper container for the data contained in ADTF MediaSamples that are contained in a dat file.
 * A media sample has time and type informations added as meta data.
 * A cADTFDataBlock has to be seen as a pointer to a part of the file and not a data container, therefore there is no 
 * deep copy defined.
 *
 * @remark A cADTFDataBlock must be destroyed BEFORE the last @ref IADTFFileReader::Release or @ref IADTFFileWriter call is made ! 
 */

class DOEXPORT cADTFDataBlock
{
    friend class cADTFFileReader;
    friend class cAdtfFileWriter;

    private:
       class cADTFDataBlockPrivate;
       /// Internal P-IMPL.
       cADTFDataBlockPrivate* _d;

    public:
        /**
         * The Possible Data Type Flags for the Block.
         */
        typedef enum tDataType_tag
        {
            DTF_DATA = 0x00,    //!< data block is standard data container, defaultvalue
            DTF_SYNC = 0x01,    //!< the data block is sync block (a indexed table entry).
            DTF_INFO = 0x02     //!< data block is info block (marker set)
        } tDataTypeFlags;

    public:
        cADTFDataBlock();
        virtual ~cADTFDataBlock();

    public:
        /**
         * Gets the data.
         *
         * @param pvData    [in, out]   Address of a pointer of data. if succeed the content of pvData
         *                              will point to the buffer where the data stored.
         * 
         * @return Returns the size in bytes of the data buffer.
         */
        tInt64                GetData(const tVoid** pvData);

        /**
         * Gets the Time of the data.
         * 
         * @return Returns the time of the data block in micro second. 
         */
        const tTimeStamp      GetTime();

        /**
         * Gets the Time the data was saved in file.
         * The difference of the two times is, that the time, which can be retrieved 
         * by GetTime() is the time the data were created within the adtf (a user time set to the data).
         * The filetime is the time, when the data were stored in the file.
         * 
         * @return Returns the file time of the data block in micro second. 
         */
        const tTimeStamp      GetFileTime();

        /**
         * Return the position of the block in the stream
         * 
         * @return block position of the data block in the stream
         */
        const tInt64          GetBlockPos();

        /**
         * Return the position of the block in the file
         * 
         * @return block position if the data block in the file
         */
        const tInt64          GetFileBlockPos();

        /**
         * Gets the unique stream handle of the data.
         * The stream handle id is a number related to a stream identifier in the file.
         * This number is unequal 0 only if user marked the data as streamed. 
         *                                (adtf intern called storage handle)
         * 
         * 
         * @return Returns the unique stream handle. 
         */
        const tUInt16         GetStreamId();

        /**
         *  Gets the internal sample flags. (needed for ADTF depended usings)
         *  @return sample flags
         */
        tUInt32               GetInternalFlags();

        /**
         *  Gets the data type.
         *  A data block can be one of three possible data types:
         *  \n Data, Info or Sync. 
         *  \n This function returns the datatype of a block.
         *
         *  @return data type flag \link cADTFDataBlock::tDataTypeFlags \c cDataTypeFlags \endlink
         */
        tUInt16               GetFlags();

        /**
         *  Sets the internal type flags.
         *  you need to see the ADTF Sample Implementation.
         *  @param[in] ui32Flags Flags to set
         */
        tVoid                 SetInternalFlags(tUInt32 ui32Flags);

        /**
        *  Sets the data type flags.
        *  A data block can be one of three possible data types:
        *  \n Data, Info or Sync. 
        *  \n This function sets the datatype of a block.
        *
        *  @param ui16Flags [in] datatype of block @ref cADTFDataBlock::tDataTypeFlags
        */
        tVoid                 SetFlags(tUInt16 ui16Flags);

        /**
         * Sets the data in the data block.
         * 
         * @param  pData      [in] The pointer to the buffer to set the data in the data block.
         * @param  i64Size    [in] The size of the buffer in bytes. (mind, this must be a real size time, which maximum often depends on the machine architecture and compiler max of tSize)
         * @param  bReference [in] if true -> The buffer will be hold as reference.
         *                            false -> The buffer will be copied to a intern buffer.
         *
         * @return The standard result code.
         */
        tResult SetData(const tVoid* pData, tInt64 i64Size, tBool bReference = tFalse);

        /**
         * Sets the time of the data.
         * 
         * @param  tmTime [in]  The Time to set in micro seconds.
         *
         * @return The standard result code.
         */
        tResult SetTime(const tTimeStamp tmTime);

        /**
         * Sets the file time of the data.
         * 
         * @param  tmFileTime [in]  The Time to set in micro seconds. 
         *
         * @return The standard result code.
         */
        tResult SetFileTime(const tTimeStamp tmFileTime);

        /**
         * Sets the unique stream handle of the data.
         * 
         * @param  ui16StreamId [in]  The stream id to set.
         *
         * @return The standard result code.
         */
        tResult SetStreamId(const tUInt16 ui16StreamId);

        /**
         * Sets the block position in the stream
         * 
         * @param  nStreamBlockPos [in]  block position in the stream
         *
         * @return The standard result code.
         */
        tResult SetBlockPos(const tInt64 nStreamBlockPos);

        /**
         * Sets the block position in the file
         * 
         * @param  nFileBlockPos [in]  block position in the file
         *
         * @return The standard result code.
         */
        tResult SetFileBlockPos(const tInt64 nFileBlockPos);

        /**
         *  Returns the additional integer value stored within the media sample.
         *  @see ADTF SDK Documentation refering to adtf::IMediaSampleInfo
         *
         *  @param  ui32Index     [in] index of the value. This value is user defined.
         *  @param  nDefault      [in] The default value if the value does not exist.
         *
         *  @return Returns the additional integer value stored within the media sample
         */
        tInt GetAdditionalInt(tUInt32 ui32Index, tInt nDefault=0);

        /**
         *  Set an additional integer value stored within the media sample.
         *  @see ADTF SDK Documentation refering to adtf::IMediaSampleInfo
         *
         *  @param  ui32Index     [in] index of the value. This value is user defined.
         *  @param  nValue        [in] the integer value to be set.
         *
         *  @return Standard Result Code
         *  @retval ERR_INVALID_INDEX Index not accessible.
         */
        tResult SetAdditionalInt(tUInt32 ui32Index, tInt nValue);

        /**
         *  Returns the additional float value stored within the media sample.
         *  @see ADTF SDK Documentation refering to adtf::IMediaSampleInfo
         *
         *  @param  ui32Index     [in] index of the value. This value is user defined.
         *  @param  fDefault      [in] The default value if the value does not exist.
         *
         *  @return Returns the additional float value stored within the media sample
         */
        tFloat GetAdditionalFloat(tUInt32 ui32Index, tFloat fDefault=0.0);

        /**
         *  Set an additional float value stored within the media sample.
         *  @see ADTF SDK Documentation refering to adtf::IMediaSampleInfo
         *
         *  @param  ui32Index     [in] index of the value. This value is user defined.
         *  @param  fValue        [in] the float value to be set.
         *
         *  @return Standard Result Code
         *  @retval ERR_INVALID_INDEX Index not accessible.
         */
        tResult SetAdditionalFloat(tUInt32 ui32Index, tFloat fValue);

        /**
         *  Returns the additional string value stored within the media sample.
         *  @see ADTF SDK Documentation refering to adtf::IMediaSampleInfo
         *
         *  @param  ui32Index     [in]    index of the value. This value is user defined.
         *  @param  strBuffer     [inout] A Pointer to a string array where to copy the value to.
         *  @param  nBufferSize   [in]    The size of the strBuffer (in bytes).
         *  @param  strDefault    [in]    A NULL terminated string as default value if not found)
         *
         *  @return Returns the value as NULL terminated string (or the default value if not found).
         *  @warning When using the return Info you need to copy the value immediately (not thread-safe)
         */
        const tChar* GetAdditionalStr(tUInt32 ui32Index,
                                      tChar* strBuffer=NULL,
                                      tInt nBufferSize=0,
                                      const tChar* strDefault="");

        /**
         *  Set an additional string value stored within the media sample.
         *  @see ADTF SDK Documentation refering to adtf::IMediaSampleInfo
         *
         *  @param  ui32Index     [in] index of the value. This value is user defined.
         *  @param  strValue      [in] the string value to be set.
         *
         *  @return Standard Result Code
         *  @retval ERR_INVALID_INDEX Index not accessible.
         */
        tResult SetAdditionalStr(tUInt32 ui32Index, const tChar* strValue);

        /**
         *  Returns the additional time stamp value stored within the media sample.
         *  @see ADTF SDK Documentation refering to adtf::IMediaSampleInfo
         *
         *  @param  ui32Index     [in] index of the value. This value is user defined.
         *  @param  tmDefault     [in] The default value if the value does not exist.
         *
         *  @return Returns the additional time stamp value stored within the media sample
         */
        tTimeStamp GetAdditionalTimestamp(tUInt32 ui32Index, tTimeStamp tmDefault=0);

        /**
         *  Set an additional time stamp value stored within the media sample.
         *  @see ADTF SDK Documentation refering to adtf::IMediaSampleInfo
         *
         *  @param  ui32Index     [in] index of the value. This value is user defined.
         *  @param  tmValue       [in] the time stamp value to be set.
         *
         *  @return Standard Result Code
         *  @retval ERR_INVALID_INDEX Index not accessible.
         */
        tResult SetAdditionalTimestamp(tUInt32 ui32Index, tTimeStamp tmValue);

        /**
         *  Returns whether the given info index exists or not.
         *  @see ADTF SDK Documentation refering to adtf::IMediaSampleInfo
         *
         *  @param  ui32Index     [in] index of the value. This value is user defined.
         *
         *  @retval tTrue if the index exists.
         *  @retval tFalse index doesn't exist
         */
        tBool HasAdditionalValue(tUInt32 ui32Index);

    protected:
        /// this is for internal use only.
        tVoid* GetInternalData();

        /**
         * This is for internal use only.
         * @param [in] pInternalData  internal data 
         */
        tVoid SetInternalData(tVoid* pInternalData);

    private:
        /**
         * Private Copy-CTOR.
         */
        cADTFDataBlock(const cADTFDataBlock&);
        /**
         * Private operator =
         */
        cADTFDataBlock& operator=(const cADTFDataBlock&);
};

} //namespace adtfstreaming

#endif //_ADTF_FILEBLOCK_HEADER_
