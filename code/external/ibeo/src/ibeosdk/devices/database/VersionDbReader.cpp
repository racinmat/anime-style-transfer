//======================================================================
/*! \file VersionDbReader.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 4, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/database/VersionDbReader.hpp>

#include <ibeosdk/database/DbConnectionFactory.hpp>
#include <ibeosdk/database/basedatamodel/BaseEntity.hpp>
#include <ibeosdk/database/DatabaseException.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

using namespace dbaccess;
using namespace dbField;

//======================================================================

VersionDbReader::VersionDbReader() : m_dbInterface() {}

//======================================================================

void VersionDbReader::connect()
{
	DbConnectionFactory::ScopedConnection sc;
	m_dbInterface = sc.getDbIfPtr();
}

//======================================================================

DatabaseVersion VersionDbReader::getModelVersion()
{
	CollectionName collName(BaseEntity::Content_DataModel + BaseEntity::ContentSuffix_InfoValues);

	if (MongoDbUtils::collectionExists(m_dbInterface->m_mDbConnection->getDbClientConnection(), collName)) {
		mongo::BSONObj obj = m_dbInterface->queryOne(collName, DbQueryOptions().includeField(DbField_Version));
		return DatabaseVersion(obj.getField(MongoDbUtils::convertFieldToName(DbField_Version)).String());
	}
	else {
		throw DatabaseException("Database version could not be detected");
	}
}

//======================================================================

} // namespace ibeosdk

//======================================================================
