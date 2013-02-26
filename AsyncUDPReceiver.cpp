/* 
 * File:   CAsyncUDPReceiver.cpp
 * Author: Josh
 *
 * Created on September 9, 2011, 9:01 AM
 *
 * This class is controls the Asycnronous UDP Receiver tool.
 *
 * This class handles all of the boost ASIO level needs to create, control, and run 
 * an Async UDP Receiver
 *
 * Being Async, the receive is actually handled via a callback, which must be defined on construction.
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
#include "AsyncUDPReceiver.h"

#ifndef NO_MFC

#ifndef BOOST_THREAD_DYN_LINK
#define BOOST_THREAD_DYN_LINK
#endif

#else

#ifndef BOOST_THREAD_USE_LIB
#define BOOST_THREAD_USE_LIB
#endif

#endif
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <boost/random.hpp>
#include <boost/thread.hpp>

#ifndef NO_MFC 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif

// Constructor
//
// This constructor will create an instance of an Asnyc UDP Receiver
// A Recv Callback is required
CAsyncUDPReceiver::CAsyncUDPReceiver( boost::asio::io_service& p_bstIOService, 
                                      const unsigned short p_usUDPPort,
                                      const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstRecvCallback ) 
:
CAsyncConnectionlessReceiver( p_bstIOService, p_bstRecvCallback ),
m_usUDPPort( p_usUDPPort )
{
	srand( (unsigned int)time(NULL) );
    int x = rand();
}

// Destructor
// 
// This will stop the Receiver and destroy the instanced
CAsyncUDPReceiver::~CAsyncUDPReceiver() 
{	
}

// Initializes and Start the Recv'r
//
// This method initializes and starts the Receiver.
// This simply means that the socket will be setup to listen to UDP Traffic on the defined port
// Socket options are set (such as loopback), and the socket is bound to the listening port
// Then, it fires "ReceiveFrom()", which will actually start the Async Listener.
bool CAsyncUDPReceiver::InitializeReceiver( boost::system::error_code& p_bstError )
{
	bool bResult = false;
    try
    {
		if( !this->GetSocket() )
		{
			ASSERT( this->GetWork() );
			this->ResetSocket( new boost::asio::ip::udp::socket( this->GetWork()->get_io_service(), boost::asio::ip::udp::endpoint( boost::asio::ip::udp::v4(), m_usUDPPort ) ) );
		}		  
		this->ReceiveFrom();
		bResult = true;
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

