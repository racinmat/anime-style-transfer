//======================================================================
/*! \file CommandId.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 12, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_COMMANDID_HPP_SEEN
#define IBEOSDK_COMMANDID_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/Unconvertable.hpp>

#include <ibeosdk/io.hpp>
#include <ibeosdk/bufferIO.hpp>
#include <ibeosdk/Log.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class CommandId : public ibeosdk::ComparableUnconvertable<uint16_t> {
public:
	enum Command {
		// ECU commands
		CmdManagerSetFilter          = 0x0005, ///< Sets the data type filter, EMPTY REPLY
		CmdManagerAppBaseCtrl        = 0x000B, //!< EMPTY REPLY
		CmdManagerAppBaseStatus      = 0x000C, //!<

		// LUX3 commands
		CmdLuxReset                  = 0x0000, //!< ID of the Reset command, NO REPLY!
		CmdLuxGetStatus              = 0x0001, //!< ID of the GetStatus command
		CmdLuxSaveConfig             = 0x0004, //!< ID of the SaveConfig command, EMPTY REPLY!
		CmdLuxSetParameter           = 0x0010, //!< sets a parameter in the sensor, EMPTY REPLY!
		CmdLuxGetParameter           = 0x0011, //!< reads a parameter from the sensor
		CmdLuxResetDefaultParameters = 0x001A, //!< resets all parameters to the factory defaults, EMPTY REPLY!
		CmdLuxStartMeasure           = 0x0020, //!< starts the measurement with the currently configured settings
		CmdLuxStopMeasure            = 0x0021, //!< stops the measurement
		CmdLuxSetNTPTimestampSync    = 0x0034  //!< set the complete NTPtime stamp, EMPTY REPLY!1
	};

public:
	explicit CommandId(const uint16_t cId) : ibeosdk::ComparableUnconvertable<uint16_t>(cId) {}
	CommandId(const Command c) : ibeosdk::ComparableUnconvertable<uint16_t>(uint16_t(c)) {}

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
}; // CommandId

//======================================================================

template<>
inline
std::string toHex<CommandId>(const CommandId c)
{
	return toHex(uint16_t(c));
}


//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_COMMANDID_HPP_SEEN

//======================================================================
