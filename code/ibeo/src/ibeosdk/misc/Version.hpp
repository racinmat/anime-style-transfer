//======================================================================
/*! \file Version.hpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date Apr 25, 2012
 *///-------------------------------------------------------------------

#ifndef IBOESDK_VERSION_HPP_SEEN
#define IBOESDK_VERSION_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/misc/Unconvertable.hpp>

#include <string>

//======================================================================

namespace ibeosdk {

//======================================================================
//======================================================================
/*!\class Version
 * \brief Class to hold the a version.
 * \author Mario Brumm
 * \version 0.1
 * \date Apr 25, 2012
 *///-------------------------------------------------------------------
class Version {
public:
	class X {};

	//========================================
	/*!\brief Class that holds the major version.
	 *
	 * It is an ComparableUnconvertable so one cannot
	 * mix it up with other interger by accident.
	 *///-------------------------------------
	class MajorVersion : public ComparableUnconvertable<int> {
	public:
		//========================================
		/*!\brief Constructor.
		 * \param[in] mv  The value of the major version.
		 *///-------------------------------------
		explicit MajorVersion(const int mv = -1) : ComparableUnconvertable<int>(mv) {}
	}; // MajorVersion

	//========================================
	/*!\brief Class that holds the minor version.
	 *
	 * It is an ComparableUnconvertable so one cannot
	 * mix it up with other interger by accident.
	 *///-------------------------------------
	class MinorVersion : public ComparableUnconvertable<int> {
	public:
		//========================================
		/*!\brief Constructor.
		 * \param[in] mv  The value of the minor version.
		 *///-------------------------------------
		explicit MinorVersion(const int mv = -1) : ComparableUnconvertable<int>(mv) {}
		MinorVersion(const X) : ComparableUnconvertable<int>(-2) {}
	}; // MinorVersion

	//========================================
	/*!\brief Class that holds the revision.
	 *
	 * It is an ComparableUnconvertable so one cannot
	 * mix it up with other interger by accident.
	 *///-------------------------------------
	class Revision : public ComparableUnconvertable<int> {
	public:
		//========================================
		/*!\brief Constructor.
		 * \param[in] r  The value of the revision.
		 *///-------------------------------------
		explicit Revision(const int r = -1) : ComparableUnconvertable<int>(r) {}
		Revision(const X) : ComparableUnconvertable<int>(-2) {}
	}; // Revision

	//========================================
	/*!\brief Class that holds the patch level.
	 *
	 * It is an ComparableUnconvertable so one cannot
	 * mix it up with other interger by accident.
	 *///-------------------------------------
	class PatchLevel : public ComparableUnconvertable<int> {
	public:
		//========================================
		/*!\brief Constructor.
		 * \param[in] pl  The value of the patch level.
		 *///-------------------------------------
		explicit PatchLevel(const int pl = -1) : ComparableUnconvertable<int>(pl) {}
		PatchLevel(const X) : ComparableUnconvertable<int>(-2) {}
	}; // PatchLevel

	//========================================
	/*!\brief Class that holds the build number.
	 *
	 * It is an ComparableUnconvertable so one cannot
	 * mix it up with other interger by accident.
	 *///-------------------------------------
	class Build : public ComparableUnconvertable<int> {
	public:
		//========================================
		/*!\brief Constructor.
		 * \param[in] b  The value of the build number.
		 *///-------------------------------------
		explicit Build(const int b = -1) : ComparableUnconvertable<int>(b) {}
	}; // Build

public:
	//========================================
	/*!\brief Constructor for Version.
	 *
	 * \param[in] majorVersion  Major version.
	 * \param[in] minorVersion  Minor version.
	 * \param[in] revision      Revision.
	 * \param[in] patchLevel    Patch level.
	 * \param[in] build         Build number.
	 * \param[in] info          Additional info string.
	 *///-------------------------------------
	Version(const MajorVersion majorVersion = MajorVersion(),
	        const MinorVersion minorVersion = MinorVersion(),
	        const Revision revision = Revision(),
	        const PatchLevel patchLevel = PatchLevel(),
	        const Build build = Build(),
	        const std::string& info = std::string());

	//========================================
	/*!\brief Constructor for Version.
	 *
	 * \param[in] majorVersion  Major version.
	 * \param[in] minorVersion  Minor version.
	 * \param[in] revision      Revision.
	 * \param[in] build         Build number.
	 * \param[in] info          Additional info string.
	 *///-------------------------------------
	Version(const MajorVersion majorVersion,
	        const MinorVersion minorVersion,
	        const Revision revision,
	        const Build build,
	        const std::string& info = std::string());

	//========================================
	/*!\brief Copy constructor
	 * \param[in] other  The other Version object
	 *                   to be copied into this
	 *                   object.
	 *///-------------------------------------
	Version(const Version& other);

	//========================================
	/*!\brief Destructor.
	 *///-------------------------------------
	virtual ~Version();

public:
	//========================================
	/*!\brief Get the version as an integer.
	 *
	 * The value will be calulated as MMmmRRPP,
	 * where MM is the major version, mm the
	 * minor version, RR the revision number and
	 * PP the patch level number.
	 *
	 * A correct output will only be produced if
	 * all of these figures are not larger than
	 * 99.
	 *
	 * \note build and info will not be used.
	 *///-------------------------------------
	int getVersion();

	//========================================
	/*!\brief Return the version as a string.
	 *
	 * The format is MM.mm.RR.PP (Build BB) (info).
	 * Where MM is the major version, mm the
	 * minor version, RR the revision number,
	 * PP the patch level number, BB the build number
	 * and info the info string.
	 *
	 * \return The version as a string.
	 *///-------------------------------------
	std::string toString() const;

	//========================================
	/*!\brief Get the major version.
	 * \return The major version.
	 *///-------------------------------------
	MajorVersion getMajorVersion() const { return m_majorVersion; }

	//========================================
	/*!\brief Get the minor version.
	 * \return The minor version.
	 *///-------------------------------------
	MinorVersion getMinorVersion() const { return m_minorVersion; }

	//========================================
	/*!\brief Get the revision.
	 * \return The revision.
	 *///-------------------------------------
	Revision getRevision() const { return m_revision; }

	//========================================
	/*!\brief Get the patch level.
	 * \return The patch level.
	 *///-------------------------------------
	PatchLevel getPatchLevel() const { return m_patchLevel; }

	//========================================
	/*!\brief Get the build number.
	 * \return The build number.
	 *///-------------------------------------
	Build getBuild() const { return m_build; }

	//========================================
	/*!\brief Get the info string.
	 * \return The info string.
	 *///-------------------------------------
	std::string getInfo() const { return m_info; }

protected:
	//========================================
	/*!\brief The major version.
	 *///-------------------------------------
	MajorVersion m_majorVersion;

	//========================================
	/*!\brief The minor version.
	 *///-------------------------------------
	MinorVersion m_minorVersion;

	//========================================
	/*!\brief The revision.
	 *///-------------------------------------
	Revision m_revision;

	//========================================
	/*!\brief The patch level.
	 *///-------------------------------------
	PatchLevel m_patchLevel;

	//========================================
	/*!\brief The build number.
	 *///-------------------------------------
	Build m_build;

	//========================================
	/*!\brief The info string.
	 *///-------------------------------------
	std::string m_info;
}; // Version

//======================================================================

std::ostream& operator<<(std::ostream& os, const Version::MinorVersion m);
std::ostream& operator<<(std::ostream& os, const Version::Revision r);
std::ostream& operator<<(std::ostream& os, const Version::PatchLevel p);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBOESDK_VERSION_HPP_SEEN

//======================================================================

