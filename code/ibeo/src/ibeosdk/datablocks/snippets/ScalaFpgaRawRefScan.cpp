//======================================================================
/*! \file ScalaFpgaRawRefScan.cpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ScalaFpgaRawRefScan.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

const uint16_t ScalaFpgaRawRefScan::blockId = 0xF060U;

//======================================================================

ScalaFpgaRawRefScan::ScalaFpgaRawRefScan()
  : ScalaFpgaRawScanPointBase()
{}

//======================================================================

ScalaFpgaRawRefScan::~ScalaFpgaRawRefScan() {}

//======================================================================

bool ScalaFpgaRawRefScan::operator==(const ScalaFpgaRawRefScan& other) const
{
	return (static_cast<const ScalaFpgaRawScanPointBase&>(*this) == static_cast<const ScalaFpgaRawScanPointBase&>(other));
}

//======================================================================

bool ScalaFpgaRawRefScan::operator!=(const ScalaFpgaRawRefScan& other) const
{
	return (static_cast<const ScalaFpgaRawScanPointBase&>(*this) != static_cast<const ScalaFpgaRawScanPointBase&>(other));
}

//======================================================================

} // namespace ibeosdk

//======================================================================
