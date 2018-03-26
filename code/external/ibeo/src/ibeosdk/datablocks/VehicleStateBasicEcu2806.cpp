//======================================================================
/*! \file VehicleStateBasicEcu2806.cpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Jan 14, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/VehicleStateBasicEcu2806.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<VehicleStateBasicEcu2806>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<VehicleStateBasicEcu2806>::dataBlockId = DataTypeId(DataTypeId::DataType_EcuVehicleStateBasic2806);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<VehicleStateBasicEcu2806>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<VehicleStateBasicEcu2806>::dataBlockId, ibeosdk::RegisteredDataBlock<VehicleStateBasicEcu2806>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<VehicleStateBasicEcu2806>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

VehicleStateBasicEcu2806::VehicleStateBasicEcu2806()
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
    m_reserved3(.0f),
    m_stearRatioCoeff0(.0f),
    m_stearRatioCoeff1(.0f),
    m_stearRatioCoeff2(.0f),
    m_stearRatioCoeff3(.0f)
{}

//======================================================================

VehicleStateBasicEcu2806::~VehicleStateBasicEcu2806() {}

//======================================================================

DataTypeId VehicleStateBasicEcu2806::getDataType() const { return dataBlockId; }

//======================================================================

bool VehicleStateBasicEcu2806::deserialize(std::istream& is, const IbeoDataHeader& dh)
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
	ibeosdk::readBE(is, m_reserved3);
	ibeosdk::readBE(is, m_stearRatioCoeff0);
	ibeosdk::readBE(is, m_stearRatioCoeff1);
	ibeosdk::readBE(is, m_stearRatioCoeff2);
	ibeosdk::readBE(is, m_stearRatioCoeff3);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();}

//======================================================================

bool VehicleStateBasicEcu2806::serialize(std::ostream& os) const
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
	ibeosdk::writeBE(os, m_reserved3);
	ibeosdk::writeBE(os, m_stearRatioCoeff0);
	ibeosdk::writeBE(os, m_stearRatioCoeff1);
	ibeosdk::writeBE(os, m_stearRatioCoeff2);
	ibeosdk::writeBE(os, m_stearRatioCoeff3);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
