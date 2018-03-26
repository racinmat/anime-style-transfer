//======================================================================
/*! \file Polygon2dFloat.hpp
 *
 * \copydoc Copyright
 * \author Ruben Jungnickel (rju)
 * \date Sep 11, 2015
 *///-------------------------------------------------------------------
#ifndef IBEOSDK_POLYGON2DFLOAT_HPP_SEEN
#define IBEOSDK_POLYGON2DFLOAT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Point2dFloat.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class Polygon2dFloat : public std::vector<Point2dFloat>,
                       public Snippet {
public:
	Polygon2dFloat();
	Polygon2dFloat(const Point2dFloat& p1);
	Polygon2dFloat(const Point2dFloat& p1, const Point2dFloat& p2);
	Polygon2dFloat(const Point2dFloat& p1, const Point2dFloat& p2, const Point2dFloat& p3);
	Polygon2dFloat(const Point2dFloat& p1, const Point2dFloat& p2, const Point2dFloat& p3, const Point2dFloat& p4);
	virtual ~Polygon2dFloat();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;
}; // Polygon2dFloat

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_POLYGON2DFLOAT_HPP_SEEN

//======================================================================
