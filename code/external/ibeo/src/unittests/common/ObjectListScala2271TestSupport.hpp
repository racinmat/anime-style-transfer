//======================================================================
/*! \file ObjectListScala2271TestSupport.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 29, 2015
 *///-------------------------------------------------------------------

#ifndef OBJECTLISTSCALA2271TESTSUPPORT_HPP_SEEN
#define OBJECTLISTSCALA2271TESTSUPPORT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include "TestSupport.hpp"

#include <ibeosdk/datablocks/ObjectListScala2271.hpp>

//======================================================================

namespace ibeosdk {
class ObjectScala2271;
class UnfilteredObjectAttributes;
class FilteredObjectAttributes;

namespace unittests {

//======================================================================

class ObjectListScala2271TestSupport : public TestSupport {
public:
	ObjectListScala2271TestSupport();
	virtual ~ObjectListScala2271TestSupport();

protected:
	static
	ibeosdk::ObjectScala2271 createObj(const bool unfAvailable,
	                                   const bool unfContourAvailable,
	                                   const bool filAvailable,
	                                   const bool filContourAvailable);

	static
	ibeosdk::ObjectListScala2271 createObjList();

private:
	static
	void fillRandomly(ibeosdk::ObjectScala2271& obj);

	static
	void fillRandomly(ibeosdk::ObjectScala2271& obj,
	                  const bool unfAvailable,
	                  const bool unfContourAvailable,
	                  const bool filAvailable,
	                  const bool filContourAvailable);

	static
	void fillRandomly(ibeosdk::UnfilteredObjectAttributes& unf,
	                  const bool isAvailable, const bool contourIsAvailable);

	static
	void fillRandomly(ibeosdk::FilteredObjectAttributes& fil,
	                  const bool isAvailable, const bool contourIsAvailable);
}; // ObjectListScala2271TestSupport

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================

#endif // OBJECTLISTSCALA2271TESTSUPPORT_HPP_SEEN

//======================================================================
