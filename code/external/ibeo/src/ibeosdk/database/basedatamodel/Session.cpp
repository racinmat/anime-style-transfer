//======================================================================
/*! \file Session.cpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 9, 2016
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/database/basedatamodel/Session.hpp>

//======================================================================

namespace ibeosdk {
namespace dbaccess {

//======================================================================

Session::Session()
  : BaseEntity(),
    m_name(),
    m_tripList(),
    m_procList()
{}

//======================================================================

Session::Session(const std::string& name)
  : BaseEntity(),
    m_name(name),
    m_tripList(),
    m_procList()
{}

//======================================================================

void Session::reset(const std::string& newName)
{
	m_name = newName;
	m_tripList.clear();
	m_procList.clear();
}

//======================================================================

std::string Session::getFullDbName(const BaseEntity::BaseEntityContent& content) const
{
	std::string s;
	if (content == BaseEntity::NotUsed) {
		s = Content_DataModel + ContentSuffix_Sessions;
	}
	return s;
}

//======================================================================

void Session::removeTrip(const std::string& tripName)
{
	TripList::iterator it = m_tripList.begin();
	for (; it != m_tripList.end(); /* no increase here */) {
		if (it->getName().compare(tripName) == 0)
			it = m_tripList.erase(it);
		else
			++it;
	}
}

//======================================================================

void Session::removeProcessing(const std::string& procName)
{
	ProcList::iterator it = m_procList.begin();
	for (; it != m_procList.end();  /* no increase here */) {
		if (it->getName().compare(procName) == 0)
			it = m_procList.erase(it);
		else
			++it;
	}
}

//======================================================================

const std::vector<std::string> Session::getTripNames() const
{
	// TODO: is this correct? names ends up twice the size of m_tripList.
//	std::vector<std::string> names(m_tripList.size());
	std::vector<std::string> names;
	names.reserve(m_tripList.size());

	TripList::const_iterator it = m_tripList.begin();
	for (; it != m_tripList.end(); ++it) {
		names.push_back(it->getName());
	}
	return names;
}

//======================================================================

const std::vector<std::string> Session::getProcessingNames() const
{
	// TODO: is this correct? names ends up twice the size of m_procList.
//	std::vector<std::string> names(m_procList.size());
	std::vector<std::string> names;
	names.reserve(m_procList.size());

	ProcList::const_iterator it = m_procList.begin();
	for (; it != m_procList.end(); ++it) {
		names.push_back(it->getName());
	}
	return names;
}

//======================================================================

bool Session::hasProcessing(const std::string& procName, Processing& proc)
{
	ProcList::const_iterator it = m_procList.begin();
	for (; it != m_procList.end(); ++it) {
		if ((it->getName()).compare(procName) == 0) {
			proc = *it;
			return true;
		}
	}

	return false;
}

//======================================================================

bool Session::hasTrip(const std::string& tripName)
{
	TripList::const_iterator it = m_tripList.begin();
	for (; it != m_tripList.end(); ++it) {
		if ((it->getName()).compare(tripName) == 0)
			return true;
	}

	return false;
}

//======================================================================

} // namespace dbaccess
} // namespace ibeosdk

//======================================================================
