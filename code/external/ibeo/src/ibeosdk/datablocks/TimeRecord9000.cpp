//======================================================================
/*! \file TimeRecord9000.cpp
 *
 * \copydoc Copyright
 * \author Anil Kumar (ank)
 * \date Feb 20, 2017
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/TimeRecord9000.hpp>
#include <ibeosdk/datablocks/snippets/ClockType.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

#include <cstring>

//======================================================================

namespace ibeosdk {

//======================================================================

template<>
inline
void readBE<TimeRecord9000::FixMode>(std::istream& is, TimeRecord9000::FixMode& value)
{
	uint8_t tmp;
	readBE(is, tmp);
	value = TimeRecord9000::FixMode(tmp);
}

//======================================================================

template<>
inline
void writeBE<TimeRecord9000::FixMode>(std::ostream& os, const TimeRecord9000::FixMode& value)
{
	const uint8_t tmp = value;
	writeBE(os, tmp);
}
//======================================================================


//======================================================================
// Specializations for RegisteredDataBlock<TimeRecord9000>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<TimeRecord9000>::dataBlockId = DataTypeId(DataTypeId::DataType_TimeRecord9000);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<TimeRecord9000>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<TimeRecord9000>::dataBlockId, ibeosdk::RegisteredDataBlock<TimeRecord9000>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<TimeRecord9000>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================
//======================================================================
//======================================================================


//======================================================================

TimeRecord9000::TimeRecord9000()
  : RegisteredDataBlock<TimeRecord9000>(),
    m_externalClockType(),
    m_internalClockType(),
    m_externalClockTimes(0),
    m_internalClockTimes(0),
    m_fixModes(0)
{
	memset(m_reserved, 0xff, nbOfReserved);
}

//======================================================================

TimeRecord9000::~TimeRecord9000() {}

//======================================================================

DataTypeId TimeRecord9000::getDataType() const { return dataBlockId; }

//======================================================================

std::streamsize TimeRecord9000::getSerializedSize() const
{
	std::streamsize result = 0;
	result += this->m_externalClockType.getSerializedSize();
	result += this->m_internalClockType.getSerializedSize();
	result += std::streamsize(sizeof(uint32_t));
	result += std::streamsize(8 * this->m_externalClockTimes.size());
	result += std::streamsize(8 * this->m_internalClockTimes.size());
	result += std::streamsize(sizeof(uint8_t) * this->m_fixModes.size());
	result += std::streamsize(sizeof(uint32_t)*TimeRecord9000::nbOfReserved);
	return result;
}

//======================================================================

bool TimeRecord9000::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	this->m_externalClockType.deserialize(is);
	this->m_internalClockType.deserialize(is);
	uint32_t vecSize = 0;
	ibeosdk::readBE(is, vecSize);

	if (this->m_externalClockTimes.size() != vecSize)
		this->m_externalClockTimes.resize(vecSize);

	TimeRecord9000::NTPTimeVector::iterator vecIter = this->m_externalClockTimes.begin();
	for (; vecIter != this->m_externalClockTimes.end(); ++vecIter) {
		ibeosdk::readBE(is, *vecIter);
	}

	if (this->m_internalClockTimes.size() != vecSize)
		this->m_internalClockTimes.resize(vecSize);
	vecIter = this->m_internalClockTimes.begin();
	for (; vecIter != this->m_internalClockTimes.end(); ++vecIter) {
		ibeosdk::readBE(is, *vecIter);
	}

	if (this->m_fixModes.size() != vecSize)
		this->m_fixModes.resize(vecSize);
	TimeRecord9000::FixModeVector::iterator vecIter2 = m_fixModes.begin();
	for (; vecIter2 != this->m_fixModes.end(); ++vecIter2) {
		uint8_t fixMode;
		ibeosdk::readBE(is, fixMode);
		(*vecIter2) = static_cast<TimeRecord9000::FixMode>(fixMode);

	}

	for (int i=0; i<TimeRecord9000::nbOfReserved; ++i) {
		ibeosdk::readBE(is, this->m_reserved[i]);
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool TimeRecord9000::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	m_externalClockType.serialize(os);
	m_internalClockType.serialize(os);
	const uint32_t vecSize = uint32_t(m_externalClockTimes.size());
	ibeosdk::writeBE(os, vecSize);

	TimeRecord9000::NTPTimeVector::const_iterator vecIter = m_externalClockTimes.begin();
	for (; vecIter != m_externalClockTimes.end(); ++vecIter) {
		ibeosdk::writeBE(os, *vecIter);
	}

	vecIter = m_internalClockTimes.begin();
	for (; vecIter != m_internalClockTimes.end(); ++vecIter) {
		ibeosdk::writeBE(os, *vecIter);
	}

	TimeRecord9000::FixModeVector::const_iterator vecIter2 = m_fixModes.begin();
	for (; vecIter2 != m_fixModes.end(); ++vecIter2) {
		ibeosdk::writeBE(os, static_cast<uint8_t>(*vecIter2));
	}

	for (int i=0; i<TimeRecord9000::nbOfReserved; ++i) {
		ibeosdk::writeBE(os, m_reserved[i]);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

void TimeRecord9000::setAll(const NTPTimeVector& externalTimes,
                            const NTPTimeVector& internalTimes,
                            const FixModeVector& fixModes)
{
	if (   (externalTimes.size() == internalTimes.size())
	    && (internalTimes.size() == fixModes.size())) {
		this->m_externalClockTimes = externalTimes;
		this->m_internalClockTimes = internalTimes;
		this->m_fixModes = fixModes;
	}
}

//======================================================================

bool operator==(const TimeRecord9000& tr1, const TimeRecord9000& tr2)
{
	return    tr1.getInternalClockType() == tr2.getInternalClockType()
	       && tr1.getExternalClockType() == tr2.getExternalClockType()
	       && tr1.getInternalClockTimes() == tr2.getInternalClockTimes()
	       && tr1.getExternalClockTimes() == tr2.getExternalClockTimes()
	       && tr1.getFixModes() == tr2.getFixModes();
}

//======================================================================

}// namespace ibeosdk

//======================================================================

