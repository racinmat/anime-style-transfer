//======================================================================
/*! \file FrameIndex.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date May 11, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_FRAMEINDEX_HPP_SEEN
#define IBEOSDK_FRAMEINDEX_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/FramingPolicy.hpp>
#include <ibeosdk/datablocks/snippets/FrameIndexEntry.hpp>
#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/Time.hpp>
#include <vector>
#include <string>

//======================================================================

namespace ibeosdk {

//======================================================================

class FrameIndex : public RegisteredDataBlock<FrameIndex> {
public:
	typedef std::vector<FrameIndexEntry> FrameVector;

public:
	FrameIndex();
	virtual ~FrameIndex();

public:
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	const FramingPolicy& getFramingPolicy() const { return m_framingPolicy; }

	const FrameVector& getFrameIndices() const { return m_frameIndices; }
	FrameVector getFrameIndices() { return m_frameIndices; }

public:
	void setFramingPolicy(const FramingPolicy& policy) { m_framingPolicy = policy; }
	void setFrameIndices(const FrameVector& frameIndices) { m_frameIndices = frameIndices; }

public:
	void addFrame(const FrameIndexEntry& entry)
	{
		m_frameIndices.push_back(entry);
		m_timeOffsetMs = entry.getTimeOffsetMs();
	}

public:
	void clearFrames() { m_frameIndices.clear(); }

public:
	static const UINT8 majorVersion;
	static const UINT8 minorVersion;
	static const std::string nameString;
	static const UINT32 lengthOfNameString;

protected:
	FramingPolicy m_framingPolicy;  ///< The FramingPolicy that has been used to generate the FrameIndex
	FrameVector   m_frameIndices;   ///< entries in this FrameIndex

protected:
	uint64_t m_timeOffsetMs;  ///< time offset of this frame in milliseconds
}; // FrameIndex

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_FRAMEINDEX_HPP_SEEN

//======================================================================
