//======================================================================
/*! \file MeasurementKey.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 29, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_MEASUREMENTKEY_HPP_SEEN
#define IBEOSDK_MEASUREMENTKEY_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/misc/Unconvertable.hpp>

#include <ibeosdk/io.hpp>
#include <ibeosdk/bufferIO.hpp>

#include <boost/functional/hash.hpp>

//======================================================================

namespace ibeosdk {

class Measurement;

//======================================================================

class MeasurementKey : public ibeosdk::ComparableUnconvertable<uint16_t> {
public:
	static bool compare(const Measurement& m, const MeasurementKey key);

public:
	//======================================================================
	/*!\brief Ids of some DataTypes. For convenience.
	 *///-------------------------------------------------------------------
	enum KeyValues {
		Key_Undefined              =  0,

		VelocityX                  =  1, ///< [m/s] in vehicle coordinate system ("Forward velocity"); (double)
		YawRate                    =  2, ///< [rad/s] in vehicle coordinate system; (double)
		CrossAcceleration          =  4, ///< [m/s^2] in vehicle coordinate system; (double)
		LongitudinalAcceleration   =  6, ///< [m/s^2] in vehicle coordinate system; (double)
		RollAngle                  =  7, ///< [rad]; (double)
		PitchAngle                 =  8, ///< [rad]; (double)

		VerticalAcceleration       = 43, ///< [m/s^2] in vehicle coordinate system; (double)
		PitchRate                  = 44, ///< [rad/s] in vehicle coordinate system; (double)
		RollRate                   = 45, ///< [rad/s] in vehicle coordinate system; (double)

		VelocityNorth              = 49, ///!< [m/s]; (double)
		VelocityUp                 = 50, ///!< [m/s]; (double)
		VelocityWest               = 51, ///!< [m/s]; (double)

		Latitude                   = 60, ///< [rad] WGS84-Latitude; (double)
		Longitude                  = 61, ///< [rad] WGS84-Longitude; (double)
		Altitude                   = 62, ///< [m] Height above sea level; (double)

		YawAngle                   = 67, ///< [rad]; (double)

		UTCHours                   = 70, ///< (UINT8)
		UTCMinutes                 = 71, ///< (UINT8)
		UTCSeconds                 = 72, ///< (UINT8)
		UTCMilliSeconds            = 73, ///< (UINT32)
		UTCDays                    = 79, ///< (UINT8)
		UTCMonths                  = 80, ///< (UINT8)
		UTCYears                   = 81, ///< (UINT16)

		// (Object) Label
		LabelUserData              = 600, //!< (string)

		// Oela
		Oela_EgoLaneProjectionX              = 700,
		Oela_EgoLaneProjectionY              = 701,
		Oela_LateralEgoLaneDistance          = 702,
		Oela_LongitudinalEgoLaneDistance     = 703,
		Oela_EgoLaneFlag                     = 704,

		RearMonitoringFlag                   = 705
	}; // KeyValues

public:
	explicit MeasurementKey(const uint16_t key) : ibeosdk::ComparableUnconvertable<uint16_t>(key) {}

	MeasurementKey(const KeyValues key) : ibeosdk::ComparableUnconvertable<uint16_t>(key) {}
	MeasurementKey(const MeasurementKey& key) : ibeosdk::ComparableUnconvertable<uint16_t>(uint16_t(key)) {}
	MeasurementKey() : ibeosdk::ComparableUnconvertable<uint16_t>(uint16_t(Key_Undefined)) {}

public:
	bool isSet() const { return (this->m_data != 0); }
	void unSet() { this->m_data = uint16_t(0); }

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
}; // MeasurementKey

//======================================================================

}// namespace ibeosdk

//======================================================================
//======================================================================
//======================================================================

namespace boost {

//======================================================================

template<>
struct hash<ibeosdk::MeasurementKey> {
	std::size_t operator() (ibeosdk::MeasurementKey const&  key) const
	{
		hash<uint16_t> h;
		return h(key);
	}
}; // :hash<DataTypeId>

//======================================================================

} // namespace boost

//======================================================================

#endif // IBEOSDK_MEASUREMENTKEY_HPP_SEEN

//======================================================================
