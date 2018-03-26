//======================================================================
/*! \file VehicleStateBasicEcu.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 2, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/VehicleStateBasicEcu.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>
#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<VehicleStateBasicEcu>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<VehicleStateBasicEcu>::dataBlockId = DataTypeId(DataTypeId::DataType_EcuVehicleStateBasic);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<VehicleStateBasicEcu>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<VehicleStateBasicEcu>::dataBlockId, ibeosdk::RegisteredDataBlock<VehicleStateBasicEcu>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<VehicleStateBasicEcu>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

VehicleStateBasicEcu::VehicleStateBasicEcu()
  : m_microseconds(0),
    m_timestamp(0),
    m_xPos(0),
    m_yPos(0),
    m_courseAngle(.0f),
    m_longitudinalVelocity(.0f),
    m_yawRate(.0f),
    m_steeringWheelAngle(.0f),
    m_reserved0(.0f),
    m_frontWheelAngle(.0f),
    m_reserved1(0),
    m_vehicleWidth(.0f),
    m_reserved2(.0f),
    m_vehicleFrontToFrontAxle(.0f),
    m_frontAxleToRearAxle(.0f),
    m_rearAxleToVehicleRear(.0f),
    m_rearAxleToOrigin(.0f),
    m_stearRatioCoeff0(.0f),
    m_stearRatioCoeff1(.0f),
    m_stearRatioCoeff2(.0f),
    m_stearRatioCoeff3(.0f),
    m_longitudinalAcceleration(.0f)
{}

//======================================================================

VehicleStateBasicEcu::~VehicleStateBasicEcu() {}

//======================================================================

DataTypeId VehicleStateBasicEcu::getDataType() const { return dataBlockId; }

//======================================================================

bool VehicleStateBasicEcu::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readBE(is, m_microseconds);
	ibeosdk::readBE(is, m_timestamp);
	ibeosdk::readBE(is, m_xPos);
	ibeosdk::readBE(is, m_yPos);
	ibeosdk::readBE(is, m_courseAngle);
	ibeosdk::readBE(is, m_longitudinalVelocity);
	ibeosdk::readBE(is, m_yawRate);
	ibeosdk::readBE(is, m_steeringWheelAngle);
	ibeosdk::readBE(is, m_reserved0);
	ibeosdk::readBE(is, m_frontWheelAngle);
	ibeosdk::readBE(is, m_reserved1);
	ibeosdk::readBE(is, m_vehicleWidth);
	ibeosdk::readBE(is, m_reserved2);
	ibeosdk::readBE(is, m_vehicleFrontToFrontAxle);
	ibeosdk::readBE(is, m_frontAxleToRearAxle);
	ibeosdk::readBE(is, m_rearAxleToVehicleRear);
	ibeosdk::readBE(is, m_rearAxleToOrigin);
	ibeosdk::readBE(is, m_stearRatioCoeff0);
	ibeosdk::readBE(is, m_stearRatioCoeff1);
	ibeosdk::readBE(is, m_stearRatioCoeff2);
	ibeosdk::readBE(is, m_stearRatioCoeff3);
	ibeosdk::readBE(is, m_longitudinalAcceleration);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool VehicleStateBasicEcu::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, m_microseconds);
	ibeosdk::writeBE(os, m_timestamp);
	ibeosdk::writeBE(os, m_xPos);
	ibeosdk::writeBE(os, m_yPos);
	ibeosdk::writeBE(os, m_courseAngle);
	ibeosdk::writeBE(os, m_longitudinalVelocity);
	ibeosdk::writeBE(os, m_yawRate);
	ibeosdk::writeBE(os, m_steeringWheelAngle);
	ibeosdk::writeBE(os, m_reserved0);
	ibeosdk::writeBE(os, m_frontWheelAngle);
	ibeosdk::writeBE(os, m_reserved1);
	ibeosdk::writeBE(os, m_vehicleWidth);
	ibeosdk::writeBE(os, m_reserved2);
	ibeosdk::writeBE(os, m_vehicleFrontToFrontAxle);
	ibeosdk::writeBE(os, m_frontAxleToRearAxle);
	ibeosdk::writeBE(os, m_rearAxleToVehicleRear);
	ibeosdk::writeBE(os, m_rearAxleToOrigin);
	ibeosdk::writeBE(os, m_stearRatioCoeff0);
	ibeosdk::writeBE(os, m_stearRatioCoeff1);
	ibeosdk::writeBE(os, m_stearRatioCoeff2);
	ibeosdk::writeBE(os, m_stearRatioCoeff3);
	ibeosdk::writeBE(os, m_longitudinalAcceleration);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool VehicleStateBasicEcu::operator==( const VehicleStateBasicEcu& other ) const
{
	if (m_microseconds != other.getMicroseconds())
		return false;

	if (m_timestamp != other.getTimestamp())
		return false;

	if (m_xPos != other.getXPos())
		return false;

	if (m_yPos != other.getYPos())
		return false;

	if ( !(fuzzyCompareT<7>(m_courseAngle, other.getCourseAngle())))
		return false;

	if ( !(fuzzyCompareT<7>(m_longitudinalVelocity, other.getLongitudinalVelocity())))
		return false;

	if ( !(fuzzyCompareT<7>(m_yawRate, other.getYawRate())))
		return false;

	if ( !(fuzzyCompareT<7>(m_steeringWheelAngle, other.getSteeringWheelAngle())))
		return false;

	if ( !(fuzzyCompareT<7>(m_reserved0, other.getReserved0())))
		return false;

	if ( !(fuzzyCompareT<7>(m_frontWheelAngle, other.getFrontWheelAngle())))
		return false;

	if (m_reserved1 != other.getReserved1())
		return false;

	if ( !(fuzzyCompareT<7>(m_vehicleWidth, other.getVehicleWidth())))
		return false;

	if ( !(fuzzyCompareT<7>(m_reserved2, other.getReserved2())))
		return false;

	if ( !(fuzzyCompareT<7>(m_vehicleFrontToFrontAxle, other.getVehicleFrontToFrontAxle())))
		return false;

	if ( !(fuzzyCompareT<7>(m_frontAxleToRearAxle, other.getFrontAxleToRearAxle())))
		return false;

	if ( !(fuzzyCompareT<7>(m_rearAxleToVehicleRear, other.getRearAxleToVehicleRear())))
		return false;

	if ( !(fuzzyCompareT<7>(m_rearAxleToOrigin, other.getRearAxleToOrigin())))
		return false;

	if ( !(fuzzyCompareT<7>(m_stearRatioCoeff0, other.getStearRatioCoeff0())))
		return false;

	if ( !(fuzzyCompareT<7>(m_stearRatioCoeff1, other.getStearRatioCoeff1())))
		return false;

	if ( !(fuzzyCompareT<7>(m_stearRatioCoeff2, other.getStearRatioCoeff2())))
		return false;

	if ( !(fuzzyCompareT<7>(m_stearRatioCoeff3, other.getStearRatioCoeff3())))
		return false;

	if ( !(fuzzyCompareT<7>(m_longitudinalAcceleration, other.getLongitudinalAcceleration())))
		return false;
	
	return true;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
