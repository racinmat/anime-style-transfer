//======================================================================
/*! \file ResolutionInfo.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 26, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_RESOLUTIONINFO_HPP_SEEN
#define IBEOSDK_RESOLUTIONINFO_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ResolutionInfo : public Snippet {
public:
	ResolutionInfo();
	ResolutionInfo(const ResolutionInfo& src);
	virtual ~ResolutionInfo();

public:
	ResolutionInfo& operator= (const ResolutionInfo& src);

public:
	static std::streamsize getSerializedSize_static();

public:
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }

	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	float getResolutionStartAngle() const { return m_resolutionStartAngle; }
	float getResolution() const { return m_resolution; }

public: // setter
	void setResolutionStartAngle(const float newResolutionStartAngle) { m_resolutionStartAngle = newResolutionStartAngle; }
	void setResolution(const float newResolution) { m_resolution= newResolution; }
	
public:
	bool operator==(const ResolutionInfo& other) const;
	bool operator!=(const ResolutionInfo& other) const { return !((*this) == other); }

protected:
	float m_resolutionStartAngle; // TODO: define FLOAT32
	float m_resolution; // TODO: define FLOAT32
}; // ResolutionInfo

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_RESOLUTIONINFO_HPP_SEEN

//======================================================================

