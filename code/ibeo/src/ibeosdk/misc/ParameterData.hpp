//======================================================================
/*! \file ParameterData.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 10, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_PARAMETERDATA_HPP_SEEN
#define IBEOSDK_PARAMETERDATA_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/io.hpp>
#include <ibeosdk/bufferIO.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ParameterData {
public:
//	explicit ParameterData(const uint8_t data);
//	explicit ParameterData(const int8_t data);
//	explicit ParameterData(const uint16_t data);
//	explicit ParameterData(const int16_t data);
	explicit ParameterData(const uint32_t data) : m_data(data) {}
	explicit ParameterData(const int32_t data) : m_data(uint32_t(data)) {}

public:
	operator uint32_t() const { return m_data; }

public: // BE io
	std::istream& readBE(std::istream& is)
	{
		ibeosdk::readBE(is, this->m_data);
		return is;
	}

	std::ostream& writeBE(std::ostream& os) const
	{
		ibeosdk::writeBE(os, this->m_data);
		return os;
	}

	void readBE(const char*& target) { ibeosdk::readBE(target, this->m_data); }
	void writeBE(char*& target) const { ibeosdk::writeBE(target, this->m_data); }

public: // LE io
	std::istream& readLE(std::istream& is)
	{
		ibeosdk::readLE(is, this->m_data);
		return is;
	}

	std::ostream& writeLE(std::ostream& os) const
	{
		ibeosdk::writeLE(os, this->m_data);
		return os;
	}

	void readLE(const char*& target) { ibeosdk::readLE(target, this->m_data); }
	void writeLE(char*& target) const { ibeosdk::writeLE(target, this->m_data); }

protected:
	uint32_t m_data;
}; // ParameterData

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_PARAMETERDATA_HPP_SEEN

//======================================================================
