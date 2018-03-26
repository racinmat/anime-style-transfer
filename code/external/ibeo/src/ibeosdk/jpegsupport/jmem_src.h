/* jmem_src.h: memory source manager for JFIF library
*/

//======================================================================

#ifndef JMEM_SRC_H_SEEN
#define JMEM_SRC_H_SEEN

//======================================================================

#include "jpeglib.h"

#include "jpegfix.h"

//======================================================================

typedef struct {
	struct jpeg_source_mgr pub;   /* public fields */

	JOCTET * buffer;              /* image buffer */
	unsigned int buffer_size;     /* image buffer size */
} mem_source_mgr;

//======================================================================

typedef mem_source_mgr * mem_src_ptr;

//======================================================================

extern GLOBAL(void)
jpeg_memory_src (j_decompress_ptr cinfo, const unsigned char* jfif_buffer,
                 unsigned int buf_size);

//======================================================================

#endif // JMEM_SRC_H_SEEN

//======================================================================
