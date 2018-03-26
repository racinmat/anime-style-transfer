//======================================================================
/*! \file ReplyEcuAppBaseStatus.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 8, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_REPLYMANAGERAPPBASESTATUS_HPP_SEEN
#define IBEOSDK_REPLYMANAGERAPPBASESTATUS_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/commands/Command.hpp>
#include <ibeosdk/datablocks/commands/AppBaseStatusDefinitions.hpp>

#include <ibeosdk/devices/IbeoEcu.hpp>

#include <ibeosdk/datablocks/DataBlock.hpp>
#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/DataTypeId.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class ReplyManagerAppBaseStatus
 * \brief
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Apr 7, 2015
 *///-------------------------------------------------------------------
class ReplyEcuAppBaseStatus : public EcuCommandReply<CommandId::CmdManagerAppBaseStatus>,
                              public AppBaseStatusDefinitions,
                              public DataBlock {
public:
	enum AppBaseStatusId {
		AppBaseStatusId_Recording = 0x01
	};
public:
	//========================================
	/*!\brief Length of the CommandManagerAppBaseStatus command.
	 *///-------------------------------------
	static const int commandSizeBase = 4;

public:
	ReplyEcuAppBaseStatus();

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
	virtual std::streamsize getSerializedSize() const { return std::streamsize(commandSizeBase + m_data.size()); }

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
	AppBaseStatusId getStatusId() { return m_statusId; }

	//========================================
	/*!\brief The status of the ECU recording.
	 * \return The status of the ECU recording.
	 * \retval '' The recording is not available,
	 *            an empty string will be returned.
	 * \retval '0' The recording is available but not active.
	 * \retval '1' The recording is available and active.
	 *///-------------------------------------
	std::string getData() { return m_data; }

protected:
	AppBaseStatusId m_statusId;
	std::string m_data;
}; // ReplyManagerAppBaseStatus

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_REPLYMANAGERAPPBASESTATUS_HPP_SEEN

//======================================================================
