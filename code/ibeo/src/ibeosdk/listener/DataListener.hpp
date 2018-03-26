//======================================================================
/*! \file DataListener.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 30, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_DATALISTENER_HPP_SEEN
#define IBEOSDK_DATALISTENER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/DataTypeId.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class DataListenerBase
 * \brief Abstract base class for all listener.
 * \author Mario Brumm (mb)
 * \version 0.1
 * \date Apr 39, 2012
 *
 * A DataListener can be registered to an IbeoDevice to receive
 * all DataBlock of the associated DataType received by that
 * device.
 *
 * Derived classes will have to implement an onData method for the
 * associated DataType. This onData method will be called in the
 * context of the receive thread of that device.
 *
 * The data received by the onData method will be deserialized.
 *
 * In case one is not interested in the contents of that DataBlock
 * it may be better to implement a DataStreamer.
 *
 * \sa DataStreamer
 *///-------------------------------------------------------------------
class DataListenerBase {
public:
	//========================================
	/*!\brief Destrutor does nothing special.
	 *///-------------------------------------
	virtual ~DataListenerBase() {}

public:
	typedef std::vector<DataTypeId> DataTypes;
	//========================================
	/*!\brief Get the DataTypes for which this
	 *        listener is listening.
	 * \return The DataTypes the listener is
	 *         listening for.
	 *///-------------------------------------
	const DataTypes& getAssociatedDataTypes() const{
		return m_registeredDataTypes;
	}


protected:
	void registerDataType(const DataTypeId type) {
		m_registeredDataTypes.push_back(type);
	}

private:
	DataTypes m_registeredDataTypes;
}; // DataListener

//======================================================================

//======================================================================
/*!\class DataListener
 * \brief Abstract base class for all DataListener listen on DataBlockImpl.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Feb 10, 2014
 *
 * \tparam DataBlockImpl  DataBlock implementation.
 *///-------------------------------------------------------------------
template<class DataBlockImpl>
class DataListener : public virtual DataListenerBase {
public:
	//========================================
	/*!\brief Constructor registers at DataListenerBase class
	 *///-------------------------------------

	DataListener(){
		registerDataType(DataBlockImpl::getDataBlockId());
	}

	//========================================
	/*!\brief Called on receiving a new DataBlockImpl DataBlock.
	 *
	 * Method to be called by the IbeoDevice where this listener
	 * is registered when a new DataBlock of type DataBlockImpl
	 * has been received.
	 *
	 * \param[in] dbImpl  Pointer to the DataBlockImpl that has
	 *                    been received.
	 *///-------------------------------------
	virtual void onData(const DataBlockImpl* const dbImpl) = 0;
}; // DataListener

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_DATALISTENER_HPP_SEEN

//======================================================================

