//======================================================================
/*! \file MeasurementListTest.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 28, 2016
 *///-------------------------------------------------------------------


#include "common/MeasurementListTestSupport.hpp"

// we link the boost_unit_test_framework library dynamically
#ifndef _WIN32
#	define BOOST_TEST_DYN_LINK
#endif // !_WIN32
#define BOOST_TEST_MODULE MeasurementListTest
#include <boost/test/unit_test.hpp>

//======================================================================

using namespace ibeosdk;

//======================================================================

BOOST_AUTO_TEST_SUITE( MeasurementListTestSuite )

//======================================================================

class Fixture : public unittests::MeasurementListTestSupport {}; // Fixture

//======================================================================

BOOST_FIXTURE_TEST_CASE(createMeasurement, Fixture)
{
	ibeosdk::Measurement m;
	BOOST_CHECK(true);
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(createMeasurementList, Fixture)
{
	ibeosdk::MeasurementList ml;
	BOOST_CHECK(true);
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isIoMeasurementIdentity, Fixture)
{
	for (int32_t r = 0; r < nbOfRepeats; ++r) {
		Measurement mOrig(createMeasurement());

		std::stringstream ss;
		BOOST_CHECK(mOrig.serialize(ss));

		Measurement mCopy;
		BOOST_CHECK(mCopy.deserialize(ss));

		BOOST_CHECK(mOrig == mCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isIoMeasurementListIdentity, Fixture)
{
	for (int32_t r = 0; r < nbOfRepeats; ++r) {
		MeasurementList mlOrig(createMeasurementList());

		std::stringstream ss;
		BOOST_CHECK(mlOrig.serialize(ss));

		MeasurementList mlCopy;
		BOOST_CHECK(mlCopy.deserialize(ss));

		BOOST_CHECK(mlOrig == mlCopy);
	} // for r
}

//======================================================================

BOOST_AUTO_TEST_SUITE_END()

//======================================================================
