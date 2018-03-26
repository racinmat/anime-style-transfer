//======================================================================
/*! \file MetaInformationAppBaseConfig.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

#ifndef METAINFORMATIONAPPBASECONFIG_HPP_SEEN
#define METAINFORMATIONAPPBASECONFIG_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/MetaInformation.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class MetaInformationAppBaseConfig : public MetaInformation {
public:
	MetaInformationAppBaseConfig();
	virtual ~MetaInformationAppBaseConfig() {}

public:
	const std::string& getAppBaseConfig() const { return m_appBaseConfig; }
	void setAppBaseConfig(const std::string& newAppBaseConfig);

public:
	virtual uint32_t getSerializedPayloadSize() const;
	virtual bool deserializePayload(std::istream& is, const uint32_t payloadSize);
	virtual bool serializePayload(std::ostream& os) const;

public:
	virtual bool isEqual(const MetaInformationAppBaseConfig& other) const;
	virtual bool isNotEqual(const MetaInformationAppBaseConfig& other) const;

private:
	std::string m_appBaseConfig;
}; // MetaInformationAppBaseConfig

//======================================================================

bool operator==(const MetaInformationAppBaseConfig& lhs, const MetaInformationAppBaseConfig& rhs);
bool operator!=(const MetaInformationAppBaseConfig& lhs, const MetaInformationAppBaseConfig& rhs);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // METAINFORMATIONAPPBASECONFIG_HPP_SEEN

//======================================================================
