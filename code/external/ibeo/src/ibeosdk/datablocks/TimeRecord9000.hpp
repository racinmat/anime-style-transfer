//======================================================================
/*! \file TimeRecord9000.hpp
 *
 * \copydoc Copyright
 * \author Anil Kumar (ank)
 * \date Feb 20, 2017
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_TIMERECORD9000_HPP_SEEN
#define IBEOSDK_TIMERECORD9000_HPP_SEEN

//======================================================================

#include <ibeosdk/Time.hpp>
#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/ClockType.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class TimeRecord9000 : public ibeosdk::RegisteredDataBlock<TimeRecord9000> {

public:
	enum FixMode {
		Not_Seen = 0,
		None = 1,
		Fix_2D = 2,
		Fix_3D = 3
	};

	typedef std::vector<NTPTime> NTPTimeVector;
	typedef std::vector<FixMode> FixModeVector;

public:
	TimeRecord9000();
	virtual ~TimeRecord9000();

public:
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	ClockType getInternalClockType() const { return m_internalClockType; }
	ClockType getExternalClockType() const { return m_externalClockType; }
	const NTPTimeVector& getInternalClockTimes() const { return m_internalClockTimes; }
	const NTPTimeVector& getExternalClockTimes() const { return m_externalClockTimes; }
	const FixModeVector& getFixModes() const { return m_fixModes; }

public:
	void setInternalClockType(const ClockType& clockType) { this->m_internalClockType = clockType; }
	void setExternalClockType(const ClockType& clockType) { this->m_externalClockType = clockType; }
	void setAll(const NTPTimeVector& externalTimes,
	            const NTPTimeVector& internalTimes,
	            const FixModeVector& fixModes);

public:
	static const int nbOfReserved = 4;

protected:
	ClockType m_externalClockType;
	ClockType m_internalClockType;
	NTPTimeVector m_externalClockTimes;
	NTPTimeVector m_internalClockTimes;
	FixModeVector m_fixModes;

private:
	uint32_t m_reserved[nbOfReserved];
}; // TimeRecord9000

//======================================================================

bool operator==(const TimeRecord9000& tr1, const TimeRecord9000& tr2);
bool operator!=(const TimeRecord9000& tr1, const TimeRecord9000& tr2);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_TIMERECORD9000_HPP_SEEN

//======================================================================
