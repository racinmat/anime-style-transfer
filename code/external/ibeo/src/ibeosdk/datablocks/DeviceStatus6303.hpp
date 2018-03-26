//======================================================================
/*! \file DeviceStatus6303.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 19, 2013
 *///-------------------------------------------------------------------
//======================================================================
#ifndef ELIA_DEVICESTATUS_HPP_SEEN
#define ELIA_DEVICESTATUS_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>

#include <ibeosdk/datablocks/snippets/Version448.hpp>
#include <ibeosdk/datablocks/snippets/SerialNumber.hpp>
#include <ibeosdk/datablocks/snippets/ScannerInfo.hpp>

#include <vector>

#ifdef DEBUG_DEVICESTATUS
# include <iostream>
#endif // DEBUG_DEVICESTATUS

class DeviceStatus6303Test;

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\brief Base class for the data type DeviceStatus6303 0x6303
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Jul 29, 2013
 *///-------------------------------------------------------------------
class DeviceStatus6303 : public RegisteredDataBlock<DeviceStatus6303> {
	friend class ::DeviceStatus6303Test;
public:
	//========================================
	/*!\brief Id of the content in the variable part of the data type.
	 *///-------------------------------------
	// Attention: This value will be saved as uint16_t, hence
	//            do not use ids greater than 0xFFFF
	enum ContentId {
		CID_Illegal         = 0x0000,
		CID_ApdVoltageTable = 0x0001, // float
		CID_NoiseTable      = 0x0002, // uint16_t
		// reserved
		CID_ScalaARMVersion = 0x0100, // uint32_t
		// reserved
		CID_ErrorScalaFPGA  = 0x0110, // uint32_t
		// reserved
		CID_YawOffsetConf   = 0x0120, // int16_t
		CID_YawOffsetCalc   = 0x0121, // int16_t
		CID_VelFactorConf   = 0x0122, // uint16_t
		CID_VelFactorCalc   = 0x0123, // uint16_t

		CID_Dummy1          = 0xFFF1,  // struct Dummy1
		CID_Dummy2          = 0xFFF2,  // struct Dummy2
		CID_Dummy3          = 0xFFF3,  // struct Dummy3
		CID_Dummy4          = 0xFFF4   // struct Dummy4
//		CID_Dummy5          = 0xFFF5,  // struct Dummy5
//		CID_Dummy6          = 0xFFF6,  // struct Dummy6
//		CID_Dummy7          = 0xFFF7,  // struct Dummy7
//		CID_Dummy8          = 0xFFF8,  // struct Dummy8
//		CID_Dummy9          = 0xFFF9,  // struct Dummy9
		// reserved
	}; // ContentId

	//========================================
	/*!\brief Type of the content.
	 *
	 * User defined structs has to be decoded by
	 * the user himself. DeviceStatus6303Base does not
	 * know about that contents.
	 *///-------------------------------------
	enum ElementType {
		ET_Illegal = 0x00,
		ET_UINT8   = 0x01, //!< Content consists of uint8_t
		ET_INT8    = 0x02, //!< Content consists of int8_t
		ET_UINT16  = 0x03, //!< Content consists of uint16_t
		ET_INT16   = 0x04, //!< Content consists of int16_t
		ET_UINT32  = 0x05, //!< Content consists of uint32_t
		ET_INT32   = 0x06, //!< Content consists of int32_t
		ET_FLOAT32 = 0x07, //!< Content consists of float
		ET_STRUCT  = 0x08  //!< Content consists of user defined struct
		// reserved
	}; // ElementType

public:
	//========================================
	/*!\brief An content entry in the variable part
	 *        of this data type.
	 * \author Jan Christian Dittmer (jcd)
	 * \version 0.1
	 * \date Jul 30, 2013
	 *
	 * An object of this class describe a content entry
	 * in the variable part of this data type. Apart from
	 * the ContentId, the ElementType and the number of
	 * used bytes it also contains a pointer to the content
	 * which is located in the content buffer of the
	 * DeviceStatus6303Base object.
	 *///-------------------------------------
	class ContentDescr {
	public:
		//========================================
		/*!\brief Default construtcor.
		 *///-------------------------------------
		ContentDescr()
		  : m_contentId(CID_Illegal),
		    m_elementType(ET_Illegal),
		    m_nbOfBytes(0),
		    m_contentBuffer(NULL)
		{}

