//======================================================================
/*! \file PointCloudPlane7510.cpp
 *\verbatim
 * ------------------------------------------
 *  (C) 2016 Ibeo Automotive Systems GmbH, Hamburg, Germany
 * ------------------------------------------
 *
 *  Created on: Mar 15, 2016
 *          by: Kristian Bischoff
 *\endverbatim
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/PointCloudPlane7510.hpp>

#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<PointCloudPlane7510>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<PointCloudPlane7510>::dataBlockId = DataTypeId(DataTypeId::DataType_PointCloudPlane7510);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<PointCloudPlane7510>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<PointCloudPlane7510>::dataBlockId, ibeosdk::RegisteredDataBlock<PointCloudPlane7510>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<PointCloudPlane7510>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

std::streamsize PointCloudPlane7510::getSerializedSize() const
{
	return PointCloudBase::getSerializedSize()
	       + std::streamsize(sizeof(UINT32))
	       + std::streamsize(m_points.size()) * (PlanePoint::getSerializedSize_static()
	                            + PointBase::getSerializedSizeWithType_static(m_type));
}

//======================================================================

DataTypeId PointCloudPlane7510::getDataType() const { return dataBlockId; }

//======================================================================

bool PointCloudPlane7510::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	PointCloudBase::deserialize(is);

	UINT32 nPoints;
	ibeosdk::readBE(is, nPoints);

	m_points.resize(nPoints);

	for (std::vector<PlanePoint>::iterator pointIter = m_points.begin(); pointIter != m_points.end(); ++pointIter)
	{
		pointIter->deserialize(is);
		pointIter->deserializeWithType(is, m_type);
	}

	unlock();

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	                  && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool PointCloudPlane7510::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	PointCloudBase::serialize(os);

	ibeosdk::writeBE(os, UINT32(m_points.size()));

	for (std::vector<PlanePoint>::const_iterator pointIter = m_points.begin(); pointIter != m_points.end(); ++pointIter)
	{
		pointIter->serialize(os);
		pointIter->serializeWithType(os, m_type);
	}

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
