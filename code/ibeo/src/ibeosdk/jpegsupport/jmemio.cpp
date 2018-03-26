/* memio.c: routines that use the memory destination and source module
** BASED ON example.c
**/

//======================================================================

/*
 * example.c
 *
 * This file illustrates how to use the IJG code as a subroutine library
 * to read or write JPEG image files.  You should look at this code in
 * conjunction with the documentation file libjpeg.doc.
 *
 * This code will not do anything useful as-is, but it may be helpful as a
 * skeleton for constructing routines that call the JPEG library.
 *
 * We present these routines in the same coding style used in the JPEG code
 * (ANSI function definitions, etc); but you are of course free to code your
 * routines in a different style if you prefer.
 */

//======================================================================

#include <stdio.h>
#include <string.h>

//======================================================================

#ifdef ibeoapi_STATIC
# define JPEG_STATIC
#endif

//======================================================================
/*
 * Include file for users of JPEG library.
 * You will need to have included system headers that define at least
 * the typedefs FILE and size_t before you can include jpeglib.h.
 * (stdio.h is sufficient on ANSI-conforming systems.)
 * You may also wish to include "jerror.h".
 */

#include "jpeglib.h"

/*
 * <setjmp.h> is used for the optional error recovery mechanism shown in
 * the second part of the example.
 */
#include <setjmp.h>


// Memory objects
#include "jmem_dest.h"
#include "jmem_src.h"


// Include this module's header for prototype checking
#include "jmemio.h"

#include <ibeosdk/Log.hpp>

/* ************************************************************ */
/* IbeoAPI handler for error message output. Code copied from jerror.c
 * line 97ff. */
METHODDEF(void)
ibeoapi_output_message (j_common_ptr cinfo)
{
	char buffer[JMSG_LENGTH_MAX];

	// Create the message
	(*cinfo->err->format_message) (cinfo, buffer);

/* #ifdef USE_WINDOWS_MESSAGEBOX */
/*   * Display it in a message dialog box * */
/*   MessageBox(GetActiveWindow(), buffer, "JPEG Library Error", */
/* 	     MB_OK | MB_ICONERROR); */
/* #else */
/*   * Send it to stderr, adding a newline * */
/*   fprintf(stderr, "%s\n", buffer); */
/* #endif */


	// Yuk. Indeed the default implementation of jpeg62.dll will pop up
	// an error message on windows (see above). Don't do that here, but
	// instead copy the message to our trace framework.
	ibeosdk::logWarning << buffer;
}
//======================================================================


//======================================================================
//                     JPEG COMPRESSION INTERFACE
//======================================================================
/* This half of the example shows how to feed data into the JPEG compressor.
 * We present a minimal version that does not worry about refinements such
 * as error recovery (the JPEG code will just exit() if it gets an error).
 */


/*
 * IMAGE DATA FORMATS:
 *
 * The standard input image format is a rectangular array of pixels, with
 * each pixel having the same number of "component" values (color channels).
 * Each pixel row is an array of JSAMPLEs (which typically are unsigned chars).
 * If you are working with color data, then the color values for each pixel
 * must be adjacent in the row; for example, R,G,B,R,G,B,R,G,B,... for 24-bit
 * RGB color.
 *
 * For this example, we'll assume that this data structure matches the way
 * our application has stored the image in memory, so we can just pass a
 * pointer to our image buffer.  In particular, let's say that the image is
 * RGB color.
 */


/* Assume 3 channel format data, with 1 byte per channel */
#define BYTES_PER_SAMPLE 3



//======================================================================
/*!\brief Store JPEG compressed image into memory.
 * \param[in]      img_buf           Pointer to uncompressed source image data in RGB format
 * \param[in]      width             Width of source image in pixels.
 * \param[in]      height            Height of source image in pixels.
 * \param[in, out] jpeg_buffer       Pointer to compressed image data buffer.
 * \param[in]      jpeg_buffer_size  Maximum size of the compressed image data buffer.
 * \param[in]      quality           A compression quality factor.
 * \param[out]     jpeg_comp_size    Size of the destination compressed image, in bytes.
 * \note Note that the "compressed image" has the entire JFIF formatted
 *       file structure, including header info.
 *///-------------------------------------------------------------------
