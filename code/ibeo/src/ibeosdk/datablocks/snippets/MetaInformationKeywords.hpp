//======================================================================
/*! \file MetaInformationKeywords.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

#ifndef METAINFORMATIONKEYWORDS_HPP_SEEN
#define METAINFORMATIONKEYWORDS_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/MetaInformation.hpp>

#include <boost/unordered_set.hpp>

#include <vector>
#include <string>

//======================================================================

namespace ibeosdk {

//======================================================================

class MetaInformationKeywords : public MetaInformation {
public:
	typedef boost::unordered_set<std::string> StringSet;
	typedef std::vector<std::string> StringVector;

public:
	MetaInformationKeywords();
	virtual ~MetaInformationKeywords() {}

public:
	const StringSet& getKeywords() const { return m_keywords; }
	void setKeywords(const StringSet& keywords) { m_keywords = keywords; }

	void addKeyword(const std::string& keyword);
	void deleteKeyword(const std::string& keyword);
	bool containsKeyword(const std::string& keyword);

	StringVector getKeywordsAsVector() const;

public: // MetaInformation interface
	virtual uint32_t getSerializedPayloadSize() const;
	virtual bool deserializePayload(std::istream& is, const uint32_t payloadSize);
	virtual bool serializePayload(std::ostream& os) const;

public:
	virtual bool isEqual(const MetaInformationKeywords& other) const;
	virtual bool isNotEqual(const MetaInformationKeywords& other) const;

protected:
	StringSet m_keywords;
}; // MetaInformationKeywords


//======================================================================

bool operator==(const MetaInformationKeywords& lhs, const MetaInformationKeywords& rhs);
bool operator!=(const MetaInformationKeywords& lhs, const MetaInformationKeywords& rhs);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // METAINFORMATIONKEYWORDS_HPP_SEEN

//======================================================================
