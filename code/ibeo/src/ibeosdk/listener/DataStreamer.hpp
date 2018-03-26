//======================================================================
/*! \file DataStreamer.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 3, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_DATASTREAMER_HPP_SEEN
#define IBEOSDK_DATASTREAMER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>
#include <ibeosdk/datablocks/DataBlock.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class DataStreamer
 * \brief Abstract base class for all streamer classes.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Sep 4, 2013
 *
 * A DataStreamer can be registered to an IbeoDevice to receive \b all
 * DataBlock received by that device.
 * The method onData will be called in the context of the receive thread
 * of that device.
 *
 * The data will \b not be deserialized. So a DataStreamer can be used
 * to forward via network or write DataBlocks to a file. Since the
 * associated IbeoDataHeader also will be provided the same timestamp
 * of the header can left as well untouched.
 *
 * User implementation of DataStreamer can apply filter or manipulate
 * the timestamps of the IbeoDataHeader.
 *
 * If one wants to manipulate the contents of a DataBlock it may be
 * better to implement a DataListener.
 *
 * \sa DataListener
 *///-------------------------------------------------------------------
class DataStreamer {
public:
	//========================================
	/*!\brief Destrutor does nothing special.
	 *///-------------------------------------
	virtual ~DataStreamer() {}

public:
	//========================================
	/*!\brief Method to be called if a new DataBlock
	 *        (in serialized form) has been received.
	 * \param[in] dh    Meta data of the DataBlock.
	 * \param[in] data  Serialized DataBlock.
	 *///-------------------------------------
	virtual void onData(const IbeoDataHeader& dh, const char* const data) = 0;
}; // DataStreamer

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_DATASTREAMER_HPP_SEEN

//======================================================================
