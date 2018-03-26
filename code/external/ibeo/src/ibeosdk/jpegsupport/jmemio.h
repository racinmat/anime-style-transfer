/* memio.c: routines that use the memory destination and source module
** BASED ON example.c
**/

//======================================================================

#ifndef JMEMIO_H_SEEN
#define JMEMIO_H_SEEN

//======================================================================

#include <sys/types.h>
#include <stdio.h>

#include "jpeglib.h"

//======================================================================

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern GLOBAL(void)
writeJpegToMemory(unsigned char* img_buf, const unsigned int width, const unsigned int height,
                  unsigned char* jpeg_buffer, const unsigned int jpeg_buffer_size,
                  const int quality, unsigned int* jpeg_comp_size);

extern GLOBAL(int)
readJpegFromMemory(unsigned char* img_buf, unsigned int* width, unsigned int* height,
                   const unsigned char* jpeg_buffer, const unsigned int jpeg_buffer_size);

extern GLOBAL(int)
readJpegFromMemoryPixelFormat(unsigned char* img_buf, unsigned int* width, unsigned int* height,
                   const J_COLOR_SPACE outputColorSpace,
                   const int nbOfOutputColorComponents,
                   const unsigned char* jpeg_buffer, const unsigned int jpeg_buffer_size);

extern GLOBAL(int)
readJpegFromMemory2(unsigned char* img_buf, unsigned int* width, unsigned int* height,
                    const unsigned char* jpeg_buffer, const unsigned int jpeg_buffer_size,
                    const int nullConv);

//======================================================================

#ifdef __cplusplus
} // extern "C"
#endif /* __cplusplus */

//======================================================================

#endif // JMEMIO_H_SEEN

//======================================================================

