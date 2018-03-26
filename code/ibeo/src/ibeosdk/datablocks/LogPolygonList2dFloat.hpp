//======================================================================
/*! \file LogPolygonList2dFloat.hpp
 *
 * \copydoc Copyright
 * \author Ruben Jungnickel (rju)
 * \date Sep 11, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LOGPOLYGONLIST2DFLOAT_HPP_SEEN
#define IBEOSDK_LOGPOLYGONLIST2DFLOAT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/LogPolygon2dFloat.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class LogPolygonList2dFloat : public RegisteredDataBlock<LogPolygonList2dFloat> {
public:
	typedef std::vector<LogPolygon2dFloat>::iterator iterator;
	typedef std::vector<LogPolygon2dFloat>::const_iterator const_iterator;

public:
	LogPolygonList2dFloat();
	virtual ~LogPolygonList2dFloat();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	const std::vector<LogPolygon2dFloat>& getLogList() const { return m_logList; }
	void setLogList(const std::vector<LogPolygon2dFloat>& logList) { m_logList = logList; }

private:
	std::vector<LogPolygon2dFloat> m_logList;
}; // LogPolygonList2dFloat

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif /* IBEOSDK_LOGPOLYGONLIST2DFLOAT_HPP_SEEN */

//======================================================================
