//======================================================================
/*! \file MetaInformation.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

#ifndef METAINFORMATION_HPP_SEEN
#define METAINFORMATION_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <ibeosdk/Time.hpp>

#include <boost/shared_ptr.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class MetaInformation : public Snippet {
public:
	enum MetaInformationType {
		MetaInfoType_Unsupported       = 0,
		MetaInfoType_VersionNumber     = 1,
		MetaInfoType_AppBaseConfig     = 2,
		MetaInfoType_AppBaseSyncMethod = 3,
		MetaInfoType_EcuId             = 4,
		MetaInfoType_Keywords          = 5
	};

public:
	static const uint32_t maxStringLength;

public:
	MetaInformation(const MetaInformationType type, const NTPTime timeStamp = NTPTime());
	virtual ~MetaInformation();

public:
	MetaInformationType getType() const { return m_type; }
	void setTimestamp(const NTPTime& time) { m_timeStamp = time; }
	const NTPTime& getTimestamp() const { return m_timeStamp; }

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	virtual uint32_t getSerializedPayloadSize() const = 0;
	virtual bool deserializePayload(std::istream& is, const uint32_t payloadSize) = 0;
	virtual bool serializePayload(std::ostream& os) const = 0;

public:
	virtual bool isEqual(const MetaInformation& other) const;
	virtual bool isNotEqual(const MetaInformation& other) const;

//public:
//	virtual bool write(UINT8*& targetBuffer) const = 0;
//	virtual bool readPayload(const UINT8*& buf) = 0;
//	virtual UINT32 getSerializedSize() const = 0;
//	virtual UINT32 getPayloadSerializedSize() const = 0;
//
//public:
//	virtual bool writeBase(UINT8*& targetBuffer, const UINT32 dataSize) const;
//	virtual bool writeBase(UINT8*& targetBuffer) const;
//	virtual UINT32 getBaseSerializedSize() const;
//
//protected:
//	virtual bool writeString(UINT8*& targetBuffer, const std::string str) const;
//	virtual UINT32 getStringSerializedSize(const std::string& str) const;
//	virtual bool readString(const UINT8*& buf, std::string& str) const;

protected:
	MetaInformationType m_type;
	NTPTime m_timeStamp;
}; // MetaInformation

//======================================================================

typedef boost::shared_ptr<MetaInformation> MetaInformationSPtr;

//======================================================================

bool operator==(const MetaInformation& lhs, const MetaInformation& rhs);
bool operator!=(const MetaInformation& lhs, const MetaInformation& rhs);

//======================================================================

std::ostream& operator<<(std::ostream& os, const MetaInformation::MetaInformationType t);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // METAINFORMATION_HPP_SEEN

//======================================================================
