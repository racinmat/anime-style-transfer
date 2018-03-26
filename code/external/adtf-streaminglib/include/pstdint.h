/*  A portable stdint.h
 * 
 *  This file has been modified for use within ADTF
 * 
 ****************************************************************************
 *  BSD License:
 ****************************************************************************
 *
 *  Copyright (c) 2005-2007 Paul Hsieh
 *  All rights reserved.
 *  
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************
 *
 *  Version 0.1.11
 *
 *  The ANSI C standard committee, for the C99 standard, specified the
 *  inclusion of a new standard include file called stdint.h.  This is
 *  a very useful and long desired include file which contains several
 *  very precise definitions for integer scalar types that is
 *  critically important for making portable several classes of
 *  applications including cryptography, hashing, variable length
 *  integer libraries and so on.  But for most developers its likely
 *  useful just for programming sanity.
 *
 *  The problem is that most compiler vendors have decided not to
 *  implement the C99 standard, and the next C++ language standard
 *  (which has a lot more mindshare these days) will be a long time in
 *  coming and its unknown whether or not it will include stdint.h or
 *  how much adoption it will have.  Either way, it will be a long time
 *  before all compilers come with a stdint.h and it also does nothing
 *  for the extremely large number of compilers available today which
 *  do not include this file, or anything comparable to it.
 *
 *  So that's what this file is all about.  Its an attempt to build a
 *  single universal include file that works on as many platforms as
 *  possible to deliver what stdint.h is supposed to.  A few things
 *  that should be noted about this file:
 *
 *    1) It is not guaranteed to be portable and/or present an identical
 *       interface on all platforms.  The extreme variability of the
 *       ANSI C standard makes this an impossibility right from the
 *       very get go. Its really only meant to be useful for the vast
 *       majority of platforms that possess the capability of
 *       implementing usefully and precisely defined, standard sized
 *       integer scalars.  Systems which are not intrinsically 2s
 *       complement may produce invalid constants.
 *
 *    2) There is an unavoidable use of non-reserved symbols.
 *
 *    3) Other standard include files are invoked.
 *
 *    4) This file may come in conflict with future platforms that do
 *       include stdint.h.  The hope is that one or the other can be
 *       used with no real difference.
 *
 *    5) In the current verison, if your platform can't represent
 *       tInt32, tInt16 and tInt8, it just dumps out with a compiler
 *       error.
 *
 *    6) 64 bit integers may or may not be defined.  Test for their
 *       presence with the test: #ifdef INT64_MAX or #ifdef UINT64_MAX.
 *       Note that this is different from the C99 specification which
 *       requires the existence of 64 bit support in the compiler.  If
 *       this is not defined for your platform, yet it is capable of
 *       dealing with 64 bits then it is because this file has not yet
 *       been extended to cover all of your system's capabilities.
 *
 *    7) (u)intptr_t may or may not be defined.  Test for its presence
 *       with the test: #ifdef PTRDIFF_MAX.  If this is not defined
 *       for your platform, then it is because this file has not yet
 *       been extended to cover all of your system's capabilities, not
 *       because its optional.
 *
 *    8) The following might not been defined even if your platform is
 *       capable of defining it:
 *
 *       WCHAR_MIN
 *       WCHAR_MAX
 *       (u)tInt64
 *       PTRDIFF_MIN
 *       PTRDIFF_MAX
 *       (u)intptr_t
 *
 *    9) The following have not been defined:
 *
 *       WINT_MIN
 *       WINT_MAX
 *
 *   10) The criteria for defining (u)int_least(*)_t isn't clear,
 *       except for systems which don't have a type that precisely
 *       defined 8, 16, or 32 bit types (which this include file does
 *       not support anyways). Default definitions have been given.
 *
 *   11) The criteria for defining (u)int_fast(*)_t isn't something I
 *       would trust to any particular compiler vendor or the ANSI C
 *       committee.  It is well known that "compatible systems" are
 *       commonly created that have very different performance
 *       characteristics from the systems they are compatible with,
 *       especially those whose vendors make both the compiler and the
 *       system.  Default definitions have been given, but its strongly
 *       recommended that users never use these definitions for any
 *       reason (they do *NOT* deliver any serious guarantee of
 *       improved performance -- not in this file, nor any vendor's
 *       stdint.h).
 *
 *   12) The following macros:
 *
 *       PRINTF_INTMAX_MODIFIER
 *       PRINTF_INT64_MODIFIER
 *       PRINTF_INT32_MODIFIER
 *       PRINTF_INT16_MODIFIER
 *       PRINTF_LEAST64_MODIFIER
 *       PRINTF_LEAST32_MODIFIER
 *       PRINTF_LEAST16_MODIFIER
 *       PRINTF_INTPTR_MODIFIER
 *
 *       are strings which have been defined as the modifiers required
 *       for the "d", "u" and "x" printf formats to correctly output
 *       (u)intmax_t, (u)tInt64, (u)tInt32, (u)tInt16, (u)least64_t,
 *       (u)least32_t, (u)least16_t and (u)intptr_t types respectively.
 *       PRINTF_INTPTR_MODIFIER is not defined for some systems which
 *       provide their own stdint.h.  PRINTF_INT64_MODIFIER is not
 *       defined if INT64_MAX is not defined.  These are an extension
 *       beyond what C99 specifies must be in stdint.h.
 *
 *       In addition, the following macros are defined:
 *
 *       PRINTF_INTMAX_HEX_WIDTH
 *       PRINTF_INT64_HEX_WIDTH
 *       PRINTF_INT32_HEX_WIDTH
 *       PRINTF_INT16_HEX_WIDTH
 *       PRINTF_INT8_HEX_WIDTH
 *       PRINTF_INTMAX_DEC_WIDTH
 *       PRINTF_INT64_DEC_WIDTH
 *       PRINTF_INT32_DEC_WIDTH
 *       PRINTF_INT16_DEC_WIDTH
 *       PRINTF_INT8_DEC_WIDTH
 *
 *       Which specifies the maximum number of characters required to
 *       print the number of that type in either hexadecimal or decimal.
 *       These are an extension beyond what C99 specifies must be in
 *       stdint.h.
 *
 *  Compilers tested (all with 0 warnings at their highest respective
 *  settings): Borland Turbo C 2.0, WATCOM C/C++ 11.0 (16 bits and 32
 *  bits), Microsoft Visual C++ 6.0 (32 bit), Microsoft Visual Studio
 *  .net (VC7), Intel C++ 4.0, GNU gcc v3.3.3
 *
 *  This file should be considered a work in progress.  Suggestions for
 *  improvements, especially those which increase coverage are strongly
 *  encouraged.
 *
 *  Acknowledgements
 *
 *  The following people have made significant contributions to the
 *  development and testing of this file:
 *
 *  Chris Howie
 *  John Steele Scott
 *  Dave Thorup
 *
 */

