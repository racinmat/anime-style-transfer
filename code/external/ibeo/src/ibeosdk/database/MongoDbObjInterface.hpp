//======================================================================
/*! \file MongoDbObjInterface.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef MONGODBOBJINTERFACE_HPP_SEEN
#define MONGODBOBJINTERFACE_HPP_SEEN

//======================================================================

#include <ibeosdk/database/CollectionName.hpp>
#include <ibeosdk/database/MongoDbConnection.hpp>
#include <ibeosdk/database/MongoDbUtils.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================
/*!\class MongoDbObjInterface
 * \brief Database interface for querying  ObjectListEcuEtDyn (0x2281).
 *
 * \note Executing queries can raise a DatabaseException, so always surround
 *       the function calls with a try/catch(const DatabaseException& e) block.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Mar 9, 2016
 *///------------------------------------------------------------------
class MongoDbObjInterface {
public:
	//========================================
	/*!\brief Boost shared pointer for MongoDbObjInterface
	 *///-------------------------------------
	typedef boost::shared_ptr<MongoDbObjInterface> DbIfPtr;

public:
	//========================================
	/*!\brief Constructor
	 *
	 * Created MongoDbObjInterface will initially check if given collection is present.
	 * \param[in]  dbConnection                    Already existing database connection.
	 * \param[in]  collectionNameObjectInvariants  Collection to connect to.
	 *                                             In this collection the invariant object
	 *                                             data is stored.
	 * \param[in]  collectionNameObjectTrajData    Collection to connect to.
	 *                                             In this collection the object trajectories
	 *                                             are stored.
	 * \param[out] collectionCheck                 Tells if the collection is present in the database.
	 *///-------------------------------------
	MongoDbObjInterface(const DbConPtr dbConnection,
	                    const CollectionName& collectionNameObjectInvariants,
	                    const CollectionName& collectionNameObjectTrajData,
	                    bool& collectionCheck);

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~MongoDbObjInterface() {}

public:

	//========================================
	/*!\brief Performs a query on a database for dynamic objects which was seen in
	 *        a time range specified by start and end time stamp.
	 *
	 * The matching objects will be inserted into one ObjectListEcuEtDyn.
	 *
	 * Uses MongoDbObjInterface::getObjectList(ObjectListEcuEtDyn& objList,
	 *                                         const mongo::Query& query) internally.
	 *
	 * \param[out] objList    Resulting ObjectListEcuEtDyn with matching objects.
	 * \param[in]  startTime  Begin of the time range.
	 * \param[in]  endTime    End of the time range.
	 * \return \c true if resulting list is not empty, \c false otherwise.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	virtual bool getObjectList(ObjectListEcuEtDyn& objList,
	                           const boost::posix_time::ptime startTime,
	                           const boost::posix_time::ptime endTime);

	//========================================
	/*!\brief Performs a query on a database for dynamic objects which was seen in
	 *        a time range specified by start and end time stamp.
	 *
	 * The matching objects will be inserted into one ObjectListEcuEtDyn.
	 *
	 * \param[out] objList  Resulting ObjectListEcuEtDyn with matching objects.
	 * \param[in]  query    Full query to perform.
	 * \return \c true if resulting list is not empty, \c false otherwise.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	bool getObjectList(ObjectListEcuEtDyn& objList, const mongo::Query& query);

	//========================================
	/*!\brief Performs a query on a database for invariant object data.
	 *
	 * The matching objects will be inserted into one ObjectListEcuEtDyn.
	 *
	 * \param[out] invariants  Resulting invariant data matching with the objId.
	 * \param[in]  objId       Object id to serach for.
	 * \return \c true if query was successful, \c false otherwise.
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	virtual bool getInvariantData(ObjectEcuEtDyn& invariants, const ObjectId32 objId);

public:
	//========================================
	/*!\brief Receive the invariant data of all objects.
	 * \return A MongoDb cursor for the result. Use
	 *         getNextInvariantData to iterate through the
	 *         result.
	 * \sa getNextInvariantData
	 * \throw DatabaseException if an error occurs.
	 *///-------------------------------------
	MongoCursorPtr getAllInvariantData();

	//========================================
	/*!\brief Iterate through all invariant data.
	 * \param[in, out] cursor         Cursor returned by getAllInvariantData.
	 * \param[out]     invariantData  On exit and success contains the invariantData
	 *                                of an object.
	 * \return \c true if there were data available and has
	 *         been stored into \a invariantData. \c false if
	 *         \a invariantData does not contain valid data.
	 *
	 * \sa getAllInvariantData
	 *///-------------------------------------
	bool getNextInvariantData(MongoCursorPtr cursor, ObjectEcuEtDyn& invariantData);

public:
	//========================================
	/*!\brief Holds the database connection.
	 *///-------------------------------------
	MongoDbConnection* const m_mdbConnection;

	//========================================
	/*!\brief Collection with invariant object data.
	 *///-------------------------------------
	CollectionName m_collectionObjectInvariants;

	//========================================
	/*!\brief Collection with object trajectories.
	 *///-------------------------------------
	CollectionName m_collectionObjectTrajData;
}; //MongoDbObjInterface

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // MONGODBOBJINTERFACE_HPP_SEEN

//======================================================================