GLOBAL(void)
writeJpegToMemory(unsigned char* img_buf, const unsigned int width, const unsigned int height,
                  unsigned char* jpeg_buffer, const unsigned int jpeg_buffer_size,
                  const int quality, unsigned int* jpeg_comp_size)
{
	//jpeg_compress_struct contains the JPEG compression parameters
	//and pointers to working space (which is allocated as needed by
	//the JPEG library).  It is possible to have several such
	//structures, representing multiple compression/decompression
	//processes, in existence at once.  We refer to any one struct
	//(and its associated working data) as a "JPEG object".
	struct jpeg_compress_struct cinfo;

	//jpeg_error_mgr represents a JPEG error handler.  It is declared
	//separately because applications often want to supply a
	//specialized error handler (see the second half of this file for
	//an example).  But here we just take the easy way out and use the
	//standard error handler, which will print a message on stderr and
	//call exit() if compression fails.  Note that this struct must
	//live as long as the main JPEG parameter struct, to avoid
	//dangling-pointer problems.
	struct jpeg_error_mgr jerr;

	// More stuff
	JSAMPROW row_pointer[1];      /* pointer to JSAMPLE row[s] */
	unsigned int row_stride;               /* physical row width in image buffer */


	// Step 1: allocate and initialize JPEG compression object

	// We have to set up the error handler first, in case the initialization
	// step fails.  (Unlikely, but it could happen if you are out of memory.)
	// This routine fills in the contents of struct jerr, and returns jerr's
	// address which we place into the link field in cinfo.
	cinfo.err = jpeg_std_error(&jerr);
	jerr.output_message = ibeoapi_output_message;

	// Now we can initialize the JPEG compression object.
	jpeg_create_compress(&cinfo);


	// (Note: steps 2 and 3 can be done in either order.)

	// Step 2: specify data destination.
	jpeg_memory_dest(&cinfo, jpeg_buffer, jpeg_buffer_size);


	// Step 3: set parameters for compression

	// First we supply a description of the input image.
	// Four fields of the cinfo struct must be filled in:
	cinfo.image_width = JDIMENSION(width);      /* image width in pixels */
	cinfo.image_height = JDIMENSION(height);    /* image height in pixels */
	/* Assume that input_components is the same as BYTES_PER_SAMPLE */
	cinfo.input_components = 3;     /* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; /* colorspace of input image */

	// Now use the library's routine to set default compression parameters.
	// (You must set at least cinfo.in_color_space before calling this,
	// since the defaults depend on the source color space.)
	jpeg_set_defaults(&cinfo);

	// Now you can set any non-default parameters you wish to.
	// Here we just illustrate the use of quality (quantization table) scaling:
	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);


	// Step 4: Start compressor

	// TRUE ensures that we will write a complete interchange-JPEG file.
	// Pass TRUE unless you are very sure of what you're doing.
	jpeg_start_compress(&cinfo, TRUE);


	// Step 5: while (scan lines remain to be written)
	//           jpeg_write_scanlines(...);

	// Here we use the library's state variable cinfo.next_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	// To keep things simple, we pass one scanline per call; you can pass
	// more if you wish, though.
	row_stride = width * 3; // JSAMPLEs per row in image_buffer

	while (cinfo.next_scanline < cinfo.image_height) {
		// jpeg_write_scanlines expects an array of pointers to scanlines.
		// Here the array is only one element long, but you could pass
		// more than one scanline at a time if that's more convenient.
		row_pointer[0] = & img_buf[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	} // while


	// Step 6: Finish compression

	jpeg_finish_compress(&cinfo);


	// Compute the size of the compressed image.
	//
	// This must be done after the image is done compressing, (i.e.
	// after jpeg_finish_compress) but before jpeg_destroy_compress,
	// because jpeg_destroy_compress frees the memory containing the
	// information we want from the mem_dest_ptr within the cinfo
	// structure.
	{
		// mem_dest_ptr is a pointer to the memory destination object.
		// From it, the total size of the compressed data can be
		// determined.
		mem_dest_ptr dest = (mem_dest_ptr) cinfo.dest;
		*jpeg_comp_size = dest->buffer_size - (unsigned int)(dest->pub.free_in_buffer);
	}


	// Step 7: release JPEG compression object

	// This is an important step since it will release a good deal of memory.
	jpeg_destroy_compress(&cinfo);


	// Now it is expected that some external routine will do something
	// useful with the compressed data, such as send it over a network, or
	// write it to a file (although there is a stdio destiation module that
	// writes the data to a file in a more elegant way.)
	//
	// Also, it is expected that some external routine will free the image
	// buffer once it's done being used.
}

