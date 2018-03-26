//======================================================================
/*! \file Image.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 29, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_IMAGE_HPP_SEEN
#define IBEOSDK_IMAGE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/Time.hpp>
#include <ibeosdk/datablocks/snippets/MountingPosition.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class Image : public RegisteredDataBlock<Image> {
public: // public types
	//========================================
	/*!\brief Enumeration of image formats.
	 *///-------------------------------------
	enum ImageFormat {
		JPEG   = 0,
		MJPEG  = 1,
		GRAY8  = 2,
		YUV420 = 3,
		YUV422 = 4
	};
	//========================================
public:
	//========================================
	/*!\brief Type of the buffer used to store the
	 *        image data.
	 *///-------------------------------------
	typedef char buffer_type;

private: // private types
	class ImageBuffer {
	public:
		//========================================
		/*!\brief Create an 0 size buffer.
		 *///-------------------------------------
		ImageBuffer();

		//========================================
		/*!\brief Create a buffer of size \a size.
		 * \param[in] size  Size of the buffer to
		 *                  be allocated.
		 *///-------------------------------------
		ImageBuffer(const size_t size);

		//========================================
		/*!\brief Create a buffer needed for the given
		 *        image dimensions.
		 * \param[in] width          Number of image pixels per row.
		 * \param[in] height         Number of rows in the image.
		 * \param[in] bytesPerPixel  Number of bytes needed per pixel.
		 *///-------------------------------------
		ImageBuffer(const UINT16 width, const UINT16 height, const UINT8 bytesPerPixel);

		//========================================
		/*!\brief Copy constructor.
		 * \param[in] other  ImageBuffer to be cloned into this
		 *                   object.
		 *///-------------------------------------
		ImageBuffer(const ImageBuffer& other);

		//========================================
		/*!\brief Assigment operator.
		 * \param[in] other  ImageBuffer to be cloned into this
		 *                   object.
		 *///-------------------------------------
		ImageBuffer& operator=(const ImageBuffer& other);

		//========================================
		/*!\brief Destructor.
		 *
		 * Frees the memory.
		 *///-------------------------------------
		~ImageBuffer();
	public:
		//========================================
		/*!\brief Equality predicate
		 * \param[in] rhs  The other mounting position to be
		 *                 compared with this object.
		 * \return \c true if both ImageBuffer objects are
		 *         equal, i.e. lenght and data contents,
		 *         \c false otherwise.
		 *///-------------------------------------
		bool operator==(const ImageBuffer& rhs) const;

	public:
		//========================================
		/*!\brief Allocate a #data buffer of size \a newSize.
		 *
		 * After this call a #data buffer of size \a newSize
		 * will have been allocated. This method is using
		 * std::realloc hence the data will be preserved up to
		 * the lesser of the new and old sizes.
		 *
		 * \param[in] newSize  New size of the buffer.
		 *///-------------------------------------
		void realloc(const size_t newSize);

		//========================================
		/*!\brief Assure the size of the data buffer to be
		 *        at least \a neededSize.
		 *
		 * After the call of this method, the #data buffer
		 * will be at least of size \a neededSize.
		 *
		 * \param[in] neededSize  Size for the #data buffer
		 *                        needed.
		 *///-------------------------------------
		void assureSize(const size_t neededSize);

	public:
		//========================================
		/*!\brief Get the length of the buffer.
		 * \return Length of the buffer in bytes.
		 *///-------------------------------------
		size_t getLength() const { return m_length; }

		//========================================
		/*!\brief Get the data in the buffer.
		 * \return Const pointer to the data.
		 *///-------------------------------------
		const buffer_type* getData() const { return m_data; }

		//========================================
		/*!\brief Get the data in the buffer.
		 * \return Pointer to the data.
		 *///-------------------------------------
		buffer_type* getData() { return m_data; }

	private:
		//========================================
		/*!\brief Length of the data array.
		 *///-------------------------------------
		size_t m_length;

		//========================================
		/*!\brief Pointer to the allocated data array
		 *        of length #length.
		 *///-------------------------------------
		buffer_type* m_data;
	}; // ImageBuffer
	//========================================

public:
	//========================================
	/*!\brief Default constructor.
	 *///-------------------------------------
	Image();

	Image(const Image& other);
	Image& operator=(const Image& other);

public:
	//========================================
	/*!\brief Equality predicate
	 *
	 * Compare this Image with the \a other Image
	 * \return \c true if both images are identically,
	 *         \c false if they differ.
	 *///-------------------------------------
	bool operator==(const Image& other) const;

