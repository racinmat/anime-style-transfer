//======================================================================
/*! \file BsonT_Session.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef BSONT_SESSION_HPP_SEEN
#define BSONT_SESSION_HPP_SEEN

//======================================================================

#ifndef BSONT_HPP_SEEN
#	error "Must be include by BsonT.hpp"
#endif // BSONT_HPP_SEEN

//======================================================================

namespace ibeosdk {
namespace dbaccess {
class Session;

//======================================================================
/*!\class BsonT<Session>
 * \brief BsonT representation for a Session
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
template<>
class BsonT<Session> : public BsonBase {

public:
	//========================================
	/*!\brief Converts a BSON object to a Session object.
	 *
	 * \param[in,out] data The Session to fill.
	 * \param[in] bsonObj The BSON object from the database query.
	 *///-------------------------------------
	static void createDataType(Session& data,
	                           const mongo::BSONObj& bsonObj);

public:
	//========================================
	/*!\brief Database field for session info.
	 *///-------------------------------------
	static const std::string bsonTypeName;

	//========================================
	/*!\brief Database field for session name.
	 *///-------------------------------------
	static const std::string bsonFtSessionName;

	//========================================
	/*!\brief Database field for session keywords.
	 *///-------------------------------------
	static const std::string bsonFtSessionKeywords;

	//========================================
	/*!\brief Database field for session description.
	 *///-------------------------------------
	static const std::string bsonFtSessionDescription;

	//========================================
	/*!\brief Database field for session date.
	 *///-------------------------------------
	static const std::string bsonFtSessionDate;

	//========================================
	/*!\brief Database field for session trips.
	 *///-------------------------------------
	static const std::string bsonFtSessionTrips;

	//========================================
	/*!\brief Database field for session processings
	 *///-------------------------------------
	static const std::string bsonFtSessionProcessings;
}; // BsonT<Session>

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // BSONT_SESSION_HPP_SEEN

//======================================================================
