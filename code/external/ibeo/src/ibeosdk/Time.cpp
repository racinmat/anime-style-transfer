//======================================================================
/*! \file Time.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 4, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/Time.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <cmath>

#ifdef _WIN32
#if _MSC_VER <= 1700
inline int round(const float x) { return int(floor(x+0.5f)); }
inline int round(const double x) { return int(floor(x+0.5)); }
#endif // _MSC_VER
#endif // _WIN32

//======================================================================
namespace ibeosdk {
namespace Time {
//======================================================================

boost::posix_time::ptime universalTime()
{
	return boost::posix_time::microsec_clock::universal_time();
}

//======================================================================

boost::posix_time::ptime localTime()
{
	return boost::posix_time::microsec_clock::local_time();
}

//======================================================================
} // namespace Time
//======================================================================

TimeConversion::TimeConversion()
  : fmt(new char[TimeConversion::szDefaultFmt+1])
{
	strncpy(this->fmt, TimeConversion::defaultFmt, TimeConversion::szDefaultFmt);
}

//======================================================================

TimeConversion::TimeConversion(const std::string& formatStr)
  : fmt(new char[formatStr.size()+1])
{
	strcpy(this->fmt, formatStr.c_str());
}

//======================================================================

TimeConversion::~TimeConversion()
{
	delete[] this->fmt;
}

//======================================================================

const char* TimeConversion::toString(const timespec ts, const int secPrecision) const
{
	const tm* ltime = localtime(&ts.tv_sec);
	return toString(*ltime, ts.tv_nsec, secPrecision);
}

//======================================================================

const char* TimeConversion::toString(const tm& ltime, const nanoseconds_t nanoseconds, const int secPrecision) const
{
	strftime(this->timeStr, this->szTimeStr, fmt, &ltime);

	std::stringstream ss;
	const int p = (secPrecision) > 9 ? 9 : secPrecision;
	if (p > 0) {
		nanoseconds_t secFrac;
		switch (p) {
		case 1: secFrac = nanoseconds / 100000000; break;
		case 2: secFrac = nanoseconds / 10000000; break;
		case 3: secFrac = nanoseconds / 1000000; break;
		case 4: secFrac = nanoseconds / 100000; break;
		case 5: secFrac = nanoseconds / 10000; break;
		case 6: secFrac = nanoseconds / 1000; break;
		case 7: secFrac = nanoseconds / 100; break;
		case 8: secFrac = nanoseconds / 10; break;
		case 9: secFrac = nanoseconds / 1; break;
		}

		ss << '.' << std::setfill('0') << std::setw(p) << secFrac;
		strncat(this->timeStr, ss.str().c_str(), this->szTimeStr);
	}
	return timeStr;

}


//======================================================================

std::string TimeConversion::toString(const boost::posix_time::ptime ts, const int secPrecision) const
{
	using namespace boost::posix_time;

	if (ts == not_a_date_time)
		return "not_a_date_time";

	// determine the nanoseconds given in ts
	const int h = ts.time_of_day().hours();
	const int m = ts.time_of_day().minutes();
	const int s = ts.time_of_day().seconds();
	const time_duration r = time_duration(h, m, s);
	const time_duration rest = ts.time_of_day() - r;
	const int nanoseconds = int(rest.total_nanoseconds()); // not more than 1 bil nanoseconds here.

	return toString(to_tm(ts), nanoseconds, secPrecision);
}

//======================================================================

const char* TimeConversion::toString(const time_t secs) const
{
	const tm* ltime = localtime(&secs);
	strftime(this->timeStr, this->szTimeStr, this->fmt, ltime);
	return this->timeStr;
}

////======================================================================
//
//const char* TimeConversion::toString(const long int secs, const uint32_t nSecs, const uint32_t nbOfDigits) const
//{
//	#error failed to take into account the different epochs.
//	//TODO: number of digit is taken as +1
//	assert(nbOfDigits <= 9);
//	tm* ltime = localtime(&secs);
//	strftime(this->timeStr, this->szTimeStr, this->fmt, ltime);
//
//	const int bufFilledTo = strlen(this->timeStr)+ 1;
//	if (bufFilledTo + nbOfDigits > int(this->szTimeStr)) {
//		std::cerr << __FILE__ << "::" << __func__ << "(" << __LINE__
//				<< ") : time will not fit into the provided buffer" << std::endl;
//	}
//
//	uint32_t f = 1;
//	for (int i=9; i > int(nbOfDigits); --i)
//		f *=10;
//	const uint32_t n = nSecs / f;
//
//	std::stringstream ss;
//	ss << '.' << std::setfill('0') << std::setw(nbOfDigits) << n;
//	strncat(this->timeStr, ss.str().c_str(), this->szTimeStr - bufFilledTo);
//	return this->timeStr;
//}

//======================================================================

std::string TimeConversion::toStdString(const time_t secs) const
{
	const tm* ltime = localtime(&secs);
	strftime(this->timeStr, sizeof(this->timeStr), fmt, ltime);
	return std::string(timeStr);
}

//======================================================================
//======================================================================
//======================================================================



TimeConversion tconv;





//======================================================================
//======================================================================
//======================================================================

//static
const double NTPTime::secondFractionNTPtoNanoseconds (0.2328306436538696); // = 2^-32 * 1e9
const double NTPTime::nanosecondsToSecondFractionNTP (4.29496729600000);   // = 2^32 * 1e-9
const uint32_t NTPTime::NOT_A_DATE_TIME = 0xFFFFFFFF;
const uint64_t NTPTime::NOT_A_DATE_TIME64 = 0xFFFFFFFFFFFFFFFFULL;
const boost::posix_time::ptime NTPTime::m_epoch(boost::gregorian::date (1900, 1, 1));
const char TimeConversion::defaultFmt[TimeConversion::szDefaultFmt] = "%Y-%m-%d %H:%M:%S";

// //======================================================================
// TODO: add a conversion to timespec
// NTP epoch 1-1-1900, Posix epoch 1-1-1970
//
//timespec NTPTime::toTimeSpec() const
//{
//  #error failed to take into account the different epochs.
//	timespec ts = {0,0};
//	if (is_not_a_date_time()) {
//		return ts;
//	}
//	else {
//		ts.tv_sec = this->getSeconds();
//		ts.tv_nsec = uint32_t(round(double(this->getFracSeconds()) * secondFractionNTPtoNanoseconds));
//
//		return ts;
//	}
//}

//======================================================================

NTPTime::NTPTime (const boost::posix_time::ptime& timestamp)
{
	setFromPTime(timestamp);
}

//======================================================================

boost::posix_time::time_duration NTPTime::toTimeDurationSinceEpoch() const
{
	if (is_not_a_date_time()) {
		return boost::posix_time::not_a_date_time;
	}
	else {
		boost::posix_time::time_duration timediff(boost::posix_time::time_duration::hour_type(getSeconds() / 3600),
		                                          boost::posix_time::time_duration::min_type((getSeconds() % 3600) / 60),
		                                          boost::posix_time::time_duration::sec_type(getSeconds() % 60));
		return timediff +
#ifdef BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
			boost::posix_time::nanoseconds (uint32_t(round(double(getFracSeconds()) * secondFractionNTPtoNanoseconds))); // POSIX time has ns resolution
#else
			boost::posix_time::microseconds (uint32_t(round(double(getFracSeconds()) * secondFractionNTPtoNanoseconds * 0.001))); // POSIX time has micro seconds resolution, e.g. on Win/mingw
#endif
	}
}

//======================================================================

boost::posix_time::ptime NTPTime::toPtime() const
{
	if (is_not_a_date_time()) {
		return boost::posix_time::not_a_date_time;
	}
	else {
		return m_epoch + toTimeDurationSinceEpoch();
	}
}

//======================================================================

bool NTPTime::is_not_a_date_time() const
{
	return (uint32_t(m_Time>>32) == NOT_A_DATE_TIME);
}

//======================================================================

void NTPTime::setFromPTime(const boost::posix_time::ptime& timestamp)
{
	if (timestamp.is_not_a_date_time()) {
		m_Time = NOT_A_DATE_TIME64;
	}
	else {
		boost::posix_time::time_duration timediff = timestamp - m_epoch;
		const uint32_t secs = uint32_t(timediff.total_seconds());
#ifdef BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
		const uint32_t fracs = uint32_t(round(double(timediff.fractional_seconds()) * nanosecondsToSecondFractionNTP)); // fractional_seconds() are nanoseconds
#else
		const uint32_t fracs = uint32_t(round(double(timediff.fractional_seconds()) * nanosecondsToSecondFractionNTP * 1000.0)); // fractional_seconds() are microseconds, e.g. on Win/mingw
#endif
		this->set(secs, fracs);
	}
}

//======================================================================

double NTPTime::round(const double v)
{
//#  ifdef _MSC_VER
//	// Microsoft doesn't have round() ?!??? It is standarized in C99 and
//	// we'll provide our own workaround here.
	return floor(v + 0.5);
//#  endif // _MSC_VER
}

//======================================================================

} // namespace ibeosdk

//======================================================================
