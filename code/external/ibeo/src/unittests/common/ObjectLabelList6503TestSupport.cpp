//======================================================================
/*! \file ObjectLabelList6503TestSupport.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 28, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <unittests/common/ObjectLabelList6503TestSupport.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

ObjectLabelList6503TestSupport::ObjectLabelList6503TestSupport()
  : TestSupport()
{}

//======================================================================

ObjectLabelList6503TestSupport::~ObjectLabelList6503TestSupport() {}

//======================================================================

ibeosdk::ObjectLabelList6503 ObjectLabelList6503TestSupport::createObjectLabelList6503()
{
	ibeosdk::ObjectLabelList6503 oll;
	fillRandomly(oll);
	return oll;
}

//======================================================================

ibeosdk::ObjectLabel6503 ObjectLabelList6503TestSupport::createObjectLabel6503()
{
	ibeosdk::ObjectLabel6503 ol;
	fillRandomly(ol);
	return ol;
}

//======================================================================

void ObjectLabelList6503TestSupport::fillRandomly(ibeosdk::ObjectLabelList6503& oll)
{
	oll.setTimeOffsetUs(getRandValue<uint32_t>());
	oll.setTimestamp(getRandValue<NTPTime>());
	oll.setScanNumber(getRandValue<uint32_t>());
	oll.setScanMidTimestamp(getRandValue<NTPTime>());

	for (uint16_t r = 0; r < ObjectLabelList6503::nbOfReserved; ++r) {
		oll.setReserved(r, getRandValue<uint16_t>());
	}

	ObjectLabelList6503::LabelVector::iterator olIter = oll.getLabels().begin();
	for (;  olIter != oll.getLabels().end(); ++olIter) {
		fillRandomly(*olIter);
	}
}

//======================================================================

void ObjectLabelList6503TestSupport::fillRandomly(ibeosdk::ObjectLabel6503& ol)
{
	ol.getObjBox() = getRandValue<Box2d>();
	ol.setWeight(getRandValue<uint8_t>());
	ol.setClassification(getRandValue<uint8_t>());
	ol.setId(getRandValue<uint32_t>());
	ol.setTrackingId(getRandValue<Uoid>());
	ol.setDisplayMagnificationFactor(getRandValue<float>());
	ol.setObjectFlags(getRandValue<uint16_t>());
	ol.setIsInOnKeyframe(getRandValue<uint16_t>());

	for (uint16_t r = 0; r < ObjectLabel6503::nbOfReserved; ++r) {
		ol.setReserved(r, getRandValue<uint16_t>());
	}

	fillRandomly(ol.getUserData());
}

//======================================================================

void ObjectLabelList6503TestSupport::fillRandomly(ibeosdk::MeasurementList& ml)
{
	const bool hasMeasurementList = getRandValue<bool>();
	if (hasMeasurementList) {
		Measurement m;
		m.setKey(MeasurementKey::LabelUserData); // currently only key

		const uint16_t strLen = getRandValue<uint16_t>();
		std::string s;
		s.resize(strLen);
		std::string::iterator sIter = s.begin();
		for (; sIter != s.end(); ++sIter) {
			*sIter = getRandValue<char>();
		}
		m.setData(s);
		ml.addMeasurement(m);
	}
}

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================
