//======================================================================
/*! \file LogFileManager.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 24, 2013
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LOGFILEMANAGER_HPP_SEEN
#define IBEOSDK_LOGFILEMANAGER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/DataTypeId.hpp>
#include <ibeosdk/Log.hpp>
#include <ibeosdk/SplitFile.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class LogFileManager {
public:
	LogFileManager();
	~LogFileManager();

public:
	bool start();
	void checkSplitRequired(const DataTypeId dt = DataTypeId(DataTypeId::DataType_Unknown));
	void splitRequest();

	int registerSplitFile(SplitFile* splitFile);
	bool unregisterSplitFile(SplitFile* splitFile);

protected:
	bool m_splitRequested;
	std::vector<SplitFile*> m_splitFiles;
}; // LogFileManager

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_LOGFILEMANAGER_HPP_SEEN

//======================================================================
