//======================================================================
/*! \file ReplyMiniLuxGetParameter.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 10, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_REPLYMINILUXGETPARAMETER_HPP_SEEN
#define IBEOSDK_REPLYMINILUXGETPARAMETER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/commands/Command.hpp>
#include <ibeosdk/datablocks/DataBlock.hpp>
#include <ibeosdk/misc/ParameterIndex.hpp>
#include <ibeosdk/misc/ParameterData.hpp>
#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/DataTypeId.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class ReplyMiniLuxGetParameter
 * \brief
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Apr 10, 2015
 *///-------------------------------------------------------------------
class ReplyMiniLuxGetParameter : public ibeosdk::MiniLuxCommandReply<CommandId::CmdLuxGetParameter>,
                                 public ibeosdk::DataBlock {
public:
	//========================================
	/*!\brief Length of the CommandManagerAppBaseStatus command.
	 *///-------------------------------------
	static const int replySize = 8;

public:
	ReplyMiniLuxGetParameter();

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

public:
	ParameterIndex getParameterIndex() const { return m_parameterIndex; }
	ParameterData getParameterData() const { return m_parameterData; }

protected:
	ParameterIndex m_parameterIndex;
	ParameterData m_parameterData;
}; // ReplyMiniLuxGetParameter

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_REPLYMINILUXGETPARAMETER_HPP_SEEN

//======================================================================
