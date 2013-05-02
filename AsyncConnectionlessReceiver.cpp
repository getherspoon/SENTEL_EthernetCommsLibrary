/* 
 * File:   AsyncConnectionlessReceiver.cpp
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class declares and defines all common code used by Asynchronous Connectionless Receiver classes
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#include "stdafx.h"
#include "AsyncConnectionlessReceiver.h"

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
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include <boost/thread.hpp>

// Constructor
// A Recv Callback is Required
CAsyncConnectionlessReceiver::CAsyncConnectionlessReceiver( boost::asio::io_service& p_bstIOService,
															const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstRecvCallback )
:
m_bstRecvCallback( p_bstRecvCallback )
{		    
    this->m_bstWork.reset( new boost::asio::io_service::work( p_bstIOService ) );            	
}

// Destructor
CAsyncConnectionlessReceiver::~CAsyncConnectionlessReceiver()
{	
    boost::system::error_code bstError;
	if( !this->TerminateReceiver( bstError ) )
	{
		TRACE("%s: ERROR: %s\n", __FUNCTION__, bstError.message().c_str() );
	}  
}

// Stop the Recv'r
//
// This method stops the Receiver
// This will return true unless an exception occurs
//
// After calling this, the Receiver is no longer valid until InitialzeReceiver() is called
bool CAsyncConnectionlessReceiver::StopReceiver( boost::system::error_code& p_bstError )
{
	bool bResult = false;
	try
	{
		ASSERT( this->m_bstSocket.get() );
		this->m_bstSocket->cancel( p_bstError );
		bResult = (!p_bstError);
	}
	catch( std::exception& ex )
	{
		std::string strEx = ex.what();
		TRACE( "%s: EXCEPTION: %s\n", __FUNCTION__, strEx.c_str() );
		ASSERT( false );
		bResult = false;
	}
	return bResult;
}

// Stop and close the Recv'r
//
// This method stops the Receiver by killing the worker thread and then closing the socket
// This will return true unless an exception occurs
//
// After this is called, one must recall "InitializeReceiver() again to restart the receiver
bool CAsyncConnectionlessReceiver::TerminateReceiver( boost::system::error_code& p_bstError )
{
     bool bResult = false;
     try
     {		
		if( this->m_bstSocket.get() )
		{
			if( this->m_bstSocket->is_open() )
			{
				this->m_bstSocket->close( p_bstError );          
				bResult = ( !p_bstError );
			}
			else
			{
				bResult = true;
			}
		}
     }
     catch( std::exception& ex )
     {
		std::string strEx = ex.what();
		TRACE( "%s: EXCEPTION: %s\n", __FUNCTION__, strEx.c_str() );
		ASSERT( false );
        bResult = false;
     }
     return bResult;
}

// Fire the Recv From call to the Socket
// 
// This is the method which actually calls async_recv_from, which starts the listener listening.
// Here we also define the method to call once a message is received.  In this cass it is always
// the HandleReceiveFrom method.
void CAsyncConnectionlessReceiver::ReceiveFrom()
{
	ASSERT( this->m_bstSocket.get() );
    this->m_bstSocket->async_receive_from( boost::asio::buffer( this->m_strRecvBuffer, this->m_c_iMaxRecvSize ), 
                                          this->m_bstSenderEndpoint,
                                          boost::bind( &CAsyncConnectionlessReceiver::HandleReceiveFrom, this,
                                                              boost::asio::placeholders::error,
                                                              boost::asio::placeholders::bytes_transferred));
}

// Handle a recv'd message
//
// This method is called as a callback once data is received by the listener
// We are provided an error code, if any and the size of the received data
// The received data is stored in this->m_strRecvBuffer and can be extracted since we know the 
// size (p_sizeBytesRecvd)
//
// Should an error occur, we won't bother to process the message string, but we will determine the sender and 
// fire the sender IP, port, and error code via the callback.
// 
void CAsyncConnectionlessReceiver::HandleReceiveFrom( const boost::system::error_code& p_bstError, std::size_t p_sizeBytesRecvd )
{
    if( !p_bstError )
    {

		ASSERT( this->m_bstRecvCallback );
        
		if( this->m_bstRecvCallback )
        {
			std::string strBytesRecv( boost::lexical_cast<std::string>( p_sizeBytesRecvd ) );            
            std::string strRecvMessage( this->m_strRecvBuffer, p_sizeBytesRecvd );
			std::string strRecvSize( boost::lexical_cast<std::string>( strRecvMessage.size() ) );                        
            std::string strSenderAddress( this->m_bstSenderEndpoint.address().to_string() );
			unsigned short usSenderPort = this->m_bstSenderEndpoint.port();
            // Fire the callback as a thread
            boost::thread bstCallbackThread( this->m_bstRecvCallback, strRecvMessage, strSenderAddress, usSenderPort, p_bstError );            
        }
        else
        {
			ASSERT( false );            
        }        
        this->ReceiveFrom();
    }
    else
    {
		if( this->m_bstRecvCallback )
        {
			// If we have an error, don't worry about the message and fire off the Error in the callback
			std::string strSenderAddress( this->m_bstSenderEndpoint.address().to_string() );
			unsigned short usSenderPort = this->m_bstSenderEndpoint.port();
			boost::thread bstCallbackThread( this->m_bstRecvCallback, "", strSenderAddress, usSenderPort, p_bstError );            
		}
		else
		{
			ASSERT( false );
		}
    }
}
