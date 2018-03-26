//======================================================================
/*! \file MetaInformationTestSupport7110.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

#ifndef METAINFORMATIONTESTSUPPORT_HPP_SEEN
#define METAINFORMATIONTESTSUPPORT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include "TestSupport.hpp"

#include <ibeosdk/datablocks/MetaInformationList7110.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationVersionNumber.hpp>

//======================================================================

namespace ibeosdk {
class MetaInformationAppBaseConfig;
class MetaInformationEcuId;
class MetaInformationKeywords;
class MetaInformationVersionNumber;

namespace unittests {

//======================================================================

class MetaInformationTestSupport : public TestSupport {
public:
	typedef void(*RandomFill)(MetaInformation&);

private:
	typedef std::map<MetaInformation::MetaInformationType, RandomFill> FillMap;

public:
	static const int nbOfRepeats = 100;

public:
	MetaInformationTestSupport();
	virtual ~MetaInformationTestSupport();

protected:
	MetaInformationList7110 createMIL() const;
	MetaInformationList7110 createMIL(const MetaInformation::MetaInformationType miType) const;
	static MetaInformationSPtr createMI(const MetaInformation::MetaInformationType miType);

protected:
	static MetaInformationSPtr createMI();

	static void fillRandomly(ibeosdk::MetaInformationAppBaseConfig& mil);
	static void fillRandomly(ibeosdk::MetaInformationEcuId& mil);
	static void fillRandomly(ibeosdk::MetaInformationKeywords& mil);
	static void fillRandomly(ibeosdk::MetaInformationUnsupported& mil);
	static void fillRandomly(ibeosdk::MetaInformationVersionNumber& mil);

	static void fill64kString(std::string& s);
	static void fillNonEmpty30String(std::string& s);

private:
	void fillRandomly(ibeosdk::MetaInformation& mi) const;

private:
	//========================
	template<class T>
	static void randomFill(MetaInformation& mi)
	{
		fillRandomly(dynamic_cast<T&>(mi));
	}

	//========================
	template<class T>
	void registerType(const MetaInformation::MetaInformationType type)
	{
		registerType(type, MetaInformationTestSupport::randomFill<T>);
	}

	//========================
	void registerType(const MetaInformation::MetaInformationType type, RandomFill func)
	{
		// Verify we don't overwrite anything with this name!
		if (m_fillMap.count(type) > 0) {
			assert(false);
			return;
		}
		m_fillMap.insert(FillMap::value_type(type, func));
		//std::cerr << "MetaInformation Type registered: " << type << std::endl;
	}

private:
	FillMap m_fillMap;
}; // MetaInformationTestSupport

//======================================================================

template<> MetaInformation::MetaInformationType TestSupport::getRandValue();
template<> MetaInformationVersionNumber::SoftwareType TestSupport::getRandValue();

//======================================================================

} // namespace unittests
} // namespace ibeosdk

//======================================================================

#endif // METAINFORMATIONTESTSUPPORT_HPP_SEEN

//======================================================================
