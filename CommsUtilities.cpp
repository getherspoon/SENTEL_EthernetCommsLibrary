#include "stdafx.h"
#include "CommsUtilities.h"

#include <boost/asio/ip/address.hpp>

const std::string		CommsUtilities::Multicast::s_strDEFAULT_LISTEN_ADDRESS					= "0.0.0.0";
const int               CommsUtilities::Multicast::s_iMULTICAST_TTL								=  5;

const std::string       CommsUtilities::Multicast::s_strMULTICAST_GROUP_RCU_RDRCP_LEGACY_RDR	=  "231.230.230.231";
const unsigned short    CommsUtilities::Multicast::s_usMULTICAST_PORT_RCU_RDRCP_LEGACY_RDR		=  50231;

const std::string       CommsUtilities::Multicast::s_strMULTICAST_GROUP_RCU_RCS					=  "231.230.230.232";
const unsigned short    CommsUtilities::Multicast::s_usMULTICAST_PORT_RCU_RCS					=  50232;                

const std::string       CommsUtilities::Multicast::s_strMULTICAST_GROUP_RDRCP_TO_RDR			=  "231.230.230.231";
const unsigned short    CommsUtilities::Multicast::s_usMULTICAST_PORT_RDRCP_TO_RDR				=  50233;

const std::string       CommsUtilities::Multicast::s_strMULTICAST_GROUP_RCU_TO_RDR				=  "231.230.230.231";
const unsigned short    CommsUtilities::Multicast::s_usMULTICAST_PORT_RCU_TO_RDR				 =  50234;

const std::string       CommsUtilities::Multicast::s_strMULTICAST_GROUP_RDR_TO_RDRCP			 =  "231.230.230.231";
const unsigned short    CommsUtilities::Multicast::s_usMULTICAST_PORT_RDR_TO_RDRCP				=  50235;

const std::string       CommsUtilities::Multicast::s_strMULTICAST_GROUP_RDR_TO_RCU				=  "231.230.230.231";
const unsigned short    CommsUtilities::Multicast::s_usMULTICAST_PORT_RDR_TO_RCU				=  50236;    

const int				CommsUtilities::Multicast::s_iINDEFINET_SEND_COUNT						= -1;
const long				CommsUtilities::Multicast::s_lDEFAULT_SEND_INTERVAL_IN_MS				= 5000;
const int				CommsUtilities::Multicast::s_iDEFAULT_MAX_SEND_COUNT					= 1;

const unsigned short    CommsUtilities::UDP::s_usDEFAULT_UDP_PORT								= 23060;
const int				CommsUtilities::UDP::s_iINDEFINET_SEND_COUNT							= -1;
const long				CommsUtilities::UDP::s_lDEFAULT_SEND_INTERVAL_IN_MS						= 5000;
const int				CommsUtilities::UDP::s_iDEFAULT_MAX_SEND_COUNT							= 1;
