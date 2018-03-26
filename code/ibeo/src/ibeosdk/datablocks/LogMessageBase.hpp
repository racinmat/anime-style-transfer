//======================================================================
/*! \file LogMessageBase.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 30, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LOGMESSAGEBASE_HPP_SEEN
#define IBEOSDK_LOGMESSAGEBASE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>

#include <ibeosdk/io.hpp>
#include <ibeosdk/Log.hpp>

#include <string>
#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

template<class LogMessageImpl>
class LogMessageBase : public ibeosdk::RegisteredDataBlock<LogMessageImpl> {
public:
	/// Level of trace output
	enum TraceLevel {
		TL_Off     = 0,  ///< No trace output
		TL_Error   = 1,  ///< Show errors only (minimal trace output)
		TL_Warning = 2,  ///< Show errors and warnings
		TL_Note    = 3,  ///< Show errors, warnings, and notes
		TL_Debug   = 4   ///< Show errors, warnings, notes, and debug messages
	};

protected:
	//========================================
	/*!\brief Constructor.
	 * \param[in] traceLevel  TraceLevel of this LogMessage.
	 * \param[in] msg         Content of this LogMessage.
	 *///-------------------------------------
	LogMessageBase(const TraceLevel traceLevel, const std::string msg)
	  : RegisteredDataBlock<LogMessageImpl>(),
	    m_traceLevel(traceLevel), m_message(msg)
	{}

	virtual ~LogMessageBase() {}

public:
	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization
	 *         of this data class.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const { return std::streamsize(1 + m_message.size()* sizeof(std::string::value_type) + 1); }

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
	 * \retval true Everything is alright, no error occured.
	 * \retval false Reading the data from the stream has failed.
	 *///-------------------------------------
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);

	//========================================
	/*!\brief Serialize data into the given stream \a os.
	 * \param[out] os  Stream that receive the serialized
	 *                 data from this class.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const;

	//========================================
	/*!\brief get the DataType of this DataBlock.
	 * \return DataType of this DataBlock.
	 *///-------------------------------------
	virtual DataTypeId getDataType() const { return ibeosdk::RegisteredDataBlock<LogMessageImpl>::dataBlockId; }

public:
	TraceLevel getTraceLevel() const { return m_traceLevel; }
	const std::string& getMessage() const { return m_message; }

public:
	void setTraceLevel(const TraceLevel newTraceLevel) { m_traceLevel = newTraceLevel; }
	void setMessage(const std::string& newMessage) { m_message = newMessage; }

protected:
	//========================================
	/*!\brief Convert all letters in \a inStr to ASCII letters.
	 * \param[in] inStr  A vector containing characters to be
	 *                   converted to a string that contains only
	 *                   ASCII characters.
	 * \return The ASCII version of the string given in \a inStr.
	 *///-------------------------------------
	static std::string toASCII(const std::vector<char>& inStr)
	{
		std::string outStr;
		std::transform(inStr.begin(), inStr.end(),
		               std::back_inserter(outStr),
		               toascii);
		return outStr;
	}

private:
	//========================================
	/*!\brief TraceLevel of this LogMessage.
	 *///-------------------------------------
	TraceLevel m_traceLevel;

	//========================================
	/*!\brief Content of this LogMessage.
	 *///-------------------------------------
	std::string m_message;
}; // LogMessageBase

//======================================================================

//======================================================================

template<class LogMessageImpl>
bool LogMessageBase<LogMessageImpl>::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	Concurrency::lock();

	uint8_t tl;
	ibeosdk::readBE(is, tl);
	m_traceLevel = TraceLevel(tl);

	int nbOfPoppedCharacters = 0;

	if (dh.getMessageSize()==1) {
		m_message.clear();
		if ((streamposToInt64(is.tellg()) - startPos) != this->getSerializedSize()) {
			Concurrency::unlock();
			return false;
		}
	}
	else {
		std::vector<char> buf(dh.getMessageSize()-1);
#if __linux || (_MSC_VER > 1500)
		is.read(buf.data(), dh.getMessageSize()-1);
#else
		is.read(&buf[0], dh.getMessageSize()-1);
#endif

		if ((streamposToInt64(is.tellg()) - startPos) != dh.getMessageSize()) {
			Concurrency::unlock();
			return false;
		}

		while (!buf.empty() && (buf.back() == std::string::value_type(0) || buf.back() == std::string::value_type('\n'))) {
			buf.pop_back();
			++nbOfPoppedCharacters;
		}
		m_message = toASCII(buf);
	}

	Concurrency::unlock();

	// take into account those characters that have been popped
	// '0' is always popped and has is already taken into account
	// by getSerializedSize().
	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == (this->getSerializedSize() + (nbOfPoppedCharacters-1)));
}

//======================================================================

template<class LogMessageImpl>
bool LogMessageBase<LogMessageImpl>::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	Concurrency::lock();

	const uint8_t tl = uint8_t(m_traceLevel);
	ibeosdk::writeBE(os, tl);
	os.write(m_message.c_str(), std::streamsize(m_message.size()));

	Concurrency::unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_LOGMESSAGEBASE_HPP_SEEN

//======================================================================
