//======================================================================
/*! \file ecu.hpp
 * \brief Include file for using IbeoEcu.
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 30, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_ECU_HPP_SEEN
#define IBEOSDK_ECU_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/devices/IbeoEcu.hpp>



#include <ibeosdk/datablocks/LogMessageDebug.hpp>
#include <ibeosdk/datablocks/LogMessageError.hpp>
#include <ibeosdk/datablocks/LogMessageNote.hpp>
#include <ibeosdk/datablocks/LogMessageWarning.hpp>
#include <ibeosdk/datablocks/PositionWgs84_2604.hpp>
#include <ibeosdk/datablocks/VehicleStateBasicEcu.hpp>
#include <ibeosdk/datablocks/VehicleStateBasicEcu2806.hpp>
#include <ibeosdk/datablocks/VehicleStateBasicEcu2808.hpp>
#include <ibeosdk/datablocks/Scan2208.hpp>
#include <ibeosdk/datablocks/ScanEcu.hpp>
#include <ibeosdk/datablocks/ScanLux.hpp>
#include <ibeosdk/datablocks/Image.hpp>
#include <ibeosdk/datablocks/ObjectListEcuEt.hpp>
#include <ibeosdk/datablocks/ObjectListEcu.hpp>
#include <ibeosdk/datablocks/ObjectListEcuEtDyn.hpp>
#include <ibeosdk/datablocks/RefObjectListEcuEtDyn.hpp>
#include <ibeosdk/datablocks/MeasurementList2821.hpp>
#include <ibeosdk/datablocks/DeviceStatus.hpp>
#include <ibeosdk/datablocks/DeviceStatus6303.hpp>
#include <ibeosdk/datablocks/ObjectAssociationList4001.hpp>
#include <ibeosdk/datablocks/ContentSeparator7100.hpp>
#include <ibeosdk/datablocks/MetaInformationList7110.hpp>
#include <ibeosdk/datablocks/TimeRecord9000.hpp>
#include <ibeosdk/datablocks/Odometry9002.hpp>
#include <ibeosdk/datablocks/GpsImu9001.hpp>
#include <ibeosdk/datablocks/SystemMonitoringCanStatus6700.hpp>
#include <ibeosdk/datablocks/SystemMonitoringDeviceStatus6701.hpp>
#include <ibeosdk/datablocks/SystemMonitoringSystemStatus6705.hpp>
#include <ibeosdk/listener/DataStreamer.hpp>

#include <ibeosdk/LogFileManager.hpp>
#include <ibeosdk/IbeoSDK.hpp>

//======================================================================

#endif // IBEOSDK_ECU_HPP_SEEN

//======================================================================
