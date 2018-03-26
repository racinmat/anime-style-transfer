//======================================================================
/*! \file BsonT_Object.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef BSONT_OBJECT_HPP_SEEN
#define BSONT_OBJECT_HPP_SEEN

//======================================================================

#ifndef BSONT_HPP_SEEN
#	error "Must be include by BsonT.hpp"
#endif // BSONT_HPP_SEEN

//======================================================================

namespace ibeosdk {
class ObjectEcuEtDyn;

namespace dbaccess {

//======================================================================
/*!\class BsonT<ObjectEcuEtDyn>
 * \brief BsonT representation for a ObjectEcuEtDyn.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
template<>
class BsonT<ObjectEcuEtDyn> : public BsonBase {

public:
	//========================================
	/*!\brief Converts a BSON object to a ObjectEcuEtDyn
	 *        (especially the invariant data)
	 *
	 * Calls BsonT<ObjectEcuEtDyn>::toTimedObj.
	 *
	 * \param[in,out] invariants The invariant data to fill.
	 * \param[in] bsonObjFromInvariants The BSON object from the database query.
	 *///-------------------------------------
	static void createObjectInv(ObjectEcuEtDyn& invariants,
	                            const mongo::BSONObj& bsonObjFromInvariants);

	//========================================
	/*!\brief Converts a BSON object to a ObjectEcuEtDyn
	 *        and merges the invariant data.
	 *
	 * Uses the binary data to deserialize.
	 *
	 * \param[in,out] obj The DataType to fill.
	 * \param[in] bsonObjFromTrajData The BSON object from the database query.
	 * \param[in] invariants The invariant data.
	 *///-------------------------------------
	static void mergeWithInvariants(ObjectEcuEtDyn& obj,
	                                const mongo::BSONObj& bsonObjFromTrajData,
	                                const ObjectEcuEtDyn& invariants);

	//========================================
	/*!\brief Converts a BSON object to a ObjectEcuEtDyn
	 *        (especially the invariant data)
	 *
	 * Uses the binary data to deserialize.
	 *
	 * \param[in,out] obj The DataType to fill.
	 * \param[in] bsonObj The BSON object from the database query.
	 *///-------------------------------------
	static void toTimedObj(ObjectEcuEtDyn& obj,
	                       const mongo::BSONObj& bsonObj);
}; //BsonT_Object

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // BSONT_OBJECT_HPP_SEEN

//======================================================================
