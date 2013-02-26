/* 
 * File:   CAsyncUDPReceiver.h
 * Author: Josh
 *
 * Created on September 9, 2011, 9:01 AM
 *
 * This class is controls the Asycnronous UDP Receiver tool.
 *
 * This class handles all of the boost ASIO level needs to create, control, and run 
 * an Async UDP Receiver
 *
 * Being Async, the receive is actually handled via a callback, which must be defined on construction.
 *
 * It is advised that developers use the CUDPComms class rather than attempt to use this lower level
 * class.  If there is a need to use this low level class, perhaps the reqired functionality can be incorporated
 * into CUDPComms, thus preparing that class for furture needs.
 *
 * Unless otherwise noted, all comments are in the CPP file with only basic comments in this Header file.
 *
 * For usage, please see ReadMe-UDP.txt
 *
 */


#ifndef CASYNCUDPRECEIVER_H
#define	CASYNCUDPRECEIVER_H

#include "AsyncConnectionlessReceiver.h"

class ETHERNETCOMMS_API CAsyncUDPReceiver : public CAsyncConnectionlessReceiver
{
public:        
	// Constructor
    CAsyncUDPReceiver(   boost::asio::io_service& p_bstIOService, 
                         const unsigned short p_usUDPPort,
			             const Callback_ConstRefStringConstRefStringUnsignedInt_Error p_bstRecvCallback );

	// Destructor
    virtual ~CAsyncUDPReceiver();
    
    // Initializes and Starts the Receiver
    virtual bool InitializeReceiver( boost::system::error_code& p_bstError );   

private:
	// Reset the Socket
	virtual void ResetSocket( boost::asio::ip::udp::socket* p_bstSocket )
	{
		CAsyncConnectionlessReceiver::ResetSocket( p_bstSocket );
	}

	// Get the Socket
	virtual boost::asio::ip::udp::socket* GetSocket() const
	{
		return CAsyncConnectionlessReceiver::GetSocket();
	}

	// Reset the IO Service Worker
	virtual void ResetWork( boost::asio::io_service::work* p_bstWork )
	{
		CAsyncConnectionlessReceiver::ResetWork( p_bstWork );
	}

	// Get the IO Service Worker
	virtual boost::asio::io_service::work* GetWork() const
	{
		return CAsyncConnectionlessReceiver::GetWork();
	}

private:     
    unsigned short									m_usUDPPort;
};

#endif	/* CASYNCUDPRECEIVER_H */

