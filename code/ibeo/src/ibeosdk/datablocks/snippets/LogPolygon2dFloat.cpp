//======================================================================
/*! \file LogPolygon2dFloat.cpp
 *
 * \copydoc Copyright
 * \author Ruben Jungnickel (rju)
 * \date Sep 11, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/LogPolygon2dFloat.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

LogPolygon2dFloat::LogPolygon2dFloat()
  : Snippet(),
    m_poly(),
    m_msg()
{}

//======================================================================

LogPolygon2dFloat::~LogPolygon2dFloat() {}

//======================================================================

std::streamsize LogPolygon2dFloat::getSerializedSize() const
{
	return std::streamsize(sizeof(UINT32) +  m_msg.size()+1) + m_poly.getSerializedSize();
}

//======================================================================

bool LogPolygon2dFloat::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	uint32_t sizeInBytes=0;
	readBE(is, sizeInBytes);

	if (!m_poly.deserialize(is))
		return false;

	// the rest of the package is the string
	const std::streamsize stringSize = std::streamsize(sizeInBytes) - m_poly.getSerializedSize();
	char* buf = new char[std::size_t(stringSize)];
	is.read(buf,stringSize);
	m_msg.assign(buf,UINT32(stringSize-1));
	delete[] buf;

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool LogPolygon2dFloat::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	const uint32_t serSz = uint32_t(m_msg.size()+1 + uint32_t(m_poly.getSerializedSize()));
	writeBE(os,serSz);

	m_poly.serialize(os);

	os << m_msg;

	// Add termination char
	writeBE(os, UINT8(std::string::value_type(0)));


	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
