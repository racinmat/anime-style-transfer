//======================================================================
/*! \file Math.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 29, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_MATH_HPP_SEEN
#define IBEOSDK_MATH_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/misc/deprecatedwarning.hpp>
#include <ibeosdk/inttypes.hpp>

#include <limits>
#include <cassert>
#include <cmath>
#if _MSC_VER == 1600
#ifdef _M_X64
#include <math.h>
#endif // _M_X64
#endif //_MSC_VER == 1600

//======================================================================

#if defined _WIN32 &&  _MSC_VER < 1900
/*!\brief rename VC _isnan function as isnan for compatibility
 *///-------------------------------------------------------------------
#	if _MSC_VER == 1800
#		error "Not tested with VS 2013"
#	endif // _MSC_VER == 1800
namespace std {
inline bool isnan(const double d)
{
	return 0 != _isnan(d);
}
} // namespace std

#endif // _WIN32 &&  _MSC_VER < 1900

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\brief Shortcut for the double NaN value.
 *///-------------------------------------------------------------------
const double NaN_double = std::numeric_limits<double>::quiet_NaN();

const double twoPi = 2.0 * M_PI;
const double rad2deg = 180.0 / M_PI;
const double deg2rad = M_PI / 180.0;

//======================================================================
/*!\brief Shortcut for the float NaN value.
 *///-------------------------------------------------------------------
const float NaN = std::numeric_limits<float>::quiet_NaN();

//======================================================================

const float twoPif = float(2.0 * M_PI);
const float rad2degf = float(180.0 / M_PI);
const float deg2radf = float(M_PI / 180.0);

//======================================================================
/*!\brief Class to hold the powers for float fuzzy compare template functions.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Apr 26, 2016
 *
 *///-------------------------------------------------------------------
class NegFloatPowersOf10 {
public:
	static const int32_t nbOfPowers = 8;

public:
	static float getPower(const uint8_t exp)
	{
		assert(exp < nbOfPowers);
		return powers[exp];
	}

private:
	static const float powers[nbOfPowers];
}; // NegFloatPotenciesOf10

//======================================================================
/*!\brief Class to hold the powers for double fuzzy compare template functions.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Apr 26, 2016
 *
 *///-------------------------------------------------------------------
class NegDoublePowersOf10 {
public:
	static const int32_t nbOfPowers = 18;

public:
	static double getPower(const uint8_t exp)
	{
		assert(exp < nbOfPowers);
		return powers[exp];
	}

private:
	static const double powers[nbOfPowers];
}; // NegFloatPotenciesOf10

//======================================================================
/*!\brief Tests whether two \c float values are nearly equal.
 * \tparam EXP   The exponent of the epsilon used for the fuzzy
 *               compare. 10^(-EXP).
 * \param[in] a  First value to be compared with second value.
 * \param[in] b  Second value to be compared with first value.
 * \return \c true if the two \c float numbers are equal in
 *         terms of the machine precision, which means their
 *         difference must be less than 10^(-EXP).
 *
 * The exponent range is defined in NegFloatPotenciesOf10.
 *///-------------------------------------------------------------------
template<uint8_t EXP>
inline bool fuzzyCompareT(const float a, const float b)
{
	return std::abs(a - b) < NegFloatPowersOf10::getPower(EXP);
}

//======================================================================

IBEOSDK_DEPRECATED inline bool fuzzyCompare(const float a, const float b)
{
	return fuzzyCompareT<6>(a,b);
}

//======================================================================
/*!\brief Tests whether two \c double values are nearly equal.
 * \tparam EXP   The exponent of the epsilon used for the fuzzy
 *               compare. 10^(-EXP).
 * \param[in] a  First value to be compared with second value.
 * \param[in] b  Second value to be compared with first value.
 * \return \c true if the two \c double numbers are equal in
 *         terms of the machine precision, which means their
 *         difference must be less than 10^(-EXP).
 *
 * The exponent range is defined in NegDoublePotenciesOf10.
 *///-------------------------------------------------------------------
template<uint8_t EXP>
inline bool fuzzyCompareT(const double a, const double b)
{
	return std::abs(a - b) < NegDoublePowersOf10::getPower(EXP);
}

//======================================================================
/*!\brief Tests whether two \c float values are nearly equal.
 * \param[in] a  First value to be compared with second value.
 * \param[in] b  Second value to be compared with first value.
 * \return \c true if the two \c float numbers are equal in
 *         terms of the machine precision, which means their
 *         difference must be less than 1E-6.
 *///-------------------------------------------------------------------
IBEOSDK_DEPRECATED inline bool fuzzyCompare(const double a, const double b)
{
	return std::abs(a - b) < 1E-6;
}

//======================================================================
/*!\brief Compare two floats \a a and \a b. NaN is equal NaN here.
 * \param[in] a  First float to be compared.
 * \param[in] b  Second float to be compared.
 * \return \c true if \a a == \a b or if both are NaN.
 *         \c false otherwise.
 *///-------------------------------------------------------------------
inline bool floatEqual(const float a, const float b)
{
	return ((std::isnan(a) && std::isnan(b)) || a == b);
}

//======================================================================
/*!\brief Compare two doubles \a a and \a b. NaN is equal NaN here.
 * \param[in] a  First double to be compared.
 * \param[in] b  Second double to be compared.
 * \return \c true if \a a == \a b or if both are NaN.
 *         \c false otherwise.
 *///-------------------------------------------------------------------
inline bool doubleEqual(const double a, const double b)
{
	return ((std::isnan(a) && std::isnan(b)) || a == b);
}

