/**
 *
 * 
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: A87AWGT $
 * $Date: 2011-12-12 14:28:44 +0100 (Mo, 12 Dez 2011) $
 * $Revision: 11506 $
 *
 * @remarks
 *
 */

/*************************************************************************
* Name:        rice.c
* Author:      Marcus Geelnard
* Description: Rice coder/decoder implementation.
* Reentrant:   Yes
*
* Rice coding is a popular compression method for integers that need many
* bits (e.g. 16 or 32 bits), and the data is such that most of the values
* are close to zero. Although Huffman coding should be optimal, it is not
* a very suitable method due to several reasons (for instance, a 32-bit
* word size would require a 16 GB histogram buffer to encode the Huffman
* tree).
*
* The basic idea behind Rice coding is to store as many words as possible
* with less bits than in the original representation (just as with Huffman
* coding). In fact, one can think of the Rice code as a fixed Huffman code
* (i.e. the codes are not determined by the actual statistical content of
* the data, but by the assumption that lower values are more common than
* higher values).
*
* The coding is very simple: Encode the value X with X '1' bits followed by
* a '0' bit. However, there are some optimizations in this implementation:
*
* 1) The k least significant bits of each word are stored as is, and the
* N-k most significant bits are encoded with Rice coding. k is chosen as
* the average number of bits for the previous few samples in the stream.
* This usually makes the best use of the Rice coding, "hides" noise from
* the Rice coder, and does not result in very long Rice codes for signals
* with varying dynamic range.
*
* 2) If the rice code becomes longer than a fixed threshold, T, an
* alternate coding is used: output T '1' bits, followed by
* floor(log2(X-T)) '1' bits, and one '0' bit, followed by X-T (represented
* by the least significant floor(log2(X-T))-1  bits). This gives pretty
* efficient coding even for large values, and prevents ridiculously long
* Rice codes (in the worst case scenario, a single Rice code for a 32-bit
* word may become as long as 2^32 bits, or 512 MB).
*
* If the threshold is set to 4, then the following is the resulting code
* table:
*
*  X  bin      Rice                  Thresholded Rice     Difference
* ------------------------------------------------------------------
*  0  00000    0                     0
*  1  00001    10                    10
*  2  00010    110                   110
*  3  00011    1110                  1110
*  4  00100    11110                 11110
*  5  00101    111110                111110
*  6  00110    1111110               11111100             +1
*  7  00111    11111110              11111101
*  8  01000    111111110             1111111000           +1
*  9  01001    1111111110            1111111001
* 10  01010    11111111110           1111111010           -1
* 11  01011    111111111110          1111111011           -2
* 12  01100    1111111111110         111111110000
* 13  01101    11111111111110        111111110001         -1
* 14  01110    111111111111110       111111110010         -2
* 15  01111    1111111111111110      111111110011         -3
* 16  10000    11111111111111110     111111110100         -4
* 17  10001    111111111111111110    111111110101         -5
* 18  10010    1111111111111111110   111111110110         -6
* 19  10011    11111111111111111110  111111110111         -7
* 20  10100    111111111111111111110 11111111100000       -5
* ...
*
* As you can see, only two codes result in a worse representation with the
* threshold method used in this implementation. The rest of the codes
* result in shorter or equally short codes as for standard Rice coding.
*
* 3) In the worst case scenario, the output buffer may grow by several
* orders of magnitude compared to the input buffer. Therefor the Rice
* coder in this implementation aborts if the output becomes larger than
* the input by simply making a copy of the input buffer to the output
* buffer, with a leading zero byte (making the output at most one byte
* larger than the input).
*
*-------------------------------------------------------------------------
* @file
 * Copyright (c) 2003-2006 Marcus Geelnard
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
* Marcus Geelnard
* marcus.geelnard at home.se
*************************************************************************/

#include "stdafx.h"
#include "rice.h"

#if defined(_M_IX86) && (_MSC_VER >= 1400)
    #include <intrin.h>
    #pragma intrinsic(_BitScanReverse)
#endif

#if defined(_MSC_VER)
    #define INLINE  __forceinline
#else
    #define INLINE  inline
#endif

/*************************************************************************
* Constants used for Rice coding
*************************************************************************/

/* Number of words to use for determining the optimum k */
#define RICE_HISTORY 16

/* Maximum length of Rice codes */
#define RICE_THRESHOLD 8


/*************************************************************************
* Types used for Rice coding
*************************************************************************/

