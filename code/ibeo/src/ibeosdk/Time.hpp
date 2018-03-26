//======================================================================
/*! \file Time.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 4, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_TIME_HPP_SEEN
#define IBEOSDK_TIME_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/inttypes.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/noncopyable.hpp>

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <time.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#if _MSC_VER < 1800 //VS 2013 is not tested 1900 == VS 2015
struct timespec {
	time_t tv_sec;
	time_t tv_nsec;
};
#endif //  before VS 2013

typedef time_t nanoseconds_t;
#else // _WIN32
typedef long int nanoseconds_t;
#endif // _WIN32
//======================================================================

namespace ibeosdk {

//======================================================================

class TimeConversion : private boost::noncopyable {
public:
	TimeConversion();
	TimeConversion(const std::string& formatStr);
	~TimeConversion();

public:
	const char* toString(const timespec ts, const int secPrecision=0) const;
	const char* toString(const tm& ltime, const nanoseconds_t nanoseconds, const int secPrecision=0) const;
	const char* toString(const time_t secs) const;
//	const char* toString(const long int secs, const uint32_t nSecs, const uint32_t nbOfDigits) const;
	std::string toString(const boost::posix_time::ptime ts, const int secPrecision = 0) const;

	std::string toStdString(const time_t secs) const;

protected:
	static const int szDefaultFmt = 18;
	static const char defaultFmt[szDefaultFmt];

	static const int szTimeStr = 64;
protected:
	char* fmt;
	mutable char timeStr[szTimeStr];
}; // TimeConversion

//======================================================================
//======================================================================
//======================================================================

namespace Time {

//======================================================================
/*!\brief Return the real-time clock of the system.
 * \return The current UTC time
 * \sa ibeo::localTime()
 *///-------------------------------------------------------------------
boost::posix_time::ptime universalTime();

//======================================================================
/*!\brief Return the real-time clock of the system.
 * \return The current UTC time
 * \sa ibeo::universalTime()
 *///-------------------------------------------------------------------
boost::posix_time::ptime localTime();

//======================================================================

} // namespace Time

//======================================================================
//======================================================================
//======================================================================

class NTPTime {
public:
	static NTPTime getInvalidTime() { NTPTime t; t.setInvalid(); return t; }

public:
	NTPTime() : m_Time(0) {}
	NTPTime(const uint64_t time) : m_Time(time) {}
	NTPTime(const uint32_t sec, const uint32_t frac)
	  : m_Time(0)
	{
		this->set(sec, frac);
	}

	NTPTime(const boost::posix_time::ptime& timestamp);

public: // Assignment operators
	NTPTime& operator=(const uint64_t u) { m_Time = u; return *this; }
	NTPTime& operator+= (const NTPTime& Q) { m_Time += Q.m_Time; return *this; }
	NTPTime& operator-= (const NTPTime& Q) { m_Time -= Q.m_Time; return *this; }

public: // Cast operators
	operator uint64_t() { return this->m_Time; }

public: // comparison operators
	bool operator==(const NTPTime& other) const { return (m_Time == other.m_Time); }
	bool operator!=(const NTPTime& other) const { return (m_Time != other.m_Time); }
	bool operator>=(const NTPTime& other) const { return (m_Time >= other.m_Time); }
	bool operator>(const NTPTime& other) const { return (m_Time > other.m_Time); }
	bool operator<=(const NTPTime& other) const { return (m_Time <= other.m_Time); }
	bool operator<(const NTPTime& other) const { return (m_Time < other.m_Time); }

public: // arithmetic operators
	NTPTime operator+(const NTPTime& other) const
	{
		NTPTime result = *this;
		result += other;
		return result;
	}

	NTPTime operator-(const NTPTime& other) const
	{
		NTPTime result = *this;
		result -= other;
		return result;
	}

public:
	/// returns the current time in seconds.
	uint32_t getSeconds() const { return ((uint32_t)(m_Time >> 32));}
	/// returns the fractional part of the second
	uint32_t getFracSeconds() const { return ((uint32_t)(m_Time & 0xFFFFFFFF));}
	/// returns the current time in milli seconds. take care for possible overflow.
	uint32_t getMilliseconds() const { const uint64_t t = 1000*m_Time ; return (uint32_t)((t>>32)&0xFFFFFFFF);}
	/// returns the current time in micro seconds. take care for possible overflow.
	uint32_t getMicroseconds() const { const uint64_t t = 125*m_Time/536871; return (uint32_t)(t&0xFFFFFFFF);}
	/// returns the time in seconds and microseconds (micros: 0..1000 0000). conversion error: 0.. -7.6 us.
	void getTime_s_us(uint32_t& sec, uint32_t& us) const { sec = getSeconds(); us = getFracSeconds()/4295;}
	uint64_t getTime(void) const {return m_Time;}

public:
	/// set the time in seconds and microseconds (micros: 0..1000 0000)
	///This routine uses the factorization: 2^32/10^6 = 4096 + 256 - 1825/32
	void setTime_s_us(const uint32_t sec, const uint32_t us) { m_Time = ((uint64_t)sec<<32) | ((us<<12)-((us*1852)>>5)+(us<<8));}

	void set(const uint64_t& u) {m_Time = u;}
	void set(const uint32_t sec, const uint32_t frac)
	{
		m_Time = sec;
		m_Time = m_Time<<32;
		m_Time |= frac;
	}

	void setInvalid() { m_Time = uint64_t(NOT_A_DATE_TIME) << 32; }

	/// This routine uses the factorization
	/// 2^32/10^6 = 4096 + 256 - 1825/32
	void setMicroseconds(const uint32_t u) { const uint64_t t = ((uint64_t)u * 1825) >> 5; m_Time = ((uint64_t)u << 12) + ((uint64_t)u << 8) - t;}
	void setMicroseconds(const uint64_t u) { const uint64_t t = (u * 1825) >> 5; m_Time = (u << 12) + (u << 8) - t;}
	void setMilliseconds(const uint32_t u) { m_Time = (uint64_t)u * 536870912 / 125;}
	void addMilliseconds(const uint32_t u) { NTPTime t; t.setMilliseconds(u); *this += t;}
	void addMicroseconds(const uint32_t u) { NTPTime t; t.setMicroseconds(u); *this += t;}

public:
//	timespec toTimeSpec() const; // NTP epoch 1-1-1900, Posix epoch 1-1-1970
	boost::posix_time::time_duration toTimeDurationSinceEpoch() const;
	boost::posix_time::ptime toPtime() const;

	/// Returns true if this timestamp does not represent an actual
	/// time, and toPTime() would return a
	/// boost::posix_time::not_a_date_time.
	bool is_not_a_date_time() const;

	void setFromPTime(const boost::posix_time::ptime& timestamp);

protected:
	static double round(const double v);

private:
	/// Constants to convert fractions of a second: 1/(2^32) s (NTP) to nanoseconds (1e-9 s).
	/*!
	 * For efficiency, the NTP epoch and factors to convert between ns and 1/(2^32)s
	 * are saved in static variables that are
	 * computed only once at system initialization.
	 * \sa epoch()
	 */
	static const double secondFractionNTPtoNanoseconds;
	static const double nanosecondsToSecondFractionNTP;
	/// Representation of a not_a_date_time value in the serialization
	static const uint32_t NOT_A_DATE_TIME;
	static const uint64_t NOT_A_DATE_TIME64;
	static const boost::posix_time::ptime m_epoch;

protected:
	uint64_t m_Time; ///< NTP time in 1/2^32 seconds (~233 ps)
}; // NTPTime

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_TIME_HPP_SEEN

//======================================================================

