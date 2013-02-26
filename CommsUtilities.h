/* 
 * File:   CommsUtilities.h
 * Author: jarmentrout
 *
 * Created on September 27, 2011, 10:02 AM
 */

#ifndef COMMSUTILITIES_H
#define	COMMSUTILITIES_H

#ifndef NO_MFC

#ifndef BOOST_THREAD_DYN_LINK
#define BOOST_THREAD_DYN_LINK
#endif

#else

#ifndef BOOST_THREAD_USE_LIB
#define BOOST_THREAD_USE_LIB
#endif

#endif

#include "EthernetCommsAPI.h"

#include <map>
#include <string>

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

class CAsyncMulticastComms;

typedef boost::function<void(const std::string&, const std::string&,const unsigned int, const boost::system::error_code&)>	Callback_ConstRefStringConstRefStringUnsignedInt_Error;
typedef boost::function<void(const std::string&, const boost::system::error_code&)>						Callback_ConstRefString_Error;
typedef boost::function<void(const std::string&, boost::system::error_code&)>							Callback_ConstRefString_RefError;
typedef boost::function<void(const boost::system::error_code&, const std::size_t&)>						Callback_ConstError_RefSizeT;
typedef boost::function<void(boost::system::error_code&, const std::size_t&)>							Callback_Error_RefSizeT;


namespace CommsUtilities
{
	enum eCommsSystemType
	{
		ASYNC,
		SYNC,
		COMMS_SYSTEM_TYPE_COUNT
	};

	enum eConnectionlessCommsType
	{
		MULTICAST,
		UDP,
		CONNECTIONLESS_COMMS_TYPE_COUNT
	};

	class ETHERNETCOMMS_API Multicast
	{
	public:		
		// Enum of Sender/Receiver Types
		enum eSendRecvType
		{
			LEGACY_RDR		= 0,
			CURRENT_RDR,
			SEND_RECV_TYPE_COUNT
		};

		// TypeDefs for our Sender/Receiver Needs
		typedef std::map<eSendRecvType, boost::shared_ptr<CAsyncMulticastComms> >			CommsMapType;
		
		static const std::string        s_strDEFAULT_LISTEN_ADDRESS;
                static const int                s_iMULTICAST_TTL;

                static const std::string        s_strMULTICAST_GROUP_RCU_RDRCP_LEGACY_RDR;
                static const unsigned short     s_usMULTICAST_PORT_RCU_RDRCP_LEGACY_RDR;        

                static const std::string        s_strMULTICAST_GROUP_RCU_RCS;
                static const unsigned short     s_usMULTICAST_PORT_RCU_RCS;

                static const std::string        s_strMULTICAST_GROUP_RDRCP_TO_RDR;
                static const unsigned short     s_usMULTICAST_PORT_RDRCP_TO_RDR;

                static const std::string        s_strMULTICAST_GROUP_RCU_TO_RDR;
                static const unsigned short     s_usMULTICAST_PORT_RCU_TO_RDR;

                static const std::string        s_strMULTICAST_GROUP_RDR_TO_RDRCP;
                static const unsigned short     s_usMULTICAST_PORT_RDR_TO_RDRCP;

                static const std::string        s_strMULTICAST_GROUP_RDR_TO_RCU;
                static const unsigned short     s_usMULTICAST_PORT_RDR_TO_RCU;

		static const int		s_iINDEFINET_SEND_COUNT;
		static const long		s_lDEFAULT_SEND_INTERVAL_IN_MS;
		static const int		s_iDEFAULT_MAX_SEND_COUNT;
    
	private:
		Multicast()			{}
		virtual ~Multicast(){}
	};

	class ETHERNETCOMMS_API UDP
	{
	public:
		static const unsigned short     s_usDEFAULT_UDP_PORT;
		static const int				s_iINDEFINET_SEND_COUNT;
		static const long				s_lDEFAULT_SEND_INTERVAL_IN_MS;  
		static const int				s_iDEFAULT_MAX_SEND_COUNT;
	private:
		UDP()			{}
		virtual ~UDP()	{}
	};

	class ETHERNETCOMMS_API TCP
	{
	public:
		
	private:
		TCP()			{}
		virtual ~TCP()	{}
	};

	const std::string        s_strINVALID_IP_ADDRESS	=	"0.0.0.0";
        const int                s_iINVALID_PORT		=	0;
};

#endif	/* COMMSUTILITIES_H */

