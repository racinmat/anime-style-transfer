//======================================================================
/*! \file MeasurementList.hpp
 *
 * \copydoc Copyright
 * \author Kai-Uwe von Deylen (kd)
 * \date Mar 14, 2014
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_MEASUREMENTLIST_HPP_SEEN
#define IBEOSDK_MEASUREMENTLIST_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/datablocks/snippets/Measurement.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <vector>
#include <iostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class MeasurementList : public ibeosdk::Snippet {
public:
	typedef std::vector<Measurement> MeasurementVector;

public:
	MeasurementList();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	void addMeasurement(const Measurement& meas) { m_measurements.push_back( meas ); }

public: // getter
	UINT16 getSize() const { return UINT16(m_measurements.size()); }

	const MeasurementVector& getMeasurements() const { return m_measurements; }
	MeasurementVector& getMeasurements() { return m_measurements; }

	MeasurementVector::const_iterator getMeasurement(const MeasurementKey key) const;
	bool contains(const MeasurementKey key) const;

public: // setter
	void setMeasurements(const MeasurementVector& measurements) { m_measurements = measurements; }

protected:
	MeasurementVector m_measurements;
}; // MeasurementList

//======================================================================

bool operator==(const MeasurementList& lhs, const MeasurementList& rhs);
bool operator!=(const MeasurementList& lhs, const MeasurementList& rhs);

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_MEASUREMENTLIST_HPP_SEEN

//======================================================================

