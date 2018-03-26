//======================================================================
/*! \file BsonT.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef BSONT_HPP_SEEN
#define BSONT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/database/MongoDbUtils.hpp>
#include <boost/iostreams/stream.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================
/*!\class BsonBase
 * \brief Base class for all specific BsonT classes.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
class BsonBase {
public:
	//========================================
	/*!\brief Database field for id.
	 *///-------------------------------------
	static const std::string bsonFtId;

	//========================================
	/*!\brief Database field for mongo id.
	 *///-------------------------------------
	static const std::string bsonFtMongoId;

	//========================================
	/*!\brief Database field for object id.
	 *///-------------------------------------
	static const std::string bsonFtObjId;

	//========================================
	/*!\brief Database field for type.
	 *///-------------------------------------
	static const std::string bsonFtType;

	//========================================
	/*!\brief Database field for time stamp.
	 *///-------------------------------------
	static const std::string bsonFtTimestamp;

	//========================================
	/*!\brief Database field for binary data.
	 *///-------------------------------------
	static const std::string bsonFtBinaryData;

	//========================================
	/*!\brief Database field for file path.
	 *///-------------------------------------
	static const std::string bsonFtFile;

	//========================================
	/*!\brief Database field for version.
	 *///-------------------------------------
	static const std::string bsonFtVersion;

	//========================================
	/*!\brief Database field for class.
	 *///-------------------------------------
	static const std::string bsonFtClass;

	//========================================
	/*!\brief Database field for X.
	 *///-------------------------------------
	static const std::string bsonFtX;

	//========================================
	/*!\brief Database field for Y.
	 *///-------------------------------------
	static const std::string bsonFtY;

	//========================================
	/*!\brief Database field for Z.
	 *///-------------------------------------
	static const std::string bsonFtZ;

	//========================================
	/*!\brief Database field for latitude.
	 *///-------------------------------------
	static const std::string bsonFtLat;

	//========================================
	/*!\brief Database field for longitude.
	 *///-------------------------------------
	static const std::string bsonFtLon;

	//========================================
	/*!\brief Database field for altitude.
	 *///-------------------------------------
	static const std::string bsonFtAlt;
}; // BsonBase

//======================================================================
//======================================================================
//======================================================================

//======================================================================
//======================================================================
//======================================================================
/*!\class BsonT
 * \brief Template class for converting between Datatypes and BSON objects.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
template<class DataType>
class BsonT : public BsonBase {
public:
	//========================================
	/*!\brief Converts a BSON object to a Datatype
	 *        (Datablock or Snippet).
	 *
	 * Uses the binary data to deserialize.
	 *
	 * \param[in,out] data The DataType to fill.
	 * \param[in] bsonObj The BSON object from the database query.
	 *///-------------------------------------
	static void createDataType(DataType& data,
	                           const mongo::BSONObj& bsonObj)
	{
		const mongo::BSONElement be = bsonObj.getField(bsonFtBinaryData);
		int binDatLen = 0;
		const char* binDatPtr = be.binData(binDatLen);
		boost::iostreams::array_source binSrc(binDatPtr, binDatLen);
		boost::iostreams::stream<boost::iostreams::array_source> binSrcStream(binSrc);

		static IbeoDataHeader hdr;
		hdr.deserialize(binSrcStream);

		bool castingSuccess = false;
		try {
			DataBlock& datablock = dynamic_cast<DataBlock&>(data);
			castingSuccess = datablock.deserialize(binSrcStream, hdr);
			datablock.setDataHeader(hdr);
		}
		catch (const std::bad_cast&) {
			logDebug << "std::bad_cast for current datatype" << std::endl;
		}

		if (!castingSuccess) { // if casting to DataBlock wasn't successful, try to cast Snippet
			try {
				Snippet& snippet = dynamic_cast<Snippet&>(data);
				castingSuccess = snippet.deserialize(binSrcStream);
			}
			catch (const std::bad_cast&) {
				logDebug << "std::bad_cast for current datatype" << std::endl;
			}
		} // if

		if (!castingSuccess)
			return;
	}
}; // BsonT

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#include "BsonT_Object.hpp"
#include "BsonT_Session.hpp"
#include "BsonT_Trip.hpp"
#include "BsonT_Processing.hpp"
#include "BsonT_Point.hpp"

//======================================================================

#endif // BSONT_HPP_SEEN

//======================================================================