		//========================================
		/*!\brief Constructor.
		 * \param[in] cId        ContentId of the content.
		 * \param[in] et         ElementType of the content.
		 * \param[in] nbOfBytes  Size of the content in bytes.
		 *///-------------------------------------
		ContentDescr(const ContentId cId,
		             const ElementType et,
		             const uint8_t nbOfBytes)
		  : m_contentId(cId),
		    m_elementType(et),
		    m_nbOfBytes(nbOfBytes),
		    m_contentBuffer(NULL)
		{}

	public:
		virtual std::streamsize getSerializedSize() const;
		virtual bool serialize(std::ostream& os) const;

	public:
		//========================================
		/*!\brief Get the ContentId of this content.
		 * \return ContentId of this content.
		 *///-------------------------------------
		ContentId getContentId() const { return m_contentId; }

		//========================================
		/*!\brief Get the ElementType of this content.
		 * \return ElementType of this content.
		 *///-------------------------------------
		ElementType getElementType() const { return m_elementType; }

		//========================================
		/*!\brief Get the size of the content in bytes.
		 * \return The size of this content in bytes.
		 *///-------------------------------------
		uint8_t getNbOfBytes() const { return m_nbOfBytes; }

		//========================================
		/*!\brief Get the content buffer pointer.
		 * \return Pointer to the content.
		 *///-------------------------------------
		const char* getContentBuffer() const { return m_contentBuffer; }

		uint8_t getNeededBufferAlignment() const;

	public:
		//========================================
		/*!\brief Set the content buffer pointer.
		 * \param[in] buffer  Set the contentBuffer to the
		 *                    \a buffer. \a buffer contains
		 *                    an address inside the
		 *                    DeviceStatus6303Base's content
		 *                    buffer.
		 *///-------------------------------------
		void setContentBuffer(char* const buffer) { m_contentBuffer = buffer; }

	protected:
		//========================================
		/*!\brief Serialized the content's elements
		 *        into the given buffer.
		 * \tparam          T          Type of the elements of
		 *                             the content.
		 * \param[in, out]  os         Target stream, where the
		 *                             content's elements will
		 *                             be serialized to.
		 * \param[in]       nbOfBytes  Total size of all elements
		 *                             to be written.
		 * \param[in]       elements   Elements of the content
		 *                             to be written.
		 * \return Number of written bytes.
		 *///-------------------------------------
		template<typename T>
		static
		void writeVcElements(std::ostream& os,
		                    const uint32_t nbOfBytes,
		                    const T* const elements);


	protected:
		ContentId m_contentId; //!< ContentId of the content.
		ElementType m_elementType; //!< ElementType of the content
		uint8_t m_nbOfBytes; //!< Size of the content in bytes.
		//========================================
		/*!\brief Pointer to the content itself inside
		 *        the DeviceStatus6303Base object's content
		 *        buffer.
		 *///-------------------------------------
		char* m_contentBuffer;
	}; // ContentDescr

	//========================================
public: // class ContentDescrDeserializer

	class ContentDescrDeserializer : public ContentDescr {
	public:
		ContentDescrDeserializer()
		  : ContentDescr()
		{
			this->setContentBuffer(deserializeBuffer);
		}

	public:
		virtual bool deserialize(std::istream& is);

	protected:
		ContentId readContentId(std::istream& is);
		ElementType readElementType(std::istream& is);

