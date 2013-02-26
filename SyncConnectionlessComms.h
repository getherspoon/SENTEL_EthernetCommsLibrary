/* 
 * File:   SyncConnectionlessComms.h
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class is a base template for the Synchronous Connectionless Communication classes (UDP and Multicast)
 *
 * This class defines methods shared by all Synchronous Connectionless Comm classes
 * 
 * This class attempts to abstract the details of the synchronous communications and allows the developer
 * to instantiate a Sender/Receiver with only Ports and some basic configuration passed to the
 * constructor.  This class derives from CConnectionelssComss which takes care of the ASIO IO Service and IO Thread management.
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#ifndef __SYNC_CONNECTIONLESS_COMMS_H__
#define __SYNC_CONNECTIONLESS_COMMS_H__

#include "CommsUtilities.h"
#include "ConnectionlessComms.h"

template <class COMMS_CLASS>
class CSyncConnectionlessComms : public CConnectionlessComms<COMMS_CLASS>
{
public:
	// Send a Message Synchronously
	//
	// This method sends out a message via either a Synchronous Connectionless Comms Class (UDP, Multicast, etc)
	// 
	// As well as the required message parameter, developers may optionaly include a MaxSendCount (defaults to 1) and 
	// a Send Interval (defaults to 5000ms).
	// These parameters will tell the sender to attempt to send the message p_iMaxSendCount times with a wait of p_lSendIntervalInMs ms between sends.
	// Subsequent sends will only occur if the prior send fails for any reason.
	// Should the send fail it will report back 'false'.  If an error occurs, it will be relayed in the p_bstError parameter
	bool Send( const std::string& p_strMsg, boost::system::error_code& p_bstError, const int p_iMaxSendCount = CommsUtilities::Multicast::s_iDEFAULT_MAX_SEND_COUNT, const long p_lSendIntervalInMs = CommsUtilities::Multicast::s_lDEFAULT_SEND_INTERVAL_IN_MS )
	{
		ASSERT( this->GetComms() );
		
		return this->GetComms()->Send( p_strMsg, p_bstError, p_iMaxSendCount, p_lSendIntervalInMs );	
	}

	// Receive a message Synchronously
	//
	// This method tells a Synchronous Connectionless Comms class to start a blocking receive_from call. 
	// 
	// This method simply calls the Receive method defined within the associated COMMS_CLASS and returns its result boolean as
	// well as the resultant values, the received message, the address of the sender, the port on which it was sent, and the 
	// error code, if any.
	bool Receive( std::string& p_strMsg, std::string& p_strSenderAddress, unsigned short& p_usPort, boost::system::error_code& p_bstError, const long p_lTimeoutInMs )
	{
		ASSERT( this->GetComms() );
		
		return this->GetComms()->Receive( p_strMsg, p_strSenderAddress, p_usPort, p_bstError, p_lTimeoutInMs );
	}

protected:
	// Constructor
	// This ctor is protected so that this class cannot be instantiated, only derived from.
	// The user will define the comms type (UDP, Multicast, etc)
	CSyncConnectionlessComms( const CommsUtilities::eConnectionlessCommsType	p_eConnectionlessCommsType ) 
	: 
	CConnectionlessComms<COMMS_CLASS>( p_eConnectionlessCommsType, CommsUtilities::SYNC ) {}

	// Destructor
	virtual ~CSyncConnectionlessComms()	{}	
};

#endif // __SYNC_CONNECTIONLESS_COMMS_H__
