/* 
 * File:   CAsyncMulticastSender.cpp
 * Author: Josh
 *
 * Created on September 9, 2011, 9:01 AM
 *
 * This class is controls the Asycnronous Multicast Sender tool.
 *
 * This class handles all of the boost ASIO level needs to create, control, and run 
 * an Async Multicast Sender
 *
 * Being Async, the send is actually performed in a non-blocking fashion and the resule is 
 * handled via a callback, which must be defined on construction.
 *
 * It is advised that developers use the CAsyncMulticastComms class rather than attempt to use this lower level
 * class.  If there is a need to use this low level class, perhaps the reqired functionality can be incorporated
 * into CAsyncMulticastComms, thus preparing that class for furture needs.
 *
 * Unless otherwise noted, all comments are in this CPP file with only basic comments in the Header file.
 *
 * For usage, please see ReadMe-Multicast.txt
 *
 */
#include "stdafx.h"
#include "AsyncMulticastSender.h"

#include <cstdlib>
#include <ctime> 

#include <boost/random.hpp>

#ifndef NO_MFC

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif

// Constructor
//
// This instantiates an Async Multicast Sender Object
// A sender callback is required
CAsyncMulticastSender::CAsyncMulticastSender(  boost::asio::io_service& p_bstIOService,
                                               const std::string& p_strMulticastAddress, 
                                               const unsigned short p_usMulticastPort,
                                               const Callback_ConstRefString_Error p_bstSendCallback )
:
CAsyncConnectionlessSender( p_bstIOService, p_bstSendCallback, CommsUtilities::Multicast::s_lDEFAULT_SEND_INTERVAL_IN_MS ),
m_strMulticastAddress( p_strMulticastAddress ),
m_usMulticastPort( p_usMulticastPort )
{   
    srand( (unsigned int)time( NULL ) );
    int x = rand();
}

// Destructor
// 
// Stop Sending and Close the Sender
CAsyncMulticastSender::~CAsyncMulticastSender() 
{           
}

// Initialization Method
// Creates our Endpoint and Socket, also opens the socket
bool CAsyncMulticastSender::InitializeSender( boost::system::error_code& p_bstError )
{
	bool bResult = false;

	ASSERT( this->GetWork() );

	boost::asio::ip::address bstMulticastAddress = boost::asio::ip::address::from_string( this->m_strMulticastAddress, p_bstError );

	if( !p_bstError )
	{
		this->ResetEndpoint( new boost::asio::ip::udp::endpoint( bstMulticastAddress , m_usMulticastPort ) );
		ASSERT( this->GetEndpoint() );
		this->ResetSocket( new boost::asio::ip::udp::socket( this->GetWork()->get_io_service(), this->GetEndpoint()->protocol() ) );
    
		bResult = true;
	}

	return bResult;
}