		//========================================
		/*!\brief Deserialized the content's elements
		 *        into the given buffer.
		 * \tparam          T                   Type of the elements of
		 *                                      the content.
		 * \param[in]       is                 Source buffer, where the
		 *                                      content's elements will
		 *                                      be deserialized from.
		 * \param[in]       nbOfElementsToWrite  Total size of all elements
		 *                                      to be write.
		 * \param[out]       elements            Target buffer, where the
		 *                                      content elements will be
		 *                                      read into.
		 * \return Number of read bytes.
		 *///-------------------------------------
		template<typename T>
		static
		void readVcElements(std::istream& is,
		                    const int nbOfElementsToWrite,
		                    T* const elements);

	protected:
		static const int maxSizeOfContent = 255;
		static char deserializeBuffer[maxSizeOfContent+1];
	}; // ContentDescrDeserializer

	//========================================
public: // class UserDefinedStructBase
	//========================================
	/*!\brief Base class for user define structures.
	 *
	 * \remark Derived class have to have a default constructor.
	 *///-------------------------------------
	class UserDefinedStructBase {
	public:
		UserDefinedStructBase(const ContentId contentId);
		~UserDefinedStructBase();

	public:
		virtual bool deserialize(const ContentDescr& cd) = 0;
		virtual bool serialize(char*& buf) const = 0;
		virtual uint8_t getSerializedSize() const = 0;

	public:
		virtual ContentId getContentId() const { return m_contentId; }
		virtual ElementType getElementType() const { return m_elementType; }

	protected:
		const ContentId m_contentId;
		const ElementType m_elementType;
	}; // UserDefinedStructBase

public: // DataBlock Interface
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	/** \addtogroup getElementType
	 *  @{
	 */

	//========================================
	/*!\brief Return the ElementType of the type of the
	 *        given parameter \a t.
	 * \tparam    T  Type that can be handled by the
	 *               variable contents mechanism of
	 *               DeviceStatus6303Base, i.e. for which
	 *               an ElementType is defined.
	 * \param[in] t  Any variable of type T.
	 * \return ElementType corresponding to the type T.
	 *///-------------------------------------
	template<typename T> static inline ElementType getElementType(const T& t);
	/** @}*/

	//========================================
	/*!\brief Determine the number of elements from given
	 *        ElementType and total size.
	 * \param[in] et         ElementType of the elements.
	 * \param[in] nbOfBytes  Total size in bytes.
	 * \return The number of elements of type ElementTypes
	 *         fit into \a nbOfBytes bytes.
	 *///-------------------------------------
	static uint8_t getNbOfElements(const ElementType et, const uint8_t nbOfBytes);

public: // constructor/destructor
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	DeviceStatus6303();

	//========================================
	/*!\brief Destructor
	 *///-------------------------------------
	virtual ~DeviceStatus6303();

private:
	//========================================
	/*!\brief Copy constructor. Forbidden.
	 *
	 * Not implemented and private.
	 *///-------------------------------------
	DeviceStatus6303(const DeviceStatus6303&); // forbidden

	//========================================
	/*!\brief Assignment operator. Forbidden.
	 *
	 * Not implemented and private.
	 *///-------------------------------------
	DeviceStatus6303& operator=(const DeviceStatus6303&); // forbidden

public: // comparison operators for DeviceStatus6303.
	//========================================
	/*!\brief Comparison operator for equality.
	 *
	 * Compare this object with another DeviceStatus6303Base
	 * object \a rhs.
	 *
	 * \param[in] rhs  Other DeviceStatus6303Base
	 *                 this object will be compared
	 *                 with.
	 * \return \c true if the content of this object
	 *         is equal to the content of the other
	 *         object \a rhs. \c false otherwise.
	 *///-------------------------------------
	bool operator==(const DeviceStatus6303& rhs) const;
//	bool operator!=(const DeviceStatus6303& rhs) const;

public: // getter for the fixed part of the DeviceStatus6303 data type
	//========================================
	/*!\brief Get the serial number of the scanner.
	 * \return The serial number of the scanner.
	 *///-------------------------------------
	const SerialNumber<__BIG_ENDIAN>& getSerialNumberOfScanner() const { return this->serialNumberOfScanner; }
//	SerialNumber& getSerialNumberOfScanner() { return this->serialNumberOfScanner; }

