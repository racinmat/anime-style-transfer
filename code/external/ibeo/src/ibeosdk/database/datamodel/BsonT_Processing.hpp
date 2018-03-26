//======================================================================
/*! \file BsonT_Processing.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef BSONT_PROCESSING_HPP_SEEN
#define BSONT_PROCESSING_HPP_SEEN

//======================================================================

#ifndef BSONT_HPP_SEEN
#	error "Must be include by BsonT.hpp"
#endif // BSONT_HPP_SEEN

//======================================================================

namespace ibeosdk {
namespace dbaccess {

class Processing;
class ProcessingJob;

//======================================================================
/*!\class BsonT<ProcessingJob>
 * \brief BsonT representation for a ProcessingJob
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
template<>
class BsonT<ProcessingJob> : public BsonBase
{
public:
	//========================================
	/*!\brief Converts a BSON object to a ProcessingJob
	 *
	 * \param[in,out] data The ProcessingJob to fill.
	 * \param[in] bsonObj The BSON object from the database query.
	 *///-------------------------------------
	static void createDataType(ProcessingJob& data,
	                           const mongo::BSONObj& bsonObj);

public:
	//========================================
	/*!\brief Database field for the type name.
	 *///-------------------------------------
	static const std::string bsonTypeName;

	//========================================
	/*!\brief Database field for the display name.
	 *///-------------------------------------
	static const std::string bsonFtProcJobName;

	//========================================
	/*!\brief Database field for the trip name.
	 *///-------------------------------------
	static const std::string bsonFtProcJobTripName;

	//========================================
	/*!\brief Database field for the result config.
	 *///-------------------------------------
	static const std::string bsonFtProcJobResultConfig;

	//========================================
	/*!\brief Database field for the start time.
	 *///-------------------------------------
	static const std::string bsonFtProcJobStartTime;

	//========================================
	/*!\brief Database field for the finished time.
	 *///-------------------------------------
	static const std::string bsonFtProcJobFinishTime;
}; // BsonT<ProcessingJob>

//======================================================================
//======================================================================
//======================================================================

//======================================================================
//======================================================================
//======================================================================
/*!\class BsonT<Processing>
 * \brief BsonT representation for a Processing
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
template<>
class BsonT<Processing> : public BsonBase {
public:
	//========================================
	/*!\brief Converts a BSON object to a Processing object.
	 *
	 * \param[in,out] data The Processing to fill.
	 * \param[in] bsonObj The BSON object from the database query.
	 *///-------------------------------------
	static void createDataType(Processing& data,
	                           const mongo::BSONObj& bsonObj);

public:
	//========================================
	/*!\brief Database field for Processing name.
	 *///-------------------------------------
	static const std::string bsonFtProcessingName;

	//========================================
	/*!\brief Database field for the ProcessingJob list.
	 *///-------------------------------------
	static const std::string bsonFtProcessingJobs;
}; // BsonT<Processing>

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // BSONT_PROCESSING_HPP_SEEN

//======================================================================
