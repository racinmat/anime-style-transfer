//======================================================================
/*! \file IbeoDataHeader.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date April 4, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>
#include <ibeosdk/DataTypeId.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>
#include <ibeosdk/bufferIO.hpp>

#include <iostream>
using namespace std;

//======================================================================

namespace ibeosdk {

//======================================================================

// static
const UINT32 IbeoDataHeader::magicWord = 0xAFFEC0C2;

//======================================================================

IbeoDataHeader::IbeoDataHeader()
  : m_sizeOfPrevMessage(0),
    m_sizeOfThisMessage(0),
    m_deviceId(0),
    m_dataType(DataTypeId::DataType_Unknown),
    m_ntpTime(0)
{}

//======================================================================

IbeoDataHeader::IbeoDataHeader(const DataTypeId dataType,
                               const UINT32 sizeOfPrevMessage,
                               const UINT32 sizeOfThisMessage,
                               const UINT8 deviceId,
                               const NTPTime ntpTime)
  : m_sizeOfPrevMessage(sizeOfPrevMessage),
    m_sizeOfThisMessage(sizeOfThisMessage),
    m_deviceId(deviceId),
    m_dataType(dataType),
    m_ntpTime(ntpTime)
{}

//======================================================================

IbeoDataHeader::~IbeoDataHeader() {}

//======================================================================

bool IbeoDataHeader::deserialize(std::istream& is)
{
	if (!IbeoDataHeader::findMagicWord(is))
		return false;

	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_sizeOfPrevMessage);
	ibeosdk::readBE(is, m_sizeOfThisMessage);

	UINT8 dummy;
	ibeosdk::readBE(is, dummy);
	//is.ignore(); // reserved byte

	ibeosdk::readBE(is, m_deviceId);

	this->m_dataType.readBE(is);

	ibeosdk::readBE(is, m_ntpTime); // 64 Bit NTP time stamp

	return !is.fail()
		&& ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize()-4); // adjust because of magic word
}

//======================================================================

//bool IbeoDataHeader::deserialize(const char*& buffer)
//{
//	const char* startPos = buffer;
//
//	ibeosdk::readBE(buffer, m_sizeOfPrevMessage);
//	ibeosdk::readBE(buffer, m_sizeOfThisMessage);
//
//	UINT8 dummy;
//	ibeosdk::readBE(buffer, dummy);
//	//is.ignore(); // reserved byte
//
//	ibeosdk::readBE(buffer, m_deviceId);
//
//	this->m_dataType.readBE(buffer);
//
//	ibeosdk::readBE(buffer, m_ntpTime); // 64 Bit NTP time stamp
//
//	return ((buffer - startPos) == this->getSerializedSize()-4) // adjust because of magic word
//		&& this->m_dataType != DataTypeId::DataType_Unknown;
//}

//======================================================================

bool IbeoDataHeader::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, magicWord);
	ibeosdk::writeBE(os, this->m_sizeOfPrevMessage);
	ibeosdk::writeBE(os, this->m_sizeOfThisMessage);
	const char res1 = 0;
	ibeosdk::writeBE(os, res1);
	ibeosdk::writeBE(os, this->m_deviceId);
	this->m_dataType.writeBE(os);
//	ibeosdk::writeBE(os, UINT16(this->m_dataType));
	ibeosdk::writeBE(os, this->m_ntpTime);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

//bool IbeoDataHeader::serialize(char*& target) const
//{
//	const char* startPos = target;
//
//	ibeosdk::writeBE(target, magicWord);
//	ibeosdk::writeBE(target, this->m_sizeOfPrevMessage);
//	ibeosdk::writeBE(target, this->m_sizeOfThisMessage);
//	const char res1 = 0;
//	ibeosdk::writeBE(target, res1);
//	ibeosdk::writeBE(target, this->m_deviceId);
//	this->m_dataType.writeBE(target);
////	ibeosdk::writeBE(os, UINT16(this->m_dataType));
//	ibeosdk::writeBE(target, this->m_ntpTime);
//
//	return ((target - startPos) == this->getSerializedSize());
//}

//======================================================================

bool IbeoDataHeader::findMagicWord(std::istream& is)
{
	if (is.bad() || is.eof())
		return false;

	UINT32 nMagicWord = 0;
	int nbOfSkippedBytes = -4;
	char cByte;

	// Read bytes from input stream until the next magic word is found
	while (nMagicWord != 0xAFFEC0C2) {
		// Note that the right-hand side of the &&-operator is evaluated
		// only if the left-hand side is true, i.e. is.get() will be
		// called only if the magic word is incorrect.
		++nbOfSkippedBytes;
		is >> cByte;
		if (is.eof()) {
			return false;
		}
		//std::cerr << " " << toHex(cByte) << dec;

		// By definition, header data have network byte order (Big Endian),
		// i.e. the high byte of the magic word is received first. Therefore
		// we must shift the received byte from the LSB side into the magic
		// word m_uVersion.
		nMagicWord <<= 8;
		nMagicWord |= cByte & 0x000000FF;
	}
//	std::cerr << endl;

	if (nbOfSkippedBytes > 0)
		logWarning << nbOfSkippedBytes << " Bytes skipped" << std::endl;

	return true;
}

//======================================================================

bool IbeoDataHeader::moveToMagicWord(std::istream& is)
{
	if (IbeoDataHeader::findMagicWord(is)) {
		is.putback(char(0xc2));
		is.putback(char(0xc0));
		is.putback(char(0xfe));
		is.putback(char(0xaf));
		return true;
	}
	return false;
}

//======================================================================

bool operator==(const IbeoDataHeader& lhs, const IbeoDataHeader& rhs)
{
	if (lhs.getPreviousMessageSize() != rhs.getPreviousMessageSize())
		return false;

	if (lhs.getMessageSize() != rhs.getMessageSize())
		return false;

	if (lhs.getDeviceId() != rhs.getDeviceId())
		return false;

	if (lhs.getDataType() != rhs.getDataType())
		return false;

	if (lhs.getTimestamp() != rhs.getTimestamp())
		return false;

	return true;
}

//======================================================================

bool operator!=(const IbeoDataHeader& lhs, const IbeoDataHeader& rhs)
{
	if (lhs.getPreviousMessageSize() == rhs.getPreviousMessageSize())
		return false;

	if (lhs.getMessageSize() == rhs.getMessageSize())
		return false;

	if (lhs.getDeviceId() == rhs.getDeviceId())
		return false;

	if (lhs.getDataType() == rhs.getDataType())
		return false;

	if (lhs.getTimestamp() == rhs.getTimestamp())
		return false;

	return true;
}

//======================================================================

bool compareIbeoDataHeaderWithoutDate(const IbeoDataHeader& lhs, const IbeoDataHeader& rhs)
{
	if (lhs.getPreviousMessageSize() != rhs.getPreviousMessageSize())
		return false;

	if (lhs.getMessageSize() != rhs.getMessageSize())
		return false;

	if (lhs.getDeviceId() != rhs.getDeviceId())
		return false;

	if (lhs.getDataType() != rhs.getDataType())
		return false;

	return true;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
