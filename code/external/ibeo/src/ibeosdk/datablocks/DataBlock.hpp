//======================================================================
/*! \file DataBlock.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 25, 2012
 * \date Sep 30, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_DATABLOCK_HPP_SEEN
#define IBEOSDK_DATABLOCK_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/misc/Concurrency.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>
#include <ibeosdk/DataTypeId.hpp>

#include <ibeosdk/inttypes.hpp>

#include <boost/function.hpp>

//======================================================================

namespace ibeosdk {

class DataListenerBase;


//======================================================================
/*!\class DataBlock
 * \brief Base class for Serializable which can be read and written.
 * \author Mario Brumm (mb)
 * \version 0.1
 * \date Apr 25, 2012
 *
 * Apart from the data for the message body to send or write it also
 * contains data from the header which are m_headerDeviceId and the
 * m_headerNtpTime.
 *///-------------------------------------------------------------------
class DataBlock : public Concurrency {
public:
	class DataBlockRegisterId {
	public:
		typedef DataTypeId Key;
		typedef boost::function<DataBlock*()> DataBlockCreateFunction;
	public:
		DataBlockRegisterId(const Key dataTypeId, DataBlockCreateFunction create)
		  : m_dataTypeId(dataTypeId), m_dataBlockCreate(create)
		{}

	public:
		Key getKey() const { return m_dataTypeId; }
		DataBlockCreateFunction getValue() const { return m_dataBlockCreate; }

	protected:
		Key m_dataTypeId;
		DataBlockCreateFunction m_dataBlockCreate;
	}; // DataBlockRegisterId

public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	DataBlock();

public:
	DataBlock(const DataBlock& other);
	DataBlock& operator=(const DataBlock& other);

public:
	//========================================
	/*!\brief Get the DataType of this DataBlock.
	 * \return DataType of this DataBlock.
	 *///-------------------------------------
	virtual DataTypeId getDataType() const = 0;

	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization
	 *         of this data class.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const = 0;

	//========================================
	/*!\brief Deserialize data from the given stream \a is into
	 *        this class.
	 * \param[in, out] is  Stream that provides the serialized
	 *                     data to fill this class.
	 *                     On exit the \a is get pointer will
	 *                     be behind the read data.
	 * \param[in]      dh  IbeoDataHeader that has been received
	 *                     together with the serialized data in \a is.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occurred.
	 * \retval false Reading the data from the stream has failed.
	 *///-------------------------------------
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh) = 0;

	//========================================
	/*!\brief Serialize data into the given stream \a os.
	 * \param[out] os  Stream that receive the serialized
	 *                 data from this class.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occurred.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const = 0;

public: // getter
	//========================================
	/*!\brief Get the header device id.
	 * \return The header device id.
	 *///-------------------------------------
	virtual UINT8 getHeaderDeviceId() const { return m_headerDeviceId; }

	//========================================
	/*!\brief get the header timestamp.
	 * \return The header timestamp.
	 *///-------------------------------------
	virtual NTPTime getHeaderNtpTime() const { return m_headerNtpTime; }

public:
	//========================================
	/*!\brief Set the header data.
	 * \param[in] dh  IbeoDataHeader which contains the
	 *                data to set the m_headerDeviceId
	 *                and m_headerNtpTime of this
	 *                DataBlock.
	 *///-------------------------------------
	virtual void setDataHeader(const IbeoDataHeader& dh);

public:
	virtual bool callListener(DataListenerBase& l) const = 0;

protected:
	//========================================
	/*!\brief Device id of the header.
	 *///-------------------------------------
	UINT8 m_headerDeviceId;

	//========================================
	/*!\brief Timestamp of the header.
	 *///-------------------------------------
	NTPTime m_headerNtpTime;
}; // DataBlock

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_DATABLOCK_HPP_SEEN

//======================================================================

