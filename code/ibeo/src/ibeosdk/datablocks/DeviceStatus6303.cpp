//======================================================================
/*! \file DeviceStatus6303.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jul 19, 2013
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/DeviceStatus6303.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>
#include <ibeosdk/bufferIO.hpp>

#ifdef DEBUG_DEVICESTATUS
# include <iostream>
# include <ostream>
# include <sstream>
# include <iomanip>

	using namespace std;
#endif // DEBUG_DEVICESTATUS

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<DeviceStatus6303>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<DeviceStatus6303>::dataBlockId = DataTypeId(DataTypeId::DataType_DeviceStatus6303);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<DeviceStatus6303>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<DeviceStatus6303>::dataBlockId, ibeosdk::RegisteredDataBlock<DeviceStatus6303>::create);

class IdcFile;
class IbeoEcu;
class IbeoScala;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<DeviceStatus6303>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoScala>::getInstance().registerDataBlock(registerIdInitial)
		));


//======================================================================
//======================================================================
//======================================================================

template<>
inline
void writeBE(std::ostream& os, const DeviceStatus6303::ContentId& value)
{
	writeBE(os, uint16_t(value));
}

//======================================================================

template<>
inline
void writeBE(std::ostream& os, const DeviceStatus6303::ElementType& value)
{
	writeBE(os, uint8_t(value));
}

//======================================================================

#ifdef DEBUG_DEVICESTATUS

//======================================================================

string getHex(const unsigned char c)
{
	std::stringstream ss;
	ss << "0x" << setfill('0') << setw(2) << hex << int(c);
	return ss.str();
}

//======================================================================

string getHex(const char c)
{
	std::stringstream ss;
	ss << "0x" << setfill('0') << setw(2) << hex << int(c);
	return ss.str();
}

//=====================================================

string getHex(const uint16_t u)
{
	std::stringstream ss;
	ss << "0x" << setfill('0') << setw(2) << hex << u;
	return ss.str();
}

//=====================================================

string getHex(const int16_t i)
{
	std::stringstream ss;
	ss << "0x" << setfill('0') << setw(2) << hex << i;
	return ss.str();
}

//=====================================================

string getHex(const uint32_t u)
{
	std::stringstream ss;
	ss << "0x" << setfill('0') << setw(4) << hex << u;
	return ss.str();
}

//=====================================================

string getHex(const int32_t i)
{
	std::stringstream ss;
	ss << "0x" << setfill('0') << setw(4) << hex << i;
	return ss.str();
}

//=====================================================

string getHex(const float f)
{
//	const unsigned char* p = reinterpret_cast<const unsigned char*>(&f);
	std::stringstream ss;
//	for (int i = 0; i < 4; ++i)
//		ss << getHex(*p++) << "  ";
//
//	ss << " (";
	const uint32_t* u = reinterpret_cast<const uint32_t*>(&f);
	ss << "0x" << setfill('0') << setw(4) << hex << *u;
//	ss << ")";
	return ss.str();
}

//======================================================================

#endif // DEBUG_DEVICESTATUS

//======================================================================

#ifdef DEBUG_DEVICESTATUS

//=====================================================================

std::ostream& operator<<(std::ostream& os, const DeviceStatus6303::ElementType m_elementType)
{
	os << left << setw(3);
	switch (m_elementType) {
	case DeviceStatus6303::ET_Illegal: os << "Ill"; break;
	case DeviceStatus6303::ET_UINT8:   os << "u8"; break;
	case DeviceStatus6303::ET_INT8:    os << "i8"; break;
	case DeviceStatus6303::ET_UINT16:  os << "u16"; break;
	case DeviceStatus6303::ET_INT16:   os << "i16"; break;
	case DeviceStatus6303::ET_UINT32:  os << "u32"; break;
	case DeviceStatus6303::ET_INT32:   os << "i32"; break;
	case DeviceStatus6303::ET_FLOAT32: os << "f16"; break;
	case DeviceStatus6303::ET_STRUCT:  os << "stc"; break;
	default:  break;
	}
	return os;
}

//======================================================================

std::ostream& operator<<(std::ostream& os, const DeviceStatus6303::ContentId m_contentId)
{
	os << left << setw(4);
	switch (m_contentId) {
	case DeviceStatus6303::CID_Illegal: os << "Ill"; break;
	case DeviceStatus6303::CID_ApdVoltageTable:   os << "AVT"; break;
	case DeviceStatus6303::CID_NoiseTable:    os << "NT"; break;

	case DeviceStatus6303::CID_ScalaARMVersion:    os << "SAV"; break;
	case DeviceStatus6303::CID_ErrorScalaFPGA:    os << "ESF"; break;

	case DeviceStatus6303::CID_YawOffsetConf:    os << "YOCo"; break;
	case DeviceStatus6303::CID_YawOffsetCalc:    os << "YOCa"; break;
	case DeviceStatus6303::CID_VelFactorConf:    os << "VFCo"; break;
	case DeviceStatus6303::CID_VelFactorCalc:    os << "VFCa"; break;

	case DeviceStatus6303::CID_Dummy1: os << "d1"; break;
	case DeviceStatus6303::CID_Dummy2: os << "d2"; break;
	case DeviceStatus6303::CID_Dummy3: os << "d3"; break;
	case DeviceStatus6303::CID_Dummy4: os << "d4"; break;
//	case DeviceStatus6303::CID_Dummy5: os << "d5"; break;
//	case DeviceStatus6303::CID_Dummy6: os << "d6"; break;
//	case DeviceStatus6303::CID_Dummy7: os << "d7"; break;
//	case DeviceStatus6303::CID_Dummy8: os << "d8"; break;
//	case DeviceStatus6303::CID_Dummy9: os << "d9"; break;
	default:  break;
	}
	return os;
}

//======================================================================

#endif // DEBUG_DEVICESTATUS

//======================================================================
//======================================================================
//======================================================================

std::streamsize DeviceStatus6303::ContentDescr::getSerializedSize() const
{
	return 2*std::streamsize(sizeof(uint8_t)) + std::streamsize(sizeof(uint16_t)) + m_nbOfBytes;
}

//======================================================================

bool DeviceStatus6303::ContentDescr::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

#	ifdef DEBUG_DEVICESTATUS
	cout.clear();
	cout << "  Write: " << m_contentId << " " << m_elementType;
#	endif // DEBUG_DEVICESTATUS

	writeBE(os, m_contentId);
	writeBE(os, m_elementType);
	writeBE(os, m_nbOfBytes);

	switch (m_elementType) {
	case ET_UINT8:
		writeVcElements(os, m_nbOfBytes, reinterpret_cast<const uint8_t*>(m_contentBuffer));
		break;
	case ET_INT8:
		writeVcElements(os, m_nbOfBytes, reinterpret_cast<const int8_t*>(m_contentBuffer));
		break;
	case ET_UINT16:
		writeVcElements(os, m_nbOfBytes, reinterpret_cast<const uint16_t*>(m_contentBuffer));
		break;
	case ET_INT16:
		writeVcElements(os, m_nbOfBytes, reinterpret_cast<const int16_t*>(m_contentBuffer));
		break;
	case ET_UINT32:
		writeVcElements(os, m_nbOfBytes, reinterpret_cast<const uint32_t*>(m_contentBuffer));
		break;
	case ET_INT32:
		writeVcElements(os, m_nbOfBytes, reinterpret_cast<const int32_t*>(m_contentBuffer));
		break;
	case ET_FLOAT32:
		writeVcElements(os, m_nbOfBytes, reinterpret_cast<const float*>(m_contentBuffer));
		break;
	case ET_STRUCT:
		// structs are already serialized inside the buffer.
		writeVcElements(os, m_nbOfBytes, reinterpret_cast<const char*>(m_contentBuffer));
		break;
	default:
		break; // ignore;
	} // witch element type

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == getSerializedSize());
}


//======================================================================

template<typename T>
void DeviceStatus6303::ContentDescr::writeVcElements(std::ostream& os,
                                                    const uint32_t nbOfBytes,
                                                    const T* const elements)
{
	const uint32_t nbOfElements = uint32_t(nbOfBytes/sizeof(T));
#ifdef DEBUG_DEVICESTATUS
	cout << " #e:" << setw(3) << nbOfElements << " Content:  ";
#endif // DEBUG_DEVICESTATUS

	for (uint32_t i = 0; i < nbOfElements; ++i) {
		writeBE(os, elements[i]);
	}
#ifdef DEBUG_DEVICESTATUS
	cout<< getHex(elements[0]) << "...";
	cout<< getHex(elements[nbOfElements-1]);
	cout  << endl;
#endif // DEBUG_DEVICESTATUS
}


//======================================================================

uint8_t DeviceStatus6303::ContentDescr::getNeededBufferAlignment() const
{
	switch (m_elementType) {
	case ET_UINT16:
	case ET_INT16:
		return 2;
	case ET_UINT32:
	case ET_INT32:
	case ET_FLOAT32:
		return 4;
	case ET_Illegal:
	case ET_UINT8:
	case ET_INT8:
	case ET_STRUCT:
	default:
		return 1;
	}
}




//======================================================================
//======================================================================
//======================================================================

//static
char DeviceStatus6303::ContentDescrDeserializer::deserializeBuffer[maxSizeOfContent+1];

bool DeviceStatus6303::ContentDescrDeserializer::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	m_contentId = this->readContentId(is);
	m_elementType = this->readElementType(is);
	readBE(is, m_nbOfBytes);

	const uint8_t nbOfElements = getNbOfElements(m_elementType, m_nbOfBytes);
#	ifdef DEBUG_DEVICESTATUS
	cout.clear();
	cout << "  Read: " << this->m_contentId << " #e:" << setw(3) << nbOfElements << " Content:  ";
#	endif // DEBUG_DEVICESTATUS

	switch (m_elementType) {
	case ET_UINT8:
		readVcElements(is, nbOfElements, reinterpret_cast<UINT8*>(m_contentBuffer));
		break;
	case ET_INT8:
		readVcElements(is, nbOfElements, reinterpret_cast<INT8*>(m_contentBuffer));
		break;
	case ET_UINT16:
		readVcElements(is, nbOfElements, reinterpret_cast<UINT16*>(m_contentBuffer));
		break;
	case ET_INT16:
		readVcElements(is, nbOfElements, reinterpret_cast<INT16*>(m_contentBuffer));
		break;
	case ET_UINT32:
		readVcElements(is, nbOfElements, reinterpret_cast<UINT32*>(m_contentBuffer));
		break;
	case ET_INT32:
		readVcElements(is, nbOfElements, reinterpret_cast<INT32*>(m_contentBuffer));
		break;
	case ET_FLOAT32:
		readVcElements(is, nbOfElements, reinterpret_cast<float*>(m_contentBuffer));
		break;
	case ET_STRUCT:
		// structs are stored serialized inside the buffer.
		// so no need to change the byte order.
		readVcElements(is, m_nbOfBytes, reinterpret_cast<UINT8*>(m_contentBuffer)); // nbOfElements==1
		break;
	default:
		is.ignore(this->m_nbOfBytes);
		this->m_nbOfBytes = 0;
		break; // ignore;
	} // switch element type

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == getSerializedSize());
}

//======================================================================

template<typename T>
void DeviceStatus6303::ContentDescrDeserializer::readVcElements(std::istream& is,
                                                                const int nbOfElementsToRead,
                                                                T* const elements)
{
	for (int i = 0; i < nbOfElementsToRead; ++i) {
		readBE(is, elements[i]);
	}
#ifdef DEBUG_DEVICESTATUS
	cout<< getHex(elements[0]) << "...";
	cout<< getHex(elements[nbOfElementsToRead-1]);
	cout  << endl;
#endif //  DEBUG_DEVICESTATUS
}


//======================================================================

DeviceStatus6303::ContentId DeviceStatus6303::ContentDescrDeserializer::readContentId(std::istream& is)
{
	uint16_t tmp;
	readBE(is, tmp);
	return ContentId(tmp);
}

//======================================================================

DeviceStatus6303::ElementType DeviceStatus6303::ContentDescrDeserializer::readElementType(std::istream& is)
{
	uint8_t tmp;
	readBE(is, tmp);
	return ElementType(tmp);
}



//======================================================================
//======================================================================
//======================================================================



DeviceStatus6303::
UserDefinedStructBase::UserDefinedStructBase(const ContentId contentId)
  : m_contentId(contentId),
    m_elementType(ET_STRUCT)
{}

//======================================================================

DeviceStatus6303::
UserDefinedStructBase::~UserDefinedStructBase()
{}



//======================================================================
//======================================================================
//======================================================================



DeviceStatus6303::DeviceStatus6303()
  : serialNumberOfScanner(),
    scannerType(0),
    deviceId(0),
    fpgaVersion(),
    hostVersion(),
    fpgaStatusRegister(0),
    fpgaOperationRegister(0),
    scanPeriod(0),
    sensorTemperatureApd0(0.0F),
    sensorTemperatureApd1(0.0F),
    minApdVoltageOffset(0.0F),
    maxApdVoltageOffset(0.0F),
    noiseMeasurementThreshold(0),
    referenceNoise(0),
    m_contentEntries(),
    // m_contentData
    m_usedBytesInContentData(0)
{}

//======================================================================

DeviceStatus6303::~DeviceStatus6303() {}

//======================================================================

bool DeviceStatus6303::operator==(const DeviceStatus6303& rhs) const
{
	if (!(this->serialNumberOfScanner == rhs.serialNumberOfScanner
		&& this->scannerType == rhs.scannerType
		&& this->deviceId == rhs.deviceId
		&& this->fpgaVersion == rhs.fpgaVersion
		&& this->hostVersion == rhs.hostVersion
		&& this->fpgaStatusRegister == rhs.fpgaStatusRegister
		&& this->fpgaOperationRegister == rhs.fpgaOperationRegister
		&& this->scanPeriod == rhs.scanPeriod
		&& this->sensorTemperatureApd0 == rhs.sensorTemperatureApd0
		&& this->sensorTemperatureApd1 == rhs.sensorTemperatureApd1
		&& this->minApdVoltageOffset == rhs.minApdVoltageOffset
		&& this->maxApdVoltageOffset == rhs.maxApdVoltageOffset
		&& this->noiseMeasurementThreshold == rhs.noiseMeasurementThreshold
		&& this->referenceNoise == rhs.referenceNoise
		&& this->m_contentEntries.size() == rhs.m_contentEntries.size())) {
		return false;
	}

	for (unsigned int i = 0; i < this->m_contentEntries.size(); ++i) {
		const ContentDescr& cr = this->m_contentEntries.at(i);
		bool foundCIdMatch = false;
		for (unsigned int j = 0; j < rhs.m_contentEntries.size(); ++j) {
			const ContentDescr& cl = rhs.m_contentEntries.at(j);
			if (cr.getContentId() == cl.getContentId()) {
				foundCIdMatch = true;
				if (cr.getElementType() != cl.getElementType()
						|| cr.getNbOfBytes() != cl.getNbOfBytes())
					return false;

				if (memcmp(cr.getContentBuffer(), cl.getContentBuffer(),cr.getNbOfBytes()))
					return false;
				// perfect match
				break;
			} // if cr::cId = cl::cId
			if (foundCIdMatch)
				continue;
		} // for j
		if (!foundCIdMatch)
			return false;
	} // for i
	return true;
}

//======================================================================

//bool DeviceStatus6303Base::operator!=(const DeviceStatus6303Base& rhs) const
//{
//	return this->serialNumberOfScanner != rhs.serialNumberOfScanner
//		|| this->scannerType != rhs.scannerType
//		|| this->deviceId != rhs.deviceId
//		|| this->fpgaVersion != rhs.fpgaVersion
//		|| this->hostVersion != rhs.hostVersion
//		|| this->fpgaStatusRegister != rhs.fpgaStatusRegister
//		|| this->fpgaOperationRegister != rhs.fpgaOperationRegister
//		|| this->scanPeriod != rhs.scanPeriod
//		|| this->sensorTemperatureApd0 != rhs.sensorTemperatureApd0
//		|| this->sensorTemperatureApd1 != rhs.sensorTemperatureApd1
//		|| this->minApdVoltageOffset != rhs.minApdVoltageOffset
//		|| this->maxApdVoltageOffset != rhs.maxApdVoltageOffset
//		|| this->noiseMeasurementThreshold != rhs.noiseMeasurementThreshold
//		|| this->referenceNoise != rhs.referenceNoise;
//}

//======================================================================
bool DeviceStatus6303::addContent(const ContentDescr& cd)
{
	return addContent(cd.getContentId(),
	                  cd.getElementType(),
	                  cd.getNbOfBytes(),
	                  cd.getNeededBufferAlignment(),
	                  cd.getContentBuffer());
}

//======================================================================

bool DeviceStatus6303::addContent(const ContentId cId,
                                  const ElementType et,
                                  const uint8_t nbOfBytes,
                                  const uint32_t alignment,
                                  const void* cData)
{
	if (m_contentEntries.size() == maxNbOfContentEntries)
		return false;

	if (findContent(cId) >= 0) {
		// no duplicate content types are allowed
		return false;
	}

	if (nbOfBytes <= 0)
		return false;

	char* targetBuffer = m_contentData+m_usedBytesInContentData;
	const uint32_t padding = uint32_t((alignment - (UINT64(targetBuffer) % alignment)) % alignment);

	if (m_usedBytesInContentData+nbOfBytes+padding >= contentBufferSize)
		return false;


	targetBuffer += padding;
	m_usedBytesInContentData += nbOfBytes+padding;

//#ifdef DEBUG_DEVICESTATUS
//	std::cout.clear();
//	cout << "Adding Content: " << cId << "  et: " << et << " #Bytes: " << setw(3) << nbOfBytes
//			<< "  BufPosition: " << setw(4) << INT64(targetBuffer)-INT64(contentData)
//			<< "  Alignment: " << alignment << " ==> Padding: " << padding << endl;
//#endif //  DEBUG_DEVICESTATUS

	ContentDescr contentDescr(cId, et, nbOfBytes);
	contentDescr.setContentBuffer(targetBuffer);
	memcpy(targetBuffer, cData, nbOfBytes);
	m_contentEntries.push_back(contentDescr);
	return true;
}

//======================================================================

bool DeviceStatus6303::addContent(const UserDefinedStructBase& uds)
{
	if (m_contentEntries.size() == maxNbOfContentEntries)
		return false;

	if (findContent(uds.getContentId()) >= 0) {
		// no duplicate content types are allowed
		return false;
	}

	const uint8_t nbOfBytes = uds.getSerializedSize();
	if (nbOfBytes <= 0)
		return false;

	char* targetBuffer = m_contentData+m_usedBytesInContentData;
	if (m_usedBytesInContentData+nbOfBytes >= contentBufferSize)
		return false;

	m_usedBytesInContentData += nbOfBytes;

//#ifdef DEBUG_DEVICESTATUS
//	std::cout.clear();
//	cout << "Adding Content: " << uds.getContentId() << "  et: " << ET_STRUCT << " #Bytes: " << setw(3) << nbOfBytes
//			<< "  BufPosition: " << setw(4)  << INT64(targetBuffer)-INT64(contentData)<< endl;
//#endif //  DEBUG_DEVICESTATUS

	ContentDescr contentDescr(uds.getContentId(), ET_STRUCT, nbOfBytes);
	contentDescr.setContentBuffer(targetBuffer);
	uds.serialize(targetBuffer);
	m_contentEntries.push_back(contentDescr);
	return true;
}

//======================================================================

int DeviceStatus6303::findContent(const ContentId cId)
{
	for (unsigned int i = 0; i < m_contentEntries.size(); ++i) {
		if (m_contentEntries.at(i).getContentId() == cId) {
			return int(i);
		}
	}

	return -1;
}

//======================================================================

bool DeviceStatus6303::getContent(const ContentId cId, UserDefinedStructBase& uds)
{
	const int idx = findContent(cId);
	if (idx == -1) {
		return false;
	}

	const ContentDescr& c = m_contentEntries.at(uint32_t(idx));
	if (c.getElementType() != ET_STRUCT) {
		return false;
	}

	return uds.deserialize(c);
}

//======================================================================

uint8_t DeviceStatus6303::getNbOfElements(const ElementType et, const uint8_t nbOfBytes)
{
	switch (et) {
	case ET_UINT8:   return nbOfBytes;
	case ET_INT8:    return nbOfBytes;
	case ET_UINT16:  return nbOfBytes/uint8_t(sizeof(UINT16));
	case ET_INT16:   return nbOfBytes/uint8_t(sizeof(INT16));
	case ET_UINT32:  return nbOfBytes/uint8_t(sizeof(UINT32));
	case ET_INT32:   return nbOfBytes/uint8_t(sizeof(INT32));
	case ET_FLOAT32: return nbOfBytes/uint8_t(sizeof(float));
	case ET_STRUCT:  return 1;
	default:         return 0;
	}
}

//======================================================================
//======================================================================
//======================================================================

bool DeviceStatus6303::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	this->clear();

	if (!serialNumberOfScanner.deserialize(is)) { unlock(); return false; }

	readBE(is, scannerType);
	readBE(is, deviceId);

	if (!fpgaVersion.deserialize(is)) { unlock(); return false; }
	if (!hostVersion.deserialize(is)) { unlock(); return false; }

	readBE(is, fpgaStatusRegister);
	readBE(is, fpgaOperationRegister);

	readBE(is, scanPeriod);

	readBE(is, sensorTemperatureApd0);
	readBE(is, sensorTemperatureApd1);
	readBE(is, minApdVoltageOffset);
	readBE(is, maxApdVoltageOffset);

	readBE(is, noiseMeasurementThreshold);
	readBE(is, referenceNoise);


	uint16_t nbOfVariableContentEntries = 0;
	readBE(is, nbOfVariableContentEntries);

	bool contentsAreOk = true;
	bool allContentsAdded = true;
	ContentDescrDeserializer c;
	for (int i = 0; i < nbOfVariableContentEntries; ++i) {
		if (!c.deserialize(is))
			contentsAreOk = false;
		if (!this->addContent(c))
			allContentsAdded = false;
	} // for i

	unlock();

	return !is.fail()
	       && contentsAreOk
	       && allContentsAdded
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool DeviceStatus6303::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	if (!serialNumberOfScanner.serialize(os)) { unlock(); return false; }

	writeBE(os, scannerType);
	writeBE(os, deviceId);

	if (!fpgaVersion.serialize(os)) { unlock(); return false; }
	if (!hostVersion.serialize(os)) { unlock(); return false; }

	writeBE(os, fpgaStatusRegister);
	writeBE(os, fpgaOperationRegister);

	writeBE(os, scanPeriod);

	writeBE(os, sensorTemperatureApd0);
	writeBE(os, sensorTemperatureApd1);
	writeBE(os, minApdVoltageOffset);
	writeBE(os, maxApdVoltageOffset);

	writeBE(os, noiseMeasurementThreshold);
	writeBE(os, referenceNoise);

	const uint16_t nbOfContentsEnries = uint16_t(m_contentEntries.size());
	writeBE(os, nbOfContentsEnries);

	std::vector<ContentDescr>::const_iterator contentsEntryIter = m_contentEntries.begin();
	for (; contentsEntryIter != m_contentEntries.end(); ++ contentsEntryIter) {
		if (!contentsEntryIter->serialize(os)) {
			unlock();
			return false;
		} // if
	} // for contentsEntryIter

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

DataTypeId DeviceStatus6303::getDataType() const { return dataBlockId; }

//======================================================================

std::streamsize DeviceStatus6303::getSerializedSize() const
{
	std::streamsize sz = serialNumberOfScanner.getSerializedSize()
		+ 2*std::streamsize(sizeof(UINT8))
		+ fpgaVersion.getSerializedSize() + hostVersion.getSerializedSize()
		+ 2*std::streamsize(sizeof(UINT16))
		+ std::streamsize(sizeof(UINT32))
		+ 5*std::streamsize(sizeof(float))
		+ 2*std::streamsize(sizeof(UINT16));

	for (unsigned int i = 0; i < this->m_contentEntries.size(); ++i) {
		sz += m_contentEntries.at(i).getSerializedSize();
	}
	return sz;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
