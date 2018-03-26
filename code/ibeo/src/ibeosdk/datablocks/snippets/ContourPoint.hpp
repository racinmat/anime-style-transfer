//======================================================================
/*! \file ContourPoint.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 23, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_CONTOURPOINT_HPP_SEEN
#define IBEOSDK_CONTOURPOINT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/Point2d.hpp>
#include <ibeosdk/inttypes.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ContourPoint : public ibeosdk::Snippet {
public:
	ContourPoint();
	ContourPoint(const Point2d pt);
	virtual ~ContourPoint();

public:
	int16_t getX() const { return m_posX; }
	int16_t getY() const { return m_posY; }
	uint8_t getXSigma() const { return m_posXSigma; }
	uint8_t getYSigma() const { return m_posYSigma; }
	int8_t getCorrCoeff() const { return m_corrCoeff; }
	uint8_t getExistenceProbability() const { return m_existenceProbability; }

public:
	void setX(const int16_t newX) { m_posX = newX; }
	void setY(const int16_t newY) { m_posY = newY; }
	void setXSigma(const uint8_t newXSigma) { m_posXSigma = newXSigma; }
	void setYSigma(const uint8_t newYSigma) { m_posYSigma = newYSigma; }
	void setCorrCoeff(const int8_t newCorrCoeff) { m_corrCoeff = newCorrCoeff; }
	void setExistenceProbability(const uint8_t newExistenceProbability) { m_existenceProbability = newExistenceProbability; }

public:
	static std::streamsize getSerializedSize_static() { return 8; }

public:
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

protected:
	int16_t m_posX;
	int16_t m_posY;
	uint8_t m_posXSigma;
	uint8_t m_posYSigma;
	int8_t m_corrCoeff;
	uint8_t m_existenceProbability;
}; // ContourPoint

//======================================================================
//======================================================================
//======================================================================

bool operator==(const ContourPoint& lhs, const ContourPoint& rhs);
bool operator!=(const ContourPoint& lhs, const ContourPoint& rhs);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_CONTOURPOINT_HPP_SEEN

//======================================================================
