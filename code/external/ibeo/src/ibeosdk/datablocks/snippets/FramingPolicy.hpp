//======================================================================
/*! \file FramingPolicy.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date May 11, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_FRAMINGPOLICY_HPP_SEEN
#define IBEOSDK_FRAMINGPOLICY_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/DataTypeId.hpp>
#include <ibeosdk/inttypes.hpp>
#include <map>

//======================================================================

namespace ibeosdk {

//======================================================================

class FramingPolicy : public ibeosdk::Snippet {

public:
	typedef std::pair<ibeosdk::DataTypeId, uint8_t> Trigger;
	typedef std::map<ibeosdk::DataTypeId, uint8_t> TriggerSet;

public:
	static const uint8_t deviceIdAny;

public:
	FramingPolicy();
	virtual ~FramingPolicy();

public:
	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization
	 *         of this data class.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const;

	//========================================
	/*!\brief Deserialize data from the given stream \a is into
	 *        this class.
	 * \param[in, out] is  Stream that provides the serialized
	 *                     data to fill this class.
	 *                     On exit the \a is get pointer will
	 *                     be behind the read data.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Reading the data from the stream has failed.
	 *///-------------------------------------
	virtual bool deserialize(std::istream& is);

	//========================================
	/*!\brief Serialize data into the given stream \a os.
	 * \param[out] os  Stream that receive the serialized
	 *                 data from this class.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const;

public:
	bool getTriggerInNewFrame() const { return m_triggerInNewFrame; }
	const TriggerSet& getTriggers() const { return m_triggers; }
	TriggerSet& getTriggers() { return m_triggers; }

public:
	void setTriggerInNewFrame(const bool triggerInNewFrame)  { m_triggerInNewFrame = triggerInNewFrame; }
	void setTriggers(const TriggerSet& triggers)  { m_triggers = triggers; }

public:
	//========================================
	/*!\brief Returns true if the given dataType and fromDeviceID is the trigger for a new frame
	 *///-------------------------------------
	bool isTrigger(const ibeosdk::DataTypeId dataType, const UINT8 fromDeviceID) const;

protected:
	bool        m_triggerInNewFrame;  ///< states if a trigger message belongs to next or prev frame (true = next)
	TriggerSet  m_triggers;           ///< contains all trigger datatypes
}; // FramingPolicy

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_FRAMINGPOLICY_HPP_SEEN

//======================================================================
