//======================================================================
/*! \file SubScan2208.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Aug 2, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/snippets/SubScan2208.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

SubScan2208::SubScan2208() {}

//======================================================================

SubScan2208::~SubScan2208() {}

//======================================================================

bool SubScan2208::operator==(const SubScan2208& other) const
{
	bool eq = this->startScanTimestamp == other.startScanTimestamp
			&& this->endScanTimestamp == other.endScanTimestamp
			&& this->startScanAngle == other.startScanAngle
			&& this->endScanAngle == other.endScanAngle
			&& this->flags == other.flags
			&& this->mirrorSide == other.mirrorSide
			&& this->mirrorTiltDeprecated == other.mirrorTiltDeprecated
			&& this->mirrorTilt == other.mirrorTilt;
	if (!eq)
		return false;

	for (int i = 0; i < SubScan2208::nbOfReserved; ++i) {
		eq &= this->reserved[i] == other.reserved[i];
	}

	eq &= this->getNbOfPoints() == other.getNbOfPoints();

	if (!eq)
		return false;


	PointVector::const_iterator pIter = scanPoints.begin();
	PointVector::const_iterator opIter = scanPoints.begin();
	for (; pIter != scanPoints.end(); ++pIter, ++opIter) {
		if (*pIter != *opIter)
			return false;
	}
	return true;
}

//======================================================================

bool SubScan2208::operator!=(const SubScan2208& other) const
{
	bool neq = this->startScanTimestamp != other.startScanTimestamp
			|| this->endScanTimestamp != other.endScanTimestamp
			|| this->startScanAngle != other.startScanAngle
			|| this->endScanAngle != other.endScanAngle
			|| this->flags != other.flags
			|| this->mirrorSide != other.mirrorSide
			|| this->mirrorTiltDeprecated != other.mirrorTiltDeprecated
			|| this->mirrorTilt != other.mirrorTilt;
	if (neq)
		return true;

	for (int i = 0; i < SubScan2208::nbOfReserved; ++i) {
		neq |= this->reserved[i] != other.reserved[i];
	}

	neq |= this->getNbOfPoints() != other.getNbOfPoints();

	if (neq)
		return true;


	PointVector::const_iterator pIter = scanPoints.begin();
	PointVector::const_iterator opIter = scanPoints.begin();
	for (; pIter != scanPoints.end(); ++pIter, ++opIter) {
		if (*pIter != *opIter)
			return true;
	}
	return false;
}

//======================================================================

bool SubScan2208::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readBE(is, startScanTimestamp);
	readBE(is, endScanTimestamp);
	readBE(is, startScanAngle);
	readBE(is, endScanAngle);
	readBE(is, flags);
	readBE(is, mirrorSide);
	readBE(is, mirrorTiltDeprecated);
	readBE(is, mirrorTilt);
	for (int i = 0; i < nbOfReserved; ++i) {
		readBE(is, reserved[i]);
	}

	uint16_t nbOfPoints;
	readBE(is, nbOfPoints);
	scanPoints.resize(nbOfPoints);

	PointVector::iterator ptIter = scanPoints.begin();
	for (;ptIter != scanPoints.end(); ++ptIter) {
		if (!ptIter->deserialize(is))
			return false;
	}

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool SubScan2208::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, startScanTimestamp.getTime());
	writeBE(os, endScanTimestamp.getTime());
	writeBE(os, startScanAngle);
	writeBE(os, endScanAngle);
	writeBE(os, flags);
	writeBE(os, mirrorSide);
	writeBE(os, mirrorTiltDeprecated);
	writeBE(os, mirrorTilt);
	for (int i = 0; i < nbOfReserved; ++i) {
		writeBE(os, reserved[i]);
	}

	const uint16_t nbOfPoints = uint16_t(scanPoints.size());
	writeBE(os, nbOfPoints);

	PointVector::const_iterator ptIter = scanPoints.begin();
	for (;ptIter != scanPoints.end(); ++ptIter) {
		if (!ptIter->serialize(os))
			return false;
	}

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

} // namespace ibeosdk

//======================================================================
