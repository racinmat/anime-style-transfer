//======================================================================
/*! \file PointSigma2d.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 17, 2014
 *///-------------------------------------------------------------------

#include <ibeosdk/PointSigma2d.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

PointSigma2d::PointSigma2d()
  : m_sigmaX(0), m_sigmaY(0)
{}

//======================================================================

PointSigma2d::PointSigma2d(const UINT16 sigmaX, const UINT16 sigmaY)
  : m_sigmaX(sigmaX), m_sigmaY(sigmaY)
{}

//======================================================================

PointSigma2d::PointSigma2d(const PointSigma2d& other)
  : m_sigmaX(other.m_sigmaX), m_sigmaY(other.m_sigmaY)
{}

//======================================================================

PointSigma2d::~PointSigma2d() {}

//======================================================================

PointSigma2d& PointSigma2d::operator=(const PointSigma2d& other)
{
	m_sigmaX = other.m_sigmaX;
	m_sigmaY = other.m_sigmaY;

	return *this;
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const PointSigma2d& pt)
{
	os << "(" << pt.getX() << ", " << pt.getY() << ")";
	return os;
}

//======================================================================

bool operator==(const PointSigma2d& lhs, const PointSigma2d& rhs)
{
	return (lhs.getX() == rhs.getX())
			&& (lhs.getY() == rhs.getY());
}

//======================================================================

bool operator!=(const PointSigma2d& lhs, const PointSigma2d& rhs)
{
	return (lhs.getX() != rhs.getX())
			|| (lhs.getY() != rhs.getY());
}
//======================================================================

} // namespace ibeosdk

//======================================================================
