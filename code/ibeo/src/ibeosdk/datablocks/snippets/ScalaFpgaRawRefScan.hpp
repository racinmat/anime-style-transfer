//======================================================================
/*! \file ScalaFpgaRawRefScan.hpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SCALAFPGARAWREFSCAN_HPP_SEEN
#define IBEOSDK_SCALAFPGARAWREFSCAN_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/ScalaFpgaRawScanPointBase.hpp>
#include <ibeosdk/inttypes.hpp>

#include <ibeosdk/misc/deprecatedwarning.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ScalaFpgaRawRefScan : public ScalaFpgaRawScanPointBase {
public:
	ScalaFpgaRawRefScan();
	virtual ~ScalaFpgaRawRefScan();

public:
	//! Equality predicate
	bool operator==(const ScalaFpgaRawRefScan& other) const;
	bool operator!=(const ScalaFpgaRawRefScan& other) const;

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
}; // ScalaFpgaRawRefScan

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCALAFPGARAWREFSCAN_HPP_SEEN

//======================================================================

