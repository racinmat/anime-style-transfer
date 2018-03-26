//======================================================================
/*! \file PointCloudGlobal7500.hpp
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

#ifndef POINTCLOUDGLOBAL7500_HPP_SEEN
#define POINTCLOUDGLOBAL7500_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>

#include <ibeosdk/datablocks/snippets/GlobalPoint.hpp>
#include <ibeosdk/datablocks/snippets/PointCloudBase.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class PointCloudGlobal7500 : public RegisteredDataBlock<PointCloudGlobal7500>,
                             public PointCloudBase
{
public:
	PointCloudGlobal7500() : PointCloudBase() {}
	virtual ~PointCloudGlobal7500() {}

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	virtual bool empty() const { return m_points.empty(); }

public:
	typedef CustomIterator<GlobalPoint, std::vector<GlobalPoint>::iterator, GlobalPointProxy, GlobalPointProxy, PointCloudGlobal7500, PointCloudGlobal7500> iterator;
	typedef CustomIterator<GlobalPoint , std::vector<GlobalPoint>::const_iterator, GlobalPointProxy const, GlobalPointProxy, PointCloudGlobal7500 const, PointCloudGlobal7500> const_iterator;

	virtual iterator begin() { return iterator(m_points.begin(), this); }
	virtual iterator end()   { return iterator(m_points.end(), this); }

	virtual const_iterator begin() const { return const_iterator(m_points.begin(), this); }
	virtual const_iterator end() const  { return const_iterator(m_points.end(), this); }

	void push_back(const GlobalPoint& point) { m_points.push_back(point); }

private:
	std::vector<GlobalPoint> m_points;
}; // PointCloudGlobal7500

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // POINTCLOUDGLOBAL7500_HPP_SEEN

//======================================================================
