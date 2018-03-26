//======================================================================
/*! \file DbQueryOptions.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Feb 11, 2016
 *///-------------------------------------------------------------------
//======================================================================

#ifndef DBQUERYOPTIONS_HPP_
#define DBQUERYOPTIONS_HPP_

//======================================================================

#include <ibeosdk/inttypes.hpp>

#include <boost/any.hpp>

#include <vector>
#include <cassert>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

namespace dbField {

	//========================================
	/*!\brief Enumeration for database fields.
	 *///-------------------------------------
	enum DbField {
		DbField_Id,
		DbField_MongoId,
		DbField_Timestamp,
		DbField_ObjectId,
		DbField_Type,
		DbField_Class,
		DbField_BinaryData,
		DbField_File,
		DbField_X,
		DbField_Y,
		DbField_Z,
		DbField_Longitude,
		DbField_Latitude,
		DbField_Altitude,
		DbField_Version
	}; // DbField
} // dbField

//======================================================================
/*!\class DbQueryOptions
 * \brief Class to unify and simplify the creation of database queries.
 * \author Kristian Bischoff (kb)
 * \version 0.1
 * \date Feb 9, 2016
 *///------------------------------------------------------------------
class DbQueryOptions {
public:
	//========================================
	/*!\brief OrderBy as a pair of database field and bool.
	 *
	 * True sets the ordering by the field,
	 * false does not.
	 *///-------------------------------------
	typedef std::pair<dbField::DbField, bool> OrderBy;

	//========================================
	/*!\brief List of ReturnField
	 *///-------------------------------------
	typedef std::vector<OrderBy> OrderByList;

	//========================================
	/*!\brief Returnfield as a pair of database field and bool.
	 *
	 * True includes the field, false does not.
	 *///-------------------------------------
	typedef std::pair<dbField::DbField, bool> ReturnField;

	//========================================
	/*!\brief List of ReturnField
	 *///-------------------------------------
	typedef std::vector<ReturnField> ReturnFieldList;

	//========================================
	/*!\brief Enumeration for matching criterion.
	 *///------------------------------------
	enum Criterion {
		Crit_Eq,
		Crit_Neq,
		Crit_Lt,
		Crit_Lte,
		Crit_Gt,
		Crit_Gte
	};

	//========================================
	/*!\brief Struct to create matching criterion
	 *///-------------------------------------
	struct MatchCriterion {
	public:
		//========================================
		/*!\brief Database field
		 *///-------------------------------------
		dbField::DbField m_field;

		//========================================
		/*!\brief Tells which criterion is be used
		 *///-------------------------------------
		Criterion m_crit;

		//========================================
		/*!\brief Holds the value for the criterion
		 *///-------------------------------------
		boost::any m_val;
	}; // MatchCriterion

	//========================================
	/*!\brief Struct to create return criterion
	 *///-------------------------------------
	struct ReturnCriterion {
	public:
		//========================================
		/*!\brief Database field
		 *///-----------------------------------
		dbField::DbField m_field;

		//========================================
		/*!\brief Tells if the database field should
		 *        be included in the query results.
		 *///-------------------------------------
		bool m_isIncluded;
	}; // ReturnCriterion

	//========================================
	/*!\brief Vector of multiple MatchCriterions
	 *///-------------------------------------
	typedef std::vector<MatchCriterion> MatchCriteria;

public:
	//========================================
	/*!\brief Default constructor
	 *///-------------------------------------
	DbQueryOptions()
	  : m_limit(0),
	    m_skip(0),
	    m_batchSize(0)
	{}

public:
	//========================================
	/*!\brief Sets the result returning limit.
	 *
	 * \param[in] n  Number to return
	 * \return This instance
	 *///-------------------------------------
	DbQueryOptions& limit(const UINT32 n)
	{
		m_limit = n;
		return *this;
	}

