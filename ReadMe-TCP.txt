EthernetCommsModule: TCP
==============================

This set of classes is indended to aid in the development of integrations requiring TCP communications.

From a developer's perspective, there are 2 classes which should typically be used:
	CAsyncTCPComms (AsyncTCPComms.cpp/.h)
	and
	CSyncTCPComms (SyncTCPComms.cpp/.h)

The other classes cover the lower-level details, such as Boost ASIO & IO Services.  

Using these classes, a developer need only define the TCP Address and 
Port for the Connection. 

For Asynchronous comms, Send and Read callbacks are required to be defined.

Usage:
======

// Async TCP Example
//
// Assume "m_pComms" is a member variable defined as: "std::auto_ptr<CAsyncTCPComms>"
#include "AsyncTCPComms.h"

// Define Callbacks
void CMyClass::HandleRecvMessage( const std::string& p_strMsg, const boost::system::error_code& p_bstError )
{
	if( p_bstError )
	{
		TRACE("Recv Error: %s\n", bstError.message().c_str() );
	}
	else
	{
		TRACE("Message: %s, was recv'd successfully\n", p_strMsg.c_str());
		// Now send an Echo
		boost::system::error_code bstError;
		this->m_pComms->Send( p_strMsg, bstError );
	}
}
void CMyClass::HandleSendMessage(const boost::system::error_code& p_bstError, const std::size_t &st )
{
	if( p_bstError )
	{
		TRACE("Send Error: %s\n", bstError.message().c_str() );
	}
	else
	{
		TRACE("%d bytes were sent successfully\n", st );
	}
}

void CMyClass::RunComms()
{
	// Instantiate and Initialize your Comms
	boost::system::error_code bstError;
	m_pComms.reset( new CAsyncTCPComms( strIpAddress, 
										usPort, 
										boost::bind( &CMyClass::HandleRecvMessage, this, _1, _2 ),
										boost::bind( &CMyClass::HandleSendMessage, this, _1, _2 ) ) );
	//Communications are now active.
	
}

-------------------------------------------------------------

// Sync TCP Example
//
// Assume "m_pComms" is a member variable defined as: "std::auto_ptr<CSyncTCPComms>"

#include "SyncTCPComms.h"

// Define Callbacks
void CMyClass::ListenThread()
{
	this->m_bListen = true;

	while( this->m_bListen )
	{
		std::string strMsg, strAddress;
		unsigned short usPort;
		boost::system::error_code bstError;
		if( this->m_pComms->Read( strMsg, bstError, 5000 ) && !bstError ) // This means a 5 sec timeout on the Recv.
		{		
			TRACE("Message: %s, was recv'd successfully\n", strMsg.c_str());
			// Now send an Echo			
			this->m_pComms->Send( p_strMsg, bstError, 5000 ); //This means a 5 sec timeout on the Send
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
	m_pComms.reset( new CUDPComms(	strIpAddress, 
									usPort ) );
	//Communications are now active.								
	// Manually start a Listener Thread which will use the Blocking Recv Method
	this->m_bstListen.reset( new boost::thread( boost::bind( &CMyClass::ListenThread, this ) ) );
}

