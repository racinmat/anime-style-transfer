//======================================================================
/*! \file io.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 4, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_IO_HPP_SEEN
#define IBEOSDK_IO_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/io_prototypes.hpp>
#include <ibeosdk/inttypes.hpp>

#include <boost/detail/endian.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_fundamental.hpp>

#include <iostream>
#include <istream>
#include <string>
#include <iomanip>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\brief Read a value of type \a T from stream \a is.
 *
 * It is assumed that the stream provides the data in the same byte order
 * as the system the code is running on. So the byte order will left
 * untouched.
 *
 * \tparam T              Type of the value to be read.
 * \param[in, out] is     Stream providing the
 *                        data to be read.
 * \param[out]     value  On exit it will hold
 *                        the value that has
 *                        been read.
 *///------------------------------------------------------------------
template<typename T>
void readLinear(std::istream& is, T& value)
{
	is.read(reinterpret_cast<char*>(&value), sizeof(value));
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
 * \tparam T              Type of the value to be read.
 * \param[in, out] is     Stream providing the
 *                        data to be read.
 * \param[out]     value  On exit it will hold
 *                        the value that has
 *                        been read.
 *///------------------------------------------------------------------
template<typename T>
void readSwapped(std::istream& is, T& value)
{
	const std::streamsize maxSize = sizeof(value);
	unsigned short n = maxSize;
	char* data = reinterpret_cast<char *>(&value);
	char swapBuffer[maxSize];
	is.read(swapBuffer, n);

	char* swap = swapBuffer + n - 1;
	while (n--)
		*data++ = *swap--;
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
 * \tparam T              Type of the value to be read.
 * \param[in, out] is     Stream providing the
 *                        data to be read.
 * \param[out]     value  On exit it will hold
 *                        the value that has
 *                        been read.
 *///------------------------------------------------------------------
template<typename T>
void readLE(std::istream& is, T& value)
{
#ifdef BOOST_BIG_ENDIAN
	readSwapped(is, value);
#else
	readLinear(is, value);
#endif
}

//======================================================================

template<>
inline
void readLE<bool>(std::istream& is, bool& value)
{
	uint8_t tmp;
	readLinear(is, tmp); // endianess does not matter for 1 Byte read.
	value = (tmp != 0);
}


//======================================================================
/*!\brief Read a value of type \a T from stream
 *        \a is given in big endian byte order.
 *
 * \tparam T              Type of the value to be read.
 * \param[in, out] is     Stream providing the
 *                        data to be read.
 * \param[out]     value  On exit it will hold
 *                        the value that has
 *                        been read.
 *///------------------------------------------------------------------
template<typename T>
void readBE(std::istream& is, T& value)
{
#ifdef BOOST_BIG_ENDIAN
	readLinear(is, value);
#else
	readSwapped(is, value);
#endif
}

//======================================================================

template<>
inline
void readBE<bool>(std::istream& is, bool& value)
{
	uint8_t tmp;
	readLinear(is, tmp); // endianess does not matter for 1 Byte read.
	value = (tmp != 0);
}


//======================================================================
/*!\brief Read a value of type \a T from stream \a is.
 *
 * \tparam T              Type of the value to be read.
 * \param[in, out] is     Stream providing the
 *                        data to be read.
 * \param[out]     value  On exit it will hold
 *                        the value that has
 *                        been read.
 * \param[in]      sourceIsBigEndian
 *                        tells whether the
 *                        stream \a is has big
 *                        endian byte order.
 *///------------------------------------------------------------------
template<typename T>
void read(std::istream& is, T& value, bool sourceIsBigEndian = false)
{
#ifdef BOOST_BIG_ENDIAN
	const bool do_swap = !sourceIsBigEndian;
#else
	const bool do_swap = sourceIsBigEndian;
#endif

	if (do_swap)
		readSwapped(is, value);
	else
		readLinear(is, value);
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
 * \tparam T  Type of the value to be written.
 * \param[in, out] os     Stream receiving the data been written.
 * \param[in]      value  The value to be written.
 *///------------------------------------------------------------------
template<typename T>
void writeLinear(std::ostream& os, const T& value)
{
	os.write(reinterpret_cast<const char* const>(&value), sizeof(value));
}



//======================================================================
/*!\brief Write a value of type \a T into stream \a os.
 *
 * It is assumed that the stream receiving the data in different byte order
 * as the system the code is running on. So the byte order  has to be swapped.
 *
 * \tparam T              Type of the value to be written.
 * \param[in, out] os     Stream receiving the data been written.
 * \param[in]      value  The value to be written.
 *///------------------------------------------------------------------
template<typename T>
void writeSwapped(std::ostream& os, const T& value)
{
	const std::streamsize typeSize = sizeof(value);
	const short n = typeSize;
	const unsigned char* data = reinterpret_cast<const unsigned char*>(&value) + n-1;

	unsigned char swapBuffer[typeSize];
	unsigned char* swap = swapBuffer;

	while (swap < swapBuffer+n)
		*swap++ = *data--;


	os.write(reinterpret_cast<const char*>(swapBuffer), typeSize);
}



//======================================================================
/*!\brief Write a value of type \a T into stream
 *        \a os given in little endian byte order.
 *
 * \tparam T              Type of the value to be written.
 * \param[in, out] os     Stream that will receive the
 *                        data to be written.
 * \param[in]      value  The value to be written.
 *///------------------------------------------------------------------
template<typename T>
void writeLE(std::ostream& os, const T& value)
{
#ifdef BOOST_BIG_ENDIAN
	writeSwapped(os, value);
#else
	writeLinear(os, value);
#endif
}

//======================================================================

template<>
inline
void writeLE<bool>(std::ostream& os, const bool& value)
{
	const uint8_t tmp = value ? 1: 0;
	writeLinear(os, tmp);  // endianess does not matter for 1 Byte read.
}


//======================================================================
/*!\brief Write a value of type \a T into stream
 *        \a os given in big endian byte order.
 *
 * \tparam T              Type of the value to be written.
 * \param[in, out] os     Stream that will receive the
 *                        data to be written.
 * \param[in]      value  The value to be written.
 *///------------------------------------------------------------------
template<typename T>
void writeBE(std::ostream& os, const T& value)
{
#ifdef BOOST_BIG_ENDIAN
	writeLinear(os, value);
#else
	writeSwapped(os, value);
#endif
}

//======================================================================

template<>
inline
void writeBE(std::ostream& os, const bool& value)
{
	const uint8_t tmp = value ? 1: 0;
	writeLinear(os, tmp); // endianess does not matter for 1 Byte read.
}

//======================================================================
/*!\brief Write a value of type \a T into stream \a os.
 *
 * \tparam T              Type of the value to be read.
 * \param[in, out] os     Stream the data will be written to.
 * \param[in]      value  The data to be written.
 * \param[in]      destIsBigEndian
 *                        tells whether the stream \a os shall have big
 *                        endian byte order.
 *///------------------------------------------------------------------
template<typename T>
void write(std::ostream& os, const T& value, bool destIsBigEndian = false)
{
#ifdef BOOST_BIG_ENDIAN
	const bool do_swap = !destIsBigEndian;
#else
	const bool do_swap = destIsBigEndian;
#endif

	if (do_swap)
		writeSwapped(os, value);
	else
		writeLinear(os, value);
}

//======================================================================





//======================================================================
//======================================================================
//======================================================================




//======================================================================

enum BitNumber {
	Bit0 = 0,
	Bit1 = 1,
	Bit2 = 2,
	Bit3 = 3,
	Bit4 = 4,
	Bit5 = 5,
	Bit6 = 6,
	Bit7 = 7
};

//======================================================================
/*!\brief Write a value \a val with \a nbOfDataBits (between 1 and 8) bits to the given
 *        buffer \a buf starting at bit \a firstBitToWrite.
 *
 * If not all given bits fit into the byte pointed to by \a buf,
 * the rest of the bits will be written into the next byte. Starting
 * with the least significant bit.
 *
 * \param[in, out] buf              Pointer to the position of the byte where
 *                                  to write the first bit of \a val into.
 *                                  On exit this variable will point to the
 *                                  position inside of the buffer where to write
 *                                  from the next bit after this write operation.
 * \param[in, out] firstBitToWrite  Id of the first bit to be (over)written in
 *                                  the buffer.
 *                                  On exit this variable will hold the id of the
 *                                  next bit to be written to after this write
 *                                  operation.
 * \param[in]      val              Variable that contains the bit to be written
 *                                  into the buffer \a buf.
 * \param[in]      nbOfDataBits     The number of bits to be written into the buffer \a buf.
 *                                  These bits are provided in the parameter \a val. Starting
 *                                  with the least significant bit. \a nbOfDataBits must between 1 and 8.
 *///-------------------------------------------------------------------
void writeLE(UINT8*& buf, BitNumber& firstBitToWrite, UINT8 val, const int nbOfDataBits);

//======================================================================

//======================================================================
/*!\brief Write a value \a val with \a nbOfDataBits (between 9 and 16) bits to the given
 *        buffer \a buf starting at bit \a firstBitToWrite.
 *
 * If not all given bits fit into the byte pointed to by \a buf,
 * the rest of the bits will be written into the next byte. Starting
 * with the least significant bit.
 *
 * \param[in, out] buf              Pointer to the position of the byte where
 *                                  to write the first bit of \a val into.
 *                                  On exit this variable will point to the
 *                                  position inside of the buffer where to write
 *                                  from the next bit after this write operation.
 * \param[in, out] firstBitToWrite  Id of the first bit to be (over)written in
 *                                  the buffer.
 *                                  On exit this variable will hold the id of the
 *                                  next bit to be written to after this write
 *                                  operation.
 * \param[in]      val              Variable that contains the bit to be written
 *                                  into the buffer \a buf.
 * \param[in]      nbOfDataBits     The number of bits to be written into the buffer \a buf.
 *                                  These bits are provided in the parameter \a val. Starting
 *                                  with the least significant bit. \a nbOfDataBits must between 9 and 16.
 *///-------------------------------------------------------------------
void writeLE(UINT8*& buf, BitNumber& firstBitToWrite, const UINT16 val, const int nbOfDataBits);

//======================================================================
/*!\brief Read a value \a val with \a nbOfDataBits (between 1 and 8) bits from the given
 *        buffer \a buf starting at bit \a firstBitToRead inside the buffer.
 *
 * If not all bits to be read available inside the byte pointed to by \a buf,
 * the rest of the bits will be read from the next byte. Starting
 * with the least significant bit.
 *
 * \param[in, out] buf              Pointer to the position of the byte where
 *                                  to read the first bit from.
 *                                  On exit this variable will point to the
 *                                  position inside of the buffer where to read
 *                                  the next bit from after this read operation.
 * \param[in, out] firstBitToRead   Id of the first bit to be read from
 *                                  the buffer.
 *                                  On exit this variable will hold the id of the
 *                                  next bit to be read from after this read
 *                                  operation.
 * \param[in]      nbOfDataBits     The number of bits to be read from the buffer \a buf.
 *                                  These bits are returned. \a nbOfDataBits must between 1 and 8.
 *
 * \return A byte that contains the read bits. Starting with the least significant bit.
 *///-------------------------------------------------------------------
UINT8 readLE8(const UINT8*& buf, BitNumber& firstBitToRead, const int nbOfDataBits);

//======================================================================
/*!\brief Read a value \a val with \a nbOfDataBits (between 9 and 16) bits from the given
 *        buffer \a buf starting at bit \a firstBitToRead inside the buffer.
 *
 * If not all bits to be read available inside the byte pointed to by \a buf,
 * the rest of the bits will be read from the next byte. Starting
 * with the least significant bit.
 *
 * \param[in, out] buf              Pointer to the position of the byte where
 *                                  to read the first bit from.
 *                                  On exit this variable will point to the
 *                                  position inside of the buffer where to read
 *                                  the next bit from after this read operation.
 * \param[in, out] firstBitToRead   Id of the first bit to be read from
 *                                  the buffer.
 *                                  On exit this variable will hold the id of the
 *                                  next bit to be read from after this read
 *                                  operation.
 * \param[in]      nbOfDataBits     The number of bits to be read from the buffer \a buf.
 *                                  These bits are returned. \a nbOfDataBits must between 9 and 16.
 *
 * \return A byte that contains the read bits. Starting with the least significant bit.
 *///-------------------------------------------------------------------
UINT16 readLE16(const UINT8*& buf, BitNumber& firstBitToRead, const int nbOfDataBits);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IO_HPP_SEEN

//======================================================================

