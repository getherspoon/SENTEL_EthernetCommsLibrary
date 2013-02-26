/* 
 * File:   CSyncUDPReceiver.cpp
 * Author: jarmentrout
 * 
 * Created on August 26, 2010, 1:43 PM
 *
 * This class is controls the Sycnronous UDP Receiver tool.
 *
 * This class handles all of the boost ASIO level needs to create, control, and run 
 * an Sync UDP Receiver
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
#include "SyncUDPReceiver.h"

// Constructor
//
// This constructor will create an instance of an Snyc UDP Receiver
// If Loopback is Enabled, then the receiver will listen for local messages as well.
//	This is typically only advised if there is no sender or if the sender is sending to a different Port
CSyncUDPReceiver::CSyncUDPReceiver(	boost::asio::io_service& p_bstIOService, 
									const unsigned short p_usUDPPort )
:
m_usUDPPort( p_usUDPPort ),
CSyncConnectionlessReceiver( p_bstIOService )
{
	srand( (unsigned int)time(NULL) );
    int x = rand();
}

// Destructor
// 
// This will stop the Receiver and destroy the instanced
CSyncUDPReceiver::~CSyncUDPReceiver( )
{	 
}

// Init the Recv'r
//
// This method Initializes the Receiver.
// This simply means that the socket will be setup
// Socket options are set (such as loopback), and the socket is bound to the listening port
bool CSyncUDPReceiver::InitializeReceiver( boost::system::error_code& p_bstError )
{
	 bool bResult = false;
     try
     {
		boost::asio::ip::udp::endpoint bstListenEndpoint( boost::asio::ip::udp::v4(), this->m_usUDPPort );
		
		ASSERT( this->GetWork() );

		this->ResetSocket( new boost::asio::ip::udp::socket( this->GetWork()->get_io_service(), bstListenEndpoint ) );

		ASSERT( this->GetSocket() );			

		bResult = true;
     }
     catch( std::exception& ex )
     {
		std::string strEx = ex.what();
		TRACE( "%s: EXCEPTION: %s\n", __FUNCTION__, strEx.c_str() );
		ASSERT( false );
     }	
	 return bResult;
}
