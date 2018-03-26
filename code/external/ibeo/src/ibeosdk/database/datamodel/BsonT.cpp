//======================================================================
/*! \file BsonT.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/datamodel/BsonT.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

const std::string BsonBase::bsonFtId         = "ID";
const std::string BsonBase::bsonFtMongoId    = "_id";
const std::string BsonBase::bsonFtObjId      = "ObjectID";
const std::string BsonBase::bsonFtType       = "Type";
const std::string BsonBase::bsonFtTimestamp  = "Timestamp";
const std::string BsonBase::bsonFtBinaryData = "BinaryData";
const std::string BsonBase::bsonFtFile       = "File";
const std::string BsonBase::bsonFtVersion    = "Version";
const std::string BsonBase::bsonFtClass      = "Class";

const std::string BsonBase::bsonFtX          = "X";
const std::string BsonBase::bsonFtY          = "Y";
const std::string BsonBase::bsonFtZ          = "Z";
const std::string BsonBase::bsonFtLat        = "Latitude";
const std::string BsonBase::bsonFtLon        = "Longitude";
const std::string BsonBase::bsonFtAlt        = "Altitude";

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
