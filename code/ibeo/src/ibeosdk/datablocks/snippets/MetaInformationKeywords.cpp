//======================================================================
/*! \file MetaInformationKeywords.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/snippets/MetaInformationKeywords.hpp>

#include <ibeosdk/io.hpp>

#include <boost/foreach.hpp>

#include <algorithm>

//======================================================================

namespace ibeosdk {

//======================================================================

MetaInformationKeywords::MetaInformationKeywords()
  : MetaInformation(MetaInformation::MetaInfoType_Keywords),
    m_keywords()
{}

//==============================================================================

void MetaInformationKeywords::addKeyword(const std::string& keyword)
{
	m_keywords.insert(keyword);
}

//==============================================================================

bool MetaInformationKeywords::containsKeyword(const std::string& keyword)
{
	return m_keywords.find(keyword) != m_keywords.end();
}

//==============================================================================

void MetaInformationKeywords::deleteKeyword(const std::string& keyword)
{
	m_keywords.erase(keyword);
}

//==============================================================================

MetaInformationKeywords::StringVector MetaInformationKeywords::getKeywordsAsVector() const
{
	std::vector<std::string> ret;
	ret.reserve(m_keywords.size());
	BOOST_FOREACH(const std::string& s, m_keywords)
	{
		ret.push_back(s);
	}

	return ret;
}

//======================================================================

uint32_t MetaInformationKeywords::getSerializedPayloadSize() const
{
	uint32_t sz = 0;

	if (!m_keywords.empty()) {
		BOOST_FOREACH(const std::string& keyword, m_keywords) {
			sz += uint32_t(keyword.size());
		}
		sz += uint32_t(m_keywords.size()-1); // for ; between the keywords

		if (sz > maxStringLength) // truncation to be applied
			sz = maxStringLength;
	}

	sz += uint32_t(sizeof(uint16_t));

	return std::streamsize(sz);
}

//======================================================================

bool MetaInformationKeywords::deserializePayload(std::istream& is, const uint32_t payloadSize)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	uint16_t len = 0;
	readBE(is, len);

	if (len > 0) {
		std::string allKeywords;
		allKeywords.resize(len);
		is.read(&allKeywords[0], len);

		std::string::const_iterator startIt = allKeywords.begin();
		std::string::const_iterator endIt = allKeywords.begin() +1; // ok since len > 0

		while (endIt != allKeywords.end()) {
			for (; endIt != allKeywords.end() && *endIt != ';'; ++endIt) {}

			std::string currKeyword(startIt, endIt);
			this->addKeyword(currKeyword);

			// end already reached? Then leave the loop
			if (endIt == allKeywords.end())
				break;

			startIt = endIt + 1; // skip the ";", can be allKeywords.end()
			endIt = startIt;
		}
	}

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == payloadSize && payloadSize == getSerializedPayloadSize());
}

//======================================================================

bool MetaInformationKeywords::serializePayload(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	if (m_keywords.empty()) {
		writeBE(os, uint16_t(0));
	}
	else {
		// gather all keywords together into one string
		// separated by a ";".
		StringSet::const_iterator kwIter = m_keywords.begin();
		// first keyword
		std::string allKeywords(*kwIter);
		++kwIter;

		// all other keywords with a prepending ";"
		for (; kwIter != m_keywords.end(); ++kwIter) {
			allKeywords.append(";");
			allKeywords.append(*kwIter);
		}

		// truncation needed?
		if (allKeywords.size() > maxStringLength) {
			allKeywords.resize(maxStringLength);
		}
		writeBE(os, uint16_t(allKeywords.size()));

		std::ostream_iterator<char> outputIter(os);
		std::copy(allKeywords.begin(), allKeywords.end(), outputIter);
	} // m_keywords is not empty

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool MetaInformationKeywords::isEqual(const MetaInformationKeywords& other) const
{
	return MetaInformation::isEqual(other)
	    && this->getKeywords() == other.getKeywords();
}

//======================================================================

bool MetaInformationKeywords::isNotEqual(const MetaInformationKeywords& other) const
{
	return MetaInformation::isNotEqual(other)
	    || this->getKeywords() != other.getKeywords();
}

//======================================================================
//======================================================================
//======================================================================

bool operator==(const MetaInformationKeywords& lhs, const MetaInformationKeywords& rhs)
{
	return lhs.isEqual(rhs);
}

//======================================================================

bool operator!=(const MetaInformationKeywords& lhs, const MetaInformationKeywords& rhs)
{
	return lhs.isNotEqual(rhs);
}

//======================================================================

}// namespace ibeosdk

//======================================================================
