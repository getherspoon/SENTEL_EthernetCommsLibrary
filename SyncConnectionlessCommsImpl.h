/* 
 * File:   SyncConnectionlessCommsImpl.h
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class is a base template for the Synchronous Connectionless Communication Implementation classes (UDP and Multicast)
 *
 * This class defines methods shared by all Synchronous Connectionless Comm Implementation classes
 * 
 * This class attempts to abstract the implemenation details of the synchronous communications and allows the developer
 * to instantiate a Sender/Receiver with only Ports and some basic configuration passed to the
 * constructor.  This class derives from CConnectionelssComssImpl which takes care of the shared methods and members
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#ifndef __SYNC_CONNECTIONLESS_COMMS_IMPL_H__
#define __SYNC_CONNECTIONLESS_COMMS_IMPL_H__

#include "CommsUtilities.h"
#include "ConnectionlessCommsImpl.h"

template <class RECEIVER_CLASS, class SENDER_CLASS>
class CSyncConnectionlessCommsImpl : public CConnectionlessCommsImpl<RECEIVER_CLASS, SENDER_CLASS>
{
public:
	// Send a Message
	// 
	// This will send a message, synchronously
	// Should an error occur, the method attempt to send the message p_iMaxSendCount times, waiting p_lSendIntervalInMs ms between sends
	// If after all attempts are made, the send still fails, it will be reported back.
	// This is a blocking call.
	virtual bool Send( const std::string& p_strMsg, boost::system::error_code& p_bstError, const int p_iMaxSendCount = CommsUtilities::Multicast::s_iDEFAULT_MAX_SEND_COUNT, const long p_lSendIntervalInMs = CommsUtilities::Multicast::s_lDEFAULT_SEND_INTERVAL_IN_MS )
	{
		bool bResult = false;
		if( this->GetSend() )
		{
			bResult = this->GetSend()->Send( p_strMsg, p_bstError, p_iMaxSendCount, p_lSendIntervalInMs );
		}
		return bResult;
	}

	// Sync Recv 
	// 
	// This will Recv a message, synchronously
	// Should an error occur, it will be reported back.
	// This is a blocking call.
	// On Success, the method will fill in the values for Message, Sender Address and Sender Port
	virtual bool Receive( std::string& p_strMsg, std::string& p_strSenderAddress, unsigned short& p_usPort, boost::system::error_code& p_bstError, const long p_lTimeoutInMs )
	{
		bool bResult = false;
		if( this->GetRecv() )
		{
			bResult = this->GetRecv()->Receive( p_lTimeoutInMs, p_strMsg, p_strSenderAddress, p_usPort, p_bstError );
		}
		return bResult;
	}

protected:
	// Constructor
	// This ctor is protected so that this class cannot be instantiated, only derived from.
	// The user will define the comms type (UDP, Multicast, etc)
	CSyncConnectionlessCommsImpl( const CommsUtilities::eConnectionlessCommsType	p_eConnectionlessCommsType ) 
	: 
	CConnectionlessCommsImpl<RECEIVER_CLASS, SENDER_CLASS>( p_eConnectionlessCommsType, CommsUtilities::SYNC ) {}

	// Destructor
	virtual ~CSyncConnectionlessCommsImpl()	{}	
};

#endif // __SYNC_CONNECTIONLESS_COMMS_H__
