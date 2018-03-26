//======================================================================
/*! \file MeasurementListTestSupport.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 28, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <unittests/common/MeasurementListTestSupport.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

MeasurementListTestSupport::MeasurementListTestSupport()
  : TestSupport()
{}

//======================================================================

MeasurementListTestSupport::~MeasurementListTestSupport() {}

//======================================================================

ibeosdk::MeasurementList MeasurementListTestSupport::createMeasurementList()
{
	ibeosdk::MeasurementList ml;
	fillRandomly(ml);
	return ml;
}

//======================================================================

ibeosdk::Measurement MeasurementListTestSupport::createMeasurement()
{
	ibeosdk::Measurement m;
	fillRandomly(m);
	return m;
}

//======================================================================

void MeasurementListTestSupport::fillRandomly(ibeosdk::MeasurementList& ml)
{
	const uint8_t nbOfMeasurements = getRandValue<uint8_t>();
	for (uint16_t mIdx = 0; mIdx < nbOfMeasurements; ++mIdx) {
		Measurement m;
		fillRandomly(m, 0x0000, 0xFFFF);
		ml.addMeasurement(m);
	}
}
//======================================================================

void MeasurementListTestSupport::fillRandomly(ibeosdk::Measurement& m,
                                              const uint16_t keyRangeStart,
                                              const uint16_t keyRangeEnd)
{
	m.setKey(MeasurementKey(getRandValue<uint16_t>(keyRangeStart, keyRangeEnd)));

	switch(getRandValue<Measurement::MeasurementType>()) {
	case Measurement::TypeVoid:      fillDataRandomly<Measurement::TypeVoid>(m); return;
	case Measurement::TypeFloat:     fillDataRandomly<Measurement::TypeFloat>(m); return;
	case Measurement::TypeDouble:    fillDataRandomly<Measurement::TypeDouble>(m); return;
	case Measurement::TypeINT8:      fillDataRandomly<Measurement::TypeINT8>(m); return;
	case Measurement::TypeUINT8:     fillDataRandomly<Measurement::TypeUINT8>(m); return;
	case Measurement::TypeINT16:     fillDataRandomly<Measurement::TypeINT16>(m); return;
	case Measurement::TypeUINT16:    fillDataRandomly<Measurement::TypeUINT16>(m); return;
	case Measurement::TypeINT32:     fillDataRandomly<Measurement::TypeINT32>(m); return;
	case Measurement::TypeUINT32:    fillDataRandomly<Measurement::TypeUINT32>(m); return;
	case Measurement::TypeINT64:     fillDataRandomly<Measurement::TypeINT64>(m); return;
	case Measurement::TypeUINT64:    fillDataRandomly<Measurement::TypeUINT64>(m); return;
	case Measurement::TypeBool:      fillDataRandomly<Measurement::TypeBool>(m); return;
	case Measurement::TypeStdString: fillDataRandomly<Measurement::TypeStdString>(m); return;
	}
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeVoid>(Measurement& m)
{
	m.resetValue();
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeFloat>(Measurement& m)
{
	const float v = getRandValue<float>();
	m.setData(v);
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeDouble>(Measurement& m)
{
	const double v = getRandValue<double>();
	m.setData(v);
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeINT8>(Measurement& m)
{
	const int8_t v = int8_t(getRandValue<uint8_t>());
	m.setData(v);
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeUINT8>(Measurement& m)
{
	const uint8_t v = getRandValue<uint8_t>();
	m.setData(v);
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeINT16>(Measurement& m)
{
	const int16_t v = int16_t(getRandValue<uint16_t>());
	m.setData(v);
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeUINT16>(Measurement& m)
{
	const uint16_t v = getRandValue<uint16_t>();
	m.setData(v);
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeINT32>(Measurement& m)
{
	const int32_t v = int32_t(getRandValue<uint32_t>());
	m.setData(v);
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeUINT32>(Measurement& m)
{
	const uint32_t v = getRandValue<uint32_t>();
	m.setData(v);
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeINT64>(Measurement& m)
{
	const int64_t v = int64_t(getRandValue<uint64_t>());
	m.setData(v);
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeUINT64>(Measurement& m)
{
	const uint64_t v = getRandValue<uint64_t>();
	m.setData(v);
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeBool>(Measurement& m)
{
	const bool v = getRandValue<bool>();
	m.setData(v);
}

//======================================================================

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeStdString>(Measurement& m)
{
	std::string s;
	fill64kString(s);
	m.setData(s);
}

//======================================================================

void MeasurementListTestSupport::fill64kString(std::string& s)
{
	const uint16_t n = getRandValue<uint16_t>();
	s.resize(n);

	std::string::iterator it = s.begin();
	for (; it != s.end(); ++it) {
		*it = int8_t(65 + getRandValue<uint8_t>(25));
	}
}

//======================================================================

template<> Measurement::MeasurementType TestSupport::getRandValue()
{
	return Measurement::MeasurementType(getRandValue<uint8_t>(Measurement::TypeStdString-Measurement::TypeVoid) + Measurement::TypeVoid);
}

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================
