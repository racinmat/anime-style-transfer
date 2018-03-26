//======================================================================
/*! \file SerialNumber.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Nov 4, 2013
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_SERIALNUMBER_HPP_SEEN
#define IBEOSDK_SERIALNUMBER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/io.hpp>

#include <boost/detail/endian.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
//======================================================================
//======================================================================
/*!\brief Serial number of a laserscanner.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Jul 30, 2013
 *
 *///-------------------------------------------------------------------
template<uint16_t endianess>
class SerialNumber : public ibeosdk::Snippet {
public:
	static std::streamsize getSerializedSize_static();

public:
	//========================================
	/*!\brief Get the month.
	 * \return The month.
	 *///-------------------------------------
	UINT8 getMonth() const { return m_month; }

	//========================================
	/*!\brief Get the year.
	 * \return The year.
	 *///-------------------------------------
	UINT8 getYear() const { return m_year; }

	//========================================
	/*!\brief Get the cnt1.
	 * \return The cnt1.
	 *///-------------------------------------
	UINT8 getCnt1() const { return m_cnt1; }

	//========================================
	/*!\brief Get the cnt0.
	 * \return The cnt0.
	 *///-------------------------------------
	UINT8 getCnt0() const { return m_cnt0; }

	//========================================
	/*!\brief Get the null.
	 * \return The null.
	 *///-------------------------------------
	UINT16 getNull() const { return m_null; }

public:
	//========================================
	/*!\brief Set the month.
	 * \param[in] newMonth  New month to be set.
	 *///-------------------------------------
	void setMonth(const UINT8 newMonth) { this->m_month = newMonth; }

	//========================================
	/*!\brief Set the year.
	 * \param[in] newYear  New year to be set.
	 *///-------------------------------------
	void setYear(const UINT8 newYear) { this->m_year = newYear; }

	//========================================
	/*!\brief Set the cnt1 value.
	 * \param[in] newCnt1  New cnt1 value to be set.
	 *///-------------------------------------
	void setCnt1(const UINT8 newCnt1) { this->m_cnt1 = newCnt1; }

	//========================================
	/*!\brief Set the cnt0 value.
	 * \param[in] newCnt0  New cnt0 value to be set.
	 *///-------------------------------------
	void setCnt0(const UINT8 newCnt0) { this->m_cnt0 = newCnt0; }

	//========================================
	/*!\brief Set the null value.
	 * \param[in] newNull  New null value to be set.
	 *///-------------------------------------
	void setNull(const UINT16 newNull) { this->m_null = newNull; }

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
	UINT8 m_month; //!< Month entry of the serial number
	UINT8 m_year;  //!< Year entry of the serial number
	UINT8 m_cnt1;
	UINT8 m_cnt0;
	UINT16 m_null;
}; // SerialNumber


//======================================================================
/*!\brief Comparison operator for two SerialNumber objects for equality.
 * \param[in] lhs  First (left) SerialNumber object to be compared.
 * \param[in] rhs  Second (right) SerialNumber object to be compared.
 * \return \c true if the contents of both SerialNumber objects are
 *         identically. \c false otherwise.
 *///-------------------------------------------------------------------
template<uint16_t endianess>
bool operator==(const SerialNumber<endianess> lhs, const SerialNumber<endianess> rhs);

//======================================================================
/*!\brief Comparison operator for two SerialNumber objects for inequality.
 * \param[in] lhs  First (left) SerialNumber object to be compared.
 * \param[in] rhs  Second (right) SerialNumber object to be compared.
 * \return \c true if the contents of both SerialNumber objects are
 *         different. \c false otherwise.
 *///-------------------------------------------------------------------
template<uint16_t endianess>
bool operator!=(const SerialNumber<endianess> lhs, const SerialNumber<endianess> rhs);

//======================================================================
//======================================================================
//======================================================================


//static
template<uint16_t endianess>
inline
std::streamsize SerialNumber<endianess>::getSerializedSize_static()
{
	return 4*std::streamsize(sizeof(UINT8)) + std::streamsize(sizeof(UINT16));
}

//======================================================================

template<>
inline
bool SerialNumber<__BIG_ENDIAN>::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_month);
	ibeosdk::readBE(is, m_year);
	ibeosdk::readBE(is, m_cnt1);
	ibeosdk::readBE(is, m_cnt0);
	ibeosdk::readBE(is, m_null);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == getSerializedSize_static());
}

//======================================================================

template<>
inline
bool SerialNumber<__LITTLE_ENDIAN>::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readLE(is, m_month);
	ibeosdk::readLE(is, m_year);
	ibeosdk::readLE(is, m_cnt1);
	ibeosdk::readLE(is, m_cnt0);
	ibeosdk::readLE(is, m_null);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == getSerializedSize_static());
}

//======================================================================

template<>
inline
bool SerialNumber<__BIG_ENDIAN>::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_month);
	ibeosdk::writeBE(os, m_year);
	ibeosdk::writeBE(os, m_cnt1);
	ibeosdk::writeBE(os, m_cnt0);
	ibeosdk::writeBE(os, m_null);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == getSerializedSize_static());
}

//======================================================================

template<>
inline
bool SerialNumber<__LITTLE_ENDIAN>::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeLE(os, m_month);
	ibeosdk::writeLE(os, m_year);
	ibeosdk::writeLE(os, m_cnt1);
	ibeosdk::writeLE(os, m_cnt0);
	ibeosdk::writeLE(os, m_null);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == getSerializedSize_static());
}

//======================================================================

template<uint16_t endianess>
inline
std::streamsize SerialNumber<endianess>::getSerializedSize() const
{
	return getSerializedSize_static();
}

//======================================================================

template<uint16_t endianess>
inline
bool operator==(const SerialNumber<endianess> lhs, const SerialNumber<endianess> rhs)
{
	return (lhs.getMonth() == rhs.getMonth())
		&& (lhs.getYear() == rhs.getYear())
		&& (lhs.getCnt1() == rhs.getCnt1())
		&& (lhs.getCnt0() == rhs.getCnt0())
		&& (lhs.getNull() == rhs.getNull());
}

//======================================================================

template<uint16_t endianess>
inline
bool operator!=(const SerialNumber<endianess> lhs, const SerialNumber<endianess> rhs)
{
	return (lhs.getMonth() != rhs.getMonth())
		|| (lhs.getYear() != rhs.getYear())
		|| (lhs.getCnt1() != rhs.getCnt1())
		|| (lhs.getCnt0() != rhs.getCnt0())
		|| (lhs.getNull() != rhs.getNull());
}

//======================================================================



//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SERIALNUMBER_HPP_SEEN

//======================================================================
