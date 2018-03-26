//======================================================================
/*! \file IpHelper.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Jan 20, 2014
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_IPHELPER_HPP_SEEN
#define IBEOSDK_IPHELPER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/inttypes.hpp>

#include <string>

//======================================================================

namespace ibeosdk {

//======================================================================

inline
uint16_t getPort(std::string& ip, const uint16_t defaultPort = 0)
{
	const size_t portPos = ip.find(':');
	uint16_t port = defaultPort;
	if (portPos != std::string::npos) {
		std::string portStr = ip.substr(portPos+1);
		std::stringstream ss;
		ss.str(portStr);
		ss >> port;
		ip = ip.substr(0, portPos);
	}

	return port;
}

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_IPHELPER_HPP_SEEN

//======================================================================

