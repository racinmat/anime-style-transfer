//======================================================================
/*! \file MetaInformationEcuId.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

#ifndef METAINFORMATIONECUID_HPP_SEEN
#define METAINFORMATIONECUID_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/MetaInformation.hpp>

#include <string>

//======================================================================

namespace ibeosdk {

//======================================================================

class MetaInformationEcuId : public MetaInformation {
public:
	MetaInformationEcuId();
	virtual ~MetaInformationEcuId() {}

public:
	const std::string& getEcuId() const { return m_ecuId; }
	void setEcuId(const std::string& newEcuId);

public: // MetaInformation interface
	virtual uint32_t getSerializedPayloadSize() const;
	virtual bool deserializePayload(std::istream& is, const uint32_t payloadSize);
	virtual bool serializePayload(std::ostream& os) const;

public:
	virtual bool isEqual(const MetaInformationEcuId& other) const;
	virtual bool isNotEqual(const MetaInformationEcuId& other) const;

private:
	std::string m_ecuId;
}; // MetaInformationEcuId

//======================================================================

bool operator==(const MetaInformationEcuId& lhs, const MetaInformationEcuId& rhs);
bool operator!=(const MetaInformationEcuId& lhs, const MetaInformationEcuId& rhs);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // METAINFORMATIONECUID_HPP_SEEN

//======================================================================
