//======================================================================
/*! \file ScalaFpgaRawData2310.hpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#ifndef SCALAFPGARAWDATA2310_HPP_SEEN
#define SCALAFPGARAWDATA2310_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/ScalaFpgaRawData.hpp>
#include <ibeosdk/datablocks/Scan2208.hpp>
#include <ibeosdk/Time.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class ScalaFpgaRawData2310 : public RegisteredDataBlock<ScalaFpgaRawData2310> {
public:
	ScalaFpgaRawData2310();
	virtual ~ScalaFpgaRawData2310();

public:
	//========================================
	/*!\brief Extract scan data from the FPGA raw data.
	 * \param[out] scan  Scan2008 container to hold
	 *                   the extracted scan data.
	 *                   Whether the data in \a scan are
	 *                   valid or not depends on the
	 *                   return value.
	 * \return \c true if the extraction was successful,
	 *         \c false if the data in scan are not valid.
	 *///-------------------------------------
	bool extractScan(Scan2208& scan) const;

public: // DataBlock interface
	uint32_t getMagicWord() const { return m_magicWord; }
	uint16_t getInternalDataTypeId() const { return m_internalDataTypeId; }
	uint16_t getReserved00() const { return m_reserved00; }
	uint32_t getPayLoadSize() const { return m_payloadSize; }

	uint32_t getUtcSeconds() const { return m_utcSeconds; }
	uint32_t getFlexrayMeasTime() const { return m_flexrayMeasTime; }
	uint16_t getReserved01() const { return m_reserved01; }
	uint8_t getReserved02() const { return m_reserved02; }
	uint8_t  getFlexrayMasterClock() const { return m_flexrayMasterClock; }
	uint32_t getReserved03() const { return m_reserved03; }
	uint16_t getVersionInfo0() const { return m_versionInfo0; }
	uint16_t getVersionInfo1() const { return m_versionInfo1; }
	const NTPTime& getScanStartTime() const { return m_scanStartTime; }
	const NTPTime& getScanEndTime() const { return m_scanEndTime; }
	const NTPTime& getScanMidTime() const { return m_scanMidTime; }
	uint32_t getReserved04() const { return m_reserved04; }

	const ScalaFpgaRawData& getScalaFpgaRawData() const { return m_fpgaData; }
	ScalaFpgaRawData& getScalaFpgaRawData() { return m_fpgaData; }

public: // DataBlock interface
	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const;

	//========================================
	/*!\brief Get the DataType of this ScalaFpgaRawData DataBlock.
	 *///-------------------------------------
	virtual DataTypeId getDataType() const;

	//========================================
	/*!\brief Deserialize data from the given stream \a is into
	 *        this ScalaFpgaRawData.
	 * \param[in, out] is  Stream that provides the serialized
	 *                     data to fill this ScalaFpgaRawData.
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
	 *                 data from this ScalaFpgaRawData.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const;

protected: // conversion support
	uint16_t rawTo2208Flags(const uint16_t rawFlags) const;
	void setFlag(uint16_t& flagsToBeModified, const uint16_t flagToSet) const;

protected:
	// header
	uint32_t m_magicWord;
	uint16_t m_internalDataTypeId;
	uint16_t m_reserved00;
	uint32_t m_payloadSize;
	
	//payload
	uint32_t m_utcSeconds;
	uint32_t m_flexrayMeasTime;
	uint16_t m_reserved01;
	uint8_t m_reserved02;
	uint8_t m_flexrayMasterClock;
	uint32_t m_reserved03;
	uint16_t m_versionInfo0;
	uint16_t m_versionInfo1;
	NTPTime m_scanStartTime;
	NTPTime m_scanEndTime;
	NTPTime m_scanMidTime;
	uint32_t m_reserved04;
	ScalaFpgaRawData m_fpgaData;
}; // ScalaFpgaRawData2310

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // SCALAFPGARAWDATA2310_HPP_SEEN

//======================================================================
