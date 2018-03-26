//======================================================================
/*! \file MetaInformationList7110.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

#ifndef METAINFORMATIONLIST7110_HPP_SEEN
#define METAINFORMATIONLIST7110_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/MetaInformationUnsupported.hpp>

#include <ibeosdk/MetaInformationFactory.hpp>

#include <boost/unordered_map.hpp>
#include <boost/foreach.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class MetaInformationList7110 : public RegisteredDataBlock<MetaInformationList7110> {
public:
	typedef std::vector<MetaInformationSPtr> MetaInformationSPtrVector;
	typedef boost::unordered_map<MetaInformation::MetaInformationType, MetaInformationSPtrVector> MetaInformationMap;

private:
	class MetaInformationHeader;

public:
	MetaInformationList7110();
	virtual ~MetaInformationList7110();

public:
	const MetaInformationMap& getMetaInformationMap() const { return m_metaInformations; }

	uint32_t getNumberOfMetaInformationElements() const;
	uint32_t getNumberOfMetaInformationTypes() const;

	void deleteInformation(const MetaInformationSPtr info);
	void deleteInformationForType(const MetaInformation::MetaInformationType infoType);

public:
	void addMetaInformation(const MetaInformationSPtr info);

	template<class T>
	std::vector<T> getMetaInformations(const MetaInformation::MetaInformationType infoType)
	{
		std::vector<T> ret;

		if (m_metaInformations.find(infoType) == m_metaInformations.end())
			return ret;

		BOOST_FOREACH(MetaInformationSPtr info, m_metaInformations[infoType])
		{
			ret.push_back(*(boost::static_pointer_cast<T>(info)));
		}
		return ret;
	}

	void printStatistic(std::ostream& os) const;

public:
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

protected:
	static bool isSame(const MetaInformationSPtr info1, const MetaInformationSPtr info2);

protected:
	MetaInformationMap m_metaInformations;
}; // MetaInformationList

//======================================================================

class MetaInformationList7110::MetaInformationHeader {
public:
	bool deserialize(std::istream& is);

public:
	MetaInformation::MetaInformationType m_type;
	NTPTime m_timeStamp;
	uint32_t m_payloadSize;
}; // MetaInformationList::MetaInformationHeader

//======================================================================

bool operator==(const MetaInformationList7110& lhs, const MetaInformationList7110& rhs);
bool operator!=(const MetaInformationList7110& lhs, const MetaInformationList7110& rhs);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // METAINFORMATIONLIST7110_HPP_SEEN

//======================================================================
