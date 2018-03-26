//======================================================================
/*! \file MeasurementListTestSupport.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 28, 2016
 *///-------------------------------------------------------------------

#ifndef MEASUREMENTLISTTESTSUPPORT_HPP_SEEN
#define MEASUREMENTLISTTESTSUPPORT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include "TestSupport.hpp"

#include <ibeosdk/datablocks/snippets/MeasurementList.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

class MeasurementListTestSupport : public TestSupport {
public:
	MeasurementListTestSupport();
	virtual ~MeasurementListTestSupport();

	static
	ibeosdk::MeasurementList createMeasurementList();

	static
	ibeosdk::Measurement createMeasurement();

protected:
	static
	void fillRandomly(ibeosdk::MeasurementList& ml);

	static
	void fillRandomly(ibeosdk::Measurement& m,
	                  const uint16_t keyRangeStart = 0x0000,
	                  const uint16_t keyRangeend= 0xFFFF);

	template<Measurement::MeasurementType MT>
	static
	void fillDataRandomly(ibeosdk::Measurement& m);


	static void fill64kString(std::string& s);

protected:
	static const int32_t nbOfRepeats = 100;
}; // MeasurementListTestSupport

template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeVoid>(Measurement& m);
template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeFloat>(Measurement& m);
template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeDouble>(Measurement& m);
template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeINT8>(Measurement& m);
template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeUINT8>(Measurement& m);
template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeINT16>(Measurement& m);
template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeUINT16>(Measurement& m);
template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeINT32>(Measurement& m);
template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeUINT32>(Measurement& m);
template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeINT64>(Measurement& m);
template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeUINT64>(Measurement& m);
template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeBool>(Measurement& m);
template<>
void MeasurementListTestSupport::fillDataRandomly<Measurement::TypeStdString>(Measurement& m);

//======================================================================

template<> Measurement::MeasurementType TestSupport::getRandValue();

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//================================================= =====================

#endif // MEASUREMENTLISTTESTSUPPORT_HPP_SEEN

//======================================================================
