//======================================================================
/*! \file MeasurementKey.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date May 3, 2016
 *///-------------------------------------------------------------------


//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/MeasurementKey.hpp>

#include <ibeosdk/datablocks/snippets/Measurement.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

bool MeasurementKey::compare(const Measurement& m, const MeasurementKey key)
{
	return key == m.getKey();
}

//======================================================================

} // namespace ibeosdk

//======================================================================
