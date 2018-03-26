//======================================================================
/*! \file  ObjectBasic.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 4, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_OBJECTBASIC_HPP_SEEN
#define IBEOSDK_OBJECTBASIC_HPP_SEEN

//======================================================================

#include <ibeosdk/io_prototypes.hpp>
#include <ibeosdk/datablocks/snippets/Point2dFloat.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

enum ObjectClass {
	ObjectClass_Unclassified = 0,
	ObjectClass_UnknownSmall = 1,
	ObjectClass_UnknownBig = 2,
	ObjectClass_Pedestrian = 3,
	ObjectClass_Bike = 4,
	ObjectClass_Car = 5,
	ObjectClass_Truck = 6,
	ObjectClass_Underdriveable = 12,
	ObjectClass_Motorbike = 15,
	ObjectClass_Bicycle = 17
}; // ObjectClass

//======================================================================

template<> void writeLE<ibeosdk::ObjectClass>(std::ostream& os, const ibeosdk::ObjectClass& oc);
template<> void writeBE<ibeosdk::ObjectClass>(std::ostream& os, const ibeosdk::ObjectClass& oc);
template<> void readLE<ibeosdk::ObjectClass>(std::istream& is, ibeosdk::ObjectClass& oc);
template<> void readBE<ibeosdk::ObjectClass>(std::istream& is, ibeosdk::ObjectClass& oc);

//======================================================================

namespace luxObjectClass { // prevent polluting of :: namespace.
//======================================================================
/*!\brief LuxObjectClass
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Apr 27, 2016
 *
 * Not all classes may are available on every scanner type.
 * See the documentation of your model.
 *///-------------------------------------------------------------------
enum LuxObjectClass {
	LuxObjectClass_Unclassified = 0,
	LuxObjectClass_UnknownSmall = 1,
	LuxObjectClass_UnknownBig = 2,
	LuxObjectClass_Pedestrian = 3,
	LuxObjectClass_Bike = 4,
	LuxObjectClass_Car = 5,
	LuxObjectClass_Truck = 6,
	LuxObjectClass_Bicycle = 12
}; // RawObjectClass
} // namespace rawObjectClass

//======================================================================

template<> void writeLE<ibeosdk::luxObjectClass::LuxObjectClass>(std::ostream& os, const ibeosdk::luxObjectClass::LuxObjectClass& oc);
template<> void writeBE<ibeosdk::luxObjectClass::LuxObjectClass>(std::ostream& os, const ibeosdk::luxObjectClass::LuxObjectClass& oc);
template<> void readLE<ibeosdk::luxObjectClass::LuxObjectClass>(std::istream& is, ibeosdk::luxObjectClass::LuxObjectClass& oc);
template<> void readBE<ibeosdk::luxObjectClass::LuxObjectClass>(std::istream& is, ibeosdk::luxObjectClass::LuxObjectClass& oc);

//======================================================================

namespace rawObjectClass { // prevent polluting of :: namespace.

//======================================================================
/*!\brief RawObjectClass
 * \author Kai-Uwe von Deylen (kd)
 * \version 0.1
 * \date Sep 2, 2016
 *///-------------------------------------------------------------------
enum RawObjectClass {
	RawObjectClass_Unclassified   =  0,
	RawObjectClass_UnknownSmall   =  1,
	RawObjectClass_UnknownBig     =  2,
	RawObjectClass_Pedestrian     =  3,
	RawObjectClass_Bike           =  4,
	RawObjectClass_Car            =  5,
	RawObjectClass_Truck          =  6,
	RawObjectClass_Overdrivable   = 10,
	RawObjectClass_Underdrivable  = 11,
	RawObjectClass_Bicycle        = 12,
	RawObjectClass_Motorbike      = 15,
	RawObjectClass_Infrastructure = 16
}; // RawObjectClass

//======================================================================

} // namespace rawObjectClass

//======================================================================

