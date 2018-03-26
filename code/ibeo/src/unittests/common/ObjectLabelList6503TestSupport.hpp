//======================================================================
/*! \file ObjectLabelList6503TestSupport.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 28, 2016
 *///-------------------------------------------------------------------

#ifndef OBJECTLABELLIST6503TESTSUPPORT_HPP_SEEN
#define OBJECTLABELLIST6503TESTSUPPORT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include "TestSupport.hpp"

#include <ibeosdk/datablocks/ObjectLabelList6503.hpp>

//======================================================================

namespace ibeosdk {

typedef uint64_t Uoid;

namespace unittests {

//======================================================================

class ObjectLabelList6503TestSupport : public TestSupport {
public:
	ObjectLabelList6503TestSupport();
	virtual ~ObjectLabelList6503TestSupport();

public:
	static
	ibeosdk::ObjectLabelList6503 createObjectLabelList6503();

	static
	ibeosdk::ObjectLabel6503 createObjectLabel6503();

private:
	static
	void fillRandomly(ibeosdk::ObjectLabelList6503& oll);

	static
	void fillRandomly(ibeosdk::ObjectLabel6503& ol);

	static
	void fillRandomly(ibeosdk::MeasurementList& ml);

protected:
	static const int32_t nbOfRepeats = 1;

}; // ObjectLabelList6503TestSupport

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================

#endif // OBJECTLABELLIST6503TESTSUPPORT_HPP_SEEN

//======================================================================