typedef struct {
    unsigned char *BytePtr;
    unsigned int  Offset;
    unsigned int  NumBytes;
    unsigned int  BitPos;
    unsigned int  Value;
} rice_bitstream_t;


/*************************************************************************
*                           INTERNAL FUNCTIONS                           *
*************************************************************************/

/*************************************************************************
* _Rice_NumBits() - Determine number of information bits in a word.
*************************************************************************/
static bool             s_bsr_init = false;
static unsigned short   s_bsr_table_16[65536];  // 16bit Lookup table

static unsigned int BitScanReverse( unsigned int x )
{
    unsigned int n = 0;
    while (x != 0)
    {
        n++;
        x >>= 1;
    }

    return n;
}

//#if defined(_M_IX86) && (_MSC_VER >= 1400)
//
//static INLINE unsigned int _Rice_NumBits( unsigned int x )
//{
//    unsigned long n;
//    int b = _BitScanReverse(&n, x);
//    return (n+1) & (-b);
//}
//
//#else

static INLINE unsigned int _Rice_NumBits( unsigned int x )
{
    return s_bsr_table_16[x & 0xFFFF];
}

//#endif

/*************************************************************************
* _Rice_InitBitstream() - Initialize a bitstream.
*************************************************************************/

static INLINE void _Rice_InitBitstream( rice_bitstream_t *stream,
                                        void *buf, unsigned int bytes )
{
    stream->BytePtr  = (unsigned char *) buf;
    stream->BitPos   = 0;
    stream->NumBytes = bytes;
    stream->Offset   = 0;
    stream->Value    = 0;
}


/*************************************************************************
* _Rice_ReadBit() - Read a bit from the input stream.
*************************************************************************/

static INLINE int _Rice_ReadBit( rice_bitstream_t *stream )
{
    unsigned int x;

    if (stream->BitPos == 0)
    {
        stream->Value = stream->BytePtr[stream->Offset++];
        stream->BitPos = 8;
    }

    x = (stream->Value & 1);
    stream->Value >>= 1;
    stream->BitPos--;
    return x;
}


/*************************************************************************
* _Rice_ReadBits() - Read a bit from the input stream.
*************************************************************************/

static INLINE int _Rice_ReadBits( rice_bitstream_t *stream, unsigned int n )
{
    unsigned int x;

    while (stream->BitPos < n)
    {
        stream->Value |= (stream->BytePtr[stream->Offset++] << stream->BitPos);
        stream->BitPos += 8;
    }

    x = stream->Value & ~(-1 << n);
    stream->Value >>= n;
    stream->BitPos -= n;
    return x;
}


/*************************************************************************
* _Rice_WriteBit() - Write a bit to the output stream.
*************************************************************************/
#ifdef _M_IX86

static INLINE void _Rice_WriteBit( rice_bitstream_t *stream, unsigned int x )
{
    unsigned int m = ((x & 1) << stream->BitPos) | stream->Value;
    stream->BitPos++;

    if (stream->BitPos == 32)
    {
        *(unsigned int*)(stream->BytePtr + stream->Offset) = m;
        stream->BitPos = 0;
        stream->Value = 0;
        stream->Offset += 4;
    }
    else
    {
        stream->Value = m;
    }
}

static INLINE void _Rice_WriteZeroBit( rice_bitstream_t *stream)
{
    stream->BitPos++;

    if (stream->BitPos == 32)
    {
        *(unsigned int*)(stream->BytePtr + stream->Offset) = stream->Value;
        stream->BitPos = 0;
        stream->Value = 0;
        stream->Offset += 4;
    }
}

#else

static INLINE void _Rice_WriteBit( rice_bitstream_t *stream, unsigned int x )
{
    stream->Value |= (x & 1) << stream->BitPos;
    stream->BitPos++;
}

static INLINE void _Rice_WriteZeroBit( rice_bitstream_t *stream )
{
    stream->BitPos++;
}

#endif

/*************************************************************************
* _Rice_WriteBits() - Write bits to the output stream.
*************************************************************************/
#ifdef _M_IX86

static INLINE void _Rice_WriteOneBits( rice_bitstream_t *stream, unsigned int n )
{
    unsigned int y = ~(-1 << n);
    unsigned int m = (y << stream->BitPos) | stream->Value;
    stream->BitPos += n;

    if (stream->BitPos >= 32)
    {
        *(unsigned int*)(stream->BytePtr + stream->Offset) = m;
        stream->BitPos -= 32;
        m = y >> (n - stream->BitPos);
        stream->Offset += 4;
    }

    stream->Value = m;
}


