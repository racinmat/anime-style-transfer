//======================================================================
/*! \file TimeRecord9000Test.cpp
 *
 * \copydoc Copyright
 * \author Anil Kumar (ank)
 * \date Feb 24, 2017
 *///-------------------------------------------------------------------


#include <unittests/common/TimeRecord9000TestSupport.hpp>

// we link the boost_unit_test_framework library dynamically
#ifndef _WIN32
#	define BOOST_TEST_DYN_LINK
#endif // !_WIN32
#define BOOST_TEST_MODULE TimeRecordTest9000
#include <boost/test/unit_test.hpp>

//======================================================================

using namespace ibeosdk;

//======================================================================

BOOST_AUTO_TEST_SUITE( TimeRecord9000TestSuite )

//======================================================================

class Fixture : public unittests::TimeRecord9000TestSupport {}; // Fixture

//======================================================================

BOOST_FIXTURE_TEST_CASE(createTimeRecord9000, Fixture)
{
	ibeosdk::TimeRecord9000 t;
	BOOST_CHECK(true);
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isIoTimeRecordIdentity, Fixture)
{
	for (int32_t r = 0; r < nbOfRepeats; ++r) {
		TimeRecord9000 trOrig(createTimeRecord9000());

		const IbeoDataHeader dh(TimeRecord9000::getDataBlockId(),
		                        0, uint32_t(trOrig.getSerializedSize()),
		                        0,
		                        NTPTime());
		std::stringstream ss;
		BOOST_CHECK(trOrig.serialize(ss));

		TimeRecord9000 trCopy;
		BOOST_CHECK(trCopy.deserialize(ss,dh));

		BOOST_CHECK(trCopy == trOrig);
	} // for r
}

//======================================================================

BOOST_AUTO_TEST_SUITE_END()

//======================================================================
