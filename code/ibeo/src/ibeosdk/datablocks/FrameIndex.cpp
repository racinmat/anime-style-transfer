//======================================================================
/*! \file FrameIndex.cpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date May 11, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/FrameIndex.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<FrameIndex>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<FrameIndex>::dataBlockId = DataTypeId(DataTypeId::DataType_FrameIndex);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<FrameIndex>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<FrameIndex>::dataBlockId, ibeosdk::RegisteredDataBlock<FrameIndex>::create);

class IdcFile;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<FrameIndex>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(registerIdInitial);

//======================================================================
//======================================================================
//======================================================================

const UINT8 FrameIndex::majorVersion = 2;
const UINT8 FrameIndex::minorVersion = 1;
const std::string FrameIndex::nameString = "FRAMEINDEX_HEADER";
const UINT32 FrameIndex::lengthOfNameString = UINT32(FrameIndex::nameString.length());

//======================================================================

FrameIndex::FrameIndex()
  : RegisteredDataBlock<FrameIndex>(),
    m_framingPolicy(),
    m_frameIndices()
{}

//======================================================================

FrameIndex::~FrameIndex() {}

//======================================================================

DataTypeId FrameIndex::getDataType() const { return dataBlockId; }

//======================================================================

std::streamsize FrameIndex::getSerializedSize() const
{
	return std::streamsize(
		128
		+ (int(m_frameIndices.size()) * FrameIndexEntry::getSerializedSize_static()));
}

//======================================================================

bool FrameIndex::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	UINT8 verMajor;
	ibeosdk::readBE(is, verMajor);
	assert(verMajor == majorVersion);

	UINT8 verMinor;
	ibeosdk::readBE(is, verMinor);
	assert(verMinor == minorVersion);

	std::string nameStr;
	nameStr.resize(lengthOfNameString);
	is.read(&nameStr[0], lengthOfNameString); // assuming contiguous storage for std::string.

	UINT64 nbOfFrames;
	ibeosdk::readBE(is, nbOfFrames);

	ibeosdk::readBE(is, m_timeOffsetMs);
	m_framingPolicy.deserialize(is);

	INT32 numPaddingBytes;
	numPaddingBytes = INT32(128 - 35 - m_framingPolicy.getSerializedSize());

	UINT8 padding;
	while (numPaddingBytes-- > 0) {
		ibeosdk::readBE(is, padding);
	}
	m_frameIndices.resize(FrameVector::size_type(nbOfFrames)); // stl only supports 32 bit size.
	FrameVector::iterator it = m_frameIndices.begin();
	for (; it != m_frameIndices.end(); ++it) {
		it->deserialize(is);
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool FrameIndex::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, majorVersion);
	ibeosdk::writeBE(os, minorVersion);
	os.write(&(nameString[0]), lengthOfNameString);
	ibeosdk::writeBE(os, UINT64(m_frameIndices.size()));
	if (!m_frameIndices.empty())
		ibeosdk::writeBE(os, m_frameIndices.back().getTimeOffsetMs());
	else {
		ibeosdk::writeBE(os, NTPTime());
	}
	m_framingPolicy.serialize(os);
	INT32 numPadding = INT32(128 - 35 - m_framingPolicy.getSerializedSize());
	UINT8 padding = 0;
	while (numPadding-- > 0) {
		ibeosdk::writeBE(os, padding);
	}
	FrameVector::const_iterator it = m_frameIndices.begin();
	for (  ; it != m_frameIndices.end(); ++it) {
		it->serialize(os);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
