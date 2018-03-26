//======================================================================
/*! \file DbQueryIterator.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef DBQUERYITERATOR_HPP_SEEN
#define DBQUERYITERATOR_HPP_SEEN

//======================================================================

#include <boost/shared_ptr.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================
/*!\class IteratorBase
 * \brief Base class for all derived iterator implementations
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 9, 2016
 *///------------------------------------------------------------------
class IteratorBase {
public:
	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~IteratorBase() {};
}; // IteratorBase

//======================================================================
/*!\brief Forward declaration for DbQueryIteratorImpl friend class.
 *///-------------------------------------------------------------------
template<class T>
class DbQueryIterator;

//======================================================================
/*!\class DbQueryIteratorImpl
 * \brief Implementation of DbQueryIterator.
 *
 * Pure virtual interface, should be overridden by database-implementation.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 9, 2016
 *///-------------------------------------------------------------------
template <class T>
class DbQueryIteratorImpl {
public:
	friend class DbQueryIterator<T>;

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	virtual ~DbQueryIteratorImpl() {}

private:
	virtual void next() = 0;
	virtual bool end() const = 0;
	virtual T const& get() const = 0;
}; // DbQueryIteratorImpl

//======================================================================
/*!\class DbQueryIterator
 * \brief Unidirectional iterator for database query results, such as those
 *        returned by DbInterfaceT<T>::queryData().
 *
 * If a reverse iterator is required, you can use a reverse order-by in the
 * query options.
 *
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 9, 2016
 *///-------------------------------------------------------------------
template<class T>
class DbQueryIterator : public IteratorBase {
public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	DbQueryIterator()
	  : m_impl()
	{}

	//========================================
	/*!\brief Constructor.
	 *
	 * Created DbQueryIterator takes ownership of Impl object.
	 *///-------------------------------------
	DbQueryIterator(DbQueryIteratorImpl<T>* impl)
	  : m_impl(impl)
	{}

	//========================================
	/*!\brief Copy constructor.
	 * \warning Iterators are linked, so copying an iterator and
	 *          iterating it will iterate the original iterator too.
	 * \param[in] r  Other instance of DbQueryIterator to copy.
	 *///-------------------------------------
	DbQueryIterator(const DbQueryIterator& r) : m_impl(r.m_impl) {}

	//========================================
	/*!\brief Default destructor.
	 *///-------------------------------------
	~DbQueryIterator() {}

	//========================================
	/*!\brief Assignment operator.
	 * \warning Iterators are linked, so assigning an iterator and iterating it
	 *          will iterate the original iterator too.
	 * \param[in] r  Other instance of DbQueryIterator to assign.
	 *///-------------------------------------
	DbQueryIterator& operator=(const DbQueryIterator& r) { m_impl = r.m_impl; return *this; }

	//========================================
	/*!\brief Prefix ++ operator, iterates to next query result.
	 *///-------------------------------------
	DbQueryIterator& operator++() { next(); return *this; }

	//========================================
	/*!\brief Iterates to next query result.
	 *///-------------------------------------
	void next() { m_impl->next(); }

	//========================================
	/*!\brief Returns if the iterator implementation reached the end
	 * \return \c true, if there are no more query results.
	 *         \c false otherwise.
	 *///-------------------------------------
	bool end() const { return m_impl->end(); }

	//========================================
	/*!\brief Dereference operator.
	 *///-------------------------------------
	const T& operator*() const { return get(); }

	//========================================
	/*!\brief Pointer-to-member operator.
	 *///-------------------------------------
	const T* operator->() const { return &(get()); }

	//========================================
	/*!\brief Returns current query result.
	 *
	 * Valid iff end() == false.
	 *///-------------------------------------
	T const& get() const { return m_impl->get(); }

private:
	//========================================
	/*!\brief Postfix ++ operator is explicitly disabled. Use prefix instead.
	 *
	 * Some underlying implementations do not *easily* support copying query
	 * results (though it should be possible). For now all operations requiring
	 * copying are not recommended.
	 *///-------------------------------------
	void operator++(int) {}
	void operator--(int) {}

private:
	//========================================
	/*!\brief Implementation interface.
	 *///-------------------------------------
	boost::shared_ptr<DbQueryIteratorImpl<T> > m_impl;
}; // DbQueryIterator

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // DBQUERYITERATOR_HPP_SEEN

//======================================================================
