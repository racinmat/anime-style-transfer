//======================================================================
/*! \file ObjectListScala2271Test.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 29, 2015
 *///-------------------------------------------------------------------


//======================================================================

#include "common/ObjectListScala2271TestSupport.hpp"

// we link the boost_unit_test_framework library dynamically
#ifndef _WIN32
#	define BOOST_TEST_DYN_LINK
#endif // !_WIN32
#define BOOST_TEST_MODULE ObjectListScala2271Test
#include <boost/test/unit_test.hpp>

//======================================================================

using namespace ibeosdk;

//======================================================================

BOOST_AUTO_TEST_SUITE( ObjectListScala2271Suite )

//======================================================================

class Fixture : public unittests::ObjectListScala2271TestSupport {}; // Fixture

//======================================================================

BOOST_FIXTURE_TEST_CASE(isSerializationDeserializationIdentity, Fixture)
{
	for (int i = 0; i < 100; ++i) {
		ObjectListScala2271 objListOrig = createObjList();

		const IbeoDataHeader dh(ObjectListScala2271::getDataBlockId(),
		                        0, uint32_t(objListOrig.getSerializedSize()),
		                        0,
		                        NTPTime());

		std::stringstream ss;
		BOOST_CHECK(objListOrig.serialize(ss));

		ObjectListScala2271 objListCopy;
		BOOST_CHECK(objListCopy.deserialize(ss, dh));
		BOOST_CHECK(objListOrig == objListCopy);
	}
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isEqualNotNonEqual, Fixture)
{
	for (int i = 0; i < 100; ++i) {
		ObjectListScala2271 objListOrig = createObjList();

		BOOST_CHECK(objListOrig == objListOrig);
		BOOST_CHECK(!(objListOrig != objListOrig));
	}
}

//======================================================================

BOOST_AUTO_TEST_SUITE_END()

//======================================================================
