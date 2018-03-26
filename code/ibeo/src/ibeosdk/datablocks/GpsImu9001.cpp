//======================================================================
/*!
 * \file GpsImu9001.cpp
 *
 * ---------------------------------------------------------------------
 * \copydoc Copyright
 *
 * \date   07.04.2017
 * \author Peter Oltmann (peo)
 */
//======================================================================

#include <ibeosdk/datablocks/GpsImu9001.hpp>

#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<GpsImu9001>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<GpsImu9001>::dataBlockId = DataTypeId(DataTypeId::DataType_GpsImu9001);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<GpsImu9001>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<GpsImu9001>::dataBlockId, ibeosdk::RegisteredDataBlock<GpsImu9001>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<GpsImu9001>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

GpsImu9001::GpsImu9001()
  : m_latitude(NaN_double),
    m_latitudeSigma(NaN_double),
    m_longitude(NaN_double),
    m_longitudeSigma(NaN_double),
    m_altitude(NaN_double),
    m_altitudeSigma(NaN_double),
    m_courseAngle(NaN_double),
    m_courseAngleSigma(NaN_double),
    m_yawAngle(NaN_double),
    m_yawAngleSigma(NaN_double),
    m_pitchAngle(NaN_double),
    m_pitchAngleSigma(NaN_double),
    m_rollAngle(NaN_double),
    m_rollAngleSigma(NaN_double),
    m_crossAcceleration(NaN_double),
    m_crossAccelerationSigma(NaN_double),
    m_longitudinalAcceleration(NaN_double),
    m_longitudinalAccelerationSigma(NaN_double),
    m_verticalAcceleration(NaN_double),
    m_verticalAccelerationSigma(NaN_double),
    m_velocityNorth(NaN_double),
    m_velocityNorthSigma(NaN_double),
    m_velocityWest(NaN_double),
    m_velocityWestSigma(NaN_double),
    m_velocityUp(NaN_double),
    m_velocityUpSigma(NaN_double),
    m_velocityX(NaN_double),
    m_velocityXSigma(NaN_double),
    m_velocityY(NaN_double),
    m_velocityYSigma(NaN_double),
    m_velocityZ(NaN_double),
    m_velocityZSigma(NaN_double),
    m_rollRate(NaN_double),
    m_rollRateSigma(NaN_double),
    m_yawRate(NaN_double),
    m_yawRateSigma(NaN_double),
    m_pitchRate(NaN_double),
    m_pitchRateSigma(NaN_double),
    m_gpsStatus(NaN_double),
    m_noOfSatellites(0),
    m_gpsDilutionOfPrecisionX(NaN_double),
    m_gpsDilutionOfPrecisionY(NaN_double),
    m_gpsDilutionOfPrecisionZ(NaN_double),
    m_gpsDilutionOfPrecisionVertical(NaN_double),
    m_gpsDilutionOfPrecisionPosition(NaN_double),
    m_timestamp(),
    m_source(Source_Unknown),
    m_insQuality(INSQuality_GPS)
{
	memset(m_reserved, 0, sizeof(UINT8) * nbOfReserved);
}

//======================================================================

GpsImu9001::~GpsImu9001() {}

//======================================================================

