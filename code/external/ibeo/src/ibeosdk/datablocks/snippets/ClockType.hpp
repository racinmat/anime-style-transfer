//======================================================================
/*! \file ClockType.hpp
 *\verbatim
 * ------------------------------------------
 *  (C) 2017 Ibeo Automotive Systems GmbH, Hamburg, Germany
 * ------------------------------------------
 *
 *  Created on: Feb 21, 2017
 *          by: Anil Kumar
 *\endverbatim
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_CLOCKTYPE_HPP_SEEN
#define IBEOSDK_CLOCKTYPE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <ibeosdk/Time.hpp>
#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ClockType : public ibeosdk::Snippet {
public:
	enum ClockName
	{
		Clock_Unknown      = 0,
		Clock_Laserscanner = 1,
		Clock_ECU          = 2,
		Clock_CanBus       = 3,
		Clock_Camera       = 4,
		Clock_GpsImu       = 5,
		Clock_Other        = 6
	};

public:
	ClockType();
	ClockType(const uint8_t clockId, const ClockName clockName);
	virtual ~ClockType();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public: //getter
	uint8_t getClockId() const { return m_clockId; }
	ClockName getClockName() const { return m_clockName; }
	std::string getClockNameString(const ClockName cn);

public: //setter
	void setClockId(const uint8_t clockId) { this->m_clockId = clockId; }
	void setClockName(const ClockName clockName) { this->m_clockName = clockName; }

protected:
	//========================================
	/*!\brief A unique ID given to each processing device.
	 *
	 * The ID is required to distinguish between two similar
	 * clock names but from different devices and having
	 * different properties.
	 *///-------------------------------------
	uint8_t m_clockId;

	//========================================
	/*!\brief Name of the clock (enum) indicates the type
	 *        of device it is being received.
	 *///-------------------------------------
	ClockName m_clockName;
}; // ClockType

//======================================================================

bool operator==(const ClockType& clk1, const ClockType& clk2);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_CLOCKTYPE_HPP_SEEN

//======================================================================
