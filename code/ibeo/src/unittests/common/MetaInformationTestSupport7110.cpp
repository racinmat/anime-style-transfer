//======================================================================
/*! \file MetaInformationTestSupport7110.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <unittests/common/MetaInformationTestSupport7110.hpp>
#include <ibeosdk/MetaInformationFactory.hpp>

#include <ibeosdk/datablocks/snippets/MetaInformationAppBaseConfig.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationEcuId.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationKeywords.hpp>

//======================================================================

namespace ibeosdk {
namespace unittests {

//======================================================================

MetaInformationTestSupport::MetaInformationTestSupport()
  : TestSupport()
{
	registerType<MetaInformationEcuId>(MetaInformation::MetaInfoType_EcuId);
	registerType<MetaInformationAppBaseConfig>(MetaInformation::MetaInfoType_AppBaseConfig);
	registerType<MetaInformationKeywords>(MetaInformation::MetaInfoType_Keywords);
	registerType<MetaInformationVersionNumber>(MetaInformation::MetaInfoType_VersionNumber);
	registerType<MetaInformationUnsupported>(MetaInformation::MetaInfoType_Unsupported);
}

//======================================================================

MetaInformationTestSupport::~MetaInformationTestSupport() {}

//======================================================================

MetaInformationList7110 MetaInformationTestSupport::createMIL() const
{
	MetaInformationList7110 mil;

	const uint8_t nbOfInfos = getRandValue<uint8_t>(20);

	for (uint8_t i = 0; i < nbOfInfos; ++i) {
		MetaInformationSPtr mi = createMI();
		if (!mi)
			mi.reset(new MetaInformationUnsupported(
				MetaInformation::MetaInformationType(getRandValue<uint8_t>(2) + 1 + MetaInformation::MetaInfoType_Keywords),
				getRandValue<NTPTime>(),
				getRandValue<uint16_t>())
			);

		fillRandomly(*mi);
		mil.addMetaInformation(mi);
	}

	return mil;
}

//======================================================================

MetaInformationList7110 MetaInformationTestSupport::createMIL(const MetaInformation::MetaInformationType miType) const
{
	MetaInformationList7110 mil;

	const uint8_t nbOfInfos = getRandValue<uint8_t>(20);

	MetaInformationSPtr mi = createMI(miType);
	if (!mi) {
		const MetaInformation::MetaInformationType unsupportedType = MetaInformation::MetaInformationType(getRandValue<uint8_t>(2) + 1 + MetaInformation::MetaInfoType_Keywords);
		mi.reset(new MetaInformationUnsupported(unsupportedType, getRandValue<NTPTime>(), getRandValue<uint16_t>()));
	}

	for (uint8_t i = 0; i < nbOfInfos; ++i) {
		fillRandomly(*mi);
		mil.addMetaInformation(mi);
	}

	return mil;
}

//======================================================================

MetaInformationSPtr MetaInformationTestSupport::createMI(const MetaInformation::MetaInformationType miType)
{
	return MetaInformationFactory::getInstance().create(miType);
}

//======================================================================

MetaInformationSPtr MetaInformationTestSupport::createMI()
{
	const MetaInformation::MetaInformationType mit = getRandValue<MetaInformation::MetaInformationType>();
	return createMI(mit);
}

//======================================================================

void MetaInformationTestSupport::fillRandomly(ibeosdk::MetaInformation& mi) const
{
	const FillMap::const_iterator fillIter = m_fillMap.find(mi.getType());
	if (fillIter != m_fillMap.end()) {
		(fillIter->second)(mi);
	}
}

//======================================================================

void MetaInformationTestSupport::fillRandomly(ibeosdk::MetaInformationAppBaseConfig& mil)
{
	std::string s;
	fill64kString(s);
	mil.setAppBaseConfig(s);
}

//======================================================================

void MetaInformationTestSupport::fillRandomly(ibeosdk::MetaInformationEcuId& mil)
{
	std::string s;
	fill64kString(s);
	mil.setEcuId(s);
}

//======================================================================

void MetaInformationTestSupport::fillRandomly(ibeosdk::MetaInformationKeywords& mil)
{
	const uint16_t n = getRandValue<uint16_t>(200);

	for (int i = 0; i < n; ++i) {
		std::string keyword;
		fillNonEmpty30String(keyword);
		mil.addKeyword(keyword);
	}
}

//======================================================================

void MetaInformationTestSupport::fillRandomly(ibeosdk::MetaInformationUnsupported& mil)
{
	MetaInformationUnsupported::DataVector& dv = mil.getData();
	dv.resize(getRandValue<uint16_t>());
	MetaInformationUnsupported::DataVector::iterator it = dv.begin();
	for (; it != dv.end(); ++it) {
		*it = getRandValue<uint8_t>();
	}
}

//======================================================================

void MetaInformationTestSupport::fillRandomly(ibeosdk::MetaInformationVersionNumber& mil)
{
	mil.setSoftwareType(getRandValue<MetaInformationVersionNumber::SoftwareType>());
	mil.setVersionNumber(getRandValue<uint32_t>());
	std::string s;
	fill64kString(s);
	mil.setExtraString(s);
}

//======================================================================

void MetaInformationTestSupport::fill64kString(std::string& s)
{
	const uint16_t n = getRandValue<uint16_t>();
	s.resize(n);

	std::string::iterator it = s.begin();
	for (; it != s.end(); ++it) {
		*it = int8_t(65 + getRandValue<uint8_t>(25));
	}
}

//======================================================================

void MetaInformationTestSupport::fillNonEmpty30String(std::string& s)
{
	const uint32_t n = uint32_t(getRandValue<uint8_t>(29)+1);
	s.resize(n);

	std::string::iterator it = s.begin();
	for (; it != s.end(); ++it) {
		*it = int8_t(65 + getRandValue<uint8_t>(25));
	}
}

//======================================================================
//======================================================================
//======================================================================

template<> MetaInformation::MetaInformationType TestSupport::getRandValue()
{
	uint16_t rawMit = uint16_t(getRandValue<uint16_t>(uint16_t(MetaInformation::MetaInfoType_Keywords)-1));

	// skip value MetaInfoType_AppBaseSyncMethod
	if (rawMit >= MetaInformation::MetaInfoType_AppBaseSyncMethod) {
		++rawMit;
	}

	return static_cast<MetaInformation::MetaInformationType>(rawMit);
}

//======================================================================

template<> MetaInformationVersionNumber::SoftwareType TestSupport::getRandValue()
{
	return static_cast<MetaInformationVersionNumber::SoftwareType>(getRandValue<uint16_t>(MetaInformationVersionNumber::SoftwareType_Ilv));
}

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================
