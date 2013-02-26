/* 
 * File:   SyncConnectionlessReceiver.h
 * Author: Josh
 *
 * Created on December 03, 2012
 *
 * This class declares and defines all common code used by Synchronous Connectionless Receiver classes
 * 
 * This class is not to be instantiated and should only be derived from.
 *
 */
#ifndef __SYNC_CONNECTIONLESS_RECEIVER_H__
#define __SYNC_CONNECTIONLESS_RECEIVER_H__

#ifndef NO_MFC

#ifndef BOOST_THREAD_DYN_LINK
#define BOOST_THREAD_DYN_LINK
#endif

#else

#ifndef BOOST_THREAD_USE_LIB
#define BOOST_THREAD_USE_LIB
#endif

#endif

#include "EthernetCommsAPI.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/optional.hpp>
#include <boost/system/system_error.hpp>

class ETHERNETCOMMS_API CSyncConnectionlessReceiver
{
public:
	// Destructor
	virtual ~CSyncConnectionlessReceiver();

	// Initialize Receiver (Pure Virtual Method would must be defined by derived class)
	// This forces classes to override the InitializeReceiver Method - where the socket is actually defined and opened
	virtual bool InitializeReceiver( boost::system::error_code& p_bstError ) = 0;

	// Receive (blocking) data from the Multicast Group and return the Message, IP, Port, and Errors
    virtual bool Receive(	const long p_lTimeoutInMs, 
							std::string& p_strMsg, 
							boost::asio::ip::address& p_bstSenderAddress, 
							unsigned short& p_usSenderPort, 
							boost::system::error_code& p_bstError );
	
	// Receive (blocking) data from the Multicast Group and return the Message, IP (as a string), Port, and Errors
    virtual bool Receive(	const long p_lTimeoutInMs, 
							std::string& p_strMsg, 
							std::string& p_strSenderAddress, 
							unsigned short& p_usSenderPort, 
							boost::system::error_code& p_bstError );
	
	// Close the Receiver
    virtual bool TerminateReceiver( boost::system::error_code& p_bstError );

protected:
	// Constructor
	CSyncConnectionlessReceiver( boost::asio::io_service& p_bstIOService );

	// Handle Receive From Async Receiver
	void HandleReceiveFrom( const boost::system::error_code& p_bstError, std::size_t p_sizeBytesRecvd );

	// Handle Async Wait Timeout
	void HandleTimeout( const boost::system::error_code& p_bstError );

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

	// ASIO Worker Object used to keep the ASIO IO Service alive even with no IO is occuring
	std::auto_ptr<boost::asio::io_service::work>    m_bstWork;
	// Our Socket
	std::auto_ptr<boost::asio::ip::udp::socket>     m_bstSocket;
	// The endpoint which defines where the message came from
    boost::asio::ip::udp::endpoint					m_bstSenderEndpoint;    
	// Buffer to hold the received message
    char											m_strRecvBuffer[ m_c_iMaxRecvSize ];
	// Boost Error object to hold any error that may occur during the read
	std::auto_ptr<boost::system::error_code>		m_bstReadError;
	// Boost Error object to hold any error that may occur when the timer times out
	std::auto_ptr<boost::system::error_code>		m_bstTimeoutError;
	// Size of the data Read in Bytes
	std::size_t										m_sizeReadSize;
	// Timer to determine when a Receive operation will time out 
	// (our way of faking Sync using Async, we do this so that we're not stuck at a receive_from() call when trying 
	// to terminate the class or the program
	std::auto_ptr<boost::asio::deadline_timer>		m_bstReadTimer;
	// Boolean, TRUE when trying to receive.
	bool											m_bReceiving;
	
private:
	// Mutex to lock values shared between the Receive method and the Read Handler
	boost::mutex									m_bstLock;
	// Pointer to the IO Service, saved in case access to the object is required
	boost::asio::io_service*						m_pBstIOService;

};

#endif // __SYNC_CONNECTIONLESS_RECEIVER_H__