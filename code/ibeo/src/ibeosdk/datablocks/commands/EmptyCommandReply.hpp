//======================================================================
/*! \file EmptyCommandReply.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 9, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_EMPTYCOMMANDREPLY_HPP_SEEN
#define IBEOSDK_EMPTYCOMMANDREPLY_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/commands/Command.hpp>
#include <ibeosdk/CommandId.hpp>
#include <ibeosdk/datablocks/DataBlock.hpp>
#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/DataTypeId.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

//template<CommandId::Command cmdId>
//class EmptyCommandReply : public CommandReply<cmdId>,
//                          public DataBlock {
//public:
//	//========================================
//	/*!\brief Length of the reply.
//	 *///-------------------------------------
//	static const int replySize = 2;
//
//public:
//	EmptyCommandReply()
//	  : CommandReply<cmdId>()
//	{}
//
//public:
//	//========================================
//	/*!\brief Get the DataType of this DataBlock.
//	 * \return Always DataType#DataType_Command.
//	 *///-------------------------------------
//	virtual DataTypeId getDataType() const { return CommandReplyBase::dataType; }
//
//	//========================================
//	/*!\brief Get the size of the serialization.
//	 * \return Number of bytes used by the serialization
//	 *         of this Command.
//	 *///-------------------------------------
//	virtual std::streamsize getSerializedSize() const { return std::streamsize(replySize); }
//
//public:
//	virtual bool callListener(DataListenerBase&) const { return false; }
//
//public:
//	virtual bool deserializeFromStream(std::istream& is, const IbeoDataHeader& dh)
//	{
//		return deserialize(is, dh);
//	}
//}; // EmptyCommandReply

//======================================================================
//======================================================================
//======================================================================

template<CommandId::Command cmdId>
class EmptyEcuCommandReply : public EcuCommandReply<cmdId>,
                             public DataBlock {
public:
	//========================================
	/*!\brief Length of the reply.
	 *///-------------------------------------
	static const int replySize = 2;

public:
	EmptyEcuCommandReply()
	  : EcuCommandReply<cmdId>()
	{}
	virtual ~EmptyEcuCommandReply() {}

public:
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
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh)
	{
		const int64_t startPos = streamposToInt64(is.tellg());

		readBE(is, CommandReplyBase::m_commandId);

		return !is.fail()
		       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
		       && this->getSerializedSize() == dh.getMessageSize();
	}

	//========================================
	/*!\brief Serialize data into the given stream \a os.
	 * \param[out] os  Stream that receive the serialized
	 *                 data from this CommandSetFilter.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const
	{
		const int64_t startPos = streamposToInt64(os.tellp());

		writeBE(os, CommandReplyBase::m_commandId);

		return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
	}

	//========================================
	/*!\brief Get the DataType of this DataBlock.
	 * \return Always DataType#DataType_Command.
	 *///-------------------------------------
	virtual DataTypeId getDataType() const { return CommandReplyBase::dataType; }

	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization
	 *         of this Command.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const { return std::streamsize(replySize); }

public:
	virtual bool callListener(DataListenerBase&) const { return false; }

public:
	virtual bool deserializeFromStream(std::istream& is, const IbeoDataHeader& dh)
	{
		return deserialize(is, dh);
	}
}; // EmptyEcuCommandReply

//======================================================================
//======================================================================
//======================================================================

template<CommandId::Command cmdId>
class EmptyLuxCommandReply : public LuxCommandReply<cmdId>,
                             public DataBlock {
public:
	//========================================
	/*!\brief Length of the reply.
	 *///-------------------------------------
	static const int replySize = 2;

public:
	EmptyLuxCommandReply()
	  : LuxCommandReply<cmdId>()
	{}
	virtual ~EmptyLuxCommandReply() {}

public:
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
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh)
	{
		const int64_t startPos = streamposToInt64(is.tellg());

		readLE(is, CommandReplyBase::m_commandId);

		return !is.fail()
		       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
		       && this->getSerializedSize() == dh.getMessageSize();
	}

	//========================================
	/*!\brief Serialize data into the given stream \a os.
	 * \param[out] os  Stream that receive the serialized
	 *                 data from this CommandSetFilter.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const
	{
		const int64_t startPos = streamposToInt64(os.tellp());

		writeLE(os, CommandReplyBase::m_commandId);

		return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
	}

	//========================================
	/*!\brief Get the DataType of this DataBlock.
	 * \return Always DataType#DataType_Command.
	 *///-------------------------------------
	virtual DataTypeId getDataType() const { return CommandReplyBase::dataType; }

	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization
	 *         of this Command.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const { return std::streamsize(replySize); }

