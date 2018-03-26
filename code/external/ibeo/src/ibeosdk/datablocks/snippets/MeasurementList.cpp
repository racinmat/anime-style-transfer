//======================================================================
/*! \file ObjectEcuEt.cpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Mar 14, 2014
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/MeasurementList.hpp>
#include <ibeosdk/io.hpp>

#include <boost/bind.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

MeasurementList::MeasurementList()
  : m_measurements()
{}

//======================================================================

std::streamsize MeasurementList::getSerializedSize() const
{
	std::streamsize result = std::streamsize(sizeof(UINT16));
	std::vector<Measurement>::const_iterator mIter = m_measurements.begin();
	for (; mIter != m_measurements.end(); ++mIter) {
		result += mIter->getSerializedSize();
	}
	return result;
}

//======================================================================

bool MeasurementList::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	UINT16 size;
	readBE(is, size);
	m_measurements.resize(size);

	std::vector<Measurement>::iterator mIter = m_measurements.begin();
	for (; mIter != m_measurements.end(); ++mIter) {
		if (!mIter->deserialize(is))
			return false;
	}

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool MeasurementList::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, UINT16(m_measurements.size()));
	std::vector<Measurement>::const_iterator mIter = m_measurements.begin();
	for (; mIter != m_measurements.end(); ++mIter) {
		if (!mIter->serialize(os))
			return false;
	}

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}
//======================================================================

MeasurementList::MeasurementVector::const_iterator
MeasurementList::getMeasurement (const MeasurementKey key) const
{
	return std::find_if(m_measurements.begin(), m_measurements.end(),
	                   boost::bind(&MeasurementKey::compare, _1, key));
}

//======================================================================

bool MeasurementList::contains(const MeasurementKey key) const
{
	return getMeasurement(key) != m_measurements.end();
}

//======================================================================

bool operator==(const MeasurementList& lhs, const MeasurementList& rhs)
{
	return lhs.getMeasurements() == rhs.getMeasurements();
}

//======================================================================

bool operator!=(const MeasurementList& lhs, const MeasurementList& rhs)
{
	return lhs.getMeasurements() != rhs.getMeasurements();
}

//======================================================================

} // namespace ibeosdk

//======================================================================