GpsImu9001& GpsImu9001::operator=(const GpsImu9001& other)
{
	if (this == &other)
		return *this;

	m_latitude = other.m_latitude;
	m_latitudeSigma = other.m_latitudeSigma;
	m_longitude = other.m_longitude;
	m_longitudeSigma = other.m_longitudeSigma;
	m_altitude = other.m_altitude;
	m_altitudeSigma = other.m_altitudeSigma;
	m_courseAngle = other.m_courseAngle;
	m_courseAngleSigma = other.m_courseAngleSigma;
	m_yawAngle = other.m_yawAngle;
	m_yawAngleSigma = other.m_yawAngleSigma;
	m_pitchAngle = other.m_pitchAngle;
	m_pitchAngleSigma = other.m_pitchAngleSigma;
	m_rollAngle = other.m_rollAngle;
	m_rollAngleSigma = other.m_rollAngleSigma;
	m_crossAcceleration = other.m_crossAcceleration;
	m_crossAccelerationSigma = other.m_crossAccelerationSigma;
	m_longitudinalAcceleration = other.m_longitudinalAcceleration;
	m_longitudinalAccelerationSigma = other.m_longitudinalAccelerationSigma;
	m_verticalAcceleration = other.m_verticalAcceleration;
	m_verticalAccelerationSigma = other.m_verticalAccelerationSigma;
	m_velocityNorth = other.m_velocityNorth;
	m_velocityNorthSigma = other.m_velocityNorthSigma;
	m_velocityWest = other.m_velocityWest;
	m_velocityWestSigma = other.m_velocityWestSigma;
	m_velocityUp = other.m_velocityUp;
	m_velocityUpSigma = other.m_velocityUpSigma;
	m_velocityX = other.m_velocityX;
	m_velocityXSigma = other.m_velocityXSigma;
	m_velocityY = other.m_velocityY;
	m_velocityYSigma = other.m_velocityYSigma;
	m_velocityZ = other.m_velocityZ;
	m_velocityZSigma = other.m_velocityZSigma;
	m_rollRate = other.m_rollRate;
	m_rollRateSigma = other.m_rollRateSigma;
	m_yawRate = other.m_yawRate;
	m_yawRateSigma = other.m_yawRateSigma;
	m_pitchRate = other.m_pitchRate;
	m_pitchRateSigma = other.m_pitchRateSigma;
	m_gpsStatus = other.m_gpsStatus;
	m_noOfSatellites = other.m_noOfSatellites;
	m_gpsDilutionOfPrecisionX = other.m_gpsDilutionOfPrecisionX;
	m_gpsDilutionOfPrecisionY = other.m_gpsDilutionOfPrecisionY;
	m_gpsDilutionOfPrecisionZ = other.m_gpsDilutionOfPrecisionZ;
	m_gpsDilutionOfPrecisionVertical = other.m_gpsDilutionOfPrecisionVertical;
	m_gpsDilutionOfPrecisionPosition = other.m_gpsDilutionOfPrecisionPosition;
	m_timestamp = other.m_timestamp;
	m_source = other.m_source;
	m_insQuality = other.m_insQuality;
	m_timestamp = other.m_timestamp;

	memcpy(m_reserved, other.m_reserved, sizeof(UINT8) * nbOfReserved);

	return *this;
}

//======================================================================

bool GpsImu9001::operator==(const GpsImu9001& other) const
{
	bool isResEqual = true;
	for (int i = 0; i < nbOfReserved; ++i)
	{
		isResEqual = isResEqual && (m_reserved[i] == other.m_reserved[i]);
	}

	return  m_latitude == other.m_latitude &&
	        m_latitudeSigma == other.m_latitudeSigma &&
	        m_longitude == other.m_longitude &&
	        m_longitudeSigma == other.m_longitudeSigma &&
	        m_altitude == other.m_altitude &&
	        m_altitudeSigma == other.m_altitudeSigma &&
	        m_courseAngle == other.m_courseAngle &&
	        m_courseAngleSigma == other.m_courseAngleSigma &&
	        m_yawAngle == other.m_yawAngle &&
	        m_yawAngleSigma == other.m_yawAngleSigma &&
	        m_pitchAngle == other.m_pitchAngle &&
	        m_pitchAngleSigma == other.m_pitchAngleSigma &&
	        m_rollAngle == other.m_rollAngle &&
	        m_rollAngleSigma == other.m_rollAngleSigma &&
	        m_crossAcceleration == other.m_crossAcceleration &&
	        m_crossAccelerationSigma == other.m_crossAccelerationSigma &&
	        m_longitudinalAcceleration == other.m_longitudinalAcceleration &&
	        m_longitudinalAccelerationSigma == other.m_longitudinalAccelerationSigma &&
	        m_verticalAcceleration == other.m_verticalAcceleration &&
	        m_verticalAccelerationSigma == other.m_verticalAccelerationSigma &&
	        m_velocityNorth == other.m_velocityNorth &&
	        m_velocityNorthSigma == other.m_velocityNorthSigma &&
	        m_velocityWest == other.m_velocityWest &&
	        m_velocityWestSigma == other.m_velocityWestSigma &&
	        m_velocityUp == other.m_velocityUp &&
	        m_velocityUpSigma == other.m_velocityUpSigma &&
	        m_velocityX == other.m_velocityX &&
	        m_velocityXSigma == other.m_velocityXSigma &&
	        m_velocityY == other.m_velocityY &&
	        m_velocityYSigma == other.m_velocityYSigma &&
	        m_velocityZ == other.m_velocityZ &&
	        m_velocityZSigma == other.m_velocityZSigma &&
	        m_rollRate == other.m_rollRate &&
	        m_rollRateSigma == other.m_rollRateSigma &&
	        m_yawRate == other.m_yawRate &&
	        m_yawRateSigma == other.m_yawRateSigma &&
	        m_pitchRate == other.m_pitchRate &&
	        m_pitchRateSigma == other.m_pitchRateSigma &&
	        m_gpsStatus == other.m_gpsStatus &&
	        m_noOfSatellites == other.m_noOfSatellites &&
	        m_gpsDilutionOfPrecisionX == other.m_gpsDilutionOfPrecisionX &&
	        m_gpsDilutionOfPrecisionY == other.m_gpsDilutionOfPrecisionY &&
	        m_gpsDilutionOfPrecisionZ == other.m_gpsDilutionOfPrecisionZ &&
	        m_gpsDilutionOfPrecisionVertical == other.m_gpsDilutionOfPrecisionVertical &&
	        m_gpsDilutionOfPrecisionPosition == other.m_gpsDilutionOfPrecisionPosition &&
	        m_timestamp == other.m_timestamp &&
	        m_source == other.m_source &&
	        m_insQuality == other.m_insQuality &&
	        isResEqual;
}