	//========================================
	/*!\brief Get the type of the scanner.
	 * \return The type of the scanner.
	 *///-------------------------------------
	uint8_t getScannerType() const { return scannerType; }

	//========================================
	/*!\brief Get the device id of the scanner.
	 * \return The device id of the scanner.
	 *///-------------------------------------
	uint8_t getDeviceId() const { return deviceId; }

	//========================================
	/*!\brief Get the FPGA version of the scanner.
	 * \return The FPGA version of the scanner.
	 *///-------------------------------------
	const Version448<__BIG_ENDIAN>& getFpgaVersion() const { return this->fpgaVersion; }
//	Version448& getFpgaVersion() { return this->fpgaVersion; }

	//========================================
	/*!\brief Get the host version of the scanner.
	 * \return The host version of the scanner.
	 *///-------------------------------------
	const Version448<__BIG_ENDIAN>& getHostVersion() const { return this->hostVersion; }
//	Version448& getHostVersion() { return this->hostVersion; }


	//========================================
	/*!\brief Get the FPGA status register.
	 * \return The FPGA status register.
	 *///-------------------------------------
	uint16_t getFpgaStatusRegister() { return this->fpgaStatusRegister; }

	//========================================
	/*!\brief Get the FPGA operation register.
	 * \return The FPGA operation register.
	 *///-------------------------------------
	uint16_t getFpgaOperationRegister() { return this->fpgaOperationRegister; }

	//========================================
	/*!\brief Get the scan frequency of the scanner in Hz.
	 *
	 * This method has been introduced for convenience.
	 * The DeviceStatus6303 naturally will provide the
	 * reciprocal of the scan frequency, the scanPeriod.
	 * \return The scan frequency.
	 * \sa getScanPeriod
	 *///-------------------------------------
	float getFrequency() const { return 1e6f/float(scanPeriod); }

	//========================================
	/*!\brief Get the scan period of the scanner
	 *        in microseconds.
	 * \return The scan period of the scanner.
	 * \sa getFrequency
	 *///-------------------------------------
	uint32_t getScanPeriod() const { return scanPeriod; }

	//========================================
	/*!\brief Get the sensor APD temperature 0.
	 * \return The sensor APD temperature 0.
	 *///-------------------------------------
	float getSensorTemperatureApd0() const { return sensorTemperatureApd0; }

	//========================================
	/*!\brief Get the sensor APD temperature 1.
	 * \return The sensor APD temperature 1.
	 *///-------------------------------------
	float getSensorTemperatureApd1() const { return sensorTemperatureApd1; }

	//========================================
	/*!\brief Get the minimal APD voltage offset.
	 * \return The minimal APD voltage offset.
	 *///-------------------------------------
	float getMinApdVoltageOffset() const { return minApdVoltageOffset; }

	//========================================
	/*!\brief Get the maximal APD voltage offset.
	 * \return The maximal APD voltage offset.
	 *///-------------------------------------
	float getMaxApdVoltageOffset() const { return maxApdVoltageOffset; }

	//========================================
	/*!\brief Get the noise measurement threshold.
	 * \return The noise measurement threshold.
	 *///-------------------------------------
	uint32_t getNoiseMeasurementThreshold() const { return noiseMeasurementThreshold; }

	//========================================
	/*!\brief Get the reference noise.
	 * \return The reference noise.
	 *///-------------------------------------
	uint16_t getReferenceNoise() const { return referenceNoise; }

public: // setter for the fixed part of the DeviceStatus6303 data type
	//========================================
	/*!\brief Set a new serial number.
	 * \param[in] newSerialNumber  New serial number to be set.
	 *///-------------------------------------
	void setSerialNumber(const SerialNumber<__BIG_ENDIAN> newSerialNumber) { this->serialNumberOfScanner = newSerialNumber; }

