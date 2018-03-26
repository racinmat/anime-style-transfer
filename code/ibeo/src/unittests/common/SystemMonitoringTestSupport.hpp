//======================================================================
/*! \file SystemMonitoringTestSupport.hpp
 *
 * \copydoc Copyright
 * \author Martin Schlodinski (msch)
 * \date 18.04.2017
 *///-------------------------------------------------------------------

#ifndef SYSTEMMONITORINGTESTSUPPORT_HPP_SEEN
#define SYSTEMMONITORINGTESTSUPPORT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include "TestSupport.hpp"

#include <ibeosdk/datablocks/SystemMonitoringDeviceStatus6701.hpp>
#include <ibeosdk/datablocks/SystemMonitoringSystemStatus6705.hpp>
#include <ibeosdk/datablocks/snippets/ClockType.hpp>
#include <ibeosdk/datablocks/snippets/Timestamp.hpp>
#include <ibeosdk/datablocks/SystemMonitoringCanStatus6700.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

class SystemMonitoringTestSupport : public TestSupport {
public:
	SystemMonitoringTestSupport();
	virtual ~SystemMonitoringTestSupport();

public:
	static
	ibeosdk::SystemMonitoringCanStatus6700 createSystemMonitoringCanStatus6700();

	static
	ibeosdk::SystemMonitoringDeviceStatus6701 createSystemMonitoringDeviceStatus6701();

	static
	ibeosdk::SystemMonitoringSystemStatus6705 createSystemMonitoringSystemStatus6705();

private:
	static
	void fillRandomly(ibeosdk::SystemMonitoringCanStatus6700& input);

	static
	void fillRandomly(ibeosdk::SystemMonitoringDeviceStatus6701& input);

	static
	void fillRandomly(ibeosdk::SystemMonitoringSystemStatus6705& input);

protected:
	static const int32_t nbOfRepeats = 100;
}; // SystemMonitoringTestSupport

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//================================================= =====================

#endif // SYSTEMMONITORINGTESTSUPPORT_HPP_SEEN

//======================================================================