//======================================================================

bool GpsImu9001::operator!=(const GpsImu9001& other) const
{
	return !(*this == other);
}

//======================================================================

DataTypeId GpsImu9001::getDataType() const { return dataBlockId; }

//======================================================================

std::streamsize GpsImu9001::getSerializedSize() const
{
	std::streamsize result = 0;

	result += std::streamsize(sizeof(m_latitude));
	result += std::streamsize(sizeof(m_latitudeSigma));
	result += std::streamsize(sizeof(m_longitude));
	result += std::streamsize(sizeof(m_longitudeSigma));
	result += std::streamsize(sizeof(m_altitude));
	result += std::streamsize(sizeof(m_altitudeSigma));
	result += std::streamsize(sizeof(m_courseAngle));
	result += std::streamsize(sizeof(m_courseAngleSigma));
	result += std::streamsize(sizeof(m_yawAngle));
	result += std::streamsize(sizeof(m_yawAngleSigma));
	result += std::streamsize(sizeof(m_pitchAngle));
	result += std::streamsize(sizeof(m_pitchAngleSigma));
	result += std::streamsize(sizeof(m_rollAngle));
	result += std::streamsize(sizeof(m_rollAngleSigma));
	result += std::streamsize(sizeof(m_crossAcceleration));
	result += std::streamsize(sizeof(m_crossAccelerationSigma));
	result += std::streamsize(sizeof(m_longitudinalAcceleration));
	result += std::streamsize(sizeof(m_longitudinalAccelerationSigma));
	result += std::streamsize(sizeof(m_verticalAcceleration));
	result += std::streamsize(sizeof(m_verticalAccelerationSigma));
	result += std::streamsize(sizeof(m_velocityNorth));
	result += std::streamsize(sizeof(m_velocityNorthSigma));
	result += std::streamsize(sizeof(m_velocityWest));
	result += std::streamsize(sizeof(m_velocityWestSigma));
	result += std::streamsize(sizeof(m_velocityUp));
	result += std::streamsize(sizeof(m_velocityUpSigma));
	result += std::streamsize(sizeof(m_velocityX));
	result += std::streamsize(sizeof(m_velocityXSigma));
	result += std::streamsize(sizeof(m_velocityY));
	result += std::streamsize(sizeof(m_velocityYSigma));
	result += std::streamsize(sizeof(m_velocityZ));
	result += std::streamsize(sizeof(m_velocityZSigma));
	result += std::streamsize(sizeof(m_rollRate));
	result += std::streamsize(sizeof(m_rollRateSigma));
	result += std::streamsize(sizeof(m_yawRate));
	result += std::streamsize(sizeof(m_yawRateSigma));
	result += std::streamsize(sizeof(m_pitchRate));
	result += std::streamsize(sizeof(m_pitchRateSigma));
	result += std::streamsize(sizeof(m_gpsStatus));
	result += std::streamsize(sizeof(m_noOfSatellites));
	result += std::streamsize(sizeof(m_gpsDilutionOfPrecisionX));
	result += std::streamsize(sizeof(m_gpsDilutionOfPrecisionY));
	result += std::streamsize(sizeof(m_gpsDilutionOfPrecisionZ));
	result += std::streamsize(sizeof(m_gpsDilutionOfPrecisionVertical));
	result += std::streamsize(sizeof(m_gpsDilutionOfPrecisionPosition));

	result += m_timestamp.getSerializedSize();
	result += std::streamsize(sizeof(UINT8)); // m_source
	result += std::streamsize(sizeof(UINT8)); // m_insQuality

	result += std::streamsize(sizeof(UINT8) * GpsImu9001::nbOfReserved);

	return result;
}

