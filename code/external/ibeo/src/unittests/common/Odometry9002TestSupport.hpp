//======================================================================
/*! \file Odometry9002TestSupport.hpp
 *
 * \copydoc Copyright
 * \author Anil Kumar (ank)
 * \date Mar 28, 2017
 *///-------------------------------------------------------------------

#ifndef ODOMETRY9002TESTSUPPORT_HPP_SEEN
#define ODOMETRY9002TESTSUPPORT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include "TestSupport.hpp"

#include <ibeosdk/datablocks/Odometry9002.hpp>
#include <ibeosdk/datablocks/snippets/ClockType.hpp>
#include <ibeosdk/datablocks/snippets/Timestamp.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

class Odometry9002TestSupport : public TestSupport {
public:
	Odometry9002TestSupport();
	virtual ~Odometry9002TestSupport();

public:
	static
	ibeosdk::Odometry9002 createOdometry9002();

private:
	static
	void fillRandomly(ibeosdk::Odometry9002& od);

protected:
	static const int32_t nbOfRepeats = 100;
}; // Odometry9002TestSupport

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//================================================= =====================

#endif // ODOMETRY9002TESTSUPPORT_HPP_SEEN

//======================================================================
