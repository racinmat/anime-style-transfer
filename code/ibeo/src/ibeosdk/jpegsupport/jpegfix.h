//======================================================================

#ifndef JPEGFIX_H_SEEN
#define JPEGFIX_H_SEEN

//======================================================================

#if JPEG_LIB_VERSION <= 60
#undef GLOBAL
#define GLOBAL(type) type
#undef METHODDEF
#define METHODDEF(type) type
#endif // JPEG_LIB_VERSION <= 60

//======================================================================

#endif // JPEGFIX_H_SEEN

//======================================================================
