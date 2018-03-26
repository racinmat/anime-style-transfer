//======================================================================
/*! \file ReplyEcuSetFilter.cpp
 *
 * \copydoc Copyright
 * \author Julia Nitsch (jn)
 * \date May 05, 2016
 *///-------------------------------------------------------------------

#include <ibeosdk/datablocks/commands/ReplyEcuSetFilter.hpp>

//======================================================================
namespace ibeosdk {
//======================================================================

ReplyEcuSetFilter::ReplyEcuSetFilter() {}

//======================================================================

bool ReplyEcuSetFilter::deserialize( std::istream& is, const IbeoDataHeader& dh )
{
	const int64_t startPos = streamposToInt64(is.tellg());

	readBE(is, m_commandId);

	return !is.fail()
	       && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize())
	       && this->getSerializedSize() == dh.getMessageSize();
}

//======================================================================

bool ReplyEcuSetFilter::serialize( std::ostream& os ) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	writeBE(os, m_commandId);

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================
} // namespace ibeosdk
//======================================================================