	//========================================
	/*!\brief Set a new scanner type.
	 * \param[in] newScannerType  New scanner type to be set.
	 *///-------------------------------------
	void setScannerType(const uint8_t newScannerType) { this->scannerType = newScannerType; }

	//========================================
	/*!\brief Set new device id.
	 * \param[in] newDeviceId  New device id to be set.
	 *///-------------------------------------
	void setDeviceId(const uint8_t newDeviceId) { this->deviceId = newDeviceId; }

	//========================================
	/*!\brief Set new FPGA version.
	 * \param[in] newVersion  New FPGA version to be set.
	 *///-------------------------------------
	void setFpgaVersion(const Version448<__BIG_ENDIAN>& newVersion) { this->fpgaVersion = newVersion; }

	//========================================
	/*!\brief Set new host version.
	 * \param[in] newVersion  New host version to be set.
	 *///-------------------------------------
	void setHostVersion(const Version448<__BIG_ENDIAN>& newVersion) { this->hostVersion = newVersion; }

	//========================================
	/*!\brief Set new scan period.
	 * \param[in] newScanPeriod  New scan period to be set.
	 *///-------------------------------------
	void setScanPeriod(const uint32_t newScanPeriod) { scanPeriod = newScanPeriod; }

	//========================================
	/*!\brief Set new sensor APD temperature 0.
	 * \param[in] newSensorTemperatureApd0  New sensor APD
	 *                                      temperature 0 to be set.
	 *///-------------------------------------
	void setSensorTemperatureApd0(const float newSensorTemperatureApd0) { sensorTemperatureApd0 = newSensorTemperatureApd0; }

	//========================================
	/*!\brief Set new sensor APD temperature 1.
	 * \param[in] newSensorTemperatureApd1  New sensor APD
	 *                                      temperature 1 to be set.
	 *///-------------------------------------
	void setSensorTemperatureApd1(const float newSensorTemperatureApd1) { sensorTemperatureApd1 = newSensorTemperatureApd1; }

	//========================================
	/*!\brief Set new minimal APD voltage offset.
	 * \param[in] newMinApdVoltageOffset  New minimal APD voltage
	 *                                    offset to be set.
	 *///-------------------------------------
	void setMinApdVoltageOffset(const float newMinApdVoltageOffset) { minApdVoltageOffset = newMinApdVoltageOffset; }

	//========================================
	/*!\brief Set new maximal APD voltage offset.
	 * \param[in] newMaxApdVoltageOffset  New maximal APD voltage
	 *                                    offset to be set.
	 *///-------------------------------------
	void setMaxApdVoltageOffset(const float newMaxApdVoltageOffset) { maxApdVoltageOffset = newMaxApdVoltageOffset; }

	//========================================
	/*!\brief Set new noise measurement threshold.
	 * \param[in] newNoiseMeasurementThreshold
	 *    New noise measurement threshold to be set.
	 *///-------------------------------------
	void setNoiseMeasurementThreshold(const uint32_t newNoiseMeasurementThreshold) { noiseMeasurementThreshold = newNoiseMeasurementThreshold; }

	//========================================
	/*!\brief Set new reference noise.
	 * \param[in] newReferenceNoise  New reference noise to be set.
	 *///-------------------------------------
	void setReferenceNoise(const uint16_t newReferenceNoise) { referenceNoise = newReferenceNoise; }

public: // methods to handle the variable contents
	bool addContent(const ContentDescr& cd);

