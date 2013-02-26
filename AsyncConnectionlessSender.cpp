/* 
 * File:   AsyncConnectionlessReceiver.cpp
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class declares and defines all common code used by Asynchronous Connectionless Sender classes
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#include "stdafx.h"
#include "AsyncConnectionlessSender.h"

#ifndef NO_MFC

#ifndef BOOST_THREAD_DYN_LINK
#define BOOST_THREAD_DYN_LINK
#endif

#else

#ifndef BOOST_THREAD_USE_LIB
#define BOOST_THREAD_USE_LIB
#endif

#endif

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/timer.hpp>

// Constructor
// A sender callback is required
CAsyncConnectionlessSender::CAsyncConnectionlessSender( boost::asio::io_service& p_bstIOService,
														const Callback_ConstRefString_Error p_bstSendCallback,
														const long p_lSendIntervalInMs )
:
m_bstSendCallback( p_bstSendCallback ),
m_iMaxSendCount( 0 ),
m_iSendCount( 0 ),
m_lSendIntervalInMs( p_lSendIntervalInMs )
{		        
	this->m_bstWork.reset( new boost::asio::io_service::work( p_bstIOService ) );
	this->m_bstTimer.reset( new boost::asio::deadline_timer( p_bstIOService ) );
}

// Destructor
// 
// Stop Sending and Close the Sender
CAsyncConnectionlessSender::~CAsyncConnectionlessSender()
{	
	boost::system::error_code bstError;
    if( !this->TerminateSender( bstError ) )
	{
		ASSERT( false );
		TRACE( "ERROR: %s\n", bstError.message().c_str() );
	}  	
}


// Send a message
//
//
// The optional parameters will tell the sender to attempt resending the message should it fail.  it will try
// p_iMaxSendCount times waiting p_lSendIntervalInMs ms between sends.
void  CAsyncConnectionlessSender::Send( const std::string& p_strMsg, const int p_iMaxSendCount, const long p_lSendIntervalInMs )
{
	ASSERT( this->m_bstSocket.get() );

    this->m_lSendIntervalInMs = p_lSendIntervalInMs;
    this->m_iMaxSendCount     = p_iMaxSendCount;
    this->m_iSendCount        = 0;
    this->m_strMessage        = p_strMsg;
    this->Send();	
}

// Terminate Sending
//
// Stop Sending and then close the Socket
bool CAsyncConnectionlessSender::TerminateSender( boost::system::error_code& p_bstError )
{
	return true;
	bool bResult = false;

	if( !this->StopSender( p_bstError ) )
	{
		ASSERT( false );
	} 
	else
	{
		if( !this->CloseSender( p_bstError ) )
		{
			ASSERT( false );
		}
		else
		{
			bResult = true;
		}
	}
	return bResult;
}

// Stop Sending
//
// Stop any sending that is occuring, stop the worker thread and cancel the re-send timer
bool CAsyncConnectionlessSender::StopSender( boost::system::error_code& p_bstError )
{
	 bool bResult = false;
     try
     {
		ASSERT( this->m_bstTimer.get() );

		this->m_bstTimer->cancel( p_bstError ); 

		ASSERT( this->m_bstSocket.get() );

		this->m_bstSocket->cancel();

		bResult = (!p_bstError);
     }
     catch( const std::exception& ex )
     {
		std::string strEx = ex.what();
		TRACE( "%s: EXCEPTION: %s\n", __FUNCTION__, strEx.c_str() );
		ASSERT( false );
     }
     this->m_iMaxSendCount = 0;

	 return bResult;
}

// Close the Sender Socket
// Reports back success or failure and the error should it fail.
bool  CAsyncConnectionlessSender::CloseSender( boost::system::error_code& p_bstError )
{          
    // If the socket is alreay closed, then report true.
    bool bResult = true;
	
	ASSERT( this->m_bstSocket.get() );

    if( this->m_bstSocket->is_open() )
    {        
        this->m_bstSocket->close( p_bstError );
        bResult = !p_bstError;        
    }
    return bResult;
}


// Async Send
//
// This is where the message is actually sent, asynchronously
// The message is sent and HandleSend is called to report success or failure of the send.
//
// An attempt is made to pass it on to the developer defined Callback, which is set via the constructor
void  CAsyncConnectionlessSender::Send()
{
	if( this->m_bstSocket.get() )
	{
		this->m_bstSocket->async_send_to(	boost::asio::buffer( this->m_strMessage ), *this->m_bstEndpoint,
											boost::bind( &CAsyncConnectionlessSender::HandleSend, this,
														 boost::asio::placeholders::error ) );
	}
}

// Handle Send
//
// This method is called after an async Send
// This method will report if there was an error in the send
// The callback will report back the sent (or attempted to send) message as well as the error code
// If no callback is defined, an ASSERT will occur
void  CAsyncConnectionlessSender::HandleSend( const boost::system::error_code& p_bstError )
{
     if( !p_bstError )
     {
        ++this->m_iSendCount;
        if( this->m_bstSendCallback )
        {
            this->m_bstSendCallback( this->m_strMessage, p_bstError );
        }
		else
		{
			ASSERT( false );
		}
        if( ( this->m_iSendCount < this->m_iMaxSendCount ) || 
			this->m_iMaxSendCount == CommsUtilities::Multicast::s_iINDEFINET_SEND_COUNT )
        {
            this->m_bstTimer->expires_from_now( boost::posix_time::milliseconds( this->m_lSendIntervalInMs ) );
            this->m_bstTimer->async_wait( boost::bind( &CAsyncConnectionlessSender::HandleTimeout, this, boost::asio::placeholders::error ) );
        }
     }
	 else
	 {
		if( this->m_bstSendCallback )
        {
            this->m_bstSendCallback( this->m_strMessage, p_bstError );
        }
		else
		{
			ASSERT( false );
		}
	 }
}

// Handle Timeout
// This simply handles what happens when the resend timeout occures.  If there was no error in the timeout
// then resend the message.
// If an error has occured, fire the callback with the error.
void  CAsyncConnectionlessSender::HandleTimeout( const boost::system::error_code& p_bstError )
{
    if( !p_bstError )
    {
        this->Send();
    }
	else
	{
		if( this->m_bstSendCallback )
		{
			 this->m_bstSendCallback( this->m_strMessage, p_bstError );
		}
		else
		{
			ASSERT( false );
		}
	}
}
