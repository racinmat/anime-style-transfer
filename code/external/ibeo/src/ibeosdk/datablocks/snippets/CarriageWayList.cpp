//======================================================================
/*! \file CarriageWayList.cpp
 *
 * \copydoc Copyright
 * \author Stefan Kaufmann (stk)
 * \date Oct 9, 2014
 *
 * \brief CarriageWayList which is used for storing the map and providing
 *        suitable interface for source and drain
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/CarriageWayList.hpp>

//======================================================================

namespace ibeosdk {
namespace lanes{

//======================================================================

bool CarriageWayList::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	ibeosdk::writeBE(os, static_cast<UINT64>(size() ));      // 8

	CarriageWayList::const_iterator it = begin();
	for (; it != end(); ++it)
		(*it)->serialize(os);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

std::streamsize CarriageWayList::getSerializedSize() const
{
	std::streamsize s_size = 0;

	CarriageWayList::const_iterator it = begin();
	for (; it != end(); ++it)
		s_size += (*it)->getSerializedSize();

	return 8 + s_size;
}

//======================================================================

bool CarriageWayList::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	clear();
	UINT64 nbOfWays;

	ibeosdk::readBE(is, nbOfWays);

	for (UINT64 i = 0; i < nbOfWays; ++i) {
		CarriageWayPtr way = CarriageWay::create();
		way->deserialize(is);
		way->resolveConnections(way);
		push_back(way);
	}

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk
} // namespace lanes

//======================================================================
