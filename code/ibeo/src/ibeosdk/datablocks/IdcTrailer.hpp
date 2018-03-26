//======================================================================
/*! \file IdcTrailer.hpp
 *\verbatim
 * ------------------------------------------
 *  (C) 2015 Ibeo Automotive Systems GmbH, Hamburg, Germany
 * ------------------------------------------
 *
 *  Created on: Jul 2, 2015
 *          by: Jan Christian Dittmer
 *\endverbatim
 *///-------------------------------------------------------------------
#ifndef IDCTRAILER_H_SEEN
#define IDCTRAILER_H_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
/*!\brief Container class for the data type IdcTraier 0x6120.
 * \author Jan Christian Dittmer (jcd)
 * \version 0.1
 * \date Jul 02, 2015
 *///-------------------------------------------------------------------
class IdcTrailer : public ibeosdk::RegisteredDataBlock<IdcTrailer> {
public:
	IdcTrailer();
	virtual ~IdcTrailer();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;
}; // IdcTrailer

//======================================================================

}// namespace ibeo

//======================================================================

#endif // IDCTRAILER_H_SEEN

//======================================================================
