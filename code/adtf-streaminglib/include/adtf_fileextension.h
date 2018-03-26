/**
 *
 * Container class for a file extension.
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: fellnbi $
 * $Date: 2012-08-31 09:49:52 +0200 (Fri, 31 Aug 2012) $
 * $Revision: 15758 $
 *
 * @remarks
 *
 */
#ifndef _ADTF_FILEEXTENSION_HEADER_
#define _ADTF_FILEEXTENSION_HEADER_

namespace adtfstreaming
{

/**
 * This class is a helper container for file extensions of an adtf dat file.
 */
class DOEXPORT cADTFFileExtension
{

    public:
        cADTFFileExtension();
        virtual ~cADTFFileExtension();

    protected:
        const tChar*                    m_strIdentifier;    ///< identifier
        tUInt16                         m_ui16StreamId;     ///< stream ID
        tUInt32                         m_ui32TypeId;       ///< type ID
        tUInt32                         m_ui32VersionId;    ///< version ID
        tInt64                          m_szSize;           ///< size
        const tVoid*                    m_pData;            ///< pointer to data
        tUInt32                         m_ui32UserId;       ///< user ID


    public:
        /**
         * Gets the file extension data.
         *
         * @param pData    [out]       Address of a pointer of the data.
         *
         * @return Returns the size in bytes of the data.
         */
        tInt64                      GetData(const tVoid** pData) const;
        
        /**
         * Gets the file extension size
         *
         * @return Returns the size in bytes of the data.
         */
        tInt64                      GetDataSize() const;
        
        /**
         * Gets the name of the file extension.
         *
         *
         * @return Returns the name of the file extension.
         */
        const tChar*                GetIdentifier() const;

        /**
         * Gets the stream id of the file extension.
         *
         * @return Returns the stream id of the file extension..
         */
        const tUInt16               GetStreamId() const;

        /**
         * Gets the type of the file extension.
         *
         * @return Returns the type of the file extension.
         */
        const tUInt32               GetType() const;

        /**
         * Gets the version of the file extension.
         *
         * @return Returns the version of the file extension.
         */
        const tUInt32               GetVersion() const;

        /**
        * Gets the userid of the file extension.
        *
        * @return Returns the user id of the file extension.
        */
        const tUInt32               GetUserId() const;

        /**
         * Sets the file extension data.
         *
         * @param pData    [in]             File extension data.
         * @param szSize   [in]             The size of the data.
         *
         * @return Returns the standard result code.
         */
        tResult SetData(const tVoid* pData, tInt64 szSize);

        /**
         * Sets the name of the file extension.
         *
         * @param strIdentifier    [in]     Name of the file extension.
         *
         * @return Returns the standard result code.
         */
        tResult SetIdentifier(const tChar* strIdentifier);

        /**
         * Sets the stream id of the file extension.
         *
         * @param ui16StreamId       [in]     Stream id of the file extension.
         *
         * @return Returns the standard result code.
         */
        tResult SetStreamId(tUInt16 ui16StreamId);

        /**
         * Sets the type of the file extension.
         *
         * @param ui32TypeId       [in]     Type of the file extension.
         *
         * @return Returns the standard result code.
         */
        tResult SetType(tUInt32 ui32TypeId);

        /**
         * Sets the version of the file extension.
         *
         * @param ui32VersionId     [in]     Version of the file extension.
         *
         * @return Returns the standard result code.
         */
        tResult SetVersion(tUInt32 ui32VersionId);
        /**
        * Sets the user id of the file extension.
        *
        * @param ui32UserId     [in]     User Id of the file extension.
        *
        * @return Returns the standard result code.
        */
        tResult SetUserId(tUInt32 ui32UserId);
};

} //namespace adtfstreaming

#endif //_ADTF_FILEEXTENSION_HEADER_
