//======================================================================
/*! \file ScannerProperties.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Feb 10, 2014
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/ScannerProperties.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

const ScannerProperties& ScannerProperties::getScannerProperties(const uint16_t scannerTypeId)
{
	boost::unordered_map<uint16_t, ScannerProperties*>::const_iterator fIter = propertiesMap.find(scannerTypeId);
	if (fIter == propertiesMap.end())
		return *(propertiesMap.at(ScannerType_Invalid));

	return *(fIter->second);
}

//======================================================================

ScannerProperties::ScannerProperties()
  : m_scannerType(),
    m_nbOfLayers(),
    m_maxNbOfEchos(),
    m_verticalBeamDivergence(),
    m_horizontalBeamDivergence(),
    m_verticalResolution(),
    m_distanceAccuracy(),
    m_normTargetRange(),
    m_minScanAngle(),
    m_maxScanAngle()
{}

//======================================================================

ScannerProperties::ScannerProperties(const ScannerType scannerType,
                                     const uint8_t nbOfLayers,
                                     const uint8_t maxNbOfEchos,
                                     const float verticalBeamDivergence,
                                     const float horizontalBeamDivergence,
                                     const float verticalResolution,
                                     const float distanceAccuracy,
                                     const float normTargetRange,
                                     const float minScanAngle,
                                     const float maxScanAngle)
  : m_scannerType(scannerType),
    m_nbOfLayers(nbOfLayers),
    m_maxNbOfEchos(maxNbOfEchos),
    m_verticalBeamDivergence(verticalBeamDivergence),
    m_horizontalBeamDivergence(horizontalBeamDivergence),
    m_verticalResolution(verticalResolution),
    m_distanceAccuracy(distanceAccuracy),
    m_normTargetRange(normTargetRange),
    m_minScanAngle(minScanAngle),
    m_maxScanAngle(maxScanAngle)
{}

//======================================================================

int ScannerProperties::registerStdScanners()
{
	int error = 0;
	if ((error = registerAdditionalScanner(createScannerProperties<ScannerType_Invalid>())))
		return error;
	if ((error = registerAdditionalScanner(createScannerProperties<ScannerType_Ecu>())))
		return error;
	if ((error = registerAdditionalScanner(createScannerProperties<ScannerType_Lux>())))
		return error;
	if ((error = registerAdditionalScanner(createScannerProperties<ScannerType_Lux4>())))
		return error;
	if ((error = registerAdditionalScanner(createScannerProperties<ScannerType_ScalaB2>())))
		return error;
	return 1;
}

//======================================================================

int ScannerProperties::registerAdditionalScanner(const ScannerProperties& sp)
{
	boost::unordered_map<uint16_t, ScannerProperties*>::const_iterator fIter = propertiesMap.find(sp.getScannerType());
	if (fIter != propertiesMap.end())
		return 1;

	propertiesMap[sp.getScannerType()] = new ScannerProperties(sp);
	return 0;
}

//======================================================================

boost::unordered_map<uint16_t, ScannerProperties*> ScannerProperties::propertiesMap;

//======================================================================

int ScannerProperties::registerationError = registerStdScanners();

//======================================================================

} // namespace ibeosdk

//======================================================================
