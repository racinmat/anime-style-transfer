//======================================================================
/*! \file DatabaseVersion.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 4, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/basedatamodel/DatabaseVersion.hpp>
#include <sstream>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

using namespace dbVersion;

//======================================================================

DatabaseVersion::DatabaseVersion(const std::string& version)
   : m_dBVersionMajor(0),
     m_dBVersionMinor(0),
     m_dBVersionPatch(0),
     m_versionType(DbVersion_Unsupported)
{
	splitStringToValues(version);
	m_versionType = DbVersion(m_dBVersionMajor);
}

//======================================================================

void DatabaseVersion::splitStringToValues(const std::string& version)
{
	std::stringstream ss(version);
	ss >> m_dBVersionMajor;

	ss.get(); //skip point
	ss >> m_dBVersionMinor;

	ss.get(); //skip point
	ss >> m_dBVersionPatch;
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