public:
	//========================================
	/*!\brief Get the size of the serialization.
	 * \return Number of bytes used by the serialization.
	 *///-------------------------------------
	virtual std::streamsize getSerializedSize() const;

	//========================================
	/*!\brief Get the DataType of this Image DataBlock.
	 *///-------------------------------------
	virtual DataTypeId getDataType() const;

	//========================================
	/*!\brief Deserialize data from the given stream \a is into
	 *        this Image.
	 * \param[in, out] is  Stream that provides the serialized
	 *                     data to fill this Image.
	 *                     On exit the \a is get pointer will
	 *                     be behind the read data.
	 * \param[in]      dh  IbeoDataHeader that has been received
	 *                     together with the serialized data in \a is.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Reading the data from the stream has failed.
	 *///-------------------------------------
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);

	//========================================
	/*!\brief Serialize data into the given stream \a os.
	 * \param[out] os  Stream that receive the serialized
	 *                 data from this Image.
	 * \return Whether the operation was successful.
	 * \retval true Everything is alright, no error occured.
	 * \retval false Writing the data into the stream has failed.
	 *///-------------------------------------
	virtual bool serialize(std::ostream& os) const;

public:
	//========================================
	/*!\brief Copies an JPEG image source to this image object
	 *
	 * This function copies an JPEG image from the source buffer
	 * to this object.
	 *
	 * \param[in] imageFormat   Format of the image resides in \a buffer.
	 * \param[in] buffer        Source buffer, where the original image
	 *                          source resides.
	 * \param[in] length        Size of image in bytes.
	 * \param[in] width         Width of the image.
	 * \param[in] height        Height of the image.
	 * \param[in] microseconds  Number of microseconds passed since
	 *                          powering up the device.
	 * \param[in] timestamp     Time, the image was taken.
	 *///-------------------------------------
	void setImage(const ImageFormat imageFormat,
	              const unsigned char* const buffer,
	              const UINT32 length,
	              const UINT16 width,
	              const UINT16 height,
	              const UINT32 microseconds,
	              const ibeosdk::NTPTime timestamp);

public:
	//========================================
	/*!\brief Returns the format of the compressed buffer.
	 *
	 * Returns the format of the contents of image buffer
	 * as specified in enum ImageFormat.
	 *
	 * \return Format of the contents of image buffer.
	 *///-------------------------------------
	ImageFormat getFormat() const { return m_imageFormat; }

	//========================================
	/*!\brief Get the number of elapsed microseconds since the
	 *        start of the device that created this Image DataBlock.
	 * \return The number of elapsed microseconds since the
	 *         start of the device
	 *///-------------------------------------
	UINT32 getMicroseconds() const { return m_microseconds; }

	//========================================
	/*!\brief Get the timestamp of when this Image has been
	 *        generated.
	 * \return The timestamp of when this Image has been
	 *         generated.
	 *///-------------------------------------
	NTPTime getTimestamp() const { return m_timestamp; }

	//========================================
	/*!\brief Get the device id of the camera that generated this image.
	 * \return Device id of camera that generated this image.
	 *///-------------------------------------
	UINT8 getDeviceId() const { return m_deviceId; }

	//========================================
	/*!\brief Get the cameras mounting position.
	 *
	 *  In case one ore more attributes of the returned
	 *  MountingPosition are set to NaN all data are invalid.
	 *
	 * \return Cameras mounting position.
	 *///-------------------------------------
	MountingPosition getCamMountingPosition() const { return m_mountingPosition; }

	//========================================
	/*!\brief Get the horizontal opening angle of the camera.
	 *
	 * Returns the horizontal opening angle of the camera.
	 * Note that the complete position is valid ONLY if this angle is not NaN.
	 *
	 * \return Horizontal opening angle of the camera, in [rad].
	 *///-------------------------------------
	double getCamHorizontalOpeningAngle() const { return m_hOpeningAngle; }

	//========================================
	/*!\brief Get the vertical opening angle of the camera.
	 *
	 * Returns the vertical opening angle of the camera.
	 * Note that the complete position is valid ONLY if this angle is not NaN.
	 *
	 * \return Vertical opening angle of the camera, in [rad].
	 *///-------------------------------------
	double getCamVerticalOpeningAngle() const { return m_vOpeningAngle; }

	//========================================
	/*!\brief Return the width (number of pixels per row) of the image.
	 * \return Number of pixels per row.
	 *///-------------------------------------
	UINT16 getWidth() const { return m_width; }

	//========================================
	/*!\brief Return the height (number of rows) of the image.
	 * \return Number of rows.
	 *///-------------------------------------
	UINT16 getHeight() const { return m_height; }

	//========================================
	/*!\brief Get a const pointer to the buffer to the image.
	 *
	 * Get a const pointer to the image buffer and the length of this buffer.
	 *
	 * \param[out] buffer  On exit const pointer to the image buffer.
	 * \param[out] length  On exit length of the image buffer in bytes.
	 *
	 * \return \c true if the image buffer contains valid data. \c false otherwise.
	 *///-------------------------------------
	bool getImageBuffer(const buffer_type*& buffer, UINT32& length) const;

	//========================================
	/*!\brief Get a pointer to the buffer to the image.
	 *
	 * Get a pointer to the image buffer and the length of this buffer.
	 *
	 * \param[out] buffer  Pointer to the image buffer.
	 * \param[out] length  Length of the image buffer in bytes.
	 *
	 * \return \c true if the image buffer contains valid data. \c false otherwise.
	 *///-------------------------------------
	bool getImageBuffer(buffer_type*& buffer, UINT32& length);

