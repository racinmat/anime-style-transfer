//======================================================================
/*! \file OxtsMessage.hpp
 *\verbatim
 * ------------------------------------------
 *  (C) 2015 Ibeo Automotive Systems GmbH, Hamburg, Germany
 * ------------------------------------------
 *
 *  Created on: Jun 30, 2014
 *          by: Kai-Uwe von Deylen (kd)
 *\endverbatim
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_OXTSMESSAGE_HPP_SEEN
#define IBEOSDK_OXTSMESSAGE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class OxtsMessage : public RegisteredDataBlock<OxtsMessage> {
public:
	OxtsMessage();

public: // implement interface of RegisteredDataBlock
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	UINT32 getIpAddress() const { return m_ipAddress; }
	const std::vector<UINT8>& getData() const { return m_data; }
	std::vector<UINT8>& getData() { return m_data; }

public: // setter
	void setIpAddress(const UINT32 ipAddress) { m_ipAddress = ipAddress; }

protected:
	UINT32 m_ipAddress;
	std::vector<UINT8> m_data;
}; // OxtsMessage

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OXTSMESSAGE_HPP_SEEN

//======================================================================

