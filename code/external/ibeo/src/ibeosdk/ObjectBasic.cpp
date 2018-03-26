//======================================================================
/*! \file  ObjectBasic.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Mrz 18, 2015
 *///-------------------------------------------------------------------

#include <ibeosdk/ObjectBasic.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

Point2dFloat convertRefPoint(const Point2dFloat refPoint,
                             const float refCourseAngle,
                             const Point2dFloat objectBox,
                             const RefPointBoxLocation fromPos,
                             const RefPointBoxLocation toPos)
{
	if (fromPos == toPos)
		return refPoint;

	int yShift = refLocIsCenterY(fromPos) ? 0 : (refLocIsLeft(fromPos) ? -1 : 1);
	yShift += refLocIsCenterY(toPos) ? 0 : (refLocIsLeft(toPos) ? 1 : -1);

	int xShift = refLocIsCenterX(fromPos) ? 0 : (refLocIsFront(fromPos) ? -1 : +1);
	xShift += refLocIsCenterX(toPos) ? 0 : (refLocIsFront(toPos) ? +1 : -1);

	Point2dFloat shift(0.5f* (float)xShift * objectBox.getX(), 0.5f* (float) yShift * objectBox.getY());
	shift.rotate(refCourseAngle);

	return refPoint + shift;
}

//======================================================================

bool refLocIsRight(const RefPointBoxLocation refLoc)
{
	switch (refLoc) {
	case RPL_FrontRight:
	case RPL_RearRight:
	case RPL_RightCenter:
		return true;
	default:
		return false;
	} // switch
}

//======================================================================

bool refLocIsCenterY(const RefPointBoxLocation refLoc)
{
	switch (refLoc) {
	case RPL_FrontCenter:
	case RPL_RearCenter:
	case RPL_ObjectCenter:
		return true;
	default:
		return false;
	} // switch
}

//======================================================================


bool refLocIsLeft(const RefPointBoxLocation refLoc)
{
	switch (refLoc) {
	case RPL_FrontLeft:
	case RPL_RearLeft:
	case RPL_LeftCenter:
		return true;
	default:
		return false;
	} // switch
}

//======================================================================


bool refLocIsFront(const RefPointBoxLocation refLoc)
{
	switch (refLoc) {
	case RPL_FrontLeft:
	case RPL_FrontRight:
	case RPL_FrontCenter:
		return true;
	default:
		return false;
	} // switch
}

//======================================================================


bool refLocIsCenterX(const RefPointBoxLocation refLoc)
{
	switch (refLoc) {
	case RPL_RightCenter:
	case RPL_LeftCenter:
	case RPL_ObjectCenter:
		return true;
	default:
		return false;
	} // switch
}

//======================================================================


bool refLocIsRear(const RefPointBoxLocation refLoc)
{
	switch (refLoc) {
	case RPL_RearRight:
	case RPL_RearLeft:
	case RPL_RearCenter:
		return true;
	default:
		return false;
	} // switch
}

//======================================================================

} // namespace ibeosdk

//======================================================================

