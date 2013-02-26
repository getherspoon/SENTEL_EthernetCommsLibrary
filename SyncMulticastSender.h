/* 
 * File:   CSyncMulticastSender.h
 * Author: jarmentrout
 *
 * Created on August 26, 2010, 2:08 PM
 *
 * This class is controls the Sycnronous Multicast Sender tool.
 *
 * This class handles all of the boost ASIO level needs to create, control, and run 
 * an Sync Multicast Sender
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

#ifndef CSYNCMULTICASTSENDER_H
#define	CSYNCMULTICASTSENDER_H

#include "SyncConnectionlessSender.h"

class ETHERNETCOMMS_API CSyncMulticastSender : public CSyncConnectionlessSender
{
public:
	// Constructor
    CSyncMulticastSender( boost::asio::io_service& p_bstIOService,
						  const std::string& p_strMulticastAddress,
						  const unsigned short p_usMulticastPort );

	// Destructor
    virtual ~CSyncMulticastSender( );

	// Initialize the Sender
	bool InitializeSender( boost::system::error_code& p_bstError );

	// Sync Send
    bool Send( const std::string& p_strMsg, boost::system::error_code& p_bstError, const int p_iMaxSendCount = CommsUtilities::Multicast::s_iDEFAULT_MAX_SEND_COUNT, const long p_lSendIntervalInMs = CommsUtilities::Multicast::s_lDEFAULT_SEND_INTERVAL_IN_MS );

private:
	// Get the IO Service
	boost::asio::io_service& GetIOService() const	
	{ 
		return CSyncConnectionlessSender::GetIOService();
	}

	// Reset the Socket
	virtual void ResetSocket( boost::asio::ip::udp::socket* p_bstSocket )
	{
		CSyncConnectionlessSender::ResetSocket( p_bstSocket );
	}

	// Get the Socket
	virtual boost::asio::ip::udp::socket* GetSocket() const
	{
		return CSyncConnectionlessSender::GetSocket();
	}

	// Reset the Sender Endpoint
	virtual void ResetEndpoint( boost::asio::ip::udp::endpoint* p_bstEndpoint )
	{
		CSyncConnectionlessSender::ResetEndpoint( p_bstEndpoint );
	}

	// Get the Sender Endpoint
	virtual boost::asio::ip::udp::endpoint* GetEndpoint() const
	{
		return CSyncConnectionlessSender::GetEndpoint();
	}

private:
	std::string		m_strMulticastAddress;
	unsigned short m_usMulticastPort;
};

#endif	/* CSYNCMULTICASTSENDER_H */
