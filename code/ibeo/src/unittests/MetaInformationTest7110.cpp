//======================================================================
/*! \file MetaInformationTest7110.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

#include "common/MetaInformationTestSupport7110.hpp"

#include <ibeosdk/datablocks/snippets/MetaInformationAppBaseConfig.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationEcuId.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationKeywords.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationUnsupported.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationVersionNumber.hpp>

// we link the boost_unit_test_framework library dynamically
#ifndef _WIN32
#	define BOOST_TEST_DYN_LINK
#endif // !_WIN32
#define BOOST_TEST_MODULE MetaInformationTest
#include <boost/test/unit_test.hpp>

//======================================================================

using namespace ibeosdk;

//======================================================================

BOOST_AUTO_TEST_SUITE( MetaInformationListSuite )

//======================================================================

class Fixture : public unittests::MetaInformationTestSupport {}; // Fixture

//======================================================================

BOOST_FIXTURE_TEST_CASE(creationAppBaseConfig, Fixture)
{
	MetaInformationAppBaseConfig abcOrig;
	BOOST_CHECK(true);
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isSerializationDeserializationIdentityAppBaseConfig, Fixture)
{
	for (int r = 0; r < nbOfRepeats; ++r) {
		MetaInformationAppBaseConfig abcOrig;
		fillRandomly(abcOrig);

		std::stringstream ss;
		BOOST_CHECK(abcOrig.serialize(ss));

		MetaInformationAppBaseConfig abcCopy;
		BOOST_CHECK(abcCopy.deserialize(ss));

		BOOST_CHECK(abcOrig == abcCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isSerializationDeserializationIdentityEcuId, Fixture)
{
	for (int r = 0; r < nbOfRepeats; ++r) {
		MetaInformationEcuId ecuIdOrig;
		fillRandomly(ecuIdOrig);

		std::stringstream ss;
		BOOST_CHECK(ecuIdOrig.serialize(ss));

		MetaInformationEcuId ecuIdCopy;
		BOOST_CHECK(ecuIdCopy.deserialize(ss));

		BOOST_CHECK(ecuIdOrig == ecuIdCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isSerializationDeserializationIdentityKeywords, Fixture)
{
	for (int r = 0; r < nbOfRepeats; ++r) {
		MetaInformationKeywords kwOrig;
		fillRandomly(kwOrig);

		std::stringstream ss;
		BOOST_CHECK(kwOrig.serialize(ss));

		MetaInformationKeywords kwCopy;
		BOOST_CHECK(kwCopy.deserialize(ss));

		BOOST_CHECK(kwOrig == kwCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isSerializationDeserializationIdentityUnsupported, Fixture)
{
	for (int r = 0; r < nbOfRepeats; ++r) {
		MetaInformationUnsupported unsOrig(0, NTPTime(), 0);
		fillRandomly(unsOrig);

		std::stringstream ss;
		BOOST_CHECK(unsOrig.serialize(ss));

		MetaInformationUnsupported unsCopy(0, NTPTime(), 0);
		BOOST_CHECK(unsCopy.deserialize(ss));

		BOOST_CHECK(unsOrig == unsCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isSerializationDeserializationIdentityVersionNumber, Fixture)
{
	for (int r = 0; r < nbOfRepeats; ++r) {
		MetaInformationVersionNumber vnOrig;
		fillRandomly(vnOrig);

		std::stringstream ss;
		BOOST_CHECK(vnOrig.serialize(ss));

		MetaInformationVersionNumber vnCopy;
		BOOST_CHECK(vnCopy.deserialize(ss));

		BOOST_CHECK(vnOrig == vnCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isReadWriteUnsupported, Fixture)
{
	for (int r = 0; r < nbOfRepeats; ++r) {
		MetaInformationList7110 milOrig(MetaInformationTestSupport::createMIL(MetaInformation::MetaInfoType_Unsupported));

		const IbeoDataHeader dh(MetaInformationList7110::getDataBlockId(),
		                        0, uint32_t(milOrig.getSerializedSize()),
		                        0,
		                        NTPTime());

		std::stringstream ss;
		BOOST_CHECK(milOrig.serialize(ss));

		MetaInformationList7110 milCopy;
		BOOST_CHECK(milCopy.deserialize(ss, dh));

//		std::cerr << "Orig: " << std::endl;
//		milOrig.printStatistic(std::cerr);
//		std::cerr << "Copy: " << std::endl;
//		milCopy.printStatistic(std::cerr);

		BOOST_CHECK(milOrig == milCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isReadWriteVersionNumber, Fixture)
{
	for (int r = 0; r < nbOfRepeats; ++r) {
		MetaInformationList7110 milOrig(MetaInformationTestSupport::createMIL(MetaInformation::MetaInfoType_VersionNumber));

		const IbeoDataHeader dh(MetaInformationList7110::getDataBlockId(),
		                        0, uint32_t(milOrig.getSerializedSize()),
		                        0,
		                        NTPTime());

		std::stringstream ss;
		BOOST_CHECK(milOrig.serialize(ss));

		MetaInformationList7110 milCopy;
		BOOST_CHECK(milCopy.deserialize(ss, dh));

		BOOST_CHECK(milOrig == milCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isReadWriteAppBaseConfig, Fixture)
{
	for (int r = 0; r < nbOfRepeats; ++r) {
		MetaInformationList7110 milOrig(MetaInformationTestSupport::createMIL(MetaInformation::MetaInfoType_AppBaseConfig));

		const IbeoDataHeader dh(MetaInformationList7110::getDataBlockId(),
		                        0, uint32_t(milOrig.getSerializedSize()),
		                        0,
		                        NTPTime());

		std::stringstream ss;
		BOOST_CHECK(milOrig.serialize(ss));

		MetaInformationList7110 milCopy;
		BOOST_CHECK(milCopy.deserialize(ss, dh));

		BOOST_CHECK(milOrig == milCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isReadWriteEcuId, Fixture)
{
	for (int r = 0; r < nbOfRepeats; ++r) {
		MetaInformationList7110 milOrig(MetaInformationTestSupport::createMIL(MetaInformation::MetaInfoType_EcuId));

		const IbeoDataHeader dh(MetaInformationList7110::getDataBlockId(),
		                        0, uint32_t(milOrig.getSerializedSize()),
		                        0,
		                        NTPTime());

		std::stringstream ss;
		BOOST_CHECK(milOrig.serialize(ss));

		MetaInformationList7110 milCopy;
		BOOST_CHECK(milCopy.deserialize(ss, dh));

		BOOST_CHECK(milOrig == milCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isReadWriteKeywords, Fixture)
{
	for (int r = 0; r < nbOfRepeats; ++r) {
		MetaInformationList7110 milOrig(MetaInformationTestSupport::createMIL(MetaInformation::MetaInfoType_Keywords));

		const IbeoDataHeader dh(MetaInformationList7110::getDataBlockId(),
		                        0, uint32_t(milOrig.getSerializedSize()),
		                        0,
		                        NTPTime());

		std::stringstream ss;
		BOOST_CHECK(milOrig.serialize(ss));

		MetaInformationList7110 milCopy;
		BOOST_CHECK(milCopy.deserialize(ss, dh));

		BOOST_CHECK(milOrig == milCopy);
	} // for r
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isSerializationDeserializationIdentity, Fixture)
{
	for (int r = 0; r < nbOfRepeats; ++r) {
		MetaInformationList7110 milOrig(MetaInformationTestSupport::createMIL());

		const IbeoDataHeader dh(MetaInformationList7110::getDataBlockId(),
		                        0, uint32_t(milOrig.getSerializedSize()),
		                        0,
		                        NTPTime());

		std::stringstream ss;
		BOOST_CHECK(milOrig.serialize(ss));

		MetaInformationList7110 milCopy;
		BOOST_CHECK(milCopy.deserialize(ss, dh));

		BOOST_CHECK(milOrig == milCopy);
	} // for r
}

//======================================================================

BOOST_AUTO_TEST_SUITE_END()

//======================================================================
