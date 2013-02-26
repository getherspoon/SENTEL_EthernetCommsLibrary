/* 
 * File:   AsyncConnectionlessComms.h
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class is a base template for the Asynchronous Connectionless Communication classes (UDP and Multicast)
 *
 * This class defines methods shared by all Asynchronous Connectionless Comm classes
 * 
 * This class attempts to abstract the details of the asynchronous communications and allows the developer
 * to instantiate a Sender/Receiver with only Ports and some basic configuration passed to the
 * constructor.  This class derives from CConnectionelssComss which takes care of the ASIO IO Service and IO Thread management.
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#ifndef __ASYNC_CONNECTIONLESS_COMMS_H__
#define __ASYNC_CONNECTIONLESS_COMMS_H__

#include "CommsUtilities.h"
#include "ConnectionlessComms.h"

template <class COMMS_CLASS>
class CAsyncConnectionlessComms : public CConnectionlessComms<COMMS_CLASS>
{
public:
	// Send a Message Asynchronously
	// 
	// This method sends out a message via either the COMMS_CLASS's sender
	// The Asynchronous COMMS_CLASS calls its Send asyncronously, thus there is no
	// way to get an immediate return. Rather, the Send Callback Method is called
	// so that the state of the send operations is returned to the caller of this Send() method
	//
	// This is a Non-Blocking call
	//
	// As well as the required message parameter, developers may optionaly include a MaxSendCount (defaults to 1) and a Send Interval (defaults to 5000ms)
	// These parameters will tell the sender to attempt to send the message p_iMaxSendCount times with a wait of p_lSendIntervalInMs ms between sends.
	// Subsequent sends will only occur if the prior send fails for any reason.
	void Send( const std::string& p_strMsg, const int p_iMaxSendCount = CommsUtilities::Multicast::s_iDEFAULT_MAX_SEND_COUNT, const long p_lSendIntervalInMs = CommsUtilities::Multicast::s_lDEFAULT_SEND_INTERVAL_IN_MS )
	{
		ASSERT( this->GetComms() );
		
		this->GetComms()->Send( p_strMsg, p_iMaxSendCount, p_lSendIntervalInMs );	
	}

protected:
	// Constructor
	// This ctor is protected so that this class cannot be instantiated, only derived from.
	// The user will define the comms type (UDP, Multicast, etc)
	CAsyncConnectionlessComms( const CommsUtilities::eConnectionlessCommsType	p_eConnectionlessCommsType ) 
	: 
	CConnectionlessComms<COMMS_CLASS>( p_eConnectionlessCommsType, CommsUtilities::ASYNC ) {}

	// Destructor
	virtual ~CAsyncConnectionlessComms() {}
};

#endif // __ASYNC_CONNECTIONLESS_COMMS_H__
