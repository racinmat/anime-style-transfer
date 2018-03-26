//======================================================================
/*! \file Point2d.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/Point2d.hpp>
#include <ibeosdk/io.hpp>

#include <iostream>

//======================================================================

namespace ibeosdk {

//======================================================================

Point2d::Point2d()
  : m_posX(0), m_posY(0)
{}

//======================================================================

Point2d::Point2d(const INT16 posX, const INT16 posY)
  : m_posX(posX), m_posY(posY)
{}

//======================================================================

Point2d::Point2d(const Point2d& other)
  : m_posX(other.m_posX), m_posY(other.m_posY)
{}

//======================================================================

Point2d::~Point2d() {}

//======================================================================

Point2d& Point2d::operator= (const Point2d& src)
{
	m_posX = src.m_posX;
	m_posY = src.m_posY;

	return *this;
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const Point2d& pt)
{
	os << "(" << pt.getX() << ", " << pt.getY() << ")";
	return os;
}

//======================================================================

bool operator==(const Point2d& lhs, const Point2d& rhs)
{
	return (lhs.getX() == rhs.getX())
			&& (lhs.getY() == rhs.getY());
}

//======================================================================

bool operator!=(const Point2d& lhs, const Point2d& rhs)
{
	return (lhs.getX() != rhs.getX())
			|| (lhs.getY() != rhs.getY());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
