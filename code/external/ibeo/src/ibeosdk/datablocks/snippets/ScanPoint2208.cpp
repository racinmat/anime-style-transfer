//======================================================================
/*! \file ScanPoint2208.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Aug 2, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/ScanPoint2208.hpp>
#include <ibeosdk/io.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

ScanPoint2208::ScanPoint2208() {}

//======================================================================

ScanPoint2208::~ScanPoint2208() {}

//======================================================================

bool ScanPoint2208::operator==(const ScanPoint2208& other) const
{
	return this->thresholdId == other.thresholdId
			&& this->echoId == other.echoId
			&& this->reserved == other.reserved
			&& this->layerId == other.layerId
			&& this->flags == other.flags
			&& this->horizontalAngle == other.horizontalAngle
			&& this->radialDistance == other.radialDistance
			&& this->echoPulseWidth == other.echoPulseWidth
			&& this->pfValue == other.pfValue;
}

//======================================================================

bool ScanPoint2208::operator!=(const ScanPoint2208& other) const
{
	return this->thresholdId != other.thresholdId
			|| this->echoId != other.echoId
			|| this->reserved != other.reserved
			|| this->layerId != other.layerId
			|| this->flags != other.flags
			|| this->horizontalAngle != other.horizontalAngle
			|| this->radialDistance != other.radialDistance
			|| this->echoPulseWidth != other.echoPulseWidth
			|| this->pfValue != other.pfValue;

}

//======================================================================

bool ScanPoint2208::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	uint8_t tmp = 0;
	readBE(is, tmp);
	thresholdId = tmp & 0xf;
	echoId = uint8_t((tmp & 0x30) >> 4);
	reserved = (tmp & 0xc0) >> 6;

	readBE(is, layerId);
	readBE(is, flags);
	readBE(is, horizontalAngle);
	readBE(is, radialDistance);
	readBE(is, echoPulseWidth);
	readBE(is, pfValue);

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool ScanPoint2208::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	const uint8_t tmp = uint8_t((thresholdId & 0xf)
	                  | ((echoId & 0x03) << 4)
	                  | ((reserved & 0x03) << 6));
	writeBE(os, tmp);

	writeBE(os, layerId);
	writeBE(os, flags);
	writeBE(os, horizontalAngle);
	writeBE(os, radialDistance);
	writeBE(os, echoPulseWidth);
	writeBE(os, pfValue);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
