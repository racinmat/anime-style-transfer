//======================================================================
/*! \file MetaInformationFactory.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

#ifndef METAINFORMATIONFACTORY_HPP_SEEN
#define METAINFORMATIONFACTORY_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/MetaInformation.hpp>

#include <map>
#include <exception>

//======================================================================

namespace ibeosdk {

//======================================================================

class MetaInformationFactory {
public:
	typedef MetaInformation *(*FactoryFunc)();
	typedef MetaInformation::MetaInformationType MetaInformationType;

private:
	typedef std::map<MetaInformationType, FactoryFunc> FactoryMap;

public:
	static MetaInformationFactory& getInstance();

public:
	void registerType(const MetaInformationType type, FactoryFunc func);
	const MetaInformationSPtr create(const MetaInformationType type);

//	template<class T>
//	const boost::shared_ptr<T> create()
//	{
//		return boost::shared_ptr<T>(new T());
//	}

private:
	MetaInformationFactory();

	//========================
	template<class T>
	static MetaInformation* factory_function()
	{
		return new T();
	}
	//========================

public:
	//========================
	template<class T>
	void registerType(const MetaInformation::MetaInformationType type)
	{
		registerType(type, MetaInformationFactory::factory_function<T>);
	}
	//========================

private:
	FactoryMap m_factoryMap;
}; // MetaInformationFactory

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // METAINFORMATIONFACTORY_HPP_SEEN

//======================================================================
