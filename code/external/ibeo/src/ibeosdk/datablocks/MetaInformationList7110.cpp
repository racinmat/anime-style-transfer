//======================================================================
/*! \file MetaInformationList7110.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 5, 2016
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/datablocks/MetaInformationList7110.hpp>
#include <ibeosdk/MetaInformationFactory.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>

#include <boost/bind.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<MetaInformationList>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<MetaInformationList7110>::dataBlockId = DataTypeId(DataTypeId::DataType_MetaInformationList7110);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<MetaInformationList7110>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<MetaInformationList7110>::dataBlockId, ibeosdk::RegisteredDataBlock<MetaInformationList7110>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<MetaInformationList7110>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================
//======================================================================
//======================================================================

MetaInformationList7110::MetaInformationList7110()
  : m_metaInformations()
{}

//======================================================================

MetaInformationList7110::~MetaInformationList7110() {}

//==============================================================================

void MetaInformationList7110::addMetaInformation(const MetaInformationSPtr info)
{
	if (!info)
		return;

	MetaInformation::MetaInformationType type = info->getType();
	if (m_metaInformations.find(type) == m_metaInformations.end())
		m_metaInformations[type] = std::vector<MetaInformationSPtr>();

	m_metaInformations[type].push_back(info);
}

//==============================================================================

bool MetaInformationList7110::isSame(const MetaInformationSPtr info1, const MetaInformationSPtr info2)
{
	return info1 == info2;
}

//======================================================================

void MetaInformationList7110::deleteInformation(const MetaInformationSPtr info)
{
	if (!info)
		return;

	if (m_metaInformations.find(info->getType()) == m_metaInformations.end())
		return;

	MetaInformationSPtrVector& vec = m_metaInformations[info->getType()];
	MetaInformationSPtrVector::iterator newAfterEnd
		= std::remove_if(vec.begin(), vec.end(),
	                     boost::bind(&MetaInformationList7110::isSame, _1, boost::cref(info)));

	const uint32_t newNbOfElements = uint32_t(newAfterEnd - vec.begin());
	vec.resize(newNbOfElements);
}

//==============================================================================

void MetaInformationList7110::deleteInformationForType(const MetaInformation::MetaInformationType infoType)
{
	if (m_metaInformations.find(infoType) == m_metaInformations.end())
		return;

	m_metaInformations.erase(infoType);
}

//======================================================================

uint32_t MetaInformationList7110::getNumberOfMetaInformationElements() const
{
	MetaInformationMap::size_type size = 0;
	BOOST_FOREACH(const MetaInformationMap::value_type& v, m_metaInformations)
	{
		size += v.second.size();
	}
	return uint32_t(size);
}

//======================================================================

uint32_t MetaInformationList7110::getNumberOfMetaInformationTypes() const
{
	return uint32_t(m_metaInformations.size());
}

//======================================================================

DataTypeId MetaInformationList7110::getDataType() const { return dataBlockId; }

//======================================================================

std::streamsize MetaInformationList7110::getSerializedSize() const
{
	std::streamsize sz = std::streamsize(sizeof(uint32_t));

	BOOST_FOREACH(const MetaInformationMap::value_type& vt, m_metaInformations) {
		const std::vector<MetaInformationSPtr>& vec = vt.second;
		BOOST_FOREACH(MetaInformationSPtr info, vec) {
			sz += info->getSerializedSize();
		} // for each vector entry
	} // for each map entry

	return sz;
}

//======================================================================

bool MetaInformationList7110::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	MetaInformationFactory& fac = MetaInformationFactory::getInstance();

	uint32_t nbOfEntries;
	readBE(is, nbOfEntries);

	MetaInformationHeader mih;
	for (uint32_t i = 0; i < nbOfEntries && !is.fail(); ++i) {
		if (!mih.deserialize(is))
			break;

//		std::cerr << "Read type " << mih.m_type << std::endl;
		// if the type is not registered or unknown from the current version, the data and the original type
		// are stored in the unsupported type. When serializing again, the original data will be restored.
		MetaInformationSPtr info = fac.create(static_cast<MetaInformation::MetaInformationType>(mih.m_type));
		if (!info) {
			info = MetaInformationUnsupportedSPtr(new MetaInformationUnsupported(mih.m_type, mih.m_timeStamp, mih.m_payloadSize));
		}
		else {
			info->setTimestamp(mih.m_timeStamp);
		}

		if (!info->deserializePayload(is, mih.m_payloadSize)) {
			break;
		}

		this->addMetaInformation(info);
	}

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool MetaInformationList7110::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	writeBE(os, getNumberOfMetaInformationElements());

	BOOST_FOREACH(const MetaInformationMap::value_type& vt, m_metaInformations) {
		const std::vector<MetaInformationSPtr>& vec = vt.second;
		if (os.fail())
			break;
		BOOST_FOREACH(MetaInformationSPtr info, vec) {
			(&(*info))->serialize(os);
			if (os.fail())
				break;
		} // for each vector entry
		if (os.fail())
			break;
	} // for each map entry

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

void MetaInformationList7110::printStatistic(std::ostream& os) const
{
	BOOST_FOREACH(const MetaInformationMap::value_type& vt, m_metaInformations) {
		const std::vector<MetaInformationSPtr>& vec = vt.second;

		os << "Key " << std::left << std::setw(30) << vt.first
		   << "  #:" << vec.size() << std::endl;
	} // for each map entry

}

//======================================================================
//======================================================================
//======================================================================

bool MetaInformationList7110::MetaInformationHeader::deserialize(std::istream& is)
{
	uint16_t infoType;
	readBE(is, infoType);
	m_type = MetaInformation::MetaInformationType(infoType);
	readBE(is, m_timeStamp);
	readBE(is, m_payloadSize);

	return !is.fail();
}

//======================================================================
//======================================================================
//======================================================================

bool operator==(const MetaInformationList7110& lhs, const MetaInformationList7110& rhs)
{
//	std::cerr << "lhs contents:" << std::endl;
//	lhs.printStatistic(std::cerr);
//	std::cerr << "rhs contents:" << std::endl;
//	rhs.printStatistic(std::cerr);
//	std::cerr << std::endl;

	const MetaInformationList7110::MetaInformationMap& lm = lhs.getMetaInformationMap();
	const MetaInformationList7110::MetaInformationMap& rm = rhs.getMetaInformationMap();

	bool equal = lm.size() == rm.size();
	MetaInformationList7110::MetaInformationMap::const_iterator lmIter = lm.begin();

	for (; lmIter != lm.end(); ++lmIter) {

//		std::cerr << "Looking for key " << lmIter->first << std::endl;

		MetaInformationList7110::MetaInformationMap::const_iterator rmfIter = rm.find(lmIter->first);
		if (rmfIter == rm.end()) {
			equal = equal && false;
			break;
			//return false;
		}

		const MetaInformationList7110::MetaInformationSPtrVector& lmipv = lmIter->second;
		const MetaInformationList7110::MetaInformationSPtrVector& rmipv = rmfIter->second;
		equal = equal && (lmipv.size() == rmipv.size());
		BOOST_FOREACH(MetaInformationSPtr li, lmipv) {
			bool found = false;
			BOOST_FOREACH(MetaInformationSPtr ri, rmipv) {
				if (*li == *ri) {
					found = true;
					break;
				}
			}

			equal = equal && found;
		}

		BOOST_FOREACH(MetaInformationSPtr ri, rmipv) {
			bool found = false;
			BOOST_FOREACH(MetaInformationSPtr li, lmipv) {
				if (*li == *ri) {
					found = true;
					break;
				}
			}

			equal = equal && found;
		}

	}

	return equal;
}

//======================================================================

bool operator!=(const MetaInformationList7110& lhs, const MetaInformationList7110& rhs)
{
	return lhs.getMetaInformationMap() != rhs.getMetaInformationMap();
}

//======================================================================


//======================================================================

}// namespace ibeosdk

//======================================================================
