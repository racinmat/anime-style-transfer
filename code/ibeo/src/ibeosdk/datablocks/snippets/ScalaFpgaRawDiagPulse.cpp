//======================================================================
/*! \file ScalaFpgaRawDiagPulse.cpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ScalaFpgaRawDiagPulse.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

const uint16_t ScalaFpgaRawDiagPulse::blockId = 0xF050U;

//======================================================================

ScalaFpgaRawDiagPulse::ScalaFpgaRawDiagPulse()
  : ScalaFpgaRawScanPointBase()
{}

//======================================================================

ScalaFpgaRawDiagPulse::~ScalaFpgaRawDiagPulse() {}

//======================================================================

bool ScalaFpgaRawDiagPulse::operator==(const ScalaFpgaRawDiagPulse& other) const
{
	return (static_cast<const ScalaFpgaRawScanPointBase&>(*this) == static_cast<const ScalaFpgaRawScanPointBase&>(other));
}

//======================================================================

bool ScalaFpgaRawDiagPulse::operator!=(const ScalaFpgaRawDiagPulse& other) const
{
	return (static_cast<const ScalaFpgaRawScanPointBase&>(*this) != static_cast<const ScalaFpgaRawScanPointBase&>(other));
}

//======================================================================

} // namespace ibeosdk

//======================================================================
