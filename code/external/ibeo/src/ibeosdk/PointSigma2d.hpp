//======================================================================
/*! \file PointSigma2d.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 17, 2014
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_POINTSIGMA2D_HPP_SEEN
#define IBEOSDK_POINTSIGMA2D_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/io.hpp>

#include <iostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class PointSigma2d {
public:
	PointSigma2d();
	PointSigma2d(const UINT16 sigmaX, const UINT16 sigmaY);
	PointSigma2d(const PointSigma2d& src);
	virtual ~PointSigma2d();

public:
	PointSigma2d& operator=(const PointSigma2d& other);

public:
	static std::streamsize getSerializedSize() { return 2 * std::streamsize(sizeof(UINT16)); }

public:
	UINT16 getX() const { return m_sigmaX; }
	UINT16 getY() const { return m_sigmaY; }

public:
	void setX(const UINT16 sigmaX) { m_sigmaX = sigmaX; }
	void setY(const UINT16 sigmaY) { m_sigmaY = sigmaY; }

protected:
	UINT16 m_sigmaX;
	UINT16 m_sigmaY;

public:
	template<typename T> friend void readLE(std::istream& is, T& value);
	template<typename T> friend void readBE(std::istream& is, T& value);
}; // PointSigma2d

//======================================================================

std::ostream& operator<<(std::ostream& os, const PointSigma2d& pt);
bool operator==(const PointSigma2d& lhs, const PointSigma2d& rhs);
bool operator!=(const PointSigma2d& lhs, const PointSigma2d& rhs);

//======================================================================
//======================================================================
//======================================================================

template<>
inline
void writeLE<PointSigma2d>(std::ostream& os, const PointSigma2d& p)
{
	ibeosdk::writeLE(os, p.getX());
	ibeosdk::writeLE(os, p.getY());
}

//======================================================================

template<>
inline
void readLE<PointSigma2d>(std::istream& is, PointSigma2d& p)
{
	ibeosdk::readLE(is, p.m_sigmaX);
	ibeosdk::readLE(is, p.m_sigmaY);
}

//======================================================================
//======================================================================
//======================================================================

template<>
inline
void writeBE<PointSigma2d>(std::ostream& os, const PointSigma2d& p)
{
	ibeosdk::writeBE(os, p.getX());
	ibeosdk::writeBE(os, p.getY());
}

//======================================================================

template<>
inline
void readBE<PointSigma2d>(std::istream& is, PointSigma2d& p)
{
	ibeosdk::readBE(is, p.m_sigmaX);
	ibeosdk::readBE(is, p.m_sigmaY);
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_POINTSIGMA2D_HPP_SEEN

//======================================================================

