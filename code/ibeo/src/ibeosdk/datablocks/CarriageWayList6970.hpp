//======================================================================
/*! \file CarriageWayList6970.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Feb 24, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_CARRIAGEWAYLIST6970_HPP_SEEN
#define IBEOSDK_CARRIAGEWAYLIST6970_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/CarriageWayList.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class CarriageWayList6970 : public ibeosdk::RegisteredDataBlock<CarriageWayList6970> {
public:
	CarriageWayList6970();
	virtual ~CarriageWayList6970();

public:
	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const;

	//========================================
	/*!\brief Get the DataType of this Image DataBlock.
	 *///-------------------------------------
	virtual DataTypeId getDataType() const;

	//========================================
	/*!\brief Deserialize data from the given stream \a is into
	 *        this Image.
	 * \param[in, out] is  Stream that provides the serialized
	 *                     data to fill this Image.
	 *                     On exit the \a is get pointer will
	 *                     be behind the read data.
	 * \param[in]      dh  IbeoDataHeader that has been received
	 *                     together with the serialized data in \a is.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Reading the data from the stream has failed.
	 *///-------------------------------------
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);

	//========================================
	/*!\brief Serialize data into the given stream \a os.
	 * \param[out] os  Stream that receive the serialized
	 *                 data from this Image.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const;
//======================================================================
public:
	const lanes::CarriageWayList& getCarriageWayList() const { return m_carriageWayList; }
	lanes::CarriageWayList& getCarriageWayList() { return m_carriageWayList; }

private:
	lanes::CarriageWayList m_carriageWayList;

}; // CarriageWayList6970

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_CARRIAGEWAYLIST6970_HPP_SEEN

//======================================================================
