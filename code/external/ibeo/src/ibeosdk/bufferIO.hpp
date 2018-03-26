//======================================================================
/*! \file bufferIO.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Feb 20, 2014
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_BUFFERIO_HPP_SEEN
#define IBEOSDK_BUFFERIO_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/inttypes.hpp>

#include <boost/detail/endian.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_fundamental.hpp>

//#include <iostream>
//#include <istream>
//#include <string>
//#include <iomanip>
#include <string.h>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\brief Read a value of type \a T from stream \a is.
 *
 * It is assumed that the stream provides the data in the same byte order
 * as the system the code is running on. So the byte order will left
 * untouched.
 *
 * \tparam T               Type of the value to be read.
 * \param[in, out] srcBuf  Buffer providing the data to be read.
 *                         On output \a srcBuf has been increased
 *                         by the number of bytes that has been read.
 * \param[out]     value   On exit it will hold
 *                         the value that has
 *                         been read.
 *///------------------------------------------------------------------
template<typename T>
void readLinear(const char*& srcBuf, T& value)
{
	memcpy(reinterpret_cast<char*>(&value), srcBuf, sizeof(value));
	srcBuf+=sizeof(value);
}



//======================================================================
#ifndef _WIN32
#  pragma GCC diagnostic ignored "-Warray-bounds"
#endif // _WIN32
//======================================================================
/*!\brief Read a value of type \a T for stream \a is and flip the byte order.
 *
 * It is assumed that the stream provides the data in different byte order
 * as the system the code is running on. So the byte order has to be swapped.
 *
 * \tparam T               Type of the value to be read.
 * \param[in, out] srcBuf  Buffer providing the data to be read.
 *                         On output \a srcBuf has been increased
 *                         by the number of bytes that has been read.
 * \param[out]     value   On exit it will hold
 *                         the value that has
 *                         been read.
 *///------------------------------------------------------------------
template<typename T>
void readSwapped(const char*& srcBuf, T& value)
{
	const int n = sizeof(value);

	char* const targetBuf = reinterpret_cast<char*>(&value);
	char* targetBufPos = targetBuf + n -1;

	while (targetBufPos >= targetBuf)
		*targetBufPos-- = *srcBuf++;
}



//======================================================================
#ifndef _WIN32
#  pragma GCC diagnostic warning "-Warray-bounds"
#endif // _WIN32
//======================================================================

//======================================================================
/*!\brief Read a value of type \a T from stream
 *        \a is given in little endian byte order.
 *
 * \tparam T               Type of the value to be read.
 * \param[in, out] srcBuf  Buffer providing the data to be read.
 *                         On output \a srcBuf has been increased
 *                         by the number of bytes that has been read.
 * \param[out]     value   On exit it will hold
 *                         the value that has
 *                         been read.
 *///------------------------------------------------------------------
template<typename T>
void readLE(const char*& srcBuf, T& value)
{
#ifdef BOOST_BIG_ENDIAN
	readSwapped(srcBuf, value);
#else
	readLinear(srcBuf, value);
#endif
}

//======================================================================

template<>
inline
void readLE<bool>(const char*& srcBuf, bool& value)
{
	uint8_t tmp;
	readLinear(srcBuf, tmp); // endianess does not matter for 1 Byte read.
	value = (tmp != 0);
}


//======================================================================
/*!\brief Read a value of type \a T from stream
 *        \a is given in big endian byte order.
 *
 * \tparam T               Type of the value to be read.
 * \param[in, out] srcBuf  Buffer providing the data to be read.
 *                         On output \a srcBuf has been increased
 *                         by the number of bytes that has been read.
 * \param[out]     value   On exit it will hold
 *                         the value that has
 *                         been read.
 *///------------------------------------------------------------------
template<typename T>
void readBE(const char*& srcBuf, T& value)
{
#ifdef BOOST_BIG_ENDIAN
	readLinear(srcBuf, value);
#else
	readSwapped(srcBuf, value);
#endif
}

//======================================================================

template<>
inline
void readBE<bool>(const char*& srcBuf, bool& value)
{
	uint8_t tmp;
	readLinear(srcBuf, tmp); // endianess does not matter for 1 Byte read.
	value = (tmp != 0);
}


//======================================================================
/*!\brief Read a value of type \a T from stream \a is.
 *
 * \tparam T               Type of the value to be read.
 * \param[in, out] srcBuf  Buffer providing the data to be read.
 *                         On output \a srcBuf has been increased
 *                         by the number of bytes that has been read.
 * \param[out]     value   On exit it will hold
 *                         the value that has
 *                         been read.
 * \param[in]      sourceIsBigEndian
 *                         tells whether the
 *                         stream \a is has big
 *                         endian byte order.
 *///------------------------------------------------------------------
