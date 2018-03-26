//======================================================================
/*! \file ContentSeparator7100.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 4, 2016
 *///-------------------------------------------------------------------

#ifndef CONTENTSEPARATOR7100_HPP_SEEN
#define CONTENTSEPARATOR7100_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class ContentSeparator7100 : public RegisteredDataBlock<ContentSeparator7100> {
public:
	enum ContentSeparatorType
	{
		ContentSeparatorTypeUndefined   = 0,
		ContentSeparatorTypeStaticStart = 1,
		ContentSeparatorTypeStaticEnd   = 2
	};

public:
	ContentSeparator7100() : m_separatorType(ContentSeparatorTypeUndefined), m_sizeOfContent(0) {}
	ContentSeparator7100(const ContentSeparatorType type, const uint32_t size = 0)
	  : m_separatorType(type), m_sizeOfContent(size)
	{}

	virtual ~ContentSeparator7100() {}

public:
	virtual DataTypeId getDataType() const;
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

public:
	ContentSeparatorType getSeparatorType() const { return m_separatorType; }
	void getSeparatorType(const ContentSeparatorType newSeparatorType) { m_separatorType = newSeparatorType; }

	void setContentSize(const uint32_t size) { m_sizeOfContent = size; }
	uint32_t getContentSize() const { return m_sizeOfContent; }

protected:
	ContentSeparatorType m_separatorType;
	uint32_t m_sizeOfContent;
}; // ContentSeparator

//======================================================================

}// namespace ibeosdk

//======================================================================

#endif // CONTENTSEPARATOR7100_HPP_SEEN

//======================================================================