//======================================================================

bool GpsImu9001::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	ibeosdk::readBE(is, m_latitude);
	ibeosdk::readBE(is, m_latitudeSigma);
	ibeosdk::readBE(is, m_longitude);
	ibeosdk::readBE(is, m_longitudeSigma);
	ibeosdk::readBE(is, m_altitude);
	ibeosdk::readBE(is, m_altitudeSigma);
	ibeosdk::readBE(is, m_courseAngle);
	ibeosdk::readBE(is, m_courseAngleSigma);
	ibeosdk::readBE(is, m_yawAngle);
	ibeosdk::readBE(is, m_yawAngleSigma);
	ibeosdk::readBE(is, m_pitchAngle);
	ibeosdk::readBE(is, m_pitchAngleSigma);
	ibeosdk::readBE(is, m_rollAngle);
	ibeosdk::readBE(is, m_rollAngleSigma);
	ibeosdk::readBE(is, m_crossAcceleration);
	ibeosdk::readBE(is, m_crossAccelerationSigma);
	ibeosdk::readBE(is, m_longitudinalAcceleration);
	ibeosdk::readBE(is, m_longitudinalAccelerationSigma);
	ibeosdk::readBE(is, m_verticalAcceleration);
	ibeosdk::readBE(is, m_verticalAccelerationSigma);
	ibeosdk::readBE(is, m_velocityNorth);
	ibeosdk::readBE(is, m_velocityNorthSigma);
	ibeosdk::readBE(is, m_velocityWest);
	ibeosdk::readBE(is, m_velocityWestSigma);
	ibeosdk::readBE(is, m_velocityUp);
	ibeosdk::readBE(is, m_velocityUpSigma);
	ibeosdk::readBE(is, m_velocityX);
	ibeosdk::readBE(is, m_velocityXSigma);
	ibeosdk::readBE(is, m_velocityY);
	ibeosdk::readBE(is, m_velocityYSigma);
	ibeosdk::readBE(is, m_velocityZ);
	ibeosdk::readBE(is, m_velocityZSigma);
	ibeosdk::readBE(is, m_rollRate);
	ibeosdk::readBE(is, m_rollRateSigma);
	ibeosdk::readBE(is, m_yawRate);
	ibeosdk::readBE(is, m_yawRateSigma);
	ibeosdk::readBE(is, m_pitchRate);
	ibeosdk::readBE(is, m_pitchRateSigma);
	ibeosdk::readBE(is, m_gpsStatus);
	ibeosdk::readBE(is, m_noOfSatellites);
	ibeosdk::readBE(is, m_gpsDilutionOfPrecisionX);
	ibeosdk::readBE(is, m_gpsDilutionOfPrecisionY);
	ibeosdk::readBE(is, m_gpsDilutionOfPrecisionZ);
	ibeosdk::readBE(is, m_gpsDilutionOfPrecisionVertical);
	ibeosdk::readBE(is, m_gpsDilutionOfPrecisionPosition);

	m_timestamp.deserialize(is);

	UINT8 source;
	ibeosdk::readBE(is, source);
	m_source = static_cast<GpsImu9001::Source>(source);

	UINT8 insQuality;
	ibeosdk::readBE(is, insQuality);
	m_insQuality = static_cast<GpsImu9001::INSQuality>(insQuality);

	for (int i = 0; i < GpsImu9001::nbOfReserved; ++i)
	{
		ibeosdk::readBE(is, this->m_reserved[i]);
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool GpsImu9001::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	ibeosdk::writeBE(os, m_latitude);
	ibeosdk::writeBE(os, m_latitudeSigma);
	ibeosdk::writeBE(os, m_longitude);
	ibeosdk::writeBE(os, m_longitudeSigma);
	ibeosdk::writeBE(os, m_altitude);
	ibeosdk::writeBE(os, m_altitudeSigma);
	ibeosdk::writeBE(os, m_courseAngle);
	ibeosdk::writeBE(os, m_courseAngleSigma);
	ibeosdk::writeBE(os, m_yawAngle);
	ibeosdk::writeBE(os, m_yawAngleSigma);
	ibeosdk::writeBE(os, m_pitchAngle);
	ibeosdk::writeBE(os, m_pitchAngleSigma);
	ibeosdk::writeBE(os, m_rollAngle);
	ibeosdk::writeBE(os, m_rollAngleSigma);
	ibeosdk::writeBE(os, m_crossAcceleration);
	ibeosdk::writeBE(os, m_crossAccelerationSigma);
	ibeosdk::writeBE(os, m_longitudinalAcceleration);
	ibeosdk::writeBE(os, m_longitudinalAccelerationSigma);
	ibeosdk::writeBE(os, m_verticalAcceleration);
	ibeosdk::writeBE(os, m_verticalAccelerationSigma);
	ibeosdk::writeBE(os, m_velocityNorth);
	ibeosdk::writeBE(os, m_velocityNorthSigma);
	ibeosdk::writeBE(os, m_velocityWest);
	ibeosdk::writeBE(os, m_velocityWestSigma);
	ibeosdk::writeBE(os, m_velocityUp);
	ibeosdk::writeBE(os, m_velocityUpSigma);
	ibeosdk::writeBE(os, m_velocityX);
	ibeosdk::writeBE(os, m_velocityXSigma);
	ibeosdk::writeBE(os, m_velocityY);
	ibeosdk::writeBE(os, m_velocityYSigma);
	ibeosdk::writeBE(os, m_velocityZ);
	ibeosdk::writeBE(os, m_velocityZSigma);
	ibeosdk::writeBE(os, m_rollRate);
	ibeosdk::writeBE(os, m_rollRateSigma);
	ibeosdk::writeBE(os, m_yawRate);
	ibeosdk::writeBE(os, m_yawRateSigma);
	ibeosdk::writeBE(os, m_pitchRate);
	ibeosdk::writeBE(os, m_pitchRateSigma);
	ibeosdk::writeBE(os, m_gpsStatus);
	ibeosdk::writeBE(os, m_noOfSatellites);
	ibeosdk::writeBE(os, m_gpsDilutionOfPrecisionX);
	ibeosdk::writeBE(os, m_gpsDilutionOfPrecisionY);
	ibeosdk::writeBE(os, m_gpsDilutionOfPrecisionZ);
	ibeosdk::writeBE(os, m_gpsDilutionOfPrecisionVertical);
	ibeosdk::writeBE(os, m_gpsDilutionOfPrecisionPosition);

	m_timestamp.serialize(os);

	ibeosdk::writeBE(os, static_cast<UINT8>(m_source));
	ibeosdk::writeBE(os, static_cast<UINT8>(m_insQuality));

	for (int i = 0; i < GpsImu9001::nbOfReserved; ++i)
	{
		ibeosdk::writeBE(os, m_reserved[i]);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::string GpsImu9001::toString(const Source s)
{
	switch (s)
	{
		case Source_CAN:
			return "CAN";
		case Source_XSensIMU:
			return "XSensIMU";
		case Source_OxtsRt:
			return "OxtsRt";
		case Source_GenesysADMA:
			return "GenesysADMA";
		case Source_SpatialDual:
			return "SpatialDual";
		case Source_TFC:
			return "TFC";
		case Source_VBOX3i:
			return "VBOX3i";
		case Source_Unknown:
			return "Unknown";
		default:
		{
			std::stringstream ss;
			ss << s;
			return "Unknown source: " + ss.str();
		}
	}
}

//======================================================================

} // namespace ibeosdk

//======================================================================
