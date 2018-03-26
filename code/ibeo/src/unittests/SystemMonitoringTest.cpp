//======================================================================
/*! \file SystemMonitoringTest.cpp
 *
 * \copydoc Copyright
 * \author Martin Schlodinski (msch)
 * \date 18.04.2017
 *///-------------------------------------------------------------------


#include <unittests/common/SystemMonitoringTestSupport.hpp>

// we link the boost_unit_test_framework library dynamically
#ifndef _WIN32
#	define BOOST_TEST_DYN_LINK
#endif // !_WIN32
#define BOOST_TEST_MODULE SystemMonitoringTest
#include <boost/test/unit_test.hpp>

//======================================================================

using namespace ibeosdk;

//======================================================================

BOOST_AUTO_TEST_SUITE( SystemMonitoringTestSuite )

//======================================================================

class Fixture : public unittests::SystemMonitoringTestSupport {}; // Fixture

//======================================================================

BOOST_FIXTURE_TEST_CASE(createSystemMonitoring, Fixture)
{
	ibeosdk::SystemMonitoringCanStatus6700 o1;
	ibeosdk::SystemMonitoringDeviceStatus6701 o2;
	ibeosdk::SystemMonitoringSystemStatus6705 o3;
	BOOST_CHECK(true);
}

//======================================================================

BOOST_FIXTURE_TEST_CASE(isIoSystemMonitoringIdentity, Fixture)
{
	for (int32_t r = 0; r < nbOfRepeats; ++r) {
		SystemMonitoringCanStatus6700 statusCan(createSystemMonitoringCanStatus6700());
		SystemMonitoringDeviceStatus6701 statusDevice(createSystemMonitoringDeviceStatus6701());
		SystemMonitoringSystemStatus6705 statusSystem(createSystemMonitoringSystemStatus6705());

		const IbeoDataHeader dhCan(SystemMonitoringCanStatus6700::getDataBlockId(),
		                        0, uint32_t(statusCan.getSerializedSize()),
		                        0,
		                        NTPTime());

		const IbeoDataHeader dhDevice(SystemMonitoringDeviceStatus6701::getDataBlockId(),
		                        0, uint32_t(statusDevice.getSerializedSize()),
		                        0,
		                        NTPTime());

		const IbeoDataHeader dhSystem(SystemMonitoringSystemStatus6705::getDataBlockId(),
		                        0, uint32_t(statusSystem.getSerializedSize()),
		                        0,
		                        NTPTime());

		std::stringstream ssCan;
		BOOST_CHECK(statusCan.serialize(ssCan));

		std::stringstream ssDevice;
		BOOST_CHECK(statusDevice.serialize(ssDevice));

		std::stringstream ssSystem;
		BOOST_CHECK(statusSystem.serialize(ssSystem));

		SystemMonitoringCanStatus6700 copyCan;
		BOOST_CHECK(copyCan.deserialize(ssCan,dhCan));

		SystemMonitoringDeviceStatus6701 copyDevice;
		BOOST_CHECK(copyDevice.deserialize(ssDevice,dhDevice));

		SystemMonitoringSystemStatus6705 copySystem;
		BOOST_CHECK(copySystem.deserialize(ssSystem,dhSystem));

		BOOST_CHECK(copyCan == statusCan);

		BOOST_CHECK(copyDevice == statusDevice);

		BOOST_CHECK(copySystem == statusSystem);
	} // for r
}

//======================================================================

BOOST_AUTO_TEST_SUITE_END()

//======================================================================