	//========================================
	/*!\brief Add a (variable) content to the DeviceStatus6303Base.
	 * \param[in] cId        Id of the content to be added.
	 * \param[in] et         Type of the elements of the
	 *                       content to be added.
	 * \param[in] nbOfBytes  Size of the content in bytes.
	 * \param[in] alignment  Needed Alignment for the data
	 *                       of this content.
	 * \param[in] cData      Pointer to the content data
	 *                       to be added to the DeviceStatus6303Base.
	 *
	 * \return \c true if the content was added successfully.
	 *         \c false if it failed. Reasons for failure are:
	 *         # The number of content entries has reached
	 *           maxNbOfContentEntries already.
	 *         # The given size (\a nbOfByte) is 0 or negative.
	 *         # The size of the contentBuffer would be exceeded
	 *           by adding the content. (contentBufferSize)
	 *         # A content with the same ContentId already has
	 *           been added.
	 *///-------------------------------------
	bool addContent(const ContentId cId,
	                const ElementType et,
	                const uint8_t nbOfBytes,
	                const uint32_t alignment,
	                const void* cData);

	//========================================
	/*!\brief Add a (variable) content of ElementType ET_STRUCT
	 *        to the DeviceStatus6303Base.
	 *
	 * \param[in] uds        A user defined structure to be added.
	 *
	 * \return \c true if the content was added successfully.
	 *         \c false if it failed. Reasons for failure are:
	 *         # The number of content entries has reached
	 *           maxNbOfContentEntries already.
	 *         # The given size (\a nbOfByte) is 0 or negative.
	 *         # The size of the contentBuffer would be exceeded
	 *           by adding the content. (contentBufferSize)
	 *         # A content with the same ContentId already has
	 *           been added.
	 *///-------------------------------------
	bool addContent(const UserDefinedStructBase& uds);

	//========================================
	/*!\brief Find a content with the given id.
	 * \param[in] cId  Id of the content to be
	 *                 found.
	 * \return On success findContent will return
	 *         the index of the content. If there
	 *         is no such content, -1 will be
	 *         returned.
	 *///-------------------------------------
	int findContent(const ContentId cId);

	//========================================
	/*!\brief Get the data of a (variable) content. User defined
	 *        structs cannot be read by this method.
	 * \tparam     T      Type of the data to be
	 *                    expected.
	 * \param[in]  cId    Id of the content.
	 * \param[out] cData  On exit and on success
	 *                    it will contain the
	 *                    address of the content data.
	 *                    If the operation was not
	 *                    successful \a cData will be
	 *                    NULL.
	 *
	 * \return On success, the size of the content
	 *         in bytes will be returned. If a
	 *         content with the given id \a cId was
	 *         not found -1 will be returned, if
	 *         the ElementType of the content does
	 *         not match \a T, -2 will be returned.
	 * \remark No user defined structs can be read with
	 *         this method. Use
	 *         getContent(const ContentId, UserDefinedStructBase&)
	 *         instead.
	 *///-------------------------------------
	template<typename T>
	inline
	int16_t getContent(const ContentId cId, const T*& cData);


	//========================================
	/*!\brief Get the data of a (variable) content of
	 *        contents that contain an user defined
	 *        struct.
	 * \param[in]       cId    Id of the content.
	 * \param[in, out]  uds    A user defined struct to
	 *                         be filled.
	 *
	 * \return \c true in case of success. \c false
	 *         in case the content could not be found,
	 *         the content is not of element type ET_Struct
	 *         or the read method of \a uds failed.
	 *///-------------------------------------
	bool getContent(const ContentId cId, UserDefinedStructBase& uds);


	//========================================
	/*!\brief Return the vector of all content descriptions.
	 *
	 * The vector received by this method can be used to
	 * traverse through all content entries.
	 * \return The vector that contains all content descriptions.
	 *///-------------------------------------
	const std::vector<ContentDescr>& getContentDescrs() const { return m_contentEntries; }

protected:
	void clear()
	{
		m_contentEntries.clear();
		m_usedBytesInContentData = 0;
	}


public: // static attributes
	//========================================
	/*!\brief Maximal number of (variable) contents
	 *        that can be handled by DeviceStatus6303Base.
	 *///-------------------------------------
	static const unsigned int maxNbOfContentEntries = 30;

