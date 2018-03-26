//======================================================================
/*! \file IdcFile.cpp
 *
 * \copydoc Copyright
 * \author Mario Brumm (mb)
 * \date April 4, 2012
 *///-------------------------------------------------------------------

#include <ibeosdk/devices/IdcFile.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/listener/DataListener.hpp>
#include <ibeosdk/datablocks/snippets/FramingPolicy.hpp>
#include <ibeosdk/datablocks/snippets/FrameIndexEntry.hpp>

#include <iostream>

//======================================================================

using namespace std;

//======================================================================

namespace ibeosdk {

//======================================================================

IdcFile::IdcFile()
  : m_listeners(),
    m_streamers(),
    m_openMode(std::ios_base::openmode(0)),
    m_frameIndex(),
    dbbp(DataBlockBufferProviderGlobal<IdcFile>::getInstance()),
    m_strm(),
    m_latestMsgSize(0),
    m_latestHeaderTimestamp(),
    m_firstHeaderTimestamp(),
    m_dh(),
    m_firstDataBlockToBeWritten(false)
{
	logDebug << "Create Device:" << std::endl;
	logDebug << "Registered DataTypes: " << std::endl;

	std::list<DataBlock::DataBlockRegisterId::Key> dtList = getIdsOfSupportedDataBlocks();
	std::list<DataBlock::DataBlockRegisterId::Key>::const_iterator iter = dtList.begin();
	for (; iter != dtList.end(); ++iter) {
		logDebug << std::hex << "0x" << *iter << std::dec << std::endl;
	}
}

//======================================================================

IdcFile::~IdcFile()
{
	this->close();
}

//======================================================================
/**
 * Opens the given file
 */
bool IdcFile::open(const std::string& filename)
{
	this->close();
	m_latestMsgSize = 0;
	m_latestHeaderTimestamp = NTPTime();
	m_firstDataBlockToBeWritten = false;


	m_openMode = std::fstream::in | std::fstream::binary;
	m_strm.open(filename.c_str(), m_openMode);
	return this->isOpen();
}

//======================================================================

bool IdcFile::create(const std::string& filename, const bool append)
{
	this->close();
	m_latestMsgSize = 0;
	m_latestHeaderTimestamp = NTPTime();
	m_firstHeaderTimestamp = NTPTime();
	m_firstDataBlockToBeWritten = true;

	if (append)
		m_openMode = std::fstream::out | std::fstream::binary | std::fstream::app;
	else
		m_openMode = std::fstream::out | std::fstream::binary;

	this->m_strm.open(filename.c_str(), m_openMode);

	ibeosdk::FramingPolicy::TriggerSet triggers;
	triggers.insert(FramingPolicy::Trigger(ibeosdk::DataTypeId::DataType_EcuScan, ibeosdk::FramingPolicy::deviceIdAny));
	triggers.insert(FramingPolicy::Trigger(ibeosdk::DataTypeId::DataType_LuxScan, ibeosdk::FramingPolicy::deviceIdAny));
	triggers.insert(FramingPolicy::Trigger(ibeosdk::DataTypeId::DataType_Scan2208, ibeosdk::FramingPolicy::deviceIdAny));
	triggers.insert(FramingPolicy::Trigger(ibeosdk::DataTypeId::DataType_ScalaFpgaRawData, ibeosdk::FramingPolicy::deviceIdAny));
	triggers.insert(FramingPolicy::Trigger(ibeosdk::DataTypeId::DataType_IdcTrailer, ibeosdk::FramingPolicy::deviceIdAny));

	ibeosdk::FramingPolicy policy;
	policy.setTriggerInNewFrame(true);
	policy.setTriggers(triggers);
	m_frameIndex.setFramingPolicy(policy);
	m_frameIndex.clearFrames();

	return this->isOpen();
}

//======================================================================

bool IdcFile::write(const IbeoDataHeader& dh, const char* const body)
{
	bool headerOk = true;
	if (!m_firstDataBlockToBeWritten) {
		headerOk = writeHeader(dh);
	}
	else {
		IbeoDataHeader dhFirst(dh);
		dhFirst.setPreviousMessageSize(0);
		headerOk = writeHeader(dhFirst);
		m_firstDataBlockToBeWritten = false;
	}

	if (headerOk)
		return write(body, dh.getMessageSize());
	return false;
}

//======================================================================

bool IdcFile::write(const IbeoDataHeader& dh, const DataBlock* const db)
{
	bool headerOk = true;
	if (!m_firstDataBlockToBeWritten) {
		headerOk = writeHeader(dh);
	}
	else {
		IbeoDataHeader dhFirst(dh);
		dhFirst.setPreviousMessageSize(0);
		headerOk = writeHeader(dhFirst);
		m_firstDataBlockToBeWritten = false;
	}

	if (headerOk)
		return db->serialize(this->m_strm);
	return false;
}

//======================================================================

bool IdcFile::writeHeader(const IbeoDataHeader& dh)
{
	if (m_firstHeaderTimestamp == NTPTime())
		m_firstHeaderTimestamp = dh.getTimestamp();

	if (!this->isOpen())
		return false;

	if (m_frameIndex.getFramingPolicy().isTrigger(dh.getDataType(), dh.getDeviceId())) {
		// add the frame before the header is written

		const uint64_t frameOffsetMs = (dh.getTimestamp() - m_firstHeaderTimestamp).getMilliseconds();

		ibeosdk::FrameIndexEntry entry(UINT64(streamposToInt64(m_strm.tellp())), frameOffsetMs, dh.getDeviceId());
		m_frameIndex.addFrame(entry);
	}

	if (dh.serialize(this->m_strm)) {
		m_latestHeaderTimestamp = dh.getTimestamp();
		m_latestMsgSize = dh.getMessageSize();
		return true;
	}
	else
		return false;
}

//======================================================================

bool IdcFile::write(const char* const data, const uint32_t length)
{
	if (!this->isOpen())
		return false;
	this->m_strm.write(data, length);

	return this->m_strm.good();
}

//======================================================================

bool IdcFile::isOpen() { return this-> m_strm.is_open(); }

//======================================================================

bool IdcFile::isEOF() { return this->m_strm.eof(); }

//======================================================================

void IdcFile::close()
{
	if (!this->isOpen())
		return;

	if (m_openMode & std::fstream::out) {
		IbeoDataHeader frameIndexHeader(DataTypeId::DataType_FrameIndex,
		                                m_latestMsgSize,
		                                UINT32(m_frameIndex.getSerializedSize()),
		                                FramingPolicy::deviceIdAny,
		                                m_latestHeaderTimestamp);
		this->writeHeader(frameIndexHeader);
		m_frameIndex.serialize(this->m_strm);

		IbeoDataHeader trailerHeader(DataTypeId::DataType_IdcTrailer,
		                             m_latestMsgSize,
		                             0,
		                             FramingPolicy::deviceIdAny,
		                             m_latestHeaderTimestamp);
		this->writeHeader(trailerHeader);
		// trailer.serialize(this->m_strm); is of size 0
	}

	this->m_strm.close();

	m_frameIndex.clearFrames();
	m_latestMsgSize = 0;
	m_latestHeaderTimestamp = NTPTime();
}

//======================================================================

bool IdcFile::isGood() { return this->m_strm.good(); }

//======================================================================

std::list<DataBlock::DataBlockRegisterId::Key> IdcFile::getIdsOfSupportedDataBlocks() const
{
	std::list<DataBlock::DataBlockRegisterId::Key> keyList;
#ifdef _WIN32
	typedef DataBlockBufferProvider<IdcFile>::ProviderMap PM;
	const PM& pm = dbbp.getMap();
	PM::const_iterator iter = pm.begin();
#else // _WIN32
	typedef typename DataBlockBufferProvider<IdcFile>::ProviderMap PM;
	const PM& pm = dbbp.getMap();
	typename PM::const_iterator iter = pm.begin();
#endif // _WIN32
	for (; iter != dbbp.getMap().end(); ++iter) {
		keyList.push_back(iter->first);
	} // for iter
	keyList.sort();
	return keyList;
}

//======================================================================

const DataBlock* IdcFile::deserialize(std::istream& is,
                                      DataBlock& db,
                                      const IbeoDataHeader& dh)
{
	db.setDataHeader(dh);
	if (db.deserialize(is, dh)) {
		return &db;
	}

	logError << "Failed to deserialization data type 0x"
		<< std::hex << dh.getDataType() << std::dec
		<< std::endl;
	return NULL;

}

//======================================================================

const DataBlock* IdcFile::getNextDataBlock()
{
	if (!isOpen())
		return NULL;

	if (!this->m_dh.deserialize(this->m_strm))
		return NULL;

	DataBlock* db1 = dbbp.getBuffer(m_dh.getDataType());
	if (db1) {
		if (const DataBlock* db = deserialize(m_strm, *db1, m_dh))
			return db;
		else {
			logError << "Called from here" << std::endl;
			return NULL;
		}
	}

	logDebug2 << "Ignoring unknown datatype "
			<< "0x" <<  std::hex << m_dh.getDataType() << std::dec << std::endl;
				m_strm.ignore(m_dh.getMessageSize());
	return NULL;
}

//======================================================================

boost::shared_ptr<char> IdcFile::getNextDataBlockRaw(IbeoDataHeader& dh)
{
	boost::shared_ptr<char> buf;
	if (!isOpen())
		return buf; // NULL

	if (!this->m_dh.deserialize(this->m_strm))
		return buf; // NULL

	dh = m_dh;

	buf.reset(new char[dh.getMessageSize()]);
	m_strm.read(buf.get(), dh.getMessageSize());
	if (m_strm.fail()) {
		logError << "Failed to read data type 0x"
			<< std::hex << dh.getDataType() << std::dec
			<< std::endl;
		buf.reset();
	}

	return buf;
}

//======================================================================

void IdcFile::registerListener(DataListenerBase* const listener)
{
	DataListenerBase::DataTypes::const_iterator it = listener->getAssociatedDataTypes().begin();
	for (; it != listener->getAssociatedDataTypes().end(); ++it) {
		m_listeners[*it].push_back(listener);
	}
}

//======================================================================

statuscodes::Codes IdcFile::unregisterListener(DataListenerBase* const listener)
{

	statuscodes::Codes ret=statuscodes::EverythingOk;

	DataListenerBase::DataTypes::const_iterator it = listener->getAssociatedDataTypes().begin();
	for (; it != listener->getAssociatedDataTypes().end(); ++it) {
		ListenerListMap::iterator llmIter = m_listeners.find(*it);
		if (llmIter == m_listeners.end()) {
			ret= statuscodes::ListenerNotRegistered;
			break;
		}

		ListenerList& list = llmIter->second;

		ListenerList::iterator listenerIter = std::find(list.begin(), list.end(), listener);
		if (listenerIter == list.end()) {
			ret= statuscodes::ListenerNotRegistered;
			break;
		}

		list.erase(listenerIter);
	}

	return ret;
}

//======================================================================

void IdcFile::notifyListeners(const DataBlock* const data)
{
	if (data == NULL)
		return;

	ListenerListMap::iterator list = m_listeners.find(data->getDataType());
	if (list != m_listeners.end()) {
		ListenerList& listeners = list->second;
		ListenerList::iterator listenerIter = listeners.begin();
		for (; listenerIter != listeners.end(); ++listenerIter) {
			data->callListener(**listenerIter);
		} // for listenerIter
	} // if list exists
}

//======================================================================

void IdcFile::registerStreamer(DataStreamer* const streamer)
{
	m_streamers.push_back(streamer);
}

//======================================================================

statuscodes::Codes IdcFile::unregisterStreamer(DataStreamer* const streamer)
{
	std::list<DataStreamer*>::iterator steamerIter = std::find(m_streamers.begin(), m_streamers.end(), streamer);
	if (steamerIter == m_streamers.end())
		return statuscodes::StreamerNotRegistered;

	m_streamers.erase(steamerIter);
	return statuscodes::EverythingOk;
}

//======================================================================

void IdcFile::notifyStreamers(const IbeoDataHeader& dh, const boost::shared_ptr<char> bodyBuf)
{
	if (bodyBuf.get() == NULL)
		return;

	std::list<DataStreamer*>::iterator streamerIter = m_streamers.begin();
	for (; streamerIter != m_streamers.end(); ++streamerIter) {
		(*streamerIter)->onData(dh, bodyBuf.get());
	}
}

//======================================================================

} // namespace ibeosdk

//======================================================================
