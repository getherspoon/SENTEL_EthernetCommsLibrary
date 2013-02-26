/* 
 * File:   AsyncConnectionlessReceiver.h
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class declares and defines all common code used by Asynchronous Connectionless Sender classes
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#ifndef __ASYNC_CONNECTIONLESS_SENDER_H__
#define __ASYNC_CONNECTIONLESS_SENDER_H__

#include "CommsUtilities.h"

#include <boost/system/system_error.hpp>

class ETHERNETCOMMS_API CAsyncConnectionlessSender
{
public:
	// Destructor
	virtual ~CAsyncConnectionlessSender();

	// Pure Virtual Initialization Method
	virtual bool InitializeSender( boost::system::error_code& p_bstError ) = 0;

	// Sends a message
	virtual void  Send( const std::string& p_strMsg, const int p_iMaxSendCount = CommsUtilities::UDP::s_iDEFAULT_MAX_SEND_COUNT, const long p_lSendIntervalInMs = CommsUtilities::Multicast::s_lDEFAULT_SEND_INTERVAL_IN_MS );
    
	// Stops and closes the async Sender
	virtual bool  TerminateSender( boost::system::error_code& p_bstError );

	// Stops the async Sending
	virtual bool  StopSender( boost::system::error_code& p_bstError );
    
	// Closes the Sender
	virtual bool  CloseSender( boost::system::error_code& p_bstError );

protected:

	// Constructor
	CAsyncConnectionlessSender( boost::asio::io_service& p_bstIOService, 
								const Callback_ConstRefString_Error p_bstSendCallback,
								const long p_lSendIntervalInMs );

	// Actualy performs the async send
    virtual void  Send();

	// Handles the result of the Async Send
	virtual void  HandleSend( const boost::system::error_code& p_bstError );

	// Handles the Timeout for the Async repeat sending
    virtual void  HandleTimeout( const boost::system::error_code& p_bstError );

	// Reset the Socket
	virtual void ResetSocket( boost::asio::ip::udp::socket* p_bstSocket )
	{
		this->m_bstSocket.reset( p_bstSocket );
	}

	// Get the Socket
	virtual boost::asio::ip::udp::socket* GetSocket() const
	{
		return this->m_bstSocket.get();
	}

	// Reset the IO Service Worker
	virtual void ResetWork( boost::asio::io_service::work* p_bstWork )
	{
		this->m_bstWork.reset( p_bstWork );
	}

	// Get the IO Service Worker
	virtual boost::asio::io_service::work* GetWork() const
	{
		return this->m_bstWork.get();
	}

	// Reset the Sender Endpoint
	virtual void ResetEndpoint( boost::asio::ip::udp::endpoint* p_bstEndpoint )
	{
		this->m_bstEndpoint.reset( p_bstEndpoint );
	}

	// Get the Sender Endpoint
	virtual boost::asio::ip::udp::endpoint* GetEndpoint() const
	{
		return this->m_bstEndpoint.get();
	}
    	
private:
	// The endpoint to which messages will be sent
	std::auto_ptr<boost::asio::ip::udp::endpoint>     m_bstEndpoint;
	// Out Socket
	std::auto_ptr<boost::asio::ip::udp::socket>       m_bstSocket;
	// A timer used to resend as needed
	std::auto_ptr<boost::asio::deadline_timer>        m_bstTimer;
	// ASIO Worker object, keeps the IO Service alive even if no IO is occuring
	std::auto_ptr<boost::asio::io_service::work>      m_bstWork;
	// The message to be sent
	std::string                                       m_strMessage;
	// Count of the number of sends which have occured
	int                                               m_iSendCount;
	// The max number of times to attempt a send
	int                                               m_iMaxSendCount;
	// Time between send attempts to wait
	long                                              m_lSendIntervalInMs;    
	// Callback fired when the Send is completed or has an error
	Callback_ConstRefString_Error                     m_bstSendCallback;
};

#endif // __ASYNC_CONNECTIONLESS_RECEIVER_H__