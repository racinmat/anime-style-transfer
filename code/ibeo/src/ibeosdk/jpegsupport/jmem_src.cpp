/* jmem_src.c: memory source manager for JFIF library
**
** There is a companion data module called mem_dest.[ch], which should be
** read for an explanation of the implementation details of this module.
**
** (Based on jdatasrc.c)
*/

//======================================================================

#include <stdio.h>
#include <stdlib.h>

#ifdef ibeoapi_STATIC
# define JPEG_STATIC
#endif

/* this is not a core library module, so it doesn't define JPEG_INTERNALS */
#include "jpeglib.h"
#include "jerror.h"

#include "jmem_src.h"

//======================================================================
/*
** init_source:  Initialize memory source object
** Called by jpeg_read_header before any data is actually read.
**
** cinfo: (in/out) pointer to JPEG decompression object
*/

METHODDEF(void)
init_source (j_decompress_ptr cinfo)
{
	mem_src_ptr src = (mem_src_ptr) cinfo->src;

	// Initialize public members

	// See the notes in mem_dest.c for init_destination

	// next_input_byte is set to point to the beginning of the source
	// image buffer
	src->pub.next_input_byte = src->buffer;

	// bytes_in_buffer is set to the maximum size of the input buffer.
	// It is assumed that even if the JPEG compressed image data is smaller
	// than buffer_size, the JFIF library will see the EOI tag and stop
	// decomression before reaching the end of the buffer.

	src->pub.bytes_in_buffer = src->buffer_size;
}




//======================================================================
/*
 * Fill the input buffer --- called whenever buffer is emptied.
 *
 * In typical applications, this should read fresh data into the buffer
 * (ignoring the current state of next_input_byte & bytes_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been reloaded.  It is not necessary to
 * fill the buffer entirely, only to obtain at least one more byte.
 *
 * There is no such thing as an EOF return.  If the end of the file has been
 * reached, the routine has a choice of ERREXIT() or inserting fake data into
 * the buffer.  In most cases, generating a warning message and inserting a
 * fake EOI marker is the best course of action --- this will allow the
 * decompressor to output however much of the image is there.  However,
 * the resulting error message is misleading if the real problem is an empty
 * input file, so we handle that case specially.
 *
 * In applications that need to be able to suspend compression due to input
 * not being available yet, a FALSE return indicates that no more data can be
 * obtained right now, but more may be forthcoming later.  In this situation,
 * the decompressor will return to its caller (with an indication of the
 * number of scanlines it has read, if any).  The application should resume
 * decompression after it has loaded more data into the input buffer.  Note
 * that there are substantial restrictions on the use of suspension --- see
 * the documentation.
 *
 * When suspending, the decompressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_input_byte & bytes_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point must be rescanned after resumption, so move it to
 * the front of the buffer rather than discarding it.
 */

/* fill_input_buffer should never be called for memory source.  Entire
** compressed image should be in buffer at the time of decompression.
**
** Just in case something happened to the data on the way into the
** buffer, fill_input_buffer will fake a EOI.
*/
METHODDEF(boolean)
fill_input_buffer (j_decompress_ptr cinfo)
{
	mem_src_ptr src = (mem_src_ptr) cinfo->src;
//	size_t nbytes;

	fprintf(stderr, "jmem_src: fill_input_buffer: should not be called.\n");

	/* Print a warning message */
	WARNMS(cinfo, JWRN_JPEG_EOF);

	/* Insert a fake EOI marker */
	src->buffer[0] = (JOCTET) 0xFF;
	src->buffer[1] = (JOCTET) JPEG_EOI;
	src->pub.bytes_in_buffer = 2;
	src->pub.next_input_byte = src->buffer;

	return TRUE;
}



//======================================================================
/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * Writers of suspendable-input applications must note that skip_input_data
 * is not granted the right to give a suspension return.  If the skip extends
 * beyond the data currently in the buffer, the buffer can be marked empty so
 * that the next read will cause a fill_input_buffer call that can suspend.
 * Arranging for additional bytes to be discarded before reloading the input
 * buffer is the application writer's problem.
 */

METHODDEF(void)
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	mem_src_ptr src = (mem_src_ptr) cinfo->src;

	//fprintf(stderr, "jmem_src: skip_input_data: %li\n", num_bytes);

	while (num_bytes > (long) src->pub.bytes_in_buffer) {
		// This should never happen.  Entire image should be in buffer.
		num_bytes -= (long) src->pub.bytes_in_buffer;
		(void) fill_input_buffer(cinfo);

		// note we assume that fill_input_buffer will never return FALSE,
		// so suspension need not be handled.
	} // while
	src->pub.next_input_byte += (size_t) num_bytes;
	src->pub.bytes_in_buffer -= (size_t) num_bytes;
}



//======================================================================
/*
 * An additional method that can be provided by data source modules is the
 * resync_to_restart method for error recovery in the presence of RST markers.
 * For the moment, this source module just uses the default resync method
 * provided by the JPEG library.  That method assumes that no backtracking
 * is possible.
 */


/*
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.  Often a no-op.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

METHODDEF(void)
term_source (j_decompress_ptr /*cinfo*/)
{
	/* no work necessary here */
}



//======================================================================
/* jpeg_memory_src: Prepare for input from memory.
**
** The user is responsible for having already allocated the image
** buffer, filling it before calling this decompression module, and will
** be responsible for freeing that buffer when it is no longer being
** used.
**
** cinfo: (in/out) pointer to decompression object
** jfif_buffer: (in) decompression image buffer
** buf_size: (in) maximum size of the jfif_buffer, in bytes.
**
** Note that buf_size is the MAXIMUM size of the buffer, not the size of
** the image.  This is so the same jfif_buffer can be re-filled with a new
** JPEG compressed image, and the same mem_source object can be re-used
** to decompress multiple objects.  However, read the comments below
** concerning the caveat about re-use.
**
*/
GLOBAL(void)
jpeg_memory_src (j_decompress_ptr cinfo,
                 const unsigned char *jfif_buffer,
                 unsigned int buf_size)
{
	mem_src_ptr src;

	// The source object is made permanent so that a series
	// of JPEG images can be read from the same jfif_buffer by calling
	// jpeg_memory_src only before the first one.  (If we discarded the
	// buffer at the end of one image, we'd likely lose the start of
	// the next one.)
	//
	// This makes it unsafe to use this manager and a different source
	// manager serially with the same JPEG object.  Caveat programmer.

	if (cinfo->src == NULL) {     // first time for this JPEG object?
		cinfo->src =
			(struct jpeg_source_mgr *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
			                                                      ((size_t) sizeof(mem_source_mgr)));
	}

	src = (mem_src_ptr) cinfo->src;  // for casting

	// Initialize method pointers
	src->pub.init_source = init_source;
	src->pub.fill_input_buffer = fill_input_buffer;
	src->pub.skip_input_data = skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; // use default method
	src->pub.term_source = term_source;

	// Initialize private members
	src->buffer = (JOCTET*)jfif_buffer;
	src->buffer_size = buf_size;
}

//======================================================================

