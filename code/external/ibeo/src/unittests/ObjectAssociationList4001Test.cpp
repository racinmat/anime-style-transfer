//======================================================================
/*! \file ObjectAssociationList4001Test.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 26, 2016
 *///-------------------------------------------------------------------

#include "common/ObjectAssociationList4001TestSupport.hpp"

// we link the boost_unit_test_framework library dynamically
#ifndef _WIN32
#	define BOOST_TEST_DYN_LINK
#endif // !_WIN32
#define BOOST_TEST_MODULE ObjectAssociationList4001Test
#include <boost/test/unit_test.hpp>

//======================================================================

using namespace ibeosdk;

//======================================================================

BOOST_AUTO_TEST_SUITE( ObjectAssociationList4001Suite )

//======================================================================

class Fixture : public unittests::ObjectAssociationList4001TestSupport {}; // Fixture

//======================================================================

BOOST_FIXTURE_TEST_CASE(creationObjectAssociation4001, Fixture)
{
	ibeosdk::ObjectAssociation4001 oa;
	BOOST_CHECK(true);
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(creationObjectAssociationList4001, Fixture)
{
	ibeosdk::ObjectAssociationList4001 oal;
	BOOST_CHECK(true);
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isIoObjAssoc4001Identity, Fixture)
{
	for (int32_t r = 0; r < nbOfRepeats; ++r) {
		ObjectAssociation4001 oaOrig(createAssoc());
		std::stringstream ss;
		BOOST_CHECK(oaOrig.serialize(ss));

		ObjectAssociation4001 oaCopy;
		BOOST_CHECK(oaCopy.deserialize(ss));

		BOOST_CHECK(oaOrig == oaCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isIoObjAssoc4001ListIdentity, Fixture)
{
	for (int32_t r = 0; r < nbOfRepeats; ++r) {
		ObjectAssociationList4001 oalOrig(createAssocList());

		const IbeoDataHeader dh(ObjectAssociationList4001::getDataBlockId(),
		                        0, uint32_t(oalOrig.getSerializedSize()),
		                        0,
		                        NTPTime());

		std::stringstream ss;
		BOOST_CHECK(oalOrig.serialize(ss));

		ObjectAssociationList4001 oalCopy;
		BOOST_CHECK(oalCopy.deserialize(ss, dh));

		BOOST_CHECK(oalOrig == oalCopy);
	} // for r
}

//======================================================================

BOOST_AUTO_TEST_SUITE_END()

//======================================================================
