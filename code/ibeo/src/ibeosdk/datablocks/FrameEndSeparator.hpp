//======================================================================
/*! \file FrameEndSeparator.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Nov 26, 2013
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_FRAMEENDSEPARATOR_HPP_SEEN
#define IBEOSDK_FRAMEENDSEPARATOR_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class FrameEndSeparator : public RegisteredDataBlock<FrameEndSeparator> {
public:
	static std::streamsize getSerializedSize_static();

public:
	FrameEndSeparator();
	virtual ~FrameEndSeparator();

public:
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	UINT32 getFrameId() const { return m_frameId; }
	UINT32 getSizeOfThisFrame() const { return m_sizeOfThisFrame; }
	UINT32 getSizeOfNextFrame() const { return m_sizeOfNextFrame; }
	NTPTime getCreationTime() const { return m_creationTime; }

public:
	void setFrameId(const UINT32 newFrameId)  { m_frameId = newFrameId; }
	void setSizeOfThisFrame(const UINT32 newSizeOfThisFrame)  { m_sizeOfThisFrame = newSizeOfThisFrame; }
	void setSizeOfNextFrame(const UINT32 newSizeOfNextFrame)  { m_sizeOfNextFrame = newSizeOfNextFrame; }
	void setCreationTime(const NTPTime newCreationTime)  { m_creationTime = newCreationTime; }

public:
	static const int nbOfReserved = 6;

protected:
	UINT32 m_frameId;
	UINT32 m_sizeOfThisFrame;
	UINT32 m_sizeOfNextFrame;
	NTPTime m_creationTime;
	UINT16 m_reserved[nbOfReserved];
}; // FrameEndSeparator

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_FRAMEENDSEPARATOR_HPP_SEEN

//======================================================================
