/* 
 * File:   AsyncConnectionlessReceiver.cpp
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class declares and defines all common code used by Synchronous Connectionless Sender classes
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#include "stdafx.h"
#include "SyncConnectionlessSender.h"

#ifndef NO_MFC

#ifndef BOOST_THREAD_DYN_LINK
#define BOOST_THREAD_DYN_LINK
#endif

#else

#ifndef BOOST_THREAD_USE_LIB
#define BOOST_THREAD_USE_LIB
#endif

#endif

#include <boost/thread.hpp>

// Constructor
CSyncConnectionlessSender::CSyncConnectionlessSender( boost::asio::io_service& p_bstIOService )
{		        
	m_pBstIOService = &p_bstIOService;
}

// Destructor
CSyncConnectionlessSender::~CSyncConnectionlessSender()
{	
	boost::system::error_code bstError;	
	if( !this->TerminateSender( bstError ) )
	{
		ASSERT( false );
	}
}

// Send a message
//
// Sends a message, report success or failure as well as errors if any
//
// The additional parameters will tell the sender to attempt resending the message should it fail.  it will try
// p_iMaxSendCount times waiting p_lSendIntervalInMs ms between sends.
bool CSyncConnectionlessSender::Send( const std::string& p_strMsg, boost::system::error_code& p_bstError, const int p_iMaxSendCount, const long p_lSendIntervalInMs )
{
    bool bResult = false;
	int iSendCount = 0;
	while( iSendCount < p_iMaxSendCount && !bResult )
	{
		try
		{
			if( !this->m_bstSocket->is_open() )
			{
				this->m_bstSocket->open( boost::asio::ip::udp::v4(), p_bstError );
			}
			if( this->m_bstSocket->is_open() )
			{				
				this->m_bstSocket->send_to( boost::asio::buffer( p_strMsg ), 
											*this->m_bstEndpoint, 0, p_bstError );
			}
			bResult = !p_bstError;
		}
		catch (std::exception& ex)
		{
			std::string strEx = ex.what();
			TRACE( "%s: EXCEPTION: %s\n", __FUNCTION__, strEx.c_str() );
			ASSERT( false );			
		}
		++iSendCount;
		if( !bResult && iSendCount < p_iMaxSendCount )
		{
			boost::this_thread::sleep( boost::posix_time::millisec( p_lSendIntervalInMs ) );
		}		
	}
    return bResult;
}

// Close the Sender Socket
//
// Reports back success or failure and the error should it fail.
bool CSyncConnectionlessSender::TerminateSender( boost::system::error_code& p_bstError )
{
	// If the socket is alreay closed, then report true.
    bool bResult = true;
    if( this->m_bstSocket->is_open() )
    {        
        this->m_bstSocket->close( p_bstError );
        bResult = !p_bstError;        
    }
    return bResult;
}
