//======================================================================
/*! \file CompressedPoint2d.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 13, 2016
 *///-------------------------------------------------------------------

#ifndef COMPRESSEDPOINT2D_HPP_SEEN
#define COMPRESSEDPOINT2D_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/Point2d.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class CompressedPoint2d : private Point2d {
public:
	static const int16_t compressedPlusInfinity;
	static const int16_t compressedMinusInfinity;
	static const int16_t compressedNotANumber;

	static const float toCentimeter;
	static const float fromCentimeter;
	static const float toDecimeter;
	static const float fromDecimeter;

public:
	static std::streamsize getSerializedSize_static() { return Point2d::getSerializedSize_static(); }
	static float decompressMeters(const int16_t compressedMeters);
	static float decompressRadians(const int16_t compressedRadians);

	static int16_t compressMeters(const float meters);
	static int16_t compressRadians(const float radians);

public:
	CompressedPoint2d();
	CompressedPoint2d(const int16_t compressedPosX, const int16_t compressedPosY);

	virtual ~CompressedPoint2d();

public:
	CompressedPoint2d& operator=(const CompressedPoint2d& src);

public:
	using Point2d::getSerializedSize;

public:
	int16_t getCompressedX() const { return getX(); }
	int16_t getCompressedY() const { return getY(); }

	float getXinMeter() const { return decompressMeters(getX()); }
	float getYinMeter() const { return decompressMeters(getY()); }

public:
	void setCompressedX(const int16_t compressedPosX) { setX(compressedPosX); }
	void setCompressedY(const int16_t compressedPosY) { setY(compressedPosY); }

	void setX(const float posXMeter) { setX(compressMeters(posXMeter)); }
	void setY(const float posYMeter) { setY(compressMeters(posYMeter)); }

public:
	template<typename T> friend void readLE(std::istream& is, T& value);
	template<typename T> friend void readBE(std::istream& is, T& value);
}; // CompressedPoint2d

//======================================================================

std::ostream& operator<<(std::ostream& os, const Point2d& tObj);
bool operator==(const CompressedPoint2d& lhs, const CompressedPoint2d& rhs);
bool operator!=(const CompressedPoint2d& lhs, const CompressedPoint2d& rhs);

//======================================================================
//======================================================================
//======================================================================

template<>
inline
void writeLE<CompressedPoint2d>(std::ostream& os, const CompressedPoint2d& p)
{
	ibeosdk::writeLE(os, p.getCompressedX());
	ibeosdk::writeLE(os, p.getCompressedY());
}

//======================================================================

template<>
inline
void readLE<CompressedPoint2d>(std::istream& is, CompressedPoint2d& p)
{
	ibeosdk::readLE(is, p.m_posX);
	ibeosdk::readLE(is, p.m_posY);
}

//======================================================================
//======================================================================
//======================================================================

template<>
inline
void writeBE<CompressedPoint2d>(std::ostream& os, const CompressedPoint2d& p)
{
	ibeosdk::writeBE(os, p.getCompressedX());
	ibeosdk::writeBE(os, p.getCompressedY());
}

//======================================================================

template<>
inline
void readBE<CompressedPoint2d>(std::istream& is, CompressedPoint2d& p)
{
	ibeosdk::readBE(is, p.m_posX);
	ibeosdk::readBE(is, p.m_posY);
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // COMPRESSEDPOINT2D_HPP_SEEN

//======================================================================