	//========================================
	/*!\brief Sets the skip number.
	 *
	 * \param[in] n  Number to skip
	 * \return This instance
	 *///-------------------------------------
	DbQueryOptions& skip(const UINT32 n)
	{
		m_skip = n;
		return *this;
	}

	//========================================
	/*!\brief Sets the result ordering.
	 *
	 * \param[in] field      The database field to order.
	 * \param[in] ascending  Tells if the results are ordered
	 *                      in ascending or descending order.
	 * \return This instance
	 *///-------------------------------------
	DbQueryOptions& orderBy(const dbField::DbField field, const bool ascending = true)
	{
		m_orderBy.push_back(OrderBy(field, ascending));
		return *this;
	}

	//========================================
	/*!\brief Creates a MatchCriterion (equality) and inserts it to DbQueryOptions::m_matchCriteria
	 *
	 * \note The '$eq' query selector was introduced in MongoDB version 3.0.
	 *       For versions <3.0 don't use DbQueryOptions::match(const dbField::DbField field, const T value).
	 *       Build up a standard query instead.
	 *
	 * \param[in] field  The database field to use.
	 * \param[in] value  The value to match
	 * \return This instance
	 *///-------------------------------------
	template <class T>
	DbQueryOptions& match(const dbField::DbField field, const T value)
	{
		MatchCriterion crit;
		crit.m_field = field;
		crit.m_crit = Crit_Eq;
		crit.m_val = boost::any(value);
		m_matchCriteria.push_back(crit);
		return *this;
	}

	//========================================
	/*!\brief Creates a MatchCriterion (disparity)
	 *        and inserts it to DbQueryOptions::m_matchCriteria
	 *
	 * \param[in] field  The database field to use.
	 * \param[in] value  The value not to match
	 * \return This instance
	 *///-------------------------------------
	template <class T>
	DbQueryOptions& nomatch(const dbField::DbField field, const T value)
	{
		MatchCriterion crit;
		crit.m_field = field;
		crit.m_crit = Crit_Neq;
		crit.m_val = boost::any(value);
		m_matchCriteria.push_back(crit);
		return *this;
	}

	//========================================
	/*!\brief Creates a MatchCriterion (greater)
	 *        and inserts it to DbQueryOptions::m_matchCriteria
	 *
	 * \param[in] field  The database field to use.
	 * \param[in] minV   The minimum value.
	 * \return This instance
	 *///-------------------------------------
	template <class T>
	DbQueryOptions& min(const dbField::DbField field, const T minV)
	{
		MatchCriterion crit;
		crit.m_field = field;
		crit.m_crit = Crit_Gt;
		crit.m_val = boost::any(minV);
		m_matchCriteria.push_back(crit);
		return *this;
	}

	//========================================
	/*!\brief Creates a MatchCriterion (greater and equals)
	 *        and inserts it to DbQueryOptions::m_matchCriteria
	 *
	 * \param[in] field  The database field to use.
	 * \param[in] minV   The minimum value.
	 * \return This instance
	 *///-------------------------------------
	template <class T>
	DbQueryOptions& minEquals(const dbField::DbField field, const T minV)
	{
		MatchCriterion crit;
		crit.m_field = field;
		crit.m_crit = Crit_Gte;
		crit.m_val = boost::any(minV);
		m_matchCriteria.push_back(crit);
		return *this;
	}

	//========================================
	/*!\brief Creates a MatchCriterion (less and equals)
	 *        and inserts it to DbQueryOptions::m_matchCriteria
	 *
	 * \param[in] field  The database field to use.
	 * \param[in] maxV   The maximum value.
	 * \return This instance
	 *///-------------------------------------
	template <class T>
	DbQueryOptions& maxEquals(const dbField::DbField field, const T maxV)
	{
		MatchCriterion crit;
		crit.m_field = field;
		crit.m_crit = Crit_Lte;
		crit.m_val = boost::any(maxV);
		m_matchCriteria.push_back(crit);
		return *this;
	}

