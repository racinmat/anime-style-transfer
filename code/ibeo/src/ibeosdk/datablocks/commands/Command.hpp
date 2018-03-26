//======================================================================
/*! \file Command.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 5, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_COMMAND_HPP_SEEN
#define IBEOSDK_COMMAND_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/CommandId.hpp>
#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/DataTypeId.hpp>
#include <ibeosdk/datablocks/snippets/IbeoDataHeader.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\class Command
 * \brief Abstract base class for all Command classes.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Sep 5, 2013
 *///-------------------------------------------------------------------
class CommandBase {
public:
	//========================================
	/*!\brief Create a Command object.
	 * \param[in] commandId      Id of the command.
	 *///-------------------------------------
	CommandBase(const CommandId commandId)
	  : m_commandId(commandId)
	{}

	virtual ~CommandBase() {}

public:
	//========================================
	/*!\brief Get the id of this Command.
	 * \return the id of this Command.
	 *///-------------------------------------
	CommandId getCommandId() const { return m_commandId; }

public:
	virtual std::streamsize getCommandLength() const = 0;
	virtual bool serializeToStream(std::ostream& os) const = 0;

protected:
	static const DataTypeId dataType;

protected:
	//========================================
	/*!\brief The id of this Command.
	 *///-------------------------------------
	CommandId m_commandId;
}; // CommandBase

//======================================================================

class LuxCommandBase : public CommandBase {
public:
	LuxCommandBase(const CommandId commandId)
	  : CommandBase(commandId)
	{}
}; // LuxCommandBase

//======================================================================

class MiniLuxCommandBase : public CommandBase {
public:
	MiniLuxCommandBase(const CommandId commandId)
	  : CommandBase(commandId)
	{}
}; // MiniLuxCommandBase

//======================================================================

class EcuCommandBase : public CommandBase {
public:
	EcuCommandBase(const CommandId commandId)
	  : CommandBase(commandId)
	{}
}; // EcuCommandBase

//======================================================================
//======================================================================
//======================================================================

template<CommandId::Command cId>
class LuxCommand : public LuxCommandBase {
public:
	LuxCommand()
	  : LuxCommandBase(cId)
	{}
}; // LuxCommand<cId>

//======================================================================

template<CommandId::Command cId>
class MiniLuxCommand : public MiniLuxCommandBase {
public:
	MiniLuxCommand()
	  : MiniLuxCommandBase(cId)
	{}
}; // LuxCommand<cId>

//======================================================================

template<CommandId::Command cId>
class EcuCommand : public EcuCommandBase {
public:
	EcuCommand()
	  : EcuCommandBase(cId)
	{}
}; // LuxCommand<cId>

//======================================================================
//======================================================================
//======================================================================
/*!\class CommandReply
 * \brief Abstract base class for all CommandReply classes.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Sep 5, 2013
 *
 *///-------------------------------------------------------------------
class CommandReplyBase  {
public:
	static const uint16_t errorMask = 0x8000;

public:
	//========================================
	/*!\brief Create a Command object.
	 * \param[in] commandId      Id of the command.
	 *///-------------------------------------
	CommandReplyBase(const CommandId commandId)
	  : m_commandId(commandId)
	{}

	virtual ~CommandReplyBase() {}

public:
	//========================================
	/*!\brief Get the id of this Command.
	 * \return the id of this Command.
	 *///-------------------------------------
	CommandId getCommandId() const { return m_commandId; }

	virtual bool deserializeFromStream(std::istream& is, const IbeoDataHeader& dh) = 0;

public:
	bool isErrorReply() const { return (getCommandId() & errorMask) != 0; }
	void setErrorReply() { m_commandId = CommandId(uint16_t(m_commandId) | errorMask); }
	void unsetErrorReply() { m_commandId = CommandId(uint16_t(uint16_t(m_commandId) & uint16_t(~errorMask))); }


protected:
	static const DataTypeId dataType;

protected:
	//========================================
	/*!\brief The id of this Command.
	 *///-------------------------------------
	CommandId m_commandId;
}; // CommandReplyBase

//======================================================================

class LuxCommandReplyBase : public CommandReplyBase {
public:
	LuxCommandReplyBase(const CommandId commandId)
	  : CommandReplyBase(commandId)
	{}
}; // LuxCommandReplyBase

//======================================================================

class MiniLuxCommandReplyBase : public CommandReplyBase {
public:
	MiniLuxCommandReplyBase(const CommandId commandId)
	  : CommandReplyBase(commandId)
	{}
}; // MiniLuxCommandReplyBase

//======================================================================

class EcuCommandReplyBase : public CommandReplyBase {
public:
	EcuCommandReplyBase(const CommandId commandId)
	  : CommandReplyBase(commandId)
	{}
}; // EcuCommandReplyBase

//======================================================================
//======================================================================
//======================================================================

template<CommandId::Command cId>
class LuxCommandReply : public LuxCommandReplyBase {
public:
	LuxCommandReply()
	  : LuxCommandReplyBase(cId)
	{}
}; // LuxCommandReply<cId>

template<CommandId::Command cId>
class MiniLuxCommandReply : public MiniLuxCommandReplyBase {
public:
	MiniLuxCommandReply()
	  : MiniLuxCommandReplyBase(cId)
	{}
}; // MiniLuxCommandReply<cId>

//======================================================================

template<CommandId::Command cId>
class EcuCommandReply : public EcuCommandReplyBase {
public:
	EcuCommandReply()
	  : EcuCommandReplyBase(cId)
	{}
}; // LuxCommandReply<cId>



//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_COMMAND_HPP_SEEN

//======================================================================
