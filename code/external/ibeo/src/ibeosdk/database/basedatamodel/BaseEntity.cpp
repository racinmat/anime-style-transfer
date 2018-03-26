//======================================================================
/*! \file BaseEntity.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 4, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/basedatamodel/BaseEntity.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

const std::string BaseEntity::Content_DataModel                      = "EVSDataModel";
const std::string BaseEntity::Content_Trip                           = "Trip;";

const std::string BaseEntity::ContentPrefix_Processing              = ".Proc;";

const std::string BaseEntity::ContentSuffix_CanMessage              = ".CANMessages";
const std::string BaseEntity::ContentSuffix_CarriageWays            = ".CarriageWays";
const std::string BaseEntity::ContentSuffix_DutObj                  = ".DutObjects";
const std::string BaseEntity::ContentSuffix_DutObjList              = ".DutObjectLists";
const std::string BaseEntity::ContentSuffix_DynObj                  = ".DynamicObjects";
const std::string BaseEntity::ContentSuffix_ObjTraj                 = ".DynamicObjectStreams";
const std::string BaseEntity::ContentSuffix_Images                  = ".Images";
const std::string BaseEntity::ContentSuffix_InfoValues              = ".InfoValues";
const std::string BaseEntity::ContentSuffix_JobBase                 = ".JobBase";
const std::string BaseEntity::ContentSuffix_JobList                 = ".JobList";
const std::string BaseEntity::ContentSuffix_LanePoints              = ".LanePoints";
const std::string BaseEntity::ContentSuffix_MeasurementLists        = ".MeasurementLists";
const std::string BaseEntity::ContentSuffix_StatObj                 = ".StaticObjects";
const std::string BaseEntity::ContentSuffix_ObjectAssociations      = ".ObjectAssociations";
const std::string BaseEntity::ContentSuffix_ObjectFlags             = ".ObjectFlags";
const std::string BaseEntity::ContentSuffix_OelaMeasurement         = ".OelaMeasurement";
const std::string BaseEntity::ContentSuffix_ProcessingJobs          = ".ProcessingJobs";
const std::string BaseEntity::ContentSuffix_Scans                   = ".Scans";
const std::string BaseEntity::ContentSuffix_Sessions                = ".Sessions";
const std::string BaseEntity::ContentSuffix_VehicleStates           = ".VehicleStates";

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
