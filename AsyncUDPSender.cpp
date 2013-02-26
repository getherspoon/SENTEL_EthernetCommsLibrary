/* 
 * File:   CAsyncUDPSender.cpp
 * Author: Josh
 *
 * Created on September 9, 2011, 9:01 AM
 *
 * This class is controls the Asycnronous UDP Sender tool.
 *
 * This class handles all of the boost ASIO level needs to create, control, and run 
 * an Async UDP Sender
 *
 * Being Async, the send is actually performed in a non-blocking fashion and the resule is 
 * handled via a callback, which must be defined on construction.
 *
 * It is advised that developers use the CUDPComms class rather than attempt to use this lower level
 * class.  If there is a need to use this low level class, perhaps the reqired functionality can be incorporated
 * into CUDPComms, thus preparing that class for furture needs.
 *
 * Unless otherwise noted, all comments are in this CPP file with only basic comments in the Header file.
 *
 * For usage, please see ReadMe-UDP.txt
 *
 */
#include "stdafx.h"
#include "AsyncUDPSender.h"

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
// This instantiates an Async UDP Sender Object
// A sender callback is required
CAsyncUDPSender::CAsyncUDPSender(  boost::asio::io_service& p_bstIOService,
                                   const std::string& p_strUDPAddress, 
                                   const unsigned short p_usUDPPort,
                                   const Callback_ConstRefString_Error p_bstSendCallback )
:
CAsyncConnectionlessSender( p_bstIOService, p_bstSendCallback, CommsUtilities::UDP::s_lDEFAULT_SEND_INTERVAL_IN_MS ),
m_strUDPAddress( p_strUDPAddress ),
m_usUDPPort( p_usUDPPort )
{       
    srand( (unsigned int)time( NULL ) );
    int x = rand();
}

// Destructor
// 
// Stop Sending and Close the Sender
CAsyncUDPSender::~CAsyncUDPSender() 
{               
}

// Initialization Method
// Creates our Endpoint and Socket, also opens the socket
bool CAsyncUDPSender::InitializeSender( boost::system::error_code& p_bstError )
{
	bool bResult = false;

	ASSERT( this->GetWork() );

	boost::asio::ip::address bstUDPAddress = boost::asio::ip::address::from_string( this->m_strUDPAddress, p_bstError );

	if( !p_bstError )
	{
		this->ResetEndpoint( new boost::asio::ip::udp::endpoint( bstUDPAddress , this->m_usUDPPort ) );
		ASSERT( this->GetEndpoint() );
		this->ResetSocket( new boost::asio::ip::udp::socket( this->GetWork()->get_io_service(), this->GetEndpoint()->protocol() ) );              
		bResult = true;
	}

	return bResult;
}
