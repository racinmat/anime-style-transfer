//======================================================================
/*! \file RotationMatrix3dFloat.cpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Jan 01, 2016
 * \brief Transformation helper class
 *///-------------------------------------------------------------------

//======================================================================

#include "Transform.hpp"

//======================================================================

namespace ibeosdk {

//======================================================================

TransformationMatrix2dFloat Transform::transformToGlobal(const TransformationMatrix2dFloat& ref2Global,
                                                         const TransformationMatrix2dFloat& rel2ref)
{
	return ref2Global * rel2ref;
}

//======================================================================

TransformationMatrix2dFloat Transform::getTransformationSystem(const VehicleStateBasicEcu2808& vs)
{
	return TransformationMatrix2dFloat(vs.getCourseAngle(),
	                                   Point2dFloat(float(vs.getXPosition()),
	                                                float(vs.getYPosition())));
}

//======================================================================

TransformationMatrix2dFloat Transform::getTransformationSystem(const VehicleStateBasicEcu& vs)
{
	return TransformationMatrix2dFloat(vs.getCourseAngle(),
	                                   Point2dFloat(float(vs.getXPos())*10e-4F,
	                                                float(vs.getYPos())*10e-4F));
}

//======================================================================

TransformationMatrix2dFloat Transform::getTransformationSystem(const ObjectEcuEtDyn& object)
{
	return TransformationMatrix2dFloat(object.getCourseAngle(), object.getReferencePointCoord());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
