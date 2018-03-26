//======================================================================
/*! \file Odometry9002.hpp
 *
 * \copydoc Copyright
 * \author Anil Kumar (ank)
 * \date March 24, 2017
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_ODOMETRY9002_HPP_SEEN
#define IBEOSDK_ODOMETRY9002_HPP_SEEN

//======================================================================

#include <ibeosdk/Time.hpp>
#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/Timestamp.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class Odometry9002 : public ibeosdk::RegisteredDataBlock<Odometry9002> {
public:
	Odometry9002();
	virtual ~Odometry9002();

public:
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	void setSteeringAngle(const double steeringAngle)  {this->m_steeringAngle = steeringAngle;}
	void setSteeringWheelAngle(const double steeringWheelAngle) {this->m_steeringWheelAngle = steeringWheelAngle;}
	void setSteeringWheelAngleVelocity(double steeringWheelAngleVelocity) {this->m_steeringWheelAngleVelocity = steeringWheelAngleVelocity;}
	void setWheelSpeedFL(const double wheelSpeedFL) {this->m_wheelSpeedFL = wheelSpeedFL;}
	void setWheelSpeedFR(const double wheelSpeedFR) {this->m_wheelSpeedFR = wheelSpeedFR ;}
	void setWheelSpeedRL(const double wheelSpeedRL) {this->m_wheelSpeedRL = wheelSpeedRL;}
	void setWheelSpeedRR(const double wheelSpeedRR) {this->m_wheelSpeedRR = wheelSpeedRR;}
	void setWheelCircumference(const double wheelCircumference) {this-> m_wheelCircumference = wheelCircumference;}
	void setVehVelocity(const double vehVelocity) {this->m_vehVelocity = vehVelocity;}
	void setVehAcceleration(const double vehAcceleration) {this->m_vehAcceleration = vehAcceleration;}
	void setVehYawRate(const double vehYawRate) {this->m_vehYawRate = vehYawRate;}
	void setTimestamp(const Timestamp timestamp) {this->m_timestamp = timestamp;}

public:
	double getSteeringAngle() const {return m_steeringAngle ;}
	double getSteeringWheelAngle() const {return m_steeringWheelAngle ;}
	double getSteeringWheelAngleVelocity() const {return m_steeringWheelAngleVelocity ;}
	double getWheelSpeedFL() const {return m_wheelSpeedFL ;}
	double getWheelSpeedFR() const {return m_wheelSpeedFR ;}
	double getWheelSpeedRL() const {return m_wheelSpeedRL ;}
	double getWheelSpeedRR() const {return m_wheelSpeedRR ;}
	double getWheelCircumference() const {return m_wheelCircumference ;}
	double getVehVelocity() const {return m_vehVelocity ;}
	double getVehAcceleration() const {return m_vehAcceleration ;}
	double getVehYawRate() const {return m_vehYawRate ;}
	const Timestamp& getTimestamp() const {return m_timestamp;}

public:
	static const int nbOfReserved = 4;

protected:
	double m_steeringAngle;
	double m_steeringWheelAngle;
	double m_steeringWheelAngleVelocity;
	double m_wheelSpeedFL;
	double m_wheelSpeedFR;
	double m_wheelSpeedRL;
	double m_wheelSpeedRR;
	double m_wheelCircumference;
	double m_vehVelocity;
	double m_vehAcceleration;
	double m_vehYawRate;
	ibeosdk::Timestamp m_timestamp;

private:
	uint32_t m_reserved[nbOfReserved];
}; // Odometry9002

//======================================================================

bool operator==(const Odometry9002& od1, const Odometry9002& od2);
bool operator!=(const Odometry9002& od1, const Odometry9002& od2);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_ODOMETRY9002_HPP_SEEN

//======================================================================