public:
	//========================================
	/*!\brief Set the number of elapsed microseconds since the
	 *        start of the device that created this Image DataBlock.
	 * \param[in] newMicroseconds  New number of elapsed microseconds.
	 *///-------------------------------------
	void setMicroseconds(const UINT32 newMicroseconds) { m_microseconds = newMicroseconds; }

	//========================================
	/*!\brief Set the timestamp when this image has been generated.
	 * \param[in] newTimestamp  New timestamp.
	 *///-------------------------------------
	void setTimestamp(const NTPTime newTimestamp) { m_timestamp = newTimestamp; }

	//========================================
	/*!\brief Set the device id of the camera for this image.
	 * \param[in] newDeviceId  New device id of camera for this image.
	 *///-------------------------------------
	void setDeviceId(const UINT8 newDeviceId) { m_deviceId = newDeviceId; }

	//========================================
	/*!\brief Set the mounting position of the camera for this image.
	 * \param[in] pos  Mounting position of the camera.
	 * \note The mounting position is relative to the vehicle/reference
	 *       coordinate system.
	 *///-------------------------------------
	void setCamMountingPosition(const MountingPosition& pos) { m_mountingPosition = pos; }

	//========================================
	/*!\brief Set the horizontal opening angle of the camera took this image.
	 * \param[in] hOpeningAngle  Horizontal opening (view) angle in [rad].
	 *///-------------------------------------
	void setCamHorizontalOpeningAngle(const float hOpeningAngle) { m_hOpeningAngle = hOpeningAngle; }

	//========================================
	/*!\brief Set the vertical opening angle of the camera took this image.
	 * \param[in] vOpeningAngle  Vertical opening (view) angle in [rad].
	 *///-------------------------------------
	void setCamVerticalOpeningAngle(const float vOpeningAngle) { m_vOpeningAngle = vOpeningAngle; }

private:
	//========================================
	/*!\brief Converts the image format to a
	 *        human readable string.
	 *
	 * \param[in] format  Image format id to be
	 *                    converted into a string.
	 *///-------------------------------------
	static std::string formatToString(const ImageFormat format);

private: // attributes
	//========================================
	/*!\brief Format of the image stored in m_imageBuffer.
	 *///-------------------------------------
	ImageFormat m_imageFormat; // serialized as uint16_t

	//========================================
	/*!\brief Passed microseconds since start.
	 *///-------------------------------------
	UINT32 m_microseconds;

	//========================================
	/*!\brief Timestamp of this Datablock.
	 *///-------------------------------------
	ibeosdk::NTPTime m_timestamp;

	//========================================
	/*!\brief Device id of the origin of this image.
	 *///-------------------------------------
	UINT8 m_deviceId;

	//========================================
	/*!\brief Mounting position of the camera,
	 *        this image has been taken with.
	 *///-------------------------------------
	MountingPosition m_mountingPosition;

	//========================================
	/*!\brief Horizontal opening angle of the camera,
	 *        this image has been taken with.
	 *///-------------------------------------
	double m_hOpeningAngle;

	//========================================
	/*!\brief Vertical opening angle of the camera,
	 *        this image has been taken with.
	 *///-------------------------------------
	double m_vOpeningAngle;

	//========================================
	/*!\brief Number of pixel per row in the image
	 *        resides in m_imagebuffer.
	 *///-------------------------------------
	UINT16 m_width;

	//========================================
	/*!\brief Number of rows in the image
	 *        resides in m_imagebuffer.
	 *///-------------------------------------
	UINT16 m_height;

	//========================================
	/*!\brief Number of bytes used for the image
	 *        data.
	 *        m_imageSize is <= m_imageBuffer.getLength();
	 *///-------------------------------------
	UINT32 m_imageSize;

	//========================================
	/*!\brief Buffer to hold the image data.
	 *///-------------------------------------
	ImageBuffer m_imageBuffer;
}; // Image

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IMAGE_HPP_SEEN

//======================================================================

