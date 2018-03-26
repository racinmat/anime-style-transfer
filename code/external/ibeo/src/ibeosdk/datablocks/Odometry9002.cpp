//======================================================================
/*! \file Odometry9002.cpp
 *
 * \copydoc Copyright
 * \author Anil Kumar (ank)
 * \date Mar 24, 2017
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/Odometry9002.hpp>
#include <ibeosdk/datablocks/snippets/ClockType.hpp>
#include <ibeosdk/datablocks/snippets/Timestamp.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

#include <cstring>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<Odometry9002>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<Odometry9002>::dataBlockId = DataTypeId(DataTypeId::DataType_Odometry9002);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<Odometry9002>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<Odometry9002>::dataBlockId, ibeosdk::RegisteredDataBlock<Odometry9002>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<Odometry9002>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================
//======================================================================
//======================================================================


//======================================================================
Odometry9002::Odometry9002()
  : m_steeringAngle(NaN),
    m_steeringWheelAngle(NaN),
    m_steeringWheelAngleVelocity(NaN),
    m_wheelSpeedFL(NaN),
    m_wheelSpeedFR(NaN),
    m_wheelSpeedRL(NaN),
    m_wheelSpeedRR(NaN),
    m_wheelCircumference(NaN),
    m_vehVelocity(NaN),
    m_vehAcceleration(NaN),
    m_vehYawRate(NaN),
    m_timestamp()
{
	memset(m_reserved, 0, sizeof(uint32_t)*nbOfReserved);
}

//======================================================================

Odometry9002::~Odometry9002() {}

//======================================================================

DataTypeId Odometry9002::getDataType() const { return dataBlockId; }

//======================================================================

std::streamsize Odometry9002::getSerializedSize() const
{
	std::streamsize result = 0;
	result += std::streamsize(sizeof(double) * 11);
	result += std::streamsize(this->m_timestamp.getSerializedSize());
	result += std::streamsize(sizeof(UINT32)*Odometry9002::nbOfReserved);
	return result;
}

//======================================================================

bool Odometry9002::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readBE(is, this->m_steeringAngle);
	ibeosdk::readBE(is, this->m_steeringWheelAngle);
	ibeosdk::readBE(is, this->m_steeringWheelAngleVelocity);
	ibeosdk::readBE(is, this->m_wheelSpeedFL);
	ibeosdk::readBE(is, this->m_wheelSpeedFR);
	ibeosdk::readBE(is, this->m_wheelSpeedRL);
	ibeosdk::readBE(is, this->m_wheelSpeedRR);
	ibeosdk::readBE(is, this->m_wheelCircumference);
	ibeosdk::readBE(is, this->m_vehVelocity);
	ibeosdk::readBE(is, this->m_vehAcceleration);
	ibeosdk::readBE(is, this->m_vehYawRate);
	m_timestamp.deserialize(is);

	for (int i=0; i<Odometry9002::nbOfReserved; ++i) {
		ibeosdk::readBE(is, this->m_reserved[i]);
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool Odometry9002::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, m_steeringAngle);
	ibeosdk::writeBE(os, m_steeringWheelAngle);
	ibeosdk::writeBE(os, m_steeringWheelAngleVelocity);
	ibeosdk::writeBE(os, m_wheelSpeedFL);
	ibeosdk::writeBE(os, m_wheelSpeedFR);
	ibeosdk::writeBE(os, m_wheelSpeedRL);
	ibeosdk::writeBE(os, m_wheelSpeedRR);
	ibeosdk::writeBE(os, m_wheelCircumference);
	ibeosdk::writeBE(os, m_vehVelocity);
	ibeosdk::writeBE(os, m_vehAcceleration);
	ibeosdk::writeBE(os, m_vehYawRate);
	m_timestamp.serialize(os);

	for (int i=0; i<Odometry9002::nbOfReserved; ++i) {
		ibeosdk::writeBE(os, m_reserved[i]);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool operator==(const Odometry9002& od1, const Odometry9002& od2)
{
	return    od1.getSteeringAngle() == od2.getSteeringAngle()
	       && od1.getSteeringWheelAngle() == od2.getSteeringWheelAngle()
	       && od1.getSteeringWheelAngleVelocity() == od2.getSteeringWheelAngleVelocity()
	       && od1.getWheelSpeedFL() == od2.getWheelSpeedFL()
	       && od1.getWheelSpeedFR() == od2.getWheelSpeedFR()
	       && od1.getWheelSpeedRL() == od2.getWheelSpeedRL()
	       && od1.getWheelSpeedRR() == od2.getWheelSpeedRR()
	       && od1.getWheelCircumference() == od2.getWheelCircumference()
	       && od1.getVehVelocity() == od2.getVehVelocity()
	       && od1.getVehAcceleration() == od2.getVehAcceleration()
	       && od1.getVehYawRate() == od2.getVehYawRate()
	       && od1.getTimestamp() == od2.getTimestamp();
}

//======================================================================

}// namespace ibeosdk

//======================================================================

