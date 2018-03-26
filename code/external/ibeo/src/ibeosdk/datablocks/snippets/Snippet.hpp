//======================================================================
/*! \file Snippet.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date May 09, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SERIALIZABLE_HPP_SEEN
#define IBEOSDK_SERIALIZABLE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <istream>
#include <ostream>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\brief Class that provides a common interface for all snippets.
 * \author Mario Brumm (mb)
 * \version 0.1
 * \date Mai 9, 2012
 *///-------------------------------------------------------------------
class Snippet {
public:
	//========================================
	/*!\brief Destructor, doing nothing special.
	 *///-------------------------------------
	virtual ~Snippet() {}

public:
	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization
	 *         of this data class.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const = 0;

	//========================================
	/*!\brief Deserialize data from the given stream \a is into
	 *        this class.
	 * \param[in, out] is  Stream that provides the serialized
	 *                     data to fill this class.
	 *                     On exit the \a is get pointer will
	 *                     be behind the read data.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Reading the data from the stream has failed.
	 *///-------------------------------------
	virtual bool deserialize(std::istream& is) = 0;

	//========================================
	/*!\brief Serialize data into the given stream \a os.
	 * \param[out] os  Stream that receive the serialized
	 *                 data from this class.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const = 0;
}; // Snippet

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SERIALIZABLE_HPP_SEEN

//======================================================================

