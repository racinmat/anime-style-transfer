//======================================================================
/*! \file RotationMatrix3dFloat.cpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Jan 01, 2016
 * \brief Transformation helper class
 *///-------------------------------------------------------------------

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/TransformationMatrix2dFloat.hpp>
#include <ibeosdk/datablocks/snippets/ObjectEcuEtDyn.hpp>
#include <ibeosdk/datablocks/VehicleStateBasicEcu.hpp>
#include <ibeosdk/datablocks/VehicleStateBasicEcu2808.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class Transform {
public:
	static TransformationMatrix2dFloat getTransformationSystem(const ObjectEcuEtDyn& object);
	static TransformationMatrix2dFloat getTransformationSystem(const VehicleStateBasicEcu& vs);
	static TransformationMatrix2dFloat getTransformationSystem(const VehicleStateBasicEcu2808& vs);

	static TransformationMatrix2dFloat transformToGlobal(const TransformationMatrix2dFloat& ref2Global,
	                                                     const TransformationMatrix2dFloat& rel2ref);
}; // Transform

//======================================================================

} // namespace ibeosdk;

//======================================================================

#endif // TRANSFORM_HPP

//======================================================================
