//======================================================================
/*! \file MetaInformationFactory.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/MetaInformationFactory.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationEcuId.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationAppBaseConfig.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationKeywords.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationVersionNumber.hpp>

#include <ibeosdk/Log.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

MetaInformationFactory::MetaInformationFactory()
{
	registerType<MetaInformationEcuId>(MetaInformation::MetaInfoType_EcuId);
	registerType<MetaInformationAppBaseConfig>(MetaInformation::MetaInfoType_AppBaseConfig);
	registerType<MetaInformationKeywords>(MetaInformation::MetaInfoType_Keywords);
	registerType<MetaInformationVersionNumber>(MetaInformation::MetaInfoType_VersionNumber);
}

//==============================================================================

MetaInformationFactory& MetaInformationFactory::getInstance()
{
	static MetaInformationFactory instance;
	return instance;
}

//==============================================================================

void MetaInformationFactory::registerType(const MetaInformation::MetaInformationType type, MetaInformationFactory::FactoryFunc func)
{
	// Verify we don't overwrite anything with this name!
	if (m_factoryMap.count(type) > 0) {
		assert(false);
		return;
	}
	m_factoryMap.insert(FactoryMap::value_type(type, func));
	//std::cerr << "MetaInformation Type registered: " << type << std::endl;
}

//==============================================================================

const MetaInformationSPtr MetaInformationFactory::create(const MetaInformation::MetaInformationType type)
{
	MetaInformationSPtr ret;

	FactoryMap::const_iterator fact_it = m_factoryMap.find(type);
	if (fact_it != m_factoryMap.end()) {
		ret.reset(fact_it->second());
	}
	else {
		; //
	}

	return ret;
}

//======================================================================

}// namespace ibeosdk

//======================================================================
