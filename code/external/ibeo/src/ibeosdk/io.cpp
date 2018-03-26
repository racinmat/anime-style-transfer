//======================================================================
/*! \file io.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen
 * \date Mar 05, 2015
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

void writeLE(UINT8*& buf, BitNumber& firstBitToWrite, UINT8 val, const int nbOfDataBits)
{

	val = UINT8(UINT8(val<<(8-nbOfDataBits)) >> (8-nbOfDataBits));

	if (firstBitToWrite + nbOfDataBits > 8) {
		// the value will be written into two bytes.
		const int nbOfBitsInFirstByte = 8-firstBitToWrite;
		const int nbOfBitsInSecondByte = nbOfDataBits - nbOfBitsInFirstByte;
		const UINT8 firstByteBits = UINT8(UINT8(val << (8-nbOfBitsInFirstByte)) >> (8-nbOfBitsInFirstByte));
		*buf = UINT8(*buf | UINT8(firstByteBits << firstBitToWrite));
		++buf;
		const UINT8 secondByteBits = UINT8((UINT8(val >> nbOfBitsInFirstByte) << (8-nbOfBitsInSecondByte) & 0xFF) >> (8-nbOfBitsInSecondByte));
		*buf |= secondByteBits;
		firstBitToWrite = BitNumber(firstBitToWrite + nbOfDataBits - 8);
	}
	else if (firstBitToWrite + nbOfDataBits == 8) {
		// the value fits into the current byte but the byte is
		// completely filled after this write operation.
		*buf = UINT8(*buf | UINT8(val << firstBitToWrite));
		++buf;
		firstBitToWrite = Bit0;
	}
	else {
		// the value fits into the current byte and there are still
		// some bits free after this write operation.
		*buf = UINT8(*buf | UINT8(val << firstBitToWrite));
		firstBitToWrite = BitNumber(firstBitToWrite + nbOfDataBits);
	}
}

//======================================================================

void writeLE(UINT8*& buf, BitNumber& firstBitToWrite, const UINT16 val, const int nbOfDataBits)
{
	writeLE(buf, firstBitToWrite, UINT8(val), 8);
	writeLE(buf, firstBitToWrite, UINT8(val>>8), nbOfDataBits-8);
}

//======================================================================

UINT8 readLE8(const UINT8*& buf, BitNumber& firstBitToRead, const int nbOfDataBits)
{
	UINT8 val = 0;
	if (firstBitToRead + nbOfDataBits > 8) {
		// The bits to be read are distributed to two bytes.
		const int nbOfBitsInFirstByte = 8-firstBitToRead;
		const int nbOfBitsInSecondByte = nbOfDataBits - nbOfBitsInFirstByte;
		val = UINT8(((*buf) >> firstBitToRead | (((*(buf+1)) << (8 - nbOfBitsInSecondByte)) & 0xFF) >> (8-nbOfBitsInFirstByte-nbOfBitsInSecondByte)));
		++buf;
		firstBitToRead = BitNumber(firstBitToRead + nbOfDataBits - 8);
	}
	else if (firstBitToRead + nbOfDataBits == 8) {
		// The bits to be read are inside the current byte only but
		// afterwards all of the bits of this byte has been read.
		val = UINT8((*buf) >> firstBitToRead);
		++buf;
		firstBitToRead = Bit0;
	}
	else {
		// The bits to be read are inside the current byte only and
		// not all of the bits has been read.
		val = UINT8((((*buf) << (8-firstBitToRead-nbOfDataBits)) & 0xFF) >> (8-nbOfDataBits));
		firstBitToRead = BitNumber(firstBitToRead + nbOfDataBits);
	}
	return val;
}

//======================================================================

UINT16 readLE16(const UINT8*& buf, BitNumber& firstBitToRead, const int nbOfDataBits)
{
	UINT16 val = readLE8(buf, firstBitToRead, 8);

	const UINT8 valUpperBits = readLE8(buf, firstBitToRead, nbOfDataBits-8);

	val |= uint16_t(int32_t(valUpperBits) << 8);
	return val;
}

//======================================================================

} // namespace ibeosdk

//======================================================================

