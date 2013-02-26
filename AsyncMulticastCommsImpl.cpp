/* 
 * File:   CAsyncMulticastCommsImpl.cpp
 * Author: Josh
 *
 * Created on September 9, 2011, 5:42 PM
 *
 * This class is a shell for the Asynchronous Multicast Communication classes
 * Via this class, a developer can instantiate an Asnyc Sender, an Async Receiver, 
 * or Both, using the same or different Multicast Groups (IP/Port).
 * 
 * Typically, this class would be accessed via the CMulticastComms class, which allows
 * the instantiation of either Sync or Async comms and handles all of the underlying details
 * 
 * While it is allowed for this class to be used on its own, it is not advised, as it then requires
 * the developer to handle the ASIO IO Services and threads. Using the CMulticastComms class will 
 * save time, frustration, and lower the learning curve.
 *
 * Unless otherwise noted, all comments are in this CPP file with only basic comments in the Header file.
 *
 * For usage, please see ReadMe-Multicast.txt
 *
 */
#include "stdafx.h"
#include "AsyncMulticastComms.h"

#include "AsyncMulticastReceiver.h"
#include "AsyncMulticastSender.h"

#ifndef NO_MFC

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif

// Receiver Only Constructor
// 
// This constructor allows instantiation of Async Multicast Comms which will only Recv and not Send messages
// A Recv Callback is required
// If RecvLoopbackEnabled is true, the receiver will listen to local messages as well as foriegn ones.
//   This is typically only advised if there is no sender or if the sender is sending to different group.
CAsyncMulticastCommsImpl::CAsyncMulticastCommsImpl(   boost::asio::io_service& p_bstIOServiceRecv,
													  const std::string& p_strListenAddress, 
													  const std::string& p_strMulticastAddress, 
													  const unsigned short p_usMulticastPort,
													  const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_pRecvCallback,
													  const bool p_bRecvLoopbackEnabled ) 
:
CAsyncConnectionlessCommsImpl<CAsyncMulticastReceiver,CAsyncMulticastSender>( CommsUtilities::MULTICAST )
{	
	this->ResetRecv( new CAsyncMulticastReceiver( p_bstIOServiceRecv, p_strListenAddress, p_strMulticastAddress, p_usMulticastPort, p_pRecvCallback, p_bRecvLoopbackEnabled ) );
}

// Sender Only Constructor
// 
// This constructor allows instantiation of Async Multicast Comms which will only send and not receive messages
// A Send Callback is required
CAsyncMulticastCommsImpl::CAsyncMulticastCommsImpl(   boost::asio::io_service& p_bstIOServiceSend,
													  const std::string& p_strMulticastAddress, 
													  const unsigned short p_usMulticastPort,
													  const Callback_ConstRefString_Error p_pSendCallback ) 
:
CAsyncConnectionlessCommsImpl<CAsyncMulticastReceiver,CAsyncMulticastSender>( CommsUtilities::MULTICAST )
{
	this->ResetSend( new CAsyncMulticastSender( p_bstIOServiceSend, p_strMulticastAddress, p_usMulticastPort, p_pSendCallback ) );	
}

// Sender and Reciver on Same Port/Address Constructor
//
// This constructor will instantiate an Async Multicast Comms object with both a sender and Receiver on the same Group (IP/Port)
// A Recv Callback is required
// A Send Callback is required
// If RecvLoopbackEnabled is true, the receiver will listen to local messages as well as foriegn ones.
//   This is typically only advised if there is no sender or if the sender is sending to different group.
CAsyncMulticastCommsImpl::CAsyncMulticastCommsImpl(   boost::asio::io_service& p_bstIOServiceRecv,
													  boost::asio::io_service& p_bstIOServiceSend,
													  const std::string& p_strListenAddress, 
													  const std::string& p_strMulticastAddress, 
													  const unsigned short p_usMulticastPort,                                              													  
													  const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_pRecvCallback,
													  const Callback_ConstRefString_Error p_pSendCallback,
													  const bool p_bRecvLoopbackEnabled )
:
CAsyncConnectionlessCommsImpl<CAsyncMulticastReceiver,CAsyncMulticastSender>( CommsUtilities::MULTICAST )
{
	this->ResetSend( new CAsyncMulticastSender( p_bstIOServiceSend, p_strMulticastAddress, p_usMulticastPort, p_pSendCallback ) );
	this->ResetRecv( new CAsyncMulticastReceiver( p_bstIOServiceRecv, p_strListenAddress, p_strMulticastAddress, p_usMulticastPort, p_pRecvCallback, p_bRecvLoopbackEnabled ) );
}

// Sender and Reciver on Different Port/Address Constructor
//
// This constructor will instantiate an Async Multicast Comms object with a Sender on one Group (IP/Port) and Receiver on a different Group (IP/Port)
// A Recv Callback is required
// A Send Callback is required
// If RecvLoopbackEnabled is true, the receiver will listen to local messages as well as foriegn ones.
//   This is typically only advised if there is no sender or if the sender is sending to different group.
CAsyncMulticastCommsImpl::CAsyncMulticastCommsImpl(   boost::asio::io_service& p_bstIOServiceRecv,
													  boost::asio::io_service& p_bstIOServiceSend,
													  const std::string& p_strListenAddress, 
													  const std::string& p_strMulticastAddressRecv, 
													  const unsigned short p_usMulticastPortRecv,
													  const std::string& p_strMulticastAddressSend, 
													  const unsigned short p_usMulticastPortSend,													  													  
													  const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_pRecvCallback,
													  const Callback_ConstRefString_Error p_pSendCallback,
													  const bool p_bRecvLoopbackEnabled ) 
:
CAsyncConnectionlessCommsImpl<CAsyncMulticastReceiver,CAsyncMulticastSender>( CommsUtilities::MULTICAST )
{
	this->ResetSend( new CAsyncMulticastSender( p_bstIOServiceSend, p_strMulticastAddressSend, p_usMulticastPortSend, p_pSendCallback ) );
	this->ResetRecv( new CAsyncMulticastReceiver( p_bstIOServiceRecv, p_strListenAddress, p_strMulticastAddressRecv, p_usMulticastPortRecv, p_pRecvCallback, p_bRecvLoopbackEnabled ) );
}
