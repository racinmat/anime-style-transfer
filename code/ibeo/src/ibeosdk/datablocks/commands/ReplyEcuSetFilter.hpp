//======================================================================
/*! \file ReplyEcuSetFilter.hpp
 *
 * \copydoc Copyright
 * \author Julia Nitsch (jn)
 * \date May 05, 2016
 *///-------------------------------------------------------------------

#ifndef REPLYECUSETFILTER_HPP
#define REPLYECUSETFILTER_HPP

//==============================================================================

#include <ibeosdk/CommandId.hpp>
#include <ibeosdk/datablocks/commands/Command.hpp>
#include <ibeosdk/datablocks/DataBlock.hpp>

//==============================================================================

namespace ibeosdk {

//==============================================================================

class ReplyEcuSetFilter :  public ibeosdk::EcuCommandReply<CommandId::CmdManagerSetFilter>,
                           public ibeosdk::DataBlock {
public:
	//========================================
	/*!\brief Length of the CommandManagerAppBaseStatus command.
	 *///-------------------------------------
	static const int replySize = 2;

public:
	ReplyEcuSetFilter();

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
	virtual std::streamsize getSerializedSize() const { return std::streamsize(replySize); }

	//========================================
	/*!\brief Deserialize data from the given stream \a is into
	 *        this CommandManagerAppBaseStatusReply.
	 * \param[in, out] is  Stream that provides the serialized
	 *                     data to fill this CommandManagerAppBaseStatusReply.
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
	 *                 data from this CommandManagerAppBaseStatusReply.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const;

public:
	virtual bool callListener(DataListenerBase&) const { return false; }

public:
	virtual bool deserializeFromStream(std::istream& is, const IbeoDataHeader& dh) { return deserialize(is, dh); }

}; //ReplyEcuSetFilter

//==============================================================================

} //namespace ibeosdk

//==============================================================================

#endif // REPLYECUSETFILTER_HPP

//==============================================================================
