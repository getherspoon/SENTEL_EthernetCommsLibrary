EthernetCommsModule: Multicast
==============================

This set of classes is indended to aid in the development of integrations requiring multicast communications.

From a developer's perspective, there are 2 classes which should typically be used:
	CAsyncMulticastComms (CAsyncMulticastComms.cpp/.h)
	and
	CSyncMulticastComms (CSyncMulticastComms.cpp/.h)

The other classes cover the lower-level details, such as Boost ASIO & IO Services.  

Using these classes, a developer need only define the Multicast Address(es) and 
Port(s) for the Sender and Receiver, values which can be the same for each or different for each, 
the type of Comms System (Sychronous or Asynchronous) and if Loopback should be enabled for the Receiver.

For Asynchronous comms, Sender and/or Receiver callbacks must also be defined

Usage:
======

// Async Multicast Example
//
// Assume "m_pComms" is a member varialbe defined as: "std::auto_ptr<CAsyncMulticastComms>"
#include "AsyncMulticastComms.h"
#include "AsyncMulticastCommsImpl.h"
#include "AsyncMulticastReceiver.h"
#include "AsyncMulticastSender.h"

// Define Callbacks
void CMyClass::HandleRecvMessage( const std::string& p_strMsg, const std::string& p_strIpAddress, const unsigned int p_usPort, const boost::system::error_code& p_bstError )
{
	if( p_bstError )
	{
		TRACE("Recv Error: %s\n", bstError.message().c_str() );
	}
	else
	{
		TRACE("Message: %s, was recv'd successfully from %s:%d\n", p_strMsg.c_str(), p_strIpAddress.c_str(), p_usPort );
		// Now send an Echo
		boost::system::error_code bstError;
		this->m_pComms->Send( p_strMsg, bstError );
	}
}

void CMyClass::HandleSendMessage( const std::string& p_strMsg, const boost::system::error_code& p_bstError )
{
	if( p_bstError )
	{
		TRACE("Send Error: %s\n", bstError.message().c_str() );
	}
	else
	{
		TRACE("Message: %s, was sent successfully\n", p_strMsg.c_str() );
	}
}

void CMyClass::RunComms()
{
	// Instantiate and Initialize your Comms
	boost::system::error_code bstError;
	pComms.reset( new CMulticastComms(	strRecvIpAddress, 
										usRecvPort, 
										strSendIpAddress, 
										usSendPort, 
										boost::bind( &CMyClass::HandleRecvMessage, this, _1, _2, _3, _4 ),
										boost::bind( &CMyClass::HandleSendMessage, this, _1, _2 ), 
										true ) );
	
	// Init the Comms and check for an Error
	if( !pComms->InitializeComms( bstError ) )
	{
		TRACE("Initialization Error: %s\n", bstError.message().c_str() );
	}
	else
	{
		// Multicast Receiver is now Listening
	}
}

-------------------------------------------------------------

// Sync Multicast Example
//
// Assume "m_pComms" is a member variable defined as: "std::auto_ptr<CSyncMulticastComms>"
// Assume "m_bstListen" is a member variable defined as: "std::auto_ptr<boost::thread>"
// Assume "m_bListen" is a member variable defined as: "bool"
#include "SyncMulticastComms.h"
#include "SyncMulticastCommsImpl.h"
#include "SyncMulticastReceiver.h"
#include "SyncMulticastSender.h"

// Define Listener Thread
void CMyClass::ListenThread()
{
	this->m_bListen = true;

	while( this->m_bListen )
	{
		std::string strMsg, strAddress;
		unsigned short usPort;
		boost::system::error_code bstError;
		if( this->m_pComms->Receive( strMsg, strAddress, usPort, bstError, 5000 ) && !bstError ) // 5000 means a 5 sec Recv Timeout
		{		
			TRACE("Message: %s, was recv'd successfully from %s:%d\n", strMsg.c_str(), strAddress.c_str(), usPort );
			// Now send an Echo			
			this->m_pComms->Send( p_strMsg, bstError );
			if( p_bstError )
			{
				TRACE("Send Error: %s\n", bstError.message().c_str() );
			}
			else
			{
				TRACE("Message: %s, was sent successfully\n", p_strMsg.c_str() );
			}		
		}
		else if( p_bstError )
		{
			TRACE("Recv Error: %s\n", bstError.message().c_str() );		
		}
	}		
}

void CMyClass::RunComms()
{
	// Instantiate and Initialize your Comms
	boost::system::error_code bstError;
	pComms.reset( new CSyncMulticastComms(	strRecvIpAddress, 
											usRecvPort, 
											strSendIpAddress, 
											usSendPort, 
											true ) );
	
	// Init the Comms and check for an Error
	if( !pComms->InitializeComms( bstError ) )
	{
		TRACE("Initialization Error: %s\n", bstError.message().c_str() );
	}
	else
	{
		// Manually start a Listener Thread which will use the Blocking Recv Method
		this->m_bstListen.reset( new boost::thread( boost::bind( &CMyClass::ListenThread, this ) ) );
	}
}

