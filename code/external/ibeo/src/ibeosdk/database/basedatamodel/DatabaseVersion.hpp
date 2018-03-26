//======================================================================
/*! \file DatabaseVersion.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 4, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef DATABASEVERSION_HPP_SEEN
#define DATABASEVERSION_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/inttypes.hpp>
#include <string>

//======================================================================

namespace ibeosdk {
namespace dbaccess {
namespace dbVersion {

//========================================
/*!\brief Enumeration of database versions.
 *///-------------------------------------
enum DbVersion {
	DbVersion_Unsupported = 0x0000,
	DbVersion_R3          = 0x0003,
	DbVersion_Latest = DbVersion_R3
};
} // dbVersion

//======================================================================
/*!\class DatabaseVersion
 * \brief Class to store a database version number.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 4, 2016
 *///-------------------------------------------------------------------
class DatabaseVersion {
public:
	//========================================
	/*!\brief Creates database version with given string
	 * \param[in] version Database version as string.
	 *                    Version format: Major.Minor.Patch
	 *///-------------------------------------
	DatabaseVersion(const std::string& version);

public:
	//========================================
	/*!\brief Returns version type
	 * \return Version as DbVersion
	 *///-------------------------------------
	dbVersion::DbVersion getVersionType() const { return m_versionType; }

	//========================================
	/*!\brief Returns major value
	 * \return Value as UINT32
	 *///-------------------------------------
	UINT32 getMajorValue() const { return m_dBVersionMajor; }

	//========================================
	/*!\brief Returns minor value
	 * \return Value as UINT32
	 *///-------------------------------------
	UINT32 getMinorValue() const { return m_dBVersionMinor; }

	//========================================
	/*!\brief Returns patch value
	 * \return Value as UINT32
	 *///-------------------------------------
	UINT32 getPathValue() const { return m_dBVersionPatch; }

private:
	//========================================
	/*!\brief Splits a string to separate number variables
	 *
	 * Format: Major.Minor.Patch
	 * \param[in] version The string to split.
	 *///-------------------------------------
	void splitStringToValues(const std::string& version);

private:
	//========================================
	/*!\brief Major value
	 *///-------------------------------------
	UINT32 m_dBVersionMajor;

	//========================================
	/*!\brief Minor value
	 *///-------------------------------------
	UINT32 m_dBVersionMinor;

	//========================================
	/*!\brief Patch value
	 *///-------------------------------------
	UINT32 m_dBVersionPatch;

	//========================================
	/*!\brief Version type
	 *///-------------------------------------
	dbVersion::DbVersion m_versionType;
}; // DatabaseVersion

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // DATABASEVERSION_HPP_SEEN

//======================================================================
