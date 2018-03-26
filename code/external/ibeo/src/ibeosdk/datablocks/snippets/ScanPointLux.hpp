//======================================================================
/*! \file ScanPointLux.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 25, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SCANPOINTLUX_HPP_SEEN
#define IBEOSDK_SCANPOINTLUX_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/inttypes.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ScanPointLux : public Snippet {
public:
	enum LuxScanPointFlags {
		LSPF_Transparent   = 0x01, //!< transparent point
		LSPF_Rain          = 0x02, //!< clutter (atmospheric)
		LSPF_Ground        = 0x04, //!< ground
		LSPF_Dirt          = 0x08  //!< dirt
		// 0x10, 0x20, 0x40, 0x80 are reserved
	}; // ScanPointFlags

public:
	ScanPointLux();
	ScanPointLux(const ScanPointLux& src);
	virtual ~ScanPointLux();

	ScanPointLux& operator= (const ScanPointLux& src);

public:
	static std::streamsize getSerializedSize_static();

public:
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	UINT8 getLayer() const { return m_layer; }
	UINT8 getEcho() const { return m_echo; }
	UINT8 getFlags() const { return m_flags; }
	INT16 getHorizontalAngle() const { return m_angle; }
	UINT16 getDistance() const { return m_distance; }
	UINT16 getEchoPulseWidth() const { return m_epw; }

public: // setter
	void setLayer(const UINT8 newLayer) { this->m_layer = newLayer; }
	void setEcho(const UINT8 newEcho) { this->m_echo = newEcho; }
	void setFlags(const UINT8 newFlags) { this->m_flags = newFlags; }
	void setHorizontalAngle(const INT16 newHorizontalAngle) { this->m_angle = newHorizontalAngle; }
	void setDistance(const UINT16 newDistance) { this->m_distance = newDistance; }
	void setEchoPulseWidth(const UINT16 newEchoPulseWidth) { this->m_epw = newEchoPulseWidth; }

private:
	UINT8 m_layer; // 4 bit used
	UINT8 m_echo;  // 4 bit used
	UINT8 m_flags; // combination of bits from LuxScanPointFlag
	INT16 m_angle;
	UINT16 m_distance; // cm
	UINT16 m_epw; // cm
	UINT16 m_reserved;
}; // ScanPointLux

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCANPOINTLUX_HPP_SEEN

//======================================================================

