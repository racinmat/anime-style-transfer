//======================================================================
/*! \file DbDataBlockReader.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 21, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/database/DbDataBlockReader.hpp>
#include <ibeosdk/devices/DatabaseReplayer.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

DbDataBlockReader::DbDataBlockReader(const dbaccess::ProcessingJob::JobType& type)
  : m_jobType(type),
    m_useProcessedData(false)
{}

//======================================================================

void DbDataBlockReader::notifyListeners(DatabaseReplayer& dbReader, const DataBlock* const db)
{
	dbReader.notifyListeners(db);
}

//======================================================================

bool DbDataBlockReader::findSubStr(const std::string& str, const std::string& pattern)
{
	size_t iIndex = str.find(pattern);
	return (iIndex != std::string::npos);
}

//======================================================================

}// namespace ibeosdk

//======================================================================
