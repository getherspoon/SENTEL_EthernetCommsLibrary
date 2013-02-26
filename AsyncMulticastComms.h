/* 
 * File:   CAsyncMulticastComms.h
 * Author: Josh
 *
 * Created on December 3, 2012
 *
 * This class is a shell for the Asynchronous Multicast Communication classes
 * Via this class, a developer can instantiate an Asnyc Sender, an Async Receiver, 
 * or Both, using the same or different Multicast Groups (IP/Port).
 * 
 * This is the class which is too be used for Async Multicast Comms and derives from
 * CAsyncConnectionlessComms<CAsyncMulticastCommsImpl>, which will take care of all the 
 * underlying IO responsibilities
 *
 * Unless otherwise noted, all comments are in the CPP file with only basic comments in this Header file.
 *
 * For usage, please see ReadMe-Multicast.txt
 *
 */
#ifndef CASYNCMULTICASTCOMMS_H
#define	CASYNCMULTICASTCOMMS_H

#include "AsyncMulticastCommsImpl.h"
#include "AsyncConnectionlessComms.h"
#include "EthernetCommsAPI.h"

class ETHERNETCOMMS_API CAsyncMulticastComms : public CAsyncConnectionlessComms<CAsyncMulticastCommsImpl>
{
public:	
	// Recv Only
	CAsyncMulticastComms(	const std::string& p_strMulticastAddress, 
							const unsigned short p_usMulticastPort,						
							const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstPostRecvMessageCallback,							
							const bool p_bRecvLoopbackEnabled );

	// Send Only
	CAsyncMulticastComms(	const std::string& p_strMulticastAddress, 
							const unsigned short p_usMulticastPort,													
							const Callback_ConstRefString_Error p_bstPostSendMessageCallback );

	// Send and Recv from the Same IP and Port
	CAsyncMulticastComms(	const std::string& p_strMulticastAddress, 
							const unsigned short p_usMulticastPort,						
							const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstPostRecvMessageCallback,
							const Callback_ConstRefString_Error p_bstPostSendMessageCallback,
							const bool p_bRecvLoopbackEnabled );

	// Send to one Group, Recv from another Group
	CAsyncMulticastComms(	const std::string& p_strMulticastAddressRecv, 
							const unsigned short p_usMulticastPortRecv,							
							const std::string& p_strMulticastAddressSend, 
							const unsigned short p_usMulticastPortSend,
							const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstPostRecvMessageCallback,
							const Callback_ConstRefString_Error p_bstPostSendMessageCallback,
							const bool p_bRecvLoopbackEnabled );

	// Destructor
	virtual ~CAsyncMulticastComms() {}

private:
	// Reset the Comms Object
	virtual void ResetComms( CAsyncMulticastCommsImpl* p_pComms )
	{
		CAsyncConnectionlessComms<CAsyncMulticastCommsImpl>::ResetComms( p_pComms );
	}

	// Get the Comms Object
	virtual CAsyncMulticastCommsImpl* GetComms() const
	{
		return CAsyncConnectionlessComms<CAsyncMulticastCommsImpl>::GetComms();
	}

	// Get the Recv IO Service
	virtual boost::asio::io_service& GetIOServiceRecv()
	{
		return CAsyncConnectionlessComms<CAsyncMulticastCommsImpl>::GetIOServiceRecv();
	}

	// Get the Send IO Service
	virtual boost::asio::io_service& GetIOServiceSend()
	{
		return CAsyncConnectionlessComms<CAsyncMulticastCommsImpl>::GetIOServiceSend();
	}
};

#endif // CASYNCMULTICASTCOMMS_H