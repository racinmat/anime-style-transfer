//======================================================================
/*!
 * \file GpsImu9001Test.cpp
 *
 * ---------------------------------------------------------------------
 * \copydoc Copyright
 *
 * \date   07.04.2017
 * \author Peter Oltmann (peo)
 */
//======================================================================

#include <unittests/common/GpsImu9001TestSupport.hpp>

// we link the boost_unit_test_framework library dynamically
#ifndef _WIN32
#	define BOOST_TEST_DYN_LINK
#endif // !_WIN32
#define BOOST_TEST_MODULE OdometryTest9002
#include <boost/test/unit_test.hpp>

//======================================================================

using namespace ibeosdk;

//======================================================================

BOOST_AUTO_TEST_SUITE( GpsImuTestSuite9001 )

//======================================================================

class Fixture : public unittests::GpsImu9001TestSupport {}; // Fixture

//======================================================================

BOOST_FIXTURE_TEST_CASE(createGpsImu9001, Fixture)
{
	GpsImu9001 gpsImu;
	BOOST_CHECK(true);
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isIoOdometryIdentity, Fixture)
{
	for (int32_t r = 0; r < nbOfRepeats; ++r) {
		GpsImu9001 gpsImuOrig(createGpsImu9001());

		const IbeoDataHeader dh(GpsImu9001::getDataBlockId(),
		                        0, uint32_t(gpsImuOrig.getSerializedSize()),
		                        0,
		                        NTPTime());
		std::stringstream ss;
		BOOST_CHECK(gpsImuOrig.serialize(ss));

		GpsImu9001 gpsImuCopy;
		BOOST_CHECK(gpsImuCopy.deserialize(ss,dh));

		BOOST_CHECK(gpsImuCopy == gpsImuOrig);

		gpsImuOrig = gpsImuCopy;
		BOOST_CHECK(gpsImuCopy == gpsImuOrig);

		gpsImuCopy.setAltitude(ibeosdk::NaN);
		BOOST_CHECK(gpsImuCopy != gpsImuOrig);
	} // for r
}

//======================================================================

BOOST_AUTO_TEST_SUITE_END()

//======================================================================
