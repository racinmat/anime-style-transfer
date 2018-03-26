//======================================================================
/*! \file DatabaseException.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 8, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef DATABASEEXCEPTION_HPP
#define DATABASEEXCEPTION_HPP

//======================================================================
/*!\class DatabaseException
 * \brief Class to unify different kinds of exception, especially exceptions
 *        thrown by the mongo client.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Apr 11, 2016
 *///-------------------------------------------------------------------
class DatabaseException : public std::exception {
public:
	//========================================
	/*!\brief Default constructor.
	 * \param[in] reason  Error message with detailed information.
	 *///-------------------------------------
	DatabaseException(const std::string reason)
	  : std::exception(),
	    m_reason(reason)
	{}

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	~DatabaseException() throw() {}

public:
	//========================================
	/*!\brief Returns error message.
	 * \return Character sequence that may be used to identify the exception.
	 *///-------------------------------------
	virtual const char* what() const throw() { return m_reason.c_str(); }

protected:
	//========================================
	/*!\brief Error message with detailed information.
	 *///-------------------------------------
	std::string m_reason;
}; // DatabaseException

//======================================================================

#endif // DATABASEEXCEPTION_HPP

//======================================================================
