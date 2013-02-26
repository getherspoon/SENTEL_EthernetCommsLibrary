/* 
 * File:   CSyncMulticastSender.cpp
 * Author: jarmentrout
 *
 * Created on August 26, 2010, 2:08 PM
 *
 * This class is controls the Sycnronous Multicast Sender tool.
 *
 * This class handles all of the boost ASIO level needs to create, control, and run 
 * an Sync Multicast Sender
 *
 * It is advised that developers use the CSyncMulticastComms class rather than attempt to use this lower level
 * class.  If there is a need to use this low level class, perhaps the reqired functionality can be incorporated
 * into CSyncMulticastComms, thus preparing that class for furture needs.
 *
 * Unless otherwise noted, all comments are in this CPP file with only basic comments in the Header file.
 *
 * For usage, please see ReadMe-Multicast.txt
 *
 */
#include "stdafx.h"
#include "SyncMulticastSender.h"

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
#include <boost/date_time/posix_time/posix_time.hpp>

// Constructor
//
// This instantiates an Sync Multicast Sender Object
CSyncMulticastSender::CSyncMulticastSender( boost::asio::io_service& p_bstIOService,
											const std::string& p_strMulticastAddress,
											const unsigned short p_usMulticastPort ) 
:
CSyncConnectionlessSender( p_bstIOService ),
m_strMulticastAddress( p_strMulticastAddress ),
m_usMulticastPort( p_usMulticastPort )
{    
    srand( (unsigned int)time( NULL ) );
    int x = rand();
}

// Destructor
// 
// Close the Sender
CSyncMulticastSender::~CSyncMulticastSender( )
{	
}
// Initialization Method
// Creates our Endpoint and Socket, also opens the socket
bool CSyncMulticastSender::InitializeSender( boost::system::error_code& p_bstError )
{
	bool bResult = false;

	boost::asio::ip::address bstMulticastAddress = boost::asio::ip::address::from_string( this->m_strMulticastAddress, p_bstError );

	if( !p_bstError )
	{
		this->ResetEndpoint( new boost::asio::ip::udp::endpoint( bstMulticastAddress , this->m_usMulticastPort ) );
		ASSERT( this->GetEndpoint() );
		this->ResetSocket( new boost::asio::ip::udp::socket( this->GetIOService(), this->GetEndpoint()->protocol() ) );
		bResult = true;
	}

	return bResult;
}


// Sender 
// this will send the message by calling the CSyncConnectionlessSender::Send() method, but with some custom default values
bool CSyncMulticastSender::Send(	const std::string& p_strMsg, boost::system::error_code& p_bstError, 
									const int p_iMaxSendCount, const long p_lSendIntervalInMs )
{
	return CSyncConnectionlessSender::Send( p_strMsg, p_bstError, p_iMaxSendCount, p_lSendIntervalInMs );
}