template<> void writeLE<rawObjectClass::RawObjectClass>(std::ostream& os, const rawObjectClass::RawObjectClass& oc);
template<> void writeBE<rawObjectClass::RawObjectClass>(std::ostream& os, const rawObjectClass::RawObjectClass& oc);
template<> void readLE<rawObjectClass::RawObjectClass>(std::istream& is, rawObjectClass::RawObjectClass& oc);
template<> void readBE<rawObjectClass::RawObjectClass>(std::istream& is, rawObjectClass::RawObjectClass& oc);

//======================================================================

enum RefPointBoxLocation {
	RPL_CenterOfGravity = 0,
	RPL_FrontLeft       = 1,
	RPL_FrontRight      = 2,
	RPL_RearRight       = 3,
	RPL_RearLeft        = 4,
	RPL_FrontCenter     = 5,
	RPL_RightCenter     = 6,
	RPL_RearCenter      = 7,
	RPL_LeftCenter      = 8,
	RPL_ObjectCenter    = 9,
	RPL_Unknown         = 0xFF
}; // RefPointBoxLocation


//======================================================================

// declare these templates but NOT define them since RefPointBoxLocation will be serialized sometimes as
// 8 sometimes as 16 bit. :(
template<> void writeLE<ibeosdk::RefPointBoxLocation>(std::ostream& os, const ibeosdk::RefPointBoxLocation& oc);
template<> void writeBE<ibeosdk::RefPointBoxLocation>(std::ostream& os, const ibeosdk::RefPointBoxLocation& oc);
template<> void readLE<ibeosdk::RefPointBoxLocation>(std::istream& is, ibeosdk::RefPointBoxLocation& oc);
template<> void readBE<ibeosdk::RefPointBoxLocation>(std::istream& is, ibeosdk::RefPointBoxLocation& oc);

template<int WIDTH> void writeLE(std::ostream& os, const ibeosdk::RefPointBoxLocation rpbl);
template<int WIDTH> void writeBE(std::ostream& os, const ibeosdk::RefPointBoxLocation rpbl);
template<int WIDTH> void readLE(std::istream& is, ibeosdk::RefPointBoxLocation& rpbl);
template<int WIDTH> void readBE(std::istream& is, ibeosdk::RefPointBoxLocation& rpbl);

template<> void writeLE<8>(std::ostream& os, const ibeosdk::RefPointBoxLocation rpbl);
template<> void writeBE<8>(std::ostream& os, const ibeosdk::RefPointBoxLocation rpbl);
template<> void readLE<8>(std::istream& is, ibeosdk::RefPointBoxLocation& rpbl);
template<> void readBE<8>(std::istream& is, ibeosdk::RefPointBoxLocation& rpbl);

template<> void writeLE<16>(std::ostream& os, const ibeosdk::RefPointBoxLocation rpbl);
template<> void writeBE<16>(std::ostream& os, const ibeosdk::RefPointBoxLocation rpbl);
template<> void readLE<16>(std::istream& is, ibeosdk::RefPointBoxLocation& rpbl);
template<> void readBE<16>(std::istream& is, ibeosdk::RefPointBoxLocation& rpbl);

//======================================================================

bool refLocIsRight(const RefPointBoxLocation refLoc);
bool refLocIsCenterY(const RefPointBoxLocation refLoc);
bool refLocIsLeft(const RefPointBoxLocation refLoc);
bool refLocIsFront(const RefPointBoxLocation refLoc);
bool refLocIsCenterX(const RefPointBoxLocation refLoc);
bool refLocIsRear(const RefPointBoxLocation refLoc);

Point2dFloat convertRefPoint(const Point2dFloat refPoint,
                             const float refCourseAngle,
                             const Point2dFloat objectBox,
                             const RefPointBoxLocation fromPos,
                             const RefPointBoxLocation toPos);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OBJECTBASIC_HPP_SEEN
