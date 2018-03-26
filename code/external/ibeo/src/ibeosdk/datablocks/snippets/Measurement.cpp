//======================================================================
/*! \file ObjectEcuEt.cpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Mar 14, 2014
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/Measurement.hpp>
#include <ibeosdk/io.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/Math.hpp>

#include <boost/scoped_array.hpp>

#include <sstream>

//======================================================================

namespace ibeosdk {

//======================================================================

Measurement::Measurement()
 :  m_key(0),
    m_data(0)
{}

//======================================================================

std::streamsize Measurement::getSerializedSize() const
{
	std::streamsize result = std::streamsize(sizeof(UINT16)) // m_key
	                       + std::streamsize(sizeof(UINT8)); // m_measurementType

	switch (this->getMeasurementType()) {
	case TypeVoid:   return result;
	case TypeFloat:  return result + std::streamsize(sizeof(float));
	case TypeDouble: return result + std::streamsize(sizeof(double));
	case TypeINT8:
	case TypeUINT8:  return result + std::streamsize(sizeof(UINT8));
	case TypeINT16:
	case TypeUINT16: return result + std::streamsize(sizeof(UINT16));
	case TypeINT32:
	case TypeUINT32: return result + std::streamsize(sizeof(UINT32));
	case TypeINT64:
	case TypeUINT64: return result + std::streamsize(sizeof(UINT64));
	case TypeBool:   return result + std::streamsize(sizeof(UINT8));
	case TypeStdString:
		result += 4; // 4 bytes for the BLOB data length
		result += std::streamsize(boost::any_cast<std::string>(m_data).size()); // and the BLOB data itself
		break;
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
	return result;
}

//======================================================================

bool Measurement::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	union allTypes {
		float flt;
		double dbl;
		int8_t int8;
		uint8_t uint8;
		int16_t int16;
		uint16_t uint16;
		int32_t int32;
		uint32_t uint32;
		int64_t int64;
		uint64_t uint64;
		bool boolean;
	};

	allTypes tmp;

	readBE(is, m_key);

	UINT8 type;
	readBE(is, type);

	switch (MeasurementType(type)) {
	case TypeVoid:   m_data = boost::any();                         break;
	case TypeFloat:  readBE(is, tmp.flt);     m_data = tmp.flt;     break;
	case TypeDouble: readBE(is, tmp.dbl);     m_data = tmp.dbl;     break;
	case TypeINT8:   readBE(is, tmp.int8);    m_data = tmp.int8;    break;
	case TypeUINT8:  readBE(is, tmp.uint8);   m_data = tmp.uint8;   break;
	case TypeINT16:  readBE(is, tmp.int16);   m_data = tmp.int16;   break;
	case TypeUINT16: readBE(is, tmp.uint16);  m_data = tmp.uint16;  break;
	case TypeINT32:  readBE(is, tmp.int32);   m_data = tmp.int32;   break;
	case TypeUINT32: readBE(is, tmp.uint32);  m_data = tmp.uint32;  break;
	case TypeINT64:  readBE(is, tmp.int64);   m_data = tmp.int64;   break;
	case TypeUINT64: readBE(is, tmp.uint64);  m_data = tmp.uint64;  break;
	case TypeBool:   readBE(is, tmp.boolean); m_data = tmp.boolean; break;
	case TypeStdString:
		{
			UINT32 strLen;
			readBE(is, strLen);
			boost::scoped_array<char> strBuf (new char[strLen]);
			is.read(strBuf.get(), strLen);
			m_data = std::string(strBuf.get(), strLen);
		}
		break;
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool Measurement::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, m_key);

	const MeasurementType mt = this->getMeasurementType();
	const UINT8 type = UINT8(mt);
	writeBE(os, type);

	switch (mt) {
	case TypeVoid:   break;
	case TypeFloat:  writeBE(os, boost::any_cast<float>(m_data));  break;
	case TypeDouble: writeBE(os, boost::any_cast<double>(m_data)); break;
	case TypeINT8:   writeBE(os, boost::any_cast<INT8>(m_data));   break;
	case TypeUINT8:  writeBE(os, boost::any_cast<UINT8>(m_data));  break;
	case TypeINT16:  writeBE(os, boost::any_cast<INT16>(m_data));  break;
	case TypeUINT16: writeBE(os, boost::any_cast<UINT16>(m_data)); break;
	case TypeINT32:  writeBE(os, boost::any_cast<INT32>(m_data));  break;
	case TypeUINT32: writeBE(os, boost::any_cast<UINT32>(m_data)); break;
	case TypeINT64:  writeBE(os, boost::any_cast<INT64>(m_data));  break;
	case TypeUINT64: writeBE(os, boost::any_cast<UINT64>(m_data)); break;
	case TypeBool:   writeBE(os, boost::any_cast<bool>(m_data));   break;
	case TypeStdString:
		writeBE(os, UINT32(boost::any_cast<std::string>(m_data).size()));
		os.write(boost::any_cast<std::string>(m_data).c_str(), std::streamsize(boost::any_cast<std::string>(m_data).size()));
		break;
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

template<>
void Measurement::getAs<void>() const
{
	throw std::runtime_error("Not able to convert any type to void");

}

//======================================================================

template<>
float Measurement::getAs<float>() const
{
	switch (this->getMeasurementType()) {
	case TypeVoid:
		throw std::runtime_error("Not able to convert from void to float");
	case TypeFloat:    return getData<float>();
	case TypeDouble:   return static_cast<float>(getData<double>());
	case TypeINT8:     return static_cast<float>(getData<INT8>());
	case TypeUINT8:    return static_cast<float>(getData<UINT8>());
	case TypeINT16:    return static_cast<float>(getData<INT16>());
	case TypeUINT16:   return static_cast<float>(getData<UINT16>());
	case TypeINT32:    return static_cast<float>(getData<INT32>());
	case TypeUINT32:   return static_cast<float>(getData<UINT32>());
	case TypeINT64:    return static_cast<float>(getData<INT64>());
	case TypeUINT64:   return static_cast<float>(getData<UINT64>());
	case TypeBool:     return static_cast<float>(getData<bool>());
	case TypeStdString:
		throw std::runtime_error("Not able to convert from std::str to float");
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
}

//======================================================================

template<>
double Measurement::getAs<double>() const
{
	switch (this->getMeasurementType()) {
	case TypeVoid:
		throw std::runtime_error("Not able to convert from void to double");
	case TypeFloat:    return static_cast<double>(getData<float>());
	case TypeDouble:   return getData<double>();
	case TypeINT8:     return static_cast<double>(getData<INT8>());
	case TypeUINT8:    return static_cast<double>(getData<UINT8>());
	case TypeINT16:    return static_cast<double>(getData<INT16>());
	case TypeUINT16:   return static_cast<double>(getData<UINT16>());
	case TypeINT32:    return static_cast<double>(getData<INT32>());
	case TypeUINT32:   return static_cast<double>(getData<UINT32>());
	case TypeINT64:    return static_cast<double>(getData<INT64>());
	case TypeUINT64:   return static_cast<double>(getData<UINT64>());
	case TypeBool:     return static_cast<double>(getData<bool>());
	case TypeStdString:
		throw std::runtime_error("Not able to convert from std::str to double");
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
}

//======================================================================

template<>
INT8 Measurement::getAs<INT8>() const
{
	switch (this->getMeasurementType()) {
	case TypeVoid:
		throw std::runtime_error("Not able to convert from void to INT8");
	case TypeFloat:    return static_cast<INT8>(getData<float>());
	case TypeDouble:   return static_cast<INT8> (getData<double>());
	case TypeINT8:     return (getData<INT8>());
	case TypeUINT8:    return static_cast<INT8>(getData<UINT8>());
	case TypeINT16:    return static_cast<INT8>(getData<INT16>());
	case TypeUINT16:   return static_cast<INT8>(getData<UINT16>());
	case TypeINT32:    return static_cast<INT8>(getData<INT32>());
	case TypeUINT32:   return static_cast<INT8>(getData<UINT32>());
	case TypeINT64:    return static_cast<INT8>(getData<INT64>());
	case TypeUINT64:   return static_cast<INT8>(getData<UINT64>());
	case TypeBool:     return static_cast<INT8>(getData<bool>());
	case TypeStdString:
		throw std::runtime_error("Not able to convert from std::str to INT8");
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
}

//======================================================================

template<>
UINT8 Measurement::getAs<UINT8>() const
{
	switch (this->getMeasurementType()) {
	case TypeVoid:
		throw std::runtime_error("Not able to convert from void to UINT8");
	case TypeFloat:    return static_cast<UINT8>(getData<float>());
	case TypeDouble:   return static_cast<UINT8>(getData<double>());
	case TypeINT8:     return static_cast<UINT8>(getData<INT8>());
	case TypeUINT8:    return (getData<UINT8>());
	case TypeINT16:    return static_cast<UINT8>(getData<INT16>());
	case TypeUINT16:   return static_cast<UINT8>(getData<UINT16>());
	case TypeINT32:    return static_cast<UINT8>(getData<INT32>());
	case TypeUINT32:   return static_cast<UINT8>(getData<UINT32>());
	case TypeINT64:    return static_cast<UINT8>(getData<INT64>());
	case TypeUINT64:   return static_cast<UINT8>(getData<UINT64>());
	case TypeBool:     return static_cast<UINT8>(getData<bool>());
	case TypeStdString:
		throw std::runtime_error("Not able to convert from std::str to UINT8");
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
}

//======================================================================

template<>
INT16 Measurement::getAs<INT16>() const
{
	switch (this->getMeasurementType()) {
	case TypeVoid:
		throw std::runtime_error("Not able to convert from void to INT16");
	case TypeFloat:    return static_cast<INT16>(getData<float>());
	case TypeDouble:   return static_cast<INT16>(getData<double>());
	case TypeINT8:     return static_cast<INT16>(getData<INT8>());
	case TypeUINT8:    return static_cast<INT16>(getData<UINT8>());
	case TypeINT16:    return (getData<INT16>());
	case TypeUINT16:   return static_cast<INT16>(getData<UINT16>());
	case TypeINT32:    return static_cast<INT16>(getData<INT32>());
	case TypeUINT32:   return static_cast<INT16>(getData<UINT32>());
	case TypeINT64:    return static_cast<INT16>(getData<INT64>());
	case TypeUINT64:   return static_cast<INT16>(getData<UINT64>());
	case TypeBool:     return static_cast<INT16>(getData<bool>());
	case TypeStdString:
		throw std::runtime_error("Not able to convert from std::str to INT16");
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
}

//======================================================================

template<>
UINT16 Measurement::getAs<UINT16>() const
{
	switch (this->getMeasurementType()) {
	case TypeVoid:
		throw std::runtime_error("Not able to convert from void to UINT16");
	case TypeFloat:    return static_cast<UINT16>(getData<float>());
	case TypeDouble:   return static_cast<UINT16>(getData<double>());
	case TypeINT8:     return static_cast<UINT16>(getData<INT8>());
	case TypeUINT8:    return static_cast<UINT16>(getData<UINT8>());
	case TypeINT16:    return static_cast<UINT16>(getData<INT16>());
	case TypeUINT16:   return (getData<UINT16>());
	case TypeINT32:    return static_cast<UINT16>(getData<INT32>());
	case TypeUINT32:   return static_cast<UINT16>(getData<UINT32>());
	case TypeINT64:    return static_cast<UINT16>(getData<INT64>());
	case TypeUINT64:   return static_cast<UINT16>(getData<UINT64>());
	case TypeBool:     return static_cast<UINT16>(getData<bool>());
	case TypeStdString:
		throw std::runtime_error("Not able to convert from std::str to UINT16");
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
}

//======================================================================

template<>
INT32 Measurement::getAs<INT32>() const
{
	switch (this->getMeasurementType()) {
	case TypeVoid:
		throw std::runtime_error("Not able to convert from void to INT32");
	case TypeFloat:    return static_cast<INT32>(getData<float>());
	case TypeDouble:   return static_cast<INT32>(getData<double>());
	case TypeINT8:     return static_cast<INT32>(getData<INT8>());
	case TypeUINT8:    return static_cast<INT32>(getData<UINT8>());
	case TypeINT16:    return static_cast<INT32>(getData<INT16>());
	case TypeUINT16:   return static_cast<INT32>(getData<UINT16>());
	case TypeINT32:    return (getData<INT32>());
	case TypeUINT32:   return static_cast<INT32>(getData<UINT32>());
	case TypeINT64:    return static_cast<INT32>(getData<INT64>());
	case TypeUINT64:   return static_cast<INT32>(getData<UINT64>());
	case TypeBool:     return static_cast<INT32>(getData<bool>());
	case TypeStdString:
		throw std::runtime_error("Not able to convert from std::str to INT32");
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
}

//======================================================================

template<>
UINT32 Measurement::getAs<UINT32>() const
{
	switch (this->getMeasurementType()) {
	case TypeVoid:
		throw std::runtime_error("Not able to convert from void to UINT32");
	case TypeFloat:    return static_cast<UINT32>(getData<float>());
	case TypeDouble:   return static_cast<UINT32>(getData<double>());
	case TypeINT8:     return static_cast<UINT32>(getData<INT8>());
	case TypeUINT8:    return static_cast<UINT32>(getData<UINT8>());
	case TypeINT16:    return static_cast<UINT32>(getData<INT16>());
	case TypeUINT16:   return static_cast<UINT32>(getData<UINT16>());
	case TypeINT32:    return static_cast<UINT32>(getData<INT32>());
	case TypeUINT32:   return (getData<UINT32>());
	case TypeINT64:    return static_cast<UINT32>(getData<INT64>());
	case TypeUINT64:   return static_cast<UINT32>(getData<UINT64>());
	case TypeBool:     return static_cast<UINT32>(getData<bool>());
	case TypeStdString:
		throw std::runtime_error("Not able to convert from std::str to UINT32");
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
}

//======================================================================

template<>
INT64 Measurement::getAs<INT64>() const
{
	switch (this->getMeasurementType()) {
	case TypeVoid:
		throw std::runtime_error("Not able to convert from void to INT64");
	case TypeFloat:    return static_cast<INT64>(getData<float>());
	case TypeDouble:   return static_cast<INT64>(getData<double>());
	case TypeINT8:     return static_cast<INT64>(getData<INT8>());
	case TypeUINT8:    return static_cast<INT64>(getData<UINT8>());
	case TypeINT16:    return static_cast<INT64>(getData<INT16>());
	case TypeUINT16:   return static_cast<INT64>(getData<UINT16>());
	case TypeINT32:    return static_cast<INT64>(getData<INT32>());
	case TypeUINT32:   return static_cast<INT64>(getData<UINT32>());
	case TypeINT64:    return (getData<INT64>());
	case TypeUINT64:   return static_cast<INT64>(getData<UINT64>());
	case TypeBool:     return static_cast<INT64>(getData<bool>());
	case TypeStdString:
		throw std::runtime_error("Not able to convert from std::str to INT64");
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
}

//======================================================================

template<>
UINT64 Measurement::getAs<UINT64>() const
{
	switch (this->getMeasurementType()) {
	case TypeVoid:
		throw std::runtime_error("Not able to convert from void to UINT64");
	case TypeFloat:    return static_cast<UINT64>(getData<float>());
	case TypeDouble:   return static_cast<UINT64>(getData<double>());
	case TypeINT8:     return static_cast<UINT64>(getData<INT8>());
	case TypeUINT8:    return static_cast<UINT64>(getData<UINT8>());
	case TypeINT16:    return static_cast<UINT64>(getData<INT16>());
	case TypeUINT16:   return static_cast<UINT64>(getData<UINT16>());
	case TypeINT32:    return static_cast<UINT64>(getData<INT32>());
	case TypeUINT32:   return static_cast<UINT64>(getData<UINT32>());
	case TypeINT64:    return static_cast<UINT64>(getData<INT64>());
	case TypeUINT64:   return (getData<UINT64>());
	case TypeBool:     return static_cast<UINT64>(getData<bool>());
	case TypeStdString:
		throw std::runtime_error("Not able to convert from std::str to UINT64");
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
}

//======================================================================

template<>
bool Measurement::getAs<bool>() const
{
	switch (this->getMeasurementType()) {
	case TypeVoid:
		throw std::runtime_error("Not able to convert from void to bool");
	case TypeFloat:    return (getData<float>() == 0.0f);
	case TypeDouble:   return (getData<double>() == 0.0);
	case TypeINT8:     return (getData<INT8>() == 0);
	case TypeUINT8:    return (getData<UINT8>() == 0U);
	case TypeINT16:    return (getData<INT16>() == 0);
	case TypeUINT16:   return (getData<UINT16>() == 0U);
	case TypeINT32:    return (getData<INT32>() == 0);
	case TypeUINT32:   return (getData<UINT32>() == 0U);
	case TypeINT64:    return (getData<INT64>() == 0);
	case TypeUINT64:   return (getData<UINT64>() == 0U);
	case TypeBool:     return (getData<bool>());
	case TypeStdString:
		throw std::runtime_error("Not able to convert from std::str to float");
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
}

//======================================================================

template<>
std::string Measurement::getAs<std::string>() const
{
	switch (this->getMeasurementType()) {
	case TypeVoid:
		throw std::runtime_error("Not able to convert from void to std::str");
	case TypeFloat:
		throw std::runtime_error("Not able to convert from float to std::str");
	case TypeDouble:
		throw std::runtime_error("Not able to convert from double to std::str");
	case TypeINT8:
		throw std::runtime_error("Not able to convert from INT8 to std::str");
	case TypeUINT8:
		throw std::runtime_error("Not able to convert from UINT8 to std::str");
	case TypeINT16:
		throw std::runtime_error("Not able to convert from INT16 to std::str");
	case TypeUINT16:
		throw std::runtime_error("Not able to convert from UINT16 to std::str");
	case TypeINT32:
		throw std::runtime_error("Not able to convert from INT32 to std::str");
	case TypeUINT32:
		throw std::runtime_error("Not able to convert from UINT32 to std::str");
	case TypeINT64:
		throw std::runtime_error("Not able to convert from INT64 to std::str");
	case TypeUINT64:
		throw std::runtime_error("Not able to convert from UINT64 to std::str");
	case TypeBool:
		throw std::runtime_error("Not able to convert from bool to std::str");
	case TypeStdString: return (getData<std::string>());
	default:
		throw std::runtime_error("Unknown MeasurementType in Measurement");
	} // switch
}

//======================================================================

bool Measurement::isEqual(const Measurement& m) const
{
	if ((getKey() != m.getKey()) || (getMeasurementType() != m.getMeasurementType())) {
		return false;
	}

	// Same key, same type. Now check the actual value.
	const MeasurementType mtype = typeIdToType(getType());
	switch (mtype) {
	case TypeVoid:      return true;
	case TypeFloat:     return floatEqual(getData<float>(), m.getData<float>());
	case TypeDouble:    return doubleEqual(getData<double>(), m.getData<double>());
	case TypeINT8:      return getData<INT8>() == m.getData<INT8>();
	case TypeUINT8:     return getData<UINT8>() == m.getData<UINT8>();
	case TypeINT16:     return getData<INT16>() == m.getData<INT16>();
	case TypeUINT16:    return getData<UINT16>() == m.getData<UINT16>();
	case TypeINT32:     return getData<INT32>() == m.getData<INT32>();
	case TypeUINT32:    return getData<UINT32>() == m.getData<UINT32>();
	case TypeINT64:     return getData<INT64>() == m.getData<INT64>();
	case TypeUINT64:    return getData<UINT64>() == m.getData<UINT64>();
	case TypeBool:      return getData<bool>() == m.getData<bool>();
	case TypeStdString: return getData<std::string>() == m.getData<std::string>();
	default:
		throw boost::bad_any_cast();
	}
}

//======================================================================

Measurement::MeasurementType Measurement::typeIdToType(const std::type_info& tinfo)
{
	if (tinfo == typeid(void)) return TypeVoid;
	else if (tinfo == typeid(float)) return TypeFloat;
	else if (tinfo == typeid(double)) return TypeDouble;
	else if (tinfo == typeid(INT8)) return TypeINT8;
	else if (tinfo == typeid(UINT8)) return TypeUINT8;
	else if (tinfo == typeid(INT16)) return TypeINT16;
	else if (tinfo == typeid(UINT16)) return TypeUINT16;
	else if (tinfo == typeid(INT32)) return TypeINT32;
	else if (tinfo == typeid(UINT32)) return TypeUINT32;
	else if (tinfo == typeid(INT64)) return TypeINT64;
	else if (tinfo == typeid(UINT64)) return TypeUINT64;
	else if (tinfo == typeid(bool)) return TypeBool;
	else if (tinfo == typeid(std::string)) return TypeStdString;
	else {
		logError << "Unknown type_info in Measurement: " << tinfo.name() << std::endl;
		throw std::runtime_error("Unknown type_info in Measurement");
		assert(false);
	}
	return TypeFloat; // To avoid compiler warnings
}

//======================================================================

bool operator==(const Measurement& lhs, const Measurement& rhs)
{
	return lhs.isEqual(rhs);
}

//======================================================================

bool operator!=(const Measurement& lhs, const Measurement& rhs)
{
	return !lhs.isEqual(rhs);
}

//======================================================================

std::ostream& operator<<(std::ostream& oss, const Measurement& m)
{
	oss << "Key=" << m.getKey() << " type=" << m.getMeasurementType() << " ";

	switch(m.getMeasurementType()) {
	case Measurement::TypeFloat:     oss << "TypeFloat(" << m.getData<float>() << ")";           break;
	case Measurement::TypeDouble:    oss << "TypeDouble(" << m.getData<double>() << ")";         break;
	case Measurement::TypeINT8:      oss << "TypeINT8(" << int(m.getData<INT8>()) << ")";        break;
	case Measurement::TypeUINT8:     oss << "TypeUINT8(" << int(m.getData<UINT8>()) << ")";      break;
	case Measurement::TypeINT16:     oss << "TypeINT16(" << m.getData<INT16>() << ")";           break;
	case Measurement::TypeUINT16:    oss << "TypeUINT16(" << m.getData<UINT16>() << ")";         break;
	case Measurement::TypeINT32:     oss << "TypeINT32(" << m.getData<INT32>() << ")";           break;
	case Measurement::TypeUINT32:    oss << "TypeUINT32(" << m.getData<UINT32>() << ")";         break;
	case Measurement::TypeINT64:     oss << "TypeINT64(" << m.getData<INT64>() << ")";           break;
	case Measurement::TypeUINT64:    oss << "TypeUINT64(" << m.getData<UINT64>() << ")";         break;
	case Measurement::TypeBool:      oss << "TypeBool(" << m.getData<bool>() << ")";             break;
	case Measurement::TypeStdString: oss << "TypeStdString(" << m.getData<std::string>() << ")"; break;
	default:
		oss << "Undefined Type(..) ";
		break;
	}

	return oss;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
