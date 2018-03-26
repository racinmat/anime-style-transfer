//======================================================================
/*! \file CompressedPoint2d.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 13, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/CompressedPoint2d.hpp>
#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

const int16_t CompressedPoint2d::compressedPlusInfinity  = + 32767;
const int16_t CompressedPoint2d::compressedMinusInfinity = -32767;
const int16_t CompressedPoint2d::compressedNotANumber    = -32768;

const float CompressedPoint2d::toCentimeter = 0.01F;
const float CompressedPoint2d::fromCentimeter = 100.F;
const float CompressedPoint2d::toDecimeter = 0.1F;
const float CompressedPoint2d::fromDecimeter = 10.0F;

//======================================================================

float CompressedPoint2d::decompressMeters(const int16_t compressedMeters)
{
	if (compressedMeters == compressedPlusInfinity)
		return std::numeric_limits<float>::infinity();
	else if (compressedMeters > 10000)
		return float((compressedMeters - 9000)) * toDecimeter;
	else if (compressedMeters > -10000)
		return compressedMeters * toCentimeter;
	else if (compressedMeters > compressedMinusInfinity)
		return float((compressedMeters + 9000)) * toDecimeter;
	else if (compressedMeters == compressedMinusInfinity)
		return -std::numeric_limits<float>::infinity();
	else
		return std::numeric_limits<float>::quiet_NaN();
}

//======================================================================

float CompressedPoint2d::decompressRadians(const int16_t compressedRadians)
{
	// Our bounds of the normalization interval, -3.14159 and 3.14159,
	// will result in the rounded value 3.1416 which is outside of the
	// interval bounds. Hence, we manually check for these two special
	// values and return the correct value which is inside the
	// interval bounds in that case.
	if (compressedRadians > 31416)
		throw -1;
	else if (compressedRadians == 31416)
		return float(M_PI);
	else if (compressedRadians == -31416)
		return float(M_PI);
	else if (compressedRadians < -31416)
		throw -1;
	else
		return 1E-4f * compressedRadians;
}

//======================================================================

int16_t CompressedPoint2d::compressMeters(const float meters)
{
	if (meters >= 2376.65f)		// 10 * (2376.65 + 900) + 0.5 = 32767 = ibeo::CompressedPlusInfinity
		return compressedPlusInfinity;
	else if (meters > 100.f)
		return round<INT16>(fromDecimeter * (meters + 900.f)); // round to 10 cm resolution
	else if (meters > -100.f)
		return round<INT16>(fromCentimeter * meters);          // round to  1 cm resolution
	else if (meters > -2376.65f)
		return round<INT16>(fromDecimeter * (meters - 900.f)); // round to 10 cm resolution
	else if (!std::isnan(meters))
		return compressedMinusInfinity;
	else
		return compressedNotANumber;
}

//======================================================================

int16_t CompressedPoint2d::compressRadians(const float radians)
{
	return round<INT16>(1E+4f * normalizeRadians(radians));
}

//======================================================================

CompressedPoint2d::CompressedPoint2d()
  : Point2d()
{}

//======================================================================

CompressedPoint2d::CompressedPoint2d(const int16_t compressedPosX, const int16_t compressedPosY)
  : Point2d(compressedPosX, compressedPosY)
{}

//======================================================================

CompressedPoint2d::~CompressedPoint2d() {}

//======================================================================

CompressedPoint2d& CompressedPoint2d::operator= (const CompressedPoint2d& src)
{
	static_cast<Point2d&>(*this) = src;
	return *this;
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const CompressedPoint2d& pt)
{
	os << "(" << pt.getCompressedX() << ", " << pt.getCompressedY() << ")";
	return os;
}

//======================================================================

bool operator==(const CompressedPoint2d& lhs, const CompressedPoint2d& rhs)
{
	return (lhs.getCompressedX() == rhs.getCompressedX())
	    && (lhs.getCompressedY() == rhs.getCompressedY());
}

//======================================================================

bool operator!=(const CompressedPoint2d& lhs, const CompressedPoint2d& rhs)
{
	return (lhs.getCompressedX() != rhs.getCompressedX())
	    || (lhs.getCompressedY() != rhs.getCompressedY());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