//======================================================================

/*
 * SOME FINE POINTS:
 *
 * In the above loop, we ignored the return value of jpeg_write_scanlines,
 * which is the number of scanlines actually written.  We could get away
 * with this because we were only relying on the value of cinfo.next_scanline,
 * which will be incremented correctly.  If you maintain additional loop
 * variables then you should be careful to increment them properly.

   sic, from example.c: -- MJG 06jun96
 * Actually, for output to a stdio stream you needn't worry, because
 * then jpeg_write_scanlines will write all the lines passed (or else exit
 * with a fatal error).  Partial writes can only occur if you use a data
 * destination module that can demand suspension of the compressor.
 * (If you don't know what that's for, you don't need it.)

 *
 * If the compressor requires full-image buffers (for entropy-coding
 * optimization or a multi-scan JPEG file), it will create temporary
 * files for anything that doesn't fit within the maximum-memory setting.
 * (Note that temp files are NOT needed if you use the default parameters.)
 * On some systems you may need to set up a signal handler to ensure that
 * temporary files are deleted if the program is interrupted.  See libjpeg.doc.
 *
 * Scanlines MUST be supplied in top-to-bottom order if you want your JPEG
 * files to be compatible with everyone else's.  If you cannot readily read
 * your data in that order, you'll need an intermediate array to hold the
 * image.  See rdtarga.c or rdbmp.c for examples of handling bottom-to-top
 * source data using the JPEG code's internal virtual-array mechanisms.
 */




//======================================================================
//                    JPEG DECOMPRESSION INTERFACE
//======================================================================

/* This half of the example shows how to read data from the JPEG decompressor.
 * It's a bit more refined than the above, in that we show:
 *   (a) how to modify the JPEG library's standard error-reporting behavior;
 *   (b) how to allocate workspace using the library's memory manager.
 *
 * Just to make this example a little different from the first one, we'll
 * assume that we do not intend to put the whole image into an in-memory
 * buffer, but to send it line-by-line someplace else.  We need a one-
 * scanline-high JSAMPLE array as a work buffer, and we will let the JPEG
 * memory manager allocate it for us.  This approach is actually quite useful
 * because we don't need to remember to deallocate the buffer separately: it
 * will go away automatically when the JPEG object is cleaned up.
 */


/*
 * ERROR HANDLING:
 *
 * The JPEG library's standard error handler (jerror.c) is divided into
 * several "methods" which you can override individually.  This lets you
 * adjust the behavior without duplicating a lot of code, which you might
 * have to update with each future release.
 *
 * Our example here shows how to override the "error_exit" method so that
 * control is returned to the library's caller when a fatal error occurs,
 * rather than calling exit() as the standard error_exit method does.
 *
 * We use C's setjmp/longjmp facility to return control.  This means that the
 * routine which calls the JPEG library must first execute a setjmp() call to
 * establish the return point.  We want the replacement error_exit to do a
 * longjmp().  But we need to make the setjmp buffer accessible to the
 * error_exit routine.  To do this, we make a private extension of the
 * standard JPEG error handler object.  (If we were using C++, we'd say we
 * were making a subclass of the regular error handler.)
 *
 * Here's the extended error handler struct:
 */

