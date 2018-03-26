//======================================================================
/*! \file CommandEcuAppBaseCtrl.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 7, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_COMMANDMANAGERAPPBASECTRL_HPP_SEEN
#define IBEOSDK_COMMANDMANAGERAPPBASECTRL_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/commands/Command.hpp>
#include <ibeosdk/datablocks/DataBlock.hpp>
#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/DataTypeId.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class CommandManagerAppBaseCtrl
 * \brief
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Apr 7, 2015
 *///-------------------------------------------------------------------
class CommandEcuAppBaseCtrl : public EcuCommand<CommandId::CmdManagerAppBaseCtrl>,
                              public DataBlock {
public:
	enum AppBaseCtrlId {
		AppBaseCtrlId_Invalid        = 0x00,
		AppBaseCtrlId_StartRecording = 0x01,
		AppBaseCtrlId_StopRecording  = 0x02
	}; // AppBaseCtrlId

public:
	//========================================
	/*!\brief Length of the SetFilter command.
	 *///-------------------------------------
	static const int commandSizeBase = 4;

public:
//	CommandManagerAppBaseCtrl();
	CommandEcuAppBaseCtrl(const AppBaseCtrlId ctrlId);
	CommandEcuAppBaseCtrl(const AppBaseCtrlId ctrlId, const std::string& data);

public:
	AppBaseCtrlId getCtrlId() const { return this->m_ctrlId; }
	const std::string& getData() const { return this->m_data; }

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
	virtual std::streamsize getSerializedSize() const { return std::streamsize(commandSizeBase + m_data.length()); }

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

private:
	virtual bool callListener(DataListenerBase&) const { return false; }

protected:
	AppBaseCtrlId m_ctrlId;
	std::string m_data;
}; // CommandManagerAppBaseCtrl

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_COMMANDMANAGERAPPBASECTRL_HPP_SEEN

//======================================================================
