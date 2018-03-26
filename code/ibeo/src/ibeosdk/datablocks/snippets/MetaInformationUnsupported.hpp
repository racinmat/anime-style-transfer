//======================================================================
/*! \file MetaInformationUnsupported.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

#ifndef METAINFORMATIONUNSUPPORTED_HPP_SEEN
#define METAINFORMATIONUNSUPPORTED_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/MetaInformation.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class MetaInformationUnsupported : public MetaInformation {
public:
	typedef std::vector<uint8_t> DataVector;

public:
	MetaInformationUnsupported(const uint16_t originalType, const NTPTime timestamp, const uint32_t dataSize);
	MetaInformationUnsupported(const uint16_t originalType, const NTPTime timestamp, const DataVector& data);
	virtual ~MetaInformationUnsupported() {}

public:
	const DataVector& getData() const { return m_data; }
	DataVector& getData() { return m_data; }

	uint16_t getOriginalType() const { return m_originalType; }
	void setOriginalType(const uint16_t type) { m_originalType = type; }

public: // Snippet interface
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:  // MetaInformation interface
	virtual uint32_t getSerializedPayloadSize() const  { return uint32_t(m_data.size()); }
	virtual bool deserializePayload(std::istream& is, const uint32_t payloadSize);
	virtual bool serializePayload(std::ostream& os) const;

public:
	virtual bool isEqual(const MetaInformationUnsupported& other) const;
	virtual bool isNotEqual(const MetaInformationUnsupported& other) const;

private:
	uint16_t m_originalType;
	DataVector m_data;
}; // MetaInformationUnsupported

//======================================================================

typedef boost::shared_ptr<MetaInformationUnsupported> MetaInformationUnsupportedSPtr;

//======================================================================

bool operator==(const MetaInformationUnsupported& lhs, const MetaInformationUnsupported& rhs);
bool operator!=(const MetaInformationUnsupported& lhs, const MetaInformationUnsupported& rhs);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // METAINFORMATIONUNSUPPORTED_HPP_SEEN

//======================================================================