struct my_error_mgr {
	struct jpeg_error_mgr pub;    /* "public" fields */

	jmp_buf setjmp_buffer;        /* for return to caller */
}; // my_error_mgr

typedef struct my_error_mgr * my_error_ptr;


//======================================================================
/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
	// cinfo->err really points to a my_error_mgr struct, so coerce pointer
	my_error_ptr myerr = (my_error_ptr) cinfo->err;

	// Always display the message.
	// We could postpone this until after returning, if we chose.
	(*cinfo->err->output_message) (cinfo);

	// Return control to the setjmp point
	longjmp(myerr->setjmp_buffer, 1);
}


//======================================================================
/*
 * Sample routine for JPEG decompression.  We assume that the source file name
 * is passed in.  We want to return 1 on success, 0 on error.
 */


/* JPEG DHT Segment for YCrCb omitted from MJPEG data */
static unsigned char jpeg_odml_dht[0x1a4] = {
	0xff, 0xc4, 0x01, 0xa2,

	0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,

	0x01, 0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,

	0x10, 0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7d,
	0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
	0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
	0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
	0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
	0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
	0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
	0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
	0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
	0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa,

	0x11, 0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77,
	0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
	0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
	0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
	0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
	0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
	0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
	0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
	0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa
}; // jpeg_odml_dht

//======================================================================
/*
 * Parse the DHT table.
 * This code comes from jpeg6b (jdmarker.c).
 */
static int jpeg_load_dht(struct jpeg_decompress_struct* info,
                         unsigned char* dht,
                         JHUFF_TBL* ac_tables[],
                         JHUFF_TBL* dc_tables[])
{
	unsigned int length = (unsigned int)((dht[2] << 8) + dht[3] - 2);
	unsigned int pos = 4;
	unsigned int count, i;
	int index;

	JHUFF_TBL **hufftbl;
	unsigned char bits[17];
	unsigned char huffval[256];

	while (length > 16)
	{
		bits[0] = 0;
		index = dht[pos++];
		count = 0;

		for (i = 1; i <= 16; ++i)
		{
			bits[i] = dht[pos++];
			count += bits[i];
		}

		length -= 17;

		if (count > 256 || count > length)
		{
			return -1;
		}

		for (i = 0; i < count; ++i)
		{
			huffval[i] = dht[pos++];
		}

		length -= count;

		if (index & 0x10)
		{
			index -= 0x10;
			hufftbl = &ac_tables[index];
		}
		else
		{
			hufftbl = &dc_tables[index];
		}

		if (index < 0 || index >= NUM_HUFF_TBLS)
		{
			return -1;
		}

		if (*hufftbl == NULL)
		{
			*hufftbl = jpeg_alloc_huff_table ((j_common_ptr)info);
		}
		if (*hufftbl == NULL)
		{
			return -1;
		}

		memcpy ((*hufftbl)->bits, bits, sizeof (*hufftbl)->bits);
		memcpy ((*hufftbl)->huffval, huffval, sizeof (*hufftbl)->huffval);
	}

	if (length != 0)
	{
		return -1;
	}

	return 0;
}



//======================================================================
/*!\brief Read JPEG from memory
 * \param[out] img_buf          Point to uncompressed image data in RGB format
 * \param[out] width            Width of image in pixels.
 * \param[out] height           Height of image in pixels.
 * \param[in] jpeg_buffer       Pointer to compressed image data buffer.
 * \param[in] jpeg_buffer_size  Maximum size of the compressed image data buffer.
 * \return 1 on success
 *///-------------------------------------------------------------------
