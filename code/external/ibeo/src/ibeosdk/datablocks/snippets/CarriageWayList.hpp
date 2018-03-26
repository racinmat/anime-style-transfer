//======================================================================
/*! \file CarriageWayList.hpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 *
 * \brief CarriageWayList which is used for storing the map and providing
 *        suitable interface for source and drain
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_CARRIAGEWAYLIST_HPP
#define IBEOSDK_CARRIAGEWAYLIST_HPP

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/CarriageWay.hpp>

#include <ibeosdk/LaneType.hpp>

//======================================================================

namespace ibeosdk{
namespace lanes{

//======================================================================
/*! \brief A list type for storing multiple elements of type \ref CarriageWay.
 * Inherited from std::vector.
 * \sa CarriageWay
 *///-------------------------------------------------------------------
class CarriageWayList : public std::vector<CarriageWayPtr>,  public ibeosdk::Snippet {
public:
	/*! \brief default constructor */
	CarriageWayList(){}

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;
}; // CarriageWayList

//======================================================================

} // namespace ibeosdk
} // namespace lanes

//======================================================================

#endif // IBEOSDK_CARRIAGEWAYLIST_HPP

//======================================================================
