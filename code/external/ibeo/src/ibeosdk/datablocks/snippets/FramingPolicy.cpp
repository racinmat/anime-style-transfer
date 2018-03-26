//======================================================================
/*! \file FramingPolicy.cpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date May 11, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/FramingPolicy.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

const uint8_t FramingPolicy::deviceIdAny = 255;

//======================================================================

FramingPolicy::FramingPolicy()
  : m_triggerInNewFrame(true),
    m_triggers()
{}

//======================================================================

FramingPolicy::~FramingPolicy() {}

//======================================================================

std::streamsize FramingPolicy::getSerializedSize() const
{
	return std::streamsize(
	       1
	     + 2
	     + (m_triggers.size() * 3));
}

//======================================================================

bool FramingPolicy::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_triggerInNewFrame);
	UINT16 numTriggers = 0;
	ibeosdk::readBE(is, numTriggers);
	UINT16 tmpDataType = 0;
	UINT8  tmpDeviceId = 0;
	m_triggers.clear();
	for (TriggerSet::size_type i = 0; i<numTriggers; ++i) {
		ibeosdk::readBE(is, tmpDataType);
		ibeosdk::readBE(is, tmpDeviceId);
		m_triggers.insert(std::pair<ibeosdk::DataTypeId, UINT8>(ibeosdk::DataTypeId(tmpDataType), tmpDeviceId));
	}

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool FramingPolicy::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_triggerInNewFrame);
	ibeosdk::writeBE(os, UINT16(m_triggers.size()));
	TriggerSet::const_iterator it = m_triggers.begin();
	for (  ; it != m_triggers.end(); ++it) {
		ibeosdk::writeBE(os, UINT16(it->first));
		ibeosdk::writeBE(os, it->second);
	}

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool FramingPolicy::isTrigger(const ibeosdk::DataTypeId dataType, const UINT8 fromDeviceID) const
{
	TriggerSet::const_iterator it = m_triggers.find(dataType);
	return ((it != m_triggers.end())
		&& (it->second == deviceIdAny || it->second == fromDeviceID));
}

//======================================================================

} // namespace ibeosdk

//======================================================================
