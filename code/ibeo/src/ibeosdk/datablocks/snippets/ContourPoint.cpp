//======================================================================
/*! \file ContourPoint.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 23, 2014
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/ContourPoint.hpp>
#include <ibeosdk/io.hpp>

#include <iostream>

//======================================================================

namespace ibeosdk {

//======================================================================

ContourPoint::ContourPoint()
  : m_posX(0),
    m_posY(0),
    m_posXSigma(0),
    m_posYSigma(0),
    m_corrCoeff(0),
    m_existenceProbability(0)
{}

//======================================================================

ContourPoint::ContourPoint(const Point2d pt)
: m_posX(pt.getX()),
  m_posY(pt.getY()),
  m_posXSigma(0),
  m_posYSigma(0),
  m_corrCoeff(0),
  m_existenceProbability(0)
{}

//======================================================================

ContourPoint::~ContourPoint() {}

//======================================================================

bool ContourPoint::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	ibeosdk::readBE(is, m_posX);
	ibeosdk::readBE(is, m_posY);
	ibeosdk::readBE(is, m_posXSigma);
	ibeosdk::readBE(is, m_posYSigma);
	ibeosdk::readBE(is, m_corrCoeff);
	ibeosdk::readBE(is, m_existenceProbability);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ContourPoint::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, m_posX);
	ibeosdk::writeBE(os, m_posY);
	ibeosdk::writeBE(os, m_posXSigma);
	ibeosdk::writeBE(os, m_posYSigma);
	ibeosdk::writeBE(os, m_corrCoeff);
	ibeosdk::writeBE(os, m_existenceProbability);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================
//======================================================================
//======================================================================

bool operator==(const ContourPoint& lhs, const ContourPoint& rhs)
{
	if (lhs.getX() != rhs.getX()) return false;
	if (lhs.getY() != rhs.getY()) return false;
	if (lhs.getXSigma() != rhs.getXSigma()) return false;
	if (lhs.getYSigma() != rhs.getYSigma()) return false;
	if (lhs.getCorrCoeff() != rhs.getCorrCoeff()) return false;
	if (lhs.getExistenceProbability() != rhs.getExistenceProbability()) return false;
	return true;
}

//======================================================================

bool operator!=(const ContourPoint& lhs, const ContourPoint& rhs)
{
	if (lhs.getX() != rhs.getX()) return true;
	if (lhs.getY() != rhs.getY()) return true;
	if (lhs.getXSigma() != rhs.getXSigma()) return true;
	if (lhs.getYSigma() != rhs.getYSigma()) return true;
	if (lhs.getCorrCoeff() != rhs.getCorrCoeff()) return true;
	if (lhs.getExistenceProbability() != rhs.getExistenceProbability()) return true;
	return false;
}

//======================================================================

}// namespace ibeosdk

//======================================================================
