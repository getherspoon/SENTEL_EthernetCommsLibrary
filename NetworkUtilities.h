#pragma once

#include "EthernetCommsAPI.h"

#include <memory>
#include <string>

namespace NetworkUtilities
{
	bool	ETHERNETCOMMS_API IsValidIPAddress( const std::string& p_strIPAddress );	

	bool	ETHERNETCOMMS_API IsValidURL( const std::string& p_strURL );	

	bool	ETHERNETCOMMS_API IsValidMulticastIP( const std::string& p_strIPAddress );	

	bool	ETHERNETCOMMS_API IsNonURLCharacter( const char c );

	bool	ETHERNETCOMMS_API IsValidNetmask( const std::string& p_strNetmask );
};
