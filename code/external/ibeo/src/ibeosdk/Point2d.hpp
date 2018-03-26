//======================================================================
/*! \file Point2d.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_POINT2D_HPP_SEEN
#define IBEOSDK_POINT2D_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class Point2d {
public:
	static std::streamsize getSerializedSize_static() { return 2 * std::streamsize(sizeof(INT16)); }

public:
	Point2d();
	Point2d(const INT16 posX, const INT16 posY);
	Point2d(const Point2d& src);
	virtual ~Point2d();

public:
	Point2d& operator=(const Point2d& src);

public:
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }

public:
	INT16 getX() const { return m_posX; }
	INT16 getY() const { return m_posY; }

public:
	void setX(const INT16 posX) { m_posX = posX; }
	void setY(const INT16 posY) { m_posY = posY; }

protected:
	INT16 m_posX;
	INT16 m_posY;

public:
	template<typename T> friend void readLE(std::istream& is, T& value);
	template<typename T> friend void readBE(std::istream& is, T& value);
}; // Point2D

//======================================================================

std::ostream& operator<<(std::ostream& os, const Point2d& tObj);
bool operator==(const Point2d& lhs, const Point2d& rhs);
bool operator!=(const Point2d& lhs, const Point2d& rhs);

//======================================================================
//======================================================================
//======================================================================

template<>
inline
void writeLE<Point2d>(std::ostream& os, const Point2d& p)
{
	ibeosdk::writeLE(os, p.getX());
	ibeosdk::writeLE(os, p.getY());
}

//======================================================================

template<>
inline
void readLE<Point2d>(std::istream& is, Point2d& p)
{
	ibeosdk::readLE(is, p.m_posX);
	ibeosdk::readLE(is, p.m_posY);
}

//======================================================================
//======================================================================
//======================================================================

template<>
inline
void writeBE<Point2d>(std::ostream& os, const Point2d& p)
{
	ibeosdk::writeBE(os, p.getX());
	ibeosdk::writeBE(os, p.getY());
}

//======================================================================

template<>
inline
void readBE<Point2d>(std::istream& is, Point2d& p)
{
	ibeosdk::readBE(is, p.m_posX);
	ibeosdk::readBE(is, p.m_posY);
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_POINT2D_HPP_SEEN

//======================================================================

