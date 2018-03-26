//======================================================================
/*! \file Version.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 25, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/misc/Version.hpp>
#include <sstream>

//======================================================================

namespace ibeosdk {

//======================================================================

Version::Version(const MajorVersion majorVersion,
                 const MinorVersion minorVersion,
                 const Revision revision,
                 const PatchLevel patchLevel,
                 const Build build,
                 const std::string& info)
  : m_majorVersion(majorVersion),
    m_minorVersion(minorVersion),
    m_revision(revision),
    m_patchLevel(patchLevel),
    m_build(build),
    m_info(info)
{}

//======================================================================

Version::Version(const MajorVersion majorVersion,
                 const MinorVersion minorVersion,
                 const Revision revision,
                 const Build build,
                 const std::string& info)
  : m_majorVersion(majorVersion),
    m_minorVersion(minorVersion),
    m_revision(revision),
    m_patchLevel(),
    m_build(build),
    m_info(info)
{}


//======================================================================

Version::Version(const Version& other)
  : m_majorVersion(other.m_majorVersion),
    m_minorVersion(other.m_minorVersion),
    m_revision(other.m_revision),
    m_patchLevel(other.m_patchLevel),
    m_build(other.m_build),
    m_info(other.m_info)
{}

//======================================================================

Version::~Version() {}

//======================================================================

int Version::getVersion()
{
	return this->getMajorVersion()  * 1000000
			+ this->getMinorVersion()   * 10000
			+ this->getRevision() * 100
			+ this->getPatchLevel();
}

//======================================================================

std::string Version::toString() const
{
	if (this->m_majorVersion <= -1)
		return "no version";

	std::stringstream ss;
	ss << this->m_majorVersion;

	if (this->m_minorVersion != -1) {
		if (this->m_minorVersion != -2)
			ss << "." << this->m_minorVersion;
		else
			ss << ".x";

		if (this->m_revision != -1) {
			ss << "." << this->m_revision;
			if (this->m_patchLevel != -1) {
				ss << "." << this->m_patchLevel;
			}

			if (this->m_build != -1) {
				ss << " (Build " << this->m_build << ")";
			} // build
		} // revision
	} // minor

	if (!this->m_info.empty()) {
		ss << " (" << this->m_info << ")";
	}

	return ss.str();
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const Version::MinorVersion m)
{
	if (m == -2)
		os << "x";
	else
		os << int(m);
	return os;
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const Version::Revision r)
{
	if (r == -2)
		os << "x";
	else
		os << int(r);
	return os;
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const Version::PatchLevel p)
{
	if (p == -2)
		os << "x";
	else
		os << int(p);
	return os;
}

//======================================================================

} // namespace ibeosdk

//======================================================================

