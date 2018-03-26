//======================================================================
/*! \file TestSupport.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 29, 2015
 *///-------------------------------------------------------------------

#ifndef TESTSUPPORT_HPP_SEEN
#define TESTSUPPORT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/Point2d.hpp>
#include <ibeosdk/PointSigma2d.hpp>
#include <ibeosdk/Time.hpp>
#include <ibeosdk/ObjectBasic.hpp>
#include <ibeosdk/datablocks/snippets/Box2d.hpp>
#include <ibeosdk/datablocks/snippets/Box2dF.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

class TestSupport {
public:
	virtual ~TestSupport();

public:
	template<typename T>
	static
	T getRandValue();

	template<typename T>
	static
	T getRandValue(const T rangeMax);

	template<typename T>
	static
	T getRandValue(const T rangeMin, const T rangeMax);

	static double getNonNanDblRandValue();
}; // TestSupport

//======================================================================

template<> bool TestSupport::getRandValue();

template<> char TestSupport::getRandValue();

template<> uint8_t TestSupport::getRandValue();
template<> uint8_t TestSupport::getRandValue(const uint8_t rangeMax);

template<> uint16_t TestSupport::getRandValue();
template<> uint16_t TestSupport::getRandValue(const uint16_t rangeMax);
template<> uint16_t TestSupport::getRandValue(const uint16_t rangeMin, const uint16_t rangeMax);

template<> int16_t TestSupport::getRandValue();

template<> uint32_t TestSupport::getRandValue();
template<> uint32_t TestSupport::getRandValue(const uint32_t rangeMax);

template<> uint64_t TestSupport::getRandValue();


template<> float TestSupport::getRandValue();
template<> double TestSupport::getRandValue();

template<> Point2d TestSupport::getRandValue();
template<> PointSigma2d TestSupport::getRandValue();
template<> CompressedPoint2d TestSupport::getRandValue();
template<> NTPTime TestSupport::getRandValue();

template<> RefPointBoxLocation TestSupport::getRandValue();
template<> ObjectClass TestSupport::getRandValue();
template<> luxObjectClass::LuxObjectClass TestSupport::getRandValue();
template<> rawObjectClass::RawObjectClass TestSupport::getRandValue();

template<> Point2dFloat TestSupport::getRandValue();
template<> Box2d TestSupport::getRandValue();
template<> Box2dF TestSupport::getRandValue();

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================

#endif // TESTSUPPORT_HPP_SEEN

//======================================================================
