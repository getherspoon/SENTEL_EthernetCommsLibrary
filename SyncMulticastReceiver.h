/* 
 * File:   CSyncMulticastReceiver.h
 * Author: jarmentrout
 *
 * Created on August 26, 2010, 1:43 PM
 *
 * This class is controls the Sycnronous Multicast Receiver tool.
 *
 * This class handles all of the boost ASIO level needs to create, control, and run 
 * an Sync Multicast Receiver
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

#ifndef CSYNCMULTICASTRECEIVER_H
#define	CSYNCMULTICASTRECEIVER_H

#ifdef __WIN32__
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI2_
#include <winsock2.h>
#endif

#include "SyncConnectionlessReceiver.h"

#include <string>

class ETHERNETCOMMS_API CSyncMulticastReceiver : public CSyncConnectionlessReceiver
{
public:
	// Constructor
    CSyncMulticastReceiver( boost::asio::io_service& p_bstIOService,
							const std::string& p_strAddressListen,
							const std::string& p_strAddressMulticast,
							const unsigned short p_usMulticastPort,							
							const bool p_bLoopbackEnabled = false );        
	// Destructor
    virtual ~CSyncMulticastReceiver();
   
	// Initialize the Receiver
	bool InitializeReceiver( boost::system::error_code& p_bstError );

private:
	// Get the IO Service
	boost::asio::io_service& GetIOService() const	
	{ 
		return CSyncConnectionlessReceiver::GetIOService();
	}

	// Reset the Socket
	virtual void ResetSocket( boost::asio::ip::udp::socket* p_bstSocket )
	{
		CSyncConnectionlessReceiver::ResetSocket( p_bstSocket );
	}

	// Get the Socket
	virtual boost::asio::ip::udp::socket* GetSocket() const
	{
		return CSyncConnectionlessReceiver::GetSocket();
	}

	// Reset the IO Service Worker
	virtual void ResetWork( boost::asio::io_service::work* p_bstWork )
	{
		CSyncConnectionlessReceiver::ResetWork( p_bstWork );
	}

	// Get the IO Service Worker
	virtual boost::asio::io_service::work* GetWork() const
	{
		return CSyncConnectionlessReceiver::GetWork();
	}

private:

	std::string						m_strListenAddress;
	std::string						m_strMulticastAddress;
    unsigned short					m_usMulticastPort;	
	bool							m_bLoopbackEnabled;
};

#endif	/* CSYNCMULTICASTRECEIVER_H */

