//======================================================================
/*! \file CollectionName.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Apr 11, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef COLLECTIONNAME_HPP_SEEN
#define COLLECTIONNAME_HPP_SEEN

//======================================================================

#include <string>
#include <iostream>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================
/*!\class CollectionName
 * \brief Representation for a database collection name.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 11, 2016
 *///-------------------------------------------------------------------
class CollectionName {
public:
	//========================================
	/*!\brief Constructs a CollectionName.
	 * \param[in] name The actual name as string.
	 *///-------------------------------------
	explicit CollectionName(const std::string& name)
	  : m_name(name)
	{}

	//========================================
	/*!\brief Conversion operator to allow explicitly or implicitly casting to std::string.
	 *///-------------------------------------
	operator std::string() const { return m_name; }

	//========================================
	/*!\brief Appends CollectionName.
	 * \param[in] toBeAdded The string to be addded.
	 *///-------------------------------------
	CollectionName& operator+=(const std::string& toBeAdded) { m_name += toBeAdded; return *this; }

	//========================================
	/*!\brief Assign operator, assigns a string to a CollectioName
	 * \param[in] toAssign The string to be assign.
	 *///-------------------------------------
	CollectionName& operator=(const std::string& toAssign) { m_name = toAssign; return *this; }

protected:
	//========================================
	/*!\brief The collection name.
	 *///-------------------------------------
	std::string m_name;
}; // CollectioName

//======================================================================

//========================================
/*!\brief Creates new CollectionName from existing CollectionName and string.
 * \param[in] cn         CollectionName to expand.
 * \param[in] toBeAdded  The string to be added.
 * \return Created CollectionName from parameters
 *///-------------------------------------
CollectionName operator+(const CollectionName& cn, const std::string& toBeAdded);

//========================================
/*!\brief Inserts CollectionName to ostream
 * \param[in] os  Reference to an ostream.
 * \param[in] cn  CollectionName to write.
 * \return Reference to output stream
 *///-------------------------------------
std::ostream& operator<<(std::ostream& os, const CollectionName& cn);

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // COLLECTIONNAME_HPP_SEEN

//======================================================================