#include <stddef.h>
#include <limits.h>
#include <signal.h>

/*
 *  For gcc with _STDINT_H, fill in the PRINTF_INT*_MODIFIER macros, and
 *  do nothing else.  On the Mac OS X version of gcc this is _STDINT_H_.
 */

#if ((defined(__STDC__) && __STDC__ && __STDC_VERSION__ >= 199901L) \
     || (defined (__WATCOMC__) && (!defined(_STDINT_H_INCLUDED) || __WATCOMC__ >= 1250)) \
     || (defined(__GNUC__) \
          && ((!defined(_STDINT_H) || !defined(_STDINT_H_)) \
            || !defined(_PSTDINT_H_INCLUDED))) )

    #include <stdint.h>
    #define _PSTDINT_H_INCLUDED

    /*
    *  Something really weird is going on with Open Watcom.  Just pull some of
    *  these duplicated definitions from Open Watcom's stdint.h file for now.
    */
    # if defined (__WATCOMC__) && __WATCOMC__ >= 1250
        #  if !defined (INT64_C)
        #   define INT64_C(x)   (x + (INT64_MAX - INT64_MAX))
        #  endif
        #  if !defined (UINT64_C)
        #   define UINT64_C(x)  (x + (UINT64_MAX - UINT64_MAX))
        #  endif
        #  if !defined (INT32_C)
        #   define INT32_C(x)   (x + (INT32_MAX - INT32_MAX))
        #  endif
        #  if !defined (UINT32_C)
        #   define UINT32_C(x)  (x + (UINT32_MAX - UINT32_MAX))
        #  endif
        #  if !defined (INT16_C)
        #   define INT16_C(x)   (x)
        #  endif
        #  if !defined (UINT16_C)
        #   define UINT16_C(x)  (x)
        #  endif
        #  if !defined (INT8_C)
        #   define INT8_C(x)   (x)
        #  endif
        #  if !defined (UINT8_C)
        #   define UINT8_C(x)  (x)
        #  endif


        #  if !defined (UINT64_MAX)
        #   define UINT64_MAX  18446744073709551615ULL
        #  endif
        #  if !defined (INT64_MAX)
        #   define INT64_MAX  9223372036854775807LL
        #  endif
        #  if !defined (UINT32_MAX)
        #   define UINT32_MAX  4294967295UL
        #  endif
        #  if !defined (INT32_MAX)
        #   define INT32_MAX  2147483647L
        #  endif
        #  if !defined (INTMAX_MAX)
        #   define INTMAX_MAX INT64_MAX
        #  endif
        #  if !defined (INTMAX_MIN)
        #   define INTMAX_MIN INT64_MIN
        #  endif
    # endif

    typedef int8_t                  tInt8;                  /* .i8...   */
    typedef uint8_t                 tUInt8;                 /* .ui8...  */
    typedef int16_t                 tInt16;                 /* .i16...  */
    typedef uint16_t                tUInt16;                /* .ui16..  */
    typedef int32_t                 tInt32;                 /* .i32...  */
    typedef uint32_t                tUInt32;                /* .ui32..  */
    typedef int64_t                 tInt64;                 /* .i64...  */
    typedef uint64_t                tUInt64;                /* .ui64..  */


