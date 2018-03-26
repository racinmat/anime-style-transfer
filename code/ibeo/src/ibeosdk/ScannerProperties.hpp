//======================================================================
/*! \file ScannerProperties.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Feb 10, 2014
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SCANNERPROPERTIES_HPP_SEEN
#define IBEOSDK_SCANNERPROPERTIES_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/ScannerInfo.hpp>
#include <ibeosdk/Math.hpp>
#include <ibeosdk/inttypes.hpp>

#include <boost/unordered_map.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

//======================================================================
/*!\class ScannerProperties
 * \brief
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Feb 10, 2014
 *
 *///-------------------------------------------------------------------
class ScannerProperties {
public:
	//========================================
	/*!\brief Get the properties for the scanner type
	 *        of the the given ScannerTypeId.
	 *
	 * \param[in] scannerTypeId  Scanner type to receive the
	 *                           properties for.
	 * \return ScannerProperties for scanner with scanner type
	 *         \a scannerTypeId. If the ScannerProperties are
	 *         not registered for the given \a scannerTypeId,
	 *         the ScannerProperties for #ScannerType_Invalid
	 *         will be returned.
	 *         I.e. you can check whether you got valid
	 *         ScannerProperties by comparing \a scannerTypeId
	 *         and getScannerProperties(scannerTypeId).getScannerType().
	 *///-------------------------------------
	static const ScannerProperties& getScannerProperties(const uint16_t scannerTypeId);

	//========================================
	/*!\brief Register additional scanner properties.
	 *
	 * To call this method is only necessary to register
	 * additional scanner properties.
	 * Currently the following ScannerTypes are already
	 * registered: #ScannerType_Invalid, #ScannerType_Ecu,
	 * #ScannerType_Lux, #ScannerType_Lux4, #ScannerType_MiniLux,
	 * #ScannerType_ScalaB2.
	 *
	 * \param[in] sp  ScannerProperties to be registered for the
	 *                ScannerTypeId given inside \a sp.
	 * \return If any error occur a value different from 0 will be
	 *         returned.
	 * \retval 0  No error occured, ScannerProperties are registered.
	 * \retval 1  ScannerProperties for the given ScannerTypeId has
	 *            already been registered. Registration failed.
	 *///-------------------------------------
	static int registerAdditionalScanner(const ScannerProperties& sp);

protected:
	ScannerProperties();

	ScannerProperties(const ScannerType scannerType,
	                  const uint8_t nbOfLayers,
	                  const uint8_t maxNbOfEchos,
	                  const float verticalBeamDivergence,
	                  const float horizontalBeamDivergence,
	                  const float verticalResolution,
	                  const float distanceAccuracy,
	                  const float normTargetRange,
	                  const float minScanAngle,
	                  const float maxScanAngle);

public: // getter
	ScannerType getScannerType() const { return m_scannerType; }
	uint8_t getNbOfLayers () const { return m_nbOfLayers; }
	uint8_t getMaxNbOfEchos () const { return m_maxNbOfEchos; }
	float getVerticalBeamDivergence() const { return m_verticalBeamDivergence; }
	float getHorizontalBeamDivergence() const { return m_horizontalBeamDivergence; }
	float getVerticalResolution() const { return m_verticalResolution; }
	float getDistanceAccuracy() const { return m_distanceAccuracy; }
	float getNormTargetRange() const { return m_normTargetRange; }
	float getMinScanAngle() const { return m_minScanAngle; }
	float getMaxScanAngle() const { return m_maxScanAngle; }

private:
	static int registerStdScanners();

	template<uint16_t ScannerTypeId>
	static ScannerProperties createScannerProperties();

private:
	static boost::unordered_map<uint16_t, ScannerProperties*> propertiesMap;
	static int registerationError;

private:
	ScannerType m_scannerType;
	uint8_t m_nbOfLayers;
	uint8_t m_maxNbOfEchos;
	float m_verticalBeamDivergence;
	float m_horizontalBeamDivergence;
	float m_verticalResolution;
	float m_distanceAccuracy;
	float m_normTargetRange;
	float m_minScanAngle;
	float m_maxScanAngle;
}; // ScannerProperties

//======================================================================

template<>
inline
ScannerProperties ScannerProperties::createScannerProperties<ScannerType_Invalid>()
{
	return ScannerProperties(ScannerType_Invalid, 0, 0,
		NaN, NaN, NaN, NaN, NaN, NaN, NaN );
}

//======================================================================

template<>
inline
ScannerProperties ScannerProperties::createScannerProperties<ScannerType_Ecu>()
{
	return ScannerProperties(ScannerType_Ecu, 0, 0,
		NaN, NaN, NaN, NaN, NaN, NaN, NaN );
}

//======================================================================

template<>
inline
ScannerProperties ScannerProperties::createScannerProperties<ScannerType_Lux>()
{
	return ScannerProperties(
		ScannerType_Lux,
		4,
		3,
		0.8f * deg2radf,
		0.08f * deg2radf,
		0.8f * deg2radf,
		0.1f,
		50.f,
		NaN,
		NaN);
}


//======================================================================

template<>
inline
ScannerProperties ScannerProperties::createScannerProperties<ScannerType_Lux4>()
{
	return ScannerProperties(
		ScannerType_Lux4,
		4,
		3,
		0.8f * deg2radf,
		0.08f * deg2radf,
		0.8f * deg2radf,
		0.1f,
		50.f,
		NaN,
		NaN);
}

//======================================================================

template<>
inline
ScannerProperties ScannerProperties::createScannerProperties<ScannerType_ScalaB2>()
{
	return ScannerProperties(
		ScannerType_ScalaB2,
		3,
		3,
		0.8f * deg2radf,
		0.08f * deg2radf,
		0.8f * deg2radf,
		0.1f,
		50.f,
		-72.5f * deg2radf,
		72.5f * deg2radf );
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCANNERPROPERTIES_HPP_SEEN


//======================================================================
