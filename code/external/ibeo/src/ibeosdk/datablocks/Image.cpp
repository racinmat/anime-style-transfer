//======================================================================
/*! \file Image.cpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 29, 2013
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/Image.hpp>
#include <ibeosdk/DataBlockBufferProvider.hpp>
#include <ibeosdk/io.hpp>
#include <ibeosdk/Math.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================
// Specializations for RegisteredDataBlock<Image>
//======================================================================

template<>
const DataTypeId ibeosdk::RegisteredDataBlock<Image>::dataBlockId = DataTypeId(DataTypeId::DataType_Image);

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<Image>::registerIdInitial =
		DataBlockRegisterId(ibeosdk::RegisteredDataBlock<Image>::dataBlockId, ibeosdk::RegisteredDataBlock<Image>::create);

class IdcFile;
class IbeoEcu;

template<>
const DataBlock::DataBlockRegisterId ibeosdk::RegisteredDataBlock<Image>::registerId =
		DataBlockBufferProviderGlobal<IdcFile>::getInstance().registerDataBlock(
		DataBlockBufferProviderGlobal<IbeoEcu>::getInstance().registerDataBlock(registerIdInitial)
		);

//======================================================================
//======================================================================
//======================================================================

Image::ImageBuffer::ImageBuffer()
  : m_length(0),
    m_data(NULL)
{}

//======================================================================

Image::ImageBuffer::ImageBuffer(const size_t size)
  : m_length(size),
    m_data((buffer_type*)malloc(m_length))
{}

//======================================================================

Image::ImageBuffer::ImageBuffer(const UINT16 width,
                                const UINT16 height,
                                const UINT8 bytesPerPixel)
  : m_length(size_t(width*height*bytesPerPixel)),
    m_data((buffer_type*)malloc(m_length))
{}

//======================================================================

Image::ImageBuffer::ImageBuffer(const Image::ImageBuffer& other)
  : m_length(other.m_length),
    m_data((buffer_type*)malloc(m_length))
{
	memcpy(this->m_data, other.m_data, this->m_length);
}

//======================================================================

Image::ImageBuffer& Image::ImageBuffer::operator=(const Image::ImageBuffer& other)
{
	if (this != &other) {
		this->realloc(other.m_length);
		memcpy(this->m_data, other.m_data, this->m_length);
	}
	return *this;
}

//======================================================================

Image::ImageBuffer::~ImageBuffer()
{
	if (m_data) free(m_data);
}

//======================================================================

bool Image::ImageBuffer::operator==(const ImageBuffer& rhs) const
{
	return this->m_length == rhs.m_length
			&& (memcmp(this->m_data, rhs.m_data, this->m_length)==0);
}

//======================================================================

void Image::ImageBuffer::realloc(const size_t newSize)
{
	this->m_length = newSize;
	if (m_data) {
		this->m_data = (buffer_type*)std::realloc(this->m_data, this->m_length);
	}
	else {
		this->m_data = (buffer_type*)std::malloc(this->m_length);
	}
}

//======================================================================

void Image::ImageBuffer::assureSize(const size_t neededSize)
{
	if (neededSize > this->m_length)
		this->realloc(neededSize);
}

//======================================================================



//======================================================================
//======================================================================
//======================================================================



//======================================================================

Image::Image()
  : RegisteredDataBlock<Image>(),
    m_imageFormat(JPEG),
    m_microseconds(0),
    m_timestamp(),
    m_deviceId(0),
    m_mountingPosition(),
    m_hOpeningAngle(ibeosdk::NaN_double),
    m_vOpeningAngle(ibeosdk::NaN_double),
    m_width(640),
    m_height(480),
    m_imageSize(0),
    m_imageBuffer(m_width, m_height,3)
//    m_RGB24Buffer(m_width, m_height,3),
//    m_isCompressedCurrent(false),
//    m_isRGBCurrent(false)
{}

//======================================================================

Image::Image(const Image& other)
  : RegisteredDataBlock<Image>(),
    m_imageFormat(other.m_imageFormat),
    m_microseconds(other.m_microseconds),
    m_timestamp(other.m_timestamp),
    m_deviceId(other.m_deviceId),
    m_mountingPosition(other.m_mountingPosition),
    m_hOpeningAngle(other.m_hOpeningAngle),
    m_vOpeningAngle(other.m_vOpeningAngle),
    m_width(other.m_width),
    m_height(other.m_height),
    m_imageSize(other.m_imageSize),
    m_imageBuffer(other.m_imageBuffer)
{}

//======================================================================

Image& Image::operator=(const Image& other)
{
	if (this == &other)
		return *this;

	m_imageFormat = other.m_imageFormat ;
	m_microseconds = other.m_microseconds ;
	m_timestamp = other.m_timestamp ;
	m_deviceId = other.m_deviceId ;
	m_mountingPosition = other.m_mountingPosition ;
	m_hOpeningAngle = other.m_hOpeningAngle ;
	m_vOpeningAngle = other.m_vOpeningAngle ;
	m_width = other.m_width ;
	m_height = other.m_height ;
	m_imageSize = other.m_imageSize ;
	m_imageBuffer = other.m_imageBuffer;
	return *this;
}

//======================================================================

bool Image::operator==(const Image& other) const
{
	const bool otherDataEqual =
		m_imageFormat == other.m_imageFormat
		&& m_microseconds == other.m_microseconds
		&& m_timestamp == other.m_timestamp
		&& m_deviceId == other.m_deviceId
		&& m_mountingPosition == other.m_mountingPosition
		&& m_hOpeningAngle == other.m_hOpeningAngle
		&& m_vOpeningAngle == other.m_vOpeningAngle
		&& m_width == other.m_width
		&& m_height == other.m_height;

	const bool compressedEqual =
		   m_imageSize == other.m_imageSize
		&& m_imageBuffer == other.m_imageBuffer;

	// Either both images are completely empty, or those parts
	// which are set must be identical

	return otherDataEqual && compressedEqual;
}

//======================================================================

std::streamsize Image::getSerializedSize() const
{
	return 2 + 4 + 8 + 1 + m_mountingPosition.getSerializedSize()
			+ 8 + 8 + 2 + 2 + 4 + m_imageSize;
}

//======================================================================

DataTypeId Image::getDataType() const { return dataBlockId; }

//======================================================================

bool Image::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	lock();

	const UINT16 imgFmt = this->m_imageFormat;
	ibeosdk::writeBE(os, imgFmt);    // 2
	ibeosdk::writeBE(os, this->m_microseconds);   // 4
	ibeosdk::writeBE(os, this->m_timestamp);      // 8
	ibeosdk::writeBE(os, this->m_deviceId);       // 1
	this->m_mountingPosition.serialize(os);  // 6*4 = 24
	ibeosdk::writeBE(os, this->m_hOpeningAngle);  // 8
	ibeosdk::writeBE(os, this->m_vOpeningAngle);  // 8
	ibeosdk::writeBE(os, this->m_width);          // 2
	ibeosdk::writeBE(os, this->m_height);         // 2
	ibeosdk::writeBE(os, this->m_imageSize); // 4
	os.write(reinterpret_cast<const char*>(this->m_imageBuffer.getData()), this->m_imageSize);

	unlock();

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool Image::deserialize(std::istream& is, const IbeoDataHeader& dh)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	lock();

	UINT16 imgFmt = 0;
	ibeosdk::readBE(is, imgFmt);      // 2
	this->m_imageFormat = ImageFormat(imgFmt);

	ibeosdk::readBE(is, this->m_microseconds);     // 4
	ibeosdk::readBE(is, this->m_timestamp);        // 8
	ibeosdk::readBE(is, this->m_deviceId);         // 1
	this->m_mountingPosition.deserialize(is);   // 6*4 = 24
	ibeosdk::readBE(is, this->m_hOpeningAngle);    // 8
	ibeosdk::readBE(is, this->m_vOpeningAngle);    // 8
	ibeosdk::readBE(is, this->m_width);            // 2
	ibeosdk::readBE(is, this->m_height) ;          // 2
	ibeosdk::readBE(is, this->m_imageSize);        // 4

	this->m_imageBuffer.assureSize(this->m_imageSize);
	is.read(reinterpret_cast<char*>(this->m_imageBuffer.getData()), this->m_imageSize );

//	this->m_isCompressedCurrent = true;
//	this->m_isRGBCurrent = false;
//	this->m_RGB24Buffer.realloc(0);

	unlock();

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

void Image::setImage(const ImageFormat imageFormat,
                     const unsigned char* const buffer,
                     const UINT32 length,
                     const UINT16 width,
                     const UINT16 height,
                     const UINT32 microseconds,
                     const ibeosdk::NTPTime timestamp)
{
	m_imageFormat = imageFormat;
	m_microseconds = microseconds;
	m_timestamp = timestamp;
	// deviceId, mountingPosition, h/vOpeningAngle
	m_width = width;
	m_height = height;

	m_imageSize = length;
	m_imageBuffer.assureSize(m_imageSize);

	memcpy(m_imageBuffer.getData(), buffer, length);
}

//======================================================================

bool Image::getImageBuffer(const buffer_type*& buffer, UINT32& length ) const
{
	buffer = m_imageBuffer.getData();
	length = m_imageSize;

	return true;
}

//======================================================================

bool Image::getImageBuffer(buffer_type*& buffer, UINT32& length )
{
	buffer = m_imageBuffer.getData();
	length = m_imageSize;

	return true;
}

//======================================================================

std::string Image::formatToString(const ImageFormat format)
{
	std::string name;

	// Get the key name
	switch (format) {
	case JPEG:   name = "JPEG";                    break;
	case MJPEG:  name = "MotionJPEG (MJPEG)";      break;
	case GRAY8:  name = "8-bit grayscale (GRAY8)"; break;
	case YUV420: name = "YUV 4:2:0 (YUV420)";      break;
	case YUV422: name = "YUV 4:2:2 (YUV422)";      break;
	default:     name = "Unknown " + format;       break;
	} // switch

	return name;
}

//======================================================================

} // namespace ibeosdk

//======================================================================
