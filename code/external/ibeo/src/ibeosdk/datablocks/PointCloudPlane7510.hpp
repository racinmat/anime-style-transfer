//======================================================================
/*! \file PointCloudPlane7510.hpp
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

#ifndef POINTCLOUDPLANE7510_HPP_SEEN
#define POINTCLOUDPLANE7510_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>

#include <ibeosdk/datablocks/snippets/PlanePoint.hpp>
#include <ibeosdk/datablocks/snippets/PointCloudBase.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class PointCloudPlane7510 : public RegisteredDataBlock<PointCloudPlane7510>,
                            public PointCloudBase
{
public:
	PointCloudPlane7510() : PointCloudBase() {}
	virtual ~PointCloudPlane7510() {}

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	virtual bool empty() const { return m_points.empty(); }

public:
	typedef CustomIterator<PlanePoint, std::vector<PlanePoint>::iterator, PlanePointProxy, PlanePointProxy, PointCloudPlane7510, PointCloudPlane7510> iterator;
	typedef CustomIterator<PlanePoint , std::vector<PlanePoint>::const_iterator, PlanePointProxy const, PlanePointProxy, PointCloudPlane7510 const, PointCloudPlane7510> const_iterator;

	virtual iterator begin() { return iterator(m_points.begin(), this); }
	virtual iterator end()   { return iterator(m_points.end(), this); }

	virtual const_iterator begin() const { return const_iterator(m_points.begin(), this); }
	virtual const_iterator end() const  { return const_iterator(m_points.end(), this); }

	void push_back(const PlanePoint& point) { m_points.push_back(point); }

private:
	std::vector<PlanePoint> m_points;
}; // PointCloudPlane7510

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // POINTCLOUDPLANE7510_HPP_SEEN

//======================================================================
