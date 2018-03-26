//======================================================================
/*! \file ObjectAssociationList4001TestSupport.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 26, 2016
 *///-------------------------------------------------------------------

#ifndef OBJECTASSOCIATIONLIST4001TESTSUPPORT_HPP_SEEN
#define OBJECTASSOCIATIONLIST4001TESTSUPPORT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include "TestSupport.hpp"

#include <ibeosdk/datablocks/ObjectAssociationList4001.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

class ObjectAssociationList4001TestSupport : public TestSupport {
public:
	ObjectAssociationList4001TestSupport();
	virtual ~ObjectAssociationList4001TestSupport();

public:
	static
	ibeosdk::ObjectAssociationList4001 createAssocList();

	static
	ibeosdk::ObjectAssociation4001 createAssoc();

private:
	static
	void fillRandomly(ibeosdk::ObjectAssociationList4001& oal);

	static
	void fillRandomly(ibeosdk::ObjectAssociation4001& oa);

protected:
	static const int32_t nbOfRepeats = 100;
}; // ObjectAssociationList4001TestSupport

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================

#endif // OBJECTASSOCIATIONLIST4001TESTSUPPORT_HPP_SEEN

//======================================================================
