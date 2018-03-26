/* jmem_dest.c: memory destination manager for JFIF library
*/

//======================================================================

#ifndef JMEM_DEST_H_SEEN
#define JMEM_DEST_H_SEEN

#include "jpeglib.h"

#include "jpegfix.h"

//======================================================================

/* mem_destination_mgr: memory destination manager */


typedef struct {
	struct jpeg_destination_mgr pub; /* public fields */

	JOCTET * buffer;              /* image buffer */
	unsigned int buffer_size;     /* image buffer size */
} mem_destination_mgr;

//======================================================================

typedef mem_destination_mgr * mem_dest_ptr;

//======================================================================

extern GLOBAL(void)
jpeg_memory_dest (j_compress_ptr cinfo,
                  unsigned char* jfif_buffer,
                  unsigned int buf_size);

//======================================================================

#endif //JMEM_DEST_H_SEEN

//======================================================================
