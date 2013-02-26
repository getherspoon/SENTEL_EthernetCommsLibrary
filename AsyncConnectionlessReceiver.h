/* 
 * File:   AsyncConnectionlessReceiver.h
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class declares and defines all common code used by Asynchronous Connectionless Receiver classes
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#ifndef __ASYNC_CONNECTIONLESS_RECEIVER_H__
#define __ASYNC_CONNECTIONLESS_RECEIVER_H__

#include "CommsUtilities.h"
#include "EthernetCommsAPI.h"

#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>

class ETHERNETCOMMS_API CAsyncConnectionlessReceiver
{
public:
	// Destructor
	virtual ~CAsyncConnectionlessReceiver();

	// Initialize Receiver (Pure Virtual Method would must be defined by derived class)
	// This forces classes to override the InitializeReceiver Method - where the socket is actually defined and opened
	virtual bool InitializeReceiver( boost::system::error_code& p_bstError ) = 0;
	
	// Stop the Receiver
	virtual bool StopReceiver( boost::system::error_code& p_bstError );

	// Stops and Closes the Receiver
    virtual bool TerminateReceiver( boost::system::error_code& p_bstError );

protected:
	// Constructor
	CAsyncConnectionlessReceiver( boost::asio::io_service& p_bstIOService, const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstRecvCallback );

	// The actual Async Recv Message which signals the receiver to start listening for a message.
    virtual void ReceiveFrom();

	// Handles the Asyncronous Recv of a message
    virtual void HandleReceiveFrom( const boost::system::error_code& p_bstError, std::size_t p_sizeBytesRecvd );

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

private:
	static const int m_c_iMaxRecvSize  = 65536;
	
	// ASIO Worker object - this keeps the ASIO IO Service busy even if no actual IO is occuring
	std::auto_ptr<boost::asio::io_service::work>    m_bstWork;
	// Our Socket
	std::auto_ptr<boost::asio::ip::udp::socket>		m_bstSocket;
	// The Sender Endpoint, filled out with information for the sender of the data
    boost::asio::ip::udp::endpoint					m_bstSenderEndpoint;    
	// Buffer to hold received messages
    char											m_strRecvBuffer[ m_c_iMaxRecvSize ];        
	// Recv Callback
	Callback_ConstRefStringConstRefStringUnsignedInt_Error		m_bstRecvCallback;

};

#endif // __ASYNC_CONNECTIONLESS_RECEIVER_H__