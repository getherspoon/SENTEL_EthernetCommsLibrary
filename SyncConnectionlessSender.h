/* 
 * File:   AsyncConnectionlessReceiver.h
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class declares and defines all common code used by Synchronous Connectionless Sender classes
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#ifndef __SYNC_CONNECTIONLESS_SENDER_H__
#define __SYNC_CONNECTIONLESS_SENDER_H__

#include "CommsUtilities.h"

#include <boost/system/system_error.hpp>

class ETHERNETCOMMS_API CSyncConnectionlessSender
{
public:
	// Destructor
	virtual ~CSyncConnectionlessSender();

	// Pure Virtual Initialization Method
	virtual bool InitializeSender( boost::system::error_code& p_bstError ) = 0;

	// Sends a message
	virtual bool Send( const std::string& p_strMsg, boost::system::error_code& p_bstError, const int p_iMaxSendCount, const long p_lSendIntervalInMs );
    
	// Stops and closes the async Sender
	virtual bool  TerminateSender( boost::system::error_code& p_bstError );

protected:
	// Constructor
	CSyncConnectionlessSender( boost::asio::io_service& p_bstIOService );
	
	// Get the IO Service
	boost::asio::io_service&	GetIOService() const	
	{ 
		return *m_pBstIOService;	
	}

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
	// Endpoint which defines where messages are sent
	std::auto_ptr<boost::asio::ip::udp::endpoint>   m_bstEndpoint;
	// Our Socket
    std::auto_ptr<boost::asio::ip::udp::socket>     m_bstSocket;     
	// The message to be sent out
    std::string                                     m_strMessage;         
	
private:
	// Pointer to the IO Service being used, in case it needs to be referenced
	boost::asio::io_service*						m_pBstIOService;
};

#endif // __ASYNC_CONNECTIONLESS_RECEIVER_H__