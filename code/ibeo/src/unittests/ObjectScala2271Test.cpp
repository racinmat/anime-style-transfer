//======================================================================
/*! \file ObjectScala2271Test.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 28, 2015
 *///-------------------------------------------------------------------


//======================================================================

#include "common/ObjectListScala2271TestSupport.hpp"

// we link the boost_unit_test_framework library dynamically
#ifndef _WIN32
#	define BOOST_TEST_DYN_LINK
#endif // !_WIN32
#define BOOST_TEST_MODULE ObjectScala2271Test
#include <boost/test/unit_test.hpp>

//======================================================================

using namespace ibeosdk;

//======================================================================

BOOST_AUTO_TEST_SUITE( ObjectScala2271Suite )

//======================================================================

class Fixture : public unittests::ObjectListScala2271TestSupport {}; // Fixture

//======================================================================


BOOST_FIXTURE_TEST_CASE(isSerializationDeserializationIdentity, Fixture)
{
	for (int ua = 0; ua <= 1; ++ua) {
		for (int uc = 0; uc <= 1; ++uc) {
			if (ua == 0 && uc == 1)
				continue;

			for (int fa = 0; fa <= 1; ++fa) {
				for (int fc = 0; fc <= 1; ++fc) {
					if (fa == 0 && fc == 1)
						continue;

					ObjectScala2271 objOrig = createObj(ua != 0 ? true : false,
					                                    uc != 0 ? true : false,
					                                    fa != 0 ? true : false,
					                                    fc != 0 ? true : false);

					std::stringstream ss;
					BOOST_CHECK(objOrig.serialize(ss));

					ObjectScala2271 objCopy;
					BOOST_CHECK(objCopy.deserialize(ss));

					BOOST_CHECK(objOrig.getFilteredObjectAttributes() == objCopy.getFilteredObjectAttributes());
					BOOST_CHECK(objOrig.getUnfilteredObjectAttributes() == objCopy.getUnfilteredObjectAttributes());

					BOOST_CHECK(objOrig == objCopy);
				}
			}
		}
	}

}

//======================================================================

BOOST_AUTO_TEST_SUITE_END()

//======================================================================
