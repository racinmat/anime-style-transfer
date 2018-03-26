//======================================================================
/*! \file SessionDbReader.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 22, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/database/SessionDbReader.hpp>
#include <ibeosdk/database/DbConnectionFactory.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

using namespace dbaccess;

//======================================================================

SessionDbReader::SessionDbReader()
  : m_dbInterface(),
    m_itr(),
    m_list()
{}

//======================================================================

bool SessionDbReader::connect(const dbaccess::CollectionName& collection)
{
	bool collectionCheck = false;
	DbConnectionFactory::ScopedConnection sc;
	m_dbInterface = sc.getDbIfPtr<Session>(collection, collectionCheck);
	return collectionCheck;
}

//======================================================================

void SessionDbReader::queryData()
{
	m_itr = m_dbInterface->queryData();

	while (!m_itr.end()) {
		m_list.push_back(*m_itr);
		++m_itr;
	}
}

//======================================================================

bool SessionDbReader::querySessionByName(dbaccess::Session& session)
{
	std::stringstream query;
	query << "{ SessionName:" <<"'" << session.getName() << "'" << "}";

	return m_dbInterface->queryOne(query.str(), session);
}

//======================================================================

} // namespace ibeosdk

//======================================================================
