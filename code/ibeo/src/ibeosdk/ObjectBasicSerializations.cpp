//======================================================================
/*! \file ObjectBasicSerializations.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 29, 2014
 *///-------------------------------------------------------------------

//======================================================================

#include <ibeosdk/ObjectBasic.hpp>
#include <ibeosdk/inttypes.hpp>
#include <ibeosdk/io.hpp>

#include <iostream>

//======================================================================

namespace ibeosdk {

//======================================================================

template<>
void writeLE<ObjectClass>(std::ostream& os, const ObjectClass& oc)
{
	writeLE(os, uint8_t(oc));
}

//======================================================================

template<>
void writeBE<ObjectClass>(std::ostream& os, const ObjectClass& oc)
{
	writeBE(os, uint8_t(oc));
}

//======================================================================

template<>
void readLE<ObjectClass>(std::istream& is, ObjectClass& oc)
{
	uint8_t rd8;
	readLE(is, rd8);
	oc = ObjectClass(rd8);
}

//======================================================================

template<>
void readBE<ObjectClass>(std::istream& is, ObjectClass& oc)
{
	uint8_t rd8;
	readBE(is, rd8);
	oc = ObjectClass(rd8);
}

//======================================================================
//======================================================================
//======================================================================

template<>
void writeLE<luxObjectClass::LuxObjectClass>(std::ostream& os, const luxObjectClass::LuxObjectClass& oc)
{
	writeLE(os, uint8_t(oc));
}

//======================================================================

template<>
void writeBE<luxObjectClass::LuxObjectClass>(std::ostream& os, const luxObjectClass::LuxObjectClass& oc)
{
	writeBE(os, uint8_t(oc));
}

//======================================================================

template<>
void readLE<luxObjectClass::LuxObjectClass>(std::istream& is, luxObjectClass::LuxObjectClass& oc)
{
	uint8_t rd8;
	readLE(is, rd8);
	oc = luxObjectClass::LuxObjectClass(rd8);
}

//======================================================================

template<>
void readBE<luxObjectClass::LuxObjectClass>(std::istream& is, luxObjectClass::LuxObjectClass& oc)
{
	uint8_t rd8;
	readBE(is, rd8);
	oc = luxObjectClass::LuxObjectClass(rd8);
}



//======================================================================
//======================================================================
//======================================================================

template<>
void writeLE<rawObjectClass::RawObjectClass>(std::ostream& os, const rawObjectClass::RawObjectClass& oc)
{
	writeLE(os, uint8_t(oc));
}

//======================================================================

template<>
void writeBE<rawObjectClass::RawObjectClass>(std::ostream& os, const rawObjectClass::RawObjectClass& oc)
{
	writeBE(os, uint8_t(oc));
}

//======================================================================

template<>
void readLE<rawObjectClass::RawObjectClass>(std::istream& is, rawObjectClass::RawObjectClass& oc)
{
	uint8_t rd8;
	readLE(is, rd8);
	oc = rawObjectClass::RawObjectClass(rd8);
}

//======================================================================

template<>
void readBE<rawObjectClass::RawObjectClass>(std::istream& is, rawObjectClass::RawObjectClass& oc)
{
	uint8_t rd8;
	readBE(is, rd8);
	oc = rawObjectClass::RawObjectClass(rd8);
}



//======================================================================
//======================================================================
//======================================================================




template<>
void writeLE<8>(std::ostream& os, const RefPointBoxLocation rpbl)
{
	ibeosdk::writeLE(os, uint8_t(rpbl));
}

//======================================================================

template<>
void writeBE<8>(std::ostream& os, const RefPointBoxLocation rpbl)
{
	ibeosdk::writeBE(os, uint8_t(rpbl));
}

//======================================================================

template<>
void readLE<8>(std::istream& is, RefPointBoxLocation& rpbl)
{
	uint8_t rd8;
	ibeosdk::readLE(is, rd8);
	rpbl = RefPointBoxLocation(rd8);
}

//======================================================================

template<>
void readBE<8>(std::istream& is, RefPointBoxLocation& rpbl)
{
	uint8_t rd8;
	ibeosdk::readBE(is, rd8);
	rpbl = RefPointBoxLocation(rd8);
}




//======================================================================
//======================================================================
//======================================================================




template<>
void writeLE<16>(std::ostream& os, const RefPointBoxLocation rpbl)
{
	ibeosdk::writeLE(os, uint16_t(rpbl));
}

//======================================================================

template<>
void writeBE<16>(std::ostream& os, const RefPointBoxLocation rpbl)
{
	ibeosdk::writeBE(os, uint16_t(rpbl));
}

//======================================================================

template<>
void readLE<16>(std::istream& is, RefPointBoxLocation& rpbl)
{
	uint16_t rd16;
	ibeosdk::readLE(is, rd16);
	rpbl = RefPointBoxLocation(rd16);
}

//======================================================================

template<>
void readBE<16>(std::istream& is, RefPointBoxLocation& rpbl)
{
	uint16_t rd16;
	ibeosdk::readBE(is, rd16);
	rpbl = RefPointBoxLocation(rd16);
}

//======================================================================

} // namespace ibeosdk

//======================================================================
