//======================================================================
/*! \file PointCloudBase.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 15, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/PointCloudBase.hpp>

#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//==============================================================================

PointCloudBase::PointCloudBase()
	: m_kind(pointKind::PointKindUndefined)
	, m_type(pointType::PointTypePoint)
	, m_refPlane(ReferencePlane())
{}

//==============================================================================

bool PointCloudBase::operator==(const PointCloudBase& other) const
{
	return (m_kind       == other.m_kind) &&
	       (m_type       == other.m_type) &&
	       (m_refPlane   == other.m_refPlane);
}

//==============================================================================

pointKind::PointKind PointCloudBase::stringToKind(const std::string& kind)
{
	if       (kind == "LanePoint" ) { return pointKind::PointKindLanePoint; }
	else if  (kind == "ScanPoint" ) { return pointKind::PointKindScanPoint; }
	else                            { return pointKind::PointKindUndefined; }
}

//==============================================================================

std::string PointCloudBase::kindToString(const pointKind::PointKind kind)
{
	if       (kind == pointKind::PointKindLanePoint)  { return "LanePoint"; }
	else if  (kind == pointKind::PointKindScanPoint)  { return "ScanPoint"; }
	else                                              { return "Undefined"; }
}

//======================================================================

std::streamsize PointCloudBase::getSerializedSize() const
{
	return std::streamsize(2*sizeof(UINT16) + sizeof(UINT64))+ m_refPlane.getSerializedSize();
}

//======================================================================

bool PointCloudBase::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	UINT16 type, kind;

	ibeosdk::readBE(is, kind);
	ibeosdk::readBE(is, type);
	m_refPlane.deserialize(is);

	UINT64 reserved;
	ibeosdk::readBE(is, reserved);

	m_type = pointType::PointType(type);
	m_kind = pointKind::PointKind(kind);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool PointCloudBase::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, UINT16(m_kind));
	ibeosdk::writeBE(os, UINT16(m_type));
	m_refPlane.serialize(os);

	ibeosdk::writeBE(os, UINT64(0));

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//==============================================================================

} // namespace ibeosdk

//==============================================================================
