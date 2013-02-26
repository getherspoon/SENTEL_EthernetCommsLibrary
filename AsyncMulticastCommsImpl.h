/* 
 * File:   CAsyncMulticastCommsImpl.h
 * Author: Josh
 *
 * Created on September 9, 2011, 5:42 PM
 *
 * This class is an Implementation class Asynchronous Multicast Communications
 * Via this class, a developer can instantiate an Asnyc Sender, an Async Receiver, 
 * or Both, using the same or different Multicast Groups (IP/Port).
 * 
 * Typically, this class would be accessed via the CAsyncMulticastComms class, which allows
 * the instantiation of Async Multicast comms and handles all of the underlying details
 * 
 * While it is allowed for this class to be used on its own, it is not advised, as it then requires
 * the developer to handle the ASIO IO Services and threads. Using the CAsyncMulticastComms class will 
 * save time, frustration, and lower the learning curve.
 *
 * Unless otherwise noted, all comments are in the CPP file with only basic comments in this Header file.
 *
 * For usage, please see ReadMe-Multicast.txt
 *
 */

#ifndef CASYNCMULTICASTCOMMSIMPL_H
#define	CASYNCMULTICASTCOMMSIMPL_H

#include "AsyncConnectionlessCommsImpl.h"
#include "EthernetCommsAPI.h"

#include <string>

#include <boost/system/system_error.hpp>

class CAsyncMulticastReceiver;
class CAsyncMulticastSender;

class ETHERNETCOMMS_API CAsyncMulticastCommsImpl : public CAsyncConnectionlessCommsImpl<CAsyncMulticastReceiver,CAsyncMulticastSender>
{
public:	
	// Receiver Only
	CAsyncMulticastCommsImpl( boost::asio::io_service& p_bstIOServiceRecv,
							  const std::string& p_strListenAddress, 
							  const std::string& p_strMulticastAddress, 
							  const unsigned short p_usMulticastPort,
							  const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_pRecvCallback,                          
							  const bool p_bRecvLoopbackEnabled ); 

	// Sender Only
	CAsyncMulticastCommsImpl( boost::asio::io_service& p_bstIOServiceSend,                                                    
							  const std::string& p_strMulticastAddress, 
							  const unsigned short p_usMulticastPort,                          
							  const Callback_ConstRefString_Error p_pSendCallback ); 

	// Sender and Receiver on Same Port & Address
    CAsyncMulticastCommsImpl( boost::asio::io_service& p_bstIOServiceRecv,
							  boost::asio::io_service& p_bstIOServiceSend,							  
							  const std::string& p_strListenAddress, 
							  const std::string& p_strMulticastAddress, 
							  const unsigned short p_usMulticastPort,                          
							  const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_pRecvCallback,
							  const Callback_ConstRefString_Error p_pSendCallback,
							  const bool p_bRecvLoopbackEnabled );    

	// Sender and Receiver on Different Ports and Addresses
    CAsyncMulticastCommsImpl( boost::asio::io_service& p_bstIOServiceRecv,
							  boost::asio::io_service& p_bstIOServiceSend,
							  const std::string& p_strListenAddress, 
							  const std::string& p_strMulticastAddressRecv, 
							  const unsigned short p_usMulticastPortRecv,                          
							  const std::string& p_strMulticastAddressSend, 
							  const unsigned short p_usMulticastPortSend,
							  const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_pRecvCallback,
							  const Callback_ConstRefString_Error p_pSendCallback,							  
							  const bool p_bRecvLoopbackEnabled );  

	// Destructor
	virtual ~CAsyncMulticastCommsImpl() {}

private:
	// Reset the Recv Object
	virtual void ResetRecv( CAsyncMulticastReceiver* p_pRecv )
	{
		CAsyncConnectionlessCommsImpl<CAsyncMulticastReceiver,CAsyncMulticastSender>::ResetRecv( p_pRecv );
	}

	// Rest the Send Object
	virtual void ResetSend( CAsyncMulticastSender* p_pSend )
	{
		CAsyncConnectionlessCommsImpl<CAsyncMulticastReceiver,CAsyncMulticastSender>::ResetSend( p_pSend );
	}

	// Get the Recv Object
	virtual CAsyncMulticastReceiver* GetRecv() const
	{
		return CAsyncConnectionlessCommsImpl<CAsyncMulticastReceiver,CAsyncMulticastSender>::GetRecv();
	}

	// Get the Send Object
	virtual CAsyncMulticastSender* GetSend() const
	{
		return CAsyncConnectionlessCommsImpl<CAsyncMulticastReceiver,CAsyncMulticastSender>::GetSend();
	}
};

#endif	/* CASYNCMULTICASTCOMMSIMPL_H */

