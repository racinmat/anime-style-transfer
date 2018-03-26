//======================================================================
/*! \file LaneSupportPoint.hpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 * \brief Support point for a LaneSegment
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LANESUPPORTPOINT_HPP
#define IBEOSDK_LANESUPPORTPOINT_HPP

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/datablocks/snippets/PositionWgs84.hpp>
#include <ibeosdk/datablocks/snippets/Point2dFloat.hpp>
#include <ibeosdk/datablocks/snippets/Snippet.hpp>

//======================================================================

namespace ibeosdk {
namespace lanes {

//======================================================================
/*! \brief This class represents a support point of a \ref LaneSegment.
 * A point holds information about gps position and heading and width (
 * more precisely the offsets to the left and right bounding line).
 *///-------------------------------------------------------------------
class LaneSupportPoint : public ibeosdk::Snippet {
public:
	/*! \brief Default constructor, calls default constructors of members */
	LaneSupportPoint();

	/*! \brief Constructor with initialization
	 * \param[in] point A gps point with heading
	 * \param[in] lineOffsetLeft The offset to the left bounding line
	 * \param[in] lineOffsetRight The offset to the right bounding line
	 */
	LaneSupportPoint(const PositionWgs84& point,
	                 const Point2dFloat& lineOffsetLeft,
	                 const Point2dFloat& lineOffsetRight);

public:
	/*! \brief Returns the gps point and heading */
	PositionWgs84 getPoint() const;

	/*! \brief Returns the offset to the left marking start point */
	Point2dFloat getLeftOffset() const;

	/*! Returns the offset to the right marking start point */
	Point2dFloat getRightOffset() const;

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	/*! \brief The version of this implementation */
	static std::string const VERSION;

private:
	PositionWgs84 m_point;
	Point2dFloat m_lineOffsetLeft;
	Point2dFloat m_lineOffsetRight;
}; // class LaneSupportPoint

//======================================================================

} // namespace lanes
} // namespace ibeo

//======================================================================

#endif // IBEOSDK_LANESUPPORTPOINT_HPP

//======================================================================