static INLINE void _Rice_WriteBits( rice_bitstream_t *stream, unsigned int n, unsigned int x )
{
    unsigned int y = x & (~(-1 << n));
    unsigned int m = (y << stream->BitPos) | stream->Value;
    stream->BitPos += n;

    if (stream->BitPos >= 32)
    {
        *(unsigned int*)(stream->BytePtr + stream->Offset) = m;
        stream->BitPos -= 32;
        m = y >> (n - stream->BitPos);
        stream->Offset += 4;
    }

    stream->Value = m;
}

#else

static INLINE void _Rice_WriteBits( rice_bitstream_t *stream, unsigned int n, unsigned int x )
{
    unsigned int m = ((x & (~(-1 << n))) << stream->BitPos) | stream->Value;
    stream->BitPos += n;

    while (stream->BitPos >= 8)
    {
        stream->BytePtr[stream->Offset++] = m;
        stream->BitPos -= 8;
        m >>= 8;
    }

    stream->Value = m;
}

static INLINE void _Rice_WriteOneBits( rice_bitstream_t *stream, unsigned int n )
{
    unsigned int y = ~(-1 << n);
    unsigned int m = (y << stream->BitPos) | stream->Value;
    stream->BitPos += n;

    while (stream->BitPos >= 8)
    {
        stream->BytePtr[stream->Offset++] = m;
        stream->BitPos -= 8;
        m >>= 8;
    }

    stream->Value = m;
}

#endif

/*************************************************************************
* _Rice_FlushStream() - Flushes pending bits to the output stream.
*************************************************************************/

static INLINE void _Rice_FlushStream( rice_bitstream_t *stream )
{
    while (stream->BitPos > 0)
    {
        stream->BytePtr[stream->Offset++] = stream->Value;
        stream->Value >>= 8;
        stream->BitPos -= (stream->BitPos >= 8) ? 8 : stream->BitPos;
    }
}


/*************************************************************************
* _Rice_EncodeWord() - Encode and write a word to the output stream.
*************************************************************************/

static INLINE void _Rice_EncodeWord( unsigned int x, int k,
    rice_bitstream_t *stream )
{
    unsigned int q;
    int          o;

    /* Determine overflow */
    q = x >> k;

    /* Too large rice code? */
    if( q > RICE_THRESHOLD )
    {
        /* Write Rice code (except for the final zero) */
        q -= RICE_THRESHOLD;
        o = _Rice_NumBits( q );
        _Rice_WriteOneBits( stream, o+RICE_THRESHOLD );
        _Rice_WriteZeroBit( stream );

        /* Write the o-1 least significant bits of q "as is" */
        _Rice_WriteBits( stream, o-1, q );
    }
    else
    {
        /* Write Rice code */
        _Rice_WriteOneBits( stream, q );
        _Rice_WriteZeroBit( stream );
    }

    /* Encode the rest of the k bits */
    _Rice_WriteBits( stream, k, x );
}


/*************************************************************************
* _Rice_DecodeWord() - Read and decode a word from the input stream.
*************************************************************************/

static INLINE unsigned int _Rice_DecodeWord( int k, rice_bitstream_t *stream )
{
    unsigned int x, q;
    int          o;

    /* Decode Rice code */
    q = 0;
    while( _Rice_ReadBit( stream ) )
    {
        ++ q;
    }

    /* Too large Rice code? */
    if( q > RICE_THRESHOLD )
    {
        /* Bits needed for the overflow part... */
        o = q - RICE_THRESHOLD;

        /* Read additional bits (MSB is always 1) */
        x = (1 << (o-1)) | _Rice_ReadBits( stream, o-1 );

        /* Add Rice code */
        x += RICE_THRESHOLD;
    }
    else
    {
        x = q;
    }

    /* Decode the rest of the k bits */
    x = (x << k) | _Rice_ReadBits( stream, k );

    return x;
}


/*************************************************************************
* _Rice_ReadWord() - Read a word from the input stream, and convert it to
* a signed magnitude 32-bit representation (regardless of input format).
*************************************************************************/

template<typename T>
static INLINE 
unsigned int _Rice_ReadWord( T* ptr, unsigned int idx )
{
    return ptr[idx];
}

static INLINE 
unsigned int _Rice_ReadWord( signed short* ptr, unsigned int idx )
{
    int          sx = ptr[ idx ];
    unsigned int x = (sx<<1) ^ (sx >> 31);
    return x;
}