GLOBAL(int)
readJpegFromMemory(unsigned char* img_buf, unsigned int* width, unsigned int* height,
                   const unsigned char* jpeg_buffer, const unsigned int jpeg_buffer_size)
{
	// This struct contains the JPEG decompression parameters and pointers to
	// working space (which is allocated as needed by the JPEG library).
	struct jpeg_decompress_struct cinfo;

	// We use our private extension JPEG error handler.
	// Note that this struct must live as long as the main JPEG parameter
	// struct, to avoid dangling-pointer problems.
	struct my_error_mgr jerr;

	unsigned int row_stride;               /* physical row width in output buffer */
	JSAMPROW row_pointer[1];      /* pointer to JSAMPLE row[s] */



	// Step 1: allocate and initialize JPEG decompression object

	// We set up the normal JPEG error routines, then override error_exit.
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	jerr.pub.output_message = ibeoapi_output_message;

	// Establish the setjmp return context for my_error_exit to use.
	if (setjmp(jerr.setjmp_buffer)) {
		// If we get here, the JPEG code has signaled an error.
		// We need to clean up the JPEG object, close the input file, and return.
		jpeg_destroy_decompress(&cinfo);
		return 0;
	}

	// Now we can initialize the JPEG decompression object.
	jpeg_create_decompress(&cinfo);


	// Step 2: specify data source
	jpeg_memory_src(&cinfo, jpeg_buffer, jpeg_buffer_size);


	// Step 3: read JFIF parameters with jpeg_read_header()

	(void) jpeg_read_header(&cinfo, TRUE);
	// We can ignore the return value from jpeg_read_header since
	//   (a) suspension is not possible with the memory data source, and
	//  (b) we passed TRUE to reject a tables-only JPEG file as an error.
	// See libjpeg.doc for more info.

	// Step 3b: Check whether DHT is available. Otherwise inject DHT specified by ODML.

	// This should make MJPEG-Files readable by the libjpeg library.
	// Warning: This might not work for all MJPEG-Streams, e.g. interlaced ones.

	if ( cinfo.ac_huff_tbl_ptrs[0] == NULL && cinfo.ac_huff_tbl_ptrs[1] == NULL && cinfo.dc_huff_tbl_ptrs[0] == NULL && cinfo.dc_huff_tbl_ptrs[1] == NULL )
	{
		jpeg_load_dht( &cinfo, jpeg_odml_dht, cinfo.ac_huff_tbl_ptrs, cinfo.dc_huff_tbl_ptrs );
	}

	// Step 4: set parameters for decompression

	// In this example, we don't need to change any of the defaults set by
	// jpeg_read_header(), so we do nothing here.

	//cinfo.dct_method=JDCT_IFAST;


	// Step 5: Start decompressor

	(void) jpeg_start_decompress(&cinfo);

	// We can ignore the return value since suspension is not possible
	// with the memory data source.

	// We may need to do some setup of our own at this point before reading
	// the data.  After jpeg_start_decompress() we have the correct scaled
	// output image dimensions available, as well as the output colormap
	// if we asked for color quantization.

	// compute JSAMPLEs per row in output buffer
	row_stride = cinfo.output_width * (unsigned int)cinfo.output_components;

	// Set the width and height arguments
	*width = cinfo.output_width;
	*height = cinfo.output_height;



	// Step 6: while (scan lines remain to be read)
	//           jpeg_read_scanlines(...);

	// Here we use the library's state variable cinfo.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	while (cinfo.output_scanline < cinfo.output_height) {
		// jpeg_read_scanlines expects an array of pointers to scanlines.
		// Here the array is only one element long, but you could ask for
		// more than one scanline at a time if that's more convenient.
		row_pointer[0] = &img_buf[cinfo.output_scanline * row_stride];
		(void) jpeg_read_scanlines(&cinfo, row_pointer, 1);
	}


	// Step 7: Finish decompression

	(void) jpeg_finish_decompress(&cinfo);
	// We can ignore the return value since suspension is not possible
	// with the stdio data source.


	// Step 8: Release JPEG decompression object

	// This is an important step since it will release a good deal of memory.
	jpeg_destroy_decompress(&cinfo);



	// At this point you may want to check to see whether any corrupt-data
	// warnings occurred (test whether jerr.pub.num_warnings is nonzero).


	// And we're done!
	return 1;
}

