/* 
 * File:   SyncConnectionlessReceiver.cpp
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class declares and defines all common code used by Synchronous Connectionless Receiver classes
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */

#include "stdafx.h"
#include "SyncConnectionlessReceiver.h"

#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <boost/timer.hpp>

// Constructor
CSyncConnectionlessReceiver::CSyncConnectionlessReceiver( boost::asio::io_service& p_bstIOService )
{	
	m_pBstIOService = &p_bstIOService;

	this->m_bstWork.reset( new boost::asio::io_service::work( p_bstIOService ) );        
	// this->m_bstLock.initialize();
}

// Destructor
CSyncConnectionlessReceiver::~CSyncConnectionlessReceiver()
{
	// this->m_bstLock.destroy();
}

// Receive
//
// This is a blocking, Sync Receive method that will block until the socket is either closed or receives data
// If an error occurs it will be reported back to the caller
// If there is no error, the method will report success and send back the message, the IP Address of the Sender and the Port of the Sender.
//
// We use an async socket here (async_recv_from()) and then use a timer to emulate a sync receive_from operation, 
// but without the penalty of being stuck at the receive_from() call when the sender needs to terminate or be closed.
// By using an async recv call, we can interrupt the recv at will.  Using a timer, we are able to control how long we block
// even if no data is received.
bool CSyncConnectionlessReceiver::Receive(	const long p_lTimeoutInMs, 
											std::string& p_strMsg, 
											boost::asio::ip::address& p_bstSenderAddress, 
											unsigned short& p_usSenderPort, 
											boost::system::error_code& p_bstError )
{
    bool bResult = false;
    try
    {        
		boost::mutex::scoped_lock( this->m_bstLock );

		ASSERT( this->m_bstSocket.get() );

		this->m_bstTimeoutError.reset();
		this->m_bstReadError.reset();
		this->m_sizeReadSize = -1;

		// Prepare the Timer
		this->m_bstReadTimer.reset( new boost::asio::deadline_timer( this->m_bstSocket->get_io_service() ) );
		this->m_bstReadTimer->expires_from_now( boost::posix_time::milliseconds( p_lTimeoutInMs ) );
		this->m_bstReadTimer->async_wait( boost::bind( &CSyncConnectionlessReceiver::HandleTimeout, 
														this, boost::asio::placeholders::error ) );
        
		this->m_bstSocket->async_receive_from(	boost::asio::buffer( this->m_strRecvBuffer, this->m_c_iMaxRecvSize ), 
												this->m_bstSenderEndpoint,
												boost::bind( &CSyncConnectionlessReceiver::HandleReceiveFrom, this,
																  boost::asio::placeholders::error,
																  boost::asio::placeholders::bytes_transferred ) );		

		m_bReceiving = true;
		while( m_bReceiving )
		{
			if( this->m_bstReadError.get() )
			{
				this->m_bstReadTimer->cancel( p_bstError );

				if( !*m_bstReadError )
				{
					p_strMsg = std::string( this->m_strRecvBuffer, this->m_sizeReadSize );
					p_bstSenderAddress = this->m_bstSenderEndpoint.address();
					p_usSenderPort = this->m_bstSenderEndpoint.port();					
					bResult = true;
					m_bReceiving = false;
				}
				else
				{
					TRACE( "%s: ERROR: %s\n", __FUNCTION__, m_bstReadError->message().c_str() );
					p_bstError = *m_bstReadError;
					m_bReceiving = false;					
				}
				this->m_bstReadError.reset();
				this->m_bstTimeoutError.reset();
			}
			else if( this->m_bstTimeoutError.get() )
			{
				this->m_bstSocket->cancel( p_bstError );
				if( p_bstError )
				{
					TRACE( "%s: ERROR: %s\n", __FUNCTION__, p_bstError.message().c_str() );
				}				
				this->m_bstReadTimer.reset();
				this->m_bstTimeoutError.reset();
				throw boost::system::system_error(	boost::system::error_code( boost::system::errc::timed_out, 
													boost::system::get_generic_category() ) );
				m_bReceiving = false;
			}
			else
			{
				boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );
			}
		}
    }
    catch (std::exception& ex)
    {
		std::string strEx = ex.what();
		TRACE( "%s: EXCEPTION: %s\n", __FUNCTION__, strEx.c_str() );
    }

    return bResult;
}

// Receive
//
// Same as above method, but passes the Sender address as a String
bool CSyncConnectionlessReceiver::Receive(	const long p_lTimeoutInMs, 
											std::string& p_strMsg, 
											std::string& p_strSenderAddress, 
											unsigned short& p_usPort, 
											boost::system::error_code& p_bstError )
{    
	bool bResult = false;
	try
	{
		boost::asio::ip::address bstSenderAddress;
		bResult = this->Receive( p_lTimeoutInMs, p_strMsg, bstSenderAddress, p_usPort, p_bstError );
		if( bResult )
		{		
			p_strSenderAddress = bstSenderAddress.to_string( p_bstError );     
			bResult = !p_bstError;        
		}
	}
	catch( boost::system::system_error& ex )
	{
		std::string strEx = ex.what();
		TRACE( "%s: EXCEPTION: %s\n", __FUNCTION__, strEx.c_str() );
		throw ex;
	}
    return bResult;
}


// Close the Recv'r
//
// This method stops the Receiver by closing the socket
// This will return true unless an exception occurs or an error occurs
bool CSyncConnectionlessReceiver::TerminateReceiver( boost::system::error_code& p_bstError )
{
     bool bResult = false;
     try
     {	
		 this->m_bReceiving = false;

		 boost::mutex::scoped_lock( this->m_bstLock );

		 if( this->m_bstReadTimer.get() )
		 {
			this->m_bstReadTimer->cancel();
		}

		 ASSERT( this->m_bstSocket.get() );

		 this->m_bstSocket->close( p_bstError );          		 		 

		 bResult = ( !p_bstError );
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

// Handle a recv'd message
//
// This method is called as a callback once data is received by the listener
// We are provided an error code, if any and the size of the received data
// The received data is stored in this->m_strRecvBuffer and can be extracted since we know the 
// size (p_sizeBytesRecvd)
//
// This method stores the Error as well as the Size of the data read so that the Recv method 
// above can use the data to produce a result from the Recv.
void CSyncConnectionlessReceiver::HandleReceiveFrom(const boost::system::error_code& p_bstError, 
													std::size_t p_sizeBytesRecvd )
{
	m_bstReadError.reset( new boost::system::error_code( p_bstError ) );
    if( !p_bstError )
    {
		this->m_sizeReadSize = p_sizeBytesRecvd;
    }
    else
    {
        TRACE( "%s: ERROR: %s\n", __FUNCTION__, p_bstError.message().c_str() );
    }
}

// Handles when the timer fires / times out. Stores the Error, if any, for use in the
// Recv method above.
void CSyncConnectionlessReceiver::HandleTimeout( const boost::system::error_code& p_bstError )
{
	m_bstTimeoutError.reset( new boost::system::error_code( p_bstError ) );
}
