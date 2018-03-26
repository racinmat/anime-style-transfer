//======================================================================
/*! \file MetaInformationVersionNumber.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

#ifndef METAINFORMATIONVERSIONNUMBER_HPP_SEEN
#define METAINFORMATIONVERSIONNUMBER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/MetaInformation.hpp>

#include <string>

//======================================================================

namespace ibeosdk {

//======================================================================

class MetaInformationVersionNumber : public MetaInformation {
public:
	enum SoftwareType{
		SoftwareType_Undefined = 0,
		SoftwareType_Custom = 1,
		SoftwareType_AppBase = 2,
		SoftwareType_EvS = 3,
		SoftwareType_Ilv = 4
	};

	typedef uint32_t VersionNumberType;
	typedef uint16_t VersionPartType;

public:
	MetaInformationVersionNumber();
	virtual ~MetaInformationVersionNumber() {}

public:
	VersionNumberType getVersionNumber() const { return m_versionNumber; }
	void setVersionNumber(const uint32_t newVersion) { m_versionNumber = newVersion; }
	void setVersionNumber(const uint16_t major, const uint16_t minor, const uint16_t patch, const std::string extra = "");

	void setSoftwareType(const SoftwareType type) { m_softwareType = type; }
	SoftwareType getSoftwareType() const { return m_softwareType; }

	VersionPartType getMajorVersion() const;
	VersionPartType getMinorVersion() const;
	VersionPartType getPatchVersion() const;

	const std::string& getExtraString() const { return m_extraString; }
	void setExtraString(const std::string& extra) { m_extraString = extra; }

public: // MetaInformation interface
	virtual uint32_t getSerializedPayloadSize() const;
	virtual bool deserializePayload(std::istream& is, const uint32_t payloadSize);
	virtual bool serializePayload(std::ostream& os) const;

public:
	virtual bool isEqual(const MetaInformationVersionNumber& other) const;
	virtual bool isNotEqual(const MetaInformationVersionNumber& other) const;

private:
	SoftwareType m_softwareType;
	VersionNumberType m_versionNumber;
	std::string m_extraString;
}; // MetaInformationVersionNumber

//======================================================================

bool operator==(const MetaInformationVersionNumber& lhs, const MetaInformationVersionNumber& rhs);
bool operator!=(const MetaInformationVersionNumber& lhs, const MetaInformationVersionNumber& rhs);

//======================================================================

std::ostream& operator<<(std::ostream& os, const MetaInformationVersionNumber::SoftwareType st);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // METAINFORMATIONVERSIONNUMBER_HPP_SEEN

//======================================================================
