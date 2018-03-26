//======================================================================
/*! \file BsonT_Object.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/datamodel/BsonT.hpp>
#include <ibeosdk/datablocks/snippets/ObjectEcuEtDyn.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

void BsonT<ObjectEcuEtDyn>::createObjectInv(ObjectEcuEtDyn& invariants,
                                            const mongo::BSONObj& bsonObjFromInvariants)
{
	toTimedObj(invariants, bsonObjFromInvariants);
}

//======================================================================

void BsonT<ObjectEcuEtDyn>::mergeWithInvariants(ObjectEcuEtDyn& obj,
                                                const mongo::BSONObj& bsonObjFromTrajData,
                                                const ObjectEcuEtDyn& invariants)
{
	assert( &obj != &invariants);

	const mongo::BSONElement be = bsonObjFromTrajData.getField(BsonBase::bsonFtBinaryData);
	int length=0;
	const char* binData = be.binData(length);
	boost::iostreams::stream<boost::iostreams::array_source> strm(binData, length);
	std::istream& binStream = (std::istream&)strm;

	obj.deserialize(binStream);

	// merge values
	obj.setObjectBoxSize(invariants.getObjectBoxSize());
	obj.setObjectId(invariants.getObjectId());
	obj.setClassification(invariants.getClassification());
	obj.setClassificationAge(invariants.getClassificationAge());
	obj.setClassificationQuality(invariants.getClassificationQuality());
	obj.setFlags(invariants.getFlags());
}

//======================================================================

void BsonT<ObjectEcuEtDyn>::toTimedObj(ObjectEcuEtDyn& obj,
                                       const mongo::BSONObj& bo)
{
	const mongo::BSONElement be = bo.getField(bsonFtBinaryData);
	int length=0;
	const char* binData = be.binData(length);
	boost::iostreams::stream<boost::iostreams::array_source> strm(binData,
	    length);
	std::istream& binStream = (std::istream&)strm;

	obj.deserialize(binStream);
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
