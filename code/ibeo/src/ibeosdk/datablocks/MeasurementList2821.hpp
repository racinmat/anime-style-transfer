//======================================================================
/*! \file MeasurementList2821.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Aug 11, 2014
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_MEASUREMENTLIST2821_HPP_SEEN
#define IBEOSDK_MEASUREMENTLIST2821_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/MeasurementList.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class MeasurementList2821 : public RegisteredDataBlock<MeasurementList2821> {
public:
	MeasurementList2821();
	virtual ~MeasurementList2821();

public:
	bool operator==(const MeasurementList2821& other) const;
	bool operator!=(const MeasurementList2821& other) const { return !( (*this) == other );}

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

public:
	uint32_t getMicroseconds() const { return m_microseconds; }
	NTPTime getTimestamp() const { return m_timestamp; }

	const std::string& getListName() const { return m_listName; }
	std::string& getListName() { return m_listName; }
	const std::string& getGroupName() const { return m_groupName; }
	std::string& getGroupName() { return m_groupName; }
	const MeasurementList& getMeasList() const { return m_mList; }
	MeasurementList& getMeasList() { return m_mList; }

public:
	void setMicroseconds(const uint32_t newMicroseconds) { m_microseconds = newMicroseconds; }
	void setTimestamp(const NTPTime newTimestamp) { m_timestamp = newTimestamp; }
	void setListName(const std::string& newListName) { m_listName = newListName; }
	void setGroupName(const std::string& newGroupName) { m_groupName = newGroupName; }
	void setMeasurementList(const MeasurementList& newMeasurementList) { m_mList = newMeasurementList; }

protected:
	uint32_t m_microseconds;
	NTPTime m_timestamp;
	std::string m_listName;
	std::string m_groupName;
	MeasurementList m_mList;
}; // MeasurementList2821

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_MEASUREMENTLIST2821_HPP_SEEN

//======================================================================
