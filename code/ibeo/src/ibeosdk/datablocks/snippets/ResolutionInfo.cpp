//======================================================================
/*! \file ResolutionInfo.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ResolutionInfo.hpp>
#include <ibeosdk/io.hpp>
#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

ResolutionInfo::ResolutionInfo()
  : m_resolutionStartAngle(0.0),
    m_resolution(0.0)
{}

//======================================================================

ResolutionInfo::ResolutionInfo(const ResolutionInfo& other)
  : m_resolutionStartAngle(other.m_resolutionStartAngle),
    m_resolution(other.m_resolution)
{}

//======================================================================

ResolutionInfo& ResolutionInfo::operator= (const ResolutionInfo& other)
{
	m_resolutionStartAngle = other.m_resolutionStartAngle;
	m_resolution = other.m_resolution;

	return *this;
}

//======================================================================

ResolutionInfo::~ResolutionInfo() {}

//======================================================================

//static
std::streamsize ResolutionInfo::getSerializedSize_static()
{
	return 2*std::streamsize(sizeof(float));
}

//======================================================================

bool ResolutionInfo::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_resolutionStartAngle);
	ibeosdk::readBE(is, m_resolution);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ResolutionInfo::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_resolutionStartAngle);
	ibeosdk::writeBE(os, m_resolution);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ResolutionInfo::operator==( const ResolutionInfo& other ) const
{
	if ( !(fuzzyCompareT<7>(m_resolutionStartAngle, other.getResolutionStartAngle())) )
		return false;
	
	if ( !(fuzzyCompareT<7>(m_resolution, other.getResolution())) )
		return false;
	
	return true;
}


//======================================================================

} // namespace ibeosdk

//======================================================================
