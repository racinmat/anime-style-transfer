//======================================================================
/*! \file TimeInterval.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 11, 2016
 *///-------------------------------------------------------------------

#ifndef TIMEINTERVAL_HPP_SEEN
#define TIMEINTERVAL_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/Time.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class TimeInterval {
public:
	TimeInterval(const NTPTime& startTime, const NTPTime& endTime,
	             const bool startIsIncluded = true,
	             const bool endIsIncluded = true)
	  : m_startTime(startTime),
	    m_endTime(endTime),
	    m_startIsIncluded(startIsIncluded),
	    m_endIsIncluded(endIsIncluded)
	{}

public:
	const NTPTime& getStartTime() const { return m_startTime; }
	const NTPTime& getEndTime() const { return m_endTime; }

	bool isStartTimeIncluded() const { return m_startIsIncluded; }
	bool isEndTimeIncluded() const { return m_endIsIncluded; }

protected:
	NTPTime m_startTime;
	NTPTime m_endTime;
	bool m_startIsIncluded;
	bool m_endIsIncluded;
}; // TimeInterval

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // TIMEINTERVAL_HPP_SEEN

//======================================================================
