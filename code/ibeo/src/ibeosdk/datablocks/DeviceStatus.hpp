//======================================================================
/*! \file DeviceStatus.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Nov 4, 2013
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_DEVICESTATUS_HPP_SEEN
#define IBEOSDK_DEVICESTATUS_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>

#include <ibeosdk/datablocks/snippets/Version448.hpp>
#include <ibeosdk/datablocks/snippets/SerialNumber.hpp>
#include <ibeosdk/datablocks/snippets/ScannerInfo.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class DeviceStatus : public RegisteredDataBlock<DeviceStatus> {
public:
	static std::streamsize getSerializedSize_static();

public:
	DeviceStatus();
	virtual ~DeviceStatus();

public:
	const SerialNumber<__LITTLE_ENDIAN>& getSerialNumber() const { return m_serialNumberOfScanner; }
	SerialNumber<__LITTLE_ENDIAN>& getSerialNumber() { return m_serialNumberOfScanner; }

	ScannerType getScannerType() const { return m_scannerType; }
//	UINT8 getReserved0() const { return m_reserved0; }

	const Version448<__LITTLE_ENDIAN>& getFpgaVersion() const { return m_fpgaVersion; }
	Version448<__LITTLE_ENDIAN>& getFpgaVersion() { return m_fpgaVersion; }
	const Version448<__LITTLE_ENDIAN>& getDspVersion() const { return m_dspVersion; }
	Version448<__LITTLE_ENDIAN>& getDspVersion() { return m_dspVersion; }
	const Version448<__LITTLE_ENDIAN>& getHostVersion() const { return m_hostVersion; }
	Version448<__LITTLE_ENDIAN>& getHostVersion() { return m_hostVersion; }

	UINT16 getFpgaModusRegister() const { return m_fpgaModusRegister; }
//	UINT16 getReserved1() const { return m_reserved1; }
	float getSensorTemperature() const { return m_sensorTemperature; }
	float getFrequency() const { return m_frequency; }
	float getApdTableVoltage() const { return m_apdTableVoltage; }
	float getAdaptiveApdVoltage(const uint32_t idx) const { return m_adaptiveApdVoltage.at(idx); }
	float getMinApdVoltageOffset() const { return m_minApdVoltageOffset; }
	float getMaxApdVoltageOffset() const { return m_maxApdVoltageOffset; }
//	UINT16 getReserved2() const { return m_reserved2; }
//	UINT16 getReserved3() const { return m_reserved3; }
	float getNoiseMeasurementThreshold() const { return m_noiseMeasurementThreshold; }
	UINT16 getReferenceNoise() const { return m_referenceNoise; }
	UINT16 getActualNoise(const uint32_t idx) const { return m_actualNoise.at(idx); }
//	UINT16 getReservedA(const int idx) const { return m_reservedA.at(idx); }

public:
	void setScannerType(const ScannerType newScannerType) { m_scannerType = newScannerType; }
//	void setReserved0(const UINT8 newReserved0) { m_reserved0 = newReserved0; }
	void setFpgaModusRegister(const UINT16 newFpgaModusRegister) { m_fpgaModusRegister = newFpgaModusRegister; }
//	void setReserved1(const UINT16 newReserved1) { m_reserved1 = newReserved1; }
	void setSensorTemperature(const float newSensorTemperature) { m_sensorTemperature = newSensorTemperature; }
	void setFrequency(const float newFrequency) { m_frequency = newFrequency; }
	void setApdTableVoltage(const float newApdTableVoltage) { m_apdTableVoltage = newApdTableVoltage; }
	void setAdaptiveApdVoltage(const uint32_t idx, const float newAdaptiveApdVoltage) { m_adaptiveApdVoltage[idx] = newAdaptiveApdVoltage; }
	void setMinApdVoltageOffset(const float newMinApdVoltageOffset) { m_minApdVoltageOffset = newMinApdVoltageOffset; }
	void setMaxApdVoltageOffset(const float newMaxApdVoltageOffset) { m_maxApdVoltageOffset = newMaxApdVoltageOffset; }
//	void setReserved2(const UINT16 newReserved2) { m_reserved2 = newReserved2; }
//	void setReserved3(const UINT16 newReserved3) { m_reserved3 = newReserved3; }
	void setNoiseMeasurementThreshold(const float newNoiseMeasurementThreshold) { m_noiseMeasurementThreshold = newNoiseMeasurementThreshold; }
	void setReferenceNoise(const UINT16 newReferenceNoise) { m_referenceNoise = newReferenceNoise; }
	void setActualNoise(const uint32_t idx, const UINT16 newActualNoise) { m_actualNoise[idx] = newActualNoise; }
//	void setReservedA(const int idx, const UINT16 newReserved) { m_reservedA[idx] = newReserved; }

public:
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	static const unsigned int nbOfApdSectors = 10;
	static const unsigned int nbOfReservedA = 21;

protected:
	SerialNumber<__LITTLE_ENDIAN> m_serialNumberOfScanner; //!< Serial number of the scanner.
	ScannerType m_scannerType;                  //!< Type of the scanner.
	UINT8 m_reserved0;

	Version448<__LITTLE_ENDIAN> m_fpgaVersion;         //!< Version of the FPGA.
	Version448<__LITTLE_ENDIAN> m_dspVersion;          //!< Version of the DSP.
	Version448<__LITTLE_ENDIAN> m_hostVersion;         //!< Version of the host.

	UINT16 m_fpgaModusRegister;           //!< State of the FPGA modus register.
	UINT16 m_reserved1;
	float m_sensorTemperature;            //!< Sensor temperature in °C.
	float m_frequency;      //!< Sensor APD temperature 1 in °C.
	float m_apdTableVoltage;
	std::vector<float> m_adaptiveApdVoltage;
	float m_minApdVoltageOffset;        //!< Minimal APD voltage offset.
	float m_maxApdVoltageOffset;        //!< Maximal APD voltage offset.

	UINT16 m_reserved2;
	UINT16 m_reserved3;

	float m_noiseMeasurementThreshold;   //!< Noise measurement threshold.
	UINT16 m_referenceNoise;              //!< Reference noise.
	std::vector<UINT16> m_actualNoise;
	std::vector<UINT16> m_reservedA;
}; // DeviceStatus

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_DEVICESTATUS_HPP_SEEN

//======================================================================
