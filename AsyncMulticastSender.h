/* 
 * File:   CAsyncMulticastSender.h
 * Author: Josh
 *
 * Created on September 9, 2011, 9:01 AM
 *
 * This class is controls the Asycnronous Multicast Sender tool.
 *
 * This class handles all of the boost ASIO level needs to create, control, and run 
 * an Async Multicast Sender
 *
 * Being Async, the send is actually performed in a non-blocking fashion and the resule is 
 * handled via a callback, which must be defined on construction.
 *
 * It is advised that developers use the CMulticastComms class rather than attempt to use this lower level
 * class.  If there is a need to use this low level class, perhaps the reqired functionality can be incorporated
 * into CMulticastComms, thus preparing that class for furture needs.
 *
 * Unless otherwise noted, all comments are in the CPP file with only basic comments in this Header file.
 *
 * For usage, please see ReadMe-Multicast.txt
 *
 */

#ifndef CASYNCMULTICASTSENDER_H
#define	CASYNCMULTICASTSENDER_H

#include "AsyncConnectionlessSender.h"

class ETHERNETCOMMS_API CAsyncMulticastSender : public CAsyncConnectionlessSender
{
public:
	// Constructor
    CAsyncMulticastSender( boost::asio::io_service& p_bstIOService,
                           const std::string& p_strMulticastAddress, 
                           const unsigned short p_usMulticastPort,
                           const Callback_ConstRefString_Error p_bstSendCallback );

	// Destructor
    virtual ~CAsyncMulticastSender();
    
	// Initialization Method
	virtual bool InitializeSender( boost::system::error_code& p_bstError );

private:
	// Reset the Socket
	virtual void ResetSocket( boost::asio::ip::udp::socket* p_bstSocket )
	{
		CAsyncConnectionlessSender::ResetSocket( p_bstSocket );
	}

	// Get the Socket
	virtual boost::asio::ip::udp::socket* GetSocket() const
	{
		return CAsyncConnectionlessSender::GetSocket();
	}

	// Reset the IO Service Worker
	virtual void ResetWork( boost::asio::io_service::work* p_bstWork )
	{
		CAsyncConnectionlessSender::ResetWork( p_bstWork );
	}

	// Get the IO Service Worker
	virtual boost::asio::io_service::work* GetWork() const
	{
		return CAsyncConnectionlessSender::GetWork();
	}

	// Reset the Sender Endpoint
	virtual void ResetEndpoint( boost::asio::ip::udp::endpoint* p_bstEndpoint )
	{
		CAsyncConnectionlessSender::ResetEndpoint( p_bstEndpoint );
	}

	// Get the Sender Endpoint
	virtual boost::asio::ip::udp::endpoint* GetEndpoint() const
	{
		return CAsyncConnectionlessSender::GetEndpoint();
	}

private:   
	std::string     m_strMulticastAddress;
     unsigned short	m_usMulticastPort;     
};

#endif	/* CASYNCMULTICASTSENDER_H */

