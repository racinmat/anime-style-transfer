//======================================================================
/*! \file PointCloudGlobal7500.cpp
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

#include <ibeosdk/datablocks/PointCloudGlobal7500.hpp>

#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<PointCloudGlobal7500>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<PointCloudGlobal7500>::dataBlockId = DataTypeId(DataTypeId::DataType_PointCloudGlobal7500);
template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<PointCloudGlobal7500>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<PointCloudGlobal7500>::dataBlockId, ibeosdk::RegisteredDataBlock<PointCloudGlobal7500>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<PointCloudGlobal7500>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================

std::streamsize PointCloudGlobal7500::getSerializedSize() const
{
	return PointCloudBase::getSerializedSize()
	       + std::streamsize(sizeof(UINT32))
	       + std::streamsize(m_points.size()) * (GlobalPoint::getSerializedSize_static()
	                            + PointBase::getSerializedSizeWithType_static(m_type));
}

//======================================================================

DataTypeId PointCloudGlobal7500::getDataType() const { return dataBlockId; }

//======================================================================

bool PointCloudGlobal7500::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	PointCloudBase::deserialize(is);

	UINT32 nPoints;
	ibeosdk::readBE(is, nPoints);

	m_points.resize(nPoints);

	for (std::vector<GlobalPoint>::iterator pointIter = m_points.begin(); pointIter != m_points.end(); ++pointIter)
	{
		pointIter->deserialize(is);
		pointIter->deserializeWithType(is, m_type);
	}

	unlock();

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	                  && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool PointCloudGlobal7500::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	PointCloudBase::serialize(os);

	ibeosdk::writeBE(os, UINT32(m_points.size()));

	for (std::vector<GlobalPoint>::const_iterator pointIter = m_points.begin(); pointIter != m_points.end(); ++pointIter)
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