	//========================================
	/*!\brief Creates a MatchCriterion (less)
	 *        and inserts it to DbQueryOptions::m_matchCriteria
	 *
	 * \param[in] field  The database field to use.
	 * \param[in] maxV  The maximum value.
	 * \return This instance
	 *///-------------------------------------
	template <class T>
	DbQueryOptions& max(const dbField::DbField field, const T maxV)
	{
		MatchCriterion crit;
		crit.m_field = field;
		crit.m_crit = Crit_Lt;
		crit.m_val = boost::any(maxV);
		m_matchCriteria.push_back(crit);
		return *this;
	}

	//========================================
	/*!\brief Creates two MatchCriterions,
	 *        to support a range.
	 *
	 * Uses DbQueryOptions::min(const dbField::DbField field, const T minV)
	 * and DbQueryOptions::max(const dbField::DbField field, const T maxV)
	 *
	 * \param[in] field The database field to use.
	 * \param[in] minV  The minimum value.
	 * \param[in] maxV  The maximum value.
	 * \return This instance
	 *///-------------------------------------
	template <class T>
	DbQueryOptions& range(const dbField::DbField field, const T minV, const T maxV)
	{
		return min(field, minV).max(field, maxV);
	}

	//========================================
	/*!\brief Sets the result batch size.
	 *
	 * \param[in] n The batch size number.
	 * \return This instance
	 *///-------------------------------------
	DbQueryOptions& batchSize(const UINT32 n)
	{
		m_batchSize = n;
		return *this;
	}

	//========================================
	/*!\brief Sets the including result fields.
	 *
	 * \param[in] field The database field to include.
	 * \return This instance
	 *///-------------------------------------
	DbQueryOptions& includeField(const dbField::DbField field)
	{
		m_returnFields.push_back(OrderBy(field, true));
		return *this;
	}

	//========================================
	/*!\brief Sets a excluding result fields.
	 *
	 * \param[in] field The database field to exclude.
	 * \return This instance
	 *///-------------------------------------
	DbQueryOptions& excludeField(const dbField::DbField field)
	{
		m_returnFields.push_back(OrderBy(field, false));
		return *this;
	}

public:
	//========================================
	/*!\brief Returns the query limit
	 * \return Limit as UINT32
	 *///-------------------------------------
	UINT32 getLimit() const { return m_limit; }

	//========================================
	/*!\brief Returns the skip number
	 * \return Skip number as UINT32
	 *///-------------------------------------
	UINT32 getSkip() const { return m_skip; }

	//========================================
	/*!\brief Returns the batch size
	 * \return Batch size as UINT32
	 *///-------------------------------------
	UINT32 getBatchSize() const { return m_batchSize; }

	//========================================
	/*!\brief Returns list of specified ordering
	 * \return OrderByList
	 *///-------------------------------------
	const OrderByList& getOrderBy() const { return m_orderBy; }

	//========================================
	/*!\brief Returns list of specified matchings
	 * \return MatchCriteria
	 *///-------------------------------------
	const MatchCriteria& getMatchCriteria() const { return m_matchCriteria; }

	//========================================
	/*!\brief Returns list of field to return
	 * \return ReturnFieldList
	 *///-------------------------------------
	const ReturnFieldList& getReturnFields() const { return m_returnFields; }

private:
	//========================================
	/*!\brief Limit results.
	 *///-------------------------------------
	UINT32 m_limit;

	//========================================
	/*!\brief Skip results.
	 *///-------------------------------------
	UINT32 m_skip;

	//========================================
	/*!\brief Order-by list.
	 *///-------------------------------------
	OrderByList m_orderBy;

	//========================================
	/*!\brief Batch size.
	 *///------------------------------------
	UINT32 m_batchSize;

	//========================================
	/*!\brief Matching fields.
	 *///------------------------------------
	MatchCriteria m_matchCriteria;

	//========================================
	/*!\brief Returned field list.
	 *///------------------------------------
	ReturnFieldList m_returnFields;
};

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================

#endif // DBQUERYOPTIONS_HPP_SEEN

//======================================================================