//======================================================================
/*!\brief Fuzzy Compare two floats \a a and \a b. NaN is equal NaN here.
 * \tparam EXP   The exponent of the epsilon used for the fuzzy
 *               compare. 10^(-EXP).
 * \param[in] a  First float to be compared.
 * \param[in] b  Second float to be compared.
 * \return \c true if the difference between \a a and \a b is not smaller
 *         than 10^(-EXP) or if both are NaN.
 *         \c false otherwise.
 *///-------------------------------------------------------------------
template<uint8_t EXP>
inline bool fuzzyFloatEqualT(const float a, const float b)
{
	return ((std::isnan(a) && std::isnan(b)) || fuzzyCompareT<EXP>(a, b));
}

//======================================================================
/*!\brief Fuzzy Compare two doubles \a a and \a b. NaN is equal NaN here.
 * \tparam EXP   The exponent of the epsilon used for the fuzzy
 *               compare. 10^(-EXP).
 * \param[in] a  First double to be compared.
 * \param[in] b  Second double to be compared.
 * \return \c true if the difference between \a a and \a b is not smaller
 *         than 10^(-EXP) or if both are NaN.
 *         \c false otherwise.
 *///-------------------------------------------------------------------
template<uint8_t EXP>
inline bool fuzzyDoubleEqualT(const double a, const double b)
{
	return ((std::isnan(a) && std::isnan(b)) || fuzzyCompareT<EXP>(a, b));
}

//======================================================================
/*!\brief Use fuzzyFloatEqualT instead.
 * \deprecated Use fuzzyFloatEqualT instead.
 *///-------------------------------------------------------------------
IBEOSDK_DEPRECATED inline bool fuzzyFloatEqual(const float a, const float b)
{
	return fuzzyFloatEqualT<6>(a, b);
}

//======================================================================

inline bool floatInequal(const float a, const float b)
{
	return !floatEqual(a,b);
}

//======================================================================

template<uint8_t EXP>
inline bool fuzzyFloatInequalT(const float a, const float b)
{
	return ((std::isnan(a) && std::isnan(b)) || !fuzzyCompareT<EXP>(a, b));
}

//======================================================================

template<uint8_t EXP>
inline bool fuzzyDoubleInequalT(const double a, const double b)
{
	return ((std::isnan(a) && std::isnan(b)) || !fuzzyCompareT<EXP>(a, b));
}

//======================================================================

//======================================================================
/*!\brief Use fuzzyFloatInequalT instead.
 * \deprecated Use fuzzyFloatInequalT instead.
 *///-------------------------------------------------------------------
IBEOSDK_DEPRECATED inline bool fuzzyFloatInequal(const float a, const float b)
{
	return fuzzyFloatInequalT<6>(a, b);
}

//======================================================================
/*!\brief Normalize the given angle.
 *
 * Normalizes an angle given in radians by adding or subtracting an integer
 * multiple of 2*pi so that the resulting angle is in the half-open interval
 * (-pi,+pi]. The current implementation takes O(1) time, i.e. the time of
 * execution has a fixed upper boundary independend from the angle.
 *
 * \param[in] angleInRad  Angle to be normalized given in rad.
 * \return The normalized angle in (-pi, +pi].
 * \todo check whether (-pi, +pi] or [-pi, +pi) is correct.
 *///-------------------------------------------------------------------
inline float normalizeRadians (float angleInRad)
{
	if (std::abs(angleInRad) > (3.0f * M_PI)) {
		// For numerical stability we must use this sin/cos/atan2
		// implementation even though it might consume more cycles.
		// Note that radians = -pi converts to atan2(0,-1) = +pi!
		angleInRad = std::atan2 (std::sin(angleInRad), std::cos(angleInRad));
		// radians now in (-pi,+pi]
	} // if
	else {
		// fast version for "normal" out of range values
		while (angleInRad > M_PI) {
			angleInRad -= twoPif;
		} // while
		while (angleInRad <= float(-M_PI)) {
			angleInRad += twoPif;
		} // while
	} // else
	return angleInRad;
}

//======================================================================
/*!\brief Normalize the given angle.
 *
 * Normalizes an angle given in radians by adding or subtracting an integer
 * multiple of 2*pi so that the resulting angle is in the half-open interval
 * (-pi,+pi]. The current implementation takes O(1) time, i.e. the time of
 * execution has a fixed upper boundary independend from the angle.
 *
 * \param[in] angleInRad  Angle to be normalized given in rad.
 * \return The normalized angle in (-pi, +pi].
 * \todo check whether (-pi, +pi] or [-pi, +pi) is correct.
 *///-------------------------------------------------------------------
inline double normalizeRadians (double angleInRad)
{
	if (std::abs(angleInRad) > (3.0f * M_PI)) {
		// For numerical stability we must use this sin/cos/atan2
		// implementation even though it might consume more cycles.
		// Note that radians = -pi converts to atan2(0,-1) = +pi!
		angleInRad = std::atan2 (std::sin(angleInRad), std::cos(angleInRad));
		// radians now in (-pi,+pi]
	} // if
	else {
		// fast version for "normal" out of range values
		while (angleInRad > M_PI) {
			angleInRad -= twoPi;
		} // while
		while (angleInRad <= -M_PI) {
			angleInRad += twoPi;
		} // while
	} // else
	return angleInRad;
}

//======================================================================
/*!\brief Round to the closest integer
 *
 * \param[in] floatValue The float value that shall be rounded
 * \return \a floatValue rounded to the closest integer
 *///-------------------------------------------------------------------
template<typename IntT>
inline IntT round (float floatValue)
{
	return IntT (floatValue + (floatValue >= 0.0f ? + 0.5f : -0.5f));
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_MATH_HPP_SEEN
