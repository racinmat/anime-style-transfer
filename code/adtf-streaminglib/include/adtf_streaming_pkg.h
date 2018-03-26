/**
 *
 * ADTF Package header.
 * The here included files were used to provide the streaming library interface.
 * The Headers depend on the type definitions in the base reference header (adtf_base_ref).
 * To Change this reference to different types just provide different type definitions.
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: VTW8PSR $
 * $Date: 2014-11-06 11:31:01 +0100 (Thu, 06 Nov 2014) $
 * $Revision: 30737 $
 *
 * @remarks
 *
 */

#ifndef _ADTF_STREAMING_LIB_PACKAGE_ 
#define _ADTF_STREAMING_LIB_PACKAGE_

/** 
 * \mainpage ADTFStreamingLibrary
 *
 * \section section_adtf_streamingLib Automotive Data and Time-Triggered Framework Streaming Library
 *
 * \n \b See \subpage page_whats_new and the \ref page_streaminglib_change_log
 * 
 * The ADTF Streaming Library is a dynamic Library package to provide a possibility accessing the ADTF DAT File (IndexedFile).
 * The DAT File is a streambased container to serialize and deserialize binary data.
 * 
 * For a more detailed specification on the DAT File see the current 
 * <b>ADTF DAT File Format Specification</b> in this installation (in <b>doc/DATFileFormatSpecification.pdf</b>).
 * 
 * \par ADTF Streaming Library Constraints
 * 
 * The ADTF Streaming Library will give you the possibility to (read/write) access a DAT File only.
 * Using this library to stream data within an Online Simulation Environment will fail. 
 * This library has \b NO \b performance optimization for time critical requirements! To do so use ADTF !
 * 
 *
 * \par ADTF Streaming Library Programming Example
 * \n See the documentation on \subpage page_examples_main.
 * 
 * \par Additional Information
 * \n \subpage page_error_codes - Documentation on Error Codes.
 * \n \subpage page_ddl_specification - Documentation on DDL (Data Definition Language which is shown in @ref page_mediadescription).
 *
 * 
 *
 */

/// Maximum amount of indexed streams
#define ADTF_STREAMING_MAX_INDEXED_STREAMS 512

#include "adtf_streaminglib_types.h"
#include "adtf_mediatypes.h"
#include "adtf_media_description.h"

#include "adtf_datablock.h"
#include "adtf_fileextension.h"

#include "adtf_filereader_intf.h"
#include "adtf_filewriter_intf.h"
#include "adtf_filecompression_intf.h"
#include "chunkcopy_intf.h"

#include "adtf_fileextension_referencedfiles_intf.h"

#endif //_ADTF_STREAMING_LIB_PACKAGE_
