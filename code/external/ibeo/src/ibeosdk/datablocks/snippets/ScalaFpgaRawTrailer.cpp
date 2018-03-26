//======================================================================
/*! \file ScalaFpgaRawTrailer.cpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ScalaFpgaRawTrailer.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

const uint16_t ScalaFpgaRawTrailer::blockId = 0xF020U;

//======================================================================

ScalaFpgaRawTrailer::ScalaFpgaRawTrailer()
  : m_operatingRegister(0),
    m_warning(0),
    m_error(0),
    m_reservedTrailer4(0),
    m_scanPointCounter(0),
    m_configurationRegister(0),
    m_reservedTrailer7(0)
{}

//======================================================================

ScalaFpgaRawTrailer::~ScalaFpgaRawTrailer() {}

//======================================================================

bool ScalaFpgaRawTrailer::operator==(const ScalaFpgaRawTrailer& other) const
{
	return this->m_operatingRegister == other.m_operatingRegister
	    && this->m_warning == other.m_warning
	    && this->m_error == other.m_error
	    && this->m_reservedTrailer4 == other.m_reservedTrailer4
	    && this->m_scanPointCounter == other.m_scanPointCounter
	    && this->m_configurationRegister == other.m_configurationRegister
	    && this->m_reservedTrailer7 == other.m_reservedTrailer7;
}

//======================================================================

bool ScalaFpgaRawTrailer::operator!=(const ScalaFpgaRawTrailer& other) const
{
	return !((*this) == other);
}

//======================================================================

bool ScalaFpgaRawTrailer::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	uint16_t readBlockId;
	readLE(is, readBlockId);
	if (readBlockId != ScalaFpgaRawTrailer::blockId)
		return false;

	readLE(is, m_operatingRegister);
	readLE(is, m_warning);
	readLE(is, m_error);
	readLE(is, m_reservedTrailer4);
	readLE(is, m_scanPointCounter);
	readLE(is, m_configurationRegister);
	readLE(is, m_reservedTrailer7);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == (this->getSerializedSize()));
}

//======================================================================

bool ScalaFpgaRawTrailer::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeLE(os, blockId);
	writeLE(os, m_operatingRegister);
	writeLE(os, m_warning);
	writeLE(os, m_error);
	writeLE(os, m_reservedTrailer4);
	writeLE(os, m_scanPointCounter);
	writeLE(os, m_configurationRegister);
	writeLE(os, m_reservedTrailer7);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
