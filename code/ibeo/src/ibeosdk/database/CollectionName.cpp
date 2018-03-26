//======================================================================
/*! \file CollectionName.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 11, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/database/CollectionName.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

CollectionName operator+(const CollectionName& cn, const std::string& toBeAdded)
{
	return CollectionName(std::string(cn) + toBeAdded);
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const CollectionName& cn)
{
	os << std::string(cn);
	return os;
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
