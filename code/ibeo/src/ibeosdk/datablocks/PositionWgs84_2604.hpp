//======================================================================
/*! \file PositionWgs84_2604.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 24, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_POSITIONWGS84_2604_HPP_SEEN
#define IBEOSDK_POSITIONWGS84_2604_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>

#include <ibeosdk/datablocks/snippets/PositionWgs84.hpp>

#include <ibeosdk/Time.hpp>
#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class PositionWgs84_2604 : public ibeosdk::RegisteredDataBlock<PositionWgs84_2604> {
public:
	PositionWgs84_2604();
	virtual ~PositionWgs84_2604();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	const PositionWgs84& getPosition() const { return m_positionWgs84; }
	PositionWgs84& getPosition() { return m_positionWgs84; }

public:
	std::string toString() const;

protected:
	PositionWgs84 m_positionWgs84;
}; // PositionWGS84

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_POSITIONWGS84_2604_HPP_SEEN

//======================================================================