#endif //_PSTDINT_H_INCLUDED


#ifndef _PSTDINT_H_INCLUDED
#define _PSTDINT_H_INCLUDED


#ifndef SIZE_MAX
# define SIZE_MAX (~(size_t)0)
#endif

/*
 *  Deduce the type assignments from limits.h under the assumption that
 *  integer sizes in bits are powers of 2, and follow the ANSI
 *  definitions.
 */

#ifndef UINT8_MAX
# define UINT8_MAX 0xff
#endif
#ifndef tUInt8
# if (UCHAR_MAX == UINT8_MAX) || defined (S_SPLINT_S)
    typedef unsigned char tUInt8;
#   define UINT8_C(v) ((tUInt8) v)
# else
#   error "Platform not supported"
# endif
#endif

#ifndef INT8_MAX
# define INT8_MAX 0x7f
#endif
#ifndef INT8_MIN
# define INT8_MIN INT8_C(0x80)
#endif
#ifndef tInt8
# if (SCHAR_MAX == INT8_MAX) || defined (S_SPLINT_S)
    typedef signed char tInt8;
#   define INT8_C(v) ((tInt8) v)
# else
#   error "Platform not supported"
# endif
#endif

#ifndef UINT16_MAX
# define UINT16_MAX 0xffff
#endif
#ifndef tUInt16
#if (UINT_MAX == UINT16_MAX) || defined (S_SPLINT_S)
  typedef unsigned int tUInt16;
# ifndef PRINTF_INT16_MODIFIER
#  define PRINTF_INT16_MODIFIER ""
# endif
# define UINT16_C(v) ((tUInt16) (v))
#elif (USHRT_MAX == UINT16_MAX)
  typedef unsigned short tUInt16;
# define UINT16_C(v) ((tUInt16) (v))
# ifndef PRINTF_INT16_MODIFIER
#  define PRINTF_INT16_MODIFIER "h"
# endif
#else
#error "Platform not supported"
#endif
#endif

#ifndef INT16_MAX
# define INT16_MAX 0x7fff
#endif
#ifndef INT16_MIN
# define INT16_MIN INT16_C(0x8000)
#endif
#ifndef tInt16
#if (INT_MAX == INT16_MAX) || defined (S_SPLINT_S)
  typedef signed int tInt16;
# define INT16_C(v) ((tInt16) (v))
# ifndef PRINTF_INT16_MODIFIER
#  define PRINTF_INT16_MODIFIER ""
# endif
#elif (SHRT_MAX == INT16_MAX)
  typedef signed short tInt16;
# define INT16_C(v) ((tInt16) (v))
# ifndef PRINTF_INT16_MODIFIER
#  define PRINTF_INT16_MODIFIER "h"
# endif
#else
#error "Platform not supported"
#endif
#endif

#ifndef UINT32_MAX
# define UINT32_MAX (0xffffffffUL)
#endif
#ifndef tUInt32
#if (ULONG_MAX == UINT32_MAX) || defined (S_SPLINT_S)
  typedef unsigned long tUInt32;
# define UINT32_C(v) v ## UL
# ifndef PRINTF_INT32_MODIFIER
#  define PRINTF_INT32_MODIFIER "l"
# endif
#elif (UINT_MAX == UINT32_MAX)
  typedef unsigned int tUInt32;
# ifndef PRINTF_INT32_MODIFIER
#  define PRINTF_INT32_MODIFIER ""
# endif
# define UINT32_C(v) v ## U
#elif (USHRT_MAX == UINT32_MAX)
  typedef unsigned short tUInt32;
# define UINT32_C(v) ((unsigned short) (v))
# ifndef PRINTF_INT32_MODIFIER
#  define PRINTF_INT32_MODIFIER ""
# endif
#else
#error "Platform not supported"
#endif
#endif

#ifndef INT32_MAX
# define INT32_MAX (0x7fffffffL)
#endif
#ifndef INT32_MIN
# define INT32_MIN INT32_C(0x80000000)
#endif
#ifndef tInt32
#if (LONG_MAX == INT32_MAX) || defined (S_SPLINT_S)
  typedef signed long tInt32;
# define INT32_C(v) v ## L
# ifndef PRINTF_INT32_MODIFIER
#  define PRINTF_INT32_MODIFIER "l"
# endif
#elif (INT_MAX == INT32_MAX)
  typedef signed int tInt32;