	//========================================
	/*!\brief Size of the buffer to hold the data
	 *        of all (variable) contents.
	 *        (Shared buffer)
	 *///-------------------------------------
	static const unsigned int contentBufferSize = 1024;

protected:
	SerialNumber<__BIG_ENDIAN> serialNumberOfScanner; //!< Serial number of the scanner.
	uint8_t scannerType;                  //!< Type of the scanner.
	uint8_t deviceId;                     //!< Device id of the scanner.

	Version448<__BIG_ENDIAN> fpgaVersion;         //!< Version of the FPGA.
	Version448<__BIG_ENDIAN> hostVersion;         //!< Version of the host.

	uint16_t fpgaStatusRegister;          //!< State of the FPGA status register.
	uint16_t fpgaOperationRegister;       //!< State of the FPGA operation register.

	uint32_t scanPeriod;                  //!< Scan period in usec.

	float sensorTemperatureApd0;      //!< Sensor APD temperature 0 in °C.
	float sensorTemperatureApd1;      //!< Sensor APD temperature 1 in °C.
	float minApdVoltageOffset;        //!< Minimal APD voltage offset.
	float maxApdVoltageOffset;        //!< Maximal APD voltage offset.

	uint32_t noiseMeasurementThreshold;   //!< Noise measurement threshold.
	uint16_t referenceNoise;              //!< Reference noise.

protected: // container for variable sections
	//========================================
	/*!\brief Static vector to hold the ContentDescr
	 *        for the up to \a maxNbOfContentEntries
	 *        added (variable) contents).
	 *///-------------------------------------
	std::vector<ContentDescr> m_contentEntries;

	//========================================
	/*!\brief Shared buffer to hold the data of
	 *        all added contents.
	 *///-------------------------------------
	char m_contentData[contentBufferSize];

	//========================================
	/*!\brief Number of bytes already used by
	 *        the added contents.
	 *
	 * This value cannot be larger than
	 * \a contentBufferSize.
	 *///-------------------------------------
	unsigned int m_usedBytesInContentData;
}; // DeviceStatus6303

//======================================================================


/** \addtogroup getElementType
 *  @{
 */

//========================================
/*!\brief Get the #ElementType of type uint8_t.
 *
 * This method is an \c uint8_t specialization of DeviceStatus6303Base::getElementType<T>.
 *
 * \return #ET_UINT8, the ElementType of type uint8_t.
 * \sa DeviceStatus6303Base::getElementType<T>.
 *///-------------------------------------
template<> inline DeviceStatus6303::ElementType DeviceStatus6303::getElementType<uint8_t>(const uint8_t&) { return ET_UINT8; }

//========================================
/*!\brief Get the #ElementType of type int8_t.
 *
 * This method is an \c int8_t specialization of DeviceStatus6303Base::getElementType<T>.
 *
 * \return #ET_INT8, the ElementType of type int8_t.
 * \sa DeviceStatus6303Base::getElementType<T>.
 *///-------------------------------------
template<> inline DeviceStatus6303::ElementType DeviceStatus6303::getElementType<int8_t>(const int8_t&) {return ET_INT8; }

//========================================
/*!\brief Get the #ElementType of type uint16_t.
 *
 * This method is an \c uint16_t specialization of DeviceStatus6303Base::getElementType<T>.
 *
 * \return #ET_UINT16, the ElementType of type uint16_t.
 * \sa DeviceStatus6303Base::getElementType<T>.
 *///-------------------------------------
template<> inline DeviceStatus6303::ElementType DeviceStatus6303::getElementType<uint16_t>(const uint16_t&) { return ET_UINT16; }

//========================================
/*!\brief Get the #ElementType of type int16_t.
 *
 * This method is an \c int16_t specialization of DeviceStatus6303Base::getElementType<T>.
 *
 * \return #ET_INT16, the ElementType of type int16_t.
 * \sa DeviceStatus6303Base::getElementType<T>.
 *///-------------------------------------