static INLINE 
unsigned int _Rice_ReadWord( signed char* ptr, unsigned int idx )
{
    int          sx = ptr[ idx ];
    unsigned int x = (sx<<1) ^ (sx >> 31);
    return x;
}

/*************************************************************************
* _Rice_WriteWord() - Convert a signed magnitude 32-bit word to the given
* format, and write it to the otuput stream.
*************************************************************************/

template<typename T>
static INLINE 
void _Rice_WriteWord( T *ptr, unsigned int idx, unsigned int x )
{
    ptr[ idx ] = x;
}

static INLINE 
void _Rice_WriteWord( signed short *ptr, unsigned int idx, unsigned int x )
{
    int sx = (0 - ((int)x & 1)) ^ (int)(x>>1);
    ptr[ idx ] = sx;
}

static INLINE 
void _Rice_WriteWord( signed char *ptr, unsigned int idx, unsigned int x )
{
    int sx = (0 - ((int)x & 1)) ^ (int)(x>>1);
    ptr[ idx ] = sx;
}

/*************************************************************************
*                            PUBLIC FUNCTIONS                            *
*************************************************************************/


/*************************************************************************
* Rice_Compress() - Compress a block of data using a Rice coder.
*  in     - Input (uncompressed) buffer.
*  out    - Output (compressed) buffer. This buffer must one byte larger
*           than the input buffer.
*  insize - Number of input bytes.
* The function returns the size of the compressed data.
*************************************************************************/
template <typename T>
int rice_Compress( T *in, void *out, unsigned int insize )
{
    if (!s_bsr_init)
    {
        for (int i = 0;  i < 65536;  i++)
        {
            s_bsr_table_16[i] = BitScanReverse(i);
        }

        s_bsr_init = true;
    }

    rice_bitstream_t stream;
    unsigned int     i, x, k, n, wordsize, incount, ksum;
    unsigned int     hist[ RICE_HISTORY ];

    wordsize = sizeof(T) * 8;
    incount = insize / (wordsize>>3);

    /* Do we have anything to compress? */
    if( incount == 0 )
    {
        return 0;
    }

    /* Initialize output bitsream */
    _Rice_InitBitstream( &stream, out, insize+1 );

    /* Determine a good initial k */
    ksum = 0;
    for( i = 0; (i < RICE_HISTORY) && (i < incount); ++ i )
    {
        n = _Rice_NumBits( _Rice_ReadWord( in, i ) );
        hist[ i % RICE_HISTORY ] = n;
        ksum += n;
    }
    k = (ksum + (RICE_HISTORY>>1)) / RICE_HISTORY;
    if( k == 0 ) k = 1;

    /* Write k to the output stream (the decoder needs it) */
    _Rice_WriteBits( &stream, 8, k );

    /* Encode input stream */
    for( i = 0; (i < incount) && (stream.Offset < insize); ++ i )
    {
        /* Read word from input buffer */
        x = _Rice_ReadWord( in, i );

        /* Encode word to output buffer */
        _Rice_EncodeWord( x, k, &stream );

        /* Update history */
        /* Revise optimum k? */
        if( i >= RICE_HISTORY )
        {
            n = _Rice_NumBits( x );
            ksum += n;
            ksum -= hist[ i % RICE_HISTORY ];
            hist[ i % RICE_HISTORY ] = n;
            k = (ksum + (RICE_HISTORY>>1)) / RICE_HISTORY;
        }
    }

    /* Was there a buffer overflow? */
    if( i < incount )
    {
        /* Rewind bitstream and fill it with raw words */
        _Rice_InitBitstream( &stream, out, insize+1 );

        /* Indicate that the buffer was not compressed */
        _Rice_WriteBits( &stream, 8, 0 );

        for( i = 0; i < incount; ++ i )
        {
            x = _Rice_ReadWord( in, i );
            _Rice_WriteBits( &stream, wordsize, x );
        }
    }

    _Rice_FlushStream( &stream );
    return stream.Offset;
}

/*************************************************************************
* Instantiation of template functions (see rice.h)
*************************************************************************/
template int 
rice_Compress<signed char>( signed char* in, void* out, unsigned int insize );
template int 
rice_Compress<unsigned char>( unsigned char* in, void* out, unsigned int insize );
template int 
rice_Compress<signed short>( signed short* in, void* out, unsigned int insize );
template int 
rice_Compress<unsigned short>( unsigned short* in, void* out, unsigned int insize );