# define INT32_C(v) v
# ifndef PRINTF_INT32_MODIFIER
#  define PRINTF_INT32_MODIFIER ""
# endif
#elif (SHRT_MAX == INT32_MAX)
  typedef signed short tInt32;
# define INT32_C(v) ((short) (v))
# ifndef PRINTF_INT32_MODIFIER
#  define PRINTF_INT32_MODIFIER ""
# endif
#else
#error "Platform not supported"
#endif
#endif

/*
 *  The macro stdint_int64_defined is temporarily used to record
 *  whether or not 64 integer support is available.  It must be
 *  defined for any 64 integer extensions for new platforms that are
 *  added.
 */

#if !defined (stdint_int64_defined)
#if (defined(__STDC__) && defined(__STDC_VERSION__)) || defined (S_SPLINT_S)
# if (__STDC__ && __STDC_VERSION >= 199901L) || defined (S_SPLINT_S)
#  define stdint_int64_defined
   typedef long long tInt64;
   typedef unsigned long long tUInt64;
#  define UINT64_C(v) v ## ULL
#  define  INT64_C(v) v ## LL
#  ifndef PRINTF_INT64_MODIFIER
#   define PRINTF_INT64_MODIFIER "ll"
#  endif
# endif
#endif
#endif

#if !defined (stdint_int64_defined)
# if defined(__GNUC__)
#  define stdint_int64_defined
   __extension__ typedef long long tInt64;
   __extension__ typedef unsigned long long tUInt64;
#  define UINT64_C(v) v ## ULL
#  define  INT64_C(v) v ## LL
#  ifndef PRINTF_INT64_MODIFIER
#   define PRINTF_INT64_MODIFIER "ll"
#  endif
# elif defined(__MWERKS__) || defined (__SUNPRO_C) || defined (__SUNPRO_CC) || defined (__APPLE_CC__) || defined (_LONG_LONG) || defined (_CRAYC) || defined (S_SPLINT_S)
#  define stdint_int64_defined
   typedef long long tInt64;
   typedef unsigned long long tUInt64;
#  define UINT64_C(v) v ## ULL
#  define  INT64_C(v) v ## LL
#  ifndef PRINTF_INT64_MODIFIER
#   define PRINTF_INT64_MODIFIER "ll"
#  endif
# elif (defined(__WATCOMC__) && defined(__WATCOM_INT64__)) || (defined(_MSC_VER) && _INTEGRAL_MAX_BITS >= 64) || (defined (__BORLANDC__) && __BORLANDC__ > 0x460) || defined (__alpha) || defined (__DECC)
#  define stdint_int64_defined
   typedef __int64 tInt64;
   typedef unsigned __int64 tUInt64;
#  define UINT64_C(v) v ## UI64
#  define  INT64_C(v) v ## I64
#  ifndef PRINTF_INT64_MODIFIER
#   define PRINTF_INT64_MODIFIER "I64"
#  endif
# endif
#endif

#if !defined (LONG_LONG_MAX) && defined (INT64_C)
# define LONG_LONG_MAX INT64_C (9223372036854775807)
#endif
#ifndef ULONG_LONG_MAX
# define ULONG_LONG_MAX UINT64_C (18446744073709551615)
#endif

#if !defined (INT64_MAX) && defined (INT64_C)
# define INT64_MAX INT64_C (9223372036854775807)
#endif
#if !defined (INT64_MIN) && defined (INT64_C)
# define INT64_MIN INT64_C (-9223372036854775808)
#endif
#if !defined (UINT64_MAX) && defined (INT64_C)
# define UINT64_MAX UINT64_C (18446744073709551615)
#endif

/*
 *  Width of hexadecimal for number field.
 */

#ifndef PRINTF_INT64_HEX_WIDTH
# define PRINTF_INT64_HEX_WIDTH "16"
#endif
#ifndef PRINTF_INT32_HEX_WIDTH
# define PRINTF_INT32_HEX_WIDTH "8"
#endif
#ifndef PRINTF_INT16_HEX_WIDTH
# define PRINTF_INT16_HEX_WIDTH "4"
#endif
#ifndef PRINTF_INT8_HEX_WIDTH
# define PRINTF_INT8_HEX_WIDTH "2"
#endif

#ifndef PRINTF_INT64_DEC_WIDTH
# define PRINTF_INT64_DEC_WIDTH "20"
#endif
#ifndef PRINTF_INT32_DEC_WIDTH
# define PRINTF_INT32_DEC_WIDTH "10"
#endif
#ifndef PRINTF_INT16_DEC_WIDTH
# define PRINTF_INT16_DEC_WIDTH "5"
#endif
#ifndef PRINTF_INT8_DEC_WIDTH
# define PRINTF_INT8_DEC_WIDTH "3"
#endif


#endif
