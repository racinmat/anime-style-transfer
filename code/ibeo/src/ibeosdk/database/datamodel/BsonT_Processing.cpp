//======================================================================
/*! \file BsonT_Processing.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/datamodel/BsonT.hpp>
#include <ibeosdk/database/basedatamodel/Processing.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

const std::string BsonT<ProcessingJob>::bsonTypeName              = "EVSType";
const std::string BsonT<ProcessingJob>::bsonFtProcJobName         = "displayName";
const std::string BsonT<ProcessingJob>::bsonFtProcJobTripName     = "tripName";
const std::string BsonT<ProcessingJob>::bsonFtProcJobResultConfig = "resultConfig";
const std::string BsonT<ProcessingJob>::bsonFtProcJobStartTime    = "startProcessingTime";
const std::string BsonT<ProcessingJob>::bsonFtProcJobFinishTime   = "finishProcessingTime";

const std::string BsonT<Processing>::bsonFtProcessingName         = "processingJobListName";
const std::string BsonT<Processing>::bsonFtProcessingJobs         = "jobs";

//======================================================================

void BsonT<ProcessingJob>::createDataType(ProcessingJob& data,
                                          const mongo::BSONObj& bsonObj)
{
	mongo::BSONElement elem;
	bsonObj.getObjectID(elem);

	const std::string type = bsonObj.getField(bsonTypeName).String();

	mongo::Date_t start = bsonObj.getField(bsonFtProcJobStartTime).Date();
	mongo::Date_t end = bsonObj.getField(bsonFtProcJobFinishTime).Date();

	std::vector<std::string> resultConf;
	if(bsonObj.hasField(bsonFtProcJobResultConfig)){
		mongo::BSONObjIterator fields (bsonObj.getField(bsonFtProcJobResultConfig).Obj());
		while(fields.more()) {
			resultConf.push_back(fields.next().String());
		}
	};

	data.setDbId(elem.OID().toString());
	data.setJobType(ProcessingUtil::getInstance()->getTypeFromStr(type));

	data.setStartTime(NTPTime(MongoDbUtils::convertToBoostTimestamp(start)));
	data.setFinishTime(NTPTime(MongoDbUtils::convertToBoostTimestamp(end)));

	data.setTripName(bsonObj.getField(bsonFtProcJobTripName).String());

	data.setResultConfig(resultConf);
}

//======================================================================
//======================================================================
//======================================================================

//======================================================================
//======================================================================
//======================================================================

void BsonT<Processing>::createDataType(Processing& data,
                                      const mongo::BSONObj& bsonObj)
{
	data.setName(bsonObj.getField(bsonFtProcessingName).String());

	std::vector<mongo::BSONElement> jobList=bsonObj.getField(bsonFtProcessingJobs).Array();

	std::vector<mongo::BSONElement>::const_iterator pIter=jobList.begin();
	for (; pIter != jobList.end(); ++pIter) {
		CollectionName cName(( *pIter).dbrefNS());
		ProcessingJob newProcJob(cName, ( *pIter).dbrefOID().toString());

		data.addProcessingJob(newProcJob);
	}
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
