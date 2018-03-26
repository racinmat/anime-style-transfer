//======================================================================
/*! \file ScalaFpgaRawScanPoint.hpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SCALAFPGARAWSCANPOINT_HPP_SEEN
#define IBEOSDK_SCALAFPGARAWSCANPOINT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/ScalaFpgaRawScanPointBase.hpp>

#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <istream>
#include <ostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class ScalaFpgaRawScanPoint : public ScalaFpgaRawScanPointBase {
public:
	ScalaFpgaRawScanPoint();
	virtual ~ScalaFpgaRawScanPoint();

public:
	//! Equality predicate
	bool operator==(const ScalaFpgaRawScanPoint& other) const;
	bool operator!=(const ScalaFpgaRawScanPoint& other) const;

public:
	virtual uint16_t getBlockId() const { return blockId; }

public:
	IBEOSDK_DEPRECATED uint8_t getChannelID() const { return m_channelId; }
	IBEOSDK_DEPRECATED uint8_t getEchoID() const { return m_echoId; }

public:
	IBEOSDK_DEPRECATED void setChannelID(const uint8_t id) { m_channelId = id; }
	IBEOSDK_DEPRECATED void setEchoID(const uint8_t id) { m_echoId = id; }

public:
	static const uint16_t blockId;
}; // ScalaFpgaRawScanPoint

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCALAFPGARAWSCANPOINT_HPP_SEEN

//======================================================================

