//======================================================================
/*! \file ParameterIndex.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 10, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_PARAMETERINDEX_HPP_SEEN
#define IBEOSDK_PARAMETERINDEX_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/misc/Unconvertable.hpp>
#include <ibeosdk/io.hpp>
#include <ibeosdk/bufferIO.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ParameterIndex : public ibeosdk::ComparableUnconvertable<uint16_t> {
public:
	explicit ParameterIndex(const uint16_t index) : ibeosdk::ComparableUnconvertable<uint16_t>(index) {}

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
}; // ParameterIndex

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_PARAMETERINDEX_HPP_SEEN

//======================================================================
