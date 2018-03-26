//======================================================================
/*! \file Version448.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Nov 4, 2013
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_VERSION448_HPP_SEEN
#define IBEOSDK_VERSION448_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/io.hpp>

#include <boost/detail/endian.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\brief Version number in 4/4/8 bit format with its date.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Jul 30, 2013
 *
 * Version number used for FPGA and host.
 *///-------------------------------------------------------------------
template<uint16_t endianess>
class Version448 : public Snippet {
public:
	static std::streamsize getSerializedSize_static();

public:
	//========================================
	/*!\brief Gets the version.
	 * \return The version.
	 *///-------------------------------------
	UINT16 getVersion() const { return m_version; }

	//========================================
	/*!\brief Gets the year.
	 * \return The year.
	 *///-------------------------------------
	UINT16 getYear() const { return m_year; }

	//========================================
	/*!\brief Gets the month.
	 * \return The month.
	 *///-------------------------------------
	UINT8 getMonth() const { return m_month; }

	//========================================
	/*!\brief Gets the day.
	 * \return The day.
	 *///-------------------------------------
	UINT8 getDay() const { return m_day; }

	//========================================
	/*!\brief Gets the hour.
	 * \return The hour.
	 *///-------------------------------------
	UINT8 getHour() const { return m_hour; }

	//========================================
	/*!\brief Gets the minute.
	 * \return The minute.
	 *///-------------------------------------
	UINT8 getMinute() const { return m_minute; }

public:
	//========================================
	/*!\brief Set the version to the given value.
	 * \param[in] newVersion  New value for version.
	 *///-------------------------------------
	void setVersion(const UINT16 newVersion) { this->m_version = newVersion; }

	//========================================
	/*!\brief Set the year to the given value.
	 * \param[in] newYear  New value for year.
	 *///-------------------------------------
	void setYear(const UINT16 newYear) { this->m_year = newYear; }

	//========================================
	/*!\brief Set the month to the given value.
	 * \param[in] newMonth  New value for month.
	 *///-------------------------------------
	void setMonth(const UINT8 newMonth) { this->m_month = newMonth; }

	//========================================
	/*!\brief Set the day to the given value.
	 * \param[in] newDay  New value for day.
	 *///-------------------------------------
	void setDay(const UINT8 newDay) { this->m_day = newDay; }

	//========================================
	/*!\brief Set the hour to the given value.
	 * \param[in] newHour  New value for hour.
	 *///-------------------------------------
	void setHour(const UINT8 newHour) { this->m_hour = newHour; }

