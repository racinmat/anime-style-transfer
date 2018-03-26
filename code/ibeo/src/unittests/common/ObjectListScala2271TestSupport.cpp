//======================================================================
/*! \file ObjectListScala2271TestSupport.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 29, 2015
 *///-------------------------------------------------------------------

//======================================================================

#include "ObjectListScala2271TestSupport.hpp"

#include <ibeosdk/datablocks/ObjectListScala2271.hpp>

using namespace ibeosdk;

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

ObjectListScala2271TestSupport::ObjectListScala2271TestSupport()
  : TestSupport()
{}

//======================================================================

ObjectListScala2271TestSupport::~ObjectListScala2271TestSupport() {}

//======================================================================

ObjectListScala2271 ObjectListScala2271TestSupport::createObjList()
{
	ObjectListScala2271 objList(getRandValue<NTPTime>());
	objList.setScanNumber(getRandValue<uint16_t>());
	objList.setObjectListId(getRandValue<uint8_t>());
	objList.setDeviceId(getRandValue<uint8_t>());
	objList.setDeviceInterfaceVersion(getRandValue<uint16_t>(0x3FFF));
	objList.setReserved(getRandValue<uint32_t>());

	std::vector<ObjectScala2271>& objs = objList.getObjects();
	objs.resize(getRandValue<uint16_t>(500));

	for (uint16_t i = 0; i < objs.size(); ++i) {
		fillRandomly(objs[i]);
	}

	return objList;
}

//======================================================================

ObjectScala2271 ObjectListScala2271TestSupport::createObj(const bool unfAvailable,
                                                          const bool unfContourAvailable,
                                                          const bool filAvailable,
                                                          const bool filContourAvailable)
{
	ObjectScala2271 obj;
	fillRandomly(obj,
	             unfAvailable, unfContourAvailable,
	             filAvailable, filContourAvailable);
	return obj;
}


//======================================================================

void ObjectListScala2271TestSupport::fillRandomly(ObjectScala2271& obj)
{
	const bool unfAvailable = getRandValue<bool>();
	const bool unfContourAvailable = unfAvailable ? getRandValue<bool>() : false;
	const bool filAvailable = getRandValue<bool>();
	const bool filContourAvailable = filAvailable ? getRandValue<bool>() : false;

	fillRandomly(obj,
	             unfAvailable, unfContourAvailable,
	             filAvailable, filContourAvailable);
}

//======================================================================

void ObjectListScala2271TestSupport::fillRandomly(ObjectScala2271& obj,
                                                  const bool unfAvailable,
                                                  const bool unfContourAvailable,
                                                  const bool filAvailable,
                                                  const bool filContourAvailable)
{
	obj.setObjectId(getRandValue<uint32_t>());
	obj.setInternal(getRandValue<uint8_t>());
	obj.setInterfaceFlags(getRandValue<uint8_t>());

	UnfilteredObjectAttributes& unfAttr = obj.getUnfilteredObjectAttributes();
	fillRandomly(unfAttr, unfAvailable, unfContourAvailable);

	FilteredObjectAttributes& filAttr = obj.getFilteredObjectAttributes();
	fillRandomly(filAttr, filAvailable, filContourAvailable);

	obj.setReserved(getRandValue<uint32_t>());
}

//======================================================================

