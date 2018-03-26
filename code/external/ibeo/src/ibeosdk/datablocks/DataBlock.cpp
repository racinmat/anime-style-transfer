//======================================================================
/*! \file DataBlock.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 25, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/DataBlock.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

DataBlock::DataBlock()
  : Concurrency(),
    m_headerDeviceId(),
    m_headerNtpTime()
{}

//======================================================================

DataBlock::DataBlock(const DataBlock& other)
  : Concurrency(),
    m_headerDeviceId(other.m_headerDeviceId),
    m_headerNtpTime(other.m_headerNtpTime)
{}

//======================================================================

DataBlock& DataBlock::operator=(const DataBlock& other)
{
	if (this != &other) {
		m_headerDeviceId = other.m_headerDeviceId;
		m_headerNtpTime = other.m_headerNtpTime;
	}
	return *this;
}

//======================================================================

void DataBlock::setDataHeader(const IbeoDataHeader& dh)
{
	m_headerDeviceId = dh.getDeviceId();
	m_headerNtpTime = dh.getTimestamp();
}

//======================================================================

} // namespace ibeosdk

//======================================================================
