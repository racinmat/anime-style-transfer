////======================================================================
/*! \file MongoDbObjInterface.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/MongoDbObjInterface.hpp>
#include <ibeosdk/database/datamodel/BsonT.hpp>
#include <ibeosdk/database/DatabaseException.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

MongoDbObjInterface::MongoDbObjInterface(const DbConPtr dbConnection,
                                         const CollectionName& collectionNameObjectInvariants,
                                         const CollectionName& collectionNameObjectTrajData,
                                         bool& collectionCheck)
  : m_mdbConnection(dynamic_cast<MongoDbConnection*>(dbConnection.get())),
    m_collectionObjectInvariants(collectionNameObjectInvariants),
    m_collectionObjectTrajData(collectionNameObjectTrajData)
{
	collectionCheck = MongoDbUtils::collectionExists(m_mdbConnection->m_dbConnection, collectionNameObjectInvariants)
	                  && MongoDbUtils::collectionExists(m_mdbConnection->m_dbConnection, collectionNameObjectTrajData);
}

//======================================================================

bool MongoDbObjInterface::getObjectList(ObjectListEcuEtDyn& objList,
                                        const boost::posix_time::ptime startTime,
                                        const boost::posix_time::ptime endTime)
{
	const mongo::BSONObj query = MongoDbUtils::createTimestampQuery(startTime, endTime);
	return getObjectList(objList, query);
}

//======================================================================

bool MongoDbObjInterface::getObjectList(ObjectListEcuEtDyn& objList, const mongo::Query& query)
{
	try {
		std::vector<ObjectEcuEtDyn>& internalList = objList.getObjects();
		internalList.clear();

		boost::shared_ptr<mongo::DBClientCursor> queryDynamicObjects =  boost::shared_ptr<mongo::DBClientCursor>(
			m_mdbConnection->m_dbConnection.query(m_collectionObjectTrajData, query, 0,0,NULL,0));

		ObjectEcuEtDyn invariants;
		ObjectEcuEtDyn obj;

		while (queryDynamicObjects->more()) {

			mongo::BSONObj bsonObjFromTrajData = queryDynamicObjects->next();

			const mongo::BSONElement be = bsonObjFromTrajData.getField(BsonBase::bsonFtBinaryData);
			int binDatLen = 0;
			const char* binDatPtr = be.binData(binDatLen);
			boost::iostreams::array_source binSrc(binDatPtr, binDatLen);
			boost::iostreams::stream<boost::iostreams::array_source> binSrcStream(binSrc);

			static IbeoDataHeader hdr;
			hdr.deserialize(binSrcStream);

			const ObjectId32 objId = ObjectId32(bsonObjFromTrajData.getField(BsonBase::bsonFtObjId).Int());

			if (!getInvariantData(invariants, objId)) {
				logError << "Could not get invariants data for object id: " << objId << std::endl;
				break;
			}

			BsonT<ObjectEcuEtDyn>::mergeWithInvariants(obj, bsonObjFromTrajData, invariants);
			internalList.push_back(obj);

			const boost::posix_time::ptime objTrajDatasTimestamp = MongoDbUtils::convertToBoostTimestamp(bsonObjFromTrajData.getField(BsonBase::bsonFtTimestamp).Date());

			objList.setTimestamp(NTPTime(objTrajDatasTimestamp));

		} // while
		return objList.getNbOfObjects() != 0;
	}
	catch (const mongo::AssertionException& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	}
}

//======================================================================

bool MongoDbObjInterface::getInvariantData(ObjectEcuEtDyn& invariants, const ObjectId32 objId)
{
	try {
		mongo::BSONObjBuilder bob;
		bob.append(BsonBase::bsonFtId, int32_t(objId));
		mongo::BSONObj bsonObjFromInvariants = m_mdbConnection->m_dbConnection.findOne(m_collectionObjectInvariants, bob.obj());
		if (!bsonObjFromInvariants.isEmpty()) {
			BsonT<ObjectEcuEtDyn>::createObjectInv(invariants, bsonObjFromInvariants);
			return true;
		} // if
	} // try
	catch (const mongo::AssertionException& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	} // catch
	return false;
}

//======================================================================

MongoCursorPtr MongoDbObjInterface::getAllInvariantData()
{
	const mongo::Query query;
	try {
		return MongoCursorPtr(m_mdbConnection->m_dbConnection.query(m_collectionObjectInvariants, query, 0,0,NULL,0));
	}
	catch (const mongo::AssertionException& e) {
		logError << e.what() << std::endl;
		throw DatabaseException(e.what());
	}
}

//======================================================================

bool MongoDbObjInterface::getNextInvariantData(MongoCursorPtr cursor, ObjectEcuEtDyn& invariantData)
{
	if (cursor->more()) {
		try {
			mongo::BSONObj bsonObjFromInvariants = cursor->next();
			if ( !bsonObjFromInvariants.isEmpty()) {
				BsonT<ObjectEcuEtDyn>::createObjectInv(invariantData, bsonObjFromInvariants);
				return true;
			} // if
		}
		catch (const std::exception& e) {
			logError<<e.what() << std::endl;
		} // catch
	}
	return false;
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
