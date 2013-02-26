/* 
 * File:   CAsyncUDPComms.h
 * Author: Josh
 *
 * Created on December 3, 2012
 *
 * This class is a shell for the Asynchronous UDP Communication classes
 * Via this class, a developer can instantiate an Asnyc Sender, an Async Receiver, 
 * or Both, using the same or different IP Addresses and/or Ports
 * 
 * This is the class which is too be used for Async UDP Comms and derives from
 * CAsyncConnectionlessComms<CAsyncUDPCommsImpl>, which will take care of all the 
 * underlying IO responsibilities
 *
 * Unless otherwise noted, all comments are in the CPP file with only basic comments in this Header file.
 *
 * For usage, please see ReadMe-UDP.txt
 *
 */
#ifndef CASYNCMULTICASTCOMMS_H
#define	CASYNCMULTICASTCOMMS_H

#include "AsyncConnectionlessComms.h"
#include "EthernetCommsAPI.h"

class CAsyncUDPCommsImpl;

class ETHERNETCOMMS_API CAsyncUDPComms : public CAsyncConnectionlessComms<CAsyncUDPCommsImpl>
{
public:	
	// Recv Only
	CAsyncUDPComms(	const unsigned short p_usUDPPort,						
					const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstPostRecvMessageCallback );

	// Send Only
	CAsyncUDPComms(	const std::string& p_strUDPAddress, 
					const unsigned short p_usUDPPort,													
					const Callback_ConstRefString_Error p_bstPostSendMessageCallback );

	// Send and Recv from the Same IP and Port
	CAsyncUDPComms(	const std::string& p_strUDPAddress, 
					const unsigned short p_usUDPPort,						
					const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstPostRecvMessageCallback,
					const Callback_ConstRefString_Error p_bstPostSendMessageCallback );

	// Send to an IP and Port, Recv on another Port
	CAsyncUDPComms(	const unsigned short p_usUDPPortRecv,							
					const std::string& p_strUDPAddressSend, 
					const unsigned short p_usUDPPortSend,
					const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstPostRecvMessageCallback,
					const Callback_ConstRefString_Error p_bstPostSendMessageCallback );

	virtual ~CAsyncUDPComms() {}

private:
	// Reset the Comms Object
	virtual void ResetComms( CAsyncUDPCommsImpl* p_pComms )
	{
		CAsyncConnectionlessComms<CAsyncUDPCommsImpl>::ResetComms( p_pComms );
	}

	// Get the Comms Object
	virtual CAsyncUDPCommsImpl* GetComms() const
	{
		return CAsyncConnectionlessComms<CAsyncUDPCommsImpl>::GetComms();
	}

	// Get the Recv IO Service
	virtual boost::asio::io_service& GetIOServiceRecv()
	{
		return CAsyncConnectionlessComms<CAsyncUDPCommsImpl>::GetIOServiceRecv();
	}

	// Get the Send IO Service
	virtual boost::asio::io_service& GetIOServiceSend()
	{
		return CAsyncConnectionlessComms<CAsyncUDPCommsImpl>::GetIOServiceSend();
	}
};

#endif // CASYNCMULTICASTCOMMS_H