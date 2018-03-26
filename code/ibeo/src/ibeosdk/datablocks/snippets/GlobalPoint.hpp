//======================================================================
/*! \file GlobalPoint.hpp
 *\verbatim
 * ------------------------------------------
 *  (C) 2016 Ibeo Automotive Systems GmbH, Hamburg, Germany
 * ------------------------------------------
 *
 *  Created on: Mar 16, 2016
 *          by: Kristian Bischoff
 *\endverbatim
 *///-------------------------------------------------------------------
//======================================================================

#ifndef IBEOSDK_GLOBALPOINT_HPP_SEEN
#define IBEOSDK_GLOBALPOINT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/PointBase.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class PointCloudGlobal7500;
class GlobalPoint;

//======================================================================

class GlobalPointProxy : public PointProxy {
public:
	GlobalPointProxy(GlobalPoint* point, PointCloudGlobal7500* cloud);
	virtual ~GlobalPointProxy() {}

public:
	virtual GpsPoint getGlobalCoordinates() const;
	virtual Point3dFloat  getOffsetFromReferencePoint() const;
}; // GlobalPointProxy

//======================================================================

//======================================================================
//======================================================================

class GlobalPoint : public PointBase {
public:
	GlobalPoint() : PointBase() {}
	virtual ~GlobalPoint() {}

public:
	virtual bool operator==(const GlobalPoint& other) const;

public:
	static std::streamsize getSerializedSize_static();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	GpsPoint& gpsPoint() { return m_gpsPoint; }
	const GpsPoint& getGpsPoint() const { return m_gpsPoint; }

	void setGpsPoint( const GpsPoint& point) { m_gpsPoint = point; }

private:
	GpsPoint m_gpsPoint;
}; // GlobalPoint

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_GLOBALPOINT_HPP_SEEN

//======================================================================
