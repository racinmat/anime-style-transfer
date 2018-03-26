/**
 *
 * 
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: belkera $
 * $Date: 2009-11-25 11:14:32 +0100 (Mi, 25 Nov 2009) $
 * $Revision: 2641 $
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

#include "../stdafx.h"
#include "rice.h"

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

static INLINE unsigned int _Rice_NumBits( unsigned int x )
{
    return s_bsr_table_16[x];
}


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
* Rice_Uncompress() - Uncompress a block of data using a Rice decoder.
*  in      - Input (compressed) buffer.
*  out     - Output (uncompressed) buffer. This buffer must be large
*            enough to hold the uncompressed data.
*  insize  - Number of input bytes.
*  outsize - Number of output bytes.
*************************************************************************/
template <typename T>
void rice_Uncompress( void *in, T *out, unsigned int insize,
                      unsigned int outsize )
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
    unsigned int     i, x, k, wordsize, outcount, ksum, n;
    unsigned int     hist[ RICE_HISTORY ];

    for (i = 0;  i < RICE_HISTORY;  ++i)
    {
        hist[i] = 0;
    }

    wordsize = sizeof(T) * 8;
    outcount = outsize / (wordsize>>3);

    /* Do we have anything to decompress? */
    if( outcount == 0 )
    {
        return;
    }

    /* Initialize input bitsream */
    _Rice_InitBitstream( &stream, in, insize );

    /* Get initial k */
    k = _Rice_ReadBits( &stream, 8 );
    ksum = 0;

    /* Was the buffer not compressed */
    if( k == 0 )
    {
        /* Copy raw words from input stream */
        for( i = 0; i < outcount; ++ i )
        {
            x = _Rice_ReadBits( &stream, wordsize );
            _Rice_WriteWord( out, i, x );
        }
    }
    else
    {
        /* Decode input stream */
        for( i = 0; i < outcount; ++ i )
        {
            /* Decode word from input buffer */
            x = _Rice_DecodeWord( k, &stream );

            /* Write word to output buffer */
            _Rice_WriteWord( out, i, x );

            /* Update history */
            n = _Rice_NumBits( x );
            ksum += n;
            ksum -= hist[ i % RICE_HISTORY ];
            hist[ i % RICE_HISTORY ] = n;

            /* Revise optimum k? */
            if( i >= RICE_HISTORY )
            {
                k = (ksum + (RICE_HISTORY>>1)) / RICE_HISTORY;
            }
        }
    }
}


/*************************************************************************
* Instantiation of template functions (see rice.h)
*************************************************************************/
template void 
rice_Uncompress<signed char>( void *in, signed char* out, unsigned int insize,
                              unsigned int outsize );
template void 
rice_Uncompress<unsigned char>( void *in, unsigned char* out, unsigned int insize,
                                unsigned int outsize );
template void 
rice_Uncompress<signed short>( void *in, signed short* out, unsigned int insize,
                               unsigned int outsize );
template void 
rice_Uncompress<unsigned short>( void *in, unsigned short* out, unsigned int insize,
                                 unsigned int outsize );
