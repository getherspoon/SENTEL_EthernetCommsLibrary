/* 
 * File:   CAsyncUDPComms.h
 * Author: Josh
 *
 * Created on September 9, 2011, 5:42 PM
 *
 * This class is a shell for the Asynchronous UDP Communication classes
 * Via this class, a developer can instantiate an Asnyc Sender, an Async Receiver, 
 * or Both, using the same or different UDP Address & Port
 * 
 * Typically, this class would be accessed via the CUDPComms class, which allows
 * the instantiation of either Sync or Async comms and handles all of the underlying details
 * 
 * While it is allowed for this class to be used on its own, it is not advised, as it then requires
 * the developer to handle the ASIO IO Services and threads. Using the CUDPComms class will 
 * save time, frustration, and lower the learning curve.
 *
 * Unless otherwise noted, all comments are in the CPP file with only basic comments in this Header file.
 *
 * For usage, please see ReadMe-UDP.txt
 *
 */

#ifndef CASYNCUDPCOMMS_H
#define	CASYNCUDPCOMMS_H

#include "AsyncConnectionlessCommsImpl.h"
#include "EthernetCommsAPI.h"

#include <memory>

#include <boost/system/error_code.hpp>

class CAsyncUDPReceiver;
class CAsyncUDPSender;

class ETHERNETCOMMS_API CAsyncUDPCommsImpl : public CAsyncConnectionlessCommsImpl<CAsyncUDPReceiver,CAsyncUDPSender>
{
public:
	// Recv Only
	CAsyncUDPCommsImpl(	  boost::asio::io_service& p_bstIOServiceRecv,                          
						  const unsigned short p_usUDPPort,
						  const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_pRecvCallback );   

	// Send Only
	CAsyncUDPCommsImpl(	boost::asio::io_service& p_bstIOServiceSend,                          
						const std::string& p_strUDPAddress, 
						const unsigned short p_usUDPPort,                          
						const Callback_ConstRefString_Error p_pSendCallback = NULL );   

	// Send and Recv on Same Port
    CAsyncUDPCommsImpl( boost::asio::io_service& p_bstIOServiceRecv,                          
						boost::asio::io_service& p_bstIOServiceSend,
						const std::string& p_strUDPAddress, 
						const unsigned short p_usUDPPort,					
						const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_pRecvCallback,
						const Callback_ConstRefString_Error p_pSendCallback );    

	// Send and Recv on Different Ports
    CAsyncUDPCommsImpl( boost::asio::io_service& p_bstIOServiceSend,
						boost::asio::io_service& p_bstIOServiceRecv,
						const unsigned short p_usUDPPortRecv,
						const std::string& p_strUDPAddressSend, 
						const unsigned short p_usUDPPortSend,						
						const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_pRecvCallback,
						const Callback_ConstRefString_Error p_pSendCallback );  

	virtual ~CAsyncUDPCommsImpl() {}  

private:
	// Reset the Recv Object
	virtual void ResetRecv( CAsyncUDPReceiver* p_pRecv )
	{
		CAsyncConnectionlessCommsImpl<CAsyncUDPReceiver,CAsyncUDPSender>::ResetRecv( p_pRecv );
	}

	// Rest the Send Object
	virtual void ResetSend( CAsyncUDPSender* p_pSend )
	{
		CAsyncConnectionlessCommsImpl<CAsyncUDPReceiver,CAsyncUDPSender>::ResetSend( p_pSend );
	}

	// Get the Recv Object
	virtual CAsyncUDPReceiver* GetRecv() const
	{
		return CAsyncConnectionlessCommsImpl<CAsyncUDPReceiver,CAsyncUDPSender>::GetRecv();
	}

	// Get the Send Object
	virtual CAsyncUDPSender* GetSend() const
	{
		return CAsyncConnectionlessCommsImpl<CAsyncUDPReceiver,CAsyncUDPSender>::GetSend();
	}
};

#endif	/* CASYNCUDPCOMMS_H */