public:
	virtual bool callListener(DataListenerBase&) const { return false; }

public:
	virtual bool deserializeFromStream(std::istream& is, const IbeoDataHeader& dh)
	{
		return deserialize(is, dh);
	}
}; // EmptyLuxCommandReply

//======================================================================
//======================================================================
//======================================================================

template<CommandId::Command cmdId>
class EmptyMiniLuxCommandReply : public MiniLuxCommandReply<cmdId>,
                                 public DataBlock {
public:
	//========================================
	/*!\brief Length of the reply.
	 *///-------------------------------------
	static const int replySize = 2;

public:
	EmptyMiniLuxCommandReply()
	  : MiniLuxCommandReply<cmdId>()
	{}
	virtual ~EmptyMiniLuxCommandReply() {}

public:
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
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh)
	{
		const int64_t startPos = streamposToInt64(is.tellg());

		readLE(is, CommandReplyBase::m_commandId);

		return !is.fail()
		       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
		       && this->getSerializedSize() == dh.getMessageSize();
	}

	//========================================
	/*!\brief Serialize data into the given stream \a os.
	 * \param[out] os  Stream that receive the serialized
	 *                 data from this CommandSetFilter.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const
	{
		const int64_t startPos = streamposToInt64(os.tellp());

		writeLE(os, CommandReplyBase::m_commandId);

		return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
	}

	//========================================
	/*!\brief Get the DataType of this DataBlock.
	 * \return Always DataType#DataType_Command.
	 *///-------------------------------------
	virtual DataTypeId getDataType() const { return CommandReplyBase::dataType; }

	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization
	 *         of this Command.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const { return std::streamsize(replySize); }

public:
	virtual bool callListener(DataListenerBase&) const { return false; }

public:
	virtual bool deserializeFromStream(std::istream& is, const IbeoDataHeader& dh)
	{
		return deserialize(is, dh);
	}
}; // EmptyMiniLuxCommandReply

//======================================================================

typedef EmptyEcuCommandReply<CommandId::CmdManagerSetFilter>          ReplyEcuSetFilter;
typedef EmptyEcuCommandReply<CommandId::CmdManagerAppBaseCtrl>        ReplyEcuAppBaseCtrl;
typedef EmptyLuxCommandReply<CommandId::CmdLuxSaveConfig>             ReplyLuxSaveConfig;
typedef EmptyLuxCommandReply<CommandId::CmdLuxSetParameter>           ReplyLuxSetParameter;
typedef EmptyLuxCommandReply<CommandId::CmdLuxResetDefaultParameters> ReplyLuxResetDefaultParameters;
typedef EmptyLuxCommandReply<CommandId::CmdLuxSetNTPTimestampSync>    ReplyLuxSetNTPTimestampSync;
typedef EmptyMiniLuxCommandReply<CommandId::CmdLuxSaveConfig>             ReplyMiniLuxSaveConfig;
typedef EmptyMiniLuxCommandReply<CommandId::CmdLuxSetParameter>           ReplyMiniLuxSetParameter;
typedef EmptyMiniLuxCommandReply<CommandId::CmdLuxResetDefaultParameters> ReplyMiniLuxResetDefaultParameters;
typedef EmptyMiniLuxCommandReply<CommandId::CmdLuxSetNTPTimestampSync>    ReplyMiniLuxSetNTPTimestampSync;

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_EMPTYCOMMANDREPLY_HPP_SEEN

//======================================================================
