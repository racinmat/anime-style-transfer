//======================================================================
/*! \file ObjectLabelList6503Test.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 28, 2016
 *///-------------------------------------------------------------------


#include "common/ObjectLabelList6503TestSupport.hpp"

// we link the boost_unit_test_framework library dynamically
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ObjectLabelList6503Test
#include <boost/test/unit_test.hpp>

//======================================================================

using namespace ibeosdk;

//======================================================================

BOOST_AUTO_TEST_SUITE( ObjectLabelList6503TestSuite )

//======================================================================

class Fixture : public unittests::ObjectLabelList6503TestSupport {}; // Fixture

//======================================================================

BOOST_FIXTURE_TEST_CASE(createObjectLabel6503, Fixture)
{
	ibeosdk::ObjectLabel6503 oa;
	BOOST_CHECK(true);
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(createObjectLabelList6503, Fixture)
{
	ibeosdk::ObjectLabelList6503 oal;
	BOOST_CHECK(true);
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isIoObjLabel6503Identity, Fixture)
{
	for (int32_t r = 0; r < nbOfRepeats; ++r) {
		ObjectLabel6503 oaOrig(createObjectLabel6503());
		std::stringstream ss;
		BOOST_CHECK(oaOrig.serialize(ss));

		ObjectLabel6503 oaCopy;
		BOOST_CHECK(oaCopy.deserialize(ss));

		BOOST_CHECK(oaOrig == oaCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isIoObjLabelList6503Identity, Fixture)
{
	for (int32_t r = 0; r < nbOfRepeats; ++r) {
		ObjectLabelList6503 oalOrig(createObjectLabelList6503());

		const IbeoDataHeader dh(ObjectLabelList6503::getDataBlockId(),
		                        0, uint32_t(oalOrig.getSerializedSize()),
		                        0,
		                        NTPTime());

		std::stringstream ss;
		BOOST_CHECK(oalOrig.serialize(ss));

		ObjectLabelList6503 oalCopy;
		BOOST_CHECK(oalCopy.deserialize(ss, dh));

		BOOST_CHECK(oalOrig == oalCopy);
	} // for r
}

//======================================================================

BOOST_AUTO_TEST_SUITE_END()

//======================================================================
