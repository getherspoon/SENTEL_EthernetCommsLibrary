/* 
 * File:   ConnectionlessComms.h
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class is a base template for the Connectionless Communication classes (UDP and Multicast)
 *
 * This class defines methods shared by all Connectionless Comm classes, both Sync and Async variaties
 * 
 * This class attempts to abstract the details of the communications and allows the developer
 * to instantiate a Sender/Receiver with only Ports and some basic configuration passed to the
 * constructor.  This class takes care of the ASIO IO Service and IO Thread management.
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#ifndef CCONNECTIONLESS_COMMS_H
#define	CCONNECTIONLESS_COMMS_H
#pragma once

#ifndef NO_MFC

#ifndef BOOST_THREAD_DYN_LINK
#define BOOST_THREAD_DYN_LINK
#endif

#else

#ifndef BOOST_THREAD_USE_LIB
#define BOOST_THREAD_USE_LIB
#endif

#endif

#ifdef __WIN32__
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI2_
#include <winsock2.h>
#endif

#include "EthernetCommsAPI.h"

#include "CommsUtilities.h"

#include <boost/thread.hpp>

template <class COMMS_CLASS>
class ETHERNETCOMMS_API CConnectionlessComms
{
public:
	// Initialize Comms
	//
	// This method initializes any communications that need initializing
	// For the COMMS_CLASS
	// For Sychronous Connectionless Comms, this involves initializing the socket and reporting any errors
	//
	// For Asyncronous Connectionless Comss, this involves initializing the socket and starting the receiver, as well
	// as reporting any errors.
	//
	// If no errors are reported, the IO Threads are started to make sure that the ASIO IO occurs.
	virtual bool	InitializeComms( boost::system::error_code& p_bstError )
	{		
		bool bResult = false;

		ASSERT( this->m_pComms.get() );
		
		bResult = this->m_pComms->InitializeComms( p_bstError );

		if( bResult )
		{
			if( this->m_pComms->CanRecv() || this->m_pComms->CanSend() )
			{
				this->m_bstThreadRecv.reset( new boost::thread( boost::bind( &boost::asio::io_service::run, &m_bstIOServiceRecv ) ) );
			}
			if( this->m_pComms->CanSend() )
			{
				this->m_bstThreadSend.reset( new boost::thread( boost::bind( &boost::asio::io_service::run, &m_bstIOServiceSend ) ) );
			}
		}
		
		return bResult;
	}

	// Stop Comms
	//
	// This method stops all communications.
	// The ASIO IO_Services are stopped, the IO Threads are stopped and lastely the Comms are stopped
	// This means the sender and receiver are both closed.
	virtual bool	TerminateComms( boost::system::error_code& p_bstError )
	{
		bool bResult = false;	

		if( !this->m_bstIOServiceRecv.stopped() )
		{
			this->m_bstIOServiceRecv.stop();
		}
		if( !this->m_bstIOServiceSend.stopped() )
		{
			this->m_bstIOServiceSend.stop();
		}

		if( this->m_bstThreadRecv.get() && this->m_bstThreadRecv->joinable() )
		{
			this->m_bstThreadRecv->join();
			this->m_bstThreadRecv.reset();
		}
		if( this->m_bstThreadSend.get() && this->m_bstThreadSend->joinable() )
		{
			this->m_bstThreadSend->join();
			this->m_bstThreadSend.reset();
		}

		ASSERT( this->m_pComms.get() );

		bResult = this->m_pComms->TerminateCommsImpl( p_bstError );		

		return bResult;
	}
	
protected:
	// Protected Methods
	//
	// Constructor
	// This ctor is protected so that this class cannot be instantiated, only derived from.
	// The user will define the comms type (UDP, Multicast, etc) and the Comms System Type (Sync, Async)
	CConnectionlessComms<COMMS_CLASS>(	const CommsUtilities::eConnectionlessCommsType	p_eConnectionlessCommsType,
							const CommsUtilities::eCommsSystemType			p_eCommsSystemType )
	:
	m_eConnectionlessCommsType( p_eConnectionlessCommsType ),
	m_eCommsSystemType( p_eCommsSystemType ) {}

	// Destructor 
	// 
	// Stops the Comms and reports any errors.
	virtual ~CConnectionlessComms() 
	{		
		ASSERT( this->m_pComms.get() );
		boost::system::error_code bstError;
		if( !this->TerminateComms( bstError ) )
		{
			ASSERT( false );
			TRACE( "ERROR: %s\n", bstError.message().c_str() );
		} 

		this->m_pComms.reset();
	}

	// Reset the Comms Object
	virtual void ResetComms( COMMS_CLASS* p_pComms )
	{
		this->m_pComms.reset( p_pComms );
	}

	// Get the Comms Object
	virtual COMMS_CLASS* GetComms() const
	{
		return this->m_pComms.get();
	}

	// Get the Recv IO Service
	virtual boost::asio::io_service& GetIOServiceRecv()
	{
		return m_bstIOServiceRecv;
	}

	// Get the Send IO Service
	virtual boost::asio::io_service& GetIOServiceSend()
	{
		return m_bstIOServiceSend;
	}

private:
	// Protected Members Variables
	//
	// Pointer to our Comms class
	std::auto_ptr<COMMS_CLASS>										m_pComms;

	CommsUtilities::eCommsSystemType								m_eCommsSystemType;

	CommsUtilities::eConnectionlessCommsType						m_eConnectionlessCommsType;

	boost::asio::io_service											m_bstIOServiceRecv;
	boost::asio::io_service											m_bstIOServiceSend;

	std::auto_ptr<boost::thread>									m_bstThreadRecv;
	std::auto_ptr<boost::thread>									m_bstThreadSend;
};
#endif // CCONNECTIONLESS_COMMS_H
