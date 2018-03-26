//======================================================================
/*! \file TimeRecord9000TestSupport.hpp
 *
 * \copydoc Copyright
 * \author Anil Kumar (ank)
 * \date Feb 24, 2017
 *///-------------------------------------------------------------------

#ifndef TIMERECORD9000TESTSUPPORT_HPP_SEEN
#define TIMERECORD9000TESTSUPPORT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include "TestSupport.hpp"

#include <ibeosdk/datablocks/TimeRecord9000.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

class TimeRecord9000TestSupport : public TestSupport {
public:
	TimeRecord9000TestSupport();
	virtual ~TimeRecord9000TestSupport();

public:
	static
	ibeosdk::TimeRecord9000 createTimeRecord9000();

private:
	static
	void fillRandomly(ibeosdk::TimeRecord9000& tr);

protected:
	static const int32_t nbOfRepeats = 100;
}; // TimeRecord9000TestSupport

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//================================================= =====================

#endif // TIMERECORD9000TESTSUPPORT_HPP_SEEN

//======================================================================
