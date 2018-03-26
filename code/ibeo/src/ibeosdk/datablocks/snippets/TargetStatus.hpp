//======================================================================
/*! \file TargetStatus.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Jun 30, 2014
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_TARGETSTATUS_HPP_SEEN
#define IBEOSDK_TARGETSTATUS_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/ResolutionInfo.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/inttypes.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class TargetStatus : public Snippet {
public:
	TargetStatus();
	TargetStatus(const TargetStatus& src);
	virtual ~TargetStatus();

	TargetStatus& operator= (const TargetStatus& src);

public:
	static std::streamsize getSerializedSize_static();

public:
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	UINT32 getIpAddress() const { return m_ipAddress; }
	UINT8 getStatus() const { return m_status; }
	float getVisibilityLaser() const { return m_visibilityLaser; }
	float getOcclusionLaser() const { return m_occlusionLaser; }
	float getVisibilityDut() const { return m_visibilityDut; }
	float getOcclusionDut() const { return m_occlusionDut; }
	float getParamImuYaw() const { return m_paramImuYaw; }
	float getParamImuPitch() const { return m_paramImuPitch; }
	float getParamImuRoll() const { return m_paramImuRoll; }
	float getParamImuYawAccuracy() const { return m_paramImuYawAccuracy; }
	float getParamImuPitchAccuracy() const { return m_paramImuPitchAccuracy; }
	float getParamImuRollAccuracy() const { return m_paramImuRollAccuracy; }
	float getParamGpsX() const { return m_paramGpsX; }
	float getParamGpsY() const { return m_paramGpsY; }
	float getParamGpsZ() const { return m_paramGpsZ; }
	float getParamGpsXAccuracy() const { return m_paramGpsXAccuracy; }
	float getParamGpsYAccuracy() const { return m_paramGpsYAccuracy; }
	float getParamGpsZAccuracy() const { return m_paramGpsZAccuracy; }

public: // setter
	void setIpAddress(const UINT32 ipAddress) { m_ipAddress = ipAddress; }
	void setStatus(const UINT8 status) { m_status = status; }
	void setVisibilityLaser(const float visibilityLaser) { m_visibilityLaser = visibilityLaser; }
	void setOcclusionLaser(const float occlusionLaser) { m_occlusionLaser = occlusionLaser; }
	void setVisibilityDut(const float visibilityDut) { m_visibilityDut = visibilityDut; }
	void setOcclusionDut(const float occlusionDut) { m_occlusionDut = occlusionDut; }
	void setParamImuYaw(const float paramImuYaw) { m_paramImuYaw = paramImuYaw; }
	void setParamImuPitch(const float paramImuPitch) { m_paramImuPitch = paramImuPitch; }
	void setParamImuRoll(const float paramImuRoll) { m_paramImuRoll = paramImuRoll; }
	void setParamImuYawAccuracy(const float paramImuYawAccuracy) { m_paramImuYawAccuracy = paramImuYawAccuracy; }
	void setParamImuPitchAccuracy(const float paramImuPitchAccuracy) { m_paramImuPitchAccuracy = paramImuPitchAccuracy; }
	void setParamImuRollAccuracy(const float paramImuRollAccuracy) { m_paramImuRollAccuracy = paramImuRollAccuracy; }
	void setParamGpsX(const float paramGpsX) { m_paramGpsX = paramGpsX; }
	void setParamGpsY(const float paramGpsY) { m_paramGpsY = paramGpsY; }
	void setParamGpsZ(const float paramGpsZ) { m_paramGpsZ = paramGpsZ; }
	void setParamGpsXAccuracy(const float paramGpsXAccuracy) { m_paramGpsXAccuracy = paramGpsXAccuracy; }
	void setParamGpsYAccuracy(const float paramGpsYAccuracy) { m_paramGpsYAccuracy = paramGpsYAccuracy; }
	void setParamGpsZAccuracy(const float paramGpsZAccuracy) { m_paramGpsZAccuracy = paramGpsZAccuracy; }

protected:
	UINT32 m_ipAddress;
	UINT8 m_status;
	float m_visibilityLaser;
	float m_occlusionLaser;
	float m_visibilityDut;
	float m_occlusionDut;
	float m_paramImuYaw;
	float m_paramImuPitch;
	float m_paramImuRoll;
	float m_paramImuYawAccuracy;
	float m_paramImuPitchAccuracy;
	float m_paramImuRollAccuracy;
	float m_paramGpsX;
	float m_paramGpsY;
	float m_paramGpsZ;
	float m_paramGpsXAccuracy;
	float m_paramGpsYAccuracy;
	float m_paramGpsZAccuracy;
}; // ScannerInfo

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_TARGETSTATUS_HPP_SEEN

//======================================================================
