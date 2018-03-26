//======================================================================
/*! \file VehicleStateBasicLux.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 2, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/VehicleStateBasicLux.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<VehicleStateBasicLux>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<VehicleStateBasicLux>::dataBlockId = DataTypeId(DataTypeId::DataType_LuxVehicleStateBasic);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<VehicleStateBasicLux>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<VehicleStateBasicLux>::dataBlockId, ibeosdk::RegisteredDataBlock<VehicleStateBasicLux>::create);

class IdcFile;
class IbeoLux;
class IbeoTrackingBox;
class IbeoScala;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<VehicleStateBasicLux>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoLux>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoTrackingBox>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(registerIdInitial)
		)));

//======================================================================

VehicleStateBasicLux::VehicleStateBasicLux()
  : m_timestamp(0),
    m_scanNumber(0),
    m_errorFlags(0),
    m_longitudinalVelocity(0),
    m_steeringWheeAngle(0),
    m_wheelAngle(0),
    m_reserved0(0),
    m_xPos(0),
    m_yPos(0),
    m_courseAngle(0),
    m_timeDiff(0),
    m_xDiff(0),
    m_yDiff(0),
    m_yaw(0),
    m_reserved1(0),
    m_currentYawRate(0),
    m_reserved2(0)
{}

//======================================================================

VehicleStateBasicLux::~VehicleStateBasicLux() {}

//======================================================================

DataTypeId VehicleStateBasicLux::getDataType() const { return dataBlockId; }

//======================================================================

bool VehicleStateBasicLux::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readLE(is, m_timestamp);
	ibeosdk::readLE(is, m_scanNumber);
	ibeosdk::readLE(is, m_errorFlags);
	ibeosdk::readLE(is, m_longitudinalVelocity);
	ibeosdk::readLE(is, m_steeringWheeAngle);
	ibeosdk::readLE(is, m_wheelAngle);
	ibeosdk::readLE(is, m_reserved0);

	ibeosdk::readLE(is, m_xPos);
	ibeosdk::readLE(is, m_yPos);
	ibeosdk::readLE(is, m_courseAngle);

	ibeosdk::readLE(is, m_timeDiff);
	ibeosdk::readLE(is, m_xDiff);
	ibeosdk::readLE(is, m_yDiff);
	ibeosdk::readLE(is, m_yaw);

	ibeosdk::readLE(is, m_reserved1);
	ibeosdk::readLE(is, m_currentYawRate);
	ibeosdk::readLE(is, m_reserved2);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();}

//======================================================================

bool VehicleStateBasicLux::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeLE(os, m_timestamp);
	ibeosdk::writeLE(os, m_scanNumber);
	ibeosdk::writeLE(os, m_errorFlags);
	ibeosdk::writeLE(os, m_longitudinalVelocity);
	ibeosdk::writeLE(os, m_steeringWheeAngle);
	ibeosdk::writeLE(os, m_wheelAngle);
	ibeosdk::writeLE(os, m_reserved0);

	ibeosdk::writeLE(os, m_xPos);
	ibeosdk::writeLE(os, m_yPos);
	ibeosdk::writeLE(os, m_courseAngle);

	ibeosdk::writeLE(os, m_timeDiff);
	ibeosdk::writeLE(os, m_xDiff);
	ibeosdk::writeLE(os, m_yDiff);
	ibeosdk::writeLE(os, m_yaw);

	ibeosdk::writeLE(os, m_reserved1);
	ibeosdk::writeLE(os, m_currentYawRate);
	ibeosdk::writeLE(os, m_reserved2);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