void ObjectListScala2271TestSupport::fillRandomly(UnfilteredObjectAttributes& unf,
                                                  const bool isAvailable, const bool contourIsAvailable)
{
	unf.setIsValid(isAvailable);
	if (!isAvailable) {
		return;
	}

	unf.setPriority(getRandValue<uint8_t>());
	unf.setRelativeTimeOfMeasure(getRandValue<uint16_t>());
	unf.setPositionClosestObjectPoint(getRandValue<Point2d>());
	unf.setReserved(getRandValue<uint16_t>());

	unf.setObjectBoxSize(getRandValue<PointSigma2d>());
	unf.setObjectBoxSizeSigma(getRandValue<PointSigma2d>());
	unf.setObjectBoxOrientation(getRandValue<int16_t>());
	unf.setObjectBoxOrientationSigma(getRandValue<uint16_t>());
	unf.setObjectBoxHeight(getRandValue<uint8_t>());

	unf.setReferencePointLocation(getRandValue<RefPointBoxLocation>());
	unf.setReferencePointCoord(getRandValue<Point2d>());
	unf.setReferencePointCoordSigma(getRandValue<PointSigma2d>());
	unf.setReferencePointPositionCorrCoeff(getRandValue<int16_t>());

	unf.setExistenceProbaility(getRandValue<uint8_t>());

	unf.setPossibleNbOfContourPoints(getRandValue<uint8_t>(20));
	unf.setHasContourPoints(contourIsAvailable);
	if (contourIsAvailable) {
		std::vector<ContourPoint>& cps = unf.getContourPoints();
		cps.resize(unf.getPossibleNbOfContourPoints());

		for (uint8_t p = 0; p < unf.getPossibleNbOfContourPoints(); ++p) {
			cps[p] = ContourPoint(getRandValue<Point2d>());
		}
	} // unfContourAvailable
}

//======================================================================

void ObjectListScala2271TestSupport::fillRandomly(FilteredObjectAttributes& fil,
                                                  const bool isAvailable, const bool contourIsAvailable)
{
	fil.setIsValid(isAvailable);
	if (!isAvailable) {
		return;
	}

	fil.setPriority(getRandValue<uint8_t>());

	fil.setObjectAge(getRandValue<uint16_t>());
	fil.setHiddenStatusAge(getRandValue<uint16_t>());
	fil.setDynamicFlag(getRandValue<uint8_t>());

	fil.setRelativeTimeOfMeasure(getRandValue<uint16_t>());
	fil.setPositionClosestObjectPoint(getRandValue<Point2d>());

	fil.setRelativeVelocity(getRandValue<Point2d>());
	fil.setRelativeVelocitySigma(getRandValue<PointSigma2d>());

	fil.setClassification(getRandValue<rawObjectClass::RawObjectClass>());
	fil.setClassificationQuality(getRandValue<uint8_t>());
	fil.setClassificationAge(getRandValue<uint16_t>());

	fil.setReserved(getRandValue<uint16_t>());

	fil.setObjectBoxSize(getRandValue<PointSigma2d>());
	fil.setObjectBoxSizeSigma(getRandValue<PointSigma2d>());
	fil.setObjectBoxOrientation(getRandValue<int16_t>());
	fil.setObjectBoxOrientationSigma(getRandValue<uint16_t>());
	fil.setObjectBoxHeight(getRandValue<uint8_t>());

	fil.setReferencePointLocation(getRandValue<RefPointBoxLocation>());
	fil.setReferencePointCoord(getRandValue<Point2d>());
	fil.setReferencePointCoordSigma(getRandValue<PointSigma2d>());
	fil.setReferencePointPositionCorrCoeff(getRandValue<int16_t>());

	fil.setExistenceProbaility(getRandValue<uint8_t>());

	fil.setAbsoluteVelocity(getRandValue<Point2d>());
	fil.setAbsoluteVelocitySigma(getRandValue<PointSigma2d>());
	fil.setVelocityCorrCoeff(getRandValue<int16_t>());

	fil.setAcceleration(getRandValue<Point2d>());
	fil.setAccelerationSigma(getRandValue<PointSigma2d>());
	fil.setAccelerationCorrCoeff(getRandValue<int16_t>());

	fil.setYawRate(getRandValue<int16_t>());
	fil.setYawRateSigma(getRandValue<uint16_t>());

	fil.setPossibleNbOfContourPoints(getRandValue<uint8_t>(20));
	fil.setHasContourPoints(contourIsAvailable);
	if (contourIsAvailable) {
		std::vector<ContourPoint>& cps = fil.getContourPoints();
		cps.resize(fil.getPossibleNbOfContourPoints());

		for (uint8_t p = 0; p < fil.getPossibleNbOfContourPoints(); ++p) {
			cps[p] = ContourPoint(getRandValue<Point2d>());
		}
	} // unfContourAvailable
}

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================