	//========================================
	/*!\brief Set the minute to the given value.
	 * \param[in] newMinute  New value for minute.
	 *///-------------------------------------
	void setMinute(const UINT8 newMinute) { this->m_minute = newMinute; }

public:
	//========================================
	/*!\brief Read this object from a given buffer.
	 *
	 * Implements the deserialize method of Serializable.
	 *
	 * \param[in] is  Stream from where to read the
	 *                serialization from.
	 * \return \c true if the operation completed successfully.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool deserialize(std::istream& is);

	//========================================
	/*!\brief Serialize this object into the given buffer.
	 *
	 * Implements the serialize method of Serializable.
	 *
	 * \param[out] os  Stream where to write the
	 *                 serialization to.
	 * \return \c true if the operation completed successfully.
	 *         \c false otherwise.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const;

	//========================================
	/*!\brief Get length of the serialized data in bytes
	 *
	 * Implements the getSerializedSize method of Serializable.
	 *
	 * \return Size of the serialization in byte.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const;

protected:
	UINT16 m_version; //!< Version in 4bit.4bit.8bit format.
	UINT16 m_year;    //!< year of this version
	UINT8 m_month;    //!< month of this version
	UINT8 m_day;      //!< day of this version
	UINT8 m_hour;     //!< hour of this version
	UINT8 m_minute;   //!< minute of this version
}; // Version448




//======================================================================
/*!\brief Comparison operator for two Version448 objects for equality.
 * \param[in] lhs  First (left) Version448 object to be compared.
 * \param[in] rhs  Second (right) Version448 object to be compared.
 * \return \c true if the contents of both Version448 objects are
 *         identically. \c false otherwise.
 *///-------------------------------------------------------------------
template<uint16_t endianess>
bool operator==(const Version448<endianess>& lhs, const Version448<endianess>& rhs);

//======================================================================
/*!\brief Comparison operator for two Version448 objects for inequality.
 * \param[in] lhs  First (left) Version448 object to be compared.
 * \param[in] rhs  Second (right) Version448 object to be compared.
 * \return \c true if the contents of both Version448 objects are
 *         different. \c false otherwise.
 *///-------------------------------------------------------------------
template<uint16_t endianess>
bool operator!=(const Version448<endianess>& lhs, const Version448<endianess>& rhs);

//======================================================================

//======================================================================

//static
template<uint16_t endianess>
inline
std::streamsize Version448<endianess>::getSerializedSize_static()
{
	return 2 * std::streamsize(sizeof(UINT16))
	     + 4 * std::streamsize(sizeof(UINT8));
}

//======================================================================

template<>
inline
bool Version448<__BIG_ENDIAN>::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_version);
	ibeosdk::readBE(is, m_year);
	ibeosdk::readBE(is, m_month);
	ibeosdk::readBE(is, m_day);
	ibeosdk::readBE(is, m_hour);
	ibeosdk::readBE(is, m_minute);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == getSerializedSize_static());
}

//======================================================================

template<>
inline
bool Version448<__LITTLE_ENDIAN>::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readLE(is, m_version);
	ibeosdk::readLE(is, m_year);
	ibeosdk::readLE(is, m_month);
	ibeosdk::readLE(is, m_day);
	ibeosdk::readLE(is, m_hour);
	ibeosdk::readLE(is, m_minute);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == getSerializedSize_static());
}

//======================================================================

template<>
inline
bool Version448<__BIG_ENDIAN>::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_version);
	ibeosdk::writeBE(os, m_year);
	ibeosdk::writeBE(os, m_month);
	ibeosdk::writeBE(os, m_day);
	ibeosdk::writeBE(os, m_hour);
	ibeosdk::writeBE(os, m_minute);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == getSerializedSize_static());
}

//======================================================================

template<>
inline
bool Version448<__LITTLE_ENDIAN>::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeLE(os, m_version);
	ibeosdk::writeLE(os, m_year);
	ibeosdk::writeLE(os, m_month);
	ibeosdk::writeLE(os, m_day);
	ibeosdk::writeLE(os, m_hour);
	ibeosdk::writeLE(os, m_minute);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == getSerializedSize_static());
}

//======================================================================

template<uint16_t endianess>
inline
std::streamsize Version448<endianess>::getSerializedSize() const
{
	return getSerializedSize_static();
}

//======================================================================

template<uint16_t endianess>
inline
bool operator==(const Version448<endianess>& lhs, const Version448<endianess>& rhs)
{
	return (lhs.getVersion() == rhs.getVersion())
		&& (lhs.getYear() == rhs.getYear())
		&& (lhs.getMonth() == rhs.getMonth())
		&& (lhs.getDay() == rhs.getDay())
		&& (lhs.getHour() == rhs.getHour())
		&& (lhs.getMinute() == rhs.getMinute());
}

//======================================================================

template<uint16_t endianess>
inline
bool operator!=(const Version448<endianess>& lhs, const Version448<endianess>& rhs)
{
	return (lhs.getVersion() != rhs.getVersion())
		|| (lhs.getYear() != rhs.getYear())
		|| (lhs.getMonth() != rhs.getMonth())
		|| (lhs.getDay() != rhs.getDay())
		|| (lhs.getHour() != rhs.getHour())
		|| (lhs.getMinute() != rhs.getMinute());
}

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_VERSION448_HPP_SEEN

//======================================================================