//======================================================================
/*!\brief Read JPEG from memory
 * \param[out] img_buf          Point to uncompressed image data in RGB format
 * \param[out] width            Width of image in pixels.
 * \param[out] height           Height of image in pixels.
 * \param[in] jpeg_buffer       Pointer to compressed image data buffer.
 * \param[in] jpeg_buffer_size  Maximum size of the compressed image data buffer.
 * \return 1 on success
 *///-------------------------------------------------------------------
GLOBAL(int)
readJpegFromMemoryPixelFormat(unsigned char* img_buf, unsigned int* width, unsigned int* height,
                   const J_COLOR_SPACE outputColorSpace,
                   const int nbOfOutputColorComponents,
                   const unsigned char* jpeg_buffer, const unsigned int jpeg_buffer_size)
{
	// This struct contains the JPEG decompression parameters and pointers to
	// working space (which is allocated as needed by the JPEG library).
	struct jpeg_decompress_struct cinfo;

	// We use our private extension JPEG error handler.
	// Note that this struct must live as long as the main JPEG parameter
	// struct, to avoid dangling-pointer problems.
	struct my_error_mgr jerr;

	unsigned int row_stride;               /* physical row width in output buffer */
	JSAMPROW row_pointer[1];      /* pointer to JSAMPLE row[s] */



	// Step 1: allocate and initialize JPEG decompression object

	// We set up the normal JPEG error routines, then override error_exit.
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	jerr.pub.output_message = ibeoapi_output_message;

	// Establish the setjmp return context for my_error_exit to use.
	if (setjmp(jerr.setjmp_buffer)) {
		// If we get here, the JPEG code has signaled an error.
		// We need to clean up the JPEG object, close the input file, and return.
		jpeg_destroy_decompress(&cinfo);
		return 0;
	}

	// Now we can initialize the JPEG decompression object.
	jpeg_create_decompress(&cinfo);


	// Step 2: specify data source
	jpeg_memory_src(&cinfo, jpeg_buffer, jpeg_buffer_size);


	// Step 3: read JFIF parameters with jpeg_read_header()

	(void) jpeg_read_header(&cinfo, TRUE);
	// We can ignore the return value from jpeg_read_header since
	//   (a) suspension is not possible with the memory data source, and
	//  (b) we passed TRUE to reject a tables-only JPEG file as an error.
	// See libjpeg.doc for more info.

	// Step 3b: Check whether DHT is available. Otherwise inject DHT specified by ODML.

	// This should make MJPEG-Files readable by the libjpeg library.
	// Warning: This might not work for all MJPEG-Streams, e.g. interlaced ones.

	if ( cinfo.ac_huff_tbl_ptrs[0] == NULL && cinfo.ac_huff_tbl_ptrs[1] == NULL && cinfo.dc_huff_tbl_ptrs[0] == NULL && cinfo.dc_huff_tbl_ptrs[1] == NULL )
	{
		jpeg_load_dht( &cinfo, jpeg_odml_dht, cinfo.ac_huff_tbl_ptrs, cinfo.dc_huff_tbl_ptrs );
	}

	// Step 4: set parameters for decompression

	// In this example, we don't need to change any of the defaults set by
	// jpeg_read_header(), so we do nothing here.

	//cinfo.dct_method=JDCT_IFAST;


	// Step 5: Start decompressor

	(void) jpeg_start_decompress(&cinfo);

	cinfo.out_color_space = outputColorSpace;
	cinfo.out_color_components = nbOfOutputColorComponents;
	cinfo.output_components = nbOfOutputColorComponents;

	// We can ignore the return value since suspension is not possible
	// with the memory data source.

	// We may need to do some setup of our own at this point before reading
	// the data.  After jpeg_start_decompress() we have the correct scaled
	// output image dimensions available, as well as the output colormap
	// if we asked for color quantization.

	// compute JSAMPLEs per row in output buffer
	row_stride = cinfo.output_width * (unsigned int)cinfo.output_components;

	// Set the width and height arguments
	*width = cinfo.output_width;
	*height = cinfo.output_height;



	// Step 6: while (scan lines remain to be read)
	//           jpeg_read_scanlines(...);

	// Here we use the library's state variable cinfo.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	while (cinfo.output_scanline < cinfo.output_height) {
		// jpeg_read_scanlines expects an array of pointers to scanlines.
		// Here the array is only one element long, but you could ask for
		// more than one scanline at a time if that's more convenient.
		row_pointer[0] = &img_buf[cinfo.output_scanline * row_stride];
		(void) jpeg_read_scanlines(&cinfo, row_pointer, 1);
	}


	// Step 7: Finish decompression

	(void) jpeg_finish_decompress(&cinfo);
	// We can ignore the return value since suspension is not possible
	// with the stdio data source.


	// Step 8: Release JPEG decompression object

	// This is an important step since it will release a good deal of memory.
	jpeg_destroy_decompress(&cinfo);



	// At this point you may want to check to see whether any corrupt-data
	// warnings occurred (test whether jerr.pub.num_warnings is nonzero).


	// And we're done!
	return 1;
}

