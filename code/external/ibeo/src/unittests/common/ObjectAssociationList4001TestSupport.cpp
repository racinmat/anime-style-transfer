//======================================================================
/*! \file ObjectAssociationList4001TestSupport.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 26, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <unittests/common/ObjectAssociationList4001TestSupport.hpp>

using namespace ibeosdk;

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

ObjectAssociationList4001TestSupport::ObjectAssociationList4001TestSupport()
  : TestSupport()
{}

//======================================================================

ObjectAssociationList4001TestSupport::~ObjectAssociationList4001TestSupport() {}

//======================================================================

ibeosdk::ObjectAssociationList4001 ObjectAssociationList4001TestSupport::createAssocList()
{
	ibeosdk::ObjectAssociationList4001 oal;
	fillRandomly(oal);
	return oal;
}

//======================================================================

ibeosdk::ObjectAssociation4001 ObjectAssociationList4001TestSupport::createAssoc()
{
	ibeosdk::ObjectAssociation4001 oa;
	fillRandomly(oa);
	return oa;
}

//======================================================================

void ObjectAssociationList4001TestSupport::fillRandomly(ibeosdk::ObjectAssociationList4001& oal)
{
	oal.setRefObjListId(getRandValue<uint8_t>());
	oal.setRefDevType(getRandValue<uint8_t>());
	oal.setRefDevInterfaceVersion(getRandValue<uint16_t>(0x3FFF));

	oal.setDutObjListId(getRandValue<uint8_t>());
	oal.setDutDevType(getRandValue<uint8_t>());
	oal.setDutDevInterfaceVersion(getRandValue<uint16_t>(0x3FFF));

	for (int32_t i = 0; i < ObjectAssociationList4001::nbOfReserved; ++i) {
		oal.setReserved(i, getRandValue<char>());
	}

	const uint32_t nbOfAssocs = getRandValue<uint32_t>(0x3FF);

	ObjectAssociationList4001::ObjAssocVector& oav = oal.getObjectAssociations();
	oav.resize(nbOfAssocs);

	ObjectAssociationList4001::ObjAssocVector::iterator oaIter = oav.begin();
	for (;  oaIter != oav.end(); ++oaIter) {
		fillRandomly(*oaIter);
	}
}

//======================================================================

void ObjectAssociationList4001TestSupport::fillRandomly(ibeosdk::ObjectAssociation4001& oa)
{
	oa.setRefObjId(getRandValue<uint32_t>());
	oa.setDutObjId(getRandValue<uint32_t>());
	oa.setTimestampFirst(getRandValue<NTPTime>());
	oa.setTimestampLast(getRandValue<NTPTime>());
	oa.setCertainty(getRandValue<float>());
}

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================
