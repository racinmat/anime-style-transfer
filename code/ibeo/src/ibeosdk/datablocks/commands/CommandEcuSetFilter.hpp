//======================================================================
/*! \file CommandEcuSetFilter.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 7, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_COMMANDECUSETFILTER_HPP_SEEN
#define IBEOSDK_COMMANDECUSETFILTER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/commands/Command.hpp>
#include <ibeosdk/datablocks/DataBlock.hpp>
#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/DataTypeId.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class CommandEcuSetFilter
 * \brief SetFilter Command to configure the ECU output filter.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Oct 5, 2013
 *///-------------------------------------------------------------------
class CommandEcuSetFilter : public EcuCommand<CommandId::CmdManagerSetFilter>,
                            public DataBlock {
public:
	//========================================
	/*!\brief Length of the SetFilter command.
	 *///-------------------------------------
	static const int commandLength = 8;

	//========================================
	/*!\brief Version of the SetFilter command.
	 *///-------------------------------------
	static const uint16_t version = 2;

public:
	//========================================
	/*!\brief DataType range of the output filter.
	 *
	 * Datatypes with id between \a rangeStart and
	 * \a rangeEnd shall pass the output filter.
	 * \param[in] rangeStart  Lowest DataType it
	 *                        to be passed the filter.
	 * \param[in] rangeEnd    Highest DataType it
	 *                        to be passed the filter.
	 *///-------------------------------------
	CommandEcuSetFilter(const DataTypeId rangeStart, const DataTypeId rangeEnd);

public:
	//========================================
	/*!\brief Get the version of this Command.
	 * Get the version of this command.
	 *///-------------------------------------
	uint16_t getVersion() const { return m_version; }

public:
	//========================================
	/*!\brief Get the DataType of this DataBlock.
	 * \return Always DataType#DataType_Command.
	 *///-------------------------------------
	virtual DataTypeId getDataType() const { return dataType; }

	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization
	 *         of this Command.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const { return commandLength; }

	//========================================
	/*!\brief Deserialize data from the given stream \a is into
	 *        this CommandSetFilter.
	 * \param[in, out] is  Stream that provides the serialized
	 *                     data to fill this CommandSetFilter.
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
	 *                 data from this CommandSetFilter.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const;

public:
	virtual bool serializeToStream(std::ostream& os) const { return serialize(os); }
	virtual std::streamsize getCommandLength() const { return getSerializedSize(); }
	
	DataTypeId getRangeStart() const { return m_rangeStart; }
	DataTypeId getRangeEnd() const { return m_rangeEnd; }
	
private:
	virtual bool callListener(DataListenerBase&) const { return false; }

protected:
	//========================================
	/*!\brief The version of this Command.
	 *///-------------------------------------
	uint16_t m_version;

	DataTypeId m_rangeStart;
	DataTypeId m_rangeEnd;
}; // CommandEcuSetFilter

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_COMMANDECUSETFILTER_HPP_SEEN

//======================================================================
