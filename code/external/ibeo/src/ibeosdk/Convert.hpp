//======================================================================
/*! \file Convert.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jun 1, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_CONVERT_HPP_SEEN
#define IBEOSDK_CONVERT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <string>
#include <vector>
#include <sstream>
#include <ios>

#include <ibeosdk/inttypes.hpp>
//#include <arpa/inet.h>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\brief Convert the given value v into a string.
 * \tparam T     Type of the object to be converted into a string.
 *               There must exist an operator<< for type T.
 * \param[in] v  Value to be converted into a string.
 * \return The string representation of \a v.
 *///-------------------------------------------------------------------
template<typename T>
inline std::string toString(const T& v)
{
	std::stringstream ss;
	ss << v;
	return ss.str();
}


//======================================================================
/*!\brief Specialization of toString for a char.
 * \param[in] v  Value to be converted into a string.
 *               The value shall be shown as decimal number
 *               hence it will be converted into an interger
 *               before written into the string.
 * \return The string representation of \a v.
 *///-------------------------------------------------------------------
template<>
inline std::string toString<char>(const char& v)
{
	std::stringstream ss;
	ss << int(v);
	return ss.str();
}


//======================================================================
/*!\brief Specialization of toString for a unsigned char.
 * \param[in] v  Value to be converted into a string.
 *               The value shall be shown as decimal number
 *               hence it will be converted into an interger
 *               before written into the string.
 * \return The string representation of \a v.
 *///-------------------------------------------------------------------
template<>
inline std::string toString<unsigned char>(const unsigned char& v)
{
	std::stringstream ss;
	ss << int(v);
	return ss.str();
}

//======================================================================
/*!\brief Convert the given value vector into a string.
 *
 * This method read the text and convert the in text form given
 * values into a vector of the given type \a T.
 *
 * \tparam T      Type of the vector entries to be read from the string \a s.
 *                There must exist an operator>> for type T.
 * \param[in]  s  String to be read into a vector.
 * \param[out] v  On exit this vector will hold the read values.
 *///-------------------------------------------------------------------
template<typename T>
inline void fromString2Vector(const std::string& s, std::vector<T>& v)
{
	v.clear();
	if (s.empty())
		return;

	std::stringstream ss(s);
	T e;

	while (!ss.eof()) {
		ss >> e;
		v.push_back(e);
	}
}
//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_CONVERT_HPP_SEEN

//======================================================================
