//======================================================================
/*! \file Measurement.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Mar 14, 2014
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_MEASUREMENT_HPP_SEEN
#define IBEOSDK_MEASUREMENT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/MeasurementKey.hpp>

#include <ibeosdk/inttypes.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <boost/assert.hpp>

#include <stdexcept>

//======================================================================

namespace ibeosdk {

//======================================================================

class Measurement : public ibeosdk::Snippet {

public:
	enum MeasurementType {
		TypeVoid      = 0x00,
		TypeFloat     = 0x01,
		TypeDouble    = 0x02,
		TypeINT8      = 0x03,
		TypeUINT8     = 0x04,
		TypeINT16     = 0x05,
		TypeUINT16    = 0x06,
		TypeINT32     = 0x07,
		TypeUINT32    = 0x08,
		TypeINT64     = 0x09,
		TypeUINT64    = 0x0A,
		TypeBool      = 0x0B,
		TypeStdString = 0x0C
	};

public:
	Measurement();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	MeasurementKey getKey() const { return m_key; }
	MeasurementType getMeasurementType() const { return typeIdToType(getType()); }

	template<typename T>
	T getData() const { return boost::any_cast<T>(m_data); }

public: // setter
	void setKey(const MeasurementKey key) { m_key = key; }
	template<typename T> void setData(const T& data) { m_data = data; }

	template<typename T>
	void setValue(const MeasurementKey key,const T& value )
	{
		m_key = key;
		m_data = value;
	}

	void resetValue() { m_data = boost::any(); }

	//========================================

public:
	template<typename T>
	T getAs() const { throw std::runtime_error("Only specializations are allowed."); }

	bool isEqual(const Measurement& m) const;

	template<typename T>
	bool isOfType() const { return m_data.type() == typeid(T); }

	static
	MeasurementType typeIdToType(const std::type_info& tinfo);
	const std::type_info & getType() const { return m_data.type(); }

protected:
	MeasurementKey m_key;
	//MeasurementType m_measurementType;
	boost::any m_data;
}; // Measurement

//======================================================================
template<>
void Measurement::getAs<void>() const; 

template<>
float Measurement::getAs<float>() const;

template<>
double Measurement::getAs<double>() const;

template<>
INT8 Measurement::getAs<INT8>() const;

template<>
UINT8 Measurement::getAs<UINT8>() const;

template<>
INT16 Measurement::getAs<INT16>() const;

template<>
UINT16 Measurement::getAs<UINT16>() const;

template<>
INT32 Measurement::getAs<INT32>() const;

template<>
UINT32 Measurement::getAs<UINT32>() const;

template<>
INT64 Measurement::getAs<INT64>() const;

template<>
UINT64 Measurement::getAs<UINT64>() const;

template<>
bool Measurement::getAs<bool>() const;

template<>
std::string Measurement::getAs<std::string>() const;
//======================================================================

bool operator==(const Measurement& lhs, const Measurement& rhs);
bool operator!=(const Measurement& lhs, const Measurement& rhs);

//======================================================================

std::ostream& operator<<(std::ostream& oss, const Measurement& m);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_MEASUREMENT_HPP_SEEN

//======================================================================

