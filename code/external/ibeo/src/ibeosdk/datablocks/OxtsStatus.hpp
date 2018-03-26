//======================================================================
/*! \file OxtsStatus.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Jun 30, 2014
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_OXTSSTATUS_HPP_SEEN
#define IBEOSDK_OXTSSTATUS_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/MountingPosition.hpp>
#include <ibeosdk/datablocks/snippets/TargetStatus.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class OxtsStatus : public RegisteredDataBlock<OxtsStatus> {
public:
	OxtsStatus();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	UINT8 getHunterStatus() const { return m_hunterStatus; }
	MountingPosition& getDutPosition() { return m_dutPosition; }
	const MountingPosition& getDutPosition() const { return m_dutPosition; }
	float getDutViewAngleHorizontal() const { return m_dutViewAngleHorizontal; }
	float getDutViewAngleVertical() const { return m_dutViewAngleVertical; }
	float getDutRange() const { return m_dutRange; }
	UINT8 getNbOfTargets() const { return UINT8(m_targetStatus.size()); }
	std::vector<TargetStatus>& getTargets() { return m_targetStatus; }
	const std::vector<TargetStatus>& getTargets() const { return m_targetStatus; }

public: // setter
	void setHunterStatus(const UINT8 hunterStatus) { m_hunterStatus = hunterStatus; }
	void setDutViewAngleHorizontal(const float dutViewAngleHorizontal) { m_dutViewAngleHorizontal = dutViewAngleHorizontal; }
	void setDutViewAngleVertical(const float dutViewAngleVertical) { m_dutViewAngleVertical = dutViewAngleVertical; }
	void setDutRange(const float dutRange) { m_dutRange = dutRange; }

protected:
	UINT8 m_hunterStatus;
	MountingPosition m_dutPosition;
	float m_dutViewAngleHorizontal;
	float m_dutViewAngleVertical;
	float m_dutRange;
	std::vector<TargetStatus> m_targetStatus;

}; // OxtsStatus

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_OXTSSTATUS_HPP_SEEN

//======================================================================

