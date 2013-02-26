/* 
 * File:   ConnectionlessCommsImpl.h
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class is a base template for the Connectionless Communication Implementation classes (UDP and Multicast)
 *
 * This class defines methods shared by all Connectionless Comm Implementation classes, both Sync and Async variaties
 * 
 * This class attempts to abstract the implemenation details of the communications and allows the developer
 * to instantiate a Sender/Receiver with only Ports and some basic configuration passed to the
 * constructor.  This class takes care of the common resources such as methods and members
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#ifndef CCONNECTIONLESS_COMMS_IMPL_H
#define	CCONNECTIONLESS_COMMS_IMPL_H
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

template <class RECEIVER_CLASS, class SENDER_CLASS>
class ETHERNETCOMMS_API CConnectionlessCommsImpl
{
public:
	// Initialize Comms
	//
	// This method initializes any communications that need initializing
	// For the RECEIVER_CLASS
	// For Sychronous Connectionless Comms, this involves initializing the socket and reporting any errors
	//
	// For Asyncronous Connectionless Comss, this involves initializing the socket and starting the receiver, as well
	// as reporting any errors.	
	virtual bool	InitializeComms( boost::system::error_code& p_bstError )
	{
		bool bResult = true;
		try
		{		 
			if( this->m_pRecv.get() )
			{
				bResult = this->m_pRecv->InitializeReceiver( p_bstError );
			}
			if( bResult && this->m_pSend.get() )
			{
				bResult = this->m_pSend->InitializeSender( p_bstError );
			}
			else if( !this->m_pRecv.get() && this->m_pSend.get() )
			{
				bResult = true;
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

	// Stop Comms
	//
	// This method stops all communications and closes all sockets
	//
	// This means the sender and receiver are both closed, and any in progress operations are cancelled
	virtual bool	TerminateCommsImpl( boost::system::error_code& p_bstError )
	{		
		bool bResult = false;
		try
		{
			boost::system::error_code bstErrorSend, bstErrorRecv;
			if( this->m_pSend.get() )
			{
				this->m_pSend->TerminateSender( bstErrorSend );
			}
			if( this->m_pRecv.get() )
			{
				this->m_pRecv->TerminateReceiver( bstErrorRecv );
			}                  
			bResult = !bstErrorSend && !bstErrorRecv;
			if( !bResult )
			{
				if( bstErrorRecv )
				{
					p_bstError = bstErrorRecv;
				}
				else if( bstErrorSend )
				{
					p_bstError = bstErrorSend;
				}
			}
		}
		catch( std::exception& ex )
		{			
			std::string strEx = ex.what();
			TRACE( "%s: EXCEPTION: %s\n", __FUNCTION__, strEx.c_str() );
			ASSERT( false );
		}
		return bResult;
	}

	// Determines if the Capability to Receive is available
	virtual bool CanRecv() const	
	{	
		return ( m_pRecv.get() != NULL );	
	}

	// Determines if the Capability to Send is available
	virtual bool CanSend() const	
	{	
		return ( m_pSend.get() != NULL );	
	}	
	
protected:
	// Protected Methods
	//
	// Constructor
	// This ctor is protected so that this class cannot be instantiated, only derived from.
	// The user will define the comms type (UDP, Multicast, etc) and the Comms System Type (Sync, Async)
	CConnectionlessCommsImpl(	const CommsUtilities::eConnectionlessCommsType	p_eConnectionlessCommsType,
								const CommsUtilities::eCommsSystemType			p_eCommsSystemType )
	:
	m_eConnectionlessCommsType( p_eConnectionlessCommsType ),
	m_eCommsSystemType( p_eCommsSystemType ) {}	

	// Destructor 
	// 
	// Stops the Comms and reports any errors.
	virtual ~CConnectionlessCommsImpl() 
	{		
		boost::system::error_code bstError;
		if( !this->TerminateCommsImpl( bstError ) )
		{
			ASSERT( false );
			TRACE( "ERROR: %s\n", bstError.message().c_str() );
		}  

		this->m_pRecv.reset();
		this->m_pSend.reset();
	}

	// Reset the Recv Object
	virtual void ResetRecv( RECEIVER_CLASS* p_pRecv )
	{
		this->m_pRecv.reset( p_pRecv );
	}

	// Rest the Send Object
	virtual void ResetSend( SENDER_CLASS* p_pSend )
	{
		this->m_pSend.reset( p_pSend );
	}

	// Get the Recv Object
	virtual RECEIVER_CLASS* GetRecv() const
	{
		return this->m_pRecv.get();
	}

	// Get the Send Object
	virtual SENDER_CLASS* GetSend() const
	{
		return this->m_pSend.get();
	}

private:
	// Protected Members Variables
	//
	std::auto_ptr<RECEIVER_CLASS>				m_pRecv;
	std::auto_ptr<SENDER_CLASS>					m_pSend;

	CommsUtilities::eCommsSystemType			m_eCommsSystemType;
	CommsUtilities::eConnectionlessCommsType	m_eConnectionlessCommsType;

};
#endif // CCONNECTIONLESS_COMMS_IMPL_H
