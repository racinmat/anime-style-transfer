//======================================================================
/*! \file ReplyLuxGetStatus.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 10, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_REPLYLUXGETSTATUS_HPP_SEEN
#define IBEOSDK_REPLYLUXGETSTATUS_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/commands/Command.hpp>
#include <ibeosdk/datablocks/DataBlock.hpp>
#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/DataTypeId.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class ReplyLuxGetStatus
 * \brief
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Apr 10, 2015
 *///-------------------------------------------------------------------
class ReplyLuxGetStatus : public ibeosdk::LuxCommandReply<CommandId::CmdLuxGetStatus>,
                          public ibeosdk::DataBlock {
public:
	class Timestamp : public Snippet {
	public:
		virtual bool deserialize(std::istream& is);
		virtual bool serialize(std::ostream& os) const;
		virtual std::streamsize getSerializedSize() const { return std::streamsize(3 * sizeof(uint16_t)); }

	public:
		uint16_t getYYYY() const { return m_YYYY; }
		uint16_t getMMDD() const { return m_MMDD; }
		uint16_t gethhmm() const { return m_hhmm; }

	public:
		uint16_t getYear() const { return m_YYYY; }
		uint8_t getMonth() const { return uint8_t(m_MMDD >> 8); }
		uint8_t getDay() const { return uint8_t(m_MMDD & 0xFF); }
		uint8_t getHour() const { return uint8_t(m_hhmm >> 8); }
		uint8_t getMinute() const { return uint8_t(m_hhmm & 0xFF); }

	public:
		std::string toString() const;

	protected:
		uint16_t m_YYYY;
		uint16_t m_MMDD;
		uint16_t m_hhmm;
	}; // FpgaTimestamp

public:
	static std::string versionToString(const uint16_t version);

public:
	//========================================
	/*!\brief Length of the CommandManagerAppBaseStatus command.
	 *///-------------------------------------
	static const int commandSize = 32;

public:
	ReplyLuxGetStatus();

	//========================================
	/*!\brief Get the DataType of this DataBlock.
	 * \return Always DataType#DataType_Command.
	 *///-------------------------------------
	virtual DataTypeId getDataType() const { return dataType; }

	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization
	 *         of this Command.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const { return std::streamsize(commandSize); }

	//========================================
	/*!\brief Deserialize data from the given stream \a is into
	 *        this CommandManagerAppBaseStatusReply.
	 * \param[in, out] is  Stream that provides the serialized
	 *                     data to fill this CommandManagerAppBaseStatusReply.
	 *                     On exit the \a is get pointer will
	 *                     be behind the read data.
	 * \param[in]      dh  IbeoDataHeader that has been received
	 *                     together with the serialized data in \a is.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Reading the data from the stream has failed.
	 *///-------------------------------------
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);

	//========================================
	/*!\brief Serialize data into the given stream \a os.
	 * \param[out] os  Stream that receive the serialized
	 *                 data from this CommandManagerAppBaseStatusReply.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const;

public:
	virtual bool callListener(DataListenerBase&) const { return false; }

public:
	virtual bool deserializeFromStream(std::istream& is, const IbeoDataHeader& dh) { return deserialize(is, dh); }

public:
	uint16_t getFirmwareVersion() const { return m_firmwareVersion; }
	uint16_t getFpgaVersion() const { return m_fpgaVersion; }
	uint16_t getScannerStatus() const { return m_scannerStatus; }
	uint32_t getReserved1() const { return m_reserved1; }
	uint16_t getTemperature() const { return m_temperature; }
	uint16_t getSerialNumber0() const { return m_serialNumber0; }
	uint16_t getSerialNumber1() const { return m_serialNumber1; }
	uint16_t getReserved2() const { return m_reserved2; }

	Timestamp getFpgaTimeStamp() const { return m_fpgaTimeStamp; }
	Timestamp getDspTimestamp() const { return m_dspTimestamp; }

	float getTemperatureDeg() const { return float(-(m_temperature - 579.2364)/3.63); }

public:
	std::string toString() const;

protected:
	uint16_t m_firmwareVersion;
	uint16_t m_fpgaVersion;
	uint16_t m_scannerStatus;
	uint32_t m_reserved1;
	uint16_t m_temperature;
	uint16_t m_serialNumber0;
	uint16_t m_serialNumber1;
	uint16_t m_reserved2;

	Timestamp m_fpgaTimeStamp;
	Timestamp m_dspTimestamp;
}; // ReplyLuxGetStatus

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_REPLYLUXGETSTATUS_HPP_SEEN

//======================================================================