template<typename T>
void read(const char*& srcBuf, T& value, bool sourceIsBigEndian = false)
{
#ifdef BOOST_BIG_ENDIAN
	const bool do_swap = !sourceIsBigEndian;
#else
	const bool do_swap = sourceIsBigEndian;
#endif

	if (do_swap)
		readSwapped(srcBuf, value);
	else
		readLinear(srcBuf, value);
}



//======================================================================





//======================================================================
//======================================================================
//======================================================================





//======================================================================
/*!\brief Write a value of type \a T into stream \a os.
 *
 * It is assumed that the stream receiving the data in the same byte order
 * as the system the code is running on. So the byte order will left
 * untouched.
 *
 * \tparam T                  Type of the value to be written.
 * \param[in, out] targetBuf  Buffer receiving the data been written.
 *                            On output \a targetBuf has been increased
 *                            by the number of bytes that has been written.
 * \param[in]      value      The value to be written.
 *///------------------------------------------------------------------
template<typename T>
void writeLinear(char*& targetBuf, const T& value)
{
	memcpy(targetBuf, reinterpret_cast<const char* const>(&value), sizeof(value));
	targetBuf += sizeof(value);
}



//======================================================================
/*!\brief Write a value of type \a T into stream \a os.
 *
 * It is assumed that the stream receiving the data in different byte order
 * as the system the code is running on. So the byte order  has to be swapped.
 *
 * \tparam T                  Type of the value to be written.
 * \param[in, out] targetBuf  Buffer receiving the data been written.
 *                            On output \a targetBuf has been increased
 *                            by the number of bytes that has been written.
 * \param[in]      value      The value to be written.
 *///------------------------------------------------------------------
template<typename T>
void writeSwapped(char*& targetBuf, const T& value)
{
	const int n = sizeof(value);

	const char* const srcBuf = reinterpret_cast<const char*>(&value);
	const char* srcBufPos = srcBuf + n -1;

	while (srcBufPos >= srcBuf)
		*targetBuf++ = *srcBufPos--;
}



//======================================================================
/*!\brief Write a value of type \a T into stream
 *        \a os given in little endian byte order.
 *
 * \tparam T                  Type of the value to be written.
 * \param[in, out] targetBuf  Buffer receiving the data been written.
 *                            On output \a targetBuf has been increased
 *                            by the number of bytes that has been written.
 * \param[in]      value      The value to be written.
 *///------------------------------------------------------------------
template<typename T>
void writeLE(char*& targetBuf, const T& value)
{
#ifdef BOOST_BIG_ENDIAN
	writeSwapped(targetBuf, value);
#else
	writeLinear(targetBuf, value);
#endif
}

//======================================================================

template<>
inline
void writeLE<bool>(char*& targetBuf, const bool& value)
{
	const uint8_t tmp = value ? 1: 0;
	writeLinear(targetBuf, tmp);  // endianess does not matter for 1 Byte read.
}


//======================================================================
/*!\brief Write a value of type \a T into stream
 *        \a os given in big endian byte order.
 *
 * \tparam T                  Type of the value to be written.
 * \param[in, out] targetBuf  Buffer receiving the data been written.
 *                            On output \a targetBuf has been increased
 *                            by the number of bytes that has been written.
 * \param[in]      value      The value to be written.
 *///------------------------------------------------------------------
template<typename T>
void writeBE(char*& targetBuf, const T& value)
{
#ifdef BOOST_BIG_ENDIAN
	writeLinear(targetBuf, value);
#else
	writeSwapped(targetBuf, value);
#endif
}

//======================================================================

template<>
inline
void writeBE(char*& targetBuf, const bool& value)
{
	const uint8_t tmp = value ? 1: 0;
	writeLinear(targetBuf, tmp); // endianess does not matter for 1 Byte read.
}

//======================================================================
/*!\brief Write a value of type \a T into stream \a os.
 *
 * \tparam T                  Type of the value to be read.
 * \param[in, out] targetBuf  Buffer receiving the data been written.
 *                            On output \a targetBuf has been increased
 *                            by the number of bytes that has been written.
 * \param[in]      value      The value to be written.
 * \param[in]      destIsBigEndian
 *                            tells whether the stream \a os shall have big
 *                            endian byte order.
 *///------------------------------------------------------------------
template<typename T>
void write(char*& targetBuf, const T& value, bool destIsBigEndian = false)
{
#ifdef BOOST_BIG_ENDIAN
	const bool do_swap = !destIsBigEndian;
#else
	const bool do_swap = destIsBigEndian;
#endif

	if (do_swap)
		writeSwapped(targetBuf, value);
	else
		writeLinear(targetBuf, value);
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_BUFFERIO_HPP_SEEN

//======================================================================

