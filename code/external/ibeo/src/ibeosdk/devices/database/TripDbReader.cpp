//======================================================================
/*! \file TripDbReader.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 18, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/devices/database/TripDbReader.hpp>
#include <ibeosdk/database/DbConnectionFactory.hpp>

#include <boost/algorithm/string/predicate.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

using namespace dbaccess;

//======================================================================

TripDbReader::TripDbReader()
    : m_dbInterface(),
      m_procInterface(),
      m_procItr()
{}

//======================================================================

void TripDbReader::connect()
{
	DbConnectionFactory::ScopedConnection sc;
	m_dbInterface = sc.getDbIfPtr();
}

//======================================================================

bool TripDbReader::getTrip(Trip& trip)
{
	CollectionName cInfoValues = CollectionName(trip.getFullDbName() + BaseEntity::ContentSuffix_InfoValues);
	CollectionName cProcessingJobs = CollectionName(trip.getFullDbName() + BaseEntity::ContentSuffix_ProcessingJobs);

	if(MongoDbUtils::collectionExists(m_dbInterface->m_mDbConnection->getDbClientConnection(), cInfoValues)) {
		mongo::BSONObj obj = m_dbInterface->queryOne(cInfoValues);

		if (obj.isEmpty()) {
			return false;
		}
		BsonT<Trip>::createTripInfo(trip, obj);
	}
	else {
		return false;
	}

	bool collectionCheck = false;
	DbConnectionFactory::ScopedConnection sc;
	m_procInterface = sc.getDbIfPtr<ProcessingJob>(cProcessingJobs, collectionCheck);

	if(collectionCheck) {
		m_procItr = m_procInterface->queryData();

		while (!m_procItr.end()) {
			trip.addProcessingJob(*m_procItr);
			++m_procItr;
		}
	}

	return true;
}

//======================================================================

TripList TripDbReader::getTrips()
{
	TripList tripList;

	std::list<std::string> dbNames = m_dbInterface->getDatabaseNames();

	std::list<std::string>::const_iterator it = dbNames.begin();
	for (; it != dbNames.end(); ++it) {
		if (boost::algorithm::starts_with(*it, BaseEntity::Content_Trip)) {
			Trip newTrip(it->substr(BaseEntity::Content_Trip.length()));

			if (getTrip(newTrip))
				tripList.push_back(newTrip);
		}
	} // for

	return tripList;
}

//======================================================================

void TripDbReader::fillTrips(dbaccess::TripList& list)
{
	TripList::iterator itr = list.begin();
	for (; itr != list.end(); ++itr)
		getTrip(*itr);
}

//======================================================================

} // namespace ibeosdk

//======================================================================
