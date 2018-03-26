//======================================================================
/*! \file ScalaFpgaRawScanPoint.cpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ScalaFpgaRawScanPoint.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

const uint16_t ScalaFpgaRawScanPoint::blockId = 0xF030U;

//======================================================================

ScalaFpgaRawScanPoint::ScalaFpgaRawScanPoint()
  : ScalaFpgaRawScanPointBase()
{}

//======================================================================

ScalaFpgaRawScanPoint::~ScalaFpgaRawScanPoint() {}

//======================================================================

bool ScalaFpgaRawScanPoint::operator==(const ScalaFpgaRawScanPoint& other) const
{
	return (static_cast<const ScalaFpgaRawScanPointBase&>(*this) == static_cast<const ScalaFpgaRawScanPointBase&>(other));
}

//======================================================================

bool ScalaFpgaRawScanPoint::operator!=(const ScalaFpgaRawScanPoint& other) const
{
	return (static_cast<const ScalaFpgaRawScanPointBase&>(*this) != static_cast<const ScalaFpgaRawScanPointBase&>(other));
}

//======================================================================

} // namespace ibeosdk

//======================================================================
