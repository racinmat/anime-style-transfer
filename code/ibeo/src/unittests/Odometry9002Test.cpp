//======================================================================
/*! \file Odometry9002Test.cpp
 *
 * \copydoc Copyright
 * \author Anil Kumar (ank)
 * \date Feb 24, 2017
 *///-------------------------------------------------------------------


#include <unittests/common/Odometry9002TestSupport.hpp>

// we link the boost_unit_test_framework library dynamically
#ifndef _WIN32
#	define BOOST_TEST_DYN_LINK
#endif // !_WIN32
#define BOOST_TEST_MODULE OdometryTest9002
#include <boost/test/unit_test.hpp>

//======================================================================

using namespace ibeosdk;

//======================================================================

BOOST_AUTO_TEST_SUITE( OdometryTestSuite9002 )

//======================================================================

class Fixture : public unittests::Odometry9002TestSupport {}; // Fixture

//======================================================================

BOOST_FIXTURE_TEST_CASE(createOdometry9002, Fixture)
{
	ibeosdk::Odometry9002 o;
	BOOST_CHECK(true);
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isIoOdometryIdentity, Fixture)
{
	for (int32_t r = 0; r < nbOfRepeats; ++r) {
		Odometry9002 odOrig(createOdometry9002());

		const IbeoDataHeader dh(Odometry9002::getDataBlockId(),
		                        0, uint32_t(odOrig.getSerializedSize()),
		                        0,
		                        NTPTime());
		std::stringstream ss;
		BOOST_CHECK(odOrig.serialize(ss));

		Odometry9002 odCopy;
		BOOST_CHECK(odCopy.deserialize(ss,dh));

		BOOST_CHECK(odCopy == odOrig);
	} // for r
}

//======================================================================

BOOST_AUTO_TEST_SUITE_END()

//======================================================================
