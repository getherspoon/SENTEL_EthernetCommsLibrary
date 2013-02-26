/* 
 * File:   CAsyncMulticastReceiver.cpp
 * Author: Josh
 *
 * Created on September 9, 2011, 9:01 AM
 *
 * This class is controls the Asycnronous Multicast Receiver tool.
 *
 * This class handles all of the boost ASIO level needs to create, control, and run 
 * an Async Multicast Receiver
 *
 * Being Async, the receive is actually handled via a callback, which must be defined on construction.
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
#include "AsyncMulticastReceiver.h"

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
// This constructor will create an instance of an Asnyc Multicast Receiver
// A Recv Callback is required
// If Loopback is Enabled, then the receiver will listen for local messages as well.
//	This is typically only advised if there is no sender or if the sender is in a different Group (IP/Port)
CAsyncMulticastReceiver::CAsyncMulticastReceiver( boost::asio::io_service& p_bstIOService, 
                                                  const std::string& p_strListenAddress, 
                                                  const std::string& p_strMulticastAddress, 
                                                  const unsigned short p_usMulticastPort,
                                                  const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstRecvCallback,
												  const bool p_bLoopbackEnabled ) 
:
CAsyncConnectionlessReceiver( p_bstIOService, p_bstRecvCallback ),
m_bLoopbackEnabled( p_bLoopbackEnabled ),
m_usMulticastPort( p_usMulticastPort ),
m_strMulticastAddress( p_strMulticastAddress ),
m_strListenAddress( p_strListenAddress )
{
	srand( (unsigned int)time(NULL) );
    int x = rand();       
}

// Destructor
// 
// This will stop the Receiver and destroy the instanced
CAsyncMulticastReceiver::~CAsyncMulticastReceiver() 
{
}

// Initialize and Start the Recv'r
//
// This method initializes and starts the Receiver.
// This simply means that the socket will be setup to listen to and join the provide Multicast Group
// Socket options are set (such as loopback), and the socket is bound to the listening port
// Then, it fires "ReceiveFrom()", which will actually start the Async Listener.
bool CAsyncMulticastReceiver::InitializeReceiver( boost::system::error_code& p_bstError )
{
     bool bResult = false;
     try
     {
		
		 boost::asio::ip::address bstListenAddress = boost::asio::ip::address::from_string( this->m_strListenAddress.c_str(), p_bstError );
		if( p_bstError )
		{
			ASSERT( false );			
		}
		else
		{
			boost::asio::ip::address bstMulticastAddress = boost::asio::ip::address::from_string( this->m_strMulticastAddress.c_str(), p_bstError );	
			if( p_bstError )
			{
				ASSERT( false );				
			}
			else
			{	    
				if( !this->GetSocket() )
				{
					ASSERT( this->GetWork() );
					this->ResetSocket( new boost::asio::ip::udp::socket( this->GetWork()->get_io_service() ) );
				}
				// Create the socket so that multiple may be bound to the same address.
				boost::asio::ip::udp::endpoint bstListenEndpoint( bstListenAddress, this->m_usMulticastPort );
				if( !this->GetSocket()->is_open() )
				{
					this->GetSocket()->open( bstListenEndpoint.protocol(), p_bstError );
				}
				if( !p_bstError )
				{
					this->GetSocket()->set_option( boost::asio::ip::udp::socket::reuse_address( true ), p_bstError );
					if( !p_bstError )
					{
						this->GetSocket()->set_option( boost::asio::ip::multicast::enable_loopback( m_bLoopbackEnabled ), p_bstError );
						if( !p_bstError )
						{
							this->GetSocket()->bind( bstListenEndpoint, p_bstError );

							if( !p_bstError )
							{
								// Join the multicast group.
								this->GetSocket()->set_option( boost::asio::ip::multicast::join_group( bstMulticastAddress ), p_bstError );
								if( !p_bstError )
								{
									this->ReceiveFrom();
									bResult = true;
								}
							}
						}	
					}
				}
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
