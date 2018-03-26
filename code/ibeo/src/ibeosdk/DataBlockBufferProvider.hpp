//======================================================================
/*! \file DataBlockBufferProvider.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 29, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_DATABLOCKBUFFERPROVIDER_HPP_SEEN
#define IBEOSDK_DATABLOCKBUFFERPROVIDER_HPP_SEEN

//======================================================================

#include <ibeosdk/datablocks/DataBlock.hpp>

#include <ibeosdk/DataTypeId.hpp>

#include <boost/unordered_map.hpp>
#include <boost/function.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

template<class Device>
class DataBlockBufferProviderGlobal {
public:
	typedef DataBlock::DataBlockRegisterId DataBlockRegisterId;
	typedef boost::unordered_map<DataBlockRegisterId::Key, DataBlockRegisterId::DataBlockCreateFunction> GlobalProviderMap;

private:
	DataBlockBufferProviderGlobal() : glProviderMap() {}
	~DataBlockBufferProviderGlobal() {}

private:
	DataBlockBufferProviderGlobal(const DataBlockBufferProviderGlobal& other); // forbidden
	DataBlockBufferProviderGlobal& operator=(const DataBlockBufferProviderGlobal& other); // forbidden

public:
	static DataBlockBufferProviderGlobal<Device>& getInstance()
	{
		static DataBlockBufferProviderGlobal<Device> dbbp;
		return dbbp;
	}

public:
	const DataBlockRegisterId& registerDataBlock(const DataBlockRegisterId& dbri)
	{
		glProviderMap[dbri.getKey()] = dbri.getValue();
		return dbri;
	}

	const GlobalProviderMap& getMap() const { return glProviderMap; }

protected:
	GlobalProviderMap glProviderMap;
}; // DataBlockBufferProviderGlobal

//======================================================================

template<class Device>
class DataBlockBufferProvider {
public:
	class DataBlockBuffer {
	public:
		DataBlockBuffer()
		  : m_buffer(NULL),
		    m_create(NULL)
		{}

		DataBlockBuffer(DataBlock::DataBlockRegisterId::DataBlockCreateFunction create)
		  : m_buffer(NULL),
		    m_create(create)
		{}

		~DataBlockBuffer() { delete m_buffer; }

	public:
		DataBlock* getBuffer()
		{
			if (!m_buffer && m_create)
				m_buffer = m_create();
			return m_buffer;
		}

		const DataBlock* getBufferPtr() const { return m_buffer; }
		DataBlock::DataBlockRegisterId::DataBlockCreateFunction getCreatePtr() const { return m_create; }
	protected:
		DataBlock* m_buffer;
		DataBlock::DataBlockRegisterId::DataBlockCreateFunction m_create;
	}; // DataBlockBuffer

	typedef boost::unordered_map<DataBlock::DataBlockRegisterId::Key, DataBlockBuffer> ProviderMap;

public:
	DataBlockBufferProvider(DataBlockBufferProviderGlobal<Device>& globalProvider)
	{
		typedef typename DataBlockBufferProviderGlobal<Device>::GlobalProviderMap GlobalMap;
		const GlobalMap& globalMap = globalProvider.getMap();
		typename GlobalMap::const_iterator globalIter = globalMap.begin();
		for (; globalIter != globalMap.end(); ++globalIter) {
			providerMap[globalIter->first] = DataBlockBuffer(globalIter->second);
		} // for globalIter
	}

	~DataBlockBufferProvider() {}

private:
	DataBlockBufferProvider(const DataBlockBufferProvider& other); // forbidden
	DataBlockBufferProvider& operator=(const DataBlockBufferProvider& other); // forbidden

public:
	DataBlock* getBuffer(const DataBlock::DataBlockRegisterId::Key dataTypeId)
	{
		typename ProviderMap::iterator iter = providerMap.find(dataTypeId);
		if (iter == providerMap.end())
			return NULL;

		return iter->second.getBuffer();
	}

	const ProviderMap& getMap() const { return providerMap; }
protected:
	ProviderMap providerMap;
}; // DataBlockBufferProvider

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_DATABLOCKBUFFERPROVIDER_HPP_SEEN

//======================================================================
