/**
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: A87AWGT $
 * $Date: 2010-04-13 11:13:40 +0200 (Di, 13 Apr 2010) $
 * $Revision: 3289 $
 *
 * @remarks
 *
 */
#ifndef _ADTF_STREAMING_COMPRESSION_INTF_HEADER_
#define _ADTF_STREAMING_COMPRESSION_INTF_HEADER_

namespace adtfstreaming
{

/**
 * Interface for compressing and extracting files.
 */
class DOEXPORT IADTFFileCompression
{
    public:
        virtual ~IADTFFileCompression();

    public:
        /// Enumeration to decide which compression algorithm should be used
        typedef enum
        {
            CA_Auto            = 0,
            CA_Bzip2           = 1,
        } tCompressionAlgorithms;

    public:
        
        /**
         * Compresses the given input file to the given output file.
         * @param strInputFile  [in] Filename of the input file.
         * @param strOutputFile [in] Filename of the output file (optional).
         * @param nAlgo         [in] The compression algorithm that should be used.
         * @return Standard result.
         */
        static tResult Compress(const tChar* strInputFile, const tChar* strOutputFile = 0, tUInt32 nAlgo = CA_Auto);
        
        /**
         * Decompresses the given input file to the given output file.
         * @param strInputFile  [in] Filename of the input file.
         * @param strOutputFile [in] Filename of the output file.
         * @param nAlgo         [in] The compression algorithm that should be used (Auto = extension detection).
         * @return Standard result.
         */
        static tResult Decompress(const tChar* strInputFile, const tChar* strOutputFile, tUInt32 nAlgo = CA_Auto);

}; //class end

} //namespace adtfstreaming

#endif //_ADTF_STREAMING_COMPRESSION_INTF_HEADER_