template<> inline DeviceStatus6303::ElementType DeviceStatus6303::getElementType<int16_t>(const int16_t&) { return ET_INT16; }

//========================================
/*!\brief Get the #ElementType of type uint32_t.
 *
 * This method is an \c uint32_t specialization of DeviceStatus6303Base::getElementType<T>.
 *
 * \return #ET_UINT32, the ElementType of type uint32_t.
 * \sa DeviceStatus6303Base::getElementType<T>.
 *///-------------------------------------
template<> inline DeviceStatus6303::ElementType DeviceStatus6303::getElementType<uint32_t>(const uint32_t&) { return ET_UINT32; }

//========================================
/*!\brief Get the #ElementType of type int32_t.
 *
 * This method is an \c int32_t specialization of DeviceStatus6303Base::getElementType<T>.
 *
 * \return #ET_INT32, the ElementType of type int32_t.
 * \sa DeviceStatus6303Base::getElementType<T>.
 *///-------------------------------------
template<> inline DeviceStatus6303::ElementType DeviceStatus6303::getElementType<int32_t>(const int32_t&) { return ET_INT32; }

//========================================
/*!\brief Get the #ElementType of type float.
 *
 * This method is an \c float specialization of DeviceStatus6303Base::getElementType<T>.
 *
 * \return #ET_FLOAT32, the ElementType of type float.
 * \sa DeviceStatus6303Base::getElementType<T>.
 *///-------------------------------------
template<> inline DeviceStatus6303::ElementType DeviceStatus6303::getElementType<float>(const float&) { return ET_FLOAT32; }

//========================================
/*!\brief Return the ElementType of the type of the
 *        given parameter \a t.
 * \tparam    T  Type that can be handled by the
 *               variable contents mechanism of
 *               DeviceStatus6303Base, i.e. for which
 *               an ElementType is defined.
 * \param[in] t  Any variable of type T.
 *
 * Since all other case covered by template specializations
 * of DeviceStatus6303Base::getElementType<T> the only element type
 * left is #ET_STRUCT.
 *
 * \return #ET_STRUCT.
 * \sa DeviceStatus6303Base::getElementType<uint8_t>, DeviceStatus6303Base::getElementType<int8_t>,
 *     DeviceStatus6303Base::getElementType<uint16_t>, DeviceStatus6303Base::getElementType<int16_t>,
 *     DeviceStatus6303Base::getElementType<uint32_t>, DeviceStatus6303Base::getElementType<int32_t>,
 *     DeviceStatus6303Base::getElementType<float>
 *///-------------------------------------
template<typename T> DeviceStatus6303::ElementType DeviceStatus6303::getElementType(const T&) { return ET_STRUCT; }

/** @}*/

//======================================================================

template<typename T>
inline
int16_t DeviceStatus6303::getContent(const ContentId cId, const T*& cData)
{
	const int idx = findContent(cId);
	if (idx == -1) {
		cData = NULL;
		return -1;
	}

	const ContentDescr& c = m_contentEntries.at(idx);
	if (c.getElementType() == ET_STRUCT) {
			return -2;
	}

	if (DeviceStatus6303::getElementType<T>(*cData) != c.getElementType()) {
		cData = NULL;
		return -3;
	}

	cData = reinterpret_cast<const T*>(c.getContentBuffer());
	return c.getNbOfBytes();
}

//======================================================================

#ifdef DEBUG_DEVICESTATUS
extern std::ostream& operator<<(std::ostream& os, const DeviceStatus6303::ElementType m_elementType);
extern std::ostream& operator<<(std::ostream& os, const DeviceStatus6303::ContentId m_contentId);
#endif // DEBUG_DEVICESTATUS

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // ELIA_DEVICESTATUS_HPP_SEEN
