/* 
 * File:   AsyncConnectionlessCommsImpl.h
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class is a base template for the Asynchronous Connectionless Communication Implementation classes (UDP and Multicast)
 *
 * This class defines methods shared by all Asynchronous Connectionless Comm Implementation classes
 * 
 * This class attempts to abstract the implemenation details of the asynchronous communications and allows the developer
 * to instantiate a Sender/Receiver with only Ports and some basic configuration passed to the
 * constructor.  This class derives from CConnectionelssComssImpl which takes care of the common resources (methods and members)
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#ifndef __ASYNC_CONNECTIONLESS_COMMS_IMPL_H__
#define __ASYNC_CONNECTIONLESS_COMMS_IMPL_H__

#include "CommsUtilities.h"
#include "ConnectionlessCommsImpl.h"

template <class RECEIVER_CLASS, class SENDER_CLASS>
class CAsyncConnectionlessCommsImpl : public CConnectionlessCommsImpl<RECEIVER_CLASS, SENDER_CLASS>
{
public:
	// Send a Message
	//
	// If a sender is instantiated, this will signal the Async Sender to try and send the message.
	// Returns true if a sender is instantiated.  Since the send will occur asychronously, there is no way at this point
	// to determine if it was a success or a failure.  Thus, bResult will always be True if the send exists and p_bstError will
	// never be set by this method, but it needs to be here to satisfy the IMulticastComms requirement.
	// The Send Callback will provide the actual result of the send. If no callback for the sender is defined, there 
	// will be no way to tell if the send failed and why.
	virtual bool Send( const std::string& p_strMsg, const int p_iMaxSendCount = CommsUtilities::Multicast::s_iDEFAULT_MAX_SEND_COUNT, const long p_lSendIntervalInMs = CommsUtilities::Multicast::s_lDEFAULT_SEND_INTERVAL_IN_MS )
	{
		bool bResult = false;
		if( this->GetSend() )
		{
			this->GetSend()->Send( p_strMsg, p_iMaxSendCount, p_lSendIntervalInMs );
			bResult = true;
		}
		return bResult;
	}

protected:
	// Constructor
	// This ctor is protected so that this class cannot be instantiated, only derived from.
	// The user will define the comms type (UDP, Multicast, etc)
	CAsyncConnectionlessCommsImpl( const CommsUtilities::eConnectionlessCommsType	p_eConnectionlessCommsType ) 
	: 
	CConnectionlessCommsImpl<RECEIVER_CLASS, SENDER_CLASS>( p_eConnectionlessCommsType, CommsUtilities::ASYNC ) {}

	// Destructor
	virtual ~CAsyncConnectionlessCommsImpl()	{}
};

#endif // __ASYNC_CONNECTIONLESS_COMMS_IMPL_H__