//======================================================================
/*
 * SOME FINE POINTS:
 *
 * In the above code, we ignored the return value of jpeg_read_scanlines,
 * which is the number of scanlines actually read.  We could get away with
 * this because we asked for only one line at a time and we weren't using
 * a suspending data source.  See libjpeg.doc for more info.
 *
 * We cheated a bit by calling alloc_sarray() after jpeg_start_decompress();
 * we should have done it beforehand to ensure that the space would be
 * counted against the JPEG max_memory setting.  In some systems the above
 * code would risk an out-of-memory error.  However, in general we don't
 * know the output image dimensions before jpeg_start_decompress(), unless we
 * call jpeg_calc_output_dimensions().  See libjpeg.doc for more about this.
 *
 * Scanlines are returned in the same order as they appear in the JPEG file,
 * which is standardly top-to-bottom.  If you must emit data bottom-to-top,
 * you can use one of the virtual arrays provided by the JPEG memory manager
 * to invert the data.  See wrbmp.c for an example.
 *
 * As with compression, some operating modes may require temporary files.
 * On some systems you may need to set up a signal handler to ensure that
 * temporary files are deleted if the program is interrupted.  See libjpeg.doc.
 */



/* read_JPEG_memory2:
**
** img_buf: (out) pointer to uncompressed image data in RGB format
** buffer_size: (in) maximum size of the source image buffer, in bytes
** width: (out) width of image in pixels
** height: (out) height of image in pixels
** jpeg_buffer: (in) pointer to compressed image data buffer
** jpeg_buffer_size: (in) maximum size of the compressed image data buffer
*/
GLOBAL(int)
readJpegFromMemory2 (unsigned char* img_buf, unsigned int* width, unsigned int* height,
                   const unsigned char* jpeg_buffer, const unsigned int jpeg_buffer_size,
                   const int nullConv)
{
	// This struct contains the JPEG decompression parameters and pointers to
	// working space (which is allocated as needed by the JPEG library).
	struct jpeg_decompress_struct cinfo;

	// We use our private extension JPEG error handler.
	// Note that this struct must live as long as the main JPEG parameter
	// struct, to avoid dangling-pointer problems.
	struct my_error_mgr jerr;

	unsigned int row_stride;      // physical row width in output buffer
	JSAMPROW row_pointer[1];      // pointer to JSAMPLE row[s]


	// Step 1: allocate and initialize JPEG decompression object

	// We set up the normal JPEG error routines, then override error_exit.
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	jerr.pub.output_message = ibeoapi_output_message;

	// Establish the setjmp return context for my_error_exit to use.
	if (setjmp(jerr.setjmp_buffer)) {
		// If we get here, the JPEG code has signaled an error.
		// We need to clean up the JPEG object, close the input file, and return.
		jpeg_destroy_decompress(&cinfo);
		return 0;
	}

	// Now we can initialize the JPEG decompression object.
	jpeg_create_decompress(&cinfo);


	// Step 2: specify data source
	jpeg_memory_src(&cinfo, jpeg_buffer, jpeg_buffer_size);


	// Step 3: read JFIF parameters with jpeg_read_header()

	(void) jpeg_read_header(&cinfo, TRUE);
	// We can ignore the return value from jpeg_read_header since
	//   (a) suspension is not possible with the memory data source, and
	//   (b) we passed TRUE to reject a tables-only JPEG file as an error.
	// See libjpeg.doc for more info.

	// Step 3b: Check whether DHT is available. Otherwise inject DHT specified by ODML.

	// This should make MJPEG-Files readable by the libjpeg library.
	// Warning: This might not work for all MJPEG-Streams, e.g. interlaced ones.

	if ( cinfo.ac_huff_tbl_ptrs[0] == NULL && cinfo.ac_huff_tbl_ptrs[1] == NULL && cinfo.dc_huff_tbl_ptrs[0] == NULL && cinfo.dc_huff_tbl_ptrs[1] == NULL )
	{
		jpeg_load_dht( &cinfo, jpeg_odml_dht, cinfo.ac_huff_tbl_ptrs, cinfo.dc_huff_tbl_ptrs );
	}

	// Step 4: set parameters for decompression

	// In this example, we don't need to change any of the defaults set by
	// jpeg_read_header(), so we do nothing here.
	if (nullConv)
		cinfo.out_color_space=cinfo.jpeg_color_space;

	// enable the next line to gain about 10 percent decompression speed
	// //cinfo.dct_method=JDCT_IFAST;

	// Step 5: Start decompressor

	(void) jpeg_start_decompress(&cinfo);
	// We can ignore the return value since suspension is not possible
	// with the memory data source.

	// We may need to do some setup of our own at this point before reading
	// the data.  After jpeg_start_decompress() we have the correct scaled
	// output image dimensions available, as well as the output colormap
	// if we asked for color quantization.

	// compute JSAMPLEs per row in output buffer
	row_stride = cinfo.output_width * (unsigned int)cinfo.output_components;

	// Set the width and height arguments
	*width = cinfo.output_width;
	*height = cinfo.output_height;

	// Step 6: while (scan lines remain to be read)
	//              jpeg_read_scanlines(...);

	// Here we use the library's state variable cinfo.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	while (cinfo.output_scanline < cinfo.output_height) {
		// jpeg_read_scanlines expects an array of pointers to scanlines.
		// Here the array is only one element long, but you could ask for
		// more than one scanline at a time if that's more convenient.
		row_pointer[0] = & img_buf[cinfo.output_scanline * row_stride];
		(void) jpeg_read_scanlines(&cinfo, row_pointer, 1);
	}


	// Step 7: Finish decompression

	(void) jpeg_finish_decompress(&cinfo);
	// We can ignore the return value since suspension is not possible
	// with the stdio data source.


	// Step 8: Release JPEG decompression object

	// This is an important step since it will release a good deal of memory.
	jpeg_destroy_decompress(&cinfo);



	// At this point you may want to check to see whether any corrupt-data
	// warnings occurred (test whether jerr.pub.num_warnings is nonzero).

	// And we're done!
	return 1;
}

//======================================================================
