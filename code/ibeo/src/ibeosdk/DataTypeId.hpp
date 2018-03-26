//======================================================================
/*! \file DataTypeId.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Feb 7, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_DATATYPEID_HPP_SEEN
#define IBEOSDK_DATATYPEID_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/Unconvertable.hpp>

#include <ibeosdk/io.hpp>
#include <ibeosdk/bufferIO.hpp>
#include <ibeosdk/Log.hpp>

#include <boost/functional/hash.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class DataTypeId : public ibeosdk::ComparableUnconvertable<uint16_t> {
public:
	//======================================================================
	/*!\brief Ids of some DataTypes. For convenience.
	 *///-------------------------------------------------------------------
	enum DataType {
		DataType_Unknown                          = 0x0000,
		DataType_CanMessage                       = 0x1002, //!< A single can message that has been received via ethernet.
		DataType_FrameEndSeparator                = 0x1100,
		DataType_Command                          = 0x2010,
		DataType_Reply                            = 0x2020,
		DataType_Notification                     = 0x2030, //!< Not implemented in SDK. error and warning messages sent by ibeo LUX/Scala family sensors
		DataType_LuxScan                          = 0x2202, //!< scan data sent by Ibeo LUX/Scala (before B2) family sensors
		DataType_EcuScan                          = 0x2205, //!< scan data sent by Ibeo ECU devices
		DataType_Scan2208                         = 0x2208, //!< Scan data sent by Ibeo Scala B2 and Ibeo MiniLux sensors
		DataType_LuxObjectList                    = 0x2221, //!< objects sent by Ibeo LUX family sensors
		DataType_EcuObjectList                    = 0x2225, //!< objects sent by Ibeo ECU devices
		DataType_ScalaObjectList                  = 0x2270, //!< send by Scala family sensors (starting with B2)
		DataType_ScalaObjectList2271              = 0x2271, //!< send by Scala family sensors (starting with B2)
		DataType_EcuObjectListET                  = 0x2280, //!< send by ECU devices
		DataType_EcuObjectListETDyn               = 0x2281, //!< send by ECU devices
		DataType_ReferencesObjListET              = 0x2290, //!< generate by the Evaluation Suite
		DataType_ReferencesObjListETDyn           = 0x2291, //!< generate by the Evaluation Suite
		DataType_ScalaFpgaRawData                 = 0x2310, //! Uninterpreted raw data from the FPGA
		DataType_Image                            = 0x2403, //!< An image
		DataType_PositionWgs84                    = 0x2604, //!< GPS position
		DataType_OxtsMessage                      = 0x2610, //!<
		DataType_OxtsStatus                       = 0x2611, //!<
		DataType_MeasurementList2821              = 0x2821, //!< Data type that contains a single measurement list.
		DataType_LuxVehicleStateBasic             = 0x2805, //!< send by LUX/Scala
		DataType_EcuVehicleStateBasic2806         = 0x2806, //!< send by ECU
		DataType_EcuVehicleStateBasic             = 0x2807, //!< send by ECU
		DataType_EcuVehicleStateBasic2808         = 0x2808, //!< send by ECU
		DataType_ObjectAssociationList4001        = 0x4001,
		DataType_IdcTrailer                       = 0x6120, //!< Trailer Message in an IDC file
		DataType_FrameIndex                       = 0x6130, //!< Index over IDC file
		DataType_DeviceStatus                     = 0x6301,
		DataType_DeviceStatus6303                 = 0x6303,
		DataType_LogError                         = 0x6400,
		DataType_LogWarning                       = 0x6410,
		DataType_LogNote                          = 0x6420,
		DataType_LogDebug                         = 0x6430,
		DataType_ObjectLabel6503                  = 0x6503,
		DataType_SystemMonitoringCanStatus6700    = 0x6700,
		DataType_SystemMonitoringDeviceStatus6701 = 0x6701,
		DataType_SystemMonitoringSystemStatus6705 = 0x6705,
		DataType_LogPolygonList2dFloat            = 0x6817, //!<List of informational polygons with text label
		DataType_CarriageWayList                  = 0x6970,
		DataType_EventTag                         = 0x7000,
		DataType_ContentSeparator7100             = 0x7100,
		DataType_MetaInformationList7110          = 0x7110,
		DataType_PointCloudGlobal7500             = 0x7500, //!< PointCloudGlobal
		DataType_PointCloudPlane7510              = 0x7510, //!< PointCloudPlane
		DataType_TimeRecord9000                   = 0x9000,
		DataType_GpsImu9001                       = 0x9001,
		DataType_Odometry9002                     = 0x9002,
		DataType_LastId                           = 0xFFFF
	}; // DataType

public:
	//========================================
	/*!\brief Construtor of DataTypeId.
	 *
	 * \param dtId  DataType Id as integer.
	 *///-------------------------------------
	explicit DataTypeId(const uint16_t dtId) : ibeosdk::ComparableUnconvertable<uint16_t>(dtId) {}
	DataTypeId(const DataType dt) : ibeosdk::ComparableUnconvertable<uint16_t>(uint16_t(dt)) {}
	DataTypeId() : ibeosdk::ComparableUnconvertable<uint16_t>(uint16_t(DataType_Unknown)) {}

public:

	bool isSet() const { return (this->m_data != DataType_Unknown); }
	void unSet() { this->m_data = uint16_t(DataType_Unknown); }

public:
	static std::streamsize getSerializedSize() { return sizeof(uint16_t); }

public:
	std::istream& readBE(std::istream& is)
	{
		ibeosdk::readBE(is, this->m_data);
		return is;
	}

	std::ostream& writeBE(std::ostream& os) const
	{
		ibeosdk::writeBE(os, this->m_data);
		return os;
	}

	void readBE(const char*& target)
	{
		ibeosdk::readBE(target, this->m_data);
	}

	void writeBE(char*& target) const
	{
		ibeosdk::writeBE(target, this->m_data);
	}
}; // DataTypeId

//======================================================================

template<>
inline
std::string toHex<DataTypeId>(const DataTypeId t)
{
	return toHex(uint16_t(t));
}


//======================================================================

}// namespace ibeosdk

//======================================================================
//======================================================================
//======================================================================

namespace boost {

//======================================================================

template<>
struct hash<ibeosdk::DataTypeId> {
	std::size_t operator() (ibeosdk::DataTypeId const&  dtId) const
	{
		hash<uint16_t> h;
		return h(dtId);
	}
}; // :hash<DataTypeId>

//======================================================================

} // namespace boost

//======================================================================

#endif // IBEOSDK_DATATYPEID_HPP_SEEN

//======================================================================
