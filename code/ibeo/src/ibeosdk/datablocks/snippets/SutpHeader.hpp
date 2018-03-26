//======================================================================
/*! \file SutpHeader.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Aug 31, 2016
 *///-------------------------------------------------------------------

//======================================================================

#ifndef IBEOSDK_SUTPHEADER_HPP
#define IBEOSDK_SUTPHEADER_HPP

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/misc/deprecatedwarning.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <istream>
#include <string>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class SutpHeader
 * \brief Implements the SUTP
 * \author Kai-Uwe von Deylen (kd)
 * \version 0.1
 * \date Aug 31, 2016
 *///-------------------------------------------------------------------
class SutpHeader : public Snippet {
public:
	//========================================
	/*!\brief Size of the serialized header
	 *///-------------------------------------
	static std::streamsize getSerializedSize_Static() { return 24; }

public:
	//========================================
	/*!\brief Sutp Header flags bitfield.
	 *///-------------------------------------
	enum SutpFlags {
		Sutp_RawData  = 0x02,  ///< payload is not processed
		Sutp_TsFormat = 0x04   ///< timestamp format (absolute vs relative)
	};

	//========================================
	/*!\brief Enumeration of sutp header
	 *        versions (magic words).
	 *///-------------------------------------
	enum ProtocolVersion {
		ProtocolVersion_01 = 0x53CA
	};

public:

	//========================================
	/*!\brief Default constructor.
	 *
	 * Fills the header with zeroes. For
	 * details, please refer to the
	 * introduction to class IbeoDataHeader.
	 *///-------------------------------------
	SutpHeader ();

	//========================================
	/*!\brief Constructor
	 *
	 * For details, please refer to the sutp
	 * specification.
	 *///-------------------------------------
	SutpHeader (const uint64_t timestamp,
	            const uint16_t version,
	            const uint16_t seqNo,
	            const uint8_t flags,
	            const uint8_t scannerId,
	            const uint16_t dataType,
	            const uint16_t fwVersion,
	            const uint16_t scanNo,
	            const uint16_t fragsTotal,
	            const uint16_t fragNo);

	//========================================
	/*!\brief Destructor
	 *///-------------------------------------
	virtual ~SutpHeader ();

public:
	//========================================
	/*!\brief Equality predicate
	 *///-------------------------------------
	bool operator==(const SutpHeader& other) const;

public:
	//========================================
	/*!\brief Size of the serialized header
	 *///-------------------------------------
	std::streamsize getSerializedSize() const
	{
		return getSerializedSize_Static();
	}

	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	//========================================
	/*!\brief get timestamp
	 *///-------------------------------------
	uint64_t getTimestamp() const { return m_timestamp; }

	//========================================
	/*!\brief get protocol version
	 *///-------------------------------------
	uint16_t getVersion() const { return m_version; }

	//========================================
	/*!\brief get sequence number
	 *///-------------------------------------
	uint16_t getSeqNo() const { return m_seqNo; }

	//========================================
	/*!\brief get flags
	 *///-------------------------------------
	uint8_t  getFlags() const { return m_flags; }

	//========================================
	/*!\brief get scanner id
	 *///-------------------------------------
	uint8_t  getScannerId() const { return m_scannerId; }

	//========================================
	/*!\brief get datatype id
	 *///-------------------------------------
	uint16_t getDatatype() const { return m_dataType; }

	//========================================
	/*!\brief get firmware version
	 *///-------------------------------------
	uint16_t getFwVersion() const { return m_fwVersion; }

	//========================================
	/*!\brief get scan number
	 *///-------------------------------------
	uint16_t getScanNo() const { return m_scanNo; }

	//========================================
	/*!\brief get number of total fragments
	 *///-------------------------------------
	uint16_t getFragsTotal() const { return m_fragsTotal; }

	//========================================
	/*!\brief get number of this fragment
	 *///-------------------------------------
	uint16_t getFragNo() const { return m_fragNo; }

public:
	//========================================
	/*!\brief get timestamp
	 *///-------------------------------------
	void setTimestamp(const uint64_t timestamp) { m_timestamp = timestamp; }

	//========================================
	/*!\brief get protocol version
	 *///-------------------------------------
	void setVersion(const uint16_t version) { m_version = version; }

	//========================================
	/*!\brief get sequence number
	 *///-------------------------------------
	void setSeqNo(const uint16_t seqNo) { m_seqNo = seqNo; }

	//========================================
	/*!\brief get flags
	 *///-------------------------------------
	void setFlags(const uint8_t flags) { m_flags = flags; }

	//========================================
	/*!\brief get scanner id
	 *///-------------------------------------
	void setScannerId(const uint8_t scannerId) { m_scannerId = scannerId; }

	//========================================
	/*!\brief get datatype id
	 *///-------------------------------------
	void setDatatype(const uint16_t datatype) { m_dataType = datatype; }

	//========================================
	/*!\brief get firmware version
	 *///-------------------------------------
	void setFwVersion(const uint16_t fwVersion) { m_fwVersion = fwVersion; }

	//========================================
	/*!\brief get scan number
	 *///-------------------------------------
	void setScanNo(const uint16_t scanNo) { m_scanNo = scanNo; }

	//========================================
	/*!\brief get number of total fragments
	 *///-------------------------------------
	void setFragsTotal(const uint16_t fragsTotal) { m_fragsTotal = fragsTotal; }

	//========================================
	/*!\brief get number of this fragment
	 *///-------------------------------------
	void setFragNo(const uint16_t fragNo) { m_fragNo = fragNo; }

	//========================================
	/*!\brief Returns a formatted string of
	 *        header information.
	 *///-------------------------------------
	std::string prettyPrint() const;

public:
	//========================================
	/*!\brief Reads the next Ibeo data header
	 *///-------------------------------------
	IBEOSDK_DEPRECATED std::istream& read(std::istream& is) { deserialize(is); return is; }

	//========================================
	/*!\brief Writes the Ibeo data header
	 *///-------------------------------------
	IBEOSDK_DEPRECATED std::ostream& write(std::ostream& os) const { serialize(os); return os; }

protected:
	//! Timestamp (32 bit seconds, 32 bit nanoseconds)
	uint64_t m_timestamp;

	//! Magic word and protocol version
	uint16_t m_version;

	//! sequence number of this datagram
	uint16_t m_seqNo;

	//========================================
	/*!\brief Flags.
	 * \sa SutpFlags
	 *///-------------------------------------
	uint8_t  m_flags;

	//! source of payload
	uint8_t  m_scannerId;

	//! DataType of payload
	uint16_t m_dataType;

	//! senders firmware version
	uint16_t m_fwVersion;

	//! corresponding scan id
	uint16_t m_scanNo;

	//! total fragments for complete datatype
	uint16_t m_fragsTotal;

	//! fragment number of this datagram
	uint16_t m_fragNo;
}; // SutpHeader

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SUTPHEADER_HPP

//======================================================================
