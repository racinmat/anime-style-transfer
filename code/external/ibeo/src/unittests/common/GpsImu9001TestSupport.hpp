//======================================================================
/**
 * \file GpsImu9001TestSupport.hpp
 *
 * ---------------------------------------------------------------------
 * \copydoc Copyright
 *
 * \date   07.04.2017
 * \author Peter Oltmann (peo)
 */
//======================================================================

#ifndef GPSIMU9001TESTSUPPORT_HPP_SEEN
#define GPSIMU9001TESTSUPPORT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include "TestSupport.hpp"

#include <ibeosdk/datablocks/GpsImu9001.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

class GpsImu9001TestSupport : public TestSupport
{
public:
	GpsImu9001TestSupport();
	virtual ~GpsImu9001TestSupport();

public:
	static ibeosdk::GpsImu9001 createGpsImu9001();

private:
	static void fillRandomly(ibeosdk::GpsImu9001& gpsImu);

protected:
	static const int32_t nbOfRepeats = 100;
};

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================

#endif // GPSIMU9001TESTSUPPORT_HPP_SEEN

//======================================================================
