//======================================================================
/*! \file FrameIndexEntry.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date May 11, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_FRAMEINDEXENTRY_HPP_SEEN
#define IBEOSDK_FRAMEINDEXENTRY_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/Time.hpp>
#include <ibeosdk/inttypes.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class FrameIndexEntry : public Snippet {

public:
	static std::streamsize getSerializedSize_static();

public:
	FrameIndexEntry();
	FrameIndexEntry(UINT64 filePos, UINT64 timeOffsetMs, UINT8 deviceId);
	virtual ~FrameIndexEntry();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	UINT64 getFilePosition() const { return m_filePosition; }
	UINT64 getTimeOffsetMs() const { return m_timeOffsetMs; }
	UINT8 getDeviceId() const { return m_deviceId; }

public:
	void setFilePosition(const UINT64 filePosition)  { m_filePosition = filePosition; }
	void setTimeOffsetMs(const UINT64 timeOffsetMs)  { m_timeOffsetMs = timeOffsetMs; }
	void setDeviceId(const UINT8 deviceId)  { m_deviceId = deviceId; }

protected:
	UINT64 m_filePosition;  ///< position of the frame in the file
	UINT64 m_timeOffsetMs;  ///< time offset of this frame in milliseconds
	UINT8 m_deviceId;      ///< device id that defined the frame
}; // FrameIndexEntry

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_FRAMEINDEXENTRY_HPP_SEEN

//======================================================================
