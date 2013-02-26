/* 
 * File:   CAsyncUDPComms.cpp
 * Author: Josh
 *
 * Created on September 9, 2011, 5:42 PM
 *
 * This class is a shell for the Asynchronous UDP Communication classes
 * Via this class, a developer can instantiate an Asnyc Sender, an Async Receiver, 
 * or Both, using the same or different UDP Groups (IP/Port).
 * 
 * Typically, this class would be accessed via the CUDPComms class, which allows
 * the instantiation of either Sync or Async comms and handles all of the underlying details
 * 
 * While it is allowed for this class to be used on its own, it is not advised, as it then requires
 * the developer to handle the ASIO IO Services and threads. Using the CUDPComms class will 
 * save time, frustration, and lower the learning curve.
 *
 * Unless otherwise noted, all comments are in this CPP file with only basic comments in the Header file.
 *
 * For usage, please see ReadMe-UDP.txt
 *
 */
#include "stdafx.h"
#include "AsyncUDPComms.h"

#include "AsyncUDPCommsImpl.h"
#include "AsyncUDPReceiver.h"
#include "AsyncUDPSender.h"

#ifndef NO_MFC

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif

// Receiver Only Constructor
// 
// This constructor allows instantiation of Async UDP Comms which will only Recv and not Send messages
// A Recv Callback is required
// If RecvLoopbackEnabled is true, the receiver will listen to local messages as well as foriegn ones.
//   This is typically only advised if there is no sender or if the sender is sending to different group.
CAsyncUDPCommsImpl::CAsyncUDPCommsImpl( boost::asio::io_service& p_bstIOServiceRecv,
										const unsigned short p_usUDPPort,
										const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_pRecvCallback ) 
:
CAsyncConnectionlessCommsImpl<CAsyncUDPReceiver,CAsyncUDPSender>( CommsUtilities::UDP )
{
	this->ResetRecv( new CAsyncUDPReceiver( p_bstIOServiceRecv, p_usUDPPort, p_pRecvCallback ) );
}

// Sender Only Constructor
// 
// This constructor allows instantiation of Async UDP Comms which will only send and not receive messages
// A Send Callback is required
CAsyncUDPCommsImpl::CAsyncUDPCommsImpl(	boost::asio::io_service& p_bstIOServiceSend,
										const std::string& p_strUDPAddress, 
										const unsigned short p_usUDPPort,
										const Callback_ConstRefString_Error p_pSendCallback )  
:
CAsyncConnectionlessCommsImpl<CAsyncUDPReceiver,CAsyncUDPSender>( CommsUtilities::UDP )
{
	this->ResetSend( new CAsyncUDPSender( p_bstIOServiceSend, p_strUDPAddress, p_usUDPPort, p_pSendCallback ) );
}

// Sender and Reciver on Same Port/Address Constructor
//
// This constructor will instantiate an Async UDP Comms object with both a Sender and Receiver with the same IP and Port
// A Recv Callback is required
// A Send Callback is required
// If RecvLoopbackEnabled is true, the receiver will listen to local messages as well as foriegn ones.
//   This is typically only advised if there is no sender or if the sender is sending to different group.
CAsyncUDPCommsImpl::CAsyncUDPCommsImpl(	boost::asio::io_service& p_bstIOServiceRecv,
										boost::asio::io_service& p_bstIOServiceSend,
								  		const std::string& p_strUDPAddress, 
										const unsigned short p_usUDPPort,                                  									  
										const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_pRecvCallback,
										const Callback_ConstRefString_Error p_pSendCallback ) 
:
CAsyncConnectionlessCommsImpl<CAsyncUDPReceiver,CAsyncUDPSender>( CommsUtilities::UDP ) 
{
	this->ResetSend( new CAsyncUDPSender( p_bstIOServiceSend, p_strUDPAddress, p_usUDPPort, p_pSendCallback ) );
	this->ResetRecv( new CAsyncUDPReceiver( p_bstIOServiceRecv, p_usUDPPort, p_pRecvCallback ) );
}

// Sender and Reciver on Different Port/Address Constructor
//
// This constructor will instantiate an Async Multicast Comms object with a Sender with one IP and Port and Receiver with different IP and Port values
// A Recv Callback is required
// A Send Callback is required
// If RecvLoopbackEnabled is true, the receiver will listen to local messages as well as foriegn ones.
//   This is typically only advised if there is no sender or if the sender is sending to different group.
CAsyncUDPCommsImpl::CAsyncUDPCommsImpl( boost::asio::io_service& p_bstIOServiceRecv,
										boost::asio::io_service& p_bstIOServiceSend,									
										const unsigned short p_usUDPPortRecv,
										const std::string& p_strUDPAddressSend, 
										const unsigned short p_usUDPPortSend,									
										const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_pRecvCallback,
										const Callback_ConstRefString_Error p_pSendCallback ) 
:
CAsyncConnectionlessCommsImpl<CAsyncUDPReceiver,CAsyncUDPSender>( CommsUtilities::UDP ) 
{
	this->ResetSend( new CAsyncUDPSender( p_bstIOServiceSend, p_strUDPAddressSend, p_usUDPPortSend, p_pSendCallback ) );
	this->ResetRecv( new CAsyncUDPReceiver( p_bstIOServiceRecv, p_usUDPPortRecv, p_pRecvCallback ) );

}
