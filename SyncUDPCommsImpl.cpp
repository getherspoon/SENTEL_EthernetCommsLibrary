/* 
 * File:   CSyncUDPCommsImpl.cpp
 * Author: Josh
 *
 * Created on September 9, 2011, 5:42 PM
 *
 * This class is a shell for the Synchronous UDP Communication classes
 * Via this class, a developer can instantiate an Snyc Sender, an Sync Receiver, 
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
#include "SyncUDPComms.h"

#include "SyncUDPCommsImpl.h"
#include "SyncUDPReceiver.h"
#include "SyncUDPSender.h"

#ifndef NO_MFC

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif

// Receiver Only Constructor
// 
// This constructor allows instantiation of Sync UDP Comms which will only Recv and not Send messages
// If RecvLoopbackEnabled is true, the receiver will listen to local messages as well as foriegn ones.
//   This is typically only advised if there is no sender or if the sender is sending to different group.
CSyncUDPCommsImpl::CSyncUDPCommsImpl(   boost::asio::io_service& p_bstIOServiceRecv,
										const unsigned short p_usUDPPort )
:
CSyncConnectionlessCommsImpl<CSyncUDPReceiver, CSyncUDPSender>( CommsUtilities::UDP )
{
	this->ResetRecv( new CSyncUDPReceiver( p_bstIOServiceRecv, p_usUDPPort ) );
}

// Sender Only Constructor
// 
// This constructor allows instantiation of Sync UDP Comms which will only send and not receive messages
CSyncUDPCommsImpl::CSyncUDPCommsImpl(   boost::asio::io_service& p_bstIOServiceSend,
										const std::string& p_strUDPAddress, 
										const unsigned short p_usUDPPort )
:
CSyncConnectionlessCommsImpl<CSyncUDPReceiver, CSyncUDPSender>( CommsUtilities::UDP ) 
{
	this->ResetSend( new CSyncUDPSender( p_bstIOServiceSend, p_strUDPAddress, p_usUDPPort ) );
}

// Sender and Reciver on Same Port/Address Constructor
//
// This constructor will instantiate an Sync UDP Comms object with both a sender and Receiver on the same Group (IP/Port)
CSyncUDPCommsImpl::CSyncUDPCommsImpl(   boost::asio::io_service& p_bstIOServiceSend,
										boost::asio::io_service& p_bstIOServiceRecv,
										const std::string& p_strUDPAddress, 
										const unsigned short p_usUDPPort )
:
CSyncConnectionlessCommsImpl<CSyncUDPReceiver, CSyncUDPSender>( CommsUtilities::UDP ) 
{
	this->ResetSend( new CSyncUDPSender( p_bstIOServiceSend, p_strUDPAddress, p_usUDPPort ) );
	this->ResetRecv( new CSyncUDPReceiver( p_bstIOServiceRecv, p_usUDPPort ) );
}

// Sender and Reciver on Different Port/Address Constructor
//
// This constructor will instantiate an Sync UDP Comms object with a Sender on IP and Port and Receiver on a different Port
CSyncUDPCommsImpl::CSyncUDPCommsImpl(   boost::asio::io_service& p_bstIOServiceSend,
										boost::asio::io_service& p_bstIOServiceRecv,
										const unsigned short p_usUDPPortRecv, 
										const std::string& p_strUDPAddressSend, 
										const unsigned short p_usUDPPortSend )
:
CSyncConnectionlessCommsImpl<CSyncUDPReceiver, CSyncUDPSender>( CommsUtilities::UDP ) 
{
	this->ResetSend( new CSyncUDPSender( p_bstIOServiceSend, p_strUDPAddressSend, p_usUDPPortSend  ) );
	this->ResetRecv( new CSyncUDPReceiver( p_bstIOServiceRecv, p_usUDPPortRecv ) );

}
