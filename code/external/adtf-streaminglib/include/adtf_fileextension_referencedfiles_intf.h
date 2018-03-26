/**
 *
 * Interface for a container class for easy accessing ADTF file extension "referencedfiles"
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: fellnbi $
 * $Date: 2012-08-31 09:49:52 +0200 (Fr, 31 Aug 2012) $
 * $Revision: 15758 $
 *
 * @remarks
 *
 */
#ifndef _ADTF_FILEEXTENSION_REFERENCEDFILES_INTF_HEADER_
#define _ADTF_FILEEXTENSION_REFERENCEDFILES_INTF_HEADER_

namespace adtfstreaming
{

/**
 * @brief Easy querying of the referenced files extension delivered inside an ADTF *.dat file.
 * @details The typical usage to retrieve the referenced file extension would look like this:
 * @code
    //create the object and pass the file to read from
    IReferencedFilesExtension* const pReferencedFiles = IReferencedFilesExtension::Create();
    pReferencedFiles->Read("path/to/adtf_file.dat");

    //further operations
    //[...]
    
    //cleanup
    IReferencedFilesExtension::Release(pReferencedFiles);
 * @endcode
 * Please note that necessary error checks were left out for clarity reasons.
 * @note For a complete example, please consult \ref page_referencedfiles.
 */
class DOEXPORT IReferencedFilesExtension
{
public:
    /**
     * @brief Read the referenced files extension from the file given with \c i_strFilename.
     * @post If no error, any previous assigned referenced file paths will be overwritten with the
     * referenced file paths from \c i_strFilename.
     * @param[in] i_strFilename Path to the ADTF dat file which will be opened and read.
     * @return Standard error code.
     * @retval ERR_NOERRROR Everything went fine (also includes no referenced file paths found).
     * @retval ERR_POINTER NULL == \c i_strFilename
     * @retval ERR_FAILED Could not open file.
     * @retval ERR_DEVICE_IO Reading from file failed
     * @note Exception guarantee: Strong
     */
    virtual tResult Read(const tChar* const i_strFilename) = 0;

    /**
     * @brief Write the currently contained referenced files to the extension of given ADTF file.
     * @details The file given with \c Read() will be overwritten using the same path. If no
     * referenced file exists, the extension will be created empty.
     * @return Standard error code.
     * @retval ERR_NOERROR Everything went fine.
     * @retval ERR_NOT_FOUND No file given to overwrite.
     */
    virtual tResult Write() = 0;

    /**
     * @brief Get number of referenced file paths.
     * @return Number of current referenced file paths.
     */
    virtual tSize GetCount() const = 0;

    /**
     * @brief Get the referenced file from the extension using an index.
     * @param[in] i_szIndex Index to access, starting with 0. If out of range, NULL is returned.
     * @return Pointer to 0-terminated string containing a path to one referenced file. NULL if
     * index doesn't exist.
     * @note Exception guarantee: Strong
     */
    virtual const tChar* Get(tSize i_szIndex) const = 0;

    /**
     * @brief Remove a referenced file path from the extension.
     * @param[in] i_strReferencedFilePath 0-terminated string containing the referenced file path
     * to remove.
     * @return Standard error code.
     * @retval ERR_NOERROR Everything went fine.
     * @retval ERR_POINTER <tt>NULL == i_strReferencedFilePath</tt>
     * @retval ERR_NOT_FOUND \c i_strReferencedFilePath not found.
     */
    virtual tResult Remove(const tChar* const i_strReferencedFilePath) = 0;

    /**
     * @brief Add a referenced file path to the extension
     * @param[in] i_strReferencedFilePath 0-terminated string containing the referenced file path
     * to add.
     * @return Standard error code.
     * @retval ERR_NOERROR Everything went fine.
     * @retval ERR_POINTER <tt>NULL == i_strReferencedFilePath</tt>
     */
    virtual tResult Add(const tChar* const i_strReferencedFilePath) = 0;

    /**
     * @brief Replace a referenced file path.
     * @param[in] i_strTo Pointer to 0-terminated referenced file path to replace. If NULL, behaves
     * the same as \c Add().
     * @param[in] i_strWith Pointer to the new 0-terminated referenced file path. If NULL, behaves
     * the same as \c Remove().
     * @return Standard error code
     * @retval ERR_POINTER <tt>NULL == i_strTo && NULL == i_strWith</tt>
     * @retval ERR_NOERROR Everything went fine.
     * @retval ERR_NOT_FOUND \c i_strTo not found.
     */
    virtual tResult Replace(const tChar* const i_strTo, const tChar* const i_strWith) = 0;

public: //static member functions
    /**
      * @brief Creates an instance of \c IReferencedFilesExtension
      * @warning Make sure to always call \c Release() on destruction.
      * 
      * @return Pointer to newly allocated object of type \c IReferencedFilesExtension or NULL if
      * something went wrong.
      */
    static IReferencedFilesExtension* Create();

    /**
     * @brief Destroy and clean up an object of \c *this type created with \c Create()
     * @param[in] i_pReferencedFiles Pointer to object that needs to be released.
     * @return Standard error code
     * @retval ERR_NOERRROR Everything went fine.
     * @retval ERR_POINTER NULL == \c i_pReferencedFiles
     */
    static tResult Release(IReferencedFilesExtension* const i_pReferencedFiles);

protected:
    /**
     * @brief Not accessible destructor, call \c IReferencedFilesExtension::Release() for cleanup.
     */
    virtual ~IReferencedFilesExtension();
};//class IReferencedFilesExtension

}//ns adtfstreaming

#endif //_ADTF_FILEEXTENSION_REFERENCEDFILES_INTF_HEADER_
